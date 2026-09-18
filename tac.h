#ifndef TAC_H
#define TAC_H

#include <stdio.h>
#include "ast.h"

typedef struct {
    char op[8];
    char arg1[64];
    char arg2[64];
    char result[64];
} Quad;

void tac_generate(ASTNode *program);
int tac_count(void);
Quad *tac_get(int index);
int tac_temp_count(void);
int tac_label_count(void);
void tac_write_html_rows(FILE *fp);

#endif