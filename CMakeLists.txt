cmake_minimum_required(VERSION 3.20)
project(pea_tsp_1 VERSION 2.0)

# Ustawienie standardu C++23
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Włączenie ostrzeżeń
if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Dodanie pliku wykonywalnego
add_executable(pea_tsp_1
        main.cpp
        src/result_writer.cpp
        src/config.cpp
        src/solvers.cpp
        src/tsp_instance.cpp
)

# Katalogi z nagłówkami
target_include_directories(pea_tsp_1 PRIVATE ${CMAKE_SOURCE_DIR})

# Informacje o buildzie
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Compiler: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")