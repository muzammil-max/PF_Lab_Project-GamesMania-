#include "raylib.h"
#include <stdio.h>
#include "flappy.h"
#include "engine.h"
#include "tetris.h"
#include "pingpong.h"
#include "brickbreaker.h"

/* //todo

This is the main engine of game which is basically called linker
This code contains games which comes as states
meaining each games depends upon state variables which in in fact ddependent to score lmao
There are menus and buttons
No use of mouse only kjeyboard

Some part of this code was taken from raysan and tweaks were made
credit to raylib owner

Logic building:
https://miro.com/app/board/uXjVHcuZoLU=/?share_link_id=174680451795


*/

// THIS IS NOT AI CODE
// EVEYTHING WRITTEN BY 2 FCKING HUMANS
// todo flappy done and need to add tetris OH MY GOD I need some rest

int score = 0;
int tetriState = 0;
int pongState = 0;
int breakerState = 0;

// textue set krna zrori boss  T T
Texture2D b1, b2, b3;
Texture2D bg_day;
Texture2D ground;
Texture2D pipe1;
Texture2D pipe2;
// int myfps = 120;
void taketocenter()
{
    int monitor_width = GetMonitorWidth(0);
    int monitor_height = GetMonitorHeight(0);

    int difference_X = monitor_width - GetScreenWidth();
    int difference_Y = monitor_height - GetScreenHeight();
    SetWindowPosition(
        difference_X / 2, difference_Y / 2);
}

void resetall() // resets all shit
{
    score = 0;
    tetriState = 0;
    pongState = 0;
    breakerState = 0;
}

// this function uodates the states
void checkLocks() // useless mf which made me fustated
{
    tetriState = (score >= 25) ? 1 : 0;
    pongState = (score >= 100 && tetriState == 1) ? 1 : 0;
    breakerState = (score >= 200 && pongState == 1) ? 1 : 0;
}

void save() // to save the game prigress
{
    checkLocks();

    FILE *f = fopen("save.txt", "w");
    if (f != NULL)
    {
        fprintf(f, "%d %d %d %d", score, tetriState, pongState, breakerState); // here first is score then state variables , these variables i think are useless so i might deleteit
        fclose(f);
    }
}

void load()
{
    FILE *f = fopen("save.txt", "r");
    if (f != NULL)
    {
        fscanf(f, "%d %d %d %d", &score, &tetriState, &pongState, &breakerState);
        fclose(f);
    }
    else
    {
        resetall();
    }

    checkLocks();
    save();
}

void LoadFlappyTextures()
{
    b1 = LoadTexture("assets/b1.png");
    b2 = LoadTexture("assets/b2.png");
    b3 = LoadTexture("assets/b3.png");
    bg_day = LoadTexture("assets/bg_day.png");
    ground = LoadTexture("assets/ground.png");
    pipe1 = LoadTexture("assets/pipe.png"); // lesson earned never make vaibale pipe other wise face the consequences- Miss Qurat Ul Ain
    pipe2 = LoadTexture("assets/pipe2.png");
}

int main()
{
    int gamestate = 0;   // this is the state variable for switching games it is bugggy
    int state = 0;       // state acts as screen
    float faketimer = 0; // fake timer is for loading for natural look
    float loading = 0;   // loading variable to control the bar
    float screentimer = 0;
    int selected = 0;
    // myfps = 120;

    InitWindow(1500, 550, "GamesMania"); // making windows 1500x600 (todo : center window)
    LoadFlappyTextures();                // this function loads all texture to GPU
    load();
    SetTargetFPS(60);

    InitAudioDevice();
    Music epikmusic = LoadMusicStream("assets/m1.ogg");
    PlayMusicStream(epikmusic);

    while (WindowShouldClose() == 0)
    {
        // added sw and sh  for compataiblity on other screeens
        // it means no screen issues lesgo
        int sw = GetScreenWidth();
        int sh = GetScreenHeight();

        BeginDrawing();
        ClearBackground(BLACK);

        if (state == 0)
        {
            faketimer += GetFrameTime();
            loading += 50 * GetFrameTime();
            // loading += 100 * GetFrameTime(); use this if u want more delay

            DrawText("Loading Assets...", sw / 2 - 200, sh / 2 - 50, 50, WHITE);
            DrawRectangle(sw / 2 - 250, sh / 2 + 20, loading * 5, 30, RED);

            if (loading >= 100)
            {
                loading = 100;
                // loading = 300; buggy
            }

            if (faketimer > 6) //! this is buggy will fix later (FIXED!!!)
            {
                state = 1;
            }
        }
        else if (state == 1)
        {
            screentimer += GetFrameTime();

            DrawText("GAMES MANIA", sw / 2 - 350, sh / 2 - 50, 110, GREEN);
            DrawText("By Sohaib and Muzammil", sw / 2 - 250, sh / 2 + 80, 30, GREEN);

            if (screentimer > 2)
            {
                screentimer = 0;
                state = 2;
            }
        }
        else if (state == 2)
        {
            UpdateMusicStream(epikmusic);

            if (IsKeyPressed(KEY_DOWN))
                selected++;
            else if (IsKeyPressed(KEY_UP))
                selected--;

            if (selected < 0)
                selected = 1;
            else if (selected > 1)
                selected = 0;

            if (IsKeyPressed(KEY_ENTER))
            {
                if (selected == 0)
                {
                    resetall(); //! FIx it soon (by Sohaib)
                    save();
                    state = 3;
                }
                else if (selected == 1)
                {
                    load();
                    state = 3;
                }
            }

            if (IsKeyPressed(KEY_C))
            {
                score++;
                checkLocks();
                save();
            }
            // sw and sh are variables that are used to make eveything relative to screen
            DrawText(TextFormat("Coins: %d", score), 20, 20, 20, YELLOW);
            DrawText("Games Mania", sw / 2 - 250, sh / 2 - 140, 90, WHITE);
            DrawText("New Game", sw / 2 - 60, sh / 2 - 20, 20, selected == 0 ? YELLOW : WHITE);
            DrawText("Load Game", sw / 2 - 60, sh / 2 + 30, 20, selected == 1 ? YELLOW : WHITE);
        }
        else if (state == 3)
        {
            checkLocks(); // this funvtion will keep updating the progress

            if (gamestate == 0)
            {
                if (IsKeyPressed(KEY_DOWN))
                    selected++;
                else if (IsKeyPressed(KEY_UP))
                    selected--;

                if (selected < 0)
                    selected = 3;
                if (selected > 3)
                    selected = 0;

                // sohaib you miust comment these out later they are just for debugging pruposes
                DrawText(TextFormat("Coins: %d", score), 20, 20, 20, YELLOW);
                DrawText(TextFormat("select: %d", selected), 20, 40, 20, YELLOW);

                if (IsKeyPressed(KEY_C))
                {
                    score++;
                    checkLocks();
                    save();
                }

                UpdateMusicStream(epikmusic);

                DrawText("Flappy Bird", sw / 2 - 70, sh / 2 - 20, 20, selected == 0 ? YELLOW : WHITE);
                // these are states variables they are shown in menu
                if (tetriState == 1)
                    DrawText("Tetris", sw / 2 - 70, sh / 2 + 30, 20, selected == 1 ? YELLOW : WHITE);
                else
                    DrawText("Tetris", sw / 2 - 70, sh / 2 + 30, 20, GRAY);

                if (pongState == 1)
                    DrawText("Ping Pong", sw / 2 - 70, sh / 2 + 60, 20, selected == 2 ? YELLOW : WHITE);
                else
                    DrawText("Ping Pong", sw / 2 - 70, sh / 2 + 60, 20, GRAY);

                if (breakerState == 1)
                    DrawText("Brick Breaker", sw / 2 - 70, sh / 2 + 90, 20, selected == 3 ? YELLOW : WHITE);
                else
                    DrawText("Brick Breaker", sw / 2 - 70, sh / 2 + 90, 20, GRAY);

                if (IsKeyPressed(KEY_BACKSPACE))
                {
                    state = 2;
                    save();
                }

                if (IsKeyPressed(KEY_ENTER))
                {
                    if (selected == 0)
                    {
                        SetWindowSize(500, 600);
                        taketocenter();
                        // SetWindowSize(300, 600); try for better look
                        // SetWindowSize(200, 600);
                        // SetWindowSize(100, 600);

                        FlappyStart(); // thisfunction sets up all values on flappy.c
                        gamestate = 1;
                    }
                    else if (selected == 1 && tetriState == 1)
                    {
                        SetWindowSize(600, 700);
                        // SetWindowPosition(0, 0);
                        taketocenter();
                        InitTetris();

                        state = 4;
                    }
                    else if (selected == 2 && pongState == 1)
                    {
                        // SetWindowPosition(0, 0);
                        SetWindowSize(1080, 720);

                        taketocenter();
                        PlayPong();
                        // SetTargetFPS(69);

                        state = 5;
                    }
                    else if (selected == 3 && breakerState == 1)
                    {
                        SetWindowSize(1080, 720);

                        taketocenter();
                        Initbb();
                        state = 6;
                    }
                }
            }
            // this is for flappy bird loading
            if (gamestate == 1)
            {
                GameEngine1(); // game engine starts the logic of flappy bird
                checkLocks();
                UpdateMusicStream(epikmusic);
                DrawFlappy(); // this is drawing function that will draw sprites

                if (IsKeyPressed(KEY_BACKSPACE))
                {
                    
                    save();
                    gamestate = 0;
                    SetWindowSize(1500, 550);
                    taketocenter();
                    PlayMusicStream(epikmusic);
                    state = 3;
                }
            }
        }
        else if (state == 4)
        {
            // SetWindowSize(1080, 720);

            GameEngine2();
            checkLocks();
            DrawTetris();

            if (IsKeyPressed(KEY_BACKSPACE))
            {
                
                // StopMusicStream(tMusic);
                save();
                SetWindowSize(1500, 550);
                taketocenter();

                state = 3;
            }
        }
        else if (state == 5)
        {

            // added for next games , gamestate is used for flappy bird because i realized later that state can also control the game unlock and lock behaviou
            // todo use state in flappy bird
            // DrawText("PING PONG", sw / 2 - 90, sh / 2 - 20, 30, WHITE);
            // DrawText("Press BACKSPACE to return", sw / 2 - 140, sh / 2 + 20, 20, YELLOW);
            // PlayPong();
            // SetWindowSize(1080, 720);

            gameEngine3();
            checkLocks();

            if (IsKeyPressed(KEY_BACKSPACE))
            {
                // StopMusicStream(tMusic);
                save();
                SetWindowSize(1500, 550);
                taketocenter();

                state = 3;
            }
        }
        else if (state == 6)
        {
            // DrawText("BRICK BREAKER", sw / 2 - 120, sh / 2 - 20, 30, WHITE);
            // DrawText("Press BACKSPACE to return", sw / 2 - 140, sh / 2 + 20, 20, YELLOW);

            GameEngine4();
            if (IsKeyPressed(KEY_BACKSPACE))
            {
                
                save();
                SetWindowSize(1500, 550);
                taketocenter();

                state = 3;
            }
        }

        EndDrawing();
    }

    save();
    // we unload teture so that GPU space is freee
    UnloadMusicStream(epikmusic);
    UnloadTexture(b1);
    UnloadTexture(b2);
    UnloadTexture(b3);
    UnloadTexture(bg_day);
    UnloadTexture(ground);
    UnloadTexture(pipe1);
    UnloadTexture(pipe2);
    CloseAudioDevice();
    CloseWindow();
}