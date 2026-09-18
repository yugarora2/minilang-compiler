#ifndef REPORT_H
#define REPORT_H
#include "ast.h"
#include "tac.h"

struct ASTNode; // Forward declaration

void report_add_token(int line, const char *name, const char *text);
void report_add_error(int line, const char *category, const char *message);

/* Writes a self-contained, styled HTML report to out_path. */
void report_write_html(const char *out_path, const char *source_path, int error_count, struct ASTNode *ast_root);

#endif
