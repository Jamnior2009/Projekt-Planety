#pragma once

#include <raylib.h>

#include <vector>

class Point
{
private:
    Vector2 position;
    float radius;
    float mass;

    float accelerationX, accelerationY;

    Vector2 moveVector;

    std::vector<Color> colors;
    Color color;

public:
    Point(Vector2 position, float radius = 10.0f, float mass = 10.0f, Vector2 moveVector = { 0.0f, 0.0f });

    void draw() const;

    float &getRadius() { return radius; };

    float &getMass() { return mass; };

    float &getAccelerationX() { return accelerationX; };

    float &getAccelerationY() { return accelerationY; };

    Vector2 &getPosition() { return position; };

    Vector2 &getMoveVector() { return moveVector; };

    void movePoint();
};