#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>

#include "code_gen.h"
#include "symbol_table.h"
#include "regs.h"

static void print_code (const char *str, ...) {
    va_list arguments;

    va_start (arguments, str);

    printf("\t");

    vprintf(str, arguments);

    printf("\n");

    va_end(arguments);
}

static void code_move(treenode *par, char *reg) {
    if (HAS_REG (par)) {
        print_code ("movq %%%s, %%%s", REG(par), reg);
    } else {
        print_code ("movq $%d, %%%s", VAL(par), reg);
    }
}

treenode* code_op (int op, treenode *left, treenode *right) {
    treenode *node = (treenode *)malloc(sizeof(treenode));

    node->op = op;
    node->kids[0] = left;
    node->kids[1] = right;
    node->reg = NULL;
    node->val = 0;

    return node;
}

treenode* code_num (int num) {
    treenode *node = code_op(C_NUM, NULL, NULL);

    node->val = num;

    return node;
}

treenode* code_id (char *name, struct symbol *symbols) {
    if (!symbol_table_exists_type(symbols, name, variable)) {
        exit(EXIT_ERROR); 
    }

    treenode *node = code_op(C_ID, NULL, NULL);

    struct symbol* sym = symbol_table_get (symbols, name);

    node->name = name;
    node->reg = sym->reg;

    return node;
}

void code_func (char* name) {
    printf (".globl %1$s\n.type %1$s, @function\n%1$s:\n", name);
}

void code_ret_const (int val) {
    print_code ("movq $%d, %%rax", val);
    print_code ("ret");
}

void code_ret (treenode* par) {
    code_move(par, "rax");
    print_code ("ret");
}

char* code_add (treenode *par1, treenode *par2) {
    char * reg = regs_new();

    if (HAS_REG(par1) && HAS_REG(par2)) {
        code_move(par1, reg);
        print_code ("addq %%%s, %%%s", REG(par2), reg);
    } else if (!HAS_REG(par1)) {
        code_move(par2, reg);
        print_code ("addq $%d, %%%s", VAL(par1), reg);
    } else {
        code_move(par1, reg);
        print_code ("addq $%d, %%%s", VAL(par2), reg);
    }

    return reg;
}

char* code_mult (treenode *par1, treenode *par2) {
    char * reg = regs_new();

    if (HAS_REG(par1) && HAS_REG(par2)) {
        code_move(par1, reg);
        print_code ("imulq %%%s, %%%s", REG(par2), reg);
    } else if (!HAS_REG(par1)) {
        code_move(par2, reg);
        print_code ("imulq $%d, %%%s", VAL(par1), reg);
    } else {
        code_move(par1, reg);
        print_code ("imulq $%d, %%%s", VAL(par2), reg);
    }

    return reg;
}


char* code_minus (treenode *par) {
    char * reg = regs_new ();

    code_move(par, reg);
    print_code ("neg %%%s", reg);

    return reg;
}

char* code_or (treenode *par1, treenode *par2) {
    char *reg = regs_new ();

    if (HAS_REG(par1) && HAS_REG(par2)) {
        code_move(par1, reg);
        print_code ("or %%%s, %%%s", REG(par2), reg);
    } else if (!HAS_REG(par1)) {
        code_move(par2, reg);
        print_code ("or $%d, %%%s", VAL(par1), reg);
    } else {
        code_move(par1, reg);
        print_code ("or $%d, %%%s", VAL(par2), reg);
    }

    return reg;
}

char* code_not (treenode *par) {
    char *reg = regs_new ();

    code_move (par, reg);
    print_code ("not %%%s", reg);

    return reg;
}

char* code_less (treenode *par1, treenode *par2) {
    char *reg = regs_new ();
    print_code ("movq $0, %%%s", reg);

    if (HAS_REG(par1) && HAS_REG(par2)) {
        print_code ("cmp %%%s, %%%s", REG(par2), REG(par1));
        print_code ("setge %%%s", regs_8bit(reg)); 
    } else if (!HAS_REG(par1) && !HAS_REG(par2)) {
        print_code ("movq $%d, %%%s", VAL(par1) < VAL(par2) ? -1 : 0, reg);
        return reg;
    } else if (!HAS_REG(par1)) {
        print_code ("cmp $%d, %%%s", VAL(par1), REG(par2));
        print_code ("setl %%%s", regs_8bit(reg)); 
    } else {
        print_code ("cmp $%d, %%%s", VAL(par2), REG(par1));
        print_code ("setge %%%s", regs_8bit(reg)); 
    }

    print_code ("sub $1, %%%s", reg); 

    return reg;
}

char* code_mem (treenode *par) {
    char *reg = regs_new ();

    if (HAS_REG(par)) {
        print_code ("mov (%%%s), %%%s", REG(par), reg);
    } else {
        print_code ("mov ($%d), %%%s", VAL(par), reg);
    }

    return reg;
}

char* code_eq (treenode *par1, treenode *par2) {
    char *reg = regs_new ();
    print_code ("movq $0, %%%s", reg);

    if (HAS_REG(par1) && HAS_REG(par2)) {
        print_code ("cmp %%%s, %%%s", REG(par1), REG(par2));
    } else if (!HAS_REG(par1) && !HAS_REG(par2)) {
        print_code ("movq $%d, %%%s", VAL(par1) == VAL(par2) ? -1 : 0, reg);
        return reg;
    } else if (!HAS_REG(par1)) {
        print_code ("cmp $%d, %%%s", VAL(par1), REG(par2));
    } else {
        print_code ("cmp $%d, %%%s", VAL(par2), REG(par1));
    }

    print_code ("setne %%%s", regs_8bit(reg)); 
    print_code ("sub $1, %%%s", reg); 

    return reg;
}
