cmake_minimum_required(VERSION 3.30)
project(slynx_s_war_thunder)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Složková struktura
include_directories(
        resources
        src/logger
        src/launcher
        src/game
)

file(GLOB_RECURSE SRC_FILES
        resources/*.rc
        src/*.cpp
        src/*.h
)

add_executable(slynx_s_war_thunder ${SRC_FILES})

if (WIN32)
    target_link_libraries(slynx_s_war_thunder PRIVATE
            user32
            kernel32
    )
endif ()
