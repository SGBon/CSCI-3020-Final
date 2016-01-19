CC = clang
CFLAGS = -Wall -std=c99
LFLAGS =
LIBS =
SOURCES = hostd.c queue.c utility.c
OBJECTS = $(subst .c,.o,$(SOURCES))
EXE = hostd.exe
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
