cmake_minimum_required(VERSION 3.20)

project(MetaReal VERSION 1.0.0)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_C_FLAGS "-Wall -Wextra")
set(CMAKE_C_FLAGS_DEBUG "-g")
set(CMAKE_C_FLAGS_RELEASE "-O3")

add_executable(MetaReal srcs/main.c srcs/alloc.c srcs/error/error.c
    srcs/lexer/token.c srcs/lexer/lexer.c srcs/parser/node.c srcs/parser/parser.c
    srcs/optimizer/int.c srcs/optimizer/float.c srcs/optimizer/complex.c
    srcs/optimizer/list.c srcs/optimizer/value.c srcs/optimizer/context.c
    srcs/optimizer/operation.c srcs/optimizer/optimizer.c srcs/generator/generator.c)

target_include_directories(MetaReal PUBLIC "heads")

target_link_libraries(MetaReal gmp)
target_link_libraries(MetaReal mpfr)
target_link_libraries(MetaReal mpc)

add_custom_command(TARGET MetaReal POST_BUILD COMMAND "${CMAKE_COMMAND}" -E copy $<TARGET_FILE:MetaReal> ..)
