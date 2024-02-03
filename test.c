#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <raylib.h>
#include <raymath.h>

const int width = 800;
const int height = 600;

const int font_size = 20;
const char *hint = "Press SPACE to start";

#define COLOR_COUNT 8
const Color colors[COLOR_COUNT] = {
    BLUE, RED, GREEN, PURPLE, LIME, SKYBLUE, ORANGE, DARKPURPLE
};

int get_sign(float num) {
    return (num >= 0)? 1: -1;
}


int main() {
    InitWindow(width, height, "Window");

    Color bg_color = GetColor(0x1a1d23ff);
    size_t color_i = 0; // index into colors array
    Rectangle box = {width/2-50, height/2-50, 50, 50};

    // hitboxes
    Vector2 top_left = {0, 0};
    Vector2 top_right = {width, 0};
    Vector2 bot_left = {0, height};
    Vector2 bot_right = {width, height};

    Vector2 velocity = {300, 300};
    
    Rectangle player_rect = {GetMouseX(), GetMouseY(), 200, 10};

    int count = 0;
    bool paused = true;
    Vector2 player_vel = {0, 0};
    float deccel = 1;

    while (!WindowShouldClose()) {
        Rectangle next_box;
        Rectangle next_box2;
        float dt = GetFrameTime();
        if (!paused) {
            if (fabs(velocity.x) >= 500)
                velocity.x -= deccel;
            if (fabs(velocity.y) >= 500)
                velocity.y -= deccel;


            Vector2 new_pos = {box.x + velocity.x * dt, box.y + velocity.y * dt};

            if (new_pos.x <= 0 || new_pos.x + box.width >= width) {
                count++;
                velocity.x = -velocity.x;
                color_i = (color_i + 1) % COLOR_COUNT;
            } else {
                box.x = new_pos.x;
            }

            if (new_pos.y <= 0 || new_pos.y + box.height >= height) {
                count++;
                velocity.y = -velocity.y;
                color_i = (color_i + 1) % COLOR_COUNT;
            } else {
                box.y = new_pos.y;
            }
            
            Rectangle next_box = {box.x + velocity.x * dt, box.y + velocity.y * dt, box.width, box.height};
            if (CheckCollisionRecs(next_box, player_rect)) {
                velocity.x = -velocity.x;
                velocity.y = -velocity.y;
            }
        }

        if (IsKeyPressed(KEY_SPACE))
            paused = !paused;

        player_vel = (Vector2){(GetMouseX() - player_rect.x)*dt, (GetMouseY() - player_rect.y)*dt};
        player_rect.x += player_vel.x;
        player_rect.y += player_vel.y;
        

        if (player_rect.x <= 0)
            player_rect.x = 0;
        else if (player_rect.x + player_rect.width >= width)
            player_rect.x = width - player_rect.width;

        if (player_rect.y <= 0)
            player_rect.y = 0;
        else if (player_rect.y + player_rect.height >= height) 
            player_rect.y = height - player_rect.height;

    

        BeginDrawing();
        ClearBackground(bg_color);
        DrawRectangleRec(box, colors[color_i]);
        DrawRectangleRec(player_rect, RED);
        DrawText(TextFormat("%lf", (velocity.x + velocity.y)/2), 10, 5, 30, WHITE);
        DrawText(TextFormat("%lf", (player_vel.x + player_vel.y)/2), 10, 35, 30, WHITE);

        if (paused) {
            int hint_width = MeasureText(hint, font_size);
            DrawText(hint, width/2 - hint_width/2, height/2 + box.height, 20, WHITE);
        }

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
