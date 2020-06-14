EXEC=Projet.exe 
SOURCES=  afficherCarte.c  
OBJECTS=$(SOURCES:.c=.o)
CC=gcc 
CFLAGS=-W -Wall -std=gnu99 -g
 
.PHONY: default clean
 
default: $(EXEC)
 
afficherCarte.o:afficherCarte.c couleur.h

 
%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)
 
$(EXEC): $(OBJECTS)
	$(CC) -o $@ $^ -lm
 
clean:
	rm -rf $(EXEC) $(OBJECTS) $(SOURCES:.c=.c~) $(SOURCES:.c=.h~) Makefile~
