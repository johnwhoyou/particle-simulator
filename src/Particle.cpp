#include "Particle.h"

constexpr double degToRad(double degrees) {
    return degrees * M_PI / 180.0;
}

double normalizeAngle(double angle) {
    return fmod(fmod(angle, 360.0) + 360.0, 360.0);
}

Particle::Particle(double x, double y, double angle, double velocity)
    : x(x), y(y), angle(normalizeAngle(angle)), velocity(velocity) {}

void Particle::update(double deltaTime) {
    move(deltaTime);
}

void Particle::move(double deltaTime) {
    x += std::cos(degToRad(angle)) * velocity * deltaTime;
    y += std::sin(degToRad(angle)) * velocity * deltaTime;
}

void Particle::bounceOffWall(const Wall& wall) {
    double dx = wall.getX2() - wall.getX1();
    double dy = wall.getY2() - wall.getY1();

    double nx = dy;
    double ny = -dx;

    double length = std::sqrt(nx * nx + ny * ny);
    nx /= length;
    ny /= length;

    double radians = degToRad(angle);
    double vx = std::cos(radians) * velocity;
    double vy = std::sin(radians) * velocity;

    double dotProduct = vx * nx + vy * ny;
    double reflected_vx = vx - 2 * dotProduct * nx;
    double reflected_vy = vy - 2 * dotProduct * ny;

    angle = std::atan2(reflected_vy, reflected_vx) * 180.0 / M_PI;
    angle = normalizeAngle(angle);
}

double Particle::getX() const {
    return x;
}

double Particle::getY() const {
    return y;
}

double Particle::getAngle() const {
    return angle;
}

double Particle::getVelocity() const {
	return velocity;
}