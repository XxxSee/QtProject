cmake_minimum_required( VERSION 3.6.0 )
project(WindowTool)

add_definitions(-DPROJECT_LOAD_DLL)

file(GLOB H_FILES ./*.h)
file(GLOB CPP_FILES ./*.cpp)
file(GLOB UI_FILES ./*.ui)

#qt5_wrap_ui(UI_SRC ${UI_FILES})
#qt5_wrap_cpp(MOC_SRC ${H_FILES})

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

#set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/thirdplugin)
#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/thirdplugin)
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_executable(WindowTool ${H_FILES} ${CPP_FILES} ${UI_FILES})

target_link_libraries(WindowTool Qt5::Widgets)

source_group("Forms Files" FILES ${UI_FILES})
