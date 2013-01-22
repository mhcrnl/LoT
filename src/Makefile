IDIR =../include
CC=gcc
CFLAGS=-I$(IDIR) -std=c99 -Wall -g

ODIR=obj
LDIR =../lib

LIBS=-lm

_DEPS = sysinfo.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = sysinfo.o main.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

lot: $(OBJ)
	gcc -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~
