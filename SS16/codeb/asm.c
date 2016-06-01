#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "asm.h"
#include "regs.h"

static void print_code (const char *str, ...) {
    va_list arguments;

    va_start (arguments, str);

    printf("\t");

    vprintf(str, arguments);

    printf("\n");

    va_end(arguments);
}

static void asm_move(treenode *par, char *reg) {
    if (HAS_REG (par)) {
        print_code ("movq %%%s, %%%s", REG(par), reg);
    } else {
        print_code ("movq $%d, %%%s", VAL(par), reg);
    }
}

void asm_mem_write(treenode *par1, treenode *par2) {
    if (HAS_REG (par1)) {
        print_code ("movq %%%s, (%%%s)", REG(par1), REG(par2));
    } else {
        print_code ("movq $%d, (%%%s)", VAL(par1), REG(par2));
    }
}

void asm_func (char* name) {
    printf (".globl %1$s\n.type %1$s, @function\n%1$s:\n", name);
}

void asm_ret_const (int val) {
    print_code ("movq $%d, %%rax", val);
    print_code ("ret");
}

void asm_assign (treenode *par1, treenode *par2) {
    asm_move (par2, REG(par1));
}

void asm_ret (treenode *par) {
    asm_move (par, "rax");
    print_code ("ret");
}

char* asm_add (treenode *par1, treenode *par2) {
    char * reg = regs_new_temp();

    if (HAS_REG(par1) && HAS_REG(par2)) {
        asm_move(par1, reg);
        print_code ("addq %%%s, %%%s", REG(par2), reg);
    } else if (!HAS_REG(par1)) {
        asm_move(par2, reg);
        print_code ("addq $%d, %%%s", VAL(par1), reg);
    } else {
        asm_move(par1, reg);
        print_code ("addq $%d, %%%s", VAL(par2), reg);
    }

    return reg;
}

char* asm_mult (treenode *par1, treenode *par2) {
    char * reg = regs_new_temp();

    if (HAS_REG(par1) && HAS_REG(par2)) {
        asm_move(par1, reg);
        print_code ("imulq %%%s, %%%s", REG(par2), reg);
    } else if (!HAS_REG(par1)) {
        asm_move(par2, reg);
        print_code ("imulq $%d, %%%s", VAL(par1), reg);
    } else {
        asm_move(par1, reg);
        print_code ("imulq $%d, %%%s", VAL(par2), reg);
    }

    return reg;
}


char* asm_minus (treenode *par) {
    char * reg = regs_new_temp ();

    asm_move(par, reg);
    print_code ("neg %%%s", reg);

    return reg;
}

char* asm_or (treenode *par1, treenode *par2) {
    char *reg = regs_new_temp ();

    if (HAS_REG(par1) && HAS_REG(par2)) {
        asm_move(par1, reg);
        print_code ("or %%%s, %%%s", REG(par2), reg);
    } else if (!HAS_REG(par1)) {
        asm_move(par2, reg);
        print_code ("or $%d, %%%s", VAL(par1), reg);
    } else {
        asm_move(par1, reg);
        print_code ("or $%d, %%%s", VAL(par2), reg);
    }

    return reg;
}

char* asm_not (treenode *par) {
    char *reg = regs_new_temp ();

    asm_move (par, reg);
    print_code ("not %%%s", reg);

    return reg;
}

char* asm_less (treenode *par1, treenode *par2) {
    char *reg = regs_new_temp ();
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

char* asm_mem (treenode *par) {
    char *reg = regs_new_temp ();

    if (HAS_REG(par)) {
        print_code ("mov (%%%s), %%%s", REG(par), reg);
    } else {
        print_code ("mov ($%d), %%%s", VAL(par), reg);
    }

    return reg;
}

char* asm_eq (treenode *par1, treenode *par2) {
    char *reg = regs_new_temp ();
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
