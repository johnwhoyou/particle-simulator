# Problem Set #1: Particle Simulator

## TO-DO
1. Remove walls - Aless
2. Toggle between developer and explorer mode - Aless
3. Create explorer mode UI - Aless
4. Spawning a sprite at a pre-defined location - Shaun (TRY)
5. Moving using on-screen arrow keys, WASD, or arrow keys - Shaun (TRY)
6. Show zoomed in version of sprite's periphery - Aless (tentative)
7. Render only the things in the sprite's periphery - John
8. Remember last location of sprite when switching between modes - Ibz


## About the Project
This project aims to apply distributed computing concepts by implementing a concurrently programmed particle physics simulator. To achieve this, a multithreaded C++ application was developed to showcase an interface that can allow users to render particles and walls into the simulation. The application makes use of the Dear ImGui library for its graphical user interface, as well as SDL2 for its rendering functionalities.

The main features of the application include adding a single particle, batch adding particles with varying spawn points, batch adding particles with varying angles, batch adding particles with varying velocities, and adding a wall. Through these features, users will be able to perform the entire simulation on a 1280 x 720 canvas, witnessing the particles' movements as they collide against the edges of the canvas or against the user-generated walls.

## How to Run the Project
1. Clone the repository to your local machine.
2. Ensure that you have CMake installed (at least version 3.14).
3. Ensure that you have the `vcpkg` package manager installed in your Visual Studio environment. This can be checked using the Visual Studio installer.
4. Once the project is open in VS, open a Developer Powershell terminal and enter the command `vcpkg integrate install` to enable library linking.
5. Enter the command `vcpkg install` to install the following dependencies required by the project:
	- imgui
	- SDL2
6. Build the project.
7. Run the project by running the `Main.cpp` file or the `ParticleSimulatorApp` executable that will appear on the Startup Item dropdown. Make sure to use the Local Machine x64-Debug configuration.

## Important Note
The single-threaded and multi-threaded implementations of the project can be found in separate branches. The single-threaded version is in branch main, while the other is in branch multithreaded.
