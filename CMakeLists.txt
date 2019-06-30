cmake_minimum_required (VERSION 3.1)
project (Tevian)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_AUTOMOC ON)

find_package(Qt5 COMPONENTS Core Widgets Network REQUIRED)

include_directories(src ${Qt5Widgets_INCLUDE_DIRS})

set(
  SOURCES_LIB
  "src/loader.cpp"
  "src/requester.cpp"
)

set(
  SOURCES_UI
  "src/mainwindow.cpp"
  "src/viewpoint.cpp"
  "src/ui.cpp"
)

set(
  SOURCES_CLI
  "src/maintask.cpp"
  "src/cli.cpp"
)

add_library(tevian_lib ${SOURCES_LIB})
add_executable(tevian_ui ${SOURCES_UI})
add_executable(tevian_cli ${SOURCES_CLI})

target_link_libraries(tevian_lib Qt5::Widgets Qt5::Network)
target_link_libraries(tevian_ui tevian_lib)
target_link_libraries(tevian_cli tevian_lib)