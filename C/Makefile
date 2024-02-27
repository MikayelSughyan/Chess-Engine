# if you type 'make' without arguments, this is the default
PROG    = chess_engine
all:    $(PROG)

# Tell make about the file dependencies
HEAD	= headers.h
OBJ     = chess_engine.o init.o bitboard.o hashkeys.o board.o data.o attack.o io.o movegen.o validate.o makemove.o perft.o search.o pvtable.o evaluate.o misc.o uci.o xboard.o

# special libraries This can be blank
LIB     =

# select the compiler and flags
# you can over-ride these on the command line e.g. make DEBUG= 
CC      = gcc
DEBUG	= -ggdb
CSTD	=
WARN	= -Wall -Wextra #-Werror
CDEFS	=
CFLAGS	= -I. $(DEBUG) $(WARN) $(CSTD) $(CDEFS)

$(OBJ):	$(HEAD)

# specify how to compile the target
$(PROG):	$(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LIB) -o $@


# remove binaries
clean:
	rm -f $(OBJ) $(PROG)

#cl /W4 /EHsc chess_engine.c init.c bitboard.c hashkeys.c board.c data.c attack.c io.c movegen.c validate.c makemove.c perft.c search.c pvtable.c evaluate.c misc.c uci.c