#ifndef SYMTAB_H
#define SYMTAB_H

#define MAX_SYMBOLS 200
#define MAX_NAME_LEN 64

typedef enum {
    TYPE_INT,
    TYPE_FLOAT,
    TYPE_STRING,
    TYPE_UNKNOWN
} DataType;

typedef struct {
    char name[MAX_NAME_LEN];
    DataType type;
    int declared_line;
    int initialized;   /* 1 if it has been assigned a value at least once */
    char scope[16];    /* "global" for now -- MiniLang has no functions yet */
    int address;       /* simulated memory address, base 1000, incremented by type size */
} Symbol;

/* Returns the size in bytes used to simulate memory addresses (int=4, float=4, string=8). */
int size_of_type(DataType t);

/* Returns 1 on success, 0 if the symbol already exists (redeclaration). */
int symtab_insert(const char *name, DataType type, int line);

/* Returns pointer to symbol, or NULL if not found. */
Symbol *symtab_lookup(const char *name);

/* Marks a symbol as initialized (used after assignment). */
void symtab_mark_initialized(const char *name);

/* Prints the full symbol table to stdout. */
void symtab_dump(void);

/* For iterating the table from other modules (e.g. report generation). */
int symtab_count(void);
Symbol *symtab_get(int index);

const char *type_to_string(DataType t);

#endif
