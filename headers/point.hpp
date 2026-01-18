#pragma once

#ifndef POINT_HPP
#define POINT_HPP

#include <raylib.h>

#include <cstddef>
#include <vector>

#include "headers/vector.hpp"

class Point
{
private:
    Vector2 position;
    float radius;
    float mass;

    float accelerationX, accelerationY;

    Vector2 moveVector;

    size_t imagesCount;

    Texture2D waterTexture, landTexture, cloudsTexture;

    Vector vector;

    Color outlineColor;

    // std::vector<Color> colors;
    // Color color;

public:
    Point();

    Point(Vector2 position, float radius = 10.0f, float mass = 10.0f, Vector2 moveVector = { 0.0f, 0.0f });

    void draw() const;

    float &getRadius() { return radius; };

    float &getMass() { return mass; };

    float &getAccelerationX() { return accelerationX; };

    float &getAccelerationY() { return accelerationY; };

    Vector2 &getPosition() { return position; };

    Vector2 &getMoveVector() { return moveVector; };

    void movePoint();

    Vector &getVectorObject() { return vector; };

    Color &getOutlineColor() { return outlineColor; };
};

#endif