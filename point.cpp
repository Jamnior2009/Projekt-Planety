#include <raylib.h>

#include "headers/point.hpp"

Point::Point(Vector2 position, float radius, float mass, Vector2 moveVector) : position(position), radius(radius), mass(mass), moveVector(moveVector)
{
    colors.push_back(RED);
    colors.push_back(GREEN);
    colors.push_back(BLUE);
    colors.push_back(YELLOW);
    colors.push_back(ORANGE);
    colors.push_back(PURPLE);

    color = colors[GetRandomValue(0, colors.size() - 1)];
}

void Point::draw() const
{
    DrawCircleV(position, radius, color);
    // DrawText(TextFormat("X: %f ,Y: %f", position.x, position.y), position.x, position.y, 20, WHITE);
}

void Point::movePoint()
{
    position.x += moveVector.x * 10.0f / GetFPS();
    position.y += moveVector.y * 10.0f / GetFPS();
}