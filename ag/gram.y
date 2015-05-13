@attributes {char *name;} IDENT
@attributes {int val;} NUM


@traversal @postorder t

%{
    #include <stdio.h>
    #include <stdlib.h>
    
    #define YYDEBUG 1

    void yyerror(const char *s);
    
    int error_count = 0;

%}

%union {
    double val;
    const char *name;
} 

%start program
%token IDENT NUM
%token FUN IF THEN ELSE LET IN NOT HEAD TAIL AND END ISNUM ISLIST ISFUN ASSIGN
%token SEMICOLON EQUALS PLUS MINUS MULTIPLICATION POINT LESSER BRACKET_LEFT BRACKET_RIGHT
%right EQUALS

%%

program : 
        | program def ';'
        ;

        

def     : IDENT '=' lambda
        ;


lambda  : FUN IDENT ASSIGN expr END
        ;  


expr    : IF expr THEN expr ELSE expr END
        | lambda
        | LET IDENT '=' expr IN expr END
        | term
        | builtins term
        | plus term
        | minus term
        | mult term
        | and term
        | dot term
        | lower term
        | equals term 
        | expr term
        ;

plus    : term '+'
        | plus term '+'
        ;

minus   : term '-'
        ;

mult    : term '*'
        | mult term '*'
        ;

and     : term AND 
        | and term AND
        ;

dot     : term '.' 
        | dot term '.'
        ;

lower   : term '<'
        ;

equals  : term '='
        ;

builtins : builtin
         | builtins builtin
         ;

builtin : NOT 
        | HEAD 
        | TAIL 
        | ISNUM 
        | ISLIST 
        | ISFUN 
        ;

term    : '(' expr ')'
        | NUM
        | IDENT
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
