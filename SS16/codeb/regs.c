#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "symbol_table.h"

#define MAX_REG (9)
#define MAX_PARAMS (6)

char* reg_avail[MAX_REG];
bool reg_used[MAX_REG];

char *regs[] = { "rdi", "rsi", "rdx", "rcx", "r8", "r9" };
char *callee_regs[] = { "rbx", "r12", "r13", "r14", "r15" };

struct symbol*
regs_init_vars (struct symbol *pars) {
    int i = 0, j = 0;
    struct symbol *next = pars;

    while (next != NULL) {
        next->reg = strdup(regs[i++]);
        next = next->next;
    }

    while (j < MAX_REG) {
        reg_used[j] = false;
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

    return pars;
}

void 
regs_free_if_temp (char *r) {
    int i;

    for (i = 0; i < MAX_REG; i++) {
        if (reg_avail[i] == NULL) {
            break;
        }
        if (strcmp(r, reg_avail[i]) == 0) {
                reg_used[i] = false;
                printf("# freed %s\n", r);
                return;
        }
    }
}

char*
regs_new_var (void) {
    int i;

    for (i = MAX_REG - 1; i >= 0; i--) {
        if (reg_avail[i] != NULL && !reg_used[i]) {
            char *reg = strdup(reg_avail[i]);
            reg_avail[i] = NULL;
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
        if (reg_avail[i] == NULL) {
            break;
        }
        if (!reg_used[i]) {
            reg_used[i] = true;
            return strdup (reg_avail[i]);
        }
    }

    printf("Out of registers\n");
    exit(EXIT_ERROR);
}

char*
regs_8bit(char* r) {
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

void
regs_cleanup(void) {
    printf("cleaning up\n");
}
