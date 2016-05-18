#ifndef CODE_H_
#define CODE_H_

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
C_RET=11
};


#ifndef CODE
   typedef struct burm_state *STATEPTR_TYPE; 
#endif

typedef struct s_node {
    int         op;
    struct s_node *kids[2];
    STATEPTR_TYPE state;
    char*           name;
    int             val;
} treenode;

typedef treenode *treenodep;

#define NAME(p)         ((p)->name)
#define OP_LABEL(p)     ((p)->op)
#define LEFT_CHILD(p)   ((p)->kids[0])
#define RIGHT_CHILD(p)  ((p)->kids[1])
#define LEFT_VAL(p)     (LEFT_CHILD(p)->val)
#define RIGHT_VAL(p)    (RIGHT_CHILD(p)->val)
#define STATE_LABEL(p)  ((p)->state)
#define PANIC           printf
#define NODEPTR_TYPE    treenodep


treenode* code_op(int, treenode*, treenode*);
treenode* code_num(int);

void code_ret(int);
void code_func(char *);
void code_add(char *, char *);
void code_mult(char *, char *);
void code_minus(char *, char *);
void code_or(char *, char *);
void code_not(char *, char *);
void code_less(char *, char *);
void code_eq(char *, char *);

#endif
