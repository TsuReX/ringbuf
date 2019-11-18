cmake_minimum_required(VERSION 2.8)

message("-- MSG: Add dir ${CMAKE_CURRENT_SOURCE_DIR}")

set(PROJ_NAME ringbuf)

project(${PROJ_NAME} C)

set(CMAKE_C_FLAGS "-Wall -Werror")

set(C_SRC	"src/ringbuf.c")

include_directories("include")

add_library(${PROJ_NAME}  ${C_SRC})