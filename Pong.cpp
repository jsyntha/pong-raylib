#include <iostream>
#include "raylib.h"
using namespace std;

int player_score = 0;
int cpu_score = 0;

class Ball {
public:
    float x, y;
    int speed_x, speed_y;
    int radius;

    void Draw() {
          DrawCircle(x, y, radius, WHITE);
    }

    void Update() {
        x += speed_x;
        y += speed_y;

        if (y + radius >= GetScreenHeight() || y - radius <= 0) {
            speed_y *= -1;
        }
        if (x + radius >= GetScreenWidth()) {
            cpu_score++;
            ResetBall();
        }
            if( x - radius <= 0) {
            player_score++;
            ResetBall();
        }
    }

    void ResetBall() {
        x = GetScreenWidth() / 2;
        y = GetScreenHeight() / 2;

        int speed_choices[2] = { -1,1 };
        speed_x *= speed_choices[GetRandomValue(0,1)];
        speed_y *= speed_choices[GetRandomValue(0,1)];
    }
};
class Paddle {
protected:
    void LimitMovement() {
        if (y <= 0) {
            y = 0;
        } if (y + height >= GetScreenHeight()) {
            y = GetScreenHeight() - height;
        }
    }
public:
    float x, y;
    float width, height;
    int speed;

    void Draw() {
        DrawRectangleRounded(Rectangle{ x, y, width, height }, 0.8, 0, WHITE);
    }
    void Update() {
        if (IsKeyDown(KEY_W)) {
            y = y - speed;
        }
        if (IsKeyDown(KEY_S)) {
            y = y + speed;
        }
        LimitMovement();
    }
};
class CPUPaddle : public Paddle {
public:
    void Update(int ball_y) {
        if (y + height / 2 > ball_y)
        {
            y = y - speed;
        }
    if (y + height / 2 <= ball_y) {
        y = y + speed;
    }
    LimitMovement();
    }
};
Ball ball;
Paddle player;
CPUPaddle cpu;

void CenterWindow(int width, int height);
bool displayFPS();
void icon();

int main() {
    const int window_width = 1280;
    const int window_height = 720;

    InitWindow(window_width, window_height, "Pong!");
    CenterWindow(window_width, window_height);
    icon();
    SetTargetFPS(60);

    ball.radius = 20;
    ball.x = window_width / 2;
    ball.y = window_height / 2;
    ball.speed_x = 7;
    ball.speed_y = 7;

    player.width = 25;
    player.height = 120;
    player.x = window_width - player.width - 10;
    player.y = window_height / 2 - player.height / 2;
    player.speed = 6;

    cpu.height = 120;
    cpu.width = 25;
    cpu.x = 10;
    cpu.y = window_height / 2 - cpu.height / 2;
    cpu.speed = 6;

    while (!WindowShouldClose()) {
        BeginDrawing();
        GetFPS();
        displayFPS();

        ball.Update();
        player.Update();
        cpu.Update(ball.y);

        if (CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{player.x, player.y, player.width, player.height})) {
            ball.speed_x *= -1;
        }
        if (CheckCollisionCircleRec(Vector2{ ball.x, ball.y }, ball.radius, Rectangle{ cpu.x, cpu.y, cpu.width, cpu.height })) {
            ball.speed_x *= -1;
        }
        //Drawing 
        ClearBackground(BLACK);
        DrawLine(window_width / 2, 0, window_width / 2, window_height, GRAY); // middle line
        ball.Draw();
        cpu.Draw();
        player.Draw();
        DrawText(TextFormat("%i",cpu_score), window_width / 4 - 20, 20, 80, WHITE);
        DrawText(TextFormat("%i", player_score), 3 * window_width / 4 - 20, 20, 80, WHITE);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}

void CenterWindow(int width, int height) {
    // Get the screen size
    int screen_width = GetMonitorWidth(0);
    int screen_height = GetMonitorHeight(0);

    // Calculate the centered position
    int posX = (screen_width - width) / 2;
    int posY = (screen_height - height) / 2;

    SetWindowPosition(posX, posY);
}
bool displayFPS() {
    static bool showFPS = false;

    if (IsKeyPressed(KEY_ONE)) {
        showFPS = !showFPS;
    }

    if (showFPS) {
        DrawFPS(10, 10);
    }

    return showFPS;
}
void icon() {
    Image icon = LoadImage("resources/ping-pong.png");
    SetWindowIcon(icon);
    UnloadImage(icon);
}