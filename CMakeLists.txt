cmake_minimum_required(VERSION 3.10)
project(capd_utils)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS YES)

file(GLOB SOURCES_LIST capd_utils/*.cpp)

file(GLOB SOURCES_LIST_CAPD capd_utils/capd/*.cpp)
list(APPEND SOURCES_LIST ${SOURCES_LIST_CAPD})

add_library(${PROJECT_NAME} STATIC ${SOURCES_LIST})

add_subdirectory(CAPD)
add_dependencies(${PROJECT_NAME} capd)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${PROJECT_NAME} PUBLIC capd)


