cmake_minimum_required(VERSION 3.5)

project(qt-pid-scope LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(
    Qt5
    COMPONENTS
    SerialPort
    PrintSupport
    Widgets
    REQUIRED
    )

add_executable(${PROJECT_NAME}
    main.cpp
    mainwindow.cpp
    mainwindow.h
    sidebar.cpp
    sidebar.h
    serial.cpp
    serial.h
    plot.cpp
    plot.h
    qcustomplot.cpp
    qcustomplot.h
    )

target_link_libraries(${PROJECT_NAME}
    PRIVATE
    Qt5::Widgets
    Qt5::SerialPort
    Qt5::PrintSupport
    )
