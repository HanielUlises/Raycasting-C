#ifndef UTILS_H
#define UTILS_H

#pragma once

#include "stdio.h"
#include "SDL.h"

#define TRUE 1
#define FALSE 0

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

#define FPS 30
#define FRAME_TIME_LENGTH (1000/FPS)

extern SDL_Window* window;
extern SDL_Renderer* renderer;

int initializeWindow();
void destroyWindow();
void render();
void processInput(int * gameRunning);
void setup();

#endif