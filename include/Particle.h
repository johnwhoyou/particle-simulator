#ifndef PARTICLE_H
#define PARTICLE_H

#include "Wall.h"

class Particle {
public:
    Particle(double x, double y, double angle, double velocity);

    void update(double deltaTime);
    void bounceOffWall(const Wall& wall);

    double getX() const;
    double getY() const;
    double getAngle() const;

private:
    double x, y; // Position of the particle
    double angle; // Angle in degrees, 0 is east
    double velocity; // Velocity in pixels per second
    
    void move(double deltaTime);
    bool isParticleOnRightSideOfWall(const Wall& wall) const;
};

#endif