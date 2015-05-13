
struct symbol {
    char *name;
    struct symbol *next; 
};


void symbol_table_add(struct symbol*, char*);
bool symbol_table_exists(struct symbol*, char*);

