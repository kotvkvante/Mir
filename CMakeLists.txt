cmake_minimum_required(VERSION 3.16)

project(MirGame)

add_executable(Mir
)

add_subdirectory("source")

file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})
