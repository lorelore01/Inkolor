# Detecta o sistema operacional
ifeq ($(OS),Windows_NT)
    PLATFORM = windows
    TARGET = build/windows/inkolor.exe
    RAYLIB_INCLUDE = C:/raylib/include
    RAYLIB_LIB = C:/raylib/lib
    LDFLAGS = -L"$(RAYLIB_LIB)" -lraylib -lopengl32 -lgdi32 -lwinmm
else
    PLATFORM = linux
    TARGET = build/linux/inkolor
    RAYLIB_INCLUDE = /path/to/raylib/include
    RAYLIB_LIB = /path/to/raylib/lib
    LDFLAGS = -L$(RAYLIB_LIB) -lraylib -lm -ldl -lpthread -lGL -lrt -lX11
endif

# Compilador e flags
CC = gcc
CFLAGS = -O2 -Wall -std=c99 -I$(RAYLIB_INCLUDE)

# Fontes e objetos
SOURCES = main.c collision_system.c enemy_system.c game_state.c input_system.c player_system.c ui_system.c upgrade_system.c wave_system.c
OBJDIR = build/$(PLATFORM)/obj
OBJECTS = $(patsubst %.c,$(OBJDIR)/%.o,$(SOURCES))

# Regra padrão
all: $(TARGET)

# Compilação final
$(TARGET): $(OBJECTS)
	@mkdir -p $(dir $@)
	$(CC) -o $@ $^ $(LDFLAGS)

# Compilação dos .c -> .o
$(OBJDIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) -c $< -o $@ $(CFLAGS)

# Limpeza
clean:
	rm -rf build/linux build/windows

