#include <gtest/gtest.h>
#include "Particle.h"

TEST(ParticleTest, Constructor) {
    Particle particle(100.0f, 150.0f, 90.0f, 10.0f);
    EXPECT_FLOAT_EQ(particle.getX(), 100.0f);
    EXPECT_FLOAT_EQ(particle.getY(), 150.0f);
}

TEST(ParticleTest, Update) {
    Particle particle(100.0f, 150.0f, 0.0f, 100.0f);
    particle.update(1.0f); // Update for 1 second
    EXPECT_FLOAT_EQ(particle.getX(), 200.0f);
    EXPECT_FLOAT_EQ(particle.getY(), 150.0f);
}