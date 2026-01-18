#include <raylib.h>

#include <cmath>
#include <algorithm>

#include "headers/black_hole.hpp"

#include "headers/point.hpp"

BlackHole::BlackHole(Vector2 position)
{
    Point::getPosition() = position;
    Point::getRadius() = 5.0f;
    Point::getMass() = 1000.0f;

    Point::getOutlineColor() = PURPLE;
}

void BlackHole::draw()
{
    DrawCircleLinesV(Point::getPosition(), Point::getRadius(), Point::getOutlineColor());
}