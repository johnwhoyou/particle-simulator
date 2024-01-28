cmake_minimum_required(VERSION 3.14)
project(ParticleSimulator)

set(CMAKE_CXX_STANDARD 11) # You have C_STANDARD, but you're using C++
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

include_directories(include)

set(SOURCES
    src/Particle.cpp
    # Add other source files here
)

add_library(ParticleSimulator STATIC ${SOURCES})

target_include_directories(ParticleSimulator PUBLIC include)

add_subdirectory(googletest)
enable_testing()
add_subdirectory(tests)