cmake_minimum_required(VERSION 3.10)

project(MyProject)

set(CMAKE_C_STANDARD 11)

add_executable(main main.c view.c den_controller.c model.c fakenews.c)