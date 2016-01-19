CC = clang
CFLAGS = -Wall -std=c99 -lpthread
LFLAGS =
LIBS = 
SOURCES = Lab3.c
OBJECTS = $(subst .c,.o,$(SOURCES))
EXE = Lab3.exe
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
