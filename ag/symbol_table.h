#include <stdbool.h>

struct symbol {
    char *name;
    struct symbol *next; 
};


void symbol_table_add_after(struct symbol*, char*);
struct symbol* symbol_table_add_before(struct symbol*, char*);
bool symbol_table_exists(struct symbol*, char*);
struct symbol* symbol_table_clone(struct symbol*);

