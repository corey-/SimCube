# TODO: fix this
add_executable (SimCube "main.cpp" "include/device/gekko/gekko.h" "include/device/gekko/gekko_regs.h" "device/gekko/gekko.cpp" "include/disc/game_disc.h" "include/logging/logger.h" "disc/game_disc.cpp" "include/disc/ciso_disc.h" "disc/ciso_disc.cpp" "include/util/endian.h" "include/device/dvd/dvd.h" "include/device/msg/device_msg.h" "include/device/device.h" "device/dvd/dvd.cpp" "include/util/types.h" "include/util/alignment.h" "include/device/dvd/dvd_regs.h" "include/device/memory/ram.h" "device/memory/ram.cpp" "include/device/gekko/power_pc_tables.h" "device/gekko/power_pc_tables.cpp")

target_include_directories(SimCube PRIVATE "include")

set_property(TARGET SimCube PROPERTY CXX_STANDARD 20)
