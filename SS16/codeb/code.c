#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "code.h"
#include "symbol_table.h"

treenode* code_assign(treenode *left, char *name, struct symbol *symbols) {
    treenode *node = code_op(C_ASSIGN, left, NULL);

    struct symbol* sym = symbol_table_get (symbols, name);

    node->name = name;
    node->reg = sym->reg;
    node->temp = false;

    return node;
}

treenode* code_op (int op, treenode *left, treenode *right) {
    treenode *node = (treenode *)malloc(sizeof(treenode));

    node->op = op;
    node->kids[0] = left;
    node->kids[1] = right;
    node->reg = NULL;
    node->val = 0;
    node->temp = true;

    return node;
}

treenode* code_num (int num) {
    treenode *node = code_op(C_NUM, NULL, NULL);

    node->val = num;

    return node;
}

treenode* code_id (char *name, struct symbol *symbols) {
    if (!symbol_table_exists_type(symbols, name, variable | parameter)) {
        exit(EXIT_ERROR); 
    }

    treenode *node = code_op(C_ID, NULL, NULL);

    struct symbol* sym = symbol_table_get (symbols, name);

    node->name = name;
    node->reg = sym->reg;
    node->temp = false;

    return node;
}
