cmake_minimum_required(VERSION 3.20)
project(wolfenstein_raycaster
    VERSION 1.0.0
    DESCRIPTION "Software raycaster using SDL3"
    LANGUAGES C CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Default to Debug build if unspecified
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

find_package(SDL3 REQUIRED)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp" 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
)

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    SDL3::SDL3
)

# compiler warnings per platform
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
        /W4
        /permissive-
        /w14265 # class has virtual functions, but destructor is not virtual
        /w14296 # expression is always true/false
        /w14545 # expression before comma evaluates to a function which is missing an argument list
    )
else()
    target_compile_options(${PROJECT_NAME} PRIVATE
        -Wall
        -Wextra
        -Wpedantic
        -Wshadow
        -Wnon-virtual-dtor
        -Wold-style-cast
        -Wcast-align
        -Wunused
        -Woverloaded-virtual
        -Wconversion
        -Wsign-conversion
        -Wnull-dereference
        -Wdouble-promotion
        -Wformat=2
    )
endif()

# Asset Syncing Post-Build
set(ASSET_DIR "${CMAKE_CURRENT_SOURCE_DIR}/assets")
if(EXISTS "${ASSET_DIR}")
    add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${ASSET_DIR}"
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>/assets"
        COMMENT "Syncing asset directory to target binary output directory..."
    )
endif()
