cmake_minimum_required(VERSION 3.16)
project(GraphicsGameDev VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Default to Debug build if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose build type." FORCE)
endif()

# Global dependencies (Find SDL3 once for all targets)
find_package(SDL3 REQUIRED)

# Add subprojects
# add_subdirectory(01_software_2d/01_pong_renderer)
# add_subdirectory(01_software_2d/02_asteroids_vector)
add_subdirectory(02_pseudo_3d/02_wolfenstein_raycaster)
