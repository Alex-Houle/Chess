# Makefile for compiling board.c

SRC_DIR = src
CC = gcc
CFLAGS = -Wall -Wextra -std=c11
TARGET = board
OBJ = $(SRC_DIR)/board.o $(SRC_DIR)/main.o $(SRC_DIR)/rules.o

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

$(SRC_DIR)/board.o: $(SRC_DIR)/board.c $(SRC_DIR)/board.h
	$(CC) $(CFLAGS) -c $< -o $@

$(SRC_DIR)/rules.o: $(SRC_DIR)/rules.c $(SRC_DIR)/rules.h
	$(CC) $(CFLAGS) -c $< -o $@



clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all clean
