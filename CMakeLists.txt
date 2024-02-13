cmake_minimum_required(VERSION 3.14)
project(ParticleSimulator)

set(CMAKE_CXX_STANDARD 11) # You have C_STANDARD, but you're using C++
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

include_directories(include)

file(GLOB SOURCES "src/*.cpp")

add_library(ParticleSimulator STATIC ${SOURCES})

target_include_directories(ParticleSimulator PUBLIC include)

#add_subdirectory(googletest)
#enable_testing()
#add_subdirectory(tests)

add_executable(ParticleSimulatorApp "src/Main.cpp")
target_link_libraries(ParticleSimulatorApp PRIVATE ParticleSimulator)

# Add installation prefix of vcpkg to CMAKE_PREFIX_PATH
list(APPEND CMAKE_PREFIX_PATH ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg_installed/x64-windows)

# Packages
find_package(glad CONFIG REQUIRED)
target_link_libraries(ParticleSimulatorApp PRIVATE glad::glad)

find_package(SDL2 CONFIG REQUIRED)
target_link_libraries(ParticleSimulatorApp PRIVATE
	$<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
	$<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
)

find_package(imgui CONFIG REQUIRED)
target_link_libraries(ParticleSimulator PRIVATE imgui::imgui)

# Copy DLLs to the output directory
#add_custom_command(TARGET ParticleSimulator POST_BUILD
#	COMMAND ${CMAKE_COMMAND} -E copy_if_different
#		"${CMAKE_CURRENT_SOURCE_DIR}/vcpkg_installed/vcpkg/blds/sdl2/x64-windows-dbg/SDL2d.dll"
#		$<TARGET_FILE_DIR:ParticleSimulator>)