cmake_minimum_required( VERSION 3.6.0 )
project(drawtool)

file(GLOB H_FILES ./*.h)
file(GLOB CPP_FILES ./*.cpp)
file(GLOB UI_FILES ./*.ui)
list(APPEND H_FILES ${CMAKE_CURRENT_SOURCE_DIR}/../include/dll_global.h)

file(GLOB PB_H_FILES ./qtpropertybrowser/src/*.h)
file(GLOB PB_CPP_FILES ./qtpropertybrowser/src/*.cpp)

#qt5_wrap_ui(UI_SRC ${UI_FILES} ${TREE_UI_FILES})
#qt5_wrap_cpp(MOC_SRC ${H_FILES} ${TREE_H_FILES})

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
qt5_add_resources(QRC_FILES ./qtpropertybrowser/src/qtpropertybrowser.qrc)
add_definitions(-DQT_DISABLE_DEPRECATED_BEFORE=0)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/qtpropertybrowser/src)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/qtpropertybrowser/src)

#set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/thirdplugin)
#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/thirdplugin)
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/thirdplugin)

add_library(drawtool SHARED ${H_FILES} ${CPP_FILES} ${UI_FILES} ${PB_H_FILES} ${PB_CPP_FILES} ${QRC_FILES})

target_link_libraries(drawtool Qt5::Widgets Qt5::Xml)
source_group("Forms Files" FILES ${UI_FILES})
