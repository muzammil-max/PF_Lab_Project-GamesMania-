#ifndef FLAPPY_H
#define FLAPPY_H

//functions
void FlappyStart();
void GameEngine1(); // the main game engine for the flappy bird UwU
void DrawFlappy();


//textures
// extern is basically a keyword to define a variable from another file and reuse

extern Texture2D b1, b2, b3;
extern Texture2D bg_day;
extern Texture2D ground;
extern Texture2D pipe1;
extern Texture2D pipe2;
// extern myfps = 120;

#endif