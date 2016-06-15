%{
    #include "oxout.tab.h"
    #include <string.h>
    #include <stdlib.h>

    #include "code.h"
%}

%option yylineno

letter_lower    [a-z]
letter_upper    [A-Z]
letter          ({letter_lower}|{letter_upper})
digit           [0-9]
hexchar_lower   [a-f]
hexchar_upper   [A-F]
hexdigit        ({digit}|{hexchar_lower}|{hexchar_upper})

hex_num         0x{hexdigit}+
dec_num         {digit}+

comment         "//".+

identifier      (_|{letter})(_|{letter}|{digit})*

whitespace      [ \t\n]

%%

":="                    return ASSIGN;
"->"                    return GUARD;
"end"                   return END;
"return"                return RETURN;
"var"                   return VAR;
"do"                    return DO;
"continue"              return CONTINUE;
"break"                 return BREAK;
"or"                    return OR;
"not"                   return NOT;

";"                     return SEMICOLON;
"("                     return BRACKET_LEFT;
")"                     return BRACKET_RIGHT;
","                     return COMMA;
":"                     return COLON;
"*"                     return MULTIPLICATION;
"+"                     return PLUS;
"<"                     return LOWER;
"="                     return EQUALS;
"-"                     return MINUS;
"^"                     return CIRCUMFLEX;

{hex_num}               return NUM; @{ @NUM.val@ = strtol(yytext+2, NULL, 16); @}
{dec_num}               return NUM; @{ @NUM.val@ = strtol(yytext, NULL, 10); @}
{identifier}            return ID;  @{ @ID.name@ = strdup(yytext); @}
{comment}               ;
{whitespace}+           ;
.                       { fprintf(stderr, "Error: %s\n", yytext); exit(1); }
