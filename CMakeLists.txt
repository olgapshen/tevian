cmake_minimum_required (VERSION 3.1)
project (tevian)

set(CMAKE_CXX_STANDARD 11)
# set(QT_PATH "/usr/include/x86_64-linux-gnu/qt5")
# set(QT_PATH_CORE "/usr/include/x86_64-linux-gnu/qt5/QtCore")
# set(QT_PATH_GUI "/usr/include/x86_64-linux-gnu/qt5/QtGui")

find_package(Qt5 REQUIRED COMPONENTS Core Widgets)

include_directories(
  . 
  # QT_PATH
  # QT_PATH_CORE
  # QT_PATH_GUI
)

add_executable(${PROJECT_NAME} main.cpp)
target_link_librarieS(${PROJECT_NAME} Qt5::Widgets)