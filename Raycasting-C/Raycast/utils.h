#ifndef UTILS_H
#define UTILS_H

#pragma once

#include "stdio.h"
#include "limits.h"

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

#define NUM_RAYS WINDOW_WIDTH
#define FOV_ANGLE (60 * (PI/180))
#define FPS 30
#define FRAME_TIME_LENGTH (1000/FPS)

#define SCALE 0.3   // Minimap proportion

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

    float angle;    // Rotation Angle
    float walkSpeed;
    float turnSpeed;
}player;

struct Ray {
    float angle;    // Ray angle
    float wallHitX;
    float wallHitY;
    float distance; // Distance from wall to player
    int verticalHit;
    int rayFacingUp;
    int rayFacingDown;
    int rayFacingLeft;
    int rayFacingRight;
    int wallHitGrid;
}rays [NUM_RAYS];

int initializeWindow();
void destroyWindow();

void render();
void renderMap();
void renderPlayer();
void renderRays();
void movePlayer(float);

void handleEvents(int*);
void playerConstructor();
void castRays();

// Determines if there's a wall at the player's position
// Important for collision
int wallExists(float x, float y);
#endif