#include <raylib.h>

#include <cmath>

#include "headers/vector.hpp"

Vector::Vector()
{
    startPos = {0.0f, 0.0f};
    endPos = {0.0f, 0.0f};
    color = WHITE;
}

Vector::Vector(Vector2 startPos, Vector2 endPos)
{
    this->startPos = startPos;
    this->endPos = endPos;

    color = WHITE;
}

void Vector::updateStartPos(Vector2 newStartPos)
{
    startPos = newStartPos;
}

void Vector::updateEndPos(Vector2 newEndPos)
{
    endPos = newEndPos;
}

void Vector::calcVector(Vector2 move)
{
    // Normalize the move vector to get direction
    float len = sqrtf(move.x * move.x + move.y * move.y);

    Vector2 direction = {move.x / len, move.y / len};
    
    // Get perpendicular vector
    Vector2 perpendicular = {-direction.y, direction.x};

    // Arrow tip at the end
    vertex1.x = endPos.x;
    vertex1.y = endPos.y;

    // Arrow base corners
    float arrowLength = 15.0f;
    float arrowWidth = 8.0f;
    
    vertex2.x = endPos.x - arrowLength * direction.x + arrowWidth * perpendicular.x;
    vertex2.y = endPos.y - arrowLength * direction.y + arrowWidth * perpendicular.y;
    
    vertex3.x = endPos.x - arrowLength * direction.x - arrowWidth * perpendicular.x;
    vertex3.y = endPos.y - arrowLength * direction.y - arrowWidth * perpendicular.y;
}

void Vector::drawVector() const
{
    DrawLineV(startPos, endPos, WHITE);
    DrawTriangle(vertex1, vertex3, vertex2, WHITE);
}

void Vector::updateArrow(Vector2 move)
{
    Vector::calcVector(move);
}