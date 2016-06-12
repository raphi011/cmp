#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

#include "symbol_table.h"
#include "regs.h"
#include "asm.h"

struct symbol *symbol_table_add(struct symbol* table, char *name, enum symbol_type type) {
    if (symbol_table_exists(table,name)) { 
        exit(EXIT_ERROR); 
    } 

    struct symbol * new = symbol_table_clone(table);
    
    struct symbol *new_entry = malloc(sizeof(struct symbol));
    new_entry->name = name;
    new_entry->next = new;
    new_entry->type = type;

    if (type == variable) {
        new_entry->reg = regs_new_var ();
    } else if (type == label) {
        new_entry->reg = asm_new_label(name);
    }

    return new_entry;
}

struct symbol *symbol_table_merge(struct symbol* table1, struct symbol* table2)  {
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
        printf("(%s, %d)", next->name, next->type);
        next = next->next;  
    }

    printf("\n");
}

struct symbol *symbol_table_get(struct symbol* table, char *name) {
    struct symbol *next = table;

    while (next != NULL) {
        if (0 == strcmp(next->name, name)) {
            return next;
        } else {
            next = next->next;  
        }
    }

    return NULL;
}

bool symbol_table_exists (struct symbol* table, char *name) {
    return symbol_table_get (table, name) != NULL;
}

bool symbol_table_exists_type(struct symbol* table, char *name, int type) {
    struct symbol *next = table;

    while (next != NULL) {
        if ((0 == strcmp(next->name, name)) && (next->type & type)) {
            return true;
        } else {
            next = next->next;  
        }
    }

    symbol_table_print(table);
    printf("symbol %s with type %d doesn't exist\n", name, type);

    return false;
}

struct symbol* symbol_table_clone(struct symbol* table) {
    if (table == NULL) {
        return NULL;
    }

    struct symbol *current; 
    struct symbol *clone = current = malloc(sizeof(struct symbol)); 

    clone->type = table->type;
    clone->name = strdup(table->name); 
    if (table->reg) {
        clone->reg = strdup(table->reg);
    }
     
    while ((table = table->next) != NULL) {
        current->next = malloc(sizeof(struct symbol)); 
        current = current->next;
        current->type = table->type;
        current->name = strdup(table->name);
        if (table->reg) {
            current->reg = strdup(table->reg);
        }
    }

    return clone;
}
