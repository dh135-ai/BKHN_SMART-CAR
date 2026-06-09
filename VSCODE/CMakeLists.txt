cmake_minimum_required(VERSION 3.5)
project(BKHN_SmartCar LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

# --- THÊM ĐÚNG DÒNG NÀY VÀO ĐÂY ---
set(CMAKE_PREFIX_PATH "C:/Qt/6.11.1/mingw_64")

# Tìm các module cần thiết của Qt
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Network SerialPort)

add_executable(BKHN_SmartCar
    main.cpp
    mainwindow.cpp
    mainwindow.h
)

target_link_libraries(BKHN_SmartCar PRIVATE 
    Qt6::Core Qt6::Gui Qt6::Widgets Qt6::Network Qt6::SerialPort
)
