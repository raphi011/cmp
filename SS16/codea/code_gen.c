#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "code_gen.h"

treenode* code_op(int op, treenode *left, treenode *right) {
    treenode *node = (treenode *)malloc(sizeof(treenode));

    node->op = op;
    node->kids[0] = left;
    node->kids[1] = right;
    node->name = 0;
    node->val = 0;

    return node;
}

treenode* code_num(int num) {
    treenode *node = code_op(C_NUM, NULL, NULL);

    node->val = num;

    return node;
}


void code_func(char* name) {
    printf(".globl %1$s\n.type %1$s, @function\n%1$s:\n", name);
}

void code_ret(int val) {
    printf("\tmovq $%d, %%rax\n", val);
    printf("\tret\n");
}


void code_add(char *par1, char *par2) {

}

void code_mult(char *par1, char *par2) {


}

void code_sub(char *par1, char *par2) {

}

void code_or(char *par1, char *par2) {

}

void code_not(char *par1, char *par2) {


}

void code_less(char *par1, char *par2) {

}

void code_eq(char *par1, char *par2) {

}
