#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "symbol_table.h"
#include "regs.h"

#define MAX_REG (9)
#define MAX_PARAMS (6)
#define MAX_CALLEE (5)

char* avail_regs[MAX_REG];
bool used_regs[MAX_REG];

char *par_regs[] = { "rdi", "rsi", "rdx", "rcx", "r8", "r9" };

char *callee_regs[] = { "rbx", "r12", "r13", "r14", "r15" };

int used_callee_regs = 0;

struct symbol*
regs_init_vars (struct symbol *pars) {
    int i = 0, j = 0;
    struct symbol *next = pars;

    while (next != NULL) {
        next->reg = strdup(par_regs[i++]);
        next = next->next;
    }

    while (j < MAX_REG) {
        used_regs[j] = false;
        avail_regs[j] = NULL;
        j++;
    }

    j = 3;

    // general purpose regs
    avail_regs[0] = "rax"; 
    avail_regs[1] = "r10"; 
    avail_regs[2] = "r11"; 

    while (i < MAX_PARAMS) {
        // add non used argument regs
        avail_regs[j++] = par_regs[i++];
    }

    return pars;
}

void 
regs_free_if_temp (char *r) {
    int i;

    for (i = 0; i < MAX_REG; i++) {
        if (avail_regs[i] == NULL) {
            break;
        }
        if (strcmp(r, avail_regs[i]) == 0) {
                used_regs[i] = false;
                return;
        }
    }
}

char*
regs_new_par (void) {
    int i;

    for (i = MAX_REG - 1; i >= 0; i--) {
        if (avail_regs[i] != NULL && !used_regs[i]) {
            char *reg = strdup(avail_regs[i]);
            avail_regs[i] = NULL;
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
        if (avail_regs[i] == NULL) {
            break;
        }
        if (!used_regs[i]) {
            used_regs[i] = true;
            return strdup (avail_regs[i]);
        }
    }

    if (used_callee_regs < MAX_CALLEE) {
        printf("\tpush %%%s\n", callee_regs[used_callee_regs]);
        char *reg = strdup (callee_regs[used_callee_regs]);
        used_callee_regs++;
        return reg;
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
regs_pop_callee(void) {
    int i;
    for (i = used_callee_regs - 1; i >= 0; i--) {
        printf("\tpop %%%s\n", callee_regs[i]);
    }

    used_callee_regs = 0;
}
