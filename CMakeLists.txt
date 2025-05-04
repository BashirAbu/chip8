
cmake_minimum_required(VERSION 3.16)

project(chip_8 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

find_package(Qt6 REQUIRED COMPONENTS Core)
qt_standard_project_setup()


file (GLOB_RECURSE SOURCES src/*.cpp src/*.c)
list(REMOVE_ITEM ${SOURCES} src/main.cpp)
add_executable(${PROJECT_NAME}
   ${SOURCES} src/main.cpp
)
add_library(${PROJECT_NAME}-lib STATIC ${SOURCES})
target_link_libraries(${PROJECT_NAME} PUBLIC Qt6::Core)
add_subdirectory(tests)
