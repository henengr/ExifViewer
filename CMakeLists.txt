cmake_minimum_required(VERSION 3.10)
project(MyCppProject CXX)

set(CMAKE_CXX_STANDARD 17)

set(SOURCES
    src/main.cpp
    src/JPEGReader.cpp
)

add_executable(exifviewer ${SOURCES})

if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(exifviewer PRIVATE -Wno-shift-count-overflow)
endif()

target_include_directories(exifviewer PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
