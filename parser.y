%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "symtab.h"
#include "report.h"
#include "ast.h"
#include "tac.h"

extern int yylineno;
extern int verbose_tokens;
extern FILE *yyin;
int yylex(void);
void yyerror(const char *s);

int error_count = 0;
ASTNode *ast_root = NULL;

/* ---- Semantic analysis helpers ---- */
static void sem_error(int line, const char *category, const char *fmt, ...) {
    char msg[256];
    va_list args;
    va_start(args, fmt);
    vsnprintf(msg, sizeof(msg), fmt, args);
    va_end(args);
    fprintf(stderr, "%s error at line %d: %s\n", category, line, msg);
    report_add_error(line, category, msg);
    error_count++;
}

static int type_compatible(DataType target, DataType source) {
    if (target == source) return 1;
    if (target == TYPE_FLOAT && source == TYPE_INT) return 1; 
    return 0;
}

static DataType check_arith(DataType left, DataType right, int line, const char *op) {
    if (left == TYPE_STRING || right == TYPE_STRING) {
        sem_error(line, "Semantic", "operator '%s' not valid on type 'string'.", op);
        return TYPE_UNKNOWN;
    }
    if (left == TYPE_UNKNOWN || right == TYPE_UNKNOWN) return TYPE_UNKNOWN;
    if (left == TYPE_FLOAT || right == TYPE_FLOAT) return TYPE_FLOAT;
    return TYPE_INT;
}

static DataType check_relational(DataType left, DataType right, int line, const char *op) {
    if (left == TYPE_UNKNOWN || right == TYPE_UNKNOWN) return TYPE_INT;
    int left_numeric = (left == TYPE_INT || left == TYPE_FLOAT);
    int right_numeric = (right == TYPE_INT || right == TYPE_FLOAT);
    if (left_numeric && right_numeric) return TYPE_INT;
    if (left == TYPE_STRING && right == TYPE_STRING) return TYPE_INT;
    sem_error(line, "Semantic", "mismatched types on either side of '%s'.", op);
    return TYPE_INT;
}
%}

%union {
    int ival;
    double fval;
    char *sval;
    struct ASTNode *node;
}

%token <sval> ID STRING_LIT
%token <ival> INT_LIT
%token <fval> FLOAT_LIT
%token INT_KW FLOAT_KW STRING_KW
%token IF ELSE WHILE PRINT
%token EQ NEQ LE GE LT GT

%left '+' '-'
%left '*' '/'
%left LT GT LE GE EQ NEQ
%right UMINUS

%type <ival> type 
%type <node> expr stmt stmt_list decl_stmt assign_stmt if_stmt while_stmt print_stmt block program

%%

program:
    stmt_list { 
        ast_root = $1; 
        if (error_count == 0) {
            tac_generate(ast_root); 
        }
        $$ = ast_root;
        printf("\n[Parser] Program parsed & AST constructed successfully.\n"); 
    }
    ;

stmt_list:
      /* empty */ { $$ = ast_new_block(); }
    | stmt_list stmt { ast_block_add($1, $2); $$ = $1; }
    ;

stmt:
      decl_stmt     { $$ = $1; }
    | assign_stmt   { $$ = $1; }
    | if_stmt       { $$ = $1; }
    | while_stmt    { $$ = $1; }
    | print_stmt    { $$ = $1; }
    | block         { $$ = $1; }
    ;

type:
      INT_KW    { $$ = TYPE_INT; }
    | FLOAT_KW  { $$ = TYPE_FLOAT; }
    | STRING_KW { $$ = TYPE_STRING; }
    ;

decl_stmt:
      type ID ';' {
          if (!symtab_insert($2, $1, yylineno)) {
              sem_error(yylineno, "Semantic", "'%s' already declared.", $2);
          }
          $$ = ast_new_decl(yylineno, $1, $2, NULL);
          free($2);
      }
    | type ID '=' expr ';' {
          if (!symtab_insert($2, $1, yylineno)) {
              sem_error(yylineno, "Semantic", "'%s' already declared.", $2);
          } else {
              if ($4->type != TYPE_UNKNOWN && !type_compatible((DataType)$1, $4->type)) {
                  sem_error(yylineno, "Semantic", "cannot initialize '%s' of type '%s' with value of type '%s'.",
                            $2, type_to_string((DataType)$1), type_to_string($4->type));
              }
              symtab_mark_initialized($2);
          }
          $$ = ast_new_decl(yylineno, $1, $2, $4);
          free($2);
      }
    ;

assign_stmt:
      ID '=' expr ';' {
          Symbol *sym = symtab_lookup($1);
          if (sym == NULL) {
              sem_error(yylineno, "Semantic", "'%s' used without declaration.", $1);
          } else {
              if ($3->type != TYPE_UNKNOWN && !type_compatible(sym->type, $3->type)) {
                  sem_error(yylineno, "Semantic", "cannot assign type '%s' to '%s' of type '%s'.",
                            type_to_string($3->type), $1, type_to_string(sym->type));
              }
              symtab_mark_initialized($1);
          }
          $$ = ast_new_assign(yylineno, $1, $3);
          free($1);
      }
    ;

if_stmt:
      IF '(' expr ')' block { $$ = ast_new_if(yylineno, $3, $5, NULL); }
    | IF '(' expr ')' block ELSE block { $$ = ast_new_if(yylineno, $3, $5, $7); }
    ;

while_stmt:
      WHILE '(' expr ')' block { $$ = ast_new_while(yylineno, $3, $5); }
    ;

print_stmt:
      PRINT '(' expr ')' ';' { $$ = ast_new_print(yylineno, $3); }
    ;

block:
      '{' stmt_list '}' { $$ = $2; }
    ;

expr:
      expr '+' expr { $$ = ast_new_binop(yylineno, "+", check_arith($1->type, $3->type, yylineno, "+"), $1, $3); }
    | expr '-' expr { $$ = ast_new_binop(yylineno, "-", check_arith($1->type, $3->type, yylineno, "-"), $1, $3); }
    | expr '*' expr { $$ = ast_new_binop(yylineno, "*", check_arith($1->type, $3->type, yylineno, "*"), $1, $3); }
    | expr '/' expr { $$ = ast_new_binop(yylineno, "/", check_arith($1->type, $3->type, yylineno, "/"), $1, $3); }
    | expr LT expr  { $$ = ast_new_binop(yylineno, "<", check_relational($1->type, $3->type, yylineno, "<"), $1, $3); }
    | expr GT expr  { $$ = ast_new_binop(yylineno, ">", check_relational($1->type, $3->type, yylineno, ">"), $1, $3); }
    | expr LE expr  { $$ = ast_new_binop(yylineno, "<=", check_relational($1->type, $3->type, yylineno, "<="), $1, $3); }
    | expr GE expr  { $$ = ast_new_binop(yylineno, ">=", check_relational($1->type, $3->type, yylineno, ">="), $1, $3); }
    | expr EQ expr  { $$ = ast_new_binop(yylineno, "==", check_relational($1->type, $3->type, yylineno, "=="), $1, $3); }
    | expr NEQ expr { $$ = ast_new_binop(yylineno, "!=", check_relational($1->type, $3->type, yylineno, "!="), $1, $3); }
    | '(' expr ')'  { $$ = $2; }
    | '-' expr %prec UMINUS {
          if ($2->type == TYPE_STRING) {
              sem_error(yylineno, "Semantic", "unary '-' not valid on type 'string'.");
              $$ = ast_new_unary(yylineno, "uminus", TYPE_UNKNOWN, $2);
          } else {
              $$ = ast_new_unary(yylineno, "uminus", $2->type, $2);
          }
      }
    | INT_LIT    { $$ = ast_new_int_lit(yylineno, $1); }
    | FLOAT_LIT  { $$ = ast_new_float_lit(yylineno, $1); }
    | STRING_LIT { $$ = ast_new_string_lit(yylineno, $1); }
    | ID {
          Symbol *sym = symtab_lookup($1);
          DataType t = TYPE_UNKNOWN;
          if (sym == NULL) {
              sem_error(yylineno, "Semantic", "'%s' used without declaration.", $1);
          } else {
              t = sym->type;
          }
          $$ = ast_new_id(yylineno, t, $1);
          free($1);
      }
    ;

%%

void yyerror(const char *s) {
    sem_error(yylineno, "Syntax", "%s", s);
}

int main(int argc, char **argv) {
    const char *filename = NULL;
    const char *html_out = "report.html";
    int want_html = 1;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-v") == 0) {
            verbose_tokens = 1;
        } else if (strcmp(argv[i], "--no-html") == 0) {
            want_html = 0;
        } else if (strcmp(argv[i], "-o") == 0 && i + 1 < argc) {
            html_out = argv[++i];
        } else {
            filename = argv[i];
        }
    }

    if (filename == NULL) {
        fprintf(stderr, "Usage: %s [-v] [--no-html] [-o report.html] <source_file.ml>\n", argv[0]);
        return 1;
    }

    yyin = fopen(filename, "r");
    if (!yyin) return 1;

    yyparse();
    fclose(yyin);

    if (want_html) {
        report_write_html(html_out, filename, error_count, ast_root);
        printf("\nHTML report written to: %s\n", html_out);
    }
    if (want_html) {
        report_write_html(html_out, filename, error_count, ast_root);
        printf("\nHTML report written to: %s\n", html_out);
        
        // Export traditional bubble-tree graph in DOT format
        FILE *dot_fp = fopen("ast.dot", "w");
        if (dot_fp) {
            ast_write_dot(dot_fp, ast_root);
            fclose(dot_fp);
            printf("DOT graph written to: ast.dot\n");
            
            // Try to automatically compile to PNG if Graphviz 'dot' tool is available
            int ret = system("dot -Tpng ast.dot -o ast.png 2>/dev/null");
            if (ret == 0) {
                printf("Bubble tree image generated: ast.png\n");
            }
        }
    }

    return error_count == 0 ? 0 : 1;
}