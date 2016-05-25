#ifndef SYMBOL_H 
#define SYMBOL_H

#include <stdbool.h>

enum symbol_type {
    label,
    variable
};

struct symbol {
    char *name;
    char *reg;
    enum symbol_type type;
    struct symbol *next; 
};

#define EXIT_ERROR (3)

void symbol_table_print(struct symbol*);
struct symbol *symbol_table_get(struct symbol*, char*);
struct symbol* symbol_table_add(struct symbol*, char*, enum symbol_type);
struct symbol* symbol_table_merge(struct symbol*, struct symbol*);
bool symbol_table_exists(struct symbol*, char*);
bool symbol_table_exists_type(struct symbol*, char*, enum symbol_type);
struct symbol* symbol_table_clone(struct symbol*);

#endif
