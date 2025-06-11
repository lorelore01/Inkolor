# Compilador
CC = gcc

# Flags de compilação
CFLAGS = -O2 -Wall -std=c99 -IC:/raylib/include

# Flags de linkagem
LDFLAGS = -LC:/raylib/lib -lraylib -lopengl32 -lgdi32 -lwinmm

# Fontes e alvo
SOURCES = main.c collision_system.c enemy_system.c game_state.c input_system.c player_system.c ui_system.c upgrade_system.c wave_system.c
OBJECTS = $(SOURCES:.c=.o)
TARGET = inkolor.exe

# Regra padrão
all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)

clean:
	rm -f *.o $(TARGET)
