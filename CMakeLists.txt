cmake_minimum_required(VERSION 3.28)
project(vkutils C)
enable_testing()

set(CMAKE_C_STANDARD 17)

add_library(vkutils STATIC CArrayUtil.c)

add_executable(carrtest carrtest.c carrtest.c)

target_link_libraries(carrtest vkutils)
add_test(NAME array_int_test COMMAND carrtest array_int)
add_test(NAME array_pchar_test COMMAND carrtest array_pchar)
add_test(NAME array_shrink_to_fit COMMAND carrtest array_shrink_to_fit)
add_test(NAME array_expand COMMAND carrtest array_expand)