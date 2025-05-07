cmake_minimum_required(VERSION 3.16)

project(chip_8 VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 REQUIRED COMPONENTS Core Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Widgets)

set(PROJECT_SOURCES
         src/mainwindow.h
         src/mainwindow.cpp
         src/mainwindow.ui
         src/romdirwidget.h
         src/romdirwidget.cpp
         src/romdirwidget.ui
         src/configdialog.h
         src/configdialog.cpp
         src/configdialog.ui
         src/chip_8.h
         src/chip_8.cpp
         src/main.cpp
         src/chip_8_widget.cpp
         src/chip_8_widget.h
         assets/res.qrc
)


qt_add_executable(chip_8
    MANUAL_FINALIZATION
    ${PROJECT_SOURCES}
    src/romdirwidget.h src/romdirwidget.cpp src/romdirwidget.ui
    src/romlistwidget.h src/romlistwidget.cpp src/romlistwidget.ui
)

target_link_libraries(chip_8 PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
    
target_include_directories(${PROJECT_NAME} PUBLIC "src/")


qt_add_library(${PROJECT_NAME}-lib STATIC src/chip_8.cpp)
target_link_libraries(${PROJECT_NAME}-lib PUBLIC Qt6::Core Qt6::Widgets)
target_include_directories(${PROJECT_NAME}-lib PUBLIC src/)


add_subdirectory(tests)
