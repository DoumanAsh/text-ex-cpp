project(test_ex CXX)
cmake_minimum_required(VERSION 3.0)

set(MODULE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/.cmake/Modules")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${MODULE_DIR})

include(Utils)
set_msvc_flags_if("/Wall")
set_gcc_flags_if("-Wall -Wextra")

include_directories("src")
file(GLOB main_SRC "src/*.cpp")
add_executable(main "${main_SRC}")

add_custom_target(
    cppcheck
    COMMAND cppcheck
    --enable=all
    ${main_SRC}
)


############
# Tests
############
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
include(DownloadProject)
download_project(
    PROJ googletest
    URL  https://github.com/google/googletest/archive/release-1.8.0.zip
)

add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})

file(GLOB test_SRC "test/*.cpp")
add_executable(utest "${test_SRC}")
target_link_libraries(utest gtest gtest_main)
