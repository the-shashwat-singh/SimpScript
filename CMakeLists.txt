cmake_minimum_required(VERSION 3.10)
project(SimpScript VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(include)

# Add source files
file(GLOB SOURCES "src/*.cpp")

# Create executable
add_executable(simpscript ${SOURCES})

# Install
install(TARGETS simpscript DESTINATION bin) 