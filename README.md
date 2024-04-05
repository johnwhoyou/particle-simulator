# Problem Set #3: Distributed Particle Simulator

## About the Project
This project aims to apply distributed computing concepts by implementing a distributed particle physics simulator. To achieve this, a Java Server as well as a C++ Client application were developed to showcase an interface that can allow users to render particles into the simulation. The C++ Client application makes use of the Dear ImGui library for its graphical user interface, as well as SDL2 for its rendering functionalities. Meanwhile, the Java Server application utilizes the Java Swing library for the user interface and rendering. 

The main features of the Java Server application include adding a single particle, batch adding particles with varying spawn points, batch adding particles with varying angles, and batch adding particles with varying velocities. Through these features, users will be able to perform the entire simulation on a 1280 x 720 canvas, witnessing the particles' movements as they collide against the edges of the canvas.

On the other hand, the C++ Client application allows the user to explore the simulation environment maintained by the Java Server by controlling the movement of a sprite using the keyboard or on-screen arrow buttons. The sprite can be moved in all of the four cardinal directions, and a zoomed in 33 x 19 grid serves as the sprite's periphery. Particles or edges that fall within this periphery are rendered until they go out of view. The sprites of other clients connected to the server are also visible from the C++ Client application when they fall within the sprite's periphery. Additionally, the sprites of all connected clients are visible on the Java Server application as well. 

## Important Note
The source code for the C++ Client and Java Server can be found in separate branches. The C++ Client is in branch client, while the Java Server is in branch server. 