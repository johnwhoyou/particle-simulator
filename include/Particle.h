#ifndef PARTICLE_H
#define PARTICLE_H

#include "Wall.h"

class Particle {
public:
    Particle(int x, int y, double angle, double velocity);

    void update(double deltaTime);
    void bounceOffWall(const Wall& wall);

    int getX() const;
    int getY() const;
    double getAngle() const;

private:
    int x, y; // Position of the particle
    double angle; // Angle in degrees, 0 is east
    double velocity; // Velocity in pixels per second
    
    void move(double deltaTime);
    bool isParticleOnRightSideOfWall(const Wall& wall) const;
};

#endif