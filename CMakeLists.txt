cmake_minimum_required(VERSION 3.15)
project(raycaster C)

find_package(Threads REQUIRED)

add_executable(raycaster
    main.c
    glad.c
    LinkedList.c
    Fileloader.c
    Ennemy.c
    mathFuncs.c
)

target_include_directories(raycaster PRIVATE ./Librairies/include)

target_link_libraries(raycaster
    glfw
    GL
    X11
    pthread
    Xrandr
    Xi 
    dl 
    m
)
