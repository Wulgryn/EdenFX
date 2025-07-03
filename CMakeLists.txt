cmake_minimum_required(VERSION 3.10)
project(EdenFXTests)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


include(CTest)
enable_testing()

file(GLOB_RECURSE EDENFX_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp"
)
list(FILTER EDENFX_SOURCES EXCLUDE REGEX ".*main\\.cpp$")




file(GLOB_RECURSE TEST_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/tests/*.hpp"
)

file(GLOB_RECURSE LIBS
        "${CMAKE_CURRENT_SOURCE_DIR}/externals/libs/*.a"
)

add_executable(test_suite ${TEST_SOURCES} ${EDENFX_SOURCES} ${EXTERNAL_SOURCES} ${EXTERNAL_LIB_SOURCES})
target_link_directories(test_suite PUBLIC externals/libs)
target_link_libraries(test_suite PUBLIC ${LIBS})
target_include_directories(test_suite PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/externals/headers")
target_include_directories(test_suite PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src")
