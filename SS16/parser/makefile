
all: parser.l parser.y
	flex parser.l
	bison -vd parser.y
	gcc -o parser lex.yy.c parser.tab.c -lfl 

clean: 
	rm -f lex.yy.c parser.tab.c parser.tab.h parser.output
