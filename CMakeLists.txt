cmake_minimum_required(VERSION 3.10)
set(CMAKE_GENERATOR_PLATFORM win32)

project(kengine)

add_executable(kengine WIN32 
src/main.cc 
src/window.h
src/window.cc
src/dib.h
src/dib.cc
src/app.h
src/app.cc
src/memory.h
src/memory.cc
src/common.h )

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT kengine)

if (MSVC)
  target_compile_definitions(kengine PUBLIC UNICODE _UNICODE)
  target_compile_features(kengine PRIVATE cxx_std_17)
endif()