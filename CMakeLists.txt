
cmake_minimum_required(VERSION 3.16)

list(APPEND CMAKE_PREFIX_PATH "D:/Qt/6.8.1/msvc2022_64")
project(chip_8 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets)
qt_standard_project_setup()


file (GLOB_RECURSE SOURCES src/chip_8.cpp )
add_executable(${PROJECT_NAME}
   ${SOURCES} src/main.cpp src/main_window.cpp src/chip_8_widget.cpp)
target_link_libraries(${PROJECT_NAME} PUBLIC Qt6::Core Qt6::Widgets)
target_include_directories(${PROJECT_NAME} PUBLIC "src/")
add_library(${PROJECT_NAME}-lib STATIC ${SOURCES})
target_link_libraries(${PROJECT_NAME}-lib PUBLIC Qt6::Core Qt6::Widgets)
target_include_directories(${PROJECT_NAME}-lib PUBLIC src/)
add_subdirectory(tests)
