#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>

#include "code_gen.h"



treenode* code_op (int op, treenode *left, treenode *right) {
    treenode *node = (treenode *)malloc(sizeof(treenode));

    node->op = op;
    node->kids[0] = left;
    node->kids[1] = right;
    node->reg = 0;
    node->val = 0;

    return node;
}

treenode* code_num (int num) {
    treenode *node = code_op(C_NUM, NULL, NULL);

    node->val = num;

    return node;
}

treenode* code_id (char *name, struct symbol *symbols) {
    treenode *node = code_op(C_ID, NULL, NULL);

    struct symbol* sym = symbol_table_get (symbols, name);

    node->name = name;
    node->reg = strdup(sym->reg);

    return node;
}

#define MAX_REG (6)

char* reg_avail[MAX_REG];
int reg_used[MAX_REG];

char *regs[] = { "rdi", "rsi", "rdx", "rcx", "r8", "r9" };

void code_init_vars (struct symbol *pars) {

    int i = 0, j = 0;


    while (pars != NULL) {
        i++;
        pars = pars->next;
    }

    while (i < MAX_REG) {
        reg_avail[j++] = regs[i++];
    }
}

struct symbol* code_init_pars (struct symbol *pars) {
    int i = 0;

    struct symbol *next = pars;

    while (next != NULL) {
        next->reg = strdup(regs[i++]);
        next = next->next;
    }

    return pars;
}

static void print_code (const char *str, ...) {
    va_list arguments;

    va_start (arguments, str);

    printf("\t");

    vprintf(str, arguments);

    printf("\n");

    va_end(arguments);
}

char* code_get_reg (void) {
    int i;

    for (i = 0; i < MAX_REG; i++) {
        if (!reg_used[i]) {
            reg_used[i] = 1;
            return strdup (reg_avail[i]);
        }
    }

    return NULL;
}


void code_func (char* name) {
    printf (".globl %1$s\n.type %1$s, @function\n%1$s:\n", name);
}

void code_ret_const (int val) {
    print_code ("movq $%d, %%rax", val);
    print_code ("ret");
}

void code_ret (char *reg) {
    print_code ("movq %%%s, %%rax", reg);
    print_code ("ret");
}


char* code_add (char *par1, char *par2) {
    char * reg = code_get_reg();

    print_code ("movq %%%s, %%%s", par1, reg);
    print_code ("addq %%%s, %%%s", par2, reg);

    return reg;
}

char* code_mult (char *par1, char *par2) {
    char * reg = code_get_reg();

    print_code ("movq %%%s, %%%s", par1, reg);
    print_code ("imulq %%%s, %%%s", par2, reg);

    return reg;
}

char* code_minus (char *par) {
    char * reg = code_get_reg();

    print_code ("neg %%%s", par);

    return reg;
}

char* code_or (char *par1, char *par2) {

    return NULL;
}

char* code_not (char *par) {
    char *reg = code_get_reg();

    print_code ("not %%%s", par);

    return reg;
}

char* code_less (char *par1, char *par2) {

    return NULL;
}

char* code_eq (char *par1, char *par2) {

    return NULL;
}
