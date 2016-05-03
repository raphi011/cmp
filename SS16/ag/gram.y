@attributes { char *name; } ID
@attributes { int val; } NUM
@attributes { struct symbol* vars; } stats pars dostat expr exprs lexpr term guarded guardeds or mult plus
@attributes { struct symbol* vars; struct symbol* vars_new; } stat 

@traversal @postorder t

%{
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include "symbol_table.h"
    
    #define YYDEBUG 1

    int yylex();
    void yyerror(const char *s);
    
    int error_count = 0;

%}

%union {
    double val;
    const char *name;
}


%start program
%token <name> ID
%token <val> NUM
%token END RETURN VAR DO CONTINUE BREAK OR NOT ASSIGN GUARD
%token SEMICOLON BRACKET_LEFT BRACKET_RIGHT COMMA COLON MULTIPLICATION PLUS LOWER EQUALS MINUS CIRCUMFLEX
%right ASSIGN

%%

program : 
        | program funcdef SEMICOLON
        ;

funcdef : ID BRACKET_LEFT pars BRACKET_RIGHT stats END
        @{  @i @stats.vars@ = @pars.vars@; @}
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
        | stat SEMICOLON stats    
        @{
            @i @stat.vars@ = @stats.0.vars@;
            @i @stats.1.vars@ = @stat.vars_new@;
        @}
        ;

stat    : RETURN expr
        @{
            @i @expr.vars@ = @stat.vars@;
            @i @stat.vars_new@ = @stat.vars@;
        @} 
        | dostat
        @{
            @i @dostat.vars@ = @stat.vars@;
            @i @stat.vars_new@ = @stat.vars@;
        @}
        | VAR ID ASSIGN expr
        @{
            @i @expr.vars@ = @stat.vars@;
            @i @stat.vars_new@ = symbol_table_add(@stat.vars@, @ID.name@, variable);
        @}
        | lexpr ASSIGN expr
        @{
            @i @lexpr.vars@ = @stat.vars@;
            @i @expr.vars@ = @stat.vars@;
            @i @stat.vars_new@ = @stat.vars@;
        @}
        | term
        @{
            @i @term.vars@ = @stat.vars@;
            @i @stat.vars_new@ = @stat.vars@;
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
        | guarded SEMICOLON
        @{
            @i @guarded.vars@ = @guardeds.vars@;
        @}
        | guardeds guarded SEMICOLON
        @{
            @i @guardeds.1.vars@ = @guardeds.0.vars@;
            @i @guarded.vars@ = @guardeds.1.vars@;
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

expr    : term
        @{ @i  @term.vars@ = @expr.vars@; @}
        | unary term
        @{ @i  @term.vars@ = @expr.vars@; @}
        | term CIRCUMFLEX 
        @{ @i  @term.vars@ = @expr.vars@; @}
        | plus term
        @{ 
            @i  @plus.vars@ = @expr.vars@;
            @i  @term.vars@ = @expr.vars@; 
        @}
        | mult term
        @{ 
            @i  @mult.vars@ = @expr.vars@;
            @i  @term.vars@ = @expr.vars@; 
        @}
        | or term
        @{ 
            @i  @or.vars@ = @expr.vars@;
            @i  @term.vars@ = @expr.vars@; 
        @}
        | term LOWER term
        @{
            @i  @term.0.vars@ = @expr.vars@; 
            @i  @term.1.vars@ = @expr.vars@; 
        @}
        | term EQUALS term
        @{
            @i  @term.0.vars@ = @expr.vars@; 
            @i  @term.1.vars@ = @expr.vars@; 
        @}
        ;

exprs   : 
        | expr
        @{ @i  @expr.vars@ = @exprs.vars@; @}
        | expr COMMA exprs  
        @{
            @i  @expr.vars@ = @exprs.0.vars@; 
            @i  @exprs.1.vars@ = @exprs.0.vars@; 
        @}
        ;

unary   : MINUS 
        | MINUS unary
        | NOT
        | NOT unary
        ;


or      : term OR
        @{ @i  @term.vars@ = @or.vars@; @}
        | or term OR
        @{
            @i  @or.1.vars@ = @or.0.vars@; 
            @i  @term.vars@ = @or.0.vars@; 
        @}
        ;

mult    : term MULTIPLICATION 
        @{ @i  @term.vars@ = @mult.vars@; @}
        | mult term MULTIPLICATION 
        @{
            @i  @mult.1.vars@ = @mult.0.vars@; 
            @i  @term.vars@ = @mult.0.vars@; 
        @}
        ;

plus    : term PLUS 
        @{ @i  @term.vars@ = @plus.vars@; @}
        | plus term PLUS 
        @{
            @i  @plus.1.vars@ = @plus.0.vars@; 
            @i  @term.vars@ = @plus.0.vars@; 
        @}
        ;

term    : BRACKET_LEFT expr BRACKET_RIGHT
        @{ @i  @expr.vars@ = @term.vars@; @}
        | NUM
        | ID
        @{ @t  if (!symbol_table_exists_type(@term.vars@, @ID.name@, variable)) exit(EXIT_ERROR); @}
        | ID BRACKET_LEFT exprs BRACKET_RIGHT 
        @{ @i  @exprs.vars@ = @term.vars@; @}
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
