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

hex_num         0x{hexdigit}+
dec_num         {digit}+

comment         "//".+

identifier      (_|{letter})(_|{letter}|{digit})*
lexem           ("->"|":="|[;(),:*+<=-^])

whitespace      [ \t\n]

%%

"end"                   return END;
"return"                return RETURN;
"var"                   return VAR;
"do"                    return DO;
"continue"              return CONTINUE;
"break"                 return BREAK;
"or"                    return OR;
"not"                   return NOT;

{lexem}                 return yytext[0];
{hex_num}               return NUM; 
{dec_num}               return NUM; 
{identifier}            return IDENT;
{comment}               ;
{whitespace}+           ;
.                       { fprintf(stderr, "%d: Error: %s\n",yylloc.first_line, yytext); exit(1); }
