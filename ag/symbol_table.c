#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "symbol_table.h"


void symbol_table_add(struct symbol* table, char *name) {
    struct symbol *new_entry = malloc(sizeof(struct symbol));
    new_entry->name = name;

    table->next = new_entry;
}

bool symbol_table_exists(struct symbol* table, char *name) {
    struct symbol *next = table;

    while (next != NULL) {
        if (0 == strcmp(next->name, name)) {
            return true;
        } else {
            next = next->next;  
        }
    }

    return false;
}

struct symbol* symbol_table_clone(struct symbol* table) {
    

    return NULL;

}
