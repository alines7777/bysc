PROJECT = bysc

CC = cc
CC_OBJECT_FLAGS = -ansi -pedantic -Wall -c
CC_LINK_FLAGS = -pedantic -Wall
CC_OPTIMIZE_FLAGS = -O2
CC_OPT_MORE_FLAGS = -O3
CC_FAST_FLAGS = -Ofast
CC_OPT_SMALL_FLAGS = -Os
CC_SMALL_FLAGS = -Oz
ROOT = /usr/local

MAINC = main.c
MAINO = main.o

FILEC = file.c
FILEO = file.o

COMMANDC = command.c
COMMANDO = command.o

ERRORC = error.c
ERRORO = error.o

all: $(PROJECT)

$(PROJECT): $(MAINO) $(FILEO) $(COMMANDO) $(ERRORO)
	$(CC) $(CC_OPT_MORE_FLAGS) $(CC_LINK_FLAGS) $(MAINO) $(FILEO) $(COMMANDO) $(ERRORO) -o $(PROJECT)

$(MAINO): $(MAINC)
	$(CC) $(CC_OPT_MORE_FLAGS) $(CC_OBJECT_FLAGS) $(MAINC) -o $(MAINO)

$(FILEO): $(FILEC)
	$(CC) $(CC_OPT_MORE_FLAGS) $(CC_OBJECT_FLAGS) $(FILEC) -o $(FILEO)

$(COMMANDO): $(COMMANDC)
	$(CC) $(CC_OPT_MORE_FLAGS) $(CC_OBJECT_FLAGS) $(COMMANDC) -o $(COMMANDO)

$(ERRORO): $(ERRORC)
	$(CC) $(CC_OPT_MORE_FLAGS) $(CC_OBJECT_FLAGS) $(ERRORC) -o $(ERRORO)

install: $(PROJECT)
	cp ./$(PROJECT) $(ROOT)/bin

clean:
	rm -f *.o

purge:
	rm -f *.o $(PROJECT)

full_poobrain:
	rm -f *.o $(PROJECT) *.c
