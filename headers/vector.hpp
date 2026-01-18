#pragma once

#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <raylib.h>

#include "headers/vector.hpp"

class Vector
{
private:
    Vector2 startPos, endPos;

    Vector2 vertex1, vertex2, vertex3;
    Color color;

public:
    Vector();

    Vector(Vector2 startPos, Vector2 endPos);

    void updateStartPos(Vector2 newStartPos);

    void updateEndPos(Vector2 newEndPos);

    void calcVector(Vector2 move);

    void drawVector() const;

    void updateArrow(Vector2 move);
};

#endif