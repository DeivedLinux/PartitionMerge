OBJS = src/*.c
CC = gcc
OBJ_NAME = main
COMPILER_FLAGS = -Wall

all: $(OBJS)
	$(CC) $(OBJS)  -o $(OBJ_NAME) $(COMPILER_FLAGS)