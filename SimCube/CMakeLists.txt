# TODO: fix this
add_executable (SimCube "main.cpp" "include/device/gekko/gekko.h" "include/device/gekko/gekko_regs.h" "device/gekko/gekko.cpp" "include/disc/game_disc.h" "include/logging/logger.h" "disc/game_disc.cpp" "include/disc/ciso_disc.h" "disc/ciso_disc.cpp" "include/util/endian.h")

target_include_directories(SimCube PRIVATE "include")

set_property(TARGET SimCube PROPERTY CXX_STANDARD 20)
