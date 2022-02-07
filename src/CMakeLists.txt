cmake_minimum_required (VERSION 3.0)

pkg_check_modules(jCanvas REQUIRED IMPORTED_TARGET jcanvas)

macro(game)
  add_executable(${ARGV0}
    ${ARGV0}.cpp
  )

  target_link_libraries(${ARGV0}
    PRIVATE
      PkgConfig::jCanvas
      Threads::Threads
      jatari
  )
endmacro()

game(frogger)
game(pong)
game(asteroids)
game(tank)
