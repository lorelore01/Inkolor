@echo off
echo Compilando jogo...

gcc main.c collision_system.c enemy_system.c game_state.c input_system.c ^
player__system.c score_system.c ui_system.c upgrade_system.c wave_system.c ^
-I"C:/Users/Lorenzo/Downloads/raylib/include" ^
-L"C:/Users/Lorenzo/Downloads/raylib/lib" ^
-o meu_jogo.exe -lraylib -lopengl32 -lgdi32 -lwinmm

pause
