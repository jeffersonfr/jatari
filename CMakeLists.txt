cmake_minimum_required (VERSION 3.0)

if (CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
  message(FATAL_ERROR "In-source build is not allowed")
endif()

project(jatari
  LANGUAGES CXX
  HOMEPAGE_URL https://github.com/jeffersonfr/jatari
  DESCRIPTION "Atari"
  VERSION 1.0.0
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(PkgConfig REQUIRED)
find_package(Threads REQUIRED)

add_library(${PROJECT_NAME} 
  INTERFACE
)

target_include_directories(${PROJECT_NAME}
  INTERFACE
    include
)

add_subdirectory(src)
