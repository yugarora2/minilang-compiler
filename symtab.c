#include <stdio.h>
#include <string.h>
#include "symtab.h"

static Symbol table[MAX_SYMBOLS];
static int count = 0;
static int next_address = 1000; /* simulated base memory address */

const char *type_to_string(DataType t) {
    switch (t) {
        case TYPE_INT: return "int";
        case TYPE_FLOAT: return "float";
        case TYPE_STRING: return "string";
        default: return "unknown";
    }
}

int size_of_type(DataType t) {
    switch (t) {
        case TYPE_INT: return 4;
        case TYPE_FLOAT: return 4;
        case TYPE_STRING: return 8; /* treated as a reference/pointer, like a real compiler would */
        default: return 4;
    }
}

int symtab_insert(const char *name, DataType type, int line) {
    if (symtab_lookup(name) != NULL) {
        return 0; /* already declared */
    }
    if (count >= MAX_SYMBOLS) {
        fprintf(stderr, "Symbol table full, cannot insert '%s'\n", name);
        return 0;
    }
    strncpy(table[count].name, name, MAX_NAME_LEN - 1);
    table[count].name[MAX_NAME_LEN - 1] = '\0';
    table[count].type = type;
    table[count].declared_line = line;
    table[count].initialized = 0;
    strncpy(table[count].scope, "global", sizeof(table[count].scope) - 1);
    table[count].scope[sizeof(table[count].scope) - 1] = '\0';
    table[count].address = next_address;
    next_address += size_of_type(type);
    count++;
    return 1;
}

Symbol *symtab_lookup(const char *name) {
    for (int i = 0; i < count; i++) {
        if (strcmp(table[i].name, name) == 0) {
            return &table[i];
        }
    }
    return NULL;
}

void symtab_mark_initialized(const char *name) {
    Symbol *s = symtab_lookup(name);
    if (s != NULL) {
        s->initialized = 1;
    }
}

void symtab_dump(void) {
    printf("\n===== SYMBOL TABLE =====\n");
    printf("%-15s %-10s %-8s %-10s %-12s %-12s\n",
           "Identifier", "Data Type", "Scope", "Address", "Declared@Ln", "Initialized");
    printf("---------------------------------------------------------------------\n");
    for (int i = 0; i < count; i++) {
        printf("%-15s %-10s %-8s %-10d %-12d %-12s\n",
               table[i].name,
               type_to_string(table[i].type),
               table[i].scope,
               table[i].address,
               table[i].declared_line,
               table[i].initialized ? "yes" : "no");
    }
    printf("=======================================================================\n");
    printf("Total symbols: %d\n", count);
}

int symtab_count(void) {
    return count;
}

Symbol *symtab_get(int index) {
    if (index < 0 || index >= count) return NULL;
    return &table[index];
}
