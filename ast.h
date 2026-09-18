#ifndef AST_H
#define AST_H

#include <stdio.h>
#include "symtab.h"

typedef enum {
    AST_BLOCK,
    AST_DECL,
    AST_ASSIGN,
    AST_IF,
    AST_WHILE,
    AST_PRINT,
    AST_BINOP,
    AST_UNARY,
    AST_INT_LIT,
    AST_FLOAT_LIT,
    AST_STRING_LIT,
    AST_ID
} ASTKind;

typedef struct ASTNode {
    ASTKind kind;
    int line;
    DataType type;
    char *sval;
    int ival;
    double fval;

    struct ASTNode *a;
    struct ASTNode *b;
    struct ASTNode *c;

    struct ASTNode **items;
    int item_count;
    int item_cap;
} ASTNode;

ASTNode *ast_new_block(void);
void ast_block_add(ASTNode *block, ASTNode *stmt);
ASTNode *ast_new_decl(int line, DataType type, const char *name, ASTNode *init);
ASTNode *ast_new_assign(int line, const char *name, ASTNode *expr);
ASTNode *ast_new_if(int line, ASTNode *cond, ASTNode *then_blk, ASTNode *else_blk);
ASTNode *ast_new_while(int line, ASTNode *cond, ASTNode *body);
ASTNode *ast_new_print(int line, ASTNode *expr);
ASTNode *ast_new_binop(int line, const char *op, DataType type, ASTNode *left, ASTNode *right);
ASTNode *ast_new_unary(int line, const char *op, DataType type, ASTNode *operand);
ASTNode *ast_new_int_lit(int line, int value);
ASTNode *ast_new_float_lit(int line, double value);
ASTNode *ast_new_string_lit(int line, const char *raw_with_quotes);
ASTNode *ast_new_id(int line, DataType type, const char *name);

int ast_node_count(ASTNode *root);
void ast_write_html(FILE *fp, ASTNode *root);
void ast_write_dot(FILE *fp, ASTNode *root);

#endif