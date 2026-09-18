#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tac.h"

#define MAX_QUADS 4096

static Quad quads[MAX_QUADS];
static int quad_count = 0;
static int temp_counter = 0;
static int label_counter = 0;

static void emit(const char *op, const char *a1, const char *a2, const char *result) {
    if (quad_count >= MAX_QUADS) return;
    Quad *q = &quads[quad_count++];
    snprintf(q->op, sizeof q->op, "%s", op ? op : "");
    snprintf(q->arg1, sizeof q->arg1, "%s", a1 ? a1 : "");
    snprintf(q->arg2, sizeof q->arg2, "%s", a2 ? a2 : "");
    snprintf(q->result, sizeof q->result, "%s", result ? result : "");
}

static char *new_temp(void) {
    char buf[16];
    snprintf(buf, sizeof buf, "t%d", ++temp_counter);
    return strdup(buf);
}

static char *new_label(void) {
    char buf[16];
    snprintf(buf, sizeof buf, "L%d", ++label_counter);
    return strdup(buf);
}

static void tac_gen_stmt(ASTNode *n);

static char *tac_gen_expr(ASTNode *n) {
    char buf[64];
    if (n == NULL) return strdup("");

    switch (n->kind) {
        case AST_INT_LIT:
            snprintf(buf, sizeof buf, "%d", n->ival);
            return strdup(buf);
        case AST_FLOAT_LIT:
            snprintf(buf, sizeof buf, "%g", n->fval);
            return strdup(buf);
        case AST_STRING_LIT:
            return strdup(n->sval);
        case AST_ID:
            return strdup(n->sval);
        case AST_UNARY: {
            char *operand = tac_gen_expr(n->a);
            char *t = new_temp();
            emit("uminus", operand, "", t);
            free(operand);
            return t;
        }
        case AST_BINOP: {
            char *l = tac_gen_expr(n->a);
            char *r = tac_gen_expr(n->b);
            char *t = new_temp();
            emit(n->sval, l, r, t);
            free(l);
            free(r);
            return t;
        }
        default:
            return strdup("?");
    }
}

static void tac_gen_stmt(ASTNode *n) {
    if (n == NULL) return;

    switch (n->kind) {
        case AST_BLOCK:
            for (int i = 0; i < n->item_count; i++) {
                tac_gen_stmt(n->items[i]);
            }
            break;
        case AST_DECL:
            if (n->a != NULL) {
                char *v = tac_gen_expr(n->a);
                emit("=", v, "", n->sval);
                free(v);
            }
            break;
        case AST_ASSIGN: {
            char *v = tac_gen_expr(n->a);
            emit("=", v, "", n->sval);
            free(v);
            break;
        }
        case AST_PRINT: {
            char *v = tac_gen_expr(n->a);
            emit("print", v, "", "");
            free(v);
            break;
        }
        case AST_IF: {
            char *cond = tac_gen_expr(n->a);
            char *l_false = new_label();
            emit("ifFalse", cond, "", l_false);
            free(cond);

            tac_gen_stmt(n->b); /* then */

            if (n->c != NULL) {
                char *l_end = new_label();
                emit("goto", "", "", l_end);
                emit("label", "", "", l_false);
                tac_gen_stmt(n->c); /* else */
                emit("label", "", "", l_end);
                free(l_end);
            } else {
                emit("label", "", "", l_false);
            }
            free(l_false);
            break;
        }
        case AST_WHILE: {
            char *l_start = new_label();
            char *l_end = new_label();
            emit("label", "", "", l_start);

            char *cond = tac_gen_expr(n->a);
            emit("ifFalse", cond, "", l_end);
            free(cond);

            tac_gen_stmt(n->b); /* body */
            emit("goto", "", "", l_start);
            emit("label", "", "", l_end);

            free(l_start);
            free(l_end);
            break;
        }
        default:
            break; 
    }
}

void tac_generate(ASTNode *program) {
    quad_count = 0;
    temp_counter = 0;
    label_counter = 0;
    tac_gen_stmt(program);
}

int tac_count(void) { return quad_count; }
Quad *tac_get(int index) {
    if (index < 0 || index >= quad_count) return NULL;
    return &quads[index];
}
int tac_temp_count(void) { return temp_counter; }
int tac_label_count(void) { return label_counter; }

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

void tac_write_html_rows(FILE *fp) {
    for (int i = 0; i < quad_count; i++) {
        Quad *q = &quads[i];
        fprintf(fp, "<tr><td>%d</td><td><span class=\"tac-op\">", i);
        write_escaped(fp, q->op);
        fputs("</span></td><td>", fp);
        write_escaped(fp, q->arg1);
        fputs("</td><td>", fp);
        write_escaped(fp, q->arg2);
        fputs("</td><td>", fp);
        write_escaped(fp, q->result);
        fputs("</td></tr>\n", fp);
    }
}