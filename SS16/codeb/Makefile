YACC = bison
FLEX = flex
OX = ox
BURG = iburg
BFE = bfe
RM = rm
GCC = gcc 
SSH = ssh
RSYNC = rsync

TARGET = codeb
SOURCE = Makefile scan.l gram.y symbol_table.c symbol_table.h code.c code.h codeb.bfe regs.c regs.h asm.c asm.h

HOST = ueb
REMOTEDIR = abgabe/$(TARGET)
REMOTETEST = /usr/ftp/pub/ublu/test/$(TARGET)/test

RFLAGS = --chmod=o-rwxs,Dug+x,u+rw,g+r -e "$(SSH)"
YFLAGS = -d
GCCFLAGS = -std=c99 -Wall -Wextra -g -D_XOPEN_SOURCE=500 
IGNOREWARNINGS = -Wno-unused-variable -Wno-format -Wno-parentheses -Wno-unused-function -Wno-unused-parameter
LDFLAGS = -lfl

all: $(TARGET)

$(TARGET): oxout.tab.c lex.yy.c symbol_table.c symbol_table.h code.c code.h $(TARGET)_iburg.c asm.c asm.h
	$(GCC) $(GCCFLAGS) $(IGNOREWARNINGS) *.c $(LDFLAGS) -o $@  

oxout.y oxout.l: gram.y scan.l
	$(OX) $^

oxout.tab.c oxout.tab.h: oxout.y
	$(YACC) $(YFLAGS) $<

lex.yy.c: oxout.l
	$(FLEX) $<

$(TARGET)_iburg.c: $(TARGET).bfe
	$(BFE) < $< | $(BURG) > $@

hand-in: $(SOURCE)
	$(RSYNC) $(RFLAGS) $(SOURCE) $(HOST):$(REMOTEDIR)
	$(SSH) $(HOST) 'cd $(REMOTEDIR); $(REMOTETEST) | egrep "(Test|Error)"'

clean: 
	rm -f $(TARGET) oxout.l oxout.y $(TARGET)_iburg.c oxout.tab.c oxout.tab.h lex.yy.c
