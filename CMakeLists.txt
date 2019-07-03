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

add_library(tevian_lib SHARED ${SOURCES_LIB})
add_executable(tevian_ui ${SOURCES_UI})
add_executable(tevian_cli ${SOURCES_CLI})

target_link_libraries(tevian_lib Qt5::Widgets Qt5::Network)
target_link_libraries(tevian_ui tevian_lib)
target_link_libraries(tevian_cli tevian_lib)

install(TARGETS tevian_lib DESTINATION /usr/local/lib)
install(TARGETS tevian_ui DESTINATION /usr/local/bin)
install(TARGETS tevian_cli DESTINATION /usr/local/bin)
install(FILES src/loader.h DESTINATION /usr/local/include)
install(FILES src/requester.h DESTINATION /usr/local/include)

set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_NAME tevian)
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Tevian exam project")
set(CPACK_PACKAGE_VERSION 1.0-1)
set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE amd64)
set(CPACK_DEBIAN_PACKAGE_DEPENDS "libqt5gui5 (>= 5.9.5)")
set(CPACK_PACKAGE_CONTACT "ya.olgapshenichnikova@yandex.ru")
set(CPACK_DEBIAN_PACKAGE_SECTION devel)

INCLUDE(CPack)