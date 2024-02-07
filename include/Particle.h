#ifndef PARTICLE_H
#define PARTICLE_H

#include "Wall.h"

class Particle {
public:
    Particle(float x, float y, float angle, float velocity);

    void update(float deltaTime);
    void bounceOffWall(const Wall& wall);

    float getX() const;
    float getY() const;
    float getAngle() const;

private:
    float x, y; // Position of the particle
    float angle; // Angle in degrees, 0 is east
    float velocity; // Velocity in pixels per second
    
    void move(float deltaTime);
    bool isParticleOnRightSideOfWall(const Wall& wall) const;
};

#endif