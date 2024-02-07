#include <gtest/gtest.h>
#include "Particle.h"

const float epsilon = 0.01f;  // for floating-point comparison tolerance

// helper function to convert degrees to radians for test setup
constexpr float degToRad(float degrees) {
    return degrees * M_PI / 180.0f;
}

// helper function to normalize the angle to be within [0, 360)
constexpr float normalizeAngle(float angle) {
    return fmod(fmod(angle, 360.0f) + 360.0f, 360.0f);
}

// TEST(ParticleTest, ConstructorTest) {
//     Particle particle(100.0f, 150.0f, 90.0f, 10.0f);
//     EXPECT_FLOAT_EQ(particle.getX(), 100.0f);
//     EXPECT_FLOAT_EQ(particle.getY(), 150.0f);
// }

// TEST(ParticleTest, UpdateTest) {
//     Particle particle(100.0f, 150.0f, 0.0f, 100.0f);
//     particle.update(1.0f); // Update for 1 second
//     EXPECT_FLOAT_EQ(particle.getX(), 200.0f);
//     EXPECT_FLOAT_EQ(particle.getY(), 150.0f);
// }

TEST(ParticleTest, BounceOffWallTest) {
    // a particle coming from south-west at 225 degrees
    Particle particle(10.0f, 20.0f, 225.0f, 100.0f);

    Wall eastWall(1280.0f, 720.0f, 1280.0f, 0.0f);
    particle.bounceOffWall(eastWall);
    float expectedAngleAfterBounce = 135.0f;  // particle should bounce off to north-west at 135deg
    EXPECT_NEAR(particle.getAngle(), expectedAngleAfterBounce, epsilon);

    // a user-generated wall with a 30 degrees angle
    Wall customWall(0.0f, 0.0f, cos(degToRad(30.0f)), sin(degToRad(30.0f)));
    particle.bounceOffWall(customWall);
    expectedAngleAfterBounce = 105.0f;
    EXPECT_NEAR(particle.getAngle(), expectedAngleAfterBounce, epsilon);
}