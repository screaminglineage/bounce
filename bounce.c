#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <raylib.h>
#include <raymath.h>

const int width = 800;
const int height = 600;

#define COLOR_COUNT 10

Vector2 vector2_sign(Vector2 vec) {
    return (Vector2){(vec.x >= 0)? 1: -1, (vec.y >= 0)? 1: -1};
}

int main() {
    Color colors[COLOR_COUNT] = {
        BLUE, RED, YELLOW, GREEN, WHITE, PURPLE, LIME, SKYBLUE, ORANGE, DARKPURPLE
    };

    InitWindow(width, height, "Window");
    InitAudioDevice();      

    Color bg_color = GetColor(0x1a1d23ff);
    Vector2 box_size = {100, 100};
    Rectangle box = {width/2 - box_size.x/2, height/2 - box_size.y/2, box_size.x, box_size.y};

    size_t color_i = 0; // index into colors array

    const char* hint = "Press SPACE to Start!";
    const char* controls = "Z/X to adjust speed";
    int font_size = 30;
    int hint_width = MeasureText(hint, font_size);
    int controls_width = MeasureText(controls, font_size);

    // hitboxes
    Vector2 hitbox = {10, 10};
    Rectangle top_left = {0, 0, hitbox.x, hitbox.y};
    Rectangle top_right = {width - hitbox.x, 0, hitbox.x, hitbox.y};
    Rectangle bot_left = {0, height - hitbox.y, hitbox.x, hitbox.y};
    Rectangle bot_right = {width-hitbox.x, height-hitbox.y, hitbox.x, hitbox.y};

    Vector2 velocity = {300, 300};
    
    Sound corner_sfx = LoadSound("sounds/corner.wav");
    Sound bounce_sfx = LoadSound("sounds/bounce.wav");


    int count = 0;
    bool ready = true; // only readies if it hasnt hit corner before
    bool paused = true;
    while (!WindowShouldClose()) {
        
            float dt = GetFrameTime();
        if (!paused) {
            Vector2 new_pos = {box.x + velocity.x * dt, box.y + velocity.y * dt};

            // collision detection
            if (new_pos.x <= 0 || new_pos.x + box.width >= width) {
                velocity.x = -velocity.x;
                PlaySound(bounce_sfx);
            } else {
                box.x = new_pos.x;
            }

            if (new_pos.y <= 0 || new_pos.y + box.height >= height) {
                velocity.y = -velocity.y;
                PlaySound(bounce_sfx);
            } else {
                box.y = new_pos.y;
            }
        }

        // check if box hits corner
        bool hit_corner = (CheckCollisionRecs(top_left, box)
                || CheckCollisionRecs(top_right, box)
                || CheckCollisionRecs(bot_left, box)
                || CheckCollisionRecs(bot_right, box));


        if (hit_corner) {
            if (ready) {
                ready = false;
                count++;
                color_i = (color_i + 1) % COLOR_COUNT;
                PlaySound(corner_sfx);
            } 
        } else {
            ready = true;
        }
        if (IsKeyPressed(KEY_SPACE)) 
            paused = !paused;

        
        if (IsKeyPressed(KEY_Z) && !paused) { 
            velocity = Vector2ClampValue(
                Vector2Subtract(
                    velocity, 
                    Vector2Scale(vector2_sign(velocity), 100)
                ),
                200,
                1500
            );

        }
        if (IsKeyPressed(KEY_X) && !paused) { 
            velocity = Vector2ClampValue(
                Vector2Add(
                    velocity, 
                    Vector2Scale(vector2_sign(velocity), 100)
                ),
                200,
                1500
            );

        }

        BeginDrawing();
        ClearBackground(bg_color);

        DrawRectangleRec(box, colors[color_i]);

        DrawText(TextFormat("%d", count), 10, 5, font_size, WHITE);
        DrawText(TextFormat("Speed: %d", (int)Remap(Vector2Length(velocity), 200, 1500, 1, 13)), 10, font_size, font_size, WHITE);
        if (paused) {
            DrawText(hint, width/2 - hint_width/2, height/2 + box.height/2 + 30, font_size, WHITE);
            DrawText(controls, width/2 - controls_width/2, height/2 + box.height/2 + 60, font_size, WHITE);
        }

        EndDrawing();
    }
    
    UnloadSound(corner_sfx);
    UnloadSound(bounce_sfx);

    CloseAudioDevice();
    CloseWindow();

    return 0;
}
