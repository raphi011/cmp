%{
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>

    #include "symbol_table.h"
    #include "code_gen.h"
    
    #define YYDEBUG 1

    int yylex();
    void yyerror(const char *s);
    
    void burm_reduce(NODEPTR_TYPE, int);
    void burm_label(NODEPTR_TYPE);

    int error_count = 0;

%}

%start program
%token END RETURN VAR DO CONTINUE BREAK OR NOT MINUS ASSIGN GUARD
%token SEMICOLON BRACKET_LEFT BRACKET_RIGHT COMMA COLON MULTIPLICATION PLUS LOWER EQUALS CIRCUMFLEX
%right ASSIGN
%token <val> NUM
%token <name> ID

@attributes { char *name; } ID
@attributes { int val; } NUM
@attributes { treenode *node; } funcdef
@attributes { struct symbol* vars; } pars dostat exprs lexpr guarded guardeds
@attributes { struct symbol* vars; treenode *node; } stats expr plus term unary or mult
@attributes { struct symbol* vars; treenode *node; struct symbol* vars_new; } stat 

@traversal @preorder t
@traversal @preorder cmp

%union {
    double val;
    const char *name;
}

%%

program : funcdefs
        @{
            @cmp printf(".text\n");
        @}
        ;

funcdefs: funcdef funcdefs
        |
        ;

funcdef : ID BRACKET_LEFT pars BRACKET_RIGHT stats END SEMICOLON
        @{  
            @i @stats.vars@ = code_init_pars(@pars.vars@); 
            @i @funcdef.node@ = code_op(C_FUNC, @stats.node@, NULL);

            @cmp code_init_vars(@pars.vars@);
            @cmp code_func(@ID.name@);
        @}
        ;

pars    : 
        @{  @i @pars.vars@ = NULL; @}
        | ID
        @{
            @i @pars.vars@ = symbol_table_add(NULL, @ID.name@, variable);
        @}
        | ID COMMA pars
        @{
            @i @pars.0.vars@ = symbol_table_add(@pars.1.vars@, @ID.name@, variable);
        @}
        ;

stats   :
        @{
            @i @stats.node@ = NULL;
        @}
        | stat SEMICOLON stats    
        @{
            @i @stat.vars@ = @stats.0.vars@;
            @i @stats.1.vars@ = @stat.vars_new@;

            @i @stats.node@ = @stat.node@;

            @cmp burm_label(@stat.node@); burm_reduce(@stat.node@, 1);
        @}
        ;

stat    : RETURN expr
        @{
            @i @expr.vars@ = @stat.vars@;
            @i @stat.vars_new@ = @stat.vars@;

            @i @stat.node@ = code_op(C_RET, @expr.node@, NULL);
        @} 
        | dostat
        @{
            @i @dostat.vars@ = @stat.vars@;
            @i @stat.vars_new@ = @stat.vars@;

            @i @stat.node@ = NULL;
        @}
        | VAR ID ASSIGN expr
        @{
            @i @expr.vars@ = @stat.vars@;
            @i @stat.vars_new@ = symbol_table_add(@stat.vars@, @ID.name@, variable);

            @i @stat.node@ = NULL;
        @}
        | lexpr ASSIGN expr
        @{
            @i @lexpr.vars@ = @stat.vars@;
            @i @expr.vars@ = @stat.vars@;
            @i @stat.vars_new@ = @stat.vars@;

            @i @stat.node@ = NULL;
        @}
        | term
        @{
            @i @term.vars@ = @stat.vars@;
            @i @stat.vars_new@ = @stat.vars@;

            @i @stat.node@ = NULL;
        @}
        ;

dostat  : ID COLON DO guardeds END
        @{
            @i @guardeds.vars@ = symbol_table_add(@dostat.vars@, @ID.name@, label);
        @}
        | DO guardeds END
        @{
            @i @guardeds.vars@ = @dostat.vars@;
        @}
        ;

guardeds:
        | guarded SEMICOLON guardeds 
        @{
            @i @guarded.vars@ = @guardeds.0.vars@;
            @i @guardeds.1.vars@ = @guarded.0.vars@;
        @}
        ;

guarded : expr GUARD stats CONTINUE
        @{
            @i  @expr.vars@ = @guarded.vars@;
            @i  @stats.vars@ = @guarded.vars@;
        @}
        | expr GUARD stats CONTINUE ID
        @{
            @t  if (!symbol_table_exists_type(@guarded.vars@, @ID.name@, label)) exit(EXIT_ERROR); 
            @i  @expr.vars@ = @guarded.vars@;
            @i  @stats.vars@ = @guarded.vars@;
        @}
        | expr GUARD stats BREAK 
        @{
            @i  @expr.vars@ = @guarded.vars@;
            @i  @stats.vars@ = @guarded.vars@;
        @}
        | expr GUARD stats BREAK ID
        @{
            @t  if (!symbol_table_exists_type(@guarded.vars@, @ID.name@, label)) exit(EXIT_ERROR); 
            @i  @expr.vars@ = @guarded.vars@;
            @i  @stats.vars@ = @guarded.vars@;
        @}
        ;

lexpr   : ID
        @{ @t  if (!symbol_table_exists_type(@lexpr.vars@, @ID.name@, variable)) exit(EXIT_ERROR); @}
        | term CIRCUMFLEX
        @{ @i  @term.vars@ = @lexpr.vars@; @}
        ;

 expr   : unary
        @{ 
            @i @unary.vars@ = @expr.vars@; 
            
            @i @expr.node@ = @unary.node@;
        @}
        | term CIRCUMFLEX 
        @{ 
            @i  @term.vars@ = @expr.vars@; 
        
            @i @expr.node@ = code_op(C_MEM, @term.node@, NULL);
        @}
        | plus
        @{ 
            @i  @plus.vars@ = @expr.vars@; 

            @i @expr.node@ = @plus.node@;
        @}
        | mult
        @{ 
            @i  @mult.vars@ = @expr.vars@; 
        
            @i @expr.node@ = @mult.node@;
        @}
        | or
        @{ 
            @i  @or.vars@ = @expr.vars@; 
        
            @i @expr.node@ = @or.node@;
        @}
        | term LOWER term
        @{
            @i  @term.0.vars@ = @expr.vars@; 
            @i  @term.1.vars@ = @expr.vars@; 

            @i @expr.node@ = code_op(C_LESS, @term.0.node@, @term.1.node@);
        @}
        | term EQUALS term
        @{
            @i  @term.0.vars@ = @expr.vars@; 
            @i  @term.1.vars@ = @expr.vars@; 

            @i @expr.node@ = code_op(C_EQ, @term.0.node@, @term.1.node@);
        @}
        ;

exprs   : 
        | expr COMMA exprs  
        @{
            @i  @expr.vars@ = @exprs.0.vars@; 
            @i  @exprs.1.vars@ = @exprs.0.vars@; 
        @}
        | expr
        @{ @i  @expr.vars@ = @exprs.vars@; @}
        ;

unary   : MINUS unary
        @{ 
            @i  @unary.1.vars@ = @unary.0.vars@; 

            @i  @unary.0.node@ = code_op(C_MINUS, @unary.1.node@, NULL);
        @}
        | NOT unary
        @{ 
            @i  @unary.1.vars@ = @unary.0.vars@; 
            
            @i  @unary.0.node@ = code_op(C_NOT, @unary.1.node@, NULL);
        @}
        | term
        @{ 
            @i  @term.vars@ = @unary.vars@; 

            @i  @unary.node@ = @term.node@;
        @}
        ;


or      : term OR term
        @{ 
            @i  @term.0.vars@ = @or.vars@; 
            @i  @term.1.vars@ = @term.0.vars@; 

            @i @or.node@ = code_op(C_OR, @term.0.node@, @term.1.node@);
        @}
        | or OR term
        @{
            @i  @or.1.vars@ = @or.0.vars@; 
            @i  @term.vars@ = @or.1.vars@; 

            @i @or.0.node@ = code_op(C_OR, @or.1.node@, @term.node@);
        @}
        ;

mult    : term MULTIPLICATION term
        @{ 
            @i  @term.0.vars@ = @mult.vars@; 
            @i  @term.1.vars@ = @term.0.vars@; 

            @i @mult.node@ = code_op(C_MULT, @term.0.node@, @term.1.node@);
        @}
        | mult MULTIPLICATION term
        @{
            @i  @mult.1.vars@ = @mult.0.vars@; 
            @i  @term.vars@ = @mult.1.vars@; 

            @i @mult.0.node@ = code_op(C_MULT, @mult.1.node@, @term.node@);
        @}
        ;

plus    : term PLUS term
        @{ 
            @i  @term.0.vars@ = @plus.vars@; 
            @i  @term.1.vars@ = @term.0.vars@; 

            @i @plus.node@ = code_op(C_ADD, @term.0.node@, @term.1.node@);
        @}
        | plus PLUS term
        @{
            @i  @plus.1.vars@ = @plus.0.vars@; 
            @i  @term.vars@ = @plus.1.vars@; 

            @i @plus.0.node@ = code_op(C_ADD, @plus.1.node@, @term.node@);
        @}
        ;

term    : BRACKET_LEFT expr BRACKET_RIGHT
        @{ 
            @i @expr.vars@ = @term.vars@; 

            @i @term.node@ = @expr.node@;
        @}
        | NUM
        @{ 
            @i @term.node@ = code_num(@NUM.val@);
        @}
        | ID
        @{
            
            @i @term.node@ = code_id (@ID.name@, symbol_table_get (@term.vars@, @ID.name@));
        @}
        | ID BRACKET_LEFT exprs BRACKET_RIGHT 
        @{ 
            @i @exprs.vars@ = @term.vars@; 

            @i @term.node@ = NULL;
        @}
        ;
 
%% 

void yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
    error_count++;
}

int main(void) {
    yyparse();
    if (error_count > 0) {
        return 2;
    }
    return 0; 
}
