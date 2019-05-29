cmake_minimum_required (VERSION 3.0)

include_directories(
  "${CMAKE_SOURCE_DIR}/include"
  ${jlibcpp_INCLUDE_DIRS}
)

add_executable(frogger
  frogger.cpp
)

target_link_libraries(frogger
  ${jlibcpp_LIBRARIES}
)

