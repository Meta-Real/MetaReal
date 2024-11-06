cmake_minimum_required(VERSION 3.20)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

project(MetaReal VERSION 0.1.0
    DESCRIPTION "MetaReal Compiler version 0.1.0"
    LANGUAGES C)

if (CMAKE_C_COMPILER_ID STREQUAL "GNU" OR CMAKE_C_COMPILER_ID STREQUAL "Clang")
    set(CMAKE_C_FLAGS "-Wall -Wextra -Wpedantic -Werror")
    set(CMAKE_C_FLAGS_DEBUG "-g")
    set(CMAKE_C_FLAGS_RELEASE "-O3")
    set(CMAKE_C_FLAGS_MINSIZEREL "-Os")
    set(CMAKE_C_FLAGS_RELWITHDEBINFO "-O2 -g")
elseif (MSVC)
    set(CMAKE_C_FLAGS "/Wall /wd5045 /wd5105 /wd4324 /wd4820 /wd4710 /wd4711 /wd4996 /WX")
    set(CMAKE_C_FLAGS_DEBUG "/Od /Zi")
    set(CMAKE_C_FLAGS_RELEASE "/O2")
    set(CMAKE_C_FLAGS_MINSIZEREL "/Os")
    set(CMAKE_C_FLAGS_RELWITHDEBINFO "/O2 /Zi")
endif()

add_executable(MetaReal
    srcs/main.c srcs/config.c srcs/stack.c
    srcs/error/error.c
    srcs/lexer/lexer.c srcs/lexer/token.c
    srcs/parser/parser.c srcs/parser/node.c)

target_compile_definitions(MetaReal PRIVATE $<$<CONFIG:Debug>:__MR_DEBUG__>)
target_include_directories(MetaReal PRIVATE heads)
