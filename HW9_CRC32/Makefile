CFLAGS = -Wall -Wextra -Wpedantic -std=c11

EXEC =  my_prog

SRC =  main.c

OBJ =  main.o

$(EXEC): $(OBJ)
	 gcc -o $@ $^ $(CFLAGS)

%.o: %.c
	 gcc -c -o $@ $< $(CFLAGS)

.PHONY: clean

clean:
	 rm -f $(OBJ) $(EXEC) 