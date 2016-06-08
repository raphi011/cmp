#include "code.h"

void asm_cond (char *, char *);
void asm_ret_const(int);
void asm_ret(treenode *);
void asm_assign(treenode *, treenode *);
void asm_mem_write(treenode *, treenode *);
void asm_move(treenode *, char *);

void asm_func(char *);
void asm_dostat (treenode*);
void asm_guarded (char *, char *, bool);

char* asm_new_label(void);

char* asm_add(treenode *, treenode *);
char* asm_mult(treenode *, treenode *);
char* asm_or(treenode *, treenode*);
char* asm_less(treenode *, treenode*);
char* asm_eq(treenode *, treenode *);

char* asm_not(treenode*);
char* asm_minus(treenode*);
char* asm_mem(treenode*);
