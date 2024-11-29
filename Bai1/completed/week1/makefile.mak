TARGET = exercise1

SRC = $(TARGET).c
OUT = $(TARGET).exe

CC = gcc
CFLAGS = -Wall -g

$(OUT): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(OUT)
	./$(OUT)

clean:
	rm -f $(OUT)
