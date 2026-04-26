//! For logic:
//!  https://miro.com/app/board/uXjVHcuZoLU=/?share_link_id=174680451795

#include <stdio.h>
#include "raylib.h"
#include "flappy.h"
#include "engine.h"


// FLAPPY BIRD
// THIS GAME IS MADE BY Muzammil
// Using Raylib
// the bird physics logic was taken help from organic chem tutor
// this game is mostly aboutphysics and mechanics and about free fal motion
// THIS IS NOT AI CODE 100% HUMMAN CODE !!!!
// THE VS CODE HAS BUILDINT FORMAT
// SLANGS USED ARE MADE BY DEVELOPER WHO KEEPS GETTING FRUTRATED LOL
// SOME VARIABLES ARENT EVEN USED TOO
// CODE IS ALSO BUGGY



float scorescale = 1.0f;
float basespeed = 120;
float speed;
float BirdX; // maybe udse it later
float BirdY;
float gravity = 0.5f;
float velocity;
int lose = 0;
int flappypoints = 0;
float jump = -8.00f; // when u press space it will trigger it
float pipeX[3];
float pipeHeight[3];
int scored[3]; // there will be 3 pipes genereating each pipe having random height
int pipenumber = 3;
int pipewidth = 70;
int gap = 190;
int swidth = 500;
int sheight = 600;
int pipespeed = 3;
// float scaleY; //for setting up background texture in Y
// float scaleX; //for X
int BirdFrame = 0;
int birdcounter = 0; // using for framing so that bird animates
// b1 to b2 to b3 and then look back to b1 again and so on , doing this in frmae by frame
// float groundmovementhahaha = 0;
void FlappyStart()
{
    SetTargetFPS(65);
    // this function will bring back to original state when newgame or gameover
    BirdY = 200.0f;
    // gravity = 0.5f;
    lose = 0;
    velocity = 0.0f;
    flappypoints = 0;

    for (int i = 0; i <= pipenumber - 1; i++)
    {
        pipeX[i] = swidth + (i * 300);
        pipeHeight[i] = GetRandomValue(100, 300);
    }
}

void GameEngine1()
{
    int groundheight = ground.height;

    if (lose == 1 && IsKeyPressed(KEY_R))
    {
        FlappyStart(); // start new game
    }
    if (lose == 1)
    {
        return;

        // freezes the game
        //Ignore it
    }

    if (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_UP))
    {
        velocity = jump;
    }

    // PHYSICS MY FAV !!!1111
    speed = basespeed + flappypoints * 4.0f;

    // this snippet is the main physics
    velocity += gravity;
    BirdY += velocity;

    if (BirdY < 0.0f)
    {
        BirdY = 0.0f;
        velocity = 0.0f;
    }
    else if (BirdY > sheight - 30)
    {
        // bird ki height is 30 already so if we dont minus it will the whole bird will out of screen
        BirdY = sheight - 30; // takes bird to cneter
        velocity = 0.0f;
    }

    Rectangle bird = {
        100,   //!     x pos of birf
        BirdY, // #  y pos of bird
        25,    //!      width of bird
        25     //*       height of birdf meoww
    };

    //!! PIPE LOGIC
    for (int m = 0; m <= pipenumber - 1; m++)
    {

        pipeX[m] -= speed * GetFrameTime(); // move the pipe to left

        // if pipe is out of bound then reset the poition

        if (pipeX[m] < -swidth)
        {
            pipeX[m] = swidth;
            pipeHeight[m] = GetRandomValue(100, sheight - gap - 100); // obviously man this will shift the pipe to right so the height will be random
            scored[m] = 0;                                            // remeber this game is running in frames , and the object width is 30 (score box) so when
            // Also the scoring system is a bit buggy so will check it later!
            /*

            imaigne u are at pipe 1 and u passe thru the gapbox now for some time u will still be inside that box because frame by frame
            so it will increase the score unfairly when adding score it acts as a state when once hit gap state is 1 and will be 0 until new pipe geneerate
            */
        }

        // Failed!
        //! Ground logic (move it beckwards)
        // groundmovementhahaha -= 2;

        // if (groundmovementhahaha <= -ground.width)
        // {
        //     groundmovementhahaha = 0;
        // }

        // Bird ANimation
        // every 5 frames of game  increase the bird frame by 1
        birdcounter++;
        if (birdcounter > 10)
        {
            BirdFrame++;
            birdcounter = 0;
        }
        // loop back to the fiest frame of bird when last frame reach
        if (BirdFrame > 2)
        {
            BirdFrame = 0;
        }
        // collision logic

        Rectangle topPipe = {
            pipeX[m], // x position
            0,
            pipewidth,
            pipeHeight[m]

        };
        Rectangle bottomPipe = {
            pipeX[m],
            pipeHeight[m] + gap,
            pipewidth,
            sheight - (pipeHeight[m] + gap)}; // isse phr ground will not overlap woth pipes

        /// texture for the pipes

        // effects
        
        
        if (scorescale > 1.0f)
        {
            scorescale -= 0.03f;
            if (scorescale < 1.0f)
                scorescale = 1.0f;
        }

        //! GAMEOVER
        if (CheckCollisionRecs(bird, topPipe) || CheckCollisionRecs(bird, bottomPipe))
        {
            // gameover
            // shake = 10;
            lose = 1;
        }
        // also if bird out sides bounds then gameover

        if (BirdY <= 0 || BirdY >= sheight - 30)
        {
            lose = 1;
        }

        // SCoring
        // idea: making gap box that will act as a trigger when player pases thru it , it will increase score by 1
        //  when the player get score there will be a delay of 1 second
        // making rectangle of gapbox because yeh score he 8)

        Rectangle gapbox = {
            pipeX[m] + 15,
            pipeHeight[m],
            30,
            gap};

        if (CheckCollisionRecs(bird, gapbox) && scored[m] == 0)
        {
            flappypoints++;
            score++;
            scored[m] = 1;
            scorescale = 1.5f;
        }
    }
}

void DrawFlappy()
{

   

            ClearBackground(BLACK);
    DrawTexturePro(
        bg_day,
        (Rectangle){0, 0, bg_day.width, bg_day.height},         // make fkin rectangle giving the width and height of image
                         (Rectangle){0, 0, GetScreenWidth(), GetScreenHeight()}, // now stretch the rectangle such that oit cover whole screen
        (Vector2){0, 0},                                        // Texture centre me hga so 0,0
        0.0f,                                                   // no need for rotation
        WHITE);

    // bird
    // DrawRectangle(100, (int)BirdY, 30, 30, YELLOW); //commenting this shi because we be adding texture instead of rect
    if (BirdFrame == 0)
    {
       
       
       
       
       
       
        DrawTextureEx(b1, (Vector2){100, BirdY}, 0.0f, 1.0f, WHITE);
    }
    else if (BirdFrame == 1)
    {
        DrawTextureEx(b2, (Vector2){100, BirdY}, 0.0f, 1.0f, WHITE);
    }
    else
    {
        DrawTextureEx(b3, (Vector2){100, BirdY}, 0.0f, 1.0f, WHITE);
    }

    // drawing pipes

    for (int y = 0; y <= pipenumber - 1; y++)
    {
        // top pipe

        // DrawRectangle(pipeX[y], 0, pipewidth, pipeHeight[y], GREEN);
        // DrawTexture(pipe1, pipeX[m], pipeHeight[m] + gap, WHITE);

        // base pipe

        // DrawRectangle(pipeX[y], pipeHeight[y] + gap, pipewidth, sheight, GREEN);

        // score
        DrawText(
            TextFormat("Coins: %d", flappypoints),
            20,
            20,
            30 * scorescale,
            (flappypoints > 10) ? GOLD : YELLOW);
        // bottomt pipe

        DrawTexture(pipe1, pipeX[y], pipeHeight[y] + gap, WHITE);
        // top pipe
        DrawTexture(pipe2, pipeX[y], pipeHeight[y] - pipe2.height, WHITE);

        // gameover screen

        if (lose == 1)
        {
            DrawText("GAME OVER", swidth / 2 - 80, sheight / 2 - 50, 30, RED);
            DrawText("Press R to Restart", swidth / 2 - 100, sheight / 2, 20, WHITE);
        }

    }
    // ground texture (FAILED TO ADD)
    // DrawTextureEx(ground, (Vector2){groundmovementhahaha, sheight - ground.height}, 0.0f, 1.0f, WHITE); //texture will stay with groundmoventhaha which is X while the ground.
    // DrawTextureEx(ground, (Vector2){groundmovementhahaha + ground.width, sheight - ground.height}, 0.0f, 1.0f, WHITE);
}
