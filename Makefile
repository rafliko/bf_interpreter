CC = gcc
CFLAGS = -Wall

all: compile

debug: CFLAGS += -DDEBUG -g
debug: compile

compile:
	$(CC) $(CFLAGS) bf.c -o bf
