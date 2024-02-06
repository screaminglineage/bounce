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
    float radius = 100;
    Vector2 ball = {width/2, height/2};

    size_t color_i = 0; // index into colors array

    const char* hint = "Press SPACE to Start!";
    const char* controls = "Z/X to adjust speed";
    int font_size = 30;
    int hint_width = MeasureText(hint, font_size);
    int controls_width = MeasureText(controls, font_size);

    Vector2 velocity = {300, 300};
    
    Sound corner_sfx = LoadSound("sounds/corner.wav");
    Sound bounce_sfx = LoadSound("sounds/bounce.wav");


    int count = 0;
    bool ready = true; // only readies if it hasnt hit corner before
    bool paused = true;

    SetTargetFPS(60);
    while (!WindowShouldClose()) {
        
        if (!paused) {
            float dt = GetFrameTime();
            Vector2 new_pos = Vector2Add((Vector2){ball.x, ball.y}, Vector2Scale(velocity, dt)); 

            // collision detection
            if (new_pos.x - radius <= 0 || new_pos.x + radius >= width) {
                velocity.x = -velocity.x;
                PlaySound(bounce_sfx);
            } else {
                ball.x = new_pos.x;
            }

            if (new_pos.y - radius <= 0 || new_pos.y + radius >= height) {
                velocity.y = -velocity.y;
                PlaySound(bounce_sfx);
            } else {
                ball.y = new_pos.y;
            }

            if (velocity.y <= 1000)
                velocity.y += 25;
        }

        switch(GetKeyPressed()) {
            case KEY_SPACE: {
                paused = !paused;
                break;
            }
            case KEY_Z: {
                if (!paused) {
                    velocity = Vector2ClampValue(
                        Vector2Subtract(
                            velocity, 
                            Vector2Scale(vector2_sign(velocity), 100)
                        ),
                        200,
                        1500
                    );

                }
                break;
            }            
            case KEY_X: {
                if (!paused) {
                    velocity = Vector2ClampValue(
                        Vector2Add(
                            velocity, 
                            Vector2Scale(vector2_sign(velocity), 100)
                        ),
                        200,
                        1500
                    );

                }
                break;
            }
            case KEY_A: {
                if (!paused) {
                    radius = Clamp(radius - 10, 20, 200); 
                }
                break;
            }
            case KEY_S: {
                if (!paused) {
                    radius = Clamp(radius + 10, 20, 200); 
                }
                break;
            }
        }

        BeginDrawing();
        ClearBackground(bg_color);

        DrawCircleV(ball, radius, RED);

        DrawText(TextFormat("Radius: %d", (int)radius), 10, 5, font_size, WHITE);
        DrawText(TextFormat("Speed: %d", (int)Remap(Vector2Length(velocity), 200, 1500, 1, 13)), 10, font_size, font_size, WHITE);
        if (paused) {
            DrawText(hint, width/2 - hint_width/2, height/2 + radius + 30, font_size, WHITE);
            DrawText(controls, width/2 - controls_width/2, height/2 + radius + 60, font_size, WHITE);
        }

        EndDrawing();
    }
    
    UnloadSound(corner_sfx);
    UnloadSound(bounce_sfx);

    CloseAudioDevice();
    CloseWindow();

    return 0;
}
