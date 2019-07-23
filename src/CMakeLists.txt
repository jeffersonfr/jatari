cmake_minimum_required (VERSION 3.0)

include_directories(
  "${CMAKE_SOURCE_DIR}/include"
  ${jlibcpp_INCLUDE_DIRS}
)

macro(game)
  add_executable(${ARGV0}
    ${ARGV0}.cpp
  )
target_link_libraries(${ARGV0}
    ${jlibcpp_LIBRARIES}
  )
endmacro()

game(frogger)
game(pong)
game(asteroids)
