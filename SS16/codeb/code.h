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
C_MEM_READ=13,
C_ASSIGN=14,
C_MEM_WRITE=15
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
    bool temp;
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
treenode* code_assign(treenode *, char *, struct symbol *);

#endif
