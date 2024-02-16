#ifndef PARTICLE_H
#define PARTICLE_H
#define _USE_MATH_DEFINES

#include <cmath>
#include "Wall.h"

class Particle {
public:
    Particle(double x, double y, double angle, double velocity);

    void update(double deltaTime);
    void bounceOffWall(const Wall& wall);

    double getX() const;
    double getY() const;
    double getAngle() const;
    double getVelocity() const;

private:
    double x, y;
    double angle;
    double velocity;
    
    void move(double deltaTime);
};

#endif