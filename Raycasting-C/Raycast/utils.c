#include "utils.h"

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

int playerX, playerY;

int initializeWindow() {
    if (SDL_Init(SDL_INIT_EVERYTHING) != FALSE) { 
        fprintf(stderr, "Error intializing the lib\n");
        return FALSE;
    }

    window = SDL_CreateWindow(
        "Raycasting with C",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        0
    );

    if (!window) {
        fprintf(stderr, "Error creating SDL window\n");
        return FALSE;
    }

    renderer = SDL_CreateRenderer(window, -1, 0);
    if (!renderer) {
        fprintf(stderr, "Error creating SDL renderer\n");
    }

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    return TRUE;
}

void destroyWindow() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}

void render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    SDL_Rect rect = { playerX, playerY, 20, 20 };

    SDL_RenderFillRect(renderer, &rect);
    SDL_RenderPresent(renderer);
}

void processInput(int *gameRunning) {
    SDL_Event ev;
    SDL_PollEvent(&ev);
    switch (ev.type) {
    case SDL_QUIT: {
        *gameRunning = FALSE;
        break;
    }
    case SDL_KEYDOWN: {
        if (ev.key.keysym.sym == SDLK_ESCAPE)
            *gameRunning = FALSE;
        break;
    }
    }
}

void setup() {
    playerX = 0;
    playerY = 0;
}

void update() {
    playerX++;
    playerY++;
}