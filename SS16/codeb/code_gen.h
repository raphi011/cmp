#ifndef CODE_H_
#define CODE_H_

#include "symbol_table.h"

enum {
C_RETURN=1,
C_ADD=2, 
C_MULT=3,
C_MINUS=4,
C_OR=5,
C_NOT=6,
C_LESS=7,
C_EQ=8,
C_NUM=9,
C_FUNC=10,
C_RET=11,
C_ID=12,
C_MEM=13
};


#ifndef CODE
   typedef struct burm_state *STATEPTR_TYPE; 
#endif

typedef struct s_node {
    int         op;
    struct s_node *kids[2];

    STATEPTR_TYPE state;

    char *name; 
    char *reg;
    int val;

} treenode;

typedef treenode *treenodep;

#define NAME(p)         ((p)->name)
#define REG(p)          ((p)->reg)
#define VAL(p)          ((p)->val)
#define HAS_REG(p)      ((p)->reg != NULL)
#define OP_LABEL(p)     ((p)->op)
#define LEFT_CHILD(p)   ((p)->kids[0])
#define RIGHT_CHILD(p)  ((p)->kids[1])
#define LEFT_VAL(p)     (LEFT_CHILD(p)->val)
#define RIGHT_VAL(p)    (RIGHT_CHILD(p)->val)
#define LEFT_REG(p)     (REG(LEFT_CHILD(p)))
#define RIGHT_REG(p)    (REG(RIGHT_CHILD(p)))
#define STATE_LABEL(p)  ((p)->state)
#define PANIC           printf
#define NODEPTR_TYPE    treenodep


treenode* code_op(int, treenode*, treenode*);
treenode* code_num(int);
treenode* code_id(char *, struct symbol*);

void code_ret_const(int);
void code_ret(treenode *);
void code_func(char *);

char* code_add(treenode *, treenode *);
char* code_mult(treenode *, treenode *);
char* code_or(treenode *, treenode*);
char* code_less(treenode *, treenode*);
char* code_eq(treenode *, treenode *);

char* code_not(treenode*);
char* code_minus(treenode*);
char* code_mem(treenode*);

#endif
