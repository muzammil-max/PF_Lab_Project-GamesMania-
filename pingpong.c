#include <raylib.h>
#include "pingpong.h"
#include "engine.h"
#define screen_width 1080
#define screen_height 720


#define MAX_RAIN_SPOTS 60


// OBJECT DEFINITIONS
// Paddles made
Music bgMusic;
Rectangle player_1_paddle = {20, (float)screen_height / 2 - 50, 20, 100};
Rectangle player_2_paddle = {screen_width - 40, (float)screen_height / 2 - 50, 20, 100};

Vector2 ball_position = {0.5f * screen_width, 0.5f * screen_height};
Vector2 ball_velocity = {-4.0f, 4.0f};
int restart = 0;
int pong_score = 0;
int chances = 3;
bool game_over = false;
bool win = false;
int last_speed = 0;
typedef struct RainSpot
{
    Vector2 position;
    float speed;
} RainSpot;

int c1;
// const int screen_width = 1080;
// const int screen_height = 720;

RainSpot rain[MAX_RAIN_SPOTS];

void PlayPong()
{
    SetTargetFPS(100);
    for (int i = 0; i < MAX_RAIN_SPOTS; i++)
    {
        rain[i].position.x = GetRandomValue(0, screen_width);
        rain[i].position.y = GetRandomValue(-screen_height, 0);
        rain[i].speed = (float)GetRandomValue(2, 5);
    }
    // InitAudioDevice();
    bgMusic = LoadMusicStream("assets/Pingpong.mp3");
    PlayMusicStream(bgMusic);
    SetMusicVolume(bgMusic, 0.5f);
    
    // InitWindow(screen_width, screen_height, "Pong");
    // SetTargetFPS(60);
    
    // INITIALIZE NEON RAIN
}   
    void gameEngine3() {
        
        //check for restart stMusic bgMusicate
        
        UpdateMusicStream(bgMusic);
        if (game_over == true && IsKeyPressed(KEY_R)) {
            pong_score = 0;
            chances= 3;
            game_over = false;
            win = false; 
            last_speed = 0;
            ball_position =(Vector2){0.5f * screen_width, 0.5f * screen_height};
            ball_velocity = (Vector2){-4.0f, 4.0f};
            player_1_paddle.y = (screen_height/2) -50;
            player_2_paddle.y = (screen_height/2) -50;
            PlayMusicStream(bgMusic);
    SetTargetFPS(100);


        }

        if (!game_over)
        {
            // keep the neon ray falling
            for (int i = 0; i < MAX_RAIN_SPOTS; i++)
            {

                // pull the pixels down based on their individual speed
                rain[i].position.y += rain[i].speed;

                // Check if the pixel escaped the bottom of the screen
                if (rain[i].position.y > screen_height)
                {

                    rain[i].position.y = -10;

                    // doesn't look repetitive shuffle the horizontal spot so the pattern
                    rain[i].position.x = GetRandomValue(0, screen_width);
                }
            }

            ball_position.x += ball_velocity.x;
            ball_position.y += ball_velocity.y;

            if (ball_position.y <= 0 || ball_position.y >= screen_height)
                ball_velocity.y *= -1;

            player_1_paddle.y = ball_position.y - (player_1_paddle.height / 2);

            if (CheckCollisionCircleRec(ball_position, 15, player_1_paddle))
            {
                ball_velocity.x *= -1;
                ball_position.x = player_1_paddle.x + player_1_paddle.width + 15;
            }

            if (CheckCollisionCircleRec(ball_position, 15, player_2_paddle))
            {
                ball_velocity.x *= -1;
                pong_score++;
                c1+=3;
                ball_position.x = player_2_paddle.x - 15;
                if(c1>20) {
                    score++;
                    c1 = 0;
                }
                // win condition
                if (pong_score >= 30)
                {
                    win = true;
                }

                if (pong_score > 0 && pong_score % 10 == 0 && pong_score != last_speed)
                {
                    ball_velocity.x *= 1.3f;
                    ball_velocity.y *= 1.3f;
                    last_speed = pong_score;
                }
            }

            if (ball_position.x < 0)
                ball_position = (Vector2){screen_width / 2.0f, screen_height / 2.0f};
            if (ball_position.x > screen_width)
            {
                chances--;
                if (chances <= 0)
                    game_over = true;
                else
                    ball_position = (Vector2){screen_width / 2.0f, screen_height / 2.0f};
            }

            if (IsKeyDown(KEY_UP) && player_2_paddle.y > 0)
                player_2_paddle.y -= 6.0f;
            if (IsKeyDown(KEY_DOWN) && player_2_paddle.y < (screen_height - player_2_paddle.height))
                player_2_paddle.y += 6.0f;
        }

        BeginDrawing();
        ClearBackground(BLACK);

        // neon ray background
        for (int i = 0; i < MAX_RAIN_SPOTS; i++)
        {
            DrawCircleGradient(rain[i].position.x, rain[i].position.y, 4, Fade(SKYBLUE, 0.6f), BLACK);
            DrawCircle(rain[i].position.x, rain[i].position.y, 1.5f, WHITE);
        }

        if (!game_over)
        {
            // AI paddle
            DrawRectangle(player_1_paddle.x - 5, player_1_paddle.y - 5, player_1_paddle.width + 10, player_1_paddle.height + 10, Fade(RED, 0.3f));
            DrawRectangleGradientV(player_1_paddle.x, player_1_paddle.y, player_1_paddle.width, player_1_paddle.height, SKYBLUE, BLUE);
            DrawRectangleLinesEx(player_1_paddle, 2, WHITE);

            // player paddle
            DrawRectangle(player_2_paddle.x - 5, player_2_paddle.y - 5, player_2_paddle.width + 10, player_2_paddle.height + 10, Fade(SKYBLUE, 0.3f));
            DrawRectangleGradientV(player_2_paddle.x, player_2_paddle.y, player_2_paddle.width, player_2_paddle.height, SKYBLUE, BLUE);
            DrawRectangleLinesEx(player_2_paddle, 2, WHITE);

            // ball drawing
            DrawCircleGradient(ball_position.x, ball_position.y, 22, Fade(PURPLE, 0.4f), BLUE);
            DrawCircleV(ball_position, 15, WHITE);

            // right side heading
            DrawText(TextFormat("SCORE: %d", pong_score), screen_width - 200, 20, 30, RAYWHITE);
            DrawText(TextFormat("LIVES: %d", chances), screen_width - 200, 50, 30, RED);

            // left side heading
            DrawText("MODE: AI", 40, 20, 30, RED);

            // Display Winning text and keep the game running
            if (win)
            {
                DrawText("GOAL ACHIEVED: 30", screen_width / 2 - 150, 40, 30, GREEN);
            }
        }
        else
        {
            StopMusicStream(bgMusic);
            DrawText("GAME OVER!", screen_width / 2 - 150, 200, 50, RED);
            DrawText("Press R to continue", screen_width / 2 - 150, 310, 20, WHITE);

            DrawText(TextFormat("FINAL SCORE: %d", pong_score), screen_width / 2 - 100, 270, 30, WHITE);
            // DrawText(TextFormat("FINAL SCORE: %d", score), screen_width / 2 - 100, 270, 30, WHITE);

            // Special message if they lost but achieved the goal
            if (win)
            {
                DrawText("YOU ACHIEVED THE TARGET!", screen_width / 2 - 160, 320, 25, GREEN);
            }
        }
        EndDrawing();
    }

    
    
