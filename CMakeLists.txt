cmake_minimum_required(VERSION 3.0)
project(alter_tank C)
set(CMAKE_C_STANDARD 99)

file(GLOB_RECURSE SOURCE "src/*.c" "src/*.h")
add_executable(alter_tank ./main.c logic.c logic.h view.c view.h physics.c physics.h structs.h maze_builder.c maze_builder.h menu.c menu.h powerup.c powerup.h SaveLoad.c SaveLoad.h)

include_directories("/usr/include/SDL2")
target_link_libraries(alter_tank m SDL2 SDL2_gfx)
