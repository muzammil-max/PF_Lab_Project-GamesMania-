#include <raylib.h>
#include "brickbreaker.h"
#include "engine.h"

#define ROWS 10
#define COLS 10

typedef struct Brick
{
    Rectangle rect;
    bool active;
    Color color;
} Brick;

float ballRadius;
    Vector2 ballVel;
Rectangle paddle;
const int screenWidth = 1080;
Vector2 ballPos;
Brick bricks[ROWS][COLS];
float brickWidth = 90;
float brickHeight = 20;
float padding = 8;
float offsetTop = 60;
const int screenHeight = 720;
Music bgMusic1;
float offsetLeft;
int b_score = 0;
int lives = 3;
bool gameOver = false;
bool gameWon = false;

void Initbb()
{
    offsetLeft = (screenWidth - (COLS * (brickWidth + padding))) / 2;

    // initialize the music
    // InitAudioDevice();
    // detect the music from the folder
     bgMusic1 = LoadMusicStream("assets/Brickbreaker.mp3");
    PlayMusicStream(bgMusic1);

    // InitWindow(screenWidth, screenHeight, "GamesMania");
    SetTargetFPS(80);

    // laying out the brick positions

    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            // keep the bricks centered and spaced out
            bricks[i][j].rect = (Rectangle){offsetLeft + j * (brickWidth + padding), offsetTop + i * (brickHeight + padding), brickWidth, brickHeight};
            bricks[i][j].active = true;

            // same color for whole row
            if (i < 2)
                bricks[i][j].color = RED;
            else if (i < 4)
                bricks[i][j].color = ORANGE;
            else if (i < 6)
                bricks[i][j].color = YELLOW;
            else if (i < 8)
                bricks[i][j].color = GREEN;
            else
                bricks[i][j].color = BLUE;
        }
    }

    // position of paddle and ball
    paddle = (Rectangle){screenWidth / 2 - 60, screenHeight - 40, 120, 20};
    ballPos = (Vector2){screenWidth / 2, screenHeight / 2 + 100};
    ballVel = (Vector2){3.5f, -3.5f};
    ballRadius = 10.0f;
}

// void resetforrestart(bool x) {
//     if (x == true  && IsKeyPressed(KEY_R)){

//          // reset game state
//         gameOver = false;
//         gameWon = false;
//         b_score = 0;
//         lives = 3;

//         ballPos = (Vector2){screenWidth / 2, screenHeight / 2 + 100};
//         ballVel = (Vector2){3.5f, -3.5f};

//         // reset bricks
//         for (int i = 0; i < ROWS; i++)
//         {
//             for (int j = 0; j < COLS; j++)
//             {
//                 bricks[i][j].active = true;
//             }

//     }
// }

void GameEngine4()
{

    
        // keep the music on while the game runs
        UpdateMusicStream(bgMusic1);

        if (!gameOver && !gameWon)
        {
            // controls through keys
            if (IsKeyDown(KEY_LEFT) && paddle.x > 0)
                paddle.x -= 10;
            if (IsKeyDown(KEY_RIGHT) && paddle.x < screenWidth - paddle.width)
                paddle.x += 10;

            // move the ball based on its current speed and direction
            ballPos.x += ballVel.x;
            ballPos.y += ballVel.y;

            // bounce off the side walls
            if (ballPos.x <= ballRadius || ballPos.x >= screenWidth - ballRadius)
                ballVel.x *= -1;
            // Bounce off the ceiling
            if (ballPos.y <= ballRadius)
                ballVel.y *= -1;

            // ball hitting with the paddle
            if (CheckCollisionCircleRec(ballPos, ballRadius, paddle))
            {
                ballVel.y *= -1;
                ballPos.y = paddle.y - ballRadius; // prevent the ball from stucking
            }

            // loop through every brick to see if the ball smashed it
            for (int i = 0; i < ROWS; i++)
            {
                for (int j = 0; j < COLS; j++)
                {
                    if (bricks[i][j].active)
                    {
                        if (CheckCollisionCircleRec(ballPos, ballRadius, bricks[i][j].rect))
                        {
                            bricks[i][j].active = false; // breaking of the bricks
                            ballVel.y *= -1;             // bounce the ball back
                            b_score++;

                            // if you break all bricks you win
                            if (b_score >= 100)
                                gameWon = true;
                        }
                    }
                }
            }

            // check if the ball pass the player
            if (ballPos.y > screenHeight)
            {
                lives--;
                // chances finished game over
                if (lives <= 0)
                    gameOver = true;
                else
                {
                    // Reset the ball to the middle for the next try
                    ballPos = (Vector2){screenWidth / 2, screenHeight / 2 + 100};
                    ballVel = (Vector2){3.5f, -3.5f};
                }
            }
        }

        BeginDrawing();
        ClearBackground(BLACK);

        if (!gameOver && !gameWon)
        {
            // bricks that have not been broken yet
            for (int i = 0; i < ROWS; i++)
            {
                for (int j = 0; j < COLS; j++)
                {
                    if (bricks[i][j].active)
                    {
                        DrawRectangleRec(bricks[i][j].rect, bricks[i][j].color);
                        DrawRectangleLinesEx(bricks[i][j].rect, 1, BLACK);
                    }
                }
            }

            // draw the player paddle and the ball
            DrawRectangleRec(paddle, RAYWHITE);
            DrawCircleV(ballPos, ballRadius, WHITE);

            // headings of score and lives
            DrawText(TextFormat("BRICKS BROKEN: %d / 100", b_score), 20, 20, 20, RAYWHITE);
            DrawText(TextFormat("LIVES: %d", lives), screenWidth - 150, 20, 20, RED);
        }
        else if (gameWon)
        {
            // victory screen display
            DrawText("CONGRATULATIONS!", screenWidth / 2 - 200, screenHeight / 2 - 50, 40, GREEN);
            DrawText("SYSTEM BYPASSED: 100 BRICKS DESTROYED", screenWidth / 2 - 230, screenHeight / 2 + 10, 20, RAYWHITE);
        }
        else
        {
            DrawText("GAME OVER", screenWidth / 2 - 140, screenHeight / 2 - 50, 50, RED);
            DrawText(TextFormat("FINAL SCORE: %d", b_score), screenWidth / 2 - 100, screenHeight / 2 + 20, 25, WHITE);

            DrawText("PRESS R TO RESTART", screenWidth / 2 - 150, screenHeight / 2 + 80, 20, RAYWHITE);

            if (IsKeyPressed(KEY_R))
            {
                // reset game state
                gameOver = false;
                gameWon = false;
                b_score = 0;
                lives = 3;

                ballPos = (Vector2){screenWidth / 2, screenHeight / 2 + 100};
                ballVel = (Vector2){3.5f, -3.5f};

                // reset bricks
                for (int i = 0; i < ROWS; i++)
                {
                    for (int j = 0; j < COLS; j++)
                    {
                        bricks[i][j].active = true;
                    }
                }
            }
        }
        EndDrawing();
    }

    
