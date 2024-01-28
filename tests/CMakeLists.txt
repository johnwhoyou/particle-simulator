cmake_minimum_required(VERSION 3.14)

set(TEST_SOURCES
    ParticleTests.cpp
    # Add other test source files here
)

add_executable(ParticleSimulatorTests ${TEST_SOURCES})

target_link_libraries(ParticleSimulatorTests PUBLIC
    gtest_main
    ParticleSimulator
)

add_test(
    NAME ParticleSimulatorTests
    COMMAND ParticleSimulatorTests
)