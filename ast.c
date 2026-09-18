#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

static ASTNode *ast_new(ASTKind kind, int line) {
    ASTNode *n = calloc(1, sizeof(ASTNode));
    n->kind = kind;
    n->line = line;
    n->type = TYPE_UNKNOWN;
    return n;
}

ASTNode *ast_new_block(void) {
    ASTNode *n = ast_new(AST_BLOCK, 0);
    n->item_cap = 8;
    n->items = malloc(sizeof(ASTNode *) * n->item_cap);
    n->item_count = 0;
    return n;
}

void ast_block_add(ASTNode *block, ASTNode *stmt) {
    if (block->item_count >= block->item_cap) {
        block->item_cap *= 2;
        block->items = realloc(block->items, sizeof(ASTNode *) * block->item_cap);
    }
    block->items[block->item_count++] = stmt;
}

ASTNode *ast_new_decl(int line, DataType type, const char *name, ASTNode *init) {
    ASTNode *n = ast_new(AST_DECL, line);
    n->type = type;
    n->sval = strdup(name);
    n->a = init;
    return n;
}

ASTNode *ast_new_assign(int line, const char *name, ASTNode *expr) {
    ASTNode *n = ast_new(AST_ASSIGN, line);
    n->sval = strdup(name);
    n->a = expr;
    return n;
}

ASTNode *ast_new_if(int line, ASTNode *cond, ASTNode *then_blk, ASTNode *else_blk) {
    ASTNode *n = ast_new(AST_IF, line);
    n->a = cond;
    n->b = then_blk;
    n->c = else_blk;
    return n;
}

ASTNode *ast_new_while(int line, ASTNode *cond, ASTNode *body) {
    ASTNode *n = ast_new(AST_WHILE, line);
    n->a = cond;
    n->b = body;
    return n;
}

ASTNode *ast_new_print(int line, ASTNode *expr) {
    ASTNode *n = ast_new(AST_PRINT, line);
    n->a = expr;
    return n;
}

ASTNode *ast_new_binop(int line, const char *op, DataType type, ASTNode *left, ASTNode *right) {
    ASTNode *n = ast_new(AST_BINOP, line);
    n->sval = strdup(op);
    n->type = type;
    n->a = left;
    n->b = right;
    return n;
}

ASTNode *ast_new_unary(int line, const char *op, DataType type, ASTNode *operand) {
    ASTNode *n = ast_new(AST_UNARY, line);
    n->sval = strdup(op);
    n->type = type;
    n->a = operand;
    return n;
}

ASTNode *ast_new_int_lit(int line, int value) {
    ASTNode *n = ast_new(AST_INT_LIT, line);
    n->type = TYPE_INT;
    n->ival = value;
    return n;
}

ASTNode *ast_new_float_lit(int line, double value) {
    ASTNode *n = ast_new(AST_FLOAT_LIT, line);
    n->type = TYPE_FLOAT;
    n->fval = value;
    return n;
}

ASTNode *ast_new_string_lit(int line, const char *raw_with_quotes) {
    ASTNode *n = ast_new(AST_STRING_LIT, line);
    n->type = TYPE_STRING;
    n->sval = strdup(raw_with_quotes);
    return n;
}

ASTNode *ast_new_id(int line, DataType type, const char *name) {
    ASTNode *n = ast_new(AST_ID, line);
    n->type = type;
    n->sval = strdup(name);
    return n;
}

int ast_node_count(ASTNode *root) {
    if (root == NULL) return 0;
    int total = 1;
    if (root->kind == AST_BLOCK) {
        for (int i = 0; i < root->item_count; i++) {
            total += ast_node_count(root->items[i]);
        }
        return total;
    }
    total += ast_node_count(root->a);
    total += ast_node_count(root->b);
    total += ast_node_count(root->c);
    return total;
}

static void write_escaped(FILE *fp, const char *text) {
    if (!text) return;
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

static void write_node(FILE *fp, ASTNode *node);

static void write_block_children(FILE *fp, ASTNode *block) {
    fputs("<ul>", fp);
    for (int i = 0; i < block->item_count; i++) {
        fputs("<li>", fp);
        write_node(fp, block->items[i]);
        fputs("</li>", fp);
    }
    fputs("</ul>", fp);
}

static void write_node(FILE *fp, ASTNode *node) {
    if (node == NULL) {
        fputs("<span class=\"ast-leaf ast-none\">(none)</span>", fp);
        return;
    }

    switch (node->kind) {
        case AST_BLOCK:
            fputs("<details open><summary class=\"ast-tag ast-block\">Block</summary>", fp);
            write_block_children(fp, node);
            fputs("</details>", fp);
            break;

        case AST_DECL:
            fprintf(fp, "<details open><summary class=\"ast-tag ast-decl\">Decl <b>%s</b> : %s (ln %d)</summary>",
                    node->sval, type_to_string(node->type), node->line);
            if (node->a) {
                fputs("<ul><li>", fp);
                write_node(fp, node->a);
                fputs("</li></ul>", fp);
            } else {
                fputs("<ul><li><span class=\"ast-leaf ast-none\">(uninitialized)</span></li></ul>", fp);
            }
            fputs("</details>", fp);
            break;

        case AST_ASSIGN:
            fprintf(fp, "<details open><summary class=\"ast-tag ast-assign\">Assign <b>%s</b> = (ln %d)</summary><ul><li>",
                    node->sval, node->line);
            write_node(fp, node->a);
            fputs("</li></ul></details>", fp);
            break;

        case AST_IF:
            fprintf(fp, "<details open><summary class=\"ast-tag ast-if\">If (ln %d)</summary><ul>", node->line);
            fputs("<li><i>cond</i>: ", fp); write_node(fp, node->a); fputs("</li>", fp);
            fputs("<li><i>then</i>: ", fp); write_node(fp, node->b); fputs("</li>", fp);
            if (node->c) {
                fputs("<li><i>else</i>: ", fp); write_node(fp, node->c); fputs("</li>", fp);
            }
            fputs("</ul></details>", fp);
            break;

        case AST_WHILE:
            fprintf(fp, "<details open><summary class=\"ast-tag ast-while\">While (ln %d)</summary><ul>", node->line);
            fputs("<li><i>cond</i>: ", fp); write_node(fp, node->a); fputs("</li>", fp);
            fputs("<li><i>body</i>: ", fp); write_node(fp, node->b); fputs("</li>", fp);
            fputs("</ul></details>", fp);
            break;

        case AST_PRINT:
            fprintf(fp, "<details open><summary class=\"ast-tag ast-print\">Print (ln %d)</summary><ul><li>", node->line);
            write_node(fp, node->a);
            fputs("</li></ul></details>", fp);
            break;

        case AST_BINOP:
            fprintf(fp, "<details open><summary class=\"ast-tag ast-binop\">BinOp '%s' : %s</summary><ul>",
                    node->sval, type_to_string(node->type));
            fputs("<li>", fp); write_node(fp, node->a); fputs("</li>", fp);
            fputs("<li>", fp); write_node(fp, node->b); fputs("</li>", fp);
            fputs("</ul></details>", fp);
            break;

        case AST_UNARY:
            fprintf(fp, "<details open><summary class=\"ast-tag ast-unary\">Unary '%s' : %s</summary><ul><li>",
                    node->sval, type_to_string(node->type));
            write_node(fp, node->a);
            fputs("</li></ul></details>", fp);
            break;

        case AST_INT_LIT:
            fprintf(fp, "<span class=\"ast-leaf ast-lit\">IntLit %d</span>", node->ival);
            break;

        case AST_FLOAT_LIT:
            fprintf(fp, "<span class=\"ast-leaf ast-lit\">FloatLit %g</span>", node->fval);
            break;

        case AST_STRING_LIT:
            fputs("<span class=\"ast-leaf ast-lit\">StringLit ", fp);
            write_escaped(fp, node->sval);
            fputs("</span>", fp);
            break;

        case AST_ID:
            fprintf(fp, "<span class=\"ast-leaf ast-id\">Id <b>%s</b> : %s</span>",
                    node->sval, type_to_string(node->type));
            break;
    }
}

void ast_write_html(FILE *fp, ASTNode *root) {
    if (root == NULL) {
        fputs("<div class=\"empty\">No AST available</div>", fp);
        return;
    }
    fputs("<details open><summary class=\"ast-tag ast-program\">Program</summary>", fp);
    write_block_children(fp, root);
    fputs("</details>", fp);
}
/* ---- Graphviz DOT Export (Bubble Tree) ---- */

static void write_dot_node(FILE *fp, ASTNode *node) {
    if (!node) return;
    
    // Create a unique node ID based on its memory address
    fprintf(fp, "  node%p [label=\"", (void*)node);
    switch (node->kind) {
        case AST_BLOCK: fprintf(fp, "Block"); break;
        case AST_DECL: fprintf(fp, "Decl\\n%s : %s", node->sval, type_to_string(node->type)); break;
        case AST_ASSIGN: fprintf(fp, "Assign\\n%s =", node->sval); break;
        case AST_IF: fprintf(fp, "If (ln %d)", node->line); break;
        case AST_WHILE: fprintf(fp, "While (ln %d)", node->line); break;
        case AST_PRINT: fprintf(fp, "Print"); break;
        case AST_BINOP: fprintf(fp, "BinOp\\n'%s'", node->sval); break;
        case AST_UNARY: fprintf(fp, "Unary\\n'%s'", node->sval); break;
        case AST_INT_LIT: fprintf(fp, "IntLit\\n%d", node->ival); break;
        case AST_FLOAT_LIT: fprintf(fp, "FloatLit\\n%g", node->fval); break;
case AST_STRING_LIT: fprintf(fp, "StringLit\\n(string)"); break;        case AST_ID: fprintf(fp, "Id\\n%s", node->sval); break;
    }
    fprintf(fp, "\", shape=ellipse, style=filled, fillcolor=\"#171a23\", fontcolor=\"#e6e8ef\", color=\"#5b8cff\"];\n");

    // Recursively draw edges to children
    if (node->kind == AST_BLOCK) {
        for (int i = 0; i < node->item_count; i++) {
            if (node->items[i]) {
                fprintf(fp, "  node%p -> node%p;\n", (void*)node, (void*)node->items[i]);
                write_dot_node(fp, node->items[i]);
            }
        }
    } else {
        if (node->a) {
            fprintf(fp, "  node%p -> node%p [label=\"a\" fontcolor=\"#8b90a3\"];\n", (void*)node, (void*)node->a);
            write_dot_node(fp, node->a);
        }
        if (node->b) {
            fprintf(fp, "  node%p -> node%p [label=\"b\" fontcolor=\"#8b90a3\"];\n", (void*)node, (void*)node->b);
            write_dot_node(fp, node->b);
        }
        if (node->c) {
            fprintf(fp, "  node%p -> node%p [label=\"c\" fontcolor=\"#8b90a3\"];\n", (void*)node, (void*)node->c);
            write_dot_node(fp, node->c);
        }
    }
}

void ast_write_dot(FILE *fp, ASTNode *root) {
    fprintf(fp, "digraph AST {\n");
    fprintf(fp, "  rankdir=TB;\n");
    fprintf(fp, "  node [fontname=\"SF Pro, Helvetica, Arial\" fontsize=11];\n");
    fprintf(fp, "  edge [color=\"#8b90a3\" arrowhead=vee];\n");
    if (root) {
        write_dot_node(fp, root);
    }
    fprintf(fp, "}\n");
}