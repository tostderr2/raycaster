cmake_minimum_required(VERSION 3.16)
project(wolfenstein_raycaster VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
endif()

if(NOT TARGET SDL3::SDL3)
    find_package(SDL3 REQUIRED)
endif()

file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.c")

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE 
    include
)

target_link_libraries(${PROJECT_NAME} PRIVATE 
    SDL3::SDL3
)

if(IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/assets")
    add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/assets"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>/assets"
    )
endif()
