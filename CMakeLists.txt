cmake_minimum_required(VERSION 3.14)
project(btc_horcrux LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(
    -Ofast
    -Wall -Wextra -Werror -Wconversion -Wshadow -Wnon-virtual-dtor
    -Wold-style-cast -Wunused -Woverloaded-virtual -Wsign-conversion
    -Wmisleading-indentation -Wduplicated-branches -Wlogical-op
    -Wuseless-cast -Wdouble-promotion -Wformat=2 -Wfloat-equal
    -Weffc++ -Wnull-dereference -Wduplicated-cond 
    -Wmissing-declarations -Wcast-align
)

include_directories(include)

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/release-1.12.1.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(googletest)

# removing warnings caused by google test
if (TARGET gtest AND TARGET gtest_main)
    foreach(target gtest gtest_main)
        target_compile_options(${target} PRIVATE 
            -Wno-effc++
            -Wno-useless-cast
            -Wno-null-dereference
        )
    endforeach()
endif()


add_library(btc_horcrux INTERFACE)

target_include_directories(btc_horcrux INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)


add_executable(main_project src/main.cpp)
target_compile_definitions(main_project PRIVATE NDEBUG)
target_link_libraries(main_project PRIVATE btc_horcrux)


enable_testing()

add_executable(test_algebra tests/test_algebra.cpp)
target_compile_definitions(test_algebra PRIVATE TEST_BUILD)
target_link_libraries(test_algebra PRIVATE gtest gtest_main btc_horcrux)

include(GoogleTest)
gtest_discover_tests(test_algebra DISCOVERY_MODE PRE_TEST)
