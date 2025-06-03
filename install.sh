#!/bin/bash

echo "Instalando dependências do sistema..."
sudo apt update
sudo apt install -y build-essential git cmake \
libgl1-mesa-dev libopenal-dev libxi-dev libx11-dev \
libxcursor-dev libxrandr-dev libxinerama-dev libxxf86vm-dev \
libwayland-dev libxkbcommon-dev

echo "Clonando Raylib..."
git clone https://github.com/raysan5/raylib.git
cd raylib

echo "Compilando Raylib..."
mkdir build && cd build
cmake ..
make
sudo make install

echo "✅ Raylib instalado com sucesso!"
