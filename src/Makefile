IDIR =../include
CC=gcc
CFLAGS=-I$(IDIR) -std=c99 -Wall -O2 `pkg-config --cflags --libs gtk+-3.0`
CFLAGS_TEST=$(CFLAGS) -g


ODIR=obj
LDIR =../lib

LIBS=-lm `pkg-config --libs gtk+-3.0`

_DEPS = sysinfo.h polar_rings.h clock.h battery.h cpu.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = sysinfo.o polar_rings.o clock.o battery.o cpu.o main.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

lot: $(OBJ)
	gcc -o $@ $^ $(CFLAGS) $(LIBS)

test: $(OBJ)
	gcc -o $@ $^ $(CFLAGS_TEST) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ lot $(INCDIR)/*~
