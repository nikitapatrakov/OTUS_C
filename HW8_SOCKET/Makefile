CFLAGS = -Wall -g `pkg-config --cflags libconfig`

LDFLAGS = `pkg-config --libs libconfig`

EXEC =  my_prog

SRC =  main.c

OBJ =  main.o

$(EXEC): $(OBJ)
	 gcc -o $@ $^ $(CFLAGS) $(LDFLAGS)

%.o: %.c
	 gcc -c -o $@ $< $(CFLAGS)

.PHONY: clean

clean:
	 rm -f $(OBJ) $(EXEC) 