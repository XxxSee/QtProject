cmake_minimum_required( VERSION 3.6.0 )
project(handleorcode)

file(GLOB H_FILES ./*.h)
file(GLOB CPP_FILES ./*.cpp)
file(GLOB UI_FILES ./*.ui)
list(APPEND H_FILES ${CMAKE_CURRENT_SOURCE_DIR}/../include/dll_global.h)
file(GLOB QREN_H_FILES ./qrencode/*.h)
file(GLOB QREN_CPP_FILES ./qrencode/*.c)
file(GLOB_RECURSE QZXING_H_FILES ./qzxing/src/*.h)
file(GLOB_RECURSE QZXING_CPP_FILES ./qzxing/src/*.cpp)

#qt5_wrap_ui(UI_SRC ${UI_FILES})
#qt5_wrap_cpp(MOC_SRC ${H_FILES})

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

#qrencode
add_definitions(-DHAVE_CONFIG_H)
include_directories(./qrencode)
#qzxing
set(QZXING_USE_DECODER_QR_CODE ON)

#set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/thirdplugin)
#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/thirdplugin)
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/thirdplugin)


add_library(handleorcode SHARED ${H_FILES} ${CPP_FILES} ${UI_FILES} ${QREN_H_FILES} ${QREN_CPP_FILES})
add_subdirectory(qzxing)
target_link_libraries(handleorcode Qt5::Widgets Qt5::Multimedia Qt5::MultimediaWidgets qzxing)

source_group("Forms Files" FILES ${UI_FILES})
