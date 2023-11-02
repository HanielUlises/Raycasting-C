#pragma once

#include "stdio.h"
#include "SDL.h"

#define TRUE 1
#define FALSE 0
#define WINDOW_WIDHT 800
#define WINDOW_HEIGHT 600

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

void initializeWindow();
void destroyWindow();