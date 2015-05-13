#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "symbol_table.h"


struct symbol* symbol_table_add(struct symbol* table, char *name) {
    struct symbol *new_entry = malloc(sizeof(struct symbol));
    new_entry->name = name;
    new_entry->next = table;

    return new_entry;
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
    struct symbol *current; 
    struct symbol *clone = current = malloc(sizeof(struct symbol)); 
    clone->name = strdup(table->name); 
     
    while ((table = table->next) != NULL) {
        current->next = malloc(sizeof(struct symbol)); 
        current = current->next;
        current->name = strdup(table->name);
    }

    return clone;
}
