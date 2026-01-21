#include <raylib.h>

#define RAYGUI_IMPLEMENTATION
#include "raygui/raygui.h"

#include <string>
#include <cstddef>
#include <vector>
#include <algorithm>

#include "headers/point.hpp"

#include "headers/black_hole.hpp"

constexpr float G = 6.67f * 6.0f;

using namespace std;

bool showGui = false;

float radius = 10.0f;
float mass = 10.0f;

float moveVectorX = 0.0f, moveVectorY = 0.0f;

Rectangle guiBlock = {0.0f, 0.0f, 400.0f, 800.0f};

Vector2 mousePos;

bool symulationRunning = false;

void DrawGUI(Vector2 position, vector<BlackHole> &blackHoles)
{
    guiBlock.x = position.x;
    guiBlock.y = position.y;

    DrawRectangleRec(guiBlock, GRAY);
    DrawText("GUI Panel", (int)position.x + 10, (int)position.y + 10, 20, WHITE);

    GuiSetStyle(DEFAULT, TEXT_SIZE, 30);

    GuiSliderBar((Rectangle){ position.x + 150, position.y + 40, 200, 50}, "Radius", TextFormat("%.0f", radius), &radius, 1, 200);

    GuiSliderBar((Rectangle){ position.x + 150, position.y + 120, 200, 50}, "Mass", TextFormat("%.0f", mass, 30), &mass, 1, 200);

    GuiSliderBar((Rectangle){ position.x + 150, position.y + 200, 200, 60}, "X vector", TextFormat("%.0f", moveVectorX, 30), &moveVectorX, -75, 75);
    
    GuiSliderBar((Rectangle){ position.x + 150, position.y + 260, 200, 60}, "Y vector", TextFormat("%.0f", moveVectorY, 30), &moveVectorY, -75, 75);

    if(GuiButton(Rectangle{position.x + 10.0f, position.y + guiBlock.height - 160.0f, 380.0f, 50.0f}, "BLACK HOLE"))
    {
        blackHoles.emplace_back((BlackHole){ mousePos });
        showGui = false;
    }

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

                points[i].getVectorObject().updateStartPos( points[i].getPosition() );
                points[i].getVectorObject().updateEndPos((Vector2){ points[i].getPosition().x + points[i].getMoveVector().x, points[i].getPosition().y + points[i].getMoveVector().y });
                points[i].getVectorObject().updateArrow(points[i].getMoveVector());

                points[j].getVectorObject().updateStartPos( points[j].getPosition() );
                points[j].getVectorObject().updateEndPos((Vector2){ points[j].getPosition().x + points[j].getMoveVector().x, points[j].getPosition().y + points[j].getMoveVector().y });
                points[j].getVectorObject().updateArrow(points[j].getMoveVector());
            }
}

void blackHoleColision(std::vector<Point> &points, vector<BlackHole> &blackHoles)
{
    for (size_t i = 0; i < blackHoles.size(); i++)
        for (size_t j = 0; j < points.size(); j++)
            if(CheckCollisionCircles(blackHoles[i].getPosition(), blackHoles[i].getRadius(), points[j].getPosition(), points[j].getRadius()))
            {
                Point temp = points[j];
                points[j] = points[points.size() - 1];
                points[points.size() - 1] = temp;

                points.erase(points.begin() + j);
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

            if(dist < 5.0f) dist = 5.0f;

            float force = (G * points[j].getMass()) / (dist * dist);

            ax += force * (dx / dist);
            ay += force * (dy / dist);
        }
 
        points[i].getPosition().x += ax;
        points[i].getPosition().y += ay;
    }
}

void calcBlackHoleGravity(vector<Point> &points, vector<BlackHole> &blackHoles)
{
    for(size_t i = 0; i < points.size(); i++)
    {
        float ax = 0, ay = 0;

        for(size_t j = 0; j < blackHoles.size(); j++)
        {
            float dx = blackHoles[j].getPosition().x - points[i].getPosition().x;
            float dy = blackHoles[j].getPosition().y - points[i].getPosition().y;

            float distSq = dx*dx + dy*dy;
            float dist = sqrt(distSq);

            if(dist < 5.0f) dist = 5.0f;

            float force = (G * blackHoles[j].getMass()) / (dist * dist);

            ax += force * (dx / dist);
            ay += force * (dy / dist);
        }
 
        points[i].getPosition().x += ax;
        points[i].getPosition().y += ay;
    }
}

void drawPoints(vector<Point> &points)
{
    for(size_t i = 0; i < points.size(); ++i)
        points[i].draw();
    
    colision(points);
}

void drawBalckHoles(vector<Point> &points, vector<BlackHole> &blackHoles)
{
    for(size_t i = 0; i < blackHoles.size(); i++)
        blackHoles[i].draw();

    blackHoleColision(points, blackHoles);
}

int main ()
{
    int windowWidth = GetScreenWidth();
    int windowHeight = GetScreenHeight();
    string title = "Planet Simulation";

    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_MSAA_4X_HINT);
    InitWindow(windowWidth, windowHeight, title.c_str());
    SetWindowMinSize(200, 200);

    vector<Point> points;
    vector<BlackHole> blackHoles;

    while (!WindowShouldClose())
    {
        if(IsKeyPressed(KEY_F11))
            ToggleFullscreen();
        

        BeginDrawing();

            ClearBackground(BLACK);

            if(!symulationRunning)
            {
                if(IsMouseButtonPressed(MOUSE_RIGHT_BUTTON))
                {
                    showGui = true;
                    mousePos = GetMousePosition();            
                }
            }

            drawPoints(points);
            drawBalckHoles(points, blackHoles);

            if(showGui && !symulationRunning)
            {
                DrawGUI(mousePos, blackHoles);
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
                calcBlackHoleGravity(points, blackHoles);
                for(size_t i = 0; i < points.size(); i++)
                    points[i].movePoint();
            }

            DrawFPS(10, 10);
            DrawText(TextFormat("Count: %d", points.size()), 10, 40, 20, DARKGREEN);

            string state = "Symulation: "; state += (symulationRunning ? "Running" : "Stoped");
            DrawText(state.c_str(), 10, 70, 20, DARKGREEN);

            DrawText("Controls: ", windowWidth - 160, windowHeight - 100, 20, WHITE);
            // DrawText("Controls: ", windowWidth - 80, windowHeight - 50, 20, WHITE);
            // DrawText("Controls: ", windowWidth - 80, windowHeight - 50, 20, WHITE);
            // DrawText("Controls: ", windowWidth - 80, windowHeight - 50, 20, WHITE);

        EndDrawing(); 
    }

    return 0;
}