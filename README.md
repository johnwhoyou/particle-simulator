# Problem Set #2: Particle Simulator

## TO-DO
Server
- [x] remove walls - ibz
- [ ] accept connections from clients continouously - ibz
- [x] create sprite classes - shaun
- [ ] spawn/render sprites on screen when clients connect - shaun
- [ ] make sprites disappear when client disconnectst - shaun
- [ ] assign client IDs to new clients - shaun
- [ ] receive sprite location/data from client - ibz
- [ ] send filtered particle data to clients - ibz
- [ ] send other sprite data to clients - ibz


## About the Project
This project aims to apply distributed computing concepts by implementing a concurrently programmed particle physics simulator. To achieve this, a multithreaded C++ application was developed to showcase an interface that can allow users to render particles into the simulation. The application makes use of the Dear ImGui library for its graphical user interface, as well as SDL2 for its rendering functionalities.

The application has two modes that the user can toggle between: Developer Mode and Explorer Mode.

The main features of the application's Developer Mode include adding a single particle, batch adding particles with varying spawn points, batch adding particles with varying angles, and batch adding particles with varying velocities. Through these features, users will be able to perform the entire simulation on a 1280 x 720 canvas, witnessing the particles' movements as they collide against the edges of the canvas.

On the other hand, the application's Explorer Mode allows the user to explore the simulation environment by controlling the movement of a sprite using the keyboard or on-screen arrow buttons. The sprite can be moved in all of the four cardinal directions, and a zoomed in 33 x 19 grid serves as the sprite's periphery. Particles or edges that fall within this periphery are rendered until they go out of view.

## How to Run the Project
1. Clone the repository to your local machine.
2. Ensure that you have CMake installed (at least version 3.14).
3. Ensure that you have the `vcpkg` package manager installed in your Visual Studio environment. This can be checked using the Visual Studio installer.
4. Once the project is open in VS, open a Developer Powershell terminal and enter the command `vcpkg integrate install` to enable library linking.
5. Enter the command `vcpkg install` to install the following dependencies required by the project:
	- imgui
	- SDL2
	- SDL2-Image
	- libpng
	- zlib
6. Build the project.
7. Run the project by running the `Main.cpp` file or the `ParticleSimulatorApp` executable that will appear on the Startup Item dropdown. Make sure to use the Local Machine x64-Debug configuration.

## Important Note
The single-threaded and multi-threaded implementations of the project can be found in separate branches. The single-threaded version is in branch main, while the other is in branch multithreaded.
