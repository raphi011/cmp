@attributes {char *name;} IDENT
@attributes {int val;} NUM
@attributes {struct symbol* symbols;} lambda expr term plus minus mult and dot lower equals 
@attributes {struct symbol* globals;} program_start
@attributes {struct symbol* globals; struct symbol* globals_down; } program
@attributes {char *global_variable; struct symbol* symbols;} def

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

%start program_start

%token IDENT NUM
%token FUN IF THEN ELSE LET IN NOT HEAD TAIL AND END ISNUM ISLIST ISFUN ASSIGN
%token SEMICOLON EQUALS PLUS MINUS MULTIPLICATION DOT LOWER BRACKET_LEFT BRACKET_RIGHT

%right EQUALS

%%

program_start : program
        @{
            @i @program_start.globals@ = @program.globals@;
            @i @program.globals_down@ = @program_start.globals@;
        @}

program : 
        @{ @i @program.globals@ = NULL; @} 
        | program def SEMICOLON
        @{
            @i @program.0.globals@ = symbol_table_add(@program.1.globals@, @def.global_variable@);
            @i @program.1.globals_down@ = @program.0.globals_down@;
            @i @def.symbols@ = @program.0.globals_down@; 
        @}
        ;

def     : IDENT EQUALS lambda
        @{
            @i @def.global_variable@ = @IDENT.name@; 
            @i @lambda.symbols@ = @def.symbols@; 
        @} 
        ;

lambda  : FUN IDENT ASSIGN expr END
        @{ @i @expr.symbols@ = symbol_table_add(@lambda.symbols@, @IDENT.name@); @}
        ;  

expr    : IF expr THEN expr ELSE expr END
        @{
            @i @expr.1.symbols@ = @expr.0.symbols@;
            @i @expr.2.symbols@ = @expr.0.symbols@;
            @i @expr.3.symbols@ = @expr.0.symbols@;
        @}
        | lambda
        @{ @i @lambda.symbols@ = @expr.symbols@; @}
        | LET IDENT EQUALS expr IN expr END
        @{ 
            @i @expr.1.symbols@ = symbol_table_clone(@expr.0.symbols@);
            @i @expr.2.symbols@ = symbol_table_add(@expr.0.symbols@, @IDENT.name@); 
        @}
        | term
        @{  @i @term.symbols@ = @expr.symbols@; @}
        | builtins term
        @{  @i @term.symbols@ = @expr.symbols@; @}
        | plus term
        @{
            @i @term.symbols@ = @expr.symbols@; 
            @i @plus.symbols@ = @expr.symbols@; 
        @}
        | minus term
        @{
            @i @term.symbols@ = @expr.symbols@; 
            @i @minus.symbols@ = @expr.symbols@; 
        @}
        | mult term
        @{
            @i @term.symbols@ = @expr.symbols@; 
            @i @mult.symbols@ = @expr.symbols@; 
        @}
        | and term
        @{
            @i @term.symbols@ = @expr.symbols@; 
            @i @and.symbols@ = @expr.symbols@; 
        @}
        | dot term
        @{
            @i @term.symbols@ = @expr.symbols@; 
            @i @dot.symbols@ = @expr.symbols@; 
        @}
        | lower term
        @{
            @i @term.symbols@ = @expr.symbols@; 
            @i @lower.symbols@ = @expr.symbols@; 
        @}
        | equals term 
        @{
            @i @term.symbols@ = @expr.symbols@; 
            @i @equals.symbols@ = @expr.symbols@; 
        @}
        | expr term
        @{
            @i @term.symbols@ = @expr.symbols@; 
            @i @expr.1.symbols@ = @expr.0.symbols@; 
        @}
        ;

plus    : term PLUS
        @{  @i @term.symbols@ = @plus.symbols@; @}
        | plus term PLUS
        @{  
            @i @plus.1.symbols@ = @plus.0.symbols@; 
            @i @term.symbols@ = @plus.0.symbols@; 
        @}
        ;

minus   : term MINUS
        @{  @i @term.symbols@ = @minus.symbols@; @}
        ;

mult    : term MULTIPLICATION
        @{  @i @term.symbols@ = @mult.symbols@; @}
        | mult term MULTIPLICATION
        @{  
            @i @mult.1.symbols@ = @mult.0.symbols@; 
            @i @term.symbols@ = @mult.0.symbols@; 
        @}
        ;

and     : term AND 
        @{  @i @term.symbols@ = @and.symbols@; @}
        | and term AND
        @{  
            @i @and.1.symbols@ = @and.0.symbols@; 
            @i @term.symbols@ = @and.0.symbols@; 
        @}
        ;

dot     : term DOT 
        @{  @i @term.symbols@ = @dot.symbols@; @}
        | dot term DOT
        @{  
            @i @dot.1.symbols@ = @dot.0.symbols@; 
            @i @term.symbols@ = @dot.0.symbols@;
        @}
        ;

lower   : term LOWER 
        @{  @i @term.symbols@ = @lower.symbols@; @}
        ;

equals  : term EQUALS
        @{  @i @term.symbols@ = @equals.symbols@; @}
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

term    : BRACKET_LEFT expr BRACKET_RIGHT
        @{ @i @expr.symbols@ = @term.symbols@; @}
        | NUM
        | IDENT
        @{ @t if (!symbol_table_exists(@term.symbols@, @IDENT.name@)) exit(EXIT_ERROR); @}
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
