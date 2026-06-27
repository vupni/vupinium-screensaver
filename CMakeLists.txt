cmake_minimum_required(VERSION 3.16)
project(Screensaver)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/builds/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/builds/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/builds/bin)

include(FetchContent)

FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG 5.5
)

FetchContent_Declare(
    cmrc
    GIT_REPOSITORY https://github.com/vector-of-bool/cmrc.git
    GIT_TAG master
)

FetchContent_MakeAvailable(raylib cmrc)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    "${CMAKE_SOURCE_DIR}/src/*.cpp"
)

add_executable(screensaver ${SOURCES})

target_link_libraries(screensaver PRIVATE raylib)

file(GLOB_RECURSE ASSET_FILES CONFIGURE_DEPENDS LIST_DIRECTORIES false
    "${CMAKE_SOURCE_DIR}/assets/*"
)

cmrc_add_resource_library(
    screensaver_assets
    ALIAS screensaver::assets
    NAMESPACE screensaver_assets
    WHENCE ${CMAKE_SOURCE_DIR}/assets
    ${ASSET_FILES}
)

target_link_libraries(screensaver PRIVATE screensaver::assets)