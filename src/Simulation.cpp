#include "Simulation.h"

void Simulation::addParticle(int x, int y, double angle, double velocity) {
    particles.emplace_back(x, y, angle, velocity);
}

void Simulation::addWall(int x1, int y1, int x2, int y2) {
    walls.emplace_back(x1, y1, x2, y2);
}

std::vector<Particle> Simulation::getParticles() {
	return particles;
}