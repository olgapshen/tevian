cmake_minimum_required (VERSION 3.1)
project (tevian)

set(CMAKE_CXX_STANDARD 14)
#SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -fPIC")

# Find includes in corresponding build directories
#set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed
set(CMAKE_AUTOMOC ON)
# Create code from a list of Qt designer ui files
#set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Core Widgets Network REQUIRED)

include_directories(src ${Qt5Widgets_INCLUDE_DIRS})
#include_directories(src)

file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} Qt5::Widgets Qt5::Network)