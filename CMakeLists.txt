
cmake_minimum_required(VERSION 3.20)
project(chip8-emulator CXX)

# Set default build type to Debug if not specified
if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Debug)
endif()

# Enable debug symbols for all build types
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

# Add SDL3 include and library paths
include_directories(external/SDL3/include)
link_directories(external/SDL3/lib)

# Add your source files here
include_directories(include)
add_executable(chip8 src/main.cpp src/Chip8.cpp src/OpcodeHandler.cpp src/Chip8Renderer.cpp src/Emulator.cpp)

# Link SDL3 dynamic library
# If you use libSDL3.0.dylib, link as SDL3.0
# If you rename to libSDL3.dylib, link as SDL3
# You can also use target_link_libraries(chip8 SDL3::SDL3) if using SDL3 CMake config

target_link_libraries(chip8 SDL3.0)
