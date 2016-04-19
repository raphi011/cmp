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
%token <name> IDENT
%token <val> NUM
%token END RETURN VAR DO CONTINUE BREAK OR NOT
%right '='

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
    fprintf(stderr, "%d: Error: %s\n", yylloc.first_line, s);
    error_count++;
}

int main(void) {
    yyparse();
    if (error_count > 0) {
        return 2;
    }
    return 0; 
}
