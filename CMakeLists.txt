cmake_minimum_required(VERSION 3.10)
project(capd_utils)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS YES)

set(SOURCES_LIST
    capd_utils/capd/inst.cpp

    capd_utils/number_string_info.cpp
    capd_utils/number_string_utilities.cpp
    )

add_library(${PROJECT_NAME} STATIC ${SOURCES_LIST})

add_subdirectory(CAPD)
add_dependencies(${PROJECT_NAME} capd)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${PROJECT_NAME} PUBLIC capd)


