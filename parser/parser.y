%{


%}


%locations

%start program
%token <name> IDENT
%token <val> NUM
%token FUN IF THEN ELSE LET IN NOT HEAD TAIL AND END ISNUM ISFUN   
%right '='

%%

program : 
        | program funcdef ';'
        ;

term    : '(' expr ')'
        ;
        

def     : IDENT '=' lambda
        ;


lambda  : FUN IDENT '->' expr END
        ;  


expr    : IF expr THEN expr ELSE expr END
        | lambda
        | LET IDENT '=' expr IN expr END
        | term
        | builtin
        | expr term
        ;

builtin : NOT term
        | HEAD term
        | TAIL term
        | ISNUM term
        | ISLIST term
        | ISFUN term
        ;


%% 

void yyerror(const char *s) {
    fprintf(stderr, "%d: Error: %s\n", yyloc.first_line, s);
    error_count++;
}

int main(void) {
    yyparse();
    if (errcount > 0) {
        return 2;
    }
    return 0; 
}
