#include <gtest/gtest.h>
#include "Particle.h"

TEST(ParticleTest, ConstructorTest) {
    Particle particle(100.0f, 150.0f, 90.0f, 10.0f);
    EXPECT_FLOAT_EQ(particle.getX(), 100.0f);
    EXPECT_FLOAT_EQ(particle.getY(), 150.0f);
}

TEST(ParticleTest, UpdateTest) {
    Particle particle(100.0f, 150.0f, 0.0f, 100.0f);
    particle.update(1.0f); // Update for 1 second
    EXPECT_FLOAT_EQ(particle.getX(), 200.0f);
    EXPECT_FLOAT_EQ(particle.getY(), 150.0f);
}

TEST(ParticleTest, BounceOffWallTest) {
    Particle particle(10.0f, 20.0f, 45.0f, 100.0f);

    // Simulate a bounce on the east wall of the canvas (90 degrees)
    // and check if particle's new angle is correct
    particle.bounceOffWall(90.0f);
    float expectedAngleAfterBounce = 180.0f - 45.0f;
    EXPECT_FLOAT_EQ(particle.getAngle(), expectedAngleAfterBounce);

    // Simulate a bounce on a user-generated wall with a 30deg angle
    // and check if particle's new angle is correct
    particle.bounceOffWall(30.0f);
    expectedAngleAfterBounce = (2 * 30.0f - expectedAngleAfterBounce) + 360.0f;
    EXPECT_FLOAT_EQ(particle.getAngle(), expectedAngleAfterBounce);
}

// Add a test for move() check if particle's new position is correct