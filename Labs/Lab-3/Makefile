CC = gcc
CFLAGS = -Wall -std=c99
LFLAGS = -pthread
LIBS = -lm
SOURCES = solver.c utility.c
OBJECTS = $(subst .c,.o,$(SOURCES))
EXE = solver.exe
.PHONY: clean help

%.exe : $(OBJECTS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

%.o : %.c
	$(CC) $(CFLAGS) -c $<

all : $(EXE)

clean:
	rm -f $(OBJECTS) $(EXE) *~

help:
	@echo "Valid targets:"
	@echo "  clean:  removes .o and .exe files"
