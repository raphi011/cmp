#include <stdbool.h>

struct symbol {
    char *name;
    struct symbol *next; 
};

#define EXIT_ERROR (3)

void symbol_table_print(struct symbol*);
struct symbol* symbol_table_add(struct symbol*, char*);
struct symbol* symbol_table_merge(struct symbol*, struct symbol*);
bool symbol_table_exists(struct symbol*, char*);
struct symbol* symbol_table_clone(struct symbol*);
