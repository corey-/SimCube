cmake_minimum_required (VERSION 3.21)

project ("SimCube")

option(DEV_LOGGING "Verbose logging intended for development" OFF)

if(${DEV_LOGGING})
  add_compile_definitions(DEV_LOGGING)
endif()

add_subdirectory ("SimCube")
