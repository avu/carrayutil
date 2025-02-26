cmake_minimum_required(VERSION 3.28)
project(vkutils C)
enable_testing()

set(CMAKE_C_STANDARD 17)

add_library(vkutils STATIC CArrayUtil.c)
target_include_directories(vkutils PUBLIC .)

add_executable(carrtest
        test/test.c
        test/test_array.c
        test/test_ring_buffer.c
        test/test_map.c
        test/test_alloc.c)

target_link_libraries(carrtest vkutils)
add_test(NAME / COMMAND carrtest)
add_test(NAME array COMMAND carrtest array)
add_test(NAME ring_buffer COMMAND carrtest ring_buffer)
add_test(NAME map COMMAND carrtest map)
add_test(NAME alloc COMMAND carrtest alloc)