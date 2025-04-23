SRC = src
OBJ = main.o board.o rules.o

chess: $(OBJ)
	gcc -Wall -Wextra -g $(addprefix $(SRC)/, $(OBJ)) -o chess

main.o: $(SRC)/main.c $(SRC)/board.h $(SRC)/rules.h $(SRC)/types.h
	gcc -Wall -Wextra -g -c $(SRC)/main.c -o $(SRC)/main.o

board.o: $(SRC)/board.c $(SRC)/board.h $(SRC)/types.h
	gcc -Wall -Wextra -g -c $(SRC)/board.c -o $(SRC)/board.o

rules.o: $(SRC)/rules.c $(SRC)/rules.h $(SRC)/board.h $(SRC)/types.h
	gcc -Wall -Wextra -g -c $(SRC)/rules.c -o $(SRC)/rules.o

clean:
	rm -f $(SRC)/*.o chess
