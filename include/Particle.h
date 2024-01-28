#ifndef PARTICLE_H
#define PARTICLE_H

class Particle {
public:
    Particle(double x, double y, double angle, double velocity);
    void updatePosition(double timeDelta);
    // Add other necessary methods and member variables here

private:
    double x, y;      // Position of the particle
    double angle;     // Angle in degrees (0 is east, increases anticlockwise)
    double velocity;  // Velocity in pixels per second

    // Method to handle collision, might need more parameters based on canvas and walls
    void handleCollision();
};

#endif
