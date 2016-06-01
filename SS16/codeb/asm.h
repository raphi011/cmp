#include "code.h"

void asm_ret_const(int);
void asm_ret(treenode *);
void asm_assign(treenode *, treenode *);
void asm_mem_write(treenode *par1, treenode *par2);

void asm_func(char *);

char* asm_add(treenode *, treenode *);
char* asm_mult(treenode *, treenode *);
char* asm_or(treenode *, treenode*);
char* asm_less(treenode *, treenode*);
char* asm_eq(treenode *, treenode *);

char* asm_not(treenode*);
char* asm_minus(treenode*);
char* asm_mem(treenode*);
