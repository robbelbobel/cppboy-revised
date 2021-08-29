cmake_minimum_required(VERSION 3.10)

project(cppboy)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/")
set(CMAKE_PREFIX_PATH)

add_executable(${PROJECT_NAME}  src/main.cpp
                                src/gameboy/cartridge.cpp
                                src/gameboy/gameboy.cpp
                                src/gameboy/mmu.cpp
                                src/gameboy/cpu/cpu.cpp
                                src/gameboy/cpu/opcodes.cpp
                                )

find_package(SFML REQUIRED system window graphics)

include_directories(${PROJECT_NAME} ${SFML_INCLUDE_DIR})

target_link_libraries(${PROJECT_NAME} ${SFML_LIBRARIES})
