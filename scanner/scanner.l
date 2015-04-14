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
keyword         ("fun"|"if"|"then"|"else"|"let"|"in"|"not"|"head"|"tail"|"and"|"end"|"isnum")
lexem           ("->"|[;=+\-*.<()])

whitespace      [ \t\n]

    int error_count = 0;

%%


{comment}               ;
{keyword}               |
{lexem}                 printf("%s\n", yytext);
{hex_num}               printf("num %d\n",strtol(yytext+1,NULL,16));
{dec_num}               printf("num %d\n",strtol(yytext,NULL,10));
{identifier}            printf("ident %s\n", yytext);
{whitespace}            ;
.                       fprintf(stderr, "Error: %s\n", yytext); error_count++;

%%

int main(void) {
    yylex();
    if (error_count) {
        fprintf(stderr, "%d errors occured.\n", error_count);
        return 1;
    }
    return 0;
}
