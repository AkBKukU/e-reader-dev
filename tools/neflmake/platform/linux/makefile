SRCDIR = ../../source
DSTDIR = ./release

#SRC = main.c crc32.c strutil.c filefunc.c
OBJ = $(DSTDIR)/main.o $(DSTDIR)/crc32.o $(DSTDIR)/strutil.o $(DSTDIR)/filefunc.o
EXE = neflmake.exe

CC = gcc
RM = rm

CFLAGS = -O3
LFLAGS = -O3

all : $(EXE)

$(DSTDIR)/%.o : $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -o $@ -c $<

$(EXE) : $(OBJ)
	$(CC) $(OBJ) $(LFLAGS) -o $(DSTDIR)/$@

clean :
	-$(RM) $(OBJ)
