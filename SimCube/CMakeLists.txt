# TODO: fix this
add_executable (SimCube "main.cpp" "include/logging/logger.h")

target_include_directories(SimCube PRIVATE "include")

set_property(TARGET SimCube PROPERTY CXX_STANDARD 20)
