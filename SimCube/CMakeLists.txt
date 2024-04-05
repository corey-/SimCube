# TODO: fix this
add_executable (SimCube "main.cpp" "include/device/gekko/gekko.h" "include/device/gekko/gekko_regs.h" "device/gekko/gekko.cpp" "include/disc/game_disc.h" "include/logging/logger.h" "disc/game_disc.cpp" "include/disc/ciso_disc.h" "disc/ciso_disc.cpp" "include/util/endian.h" "include/device/dvd/dvd.h" "include/device/msg/device_msg.h" "include/device/device.h" "device/dvd/dvd.cpp" "include/util/types.h" "include/util/alignment.h" "include/device/dvd/dvd_regs.h" "include/device/memory/ram.h" "device/memory/ram.cpp" "include/device/gekko/power_pc_tables.h" "device/gekko/power_pc_tables.cpp" "include/device/gekko/execution.h" "include/device/gekko/jit_x64.h" "device/gekko/jit_x64.cpp")

include(FetchContent)

FetchContent_Declare(
	asmjit
	GIT_REPOSITORY https://github.com/asmjit/asmjit.git
	GIT_TAG master
)

set(ASMJIT_STATIC TRUE)
set(ASMJIT_NO_AARCH64 TRUE)
FetchContent_MakeAvailable(asmjit)

target_include_directories(SimCube PRIVATE "include" "${asmjit_SOURCE_DIR}/src")

target_link_libraries(SimCube PRIVATE asmjit::asmjit)

set_property(TARGET SimCube PROPERTY CXX_STANDARD 20)
