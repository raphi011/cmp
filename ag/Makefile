YACC = bison
FLEX = flex
OX = ox
RM = rm
GCC = gcc 

TARGET = ag
SOURCE = Makefile scan.l gram.y

YFLAGS = -d 
GCCFLAGS = -Wall -Wextra -pedantic -g 
LDFLAGS = -lfl

all: $(TARGET)

$(TARGET): lex.yy.c oxout.tab.c 
	$(GCC) $(GCCFLAGS) -o $@ $^ $(LDFLAGS) 

oxout.y oxout.l: gram.y scan.l
	$(OX) $^

oxout.tab.c oxout.tab.h: oxout.y
	$(YACC) -d $<

lex.yy.c: oxout.l
	$(FLEX) $<

clean: 
	rm -f $(TARGET) oxout.l oxout.y oxout.tab.c oxout.tab.h lex.yy.c
	
