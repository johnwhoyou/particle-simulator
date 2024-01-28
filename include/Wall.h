#ifndef WALL_H
#define WALL_H

class Wall {
public:
    Wall(float x1, float y1, float x2, float y2);

    float getX1() const;
    float getY1() const;
    float getX2() const;
    float getY2() const;

    float getAngle() const; // Useful for calculating bounce angles

private:
    float x1, y1; // First endpoint of the wall
    float x2, y2; // Second endpoint of the wall
};

#endif // WALL_H