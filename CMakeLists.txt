cmake_minimum_required(VERSION 3.17)
project(DIP VERSION 1.0 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)

include(cmake/InitUCMake.cmake)

Ubpa_InitUCMake()
Ubpa_InitProject()

find_package(Vulkan REQUIRED FATAL_ERROR)
find_package(glfw3 REQUIRED FATAL_ERROR)
find_package(OpenCV REQUIRED)

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${Vulkan_INCLUDE_DIRS})

Ubpa_AddSubDirsRec(src)
