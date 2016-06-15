#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "asm.h"
#include "regs.h"


static inline void
asm_mov_reg(char *reg1, char *reg2) {
    if (strcmp(reg1, reg2) == 0) {
        code_print ("# unncessary move");
        return;
    }

    code_print ("movq %%%s, %%%s", reg1, reg2);
}

static inline void
asm_mov_const(int val, char *reg2) {
    code_print ("movq $%d, %%%s", val, reg2);
}

void
asm_move (treenode *par, char *reg) {
    if (HAS_REG (par)) {
        asm_mov_reg(REG(par), reg);
    } else {
        asm_mov_const(VAL(par), reg);
    }
}

void
asm_mem_write (treenode *term, treenode *expr) {
    if (HAS_REG (term) && HAS_REG (expr)) {
        code_print ("mov %%%s, (%%%s)", REG(expr), REG(term));
    } else if (!HAS_REG (expr) && HAS_REG (term)) {
        code_print ("mov $%d, (%%%s)", VAL(expr), REG(term));
    } else if (HAS_REG (expr) && !HAS_REG (term)) {
        code_print ("mov %%%s, ($%d)", REG(expr), VAL(term));
    } else {
        code_print ("movq $%d, ($%d)", VAL(expr), VAL(term));
    }
}

int label_id = 0;

char*
asm_new_label(char *name) {
    size_t len = strlen (name) + 4;
    char *label = malloc (len);

    snprintf(label, len, "%s_%d", name, label_id++); 

    return label;
}

void
asm_dostat (treenode* node) {
    printf("dostat:\n");
}

void
asm_guarded (char *label, char *guarded, bool cont) {
    if (cont) {
        code_print("jmp %s", label);
    } else {
        code_print("jmp %s_end", label);
    }

    printf("%s_end:\n", guarded);
}

bool returned;

void
asm_func (char* name) {
    printf (".globl %1$s\n.type %1$s, @function\n%1$s:\n", name);
    returned = false;
}

void
asm_add_ret(void) {
    if (!returned) {
        code_print ("ret");
    }

}

void
asm_cond (char *label, treenode *node) {
    if (HAS_REG (node)) {
        code_print("cmp $0, %%%s", REG(node));
        code_print("jns %s_end", label);
    } else {
        if (VAL(node) >= 0) {
            code_print("jmp %s_end", label);
        }
    }
}

static void
asm_ret (void) {
    /* regs_pop_callee(); */

    code_print ("ret");
    returned = true;
}

void
asm_ret_reg (treenode *par) {
    asm_move (par, "rax");
    asm_ret ();
}

void
asm_ret_const (int val) {
    asm_mov_const(val, "rax");
    asm_ret ();
}

void
asm_assign (treenode *par1, treenode *par2) {
    asm_move (par2, REG(par1)); 
}


char*
asm_add (treenode *par1, treenode *par2) {
    char * reg = regs_new_temp();

    if (HAS_REG(par1) && HAS_REG(par2)) {
        asm_move(par1, reg);
        code_print ("addq %%%s, %%%s", REG(par2), reg);

        regs_free_if_temp (REG(par1));
        regs_free_if_temp (REG(par2));
    } else if (!HAS_REG (par1) && !HAS_REG (par2)) {
        
        asm_mov_const (VAL(par1) + VAL(par2), reg); 
    } else if (!HAS_REG(par1)) {
        asm_move(par2, reg);
        code_print ("addq $%d, %%%s", VAL(par1), reg);

        regs_free_if_temp (REG(par2));
    } else {
        asm_move(par1, reg);
        code_print ("addq $%d, %%%s", VAL(par2), reg);

        regs_free_if_temp (REG(par1));
    }

    return reg;
}

char*
asm_mult (treenode *par1, treenode *par2) {
    char * reg = regs_new_temp();

    if (HAS_REG(par1) && HAS_REG(par2)) {
        asm_move(par1, reg);
        code_print ("imulq %%%s, %%%s", REG(par2), reg);

        regs_free_if_temp (REG(par1));
        regs_free_if_temp (REG(par2));
    } else if (!HAS_REG (par1) && !HAS_REG (par2)) {
        asm_mov_const (VAL(par1) * VAL(par2), reg); 
    } else if (!HAS_REG(par1)) {
        asm_move(par2, reg);
        code_print ("imulq $%d, %%%s", VAL(par1), reg);

        regs_free_if_temp (REG(par2));
    } else {
        asm_move(par1, reg);
        code_print ("imulq $%d, %%%s", VAL(par2), reg);

        regs_free_if_temp (REG(par1));
    }

    return reg;
}


char*
asm_minus (treenode *par) {
    char * reg = regs_new_temp ();

    if (HAS_REG(par)) {
        asm_move(par, reg);
        code_print ("neg %%%s", reg);
        regs_free_if_temp (REG(par));
    } else {
        asm_mov_const(-VAL(par), reg);
        printf("# minus fixed value\n");
    }

    return reg;
}

char*
asm_or (treenode *par1, treenode *par2) {
    char *reg = regs_new_temp ();

    if (HAS_REG(par1) && HAS_REG(par2)) {
        asm_move(par1, reg);
        code_print ("or %%%s, %%%s", REG(par2), reg);

        regs_free_if_temp (REG(par1));
        regs_free_if_temp (REG(par2));
    } else if (!HAS_REG (par1) && !HAS_REG (par2)) {
        asm_mov_const (VAL(par1) | VAL(par2), reg); 
    } else if (!HAS_REG(par1)) {
        asm_move(par2, reg);
        code_print ("or $%d, %%%s", VAL(par1), reg);

        regs_free_if_temp (REG(par2));
    } else {
        asm_move(par1, reg);
        code_print ("or $%d, %%%s", VAL(par2), reg);

        regs_free_if_temp (REG(par1));
    }

    return reg;
}

char*
asm_not (treenode *par) {
    char *reg = regs_new_temp ();

    asm_move (par, reg);
    code_print ("not %%%s", reg);

    if (HAS_REG (par)) {
        regs_free_if_temp (REG(par));
    }

    return reg;
}

char*
asm_less (treenode *par1, treenode *par2) {
    char *reg = regs_new_temp ();
    asm_mov_const(0, reg);

    if (HAS_REG(par1) && HAS_REG(par2)) {
        code_print ("cmp %%%s, %%%s", REG(par2), REG(par1));
        code_print ("setge %%%s", regs_8bit(reg)); 

        regs_free_if_temp (REG(par1));
        regs_free_if_temp (REG(par2));
    } else if (!HAS_REG(par1) && !HAS_REG(par2)) {
        asm_mov_const(VAL(par1) < VAL(par2) ? -1 : 0, reg);
        return reg;
    } else if (!HAS_REG(par1)) {
        code_print ("cmp $%d, %%%s", VAL(par1), REG(par2));
        code_print ("setl %%%s", regs_8bit(reg)); 

        regs_free_if_temp (REG(par2));
    } else {
        code_print ("cmp $%d, %%%s", VAL(par2), REG(par1));
        code_print ("setge %%%s", regs_8bit(reg)); 

        regs_free_if_temp (REG(par1));
    }

    code_print ("sub $1, %%%s", reg); 

    return reg;
}

char*
asm_mem (treenode *par) {
    char *reg = regs_new_temp ();

    if (HAS_REG(par)) {
        code_print ("mov (%%%s), %%%s", REG(par), reg);
        regs_free_if_temp (REG(par));
    } else {
        code_print ("mov ($%d), %%%s", VAL(par), reg);
    }

    return reg;
}

char*
asm_eq (treenode *par1, treenode *par2) {
    char *reg = regs_new_temp ();
    asm_mov_const(0, reg);

    if (HAS_REG(par1) && HAS_REG(par2)) {
        code_print ("cmp %%%s, %%%s", REG(par1), REG(par2));

        regs_free_if_temp (REG(par1));
        regs_free_if_temp (REG(par2));
    } else if (!HAS_REG(par1) && !HAS_REG(par2)) {
        asm_mov_const(VAL(par1) == VAL(par2) ? -1 : 0, reg);
        return reg;
    } else if (!HAS_REG(par1)) {
        code_print ("cmp $%d, %%%s", VAL(par1), REG(par2));

        regs_free_if_temp (REG(par2));
    } else {
        code_print ("cmp $%d, %%%s", VAL(par2), REG(par1));

        regs_free_if_temp (REG(par1));
    }

    code_print ("setne %%%s", regs_8bit(reg)); 
    code_print ("sub $1, %%%s", reg); 

    return reg;
}

char* 
asm_call (treenode *call) {
    char *reg = regs_new_temp ();
    char *func_name = call->name;
    char *par_regs[MAX_PARAMS] = { NULL };
    int i;
    int reg_index = 0;

    regs_save_caller(reg);
    
    treenode *param = LEFT_CHILD(call); 


    regs_setup_call_params(param, reg);

    code_print("call %s", func_name);
    asm_mov_reg("rax", reg);

    regs_restore_caller(reg);

    return reg;
}
