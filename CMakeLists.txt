cmake_minimum_required(VERSION 3.20)

project(MetaReal VERSION 1.0.0)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_C_FLAGS "-Wall -Wextra -Wpedantic -Werror")
set(CMAKE_C_FLAGS_DEBUG "-g")
set(CMAKE_C_FLAGS_RELEASE "-O3")

add_executable(MetaReal srcs/main.c)

target_include_directories(MetaReal PUBLIC "heads")

target_link_libraries(MetaReal gmp)
target_link_libraries(MetaReal mpfr)
target_link_libraries(MetaReal mpc)
target_link_libraries(MetaReal mrfstr)

add_custom_command(TARGET MetaReal POST_BUILD COMMAND "${CMAKE_COMMAND}" -E copy $<TARGET_FILE:MetaReal> ..)
