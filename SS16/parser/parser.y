%{
    #include <stdio.h>
    #include <stdlib.h>
    
    #define YYDEBUG 1

    int yylex();
    void yyerror(const char *s);
    
    int error_count = 0;

%}

%union {
    double val;
    const char *name;
}

%locations

%start program
%token <name> ID
%token <val> NUM
%token END RETURN VAR DO CONTINUE BREAK OR NOT ASSIGN GUARD
%right ASSIGN

%%

program : 
        | program funcdef ';'
        ;

funcdef : ID '(' pars ')' stats END
        ;

pars    : 
        | ID
        | ID ',' pars
        ;

stats   :
        | stat ';'
        | stats stat ';'
        ;

stat    : RETURN expr
        | dostat
        | VAR ID ASSIGN expr
        | lexpr ASSIGN expr
        | term
        ;

dostat  : ID ':' DO guardeds END
        | DO guardeds END
        ;

guardeds:
        | guarded ';'
        | guardeds guarded ';'
        ;

guarded : expr GUARD stats CONTINUE
        | expr GUARD stats CONTINUE ID
        | expr GUARD stats BREAK 
        | expr GUARD stats BREAK ID
        ;

lexpr   : ID
        | term '^'
        ;

expr    : term
        | unary term
        | term '^' 
        | plus term
        | mult term
        | or term
        | term '<' term
        | term '=' term
        ;

exprs   : 
        | expr
        | expr ',' exprs  
        ;

unary   : '-'
        | '-' unary
        | NOT
        | NOT unary
        ;


or      : term OR
        | or term OR
        ;

mult    : term '*'
        | mult term '*'
        ;

plus    : term '+'
        | plus term '+'
        ;

term    : '(' expr ')'
        | NUM
        | ID
        | ID '(' exprs ')'
        ;
 
%% 

void yyerror(const char *s) {
    fprintf(stderr, "%d,%d: parser error: %s\n", yylloc.first_line, yylloc.first_column, s);
    error_count++;
}

int main(void) {
    yyparse();
    if (error_count > 0) {
        return 2;
    }
    return 0; 
}
