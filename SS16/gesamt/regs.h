#ifndef REGS_H_
#define REGS_H_

#include "symbol_table.h"

#define MAX_REG (9)
#define MAX_PARAMS (6)
#define MAX_CALLEE (5)

/*
extern char *caller_regs[];
extern char *par_regs[];

extern int used_callee_regs;
*/

void regs_setup_call_params (char**);
int regs_caller_stack_ofs (char *);
void regs_restore_caller(char *);
void regs_save_caller(char *);
void regs_free_if_temp(char *);
char* regs_new_par (void);
struct symbol* regs_init_vars (struct symbol *);
char* regs_new_temp (void);
char* regs_8bit (char* r);
void regs_pop_callee(void);

#endif
