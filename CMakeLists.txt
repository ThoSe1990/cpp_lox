
cmake_minimum_required(VERSION 3.16)
project(cucumber-cpp)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

if(MSVC)
    add_compile_options(
        $<$<CONFIG:>:/MT> 
        $<$<CONFIG:Debug>:/MTd>
        $<$<CONFIG:Release>:/MT>
    )
endif()

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_options(-g -O0 -fno-omit-frame-pointer -fsanitize=address,leak)
    link_libraries(-fsanitize=address,leak)
    endif()
endif()

set(target example)
add_executable(${target} 
${PROJECT_SOURCE_DIR}/src/main.cpp
${PROJECT_SOURCE_DIR}/src/environment.cpp
${PROJECT_SOURCE_DIR}/src/error.cpp
${PROJECT_SOURCE_DIR}/src/interpreter.cpp
${PROJECT_SOURCE_DIR}/src/lox_function.cpp
${PROJECT_SOURCE_DIR}/src/lox_obj.cpp
${PROJECT_SOURCE_DIR}/src/scanner.cpp
${PROJECT_SOURCE_DIR}/src/token.cpp
)
target_include_directories(${target} PRIVATE ${PROJECT_SOURCE_DIR}/src)
