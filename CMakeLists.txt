cmake_minimum_required( VERSION 3.6.0 )
project(QtProject)

include_directories(./include)
set(Qt5_DIR D:/Qt/Qt5.12.12/5.12.12/msvc2017/lib/cmake/Qt5/)
find_package(Qt5 REQUIRED COMPONENTS Widgets Network Xml Sql PrintSupport Multimedia MultimediaWidgets WebEngineWidgets)
set(CMAKE_INCLUDE_CURRENT_DIR ON)


set(DEST_DIR ${CMAKE_BINARY_DIR}/bin/)
IF (CMAKE_BUILD_TYPE MATCHES "Release")
    string(CONCAT DEST_DIR ${DEST_DIR} release)
ELSEIF (CMAKE_BUILD_TYPE MATCHES "MinSizeRel")
    string(CONCAT DEST_DIR ${DEST_DIR} minsizerel)
ELSEIF(CMAKE_BUILD_TYPE MATCHES "RelWithDebInfo")
    string(CONCAT DEST_DIR ${DEST_DIR} relwithdebinfo)
ELSEIF(CMAKE_BUILD_TYPE MATCHES "Debug")
    string(CONCAT DEST_DIR ${DEST_DIR} debug)
ELSE()
    string(CONCAT DEST_DIR ${DEST_DIR} "")
ENDIF()


#set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${DEST_DIR}/thirdplugin)
#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${DEST_DIR}/thirdplugin)
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${DEST_DIR})

add_subdirectory(application_mainprogram)
add_subdirectory(downloader)
add_subdirectory(drawtool)
add_subdirectory(everything)
add_subdirectory(handleorcode)
add_subdirectory(mediaplayer)
add_subdirectory(screenrecord)
add_subdirectory(screenshot)

set_target_properties(WindowTool
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${DEST_DIR}/thirdplugin"
    LIBRARY_OUTPUT_DIRECTORY "${DEST_DIR}/thirdplugin"
    RUNTIME_OUTPUT_DIRECTORY "${DEST_DIR}"
)

set_target_properties(downloader drawtool everything handleorcode mediaplayer screenrecord screenshot
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${DEST_DIR}/thirdplugin"
    LIBRARY_OUTPUT_DIRECTORY "${DEST_DIR}/thirdplugin"
    RUNTIME_OUTPUT_DIRECTORY "${DEST_DIR}/thirdplugin"
)
