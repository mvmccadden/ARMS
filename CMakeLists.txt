cmake_minimum_required(VERSION 3.28)
project(arms LANGUAGES CXX C)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(INPUT_DIR "${CMAKE_SOURCE_DIR}/input/") 
set(OUTPUT_DIR "${CMAKE_SOURCE_DIR}/output/") 
set(FONT_DIR "${CMAKE_SOURCE_DIR}/font/") 
set(DEFAULT_FONT "${CMAKE_SOURCE_DIR}/font/static/Roboto-Black.ttf") 

# CONFIGURE_DEPENDS tells CMake that files have been added or removed
file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS "src/*.cpp")
file(GLOB_RECURSE INC_FILES CONFIGURE_DEPENDS "inc/*.h")

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.2
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

FetchContent_Declare(nfd
    GIT_REPOSITORY https://github.com/mlabbe/nativefiledialog.git
    GIT_TAG        release_116  
)

# Make and define the NFD Library within this makefile as it doesn't have one
FetchContent_GetProperties(nfd)
if(NOT nfd_POPULATED)
    FetchContent_Populate(nfd)

    # Note: For Windows, use src/nfd_win.cpp; Using gtk3 for linux
    set(NFD_SOURCES 
        "${nfd_SOURCE_DIR}/src/nfd_common.c"
        "${nfd_SOURCE_DIR}/src/nfd_gtk.c"
    )

    add_library(nfd STATIC ${NFD_SOURCES})

    target_include_directories(nfd PUBLIC "${nfd_SOURCE_DIR}/src/include")

    find_package(PkgConfig REQUIRED)
    pkg_check_modules(GTK3 REQUIRED gtk+-3.0)

    target_include_directories(nfd PRIVATE ${GTK3_INCLUDE_DIRS})
    target_link_libraries(nfd PRIVATE ${GTK3_LIBRARIES})
endif()

add_executable(arms ${SRC_FILES} ${INC_FILES})

target_include_directories(arms PRIVATE inc)

target_compile_definitions(arms PRIVATE "INPUT_DIR=\"${INPUT_DIR}\"")
target_compile_definitions(arms PRIVATE "OUTPUT_DIR=\"${OUTPUT_DIR}\"")
target_compile_definitions(arms PRIVATE "FONT_DIR=\"${FONT_DIR}\"")
target_compile_definitions(arms PRIVATE "DEFAULT_FONT=\"${DEFAULT_FONT}\"")

target_compile_features(arms PRIVATE cxx_std_17)
target_link_libraries(arms PRIVATE SFML::Graphics SFML::Audio nfd)
