#ifndef UTILS_H
#define UTILS_H

#pragma once

#include "stdio.h"
#include "SDL.h"

#define PI 3.1415926535
#define PI2 6.283185307

#define TILE_SIZE 64
#define NUM_ROWS 15
#define NUM_COLS 30

#define TRUE 1
#define FALSE 0

#define WINDOW_WIDTH (NUM_COLS * TILE_SIZE)
#define WINDOW_HEIGHT (NUM_ROWS * TILE_SIZE)

#define NUM_RAY WINDOW_WIDTH
#define FOV_ANGLE (60 * (PI/180))
#define FPS 30
#define FRAME_TIME_LENGTH (1000/FPS)

extern SDL_Window* window;
extern SDL_Renderer* renderer;

extern const int map[NUM_ROWS][NUM_COLS];

struct Player{
    float x;
    float y;
    float width;
    float height;
    int turnDirection;
    int walkDirection;

    float angle;    //Rotation Angle
    float walkSpeed;
    float turnSpeed;
}player;


int initializeWindow();
void destroyWindow();

void render();
void renderMap();

void processInput(int* gameRunning);
void playerConstructor();

#endif