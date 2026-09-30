add_executable(exifviewer_test
    test_JPEGReader.cpp
    test_main.cpp)

target_link_libraries(exifviewer_test PRIVATE 
    exifviewer_lib
    GTest::gtest
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(exifviewer_test)

