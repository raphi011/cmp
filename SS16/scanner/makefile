
all: scanner.l
	flex -o scanner.c scanner.l
	gcc -o scanner scanner.c -lfl 

clean: 
	rm scanner.c scanner

