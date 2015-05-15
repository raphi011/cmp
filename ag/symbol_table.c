#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

#include "symbol_table.h"


struct symbol *symbol_table_add(struct symbol* table, char *name) {
    if (symbol_table_exists(table,name)) {
        exit(EXIT_ERROR); 
    } 

    struct symbol * new = symbol_table_clone(table);
    
    struct symbol *new_entry = malloc(sizeof(struct symbol));
    new_entry->name = name;
    new_entry->next = new;

    return new_entry;
}

struct symbol *symbol_table_merge(struct symbol* table1, struct symbol* table2)  {
    assert(table1 != NULL || table2 != NULL);

    struct symbol * table1_clone = symbol_table_clone(table1);
    struct symbol * table2_clone = symbol_table_clone(table2);

    struct symbol * append = table1_clone;

    if (table1_clone == NULL) {
        return table2_clone;   
    } else if (table2_clone == NULL) {
        return table1_clone;
    } else { 
        while (append->next != NULL) {
            append = append->next;
        }
        append->next = table2_clone;
        return table1_clone;
    }
}

void symbol_table_print(struct symbol* table) {
    struct symbol *next = table;
    printf("printing symbol table: ");

    while (next != NULL) {
        printf("%s ", next->name);
        next = next->next;  
    }

    printf("\n");
}

bool symbol_table_exists(struct symbol* table, char *name) {
    struct symbol *next = table;

    //symbol_table_print(table);
    

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
    if (table == NULL) {
        return NULL;
    }

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
