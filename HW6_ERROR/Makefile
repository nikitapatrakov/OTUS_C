CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -std=c11 -g
AR = ar
ARFLAGS = rcs

LIB_NAME = liblog.a
LIB_OBJS = log.o
TEST_APP = main

.PHONY: all clean

all: $(LIB_NAME) $(TEST_APP)

$(LIB_NAME): $(LIB_OBJS)
	$(AR) $(ARFLAGS) $@ $^

$(TEST_APP): main.o $(LIB_NAME)
	$(CC) $(CFLAGS) -o $@ main.o -L. -llog -lpthread -rdynamic

main.o: main.c log.h
	$(CC) $(CFLAGS) -c main.c

log.o: log.c log.h
	$(CC) $(CFLAGS) -c log.c

clean:
	rm -f *.o $(LIB_NAME) $(TEST_APP) log.txt