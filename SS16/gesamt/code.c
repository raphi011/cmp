#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdarg.h>

#include "code.h"
#include "symbol_table.h"

void burm_reduce(treenode *, int);
void burm_label(treenode *);

void
code_print (const char *str, ...) {
    va_list arguments;

    va_start (arguments, str);
    printf("\t");
    vprintf(str, arguments);
    printf("\n");

    va_end(arguments);
}

void
code_generate(treenode* node) {
    burm_label (node);
    burm_reduce (node, 1);
}

treenode*
code_assign(treenode *left, char *name, struct symbol *symbols) {
    if (!symbol_table_exists_type (symbols, name, variable | parameter)) {
        exit (EXIT_ERROR); 
    }

    treenode *node = code_op(C_ASSIGN, left, NULL);

    struct symbol* sym = symbol_table_get (symbols, name);

    node->name = name;
    if (sym->reg) {
        node->reg = strdup(sym->reg);
    }
    node->temp = false;

    return node;
}

treenode*
code_call(treenode* call, char *name) {
    treenode *node = code_op(C_FUNC, call, NULL);

    node->name = name;

    return node;
}

treenode*
code_guarded(treenode* guard, char *label) {
    treenode *guarded = code_op(C_GUARDED, guard, NULL);

    guarded->name = label;

    return guarded;
}

treenode*
code_op (int op, treenode *left, treenode *right) {
    treenode *node = (treenode *)malloc (sizeof(treenode));

    node->op = op;
    node->kids[0] = left;
    node->kids[1] = right;
    node->reg = NULL;
    node->val = 0;
    node->temp = true;

    return node;
}

treenode* 
code_num (int num) {
    treenode *node = code_op(C_NUM, NULL, NULL);

    node->val = num;

    return node;
}

treenode* 
code_id (char *name, struct symbol *symbols) {
    if (!symbol_table_exists_type (symbols, name, variable | parameter)) {
        exit (EXIT_ERROR); 
    }

    treenode *node = code_op (C_ID, NULL, NULL);

    struct symbol* sym = symbol_table_get (symbols, name);

    node->name = name;
    node->reg = sym->reg;
    node->temp = false;

    return node;
}
