cmake_minimum_required( VERSION 3.6.0 )
project(QtProject)

include_directories(./include)
set(Qt5_DIR D:/Qt/Qt5.12.12/5.12.12/msvc2017/lib/cmake/Qt5/)
find_package(Qt5 REQUIRED COMPONENTS Widgets Network Xml Sql PrintSupport Multimedia MultimediaWidgets WebEngineWidgets)
set(CMAKE_INCLUDE_CURRENT_DIR ON)


add_subdirectory(application_mainprogram)
add_subdirectory(downloader)
add_subdirectory(drawtool)
add_subdirectory(everything)
add_subdirectory(handleorcode)
add_subdirectory(mediaplayer)
add_subdirectory(screenrecord)
add_subdirectory(screenshot)

set(DEST_DIR ${CMAKE_BINARY_DIR}/bin)
set_target_properties(WindowTool
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${DEST_DIR}/debug/thirdplugin"
    LIBRARY_OUTPUT_DIRECTORY_DEBUG "${DEST_DIR}/debug/thirdplugin"
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${DEST_DIR}/debug"
    ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${DEST_DIR}/release/thirdplugin"
    LIBRARY_OUTPUT_DIRECTORY_RELEASE "${DEST_DIR}/release/thirdplugin"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${DEST_DIR}/release"
    ARCHIVE_OUTPUT_DIRECTORY_MINSIZEREL "${DEST_DIR}/minsizerel/thirdplugin"
    LIBRARY_OUTPUT_DIRECTORY_MINSIZEREL "${DEST_DIR}/minsizerel/thirdplugin"
    RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL "${DEST_DIR}/minsizerel"
    ARCHIVE_OUTPUT_DIRECTORY_RELWITHDEBINFO "${DEST_DIR}/relwithdebinfo/thirdplugin"
    LIBRARY_OUTPUT_DIRECTORY_RELWITHDEBINFO "${DEST_DIR}/relwithdebinfo/thirdplugin"
    RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${DEST_DIR}/relwithdebinfo"
)

set_target_properties(downloader drawtool everything handleorcode mediaplayer screenrecord screenshot
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${DEST_DIR}/debug/thirdplugin"
    LIBRARY_OUTPUT_DIRECTORY_DEBUG "${DEST_DIR}/debug/thirdplugin"
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${DEST_DIR}/debug/thirdplugin"
    ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${DEST_DIR}/release/thirdplugin"
    LIBRARY_OUTPUT_DIRECTORY_RELEASE "${DEST_DIR}/release/thirdplugin"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${DEST_DIR}/release/thirdplugin"
    ARCHIVE_OUTPUT_DIRECTORY_MINSIZEREL "${DEST_DIR}/minsizerel/thirdplugin"
    LIBRARY_OUTPUT_DIRECTORY_MINSIZEREL "${DEST_DIR}/minsizerel/thirdplugin"
    RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL "${DEST_DIR}/minsizerel/thirdplugin"
    ARCHIVE_OUTPUT_DIRECTORY_RELWITHDEBINFO "${DEST_DIR}/relwithdebinfo/thirdplugin"
    LIBRARY_OUTPUT_DIRECTORY_RELWITHDEBINFO "${DEST_DIR}/relwithdebinfo/thirdplugin"
    RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${DEST_DIR}/relwithdebinfo/thirdplugin"
)
