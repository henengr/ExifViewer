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
    graphicsviewex.h
    graphicsviewex.cpp
    picturepreview.h
    picturepreview.cpp
    picturepreview.ui
    ../core/IFDReader.cpp
    ../core/FileReader.cpp
    ../core/ReaderFactory.cpp
    ../core/readers/JPEGReader.cpp
    ../core/readers/HEICReader.cpp
    ../core/readers/PNGReader.cpp
    ../core/readers/WebPReader.cpp
    ../core/readers/TIFFReader.cpp
    ../core/readers/JXLReader.cpp
    ../../include/IFD.hpp
    ../../include/FileReader.hpp
    ../../include/IFDReader.hpp
    ../../include/ReaderFactory.hpp
    ../../include/readers/JPEGReader.hpp
    ../../include/readers/HEICReader.hpp
    ../../include/readers/PNGReader.hpp
    ../../include/readers/WebPReader.hpp
    ../../include/readers/TIFFReader.hpp
    ../../include/readers/JXLReader.hpp
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
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/../../include
        ${CMAKE_CURRENT_SOURCE_DIR}/../../include/readers
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
