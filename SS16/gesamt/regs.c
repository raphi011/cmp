#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "symbol_table.h"
#include "regs.h"
#include "code.h"

bool used_regs[MAX_REG];
bool temp_regs[MAX_REG];

char *caller_regs[] = { "rdi", "rsi", "rdx", "rcx", "r8", "r9", "rax", "r10", "r11" };

struct symbol*
regs_init_vars (struct symbol *pars) {
    int i = 0;
    struct symbol *next = pars;
    
    /* assign registers to parameters */
    for (i = 0; i < MAX_PARAMS && next != NULL; i++) {
        used_regs[i] = true;
        temp_regs[i] = false;
        next->reg = strdup(caller_regs[i]);
        next = next->next;
    }

    /* init used / temp values */
    for (; i < MAX_REG; i++) {
        used_regs[i] = false;
        temp_regs[i] = true;
    }

    return pars;
}

void
regs_setup_call_params (treenode* node, char *preserve) {
    int i = 0;

    while (node != NULL) {
        treenode* param = (OP_LABEL(node) == C_FUNC_PARA_EXPR) ? LEFT_CHILD(node) : node;

        /* printf("# setup_call_params: NODE node->op = %d, node->reg = %s, node->val = %d\n", OP_LABEL(node), REG(node), VAL(node));
        printf("# setup_call_params: LEFT_CHILD node->op = %d, node->reg = %s, node->val = %d\n", OP_LABEL(LEFT_CHILD(node)), LEFT_REG(node), LEFT_VAL(node));
        printf("# setup_call_params: selected: %s, reg: %s\n", (OP_LABEL(node) == C_FUNC_PARA_EXPR) ? "LEFT_CHILD" : "NODE", REG (param));  */

        if (!HAS_REG (param)) {
            code_print ("mov $%d, %%%s", VAL(param), caller_regs[i]);
        } else {
            char *reg = REG (param);

            if (strcmp (reg, caller_regs[i]) != 0) {
                int ofs = regs_caller_stack_ofs(reg, preserve);

                if (ofs == -1) {
                    code_print ("mov %%%s, %%%s", reg, caller_regs[i]);
                } else {
                    code_print ("mov %i(%%rsp), %%%s", ofs, caller_regs[i]);
                }
            }
        }


        if (OP_LABEL (node) == C_FUNC_PARA_EXPR) {
            node = RIGHT_CHILD(node);
        } else {
            node = NULL;
        }
         
        i++;
    }
}

int 
regs_caller_stack_ofs (char *reg, char *preserve) {
    int i;
    int index = 0;

    for (i = 0; i < MAX_REG; i++) {
        int cur_reg_index = MAX_REG - 1 - i;
        if (!used_regs[cur_reg_index] || strcmp (preserve, caller_regs[cur_reg_index]) == 0) {
            continue;
        }

        if (strcmp (reg, caller_regs[cur_reg_index]) == 0) {
            return index * 8;
        }

        index++;
    }

    /* reg is not on stack, direct use of reg possible */
    return -1;
}

void
regs_restore_caller(char *preserve) {
    int i;

    for (i = MAX_REG - 1; i >= 0; i--) {
        /* only restore if it was used before and is not the return register */
        if (used_regs[i] && (strcmp (preserve, caller_regs[i]) != 0)) {
            code_print("pop %%%s", caller_regs[i]);
        }
    }
}

void
regs_save_caller(char * preserve) {
    int i;

    for (i = 0; i < MAX_REG; i++) {
        /* only save if it was used before and is not the return register */
        if (used_regs[i] && strcmp (preserve, caller_regs[i]) != 0) {
            code_print("push %%%s", caller_regs[i]);
        }
    }
}

void 
regs_free_if_temp (char *r) {
    int i;


    for (i = 0; i < MAX_REG; i++) {
        if (strcmp(r, caller_regs[i]) == 0
            && temp_regs[i]) {
            used_regs[i] = false;
            return;
        }
    }
}

char*
regs_new_par (void) {
    int i;

    for (i = MAX_REG - 1; i >= 0; i--) {
        if (!used_regs[i]) {
            char *reg = strdup(caller_regs[i]);
            temp_regs[i] = false;
            used_regs[i] = true;
            return reg;
        }
    }

    printf("Out of registers\n");
    exit(EXIT_ERROR);
}

char*
regs_new_temp (void) {
    int i;

    for (i = 0; i < MAX_REG; i++) {
        if (!used_regs[i] && temp_regs[i]) {
            used_regs[i] = true;
            return strdup (caller_regs[i]);
        }
    }

    /* 
    if (used_callee_regs < MAX_CALLEE) {
        printf("\tpush %%%s\n", callee_regs[used_callee_regs]);
        char *reg = strdup (callee_regs[used_callee_regs]);
        used_callee_regs++;
        return reg;
    }
    */

    printf("Out of registers\n");
    exit(EXIT_ERROR);
}

char*
regs_8bit (char* r) {
	if (strcmp(r, "rax") == 0) {
		return "al";
	} else if (strcmp (r, "rdi") == 0) {
		return "dil";
	} else if (strcmp (r, "rsi") == 0) {
		return "sil";
	} else if (strcmp (r, "rdx") == 0) {
		return "dl";
	} else if (strcmp (r, "rcx") == 0) {
		return "cl";
	} else if (strcmp (r, "r8") == 0) {
		return "r8b";
	} else if (strcmp (r, "r9") == 0) {
		return "r9b";
	} else if (strcmp (r, "r10") == 0) {
		return "r10b";
	} else if (strcmp (r, "r11") == 0) {
		return "r11b";
	} else {
		return NULL;
	}
}


/* void
regs_pop_callee(void) {
    int i;

    for (i = used_callee_regs - 1; i >= 0; i--) {
        code_print("pop %%%s", callee_regs[i]);
    }

    used_callee_regs = 0;
} */

