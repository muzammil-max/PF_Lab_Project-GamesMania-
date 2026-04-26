#include "raylib.h"
#include <stdio.h>
#include "engine.h"
#include "tetris.h"
#include <string.h>
#define ROWS 20
#define COLS 10
#define BLOCKS 30
#define tetrmino 7
//! BEFORE READING THIS
//! SEE THE ATTACHEED LINK TO VISUALIZE LOGIC!
/* //! https://miro.com/app/board/uXjVHcuZoLU=/?share_link_id=174680451795

*/
// Logic building
// If this is AI detector pls ensure that its not AI
// the code still has some bugs that needs to be adressed
// Logic was guided through YT tutorials
// The code is 15-20% AI

// Phase 01
//  basic idea making a grid where (ROWS,COLS) define position.
//  0 means empty and 1 means that grid is occupied

/*
The grid will be like this
[ ][ ][ ][ ] 0
[ ][ ][ ][ ]
[ ][ ][ ][ ]
[ ][ ][ ][ ]
[ ][ ][ ][ ]
[ ][ ][ ][ ]
[ ][ ][ ][ ]
[ ][ ][ ][ ]
[ ][ ][ ][ ]
[ ][ ][ ][ ]
[ ][ ][ ][ ]
[ ][ ][ ][ ]

we make function named init tetris which task is to set the all grid to zero.
!now when the Piece is falling we track its position using (X,Y) position
I will use arrays to tweak its position.
left and right will be used to change its X pos
gravity variable to move down the block every gtid position
for collosion what i am gona is that kay
 case 1:
 at bottom if the collosion with bottom becomes true then lock its position and not make it move.
 case2:
 if its colloision with different blocks become true  then lock
 but thsi case is only valid for downward collision
   4
 [xx] 0
 [1] 1
 [1] 2

 block is at row 0 and at col 4

 the loop will compare the position

 since the block is moving its in 0 state
 but if (key press down OR block down ja rha) and position (y,x)  == 1 then lock its position and set that positon to 1

 Now the basic thing
 we draw table using offset values these act translating function that will shift the grid  on center
 at right there is a panel which have the score and next block


 !Phase2 Tetrimono whatever is it :/



*/

// colors:

// https://excalidraw.com/#json=cYESNpsPr-jd0Q-NFspPQ,Xjnn1btWWcm_mVq8jmYf7w
//  movement of block

int combo = 0;
int frenzystate = 0; // adding some good things in game I am cooking!
float frenzytimer = 0.0f;
float scoremutli = 1.0f;
int shakestrong = 5.0f; 
int tScore = 0;
int gameover = 0;
int blocksize = 30;
// int tablewidth = 0;
int tablewidth = COLS * BLOCKS;
int tableheight = ROWS * BLOCKS;
int shake = 0.0f;
int screenheight;
int screenwidth;
// int tablewidth = COLS * blocksize; 
int initialpositionX;
int initialpositionY = 0;
int bX;
int bY;
int table[ROWS][COLS];
float timer = 0.0; // to make increase game difficulty
float fallspeed = 0.5f;
float fallspeeddownbutton = 0.05f;
int tablelineX;
int tablelineY;
int nextpiece;
int currentType;
int rotationfahh = 0; // rotation
Music tMusic;
Music frenzymusic;
Color UI = WHITE;
int c = 0;
int current[4][4];

Vector2 Shaker()
{
    if (shake <= 0)
    {
        return (Vector2){0, 0}; // this function will help to shake the grid
    }

    float Mx = GetRandomValue(-shakestrong, shakestrong);
    float My = GetRandomValue(-shakestrong, shakestrong);

    return (Vector2){Mx, My};
}

// now the most hardest thing is  making of tetrimino
// we made array which
// 7 pieces each piece is 4x4 matrix
// 7 pieces each piece has 4 sub pieces which will be 4x4 matrix
// Thanks to YTUBER and my fri4end for resolving this logic
// its like this:
// [type][rotation state][rows][cols]
// [T][2][4][4] Omg i love matrices!!!

int onepiece[tetrmino][4][4][4] = {
    // I shape
    {
        {
            {0, 1, 0, 0}, // r1
            {0, 1, 0, 0},
            {0, 1, 0, 0},
            {0, 1, 0, 0},
        },

        {
            {0, 0, 0, 0},
            {1, 1, 1, 1}, // r2
            {0, 0, 0, 0},
            {0, 0, 0, 0},
        },
        {
            {0, 1, 0, 0},
            {0, 1, 0, 0}, //! r3
            {0, 1, 0, 0},
            {0, 1, 0, 0},
        },
        {
            {0, 0, 0, 0},
            {1, 1, 1, 1}, //* R4
            {0, 0, 0, 0},
            {0, 0, 0, 0},
        },

    },

    // O shape

    {
        {
            {0, 1, 1, 0}, // r1
            {0, 1, 1, 0},
            {0, 0, 0, 0},
            {0, 0, 0, 0},
        },

        {
            {0, 1, 1, 0}, // r2
            {0, 1, 1, 0},
            {0, 0, 0, 0},
            {0, 0, 0, 0},
        },
        {
            {0, 1, 1, 0}, // r3
            {0, 1, 1, 0},
            {0, 0, 0, 0},
            {0, 0, 0, 0},
        },
        {
            {0, 1, 1, 0}, // r4
            {0, 1, 1, 0},
            {0, 0, 0, 0},
            {0, 0, 0, 0},
        },

    },

    // T
    {
        {
            {0, 1, 0, 0},
            {1, 1, 1, 0},
            {0, 0, 0, 0}, // r1
            {0, 0, 0, 0},
        },
        {
            {0, 1, 0, 0},
            {0, 1, 1, 0},
            {0, 1, 0, 0}, // r2
            {0, 0, 0, 0},
        },
        {
            {0, 0, 0, 0},
            {1, 1, 1, 0},
            {0, 1, 0, 0}, // r3
            {0, 0, 0, 0},
        },
        {
            {0, 1, 0, 0},
            {1, 1, 0, 0}, // r4
            {0, 1, 0, 0},
            {0, 0, 0, 0},
        },
    },

    // L
    {
        {
            {0, 1, 0, 0},
            {0, 1, 0, 0}, // r1
            {0, 1, 1, 0},
            {0, 0, 0, 0},
        },
        {
            {0, 0, 0, 0},
            {1, 1, 1, 0},
            {1, 0, 0, 0}, // r2
            {0, 0, 0, 0},
        },
        {
            {1, 1, 0, 0},
            {0, 1, 0, 0},
            {0, 1, 0, 0}, // r3
            {0, 0, 0, 0},
        },
        {
            {0, 0, 1, 0},
            {1, 1, 1, 0},
            {0, 0, 0, 0}, // r4
            {0, 0, 0, 0},
        },
    },

    // J
    {
        {

            {0, 1, 0, 0},
            {0, 1, 0, 0}, // r1
            {1, 1, 0, 0},
            {0, 0, 0, 0},

        },
        {

            {1, 0, 0, 0},
            {1, 1, 1, 0},
            {0, 0, 0, 0}, // r2
            {0, 0, 0, 0},

        },
        {

            {0, 1, 1, 0},
            {0, 1, 0, 0}, // r3
            {0, 1, 0, 0},
            {0, 0, 0, 0},

        },
        {

            {0, 0, 0, 0},
            {1, 1, 1, 0}, // r4
            {0, 0, 1, 0},
            {0, 0, 0, 0},

        },

    },

    // S
    {
        {
            {0, 1, 1, 0},
            {1, 1, 0, 0}, // r1
            {0, 0, 0, 0},
            {0, 0, 0, 0},
        },
        {
            {1, 0, 0, 0},
            {1, 1, 0, 0},
            {0, 1, 0, 0}, // r2
            {0, 0, 0, 0},
        },
        {
            {0, 1, 1, 0},
            {1, 1, 0, 0},
            {0, 0, 0, 0}, // r3
            {0, 0, 0, 0},
        },
        {
            {1, 0, 0, 0},
            {1, 1, 0, 0},
            {0, 1, 0, 0}, // r4
            {0, 0, 0, 0},
        },
    },
    // Z
    {
        {
            {1, 1, 0, 0},
            {0, 1, 1, 0},
            {0, 0, 0, 0}, // r1
            {0, 0, 0, 0},
        },
        {
            {0, 1, 0, 0},
            {1, 1, 0, 0}, // r2
            {1, 0, 0, 0},
            {0, 0, 0, 0},
        },
        {
            {1, 1, 0, 0},
            {0, 1, 1, 0},
            {0, 0, 0, 0}, // r3
            {0, 0, 0, 0},
        },
        {
            {0, 1, 0, 0},
            {1, 1, 0, 0},
            {1, 0, 0, 0}, // r4
            {0, 0, 0, 0},
        },
    },

};

Color setcolor(int i)
{
    switch (i)
    {
    case 1:
        return SKYBLUE;
        // break;

    case 2:
        return YELLOW;
        // break;
    case 3:
        return PURPLE;
        // break;
    case 4:
        return ORANGE;
        // break;
    case 5:
        return BLUE;
        // break;
    case 6:
        return GREEN;
        // break;
    case 7:
        return RED;
        // break;
    default:
        return WHITE;
    }
}

void copytetri(int select)
{ // this will take type (L O T Z S) and then copy the contents of that block in the ready made sets we made

    for (int i = 0; i <= 4 - 1; i++)
    {
        for (int j = 0; j <= 4 - 1; j++)
        {
            // now copy the contents to current

            current[i][j] = onepiece[select][rotationfahh][i][j]; // this will copy the contents to current matrix 4 by 4
        }
    }
}

void InitTetris(void)
{

    // Vector2 change = Shaker();
    // DrawRectangle
    // setting up screen
SetTargetFPS(60);
    screenheight = GetScreenHeight();
    screenwidth = GetScreenWidth();
    // translatin of table at center
    tablelineX = (screenwidth - tablewidth) / 2;   // offset X
    tablelineY = (screenheight - tableheight) / 2; // offset Y

    // load music
    tMusic = LoadMusicStream("assets/t1.mp3");
    frenzymusic = LoadMusicStream("assets/frenzy.mp3");

    PlayMusicStream(tMusic);
    // Setup Table

    // blocks
    // For next block guess logic :https://excalidraw.com/#json=cYESNpsPr-jd0Q-NFspPQ,Xjnn1btWWcm_mVq8jmYf7w
    // nextpiece
    nextpiece = GetRandomValue(0, tetrmino - 1);

    currentType = nextpiece;
    nextpiece = GetRandomValue(0, tetrmino - 1);
    copytetri(currentType);
    for (int i = 0; i <= ROWS - 1; i++)
    {
        for (int j = 0; j <= COLS - 1; j++)
        {
            table[i][j] = 0; // set all values to 0 which means empty space
        }
    }

    // Setting up positon
    float fallspeed = 0.5f;

    // initialpositionX = COLS / 2; // block center me hga (DEPRECIATED)
    // 0 1 2 3 *
    // X X X X 0
    // X X X X 1
    // X X X X 2
    // X X X X 3

    initialpositionX = (COLS / 2) - 2; // 10/2 is 5 so block move to X =5 but that block will acutally be on edge so -2 (because 4 x 4 matrix) and it will be prefcetly at center

    bX = initialpositionX;
    bY = 0;
}

//! check Move
int checkMove(int nextX, int nextY)
{
    int state;
    // Dont use this code
    //  if (nextX < 0)
    //  {
    //      state = 0;

    //     // keep in mind here rows are acting as Y and cols are acting as X
    // }
    // else if (nextX >= COLS)
    // {
    //     state = 0;
    // }
    // else if (nextY >= ROWS)
    // {
    //     state = 0;
    // }
    // else if (table[nextY][nextX] == 1)
    // {
    //     state = 0;
    // }
    // else
    // {
    //     state = 1;
    // }

    // if (state == 0)
    // {
    //     return 0;
    // }
    // else
    // {
    //     return 1;
    // }

    // Best code in the world
    // logc building
    /*

    the above code only worked for single block but we need to work for a whole block that have multiple single blocks
    these blovks are in 4x4 matrix

    so we need to map those blocks in grids

    what i am gonna do is that ke
    i will loop through the tetrimino  and look at each block position whose value is one ..
    now here now the mian thing is mapping
    This formula

    tableX = nextX + cols;
    it will map the block on that specific cell
    the nextX is actuall the realtime position of tetrimono
    when the tetrimono matrix have 1 it will add the position of that block along with the grid oe
    Its like u have a table who have a glass of water and its at 1,2 position of table
    u have to put table on room on positiion for eg 12,19
    so the position 12,19 will be added to 1,2 to perfectlty map the glass on room
    */

    for (int row = 0; row <= 4 - 1; row++)
    {
        for (int col = 0; col <= 4 - 1; col++)
        {
            if (current[row][col] == 1)
            {
                int mapX = nextX + col;
                int mapY = nextY + row; // mapping

                // now check for boundaries

                if (mapX < 0 || mapX >= COLS || mapY >= ROWS)
                { // out of bounds means  khel khtm
                    state = 0;
                    return state;
                }

                // now if it collides with already placed blocks aka locked vlocks
                if (mapY >= 0 && table[mapY][mapX] != 0)
                {
                    state = 0;
                    return state;
                }
            }
        }
    }
    return 1;
}

// check each row thru column and then check state
int isrowcomplete(int row)
{
    int state;

    for (int x = 0; x <= COLS - 1; x++)
    {
        if (table[row][x] == 0)
        {
            state = 0;
            return state;
        }
    }
    state = 1;
    return state;
}

void clearline(int row) // clear line if all blocks filled
{
    for (int x = 0; x <= COLS - 1; x++)
    {

        table[row][x] = 0;
    }
}

void movedown(int row)
{
    for (int y = row; y > 0; y--)
    {
        for (int col = 0; col <= COLS - 1; col++)
        {
            table[y][col] = table[y - 1][col]; // shifintg the whole row and contents downwards
        }
    }
}

void evalline()
{
    int anylinecleared = 0;
    // go from bottom to up
    int positions = ROWS - 1;
    for (int x = positions; x > 0; x--)
    {
        if ((isrowcomplete(x)) == 1)
        {
            tScore += 10 * scoremutli;
            combo++;

            c += 3;
            if (c > 10)
            {
                c = 0;
                score++;
            }
            if (frenzystate == 1)
            {
                frenzytimer -= -combo * 0.3f;
                ;

                if (frenzytimer < 0)
                {
                    frenzytimer == 0;
                }
            }

            clearline(x);
            anylinecleared = 1;
            movedown(x);
            x = x + 1; // it wil chekc the row again
        }
    }

    if (anylinecleared == 1)
    {
        combo++;
        if (combo >= 3 && frenzystate == 0)
        {
            frenzystate = 1;
            frenzytimer = 0;
            scoremutli = 1.5f;
            shake = 1.0f;
        }
    }
    else
    {
        combo = 0;
    }
}

void GameEngine2()
{

    // start frenzy
    /// testing
    // if (IsKeyPressed(KEY_F) && frenzystate == 0)
    // {
    //     frenzystate = 1;
    // }
    // if (IsKeyPressed(KEY_F) && frenzystate == 1)
    // {
    //     frenzystate = 0;
    // }

    if (frenzystate == 1)
    {

        UI = (Color){
            GetRandomValue(100, 255),
            GetRandomValue(120, 255),
            GetRandomValue(100, 255),
            255};
        // StopMusicStream(tMusic);
        // PlayMusicStream(frenzymusic);
        // // UpdateMusicStream(frenzymusic);

        frenzytimer += GetFrameTime();

        fallspeed = 0.08f;
        // combo = 0;
        scoremutli = 1.5f;
        shake = 1.0f;

        if (IsMusicStreamPlaying(frenzymusic) == 0)
        { // if renzy music not playing
            StopMusicStream(tMusic);
            PlayMusicStream(frenzymusic);
        }
        UpdateMusicStream(frenzymusic);
    }
    // stop frenzy
    if (frenzystate == 1 && frenzytimer > 35.0f)
    {
        frenzystate = 0;
        scoremutli = 1.0f;
        combo = 0;
        scoremutli = 1.0f;
        shake = 0;
        UI = WHITE;
        StopMusicStream(frenzymusic);
        PlayMusicStream(tMusic);
    }
    else
    {
        UpdateMusicStream(tMusic);
        fallspeed = 0.5f;
    }

    // gameover
    if (gameover == 1 && IsKeyPressed(KEY_R))
    {
        gameover = 0;
        tScore = 0;
        UnloadMusicStream(tMusic);
        InitTetris();
    }

    if (gameover == 1)
    {
        return;
    }

    // this snippet will rotate tetrimono

    if (IsKeyPressed(KEY_UP))
    {
        int oldrotation = rotationfahh;
        rotationfahh = rotationfahh + 1;

        if (rotationfahh > 3)
        {
            rotationfahh = 0;
        }

        copytetri(currentType);

        if (checkMove(bX, bY) == 0)
        { // old rotation if collsion
            rotationfahh = oldrotation;
            copytetri(currentType);
        }
    }

    // playmusic

    UpdateMusicStream(tMusic);
    // todo Remeber to stop music when press backspace
    //  Block Movement setup
    if (IsKeyPressed(KEY_LEFT))
    {
        if (checkMove(bX - 1, bY) == 1)
        {
            bX--; // press left then move left
        }
    }

    if (IsKeyPressed(KEY_RIGHT))
    {
        if (checkMove(bX + 1, bY))
        {
            bX++;
        }
    }

    // Gravity
    if (IsKeyDown(KEY_DOWN))
    {
        fallspeed = fallspeeddownbutton;
    }
    else
    {
        // fallspeed = fallspeed; //dumb logic
        fallspeed = 0.5f;
    }

    timer += GetFrameTime();
    // ham aik make  timer bnye ge that will update the block movement after every some period of time and then we will reset it
    if (timer > fallspeed)
    {
        timer = 0;
        /// now main thing to check if it can go down
        if (checkMove(bX, bY + 1) == 1)
        {
            bY++;
        }
        else

        { // we will lock its position

            // table[bY][bX] = 1; //Dont use
            for (int row = 0; row <= 4 - 1; row++)
            {
                for (int col = 0; col <= 4 - 1; col++)
                {
                    if (current[row][col] == 1)
                    {
                        int meowX = bX + col;
                        int meowY = bY + row;
                        if (meowX >= 0 && meowX < COLS &&
                            meowY >= 0 && meowY < ROWS)
                        {
                            table[meowY][meowX] = currentType + 1;
                        }
                    }
                }
            }

            // now spawn new block
            evalline();
            //! SPAWN
            currentType = nextpiece;
            nextpiece = GetRandomValue(0, tetrmino - 1);
            copytetri(currentType);
            bX = (COLS / 2) - 2;
            bY = 0;

            if (checkMove(bX, bY) == 0)
            {
                gameover = 1;
            }
        }
    }

    // Stop music
    if (IsKeyPressed(KEY_BACKSPACE))
    {
        StopMusicStream(tMusic);
    }
}

void DrawTetris()
{
    Color tableColor = DARKGRAY;
    if (frenzystate == 1)
    {
        tableColor = UI;
    }
    else
    {
        tableColor = DARKGRAY;
    }

    Vector2 changer = Shaker();
    //! settting up table
    DrawRectangle(tablelineX + changer.x, tablelineY + changer.y, tablewidth, tableheight, tableColor);
    // now making cells
    for (int y = 0; y <= ROWS - 1; y++)
    {
        for (int x = 0; x <= COLS - 1; x++)
        {
            int dX = tablelineX + changer.x + x * blocksize;
            int dY = tablelineY + changer.y + y * blocksize;

            DrawRectangleLines(dX, dY, blocksize, blocksize, GRAY);
        }
    }

    //! Filled Blocks setup
    for (int y = 0; y <= ROWS - 1; y++)
    {
        for (int x = 0; x <= COLS - 1; x++)
        {
            if (table[y][x] != 0)
            {
                Color mycolor = setcolor(table[y][x]);

                int dX = tablelineX + changer.x + x * blocksize;
                int dY = tablelineY + changer.y + y * blocksize;
                DrawRectangle(dX, dY, blocksize - 3, blocksize - 3, setcolor(table[y][x]));
            }
        }
    }
    //! Setting up falling block
    // int fallX = tablelineX + bX * blocksize;
    // int fallY = tablelineY + bY * blocksize;
    for (int rows = 0; rows <= 4 - 1; rows++)
    {
        for (int cols = 0; cols <= 4 - 1; cols++)
        {
            if (current[rows][cols] == 1)
            {
                int dX = tablelineX + changer.x + (bX + cols) * BLOCKS;
                int dY = tablelineY + changer.y + (bY + rows) * BLOCKS;
                DrawRectangle(dX, dY, blocksize - 3, blocksize - 3, setcolor(table[cols][rows])); // it will basicaly set that color of grid cell to the color of the block
                // DrawRectangle(dX, dY, blocksize - 3, blocksize - 3, GREEN;  /// this code is buggy and will basocally set all colors to REd
            }
        }
    }

    // setting up right hand side Panel FAHHH

    /// leave  sume space on right
    int uiX = tablelineX + tablewidth + 30; // tweak values if not good (30 ko opar neeche kro)

    // showing score
    DrawText(TextFormat("Score: %d", tScore), uiX, 80, 20, UI);
    DrawText(TextFormat("MainScore: %d", score), uiX - 5, 110, 15, WHITE);
    // DrawText("By Muzammil", uiX-10, 400, 20, WHITE);
    // DrawText("25K-6533", uiX-10, 450, 20, WHITE);

    // next block

    DrawText("Next Piece:", uiX, 150, 20, UI);
    DrawRectangle(uiX, 180, 100, 150, DARKGRAY);

    for (int x = 0; x <= 4 - 1; x++)
    {
        for (int y = 0; y <= 4 - 1; y++)
        {
            if (onepiece[nextpiece][0][x][y] == 1)
            {
                int setX = uiX + x * BLOCKS;
                int setY = 180 + y * BLOCKS;
                DrawRectangle(setX, setY, blocksize - 5, blocksize - 5, setcolor(nextpiece + 1)); // do this so that each piece has different color
            }
        }
    }
    //! gameover!
    if (gameover == 1)
    {
        frenzystate = 0;
        shake = 0;
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.8f));

        DrawText("GAME OVER", 200, 200, 50, RED);
        DrawText("Press R to Restart", 200, 270, 20, WHITE);
    }

    // Frenzy
    Color chaoticcolor = (Color){// typecasting to  datatype color
                                 GetRandomValue(150, 255),
                                 GetRandomValue(0, 255),
                                 GetRandomValue(0, 255),
                                 255};
    if (frenzystate == 1)
    {
        DrawText("FRENZY MODE", 20, 20, 20, chaoticcolor);
        DrawText(TextFormat("COMBO: %d", combo), 20, 50, 20, YELLOW);
    }
    else
    {
        DrawText(TextFormat("COMBO: %d", combo), 20, 20, 20, WHITE);
    }
}
