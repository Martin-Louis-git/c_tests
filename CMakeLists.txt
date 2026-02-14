cmake_minimum_required(VERSION 3.22.1)
project(c_ci_test C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_library(ringbuffer src/ringbuffer.c)
target_include_directories(ringbuffer PUBLIC include)

include(CTest)
enable_testing()

find_package(PkgConfig REQUIRED)
pkg_check_modules(CMOCKA REQUIRED cmocka)

add_executable(test_ringbuffer tests/test_ringbuffer.c)
target_link_libraries(test_ringbuffer PRIVATE ringbuffer ${CMOCKA_LIBRARIES})

add_test(NAME test_ringbuffer COMMAND test_ringbuffer)