cmake_minimum_required(VERSION 4.1)
project(untitled)

set(CMAKE_CXX_STANDARD 11)

add_executable(untitled main.cpp
        geom.cpp
        geom.h
        idraw.cpp
        idraw.h
        dot.cpp
        dot.h
        rect.cpp
        rect.h
        frect.cpp
        frect.h
        canvas.cpp
        canvas.h)
