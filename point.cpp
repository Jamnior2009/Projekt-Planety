#include <raylib.h>

#include <string>

#include "headers/point.hpp"

std::string waterS = "water_";
std::string landS = "land_";
std::string cloudS = "cloud_";

Point::Point(Vector2 position, float radius, float mass, Vector2 moveVector) : position(position), radius(radius), mass(mass), moveVector(moveVector)
{
    imagesCount = 2;

    std::string imageString1 = "./textures/waters/" + waterS + std::to_string(GetRandomValue(1, imagesCount)) + ".png";
    Image img1 = LoadImage(imageString1.c_str());
    waterTexture = LoadTextureFromImage(img1);

    Image img2 = LoadImage(("./textures/lands/" + landS + std::to_string(GetRandomValue(1, imagesCount)) + ".png").c_str());
    landTexture = LoadTextureFromImage(img2);

    Image img3 = LoadImage(("./textures/clouds/" + cloudS + std::to_string(GetRandomValue(1, imagesCount)) + ".png").c_str());
    cloudsTexture = LoadTextureFromImage(img3);

    // colors.push_back(RED);
    // colors.push_back(GREEN);
    // colors.push_back(BLUE);
    // colors.push_back(YELLOW);
    // colors.push_back(ORANGE);
    // colors.push_back(PURPLE);

    // color = colors[GetRandomValue(0, colors.size() - 1)];

    vector = Vector(position, (Vector2){ position.x + moveVector.x, position.y + moveVector.y });

    vector.calcVector(moveVector);
}

void Point::draw() const
{
    DrawCircleLinesV(position, radius, WHITE);

    DrawTextureEx(waterTexture, (Vector2){ position.x - radius, position.y - radius }, 0.0f, (2 * radius / 16.0f) , WHITE);
    DrawTextureEx(landTexture, (Vector2){ position.x - radius, position.y - radius }, 0.0f, (2 * radius / 16.0f) , WHITE);
    DrawTextureEx(cloudsTexture, (Vector2){ position.x - radius, position.y - radius }, 0.0f, (2 * radius / 16.0f) , WHITE);

    vector.drawVector();
}

void Point::movePoint()
{
    position.x += moveVector.x * 10.0f / GetFPS();
    position.y += moveVector.y * 10.0f / GetFPS();

    vector.updateStartPos(position);
    vector.updateEndPos((Vector2){ position.x + moveVector.x, position.y + moveVector.y });
    vector.calcVector(moveVector);
}