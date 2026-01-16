#include <raylib.h>

#define RAYGUI_IMPLEMENTATION
#include "raygui/raygui.h"

#include <string>
#include <cstddef>
#include <vector>
#include <random>

#include <iostream>

#include "headers/point.hpp"

constexpr float G = 6.67f * 10.0f;

using namespace std;

bool showGui = false;

float radius = 10.0f; // Example radius value
float mass = 10.0f;  // Example mass value

float moveVectorX = 0.0f, moveVectorY = 0.0f;

Rectangle guiBlock = {0, 0, 400, 400};

Vector2 mousePos;

bool symulationRunning = false;

void DrawGUI(Vector2 position)
{
    guiBlock.x = position.x;
    guiBlock.y = position.y;

    DrawRectangleRec(guiBlock, GRAY);
    DrawText("GUI Panel", (int)position.x + 10, (int)position.y + 10, 20, WHITE);

    GuiSetStyle(DEFAULT, TEXT_SIZE, 30);

    GuiSliderBar((Rectangle){ position.x + 150, position.y + 40, 200, 50}, "Radius", TextFormat("%.0f", radius), &radius, 0, 200);

    // if(GuiButton(Rectangle{position.x + 10, position.y + 40, 140.0f, 30.0f}, "Increase Radius"))
    // {
    //     radius += 1.0f;
    // }
    // if(GuiButton(Rectangle{position.x + 150, position.y + 40, 140.0f, 30.0f}, "Decrease Radius"))
    // {
    //     radius -= 1.0f;
    //     if(radius < 1.0f) radius = 1.0f;
    // }
    // DrawText(TextFormat("Radius: %.2f", radius), (int)position.x + 10, (int)position.y + 80, 20, WHITE);


    GuiSliderBar((Rectangle){ position.x + 150, position.y + 120, 200, 50}, "Mass", TextFormat("%.0f", mass, 30), &mass, 0, 200);


    GuiSliderBar((Rectangle){ position.x + 150, position.y + 200, 200, 60}, "X vector", TextFormat("%.0f", moveVectorX, 30), &moveVectorX, -50, 50);
    GuiSliderBar((Rectangle){ position.x + 150, position.y + 260, 200, 60}, "Y vector", TextFormat("%.0f", moveVectorY, 30), &moveVectorY, -50, 50);
    // if(GuiButton(Rectangle{position.x + 10, position.y + 120, 140.0f, 30.0f}, "Increase Mass"))
    // {
    //     mass += 1.0f;
    // }
    // if(GuiButton(Rectangle{position.x + 150, position.y + 120, 140.0f, 30.0f}, "Decrease Mass"))
    // {
    //     mass -= 1.0f;
    //     if(mass < 1.0f) mass = 1.0f;
    // }
    // DrawText(TextFormat("Mass: %.2f", mass), (int)position.x + 10, (int)position.y + 160, 20, WHITE);

    if(GuiButton(Rectangle{position.x + 10.0f, position.y + guiBlock.height - 70.0f, 380.0f, 50.0f}, "Close GUI"))
    {
        showGui = false;
    }
}

void colision(std::vector<Point> &points)
{
    for (size_t i = 0; i < points.size(); i++)
        for (size_t j = i + 1; j < points.size(); j++)
            if (CheckCollisionCircles(points[i].getPosition(), points[i].getRadius(), points[j].getPosition(), points[j].getRadius()))
            {
                float rTotal = points[i].getRadius() + points[j].getRadius();
                
                Vector2 posI = points[i].getPosition();
                Vector2 posJ = points[j].getPosition();
                Vector2 diff = { posJ.x - posI.x, posJ.y - posI.y };
                float dist = sqrtf(diff.x * diff.x + diff.y * diff.y);
                if (dist == 0.0f)
                {
                    diff = { 1.0f, 0.0f };
                    dist = 1.0f;
                }
                
                float overlap = rTotal - dist;
                if (overlap <= 0.0f) continue;
                
                float totalMass = points[i].getMass() + points[j].getMass();
                float moveI = overlap * (points[j].getMass() / totalMass);
                float moveJ = overlap * (points[i].getMass() / totalMass);
                
                Vector2 norm = { diff.x / dist, diff.y / dist };
                
                points[i].getPosition().x -= norm.x * moveI;
                points[i].getPosition().y -= norm.y * moveI;

                points[j].getPosition().x += norm.x * moveJ;
                points[j].getPosition().y += norm.y * moveJ;
            }
}


vector<Point> stars;

void createStars(int width, int height)
{
    random_device rd;
    mt19937 mt(rd());
    uniform_int_distribution<int> rand(100, 1000);
    uniform_int_distribution<int> randX(0, width);
    uniform_int_distribution<int> randY(0, height);

    uniform_real_distribution<float> r(1.0f, 3.0f);

    for(size_t i = 0; i < stars.size(); i++)
    {
        stars.emplace_back(Point{(Vector2){(float)randX(mt), (float)randY(mt)}, (float)r(mt), (float)1.0f, (Vector2){ 0.0f, 0.0f }});
    }
}

void drawStars()
{
    for(auto s : stars)
    {
        DrawCircleV(s.getPosition(), s.getRadius(), WHITE);
    }
}

void calcGravity(vector<Point> &points)
{
    for(size_t i = 0; i < points.size(); i++)
    {
        float ax = 0, ay = 0;

        for(size_t j = 0; j < points.size(); j++)
        {
            if(i == j) continue;

            float dx = points[j].getPosition().x - points[i].getPosition().x;
            float dy = points[j].getPosition().y - points[i].getPosition().y;
            float distSq = dx*dx + dy*dy;
            float dist = sqrt(distSq);

            if(dist < 5.0f) dist = 5.0f; // Zabezpieczenie

            // F = G * m1 * m2 / r^2
            // a = F / m1  =>  a = G * m2 / r^2
            float force = (G * points[j].getMass()) / (dist * dist);

            ax += force * (dx / dist);
            ay += force * (dy / dist);
        }

        // 2. Zaktualizuj POZYCJĘ o prędkość
        points[i].getPosition().x += ax;
        points[i].getPosition().y += ay;
    }
}

void calcMouseGravity()
{
        
}

int main ()
{
    int windowWidth = GetScreenWidth();
    int windowHeight = GetScreenHeight();
    string title = "Planets Simulation";

    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_MSAA_4X_HINT);
    InitWindow(windowWidth, windowHeight, title.c_str());
    SetWindowMinSize(200, 200);

    createStars(windowWidth, windowHeight);

    // int FPS = 120;
    // SetTargetFPS(FPS);

    vector<Point> points;

    while (!WindowShouldClose())
    {
        if(IsKeyPressed(KEY_F11))
            ToggleFullscreen();

        BeginDrawing();

            ClearBackground(BLACK);

            drawStars();

            if(!symulationRunning)
            {
                if(IsMouseButtonPressed(MOUSE_RIGHT_BUTTON))
                {
                    showGui = true;
                    mousePos = GetMousePosition();            
                }
            }

            for(size_t i = 0; i < points.size(); ++i)
            {
                points[i].draw();
                colision(points);
            }

            if(showGui && !symulationRunning)
            {
                DrawGUI(mousePos);
            }
            else
            {
                if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && !symulationRunning)
                {
                    mousePos = GetMousePosition();
                    points.emplace_back(Point{mousePos, radius, mass, (Vector2){ moveVectorX, moveVectorY }});
                }
            }

            if(IsKeyPressed(KEY_SPACE))
                symulationRunning = !symulationRunning;

            if(symulationRunning)
            {
                calcGravity(points);
                for(size_t i = 0; i < points.size(); i++)
                    points[i].movePoint();
            }

            DrawFPS(10, 10);
            DrawText(TextFormat("Count: %d", points.size()), 10, 40, 20, DARKGREEN);            

        EndDrawing();   
    }
}