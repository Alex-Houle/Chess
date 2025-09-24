CC = gcc
CFLAGS = -Wall -Wextra -std=c11 `sdl2-config --cflags`
LDFLAGS = `sdl2-config --libs` -lSDL2_image -lSDL2_ttf

SRC = src/main.c src/board.c src/rules.c src/sdl_utils.c
OBJ = $(SRC:.c=.o)
EXEC = chess

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $@ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(EXEC) src/*.o