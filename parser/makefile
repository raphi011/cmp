
all: parser.l parser.y
	flex scanner.l
	bison -vd scanner.y
	gcc -o parser lex.yy.c -lfl 

clean: 
	rm -f lex.yy.c parser.tab.c parser.tab.h parser.output
