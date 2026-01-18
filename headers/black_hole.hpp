#pragma once

#ifndef BLACK_HOLE_HPP
#define BLACK_HOLE_HPP

#include <vector>

#include "headers/point.hpp"

class BlackHole : public Point
{
public:
    BlackHole(Vector2 position);

    void draw();
};

#endif