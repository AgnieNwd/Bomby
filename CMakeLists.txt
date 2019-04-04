cmake_minimum_required(VERSION 3.13)
project(Bomberman C)
set(CMAKE_C_STANDARD 11)
find_package(SDL2 REQUIRED)
include_directories(Bomberman ${SDL2_INCLUDE_DIRS})
add_executable(Bomberman
        ./src/main.c
        src/network/server.c src/gui/gui.c src/network/client.c src/config.h src/instances/cell.c src/instances/headers/cell.h src/instances/map.c src/instances/headers/map.h src/instances/player.c src/instances/headers/player.h src/instances/bomb.c src/instances/headers/bomb.h src/instances/object.c src/instances/headers/object.h src/gui/headers/gui.h src/network/headers/client.h src/instances/headers/menu.h src/gui/menu_gui.c src/gui/headers/menu_gui.h src/gui/menu_user_write.c)
target_link_libraries(Bomberman ${SDL2_LIBRARIES} ${SDL2IMAGE_LIBRARIES} -lSDL2main -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf -pthread -Wall -Wextra -Werror)