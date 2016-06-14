#ifndef REGS_H_
#define REGS_H_

#include "symbol_table.h"

extern int used_callee_regs;

void regs_free_if_temp(char *);
char* regs_new_par (void);
struct symbol* regs_init_vars (struct symbol *);
char* regs_new_temp (void);
char* regs_8bit (char* r);
void regs_pop_callee(void);

#endif
