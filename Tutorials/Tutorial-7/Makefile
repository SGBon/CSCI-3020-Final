CC = clang
CFLAGS = -Wall -Wextra -std=c99
LFLAGS =
LIBS =
SOURCES = Q5.c
OBJECTS = $(subst .c,.o,$(SOURCES))
EXE = Q5.exe
.PHONY: clean help

Q5.exe : $(OBJECTS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

%.o : %.c
	$(CC) $(CFLAGS) -c $<

all : $(EXE)

clean:
	rm -f $(OBJECTS) $(EXE) *~

cleanup:
	rm -f $(OBJECTS) *~

help:
	@echo "Valid targets:"
	@echo "  all:    generates all binary files"
	@echo "  clean:  removes .o and .exe files"
