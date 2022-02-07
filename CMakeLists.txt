cmake_minimum_required (VERSION 3.0)

project (jatari)

set(CMAKE_CXX_STANDARD 20)

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
