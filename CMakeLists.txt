cmake_minimum_required(VERSION 3.25)
project(sedit)

set(CMAKE_CXX_STANDARD 23)
add_compile_options(-Wall -Wextra -Wpedantic -Werror)

add_subdirectory(deps/fmt)
include_directories(deps/fmt/include)

file(GLOB_RECURSE COMMON_SRC "common/*")
add_library(common ${COMMON_SRC})
include_directories(common)
target_link_libraries(common fmt)

file(GLOB_RECURSE SRC "src/*")
add_executable(sedit ${SRC})
target_link_libraries(sedit common)

add_executable(sequence_reader tools/seqence_reader.cpp)
target_link_libraries(sequence_reader common)
