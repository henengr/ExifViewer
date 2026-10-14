cmake_minimum_required(VERSION 3.19)
project(ExifViewer LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets LinguistTools)

qt_standard_project_setup()

qt_add_executable(ExifViewer
    WIN32 MACOSX_BUNDLE
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    ../core/IFDReader.cpp
    ../core/JPEGReader.cpp
    ../core/HEICReader.cpp
    ../core/PNGReader.cpp
    ../core/FileReader.cpp
    ../core/ReaderFactory.cpp
    ../../include/FileReader.hpp
    ../../include/IFD.hpp
    ../../include/IFDReader.hpp
    ../../include/JFIF.hpp
    ../../include/JPEGReader.hpp
    ../../include/HEICReader.hpp
    ../../include/PNGReader.hpp
    ../../include/ReaderFactory.hpp
)

qt_add_translations(
    TARGETS ExifViewer
    TS_FILES ExifViewer_ja_JP.ts
)

target_link_libraries(ExifViewer
    PRIVATE
        Qt::Core
        Qt::Widgets
)

target_include_directories(ExifViewer
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../../include
)

if(WIN32)
    target_compile_definitions(ExifViewer PRIVATE DISABLE_GTEST)
endif()

install(TARGETS ExifViewer
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET ExifViewer
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
