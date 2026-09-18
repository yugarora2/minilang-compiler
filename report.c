#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "report.h"
#include "symtab.h"
#include "ast.h"
#include "tac.h"

typedef struct {
    int line;
    char name[16];
    char text[128];
} TokenRecord;

typedef struct {
    int line;
    char category[16];
    char message[256];
} ErrorRecord;

static TokenRecord *tokens = NULL;
static int token_count = 0;
static int token_cap = 0;

static ErrorRecord *errors = NULL;
static int error_rec_count = 0;
static int error_cap = 0;

void report_add_token(int line, const char *name, const char *text) {
    if (token_count >= token_cap) {
        token_cap = token_cap == 0 ? 256 : token_cap * 2;
        tokens = realloc(tokens, sizeof(TokenRecord) * token_cap);
    }
    tokens[token_count].line = line;
    strncpy(tokens[token_count].name, name, sizeof(tokens[token_count].name) - 1);
    tokens[token_count].name[sizeof(tokens[token_count].name) - 1] = '\0';
    strncpy(tokens[token_count].text, text, sizeof(tokens[token_count].text) - 1);
    tokens[token_count].text[sizeof(tokens[token_count].text) - 1] = '\0';
    token_count++;
}

void report_add_error(int line, const char *category, const char *message) {
    if (error_rec_count >= error_cap) {
        error_cap = error_cap == 0 ? 64 : error_cap * 2;
        errors = realloc(errors, sizeof(ErrorRecord) * error_cap);
    }
    errors[error_rec_count].line = line;
    strncpy(errors[error_rec_count].category, category, sizeof(errors[error_rec_count].category) - 1);
    errors[error_rec_count].category[sizeof(errors[error_rec_count].category) - 1] = '\0';
    strncpy(errors[error_rec_count].message, message, sizeof(errors[error_rec_count].message) - 1);
    errors[error_rec_count].message[sizeof(errors[error_rec_count].message) - 1] = '\0';
    error_rec_count++;
}

/* Writes 'text' to fp with HTML special characters escaped. */
static void write_escaped(FILE *fp, const char *text) {
    for (const char *p = text; *p; p++) {
        switch (*p) {
            case '<': fputs("&lt;", fp); break;
            case '>': fputs("&gt;", fp); break;
            case '&': fputs("&amp;", fp); break;
            case '"': fputs("&quot;", fp); break;
            default: fputc(*p, fp);
        }
    }
}

/* Returns a short CSS class for coloring a token by its category name. */
static const char *token_css_class(const char *name) {
    if (strcmp(name, "KEYWORD") == 0) return "kw";
    if (strcmp(name, "ID") == 0) return "id";
    if (strcmp(name, "INT_LIT") == 0 || strcmp(name, "FLOAT_LIT") == 0 || strcmp(name, "STRING_LIT") == 0) return "lit";
    if (strcmp(name, "RELOP") == 0) return "relop";
    if (strcmp(name, "ASSIGN") == 0) return "assign";
    if (strcmp(name, "PUNCT") == 0) return "punct";
    return "op";
}

static void write_css(FILE *out) {
    fputs(
        ":root{--bg:#0f1117;--panel:#171a23;--border:#272b38;--text:#e6e8ef;"
        "--muted:#8b90a3;--accent:#5b8cff;--ok:#3ddc84;--err:#ff5d6c;--warn:#ffb84d;}"
        "*{box-sizing:border-box;}"
        "body{margin:0;background:var(--bg);color:var(--text);"
        "font-family:-apple-system,Segoe UI,Roboto,Helvetica,Arial,sans-serif;padding:32px;}"
        "h1{font-size:22px;margin:0 0 4px;}"
        ".sub{color:var(--muted);font-size:13px;margin-bottom:24px;}"
        ".banner{display:inline-block;padding:8px 16px;border-radius:8px;font-weight:600;"
        "font-size:14px;margin-bottom:24px;}"
        ".banner.ok{background:rgba(61,220,132,0.15);color:var(--ok);border:1px solid var(--ok);}"
        ".banner.err{background:rgba(255,93,108,0.15);color:var(--err);border:1px solid var(--err);}"
        ".grid{display:grid;grid-template-columns:1fr 1fr;gap:20px;align-items:start;}"
        ".panel{background:var(--panel);border:1px solid var(--border);border-radius:10px;"
        "padding:16px;margin-bottom:20px;}"
        ".panel h2{font-size:14px;margin:0 0 12px;color:var(--accent);"
        "text-transform:uppercase;letter-spacing:0.06em;}"
        "pre{margin:0;font-family:'SF Mono',Consolas,Monaco,monospace;font-size:13px;"
        "line-height:1.6;overflow-x:auto;max-height:520px;overflow-y:auto;white-space:pre;}"
        ".lineno{color:var(--muted);display:inline-block;width:32px;text-align:right;"
        "margin-right:12px;user-select:none;}"
        "table{width:100%;border-collapse:collapse;font-size:13px;}"
        "th{text-align:left;color:var(--muted);font-weight:600;padding:6px 8px;"
        "border-bottom:1px solid var(--border);position:sticky;top:0;background:var(--panel);}"
        "td{padding:5px 8px;border-bottom:1px solid rgba(255,255,255,0.04);"
        "font-family:'SF Mono',Consolas,Monaco,monospace;}"
        ".scroll{max-height:420px;overflow-y:auto;}"
        ".tag{display:inline-block;padding:2px 8px;border-radius:5px;font-size:11px;font-weight:600;}"
        ".tag.kw{background:rgba(91,140,255,0.15);color:var(--accent);}"
        ".tag.id{background:rgba(255,184,77,0.15);color:var(--warn);}"
        ".tag.lit{background:rgba(61,220,132,0.15);color:var(--ok);}"
        ".tag.op,.tag.punct,.tag.assign,.tag.relop{background:rgba(255,255,255,0.08);color:var(--text);}"
        ".err-row td{color:var(--err);}"
        ".empty{color:var(--muted);font-style:italic;padding:8px;}"
        ".full{grid-column:1/-1;}"
        "footer{color:var(--muted);font-size:12px;margin-top:8px;}"
        ".ast-tag{font-weight:600; cursor:pointer; color:var(--accent);}"
        ".ast-leaf{color:var(--ok);}"
        ".ast-none{color:var(--muted);font-style:italic;}"
        ".tac-op{color:var(--warn);font-weight:bold;}"
        "details{margin-left:8px;}"
        "ul{margin:4px 0 8px 16px;padding:0;list-style-type:none;border-left:1px dashed rgba(255,255,255,0.1);}"
        "li{margin-bottom:4px;padding-left:8px;}"
        , out);
}

void report_write_html(const char *out_path, const char *source_path, int error_count, struct ASTNode *ast_root) {
    FILE *out = fopen(out_path, "w");
    if (!out) {
        fprintf(stderr, "Warning: could not write HTML report to '%s'\n", out_path);
        return;
    }

    fputs("<!DOCTYPE html>\n<html lang=\"en\">\n<head>\n<meta charset=\"UTF-8\">\n", out);
    fputs("<title>MiniLang Compiler Report</title>\n<style>\n", out);
    write_css(out);
    fputs("</style>\n</head>\n<body>\n", out);

    fputs("<h1>MiniLang Compiler Report</h1>\n", out);
    fprintf(out, "<div class=\"sub\">Source: %s &middot; Phase 2: Core Implementation (Lexical &rarr; Syntax &rarr; AST &rarr; TAC)</div>\n", source_path);

    if (error_count == 0) {
        fputs("<div class=\"banner ok\">&#10003; Compilation finished with 0 errors</div>\n", out);
    } else {
        fprintf(out, "<div class=\"banner err\">&#10007; Compilation finished with %d error(s)</div>\n", error_count);
    }

    fputs("<div class=\"grid\">\n", out);

    /* --- Source code panel --- */
    fputs("<div class=\"panel\"><h2>Source Code</h2><pre>", out);
    FILE *src = fopen(source_path, "r");
    if (src) {
        char line[1024];
        int lineno = 1;
        while (fgets(line, sizeof(line), src)) {
            size_t len = strlen(line);
            fprintf(out, "<span class=\"lineno\">%d</span>", lineno);
            write_escaped(out, line);
            if (len == 0 || line[len - 1] != '\n') fputc('\n', out);
            lineno++;
        }
        fclose(src);
    } else {
        fputs("(source unavailable)", out);
    }
    fputs("</pre></div>\n", out);

    /* --- Symbol table panel --- */
    fputs("<div class=\"panel\"><h2>Symbol Table</h2><div class=\"scroll\"><table>"
          "<tr><th>Identifier</th><th>Data Type</th><th>Scope</th><th>Address</th>"
          "<th>Declared@Ln</th><th>Initialized</th></tr>\n", out);
    int sc = symtab_count();
    if (sc == 0) {
        fputs("<tr><td colspan=\"6\" class=\"empty\">No symbols declared</td></tr>\n", out);
    } else {
        for (int i = 0; i < sc; i++) {
            Symbol *s = symtab_get(i);
            fprintf(out, "<tr><td>%s</td><td>%s</td><td>%s</td><td>%d</td><td>%d</td><td>%s</td></tr>\n",
                    s->name, type_to_string(s->type), s->scope, s->address,
                    s->declared_line, s->initialized ? "yes" : "no");
        }
    }
    fputs("</table></div></div>\n", out);

    /* --- AST Panel --- */
    fputs("<div class=\"panel\"><h2>Abstract Syntax Tree</h2><div class=\"scroll\">\n", out);
    ast_write_html(out, ast_root);
    fputs("</div></div>\n", out);

    /* --- IR / TAC Panel --- */
    fputs("<div class=\"panel\"><h2>Intermediate Representation (TAC)</h2><div class=\"scroll\"><table>"
          "<tr><th>Index</th><th>Op</th><th>Arg1</th><th>Arg2</th><th>Result</th></tr>\n", out);
    if (tac_count() == 0) {
        fputs("<tr><td colspan=\"5\" class=\"empty\">No IR generated (check for parse errors)</td></tr>\n", out);
    } else {
        tac_write_html_rows(out);
    }
    fputs("</table></div></div>\n", out);

    /* --- Token stream panel --- */
    fputs("<div class=\"panel\"><h2>Token Stream</h2><div class=\"scroll\"><table>"
          "<tr><th>Line</th><th>Type</th><th>Lexeme</th></tr>\n", out);
    if (token_count == 0) {
        fputs("<tr><td colspan=\"3\" class=\"empty\">No tokens recorded</td></tr>\n", out);
    } else {
        for (int i = 0; i < token_count; i++) {
            fprintf(out, "<tr><td>%d</td><td><span class=\"tag %s\">%s</span></td><td>",
                    tokens[i].line, token_css_class(tokens[i].name), tokens[i].name);
            write_escaped(out, tokens[i].text);
            fputs("</td></tr>\n", out);
        }
    }
    fputs("</table></div></div>\n", out);

    /* --- Errors panel (full width) --- */
    fputs("<div class=\"panel full\"><h2>Errors</h2><div class=\"scroll\"><table>"
          "<tr><th>Line</th><th>Category</th><th>Message</th></tr>\n", out);
    if (error_rec_count == 0) {
        fputs("<tr><td colspan=\"3\" class=\"empty\">No errors &mdash; compilation successful</td></tr>\n", out);
    } else {
        for (int i = 0; i < error_rec_count; i++) {
            fprintf(out, "<tr class=\"err-row\"><td>%d</td><td>%s</td><td>",
                    errors[i].line, errors[i].category);
            write_escaped(out, errors[i].message);
            fputs("</td></tr>\n", out);
        }
    }
    fputs("</table></div></div>\n", out);

    fputs("</div>\n", out); /* close .grid */
    fputs("<footer>Generated by the MiniLang compiler &mdash; Phase 2 (Lexical, Syntax, AST, TAC)</footer>\n", out);
    fputs("</body>\n</html>\n", out);

    fclose(out);
}