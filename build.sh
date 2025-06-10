#!/usr/bin/env bash

# Compilação do jogo com raylib
gcc -o game.exe main.c \
    -I.                            \
    -L/mingw64/bin                 \
    -lraylib -lwinmm -lgdi32

# Copia a dll para o mesmo local do .exe
cp /mingw64/bin/raylib.dll ./game.exe .

echo "Build concluído. game.exe e raylib.dll prontos."
