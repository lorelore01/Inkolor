CC=gcc
CFLAGS=-Wall -O2
LIBS=-lraylib -lm -ldl -lpthread -lGL -lrt -lX11

SRC=main.c collision_system.c enemy_system.c game_state.c input_system.c \
player_system.c score_system.c ui_system.c upgrade_system.c wave_system.c

OBJ=$(SRC:.c=.o)
OUT=inkolor

all:
	$(CC) $(SRC) -o $(OUT) $(LIBS)

clean:
	rm -f $(OUT) *.o
