cmake_minimum_required(VERSION 3.14)
project(MyCppProject CXX)

set(CMAKE_CXX_STANDARD 17)
set(PROJECT_INCLUDE_DIR
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include/readers
)

add_library(exifviewer_lib STATIC
    src/core/IFDReader.cpp
    src/core/FileReader.cpp
    src/core/ReaderFactory.cpp
    src/core/readers/JPEGReader.cpp
    src/core/readers/HEICReader.cpp
    src/core/readers/PNGReader.cpp
    src/core/readers/WebPReader.cpp
    src/core/readers/TIFFReader.cpp
    src/core/readers/JXLReader.cpp
)
target_include_directories(exifviewer_lib PUBLIC ${PROJECT_INCLUDE_DIR})

target_link_libraries(exifviewer_lib PUBLIC GTest::gtest)

add_executable(exifviewer src/cli/main.cpp)
target_link_libraries(exifviewer PRIVATE exifviewer_lib)

if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(exifviewer_lib PRIVATE -Wno-shift-count-overflow)
    target_compile_options(exifviewer PRIVATE -Wno-shift-count-overflow)
endif()

find_package(GTest REQUIRED)

enable_testing()
add_subdirectory(test)
