%{
    #include "parser.tab.h"
    #define YY_USER_ACTION yylloc.first_line = yylloc.last_line = yylineno;
%}

%option yylineno

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
lexem           ([;=+\-*.<()])

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

{lexem}                 return yytext[0];
{hex_num}               return NUM; 
{dec_num}               return NUM; 
{identifier}            return IDENT;
{comment}               ;
{whitespace}+           ;
.                       { fprintf(stderr, "%d: Error: %s\n",yylloc.first_line, yytext); exit(1); }
