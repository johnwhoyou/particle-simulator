# Problem Set #2: Particle Simulator

## About the Project
This project aims to apply distributed computing concepts by implementing a concurrently programmed particle physics simulator. To achieve this, a multithreaded C++ application was developed to showcase an interface allowing users to render particles into the simulation. The application uses the Java Swing library for its graphical user interface and rendering. For server-client communication, the application implements a hybrid TCP/UDP approach wherein events such as sprite and particle movement are sent through UDP protocol. However, to check for active client connections, TCP protocol is used through a heartbeat mechanism.

The server application is in Developer mode allowing a user to add a single particle, batch-adding particles with varying spawn points, batch-adding particles with varying angles, and batch-adding particles with varying velocities.

## How to Run the Project
1. Clone the repository to your local machine.
2. Ensure that you have Java JDK 19 or earlier.
3. Ensure that `gson-2.10.1.jar` is listed as a dependency. To check, go to File > Project Settings > Modules > particle-simulator > Dependencies.
6. Build and run the project.

## Important Note
The source code for the C++ Client is found in `client` branch.
