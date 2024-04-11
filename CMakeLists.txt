cmake_minimum_required(VERSION 3.28)
project(vkutils C)
enable_testing()

set(CMAKE_C_STANDARD 17)

add_library(vkutils STATIC VKUtils.c)

add_executable(vkutest vkutest.c
        vkutest.c)
target_link_libraries(vkutest vkutils)
add_test(NAME vec_int_test COMMAND vkutest vec_int)
add_test(NAME vec_pchar_test COMMAND vkutest vec_pchar)