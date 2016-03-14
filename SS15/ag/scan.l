%{
    #include "oxout.tab.h"
    #include <string.h>
    #include <stdlib.h>
%}


letter_lower    [a-z]
letter_upper    [A-Z]
letter          ({letter_lower}|{letter_upper})
digit           [0-9]
hexchar_lower   [a-f]
hexchar_upper   [A-F]
hexdigit        ({digit}|{hexchar_lower}|{hexchar_upper})

hex_num         ${hexdigit}+
dec_num         {digit}+

comment         "//".+

identifier      {letter}({letter}|{digit})*

whitespace      [ \t\n]

%%

"fun"                   return FUN;
"if"                    return IF;
"then"                  return THEN;
"else"                  return ELSE;
"let"                   return LET;
"in"                    return IN;
"not"                   return NOT;
"head"                  return HEAD;
"tail"                  return TAIL;
"and"                   return AND;
"end"                   return END;
"isnum"                 return ISNUM;
"islist"                return ISLIST;
"isfun"                 return ISFUN;
"->"                    return ASSIGN;

";"                     return SEMICOLON;
"="                     return EQUALS;
"+"                     return PLUS;
"-"                     return MINUS;
"*"                     return MULTIPLICATION;
"."                     return DOT;
"<"                     return LOWER;
"("                     return BRACKET_LEFT;
")"                     return BRACKET_RIGHT;

{hex_num}               return NUM; @{ @NUM.val@ = strtol(yytext+1,NULL,16); @}
{dec_num}               return NUM; @{ @NUM.val@ = strtol(yytext,NULL,10); @}
{identifier}            return IDENT; @{ @IDENT.name@ = strdup(yytext); @}
{comment}               ;
{whitespace}+           ;
.                       { fprintf(stderr, "Error: %s\n", yytext); exit(1); }
