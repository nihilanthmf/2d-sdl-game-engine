cmake_minimum_required(VERSION 3.13...3.27)
project(my_project)

find_package(SDL2 REQUIRED)

add_executable(main example.c src/engine.c src/engine.h)

target_link_libraries(main PRIVATE SDL2::SDL2 SDL2::SDL2main)