cmake_minimum_required(VERSION 3.25)

project(IMGBE)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(
    ${PROJECT_NAME}
    ./src/main.cpp
    ./src/program.cpp
    ./src/window.cpp
    ./src/logger.cpp
    ./src/emu/emucartridge.cpp
    ./src/emu/emucpu.cpp
    ./src/emu/emumemory.cpp
    ./src/emu/emuppu.cpp
    ./src/emu/emuregisters.cpp
    ./src/emu/emusys.cpp
    ./src/emu/memorybank.cpp
)

find_package(
    SDL2 REQUIRED
)

find_package(
    fmt REQUIRED
)

target_include_directories(
    ${PROJECT_NAME} PRIVATE
    ${SDL2_INCLUDE_DIRS}
    ${fmt_INCLUDE_DIRS}
)

target_link_libraries(
    ${PROJECT_NAME} PRIVATE
    ${SDL2_LIBRARIES}
    fmt::fmt
)

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    if(MSVC)
        target_compile_options(
            ${PROJECT_NAME} PRIVATE
            -Ox
        )
    else()
        target_compile_options(
            ${PROJECT_NAME} PRIVATE
            -Wall
            -Wpedantic
            -O3
        )
    endif()
else()
    if(MSVC)
        target_compile_options(
            ${PROJECT_NAME} PRIVATE
            -Od
            -DEBUG
        )
    else()
        target_compile_options(
            ${PROJECT_NAME} PRIVATE
            -Wall
            -Wpedantic
            -O0
            -g
        )
    endif()
endif()

set_target_properties(
    ${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS ON
)
