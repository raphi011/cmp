#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>

#include "code_gen.h"
#include "symbol_table.h"

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

#define MAX_REG (9)
#define MAX_PARAMS (6)

char* reg_avail[MAX_REG];
int reg_used[MAX_REG];

char *regs[] = { "rdi", "rsi", "rdx", "rcx", "r8", "r9" };

void code_init_vars (struct symbol *pars) {

    int i = 0, j = 0;


    while (pars != NULL) {
        i++;
        pars = pars->next;
    }

    while (j < MAX_REG) {
        reg_used[j] = 0;
        reg_avail[j] = NULL;
        j++;
    }

    j = 3;

    reg_avail[0] = "rax"; 
    reg_avail[1] = "r10"; 
    reg_avail[2] = "r11"; 

    while (i < MAX_PARAMS) {
        reg_avail[j++] = regs[i++];
    }
}

struct symbol* code_init_pars (struct symbol *pars) {
    int i = 0;

    struct symbol *next = pars;

    while (next != NULL) {
        next->reg = strdup(regs[i++]);
        reg_used[i] = 1;
        next = next->next;
    }

    return pars;
}


char* code_get_reg (void) {
    int i;

    for (i = 0; i < MAX_REG; i++) {
        if (reg_avail[i] == 0) {
            break;
        }
        if (!reg_used[i]) {
            reg_used[i] = 1;
            return strdup (reg_avail[i]);
        }
    }

    printf("Out of registers\n");
    exit(4);
    //return NULL;
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
    char * reg = code_get_reg();

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
    char * reg = code_get_reg();

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
    char * reg = code_get_reg ();

    code_move(par, reg);
    print_code ("neg %%%s", reg);

    return reg;
}

char* code_or (treenode *par1, treenode *par2) {
    char *reg = code_get_reg ();

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
    char *reg = code_get_reg ();

    code_move (par, reg);
    print_code ("not %%%s", reg);

    return reg;
}


static char* code_reg_8bit(char* r) {
	if (strcmp(r, "rax") == 0) {
		return "al";
	} else if (strcmp(r, "rdi") == 0) {
		return "dil";
	} else if (strcmp(r, "rsi") == 0) {
		return "sil";
	} else if (strcmp(r, "rdx") == 0) {
		return "dl";
	} else if (strcmp(r, "rcx") == 0) {
		return "cl";
	} else if (strcmp(r, "r8") == 0) {
		return "r8b";
	} else if (strcmp(r, "r9") == 0) {
		return "r9b";
	} else if (strcmp(r, "r10") == 0) {
		return "r10b";
	} else if (strcmp(r, "r11") == 0) {
		return "r11b";
	} else {
		return NULL;
	}
}

char* code_less (treenode *par1, treenode *par2) {
    char *reg = code_get_reg ();
    print_code ("movq $0, %%%s", reg);

    if (HAS_REG(par1) && HAS_REG(par2)) {
        print_code ("cmp %%%s, %%%s", REG(par2), REG(par1));
        print_code ("setge %%%s", code_reg_8bit(reg)); 
    } else if (!HAS_REG(par1) && !HAS_REG(par2)) {
        print_code ("movq $%d, %%%s", VAL(par1) < VAL(par2) ? -1 : 0, reg);
        return reg;
    } else if (!HAS_REG(par1)) {
        print_code ("cmp $%d, %%%s", VAL(par1), REG(par2));
        print_code ("setl %%%s", code_reg_8bit(reg)); 
    } else {
        print_code ("cmp $%d, %%%s", VAL(par2), REG(par1));
        print_code ("setge %%%s", code_reg_8bit(reg)); 
    }

    print_code ("sub $1, %%%s", reg); 

    return reg;
}

char* code_mem (treenode *par) {
    char *reg = code_get_reg ();

    if (HAS_REG(par)) {
        print_code ("mov (%%%s), %%%s", REG(par), reg);
    } else {
        print_code ("mov ($%d), %%%s", VAL(par), reg);
    }

    return reg;
}

char* code_eq (treenode *par1, treenode *par2) {
    char *reg = code_get_reg ();
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

    print_code ("setne %%%s", code_reg_8bit(reg)); 
    print_code ("sub $1, %%%s", reg); 

    return reg;
}
