# Problem Set #2: Particle Simulator

## About the Project
This project aims to apply distributed computing concepts by implementing a concurrently programmed particle physics simulator. To achieve this, a multithreaded C++ application was developed to showcase an interface that can allow users to render particles into the simulation. The application makes use of the Dear ImGui library for its graphical user interface, as well as SDL2 for its rendering functionalities. For client-server communication, the application implements a hybrid TCP/UDP approach.

The client application is always in Explorer Mode allowing the user to explore the simulation environment by controlling the movement of a sprite using the keyboard or on-screen arrow buttons. The sprite can be moved in all of the four cardinal directions, and a zoomed in 33 x 19 grid serves as the sprite's periphery. Particles or edges that fall within this periphery are rendered until they go out of view.

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
 	- SDL-net
  	- RapidJSON
6. Build the project.
7. Run the project by running the `Main.cpp` file or the `ParticleSimulatorApp` executable that will appear on the Startup Item dropdown. Make sure to use the Local Machine x64-Debug configuration.

## Important Note
The server application is in branch `server` this is required to be running before executing the client application.
