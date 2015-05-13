#include <stdbool.h>

struct symbol {
    char *name;
    struct symbol *next; 
};


struct symbol* symbol_table_add(struct symbol*, char*);
bool symbol_table_exists(struct symbol*, char*);
struct symbol* symbol_table_clone(struct symbol*);

