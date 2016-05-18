YACC = bison
FLEX = flex
OX = ox
RM = rm
GCC = gcc 
SSH = ssh
RSYNC = rsync

TARGET = codea
SOURCE = Makefile scan.l gram.y symbol_table.c symbol_table.h

HOST = ueb
REMOTEDIR = abgabe/$(TARGET)
REMOTETEST = /usr/ftp/pub/ublu/test/$(TARGET)/test

RFLAGS = --chmod=o-rwxs,Dug+x,u+rw,g+r -e "$(SSH)"
YFLAGS = -dv 
GCCFLAGS = -std=c99 -Wall -Wextra -g -D_XOPEN_SOURCE=500 
IGNOREWARNINGS = -Wno-unused-variable -Wno-format -Wno-parentheses -Wno-unused-function
LDFLAGS = -lfl

all: $(TARGET)

$(TARGET): lex.yy.c oxout.tab.c symbol_table.c 
	$(GCC) $(GCCFLAGS) $(IGNOREWARNINGS) -o $@ $^ $(LDFLAGS) 

oxout.y oxout.l: gram.y scan.l
	$(OX) $^

oxout.tab.c oxout.tab.h: oxout.y
	$(YACC) $(YFLAGS) $<

lex.yy.c: oxout.l
	$(FLEX) $<

hand-in: $(SOURCE)
	$(RSYNC) $(RFLAGS) $(SOURCE) $(HOST):$(REMOTEDIR)
	$(SSH) $(HOST) 'cd $(REMOTEDIR); $(REMOTETEST) | egrep "(Test|Error)"'

clean: 
	rm -f $(TARGET) oxout.l oxout.y oxout.tab.c oxout.tab.h lex.yy.c
	
