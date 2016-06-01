#include "symbol_table.h"

bool regs_is_temp(char *);
char* regs_new_var (void);
struct symbol* regs_init_vars (struct symbol *);
char* regs_new_temp (void);
char* regs_8bit (char* r);
