#include "utils.h"

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

const int map[NUM_ROWS][NUM_COLS] = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
};

// Number of ticks since the last frame
int lastFrameT = 0;

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

void movePlayer(float deltaTime) {
    player.angle += player.turnDirection * player.turnSpeed * deltaTime;
    float moveStep = player.walkDirection * player.walkSpeed * deltaTime;

    float newPlayerX = player.x + cos(player.angle) * moveStep;
    float newPlayerY = player.y + sin(player.angle) * moveStep;

    // Update position
    player.x = newPlayerX;
    player.y = newPlayerY;
}

void renderPlayer() {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_Rect pRect = {
        player.x * SCALE,
        player.y * SCALE,
        player.width * SCALE,
        player.height * SCALE
    };

    SDL_RenderFillRect(renderer, &pRect);
    SDL_RenderDrawLine(
        renderer,
        player.x * SCALE,
        player.y * SCALE,
        SCALE * player.x + cos(player.angle) * 40 ,
        SCALE * player.y + sin(player.angle) * 40
    );
}

void renderMap() {
    for (int i = 0; i < NUM_ROWS; i++) {
        for (int j = 0; j < NUM_COLS; j++) {
            int tileX = j * TILE_SIZE;
            int tileY = i * TILE_SIZE;
            int tileColor = map[i][j] != 0 ? 255 : 0;

            SDL_SetRenderDrawColor(renderer, tileColor, tileColor, tileColor, 255);
            SDL_Rect mapTile = {
                tileX * SCALE,
                tileY * SCALE,
                TILE_SIZE * SCALE,
                TILE_SIZE * SCALE
            };
            SDL_RenderFillRect(renderer, &mapTile);
        }
    }
}

void handleEvents(int *gameRunning) {
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
        if (ev.key.keysym.sym ==  SDLK_w)
            player.walkDirection = +1;
        if (ev.key.keysym.sym ==  SDLK_s)
            player.walkDirection = -1;
        if (ev.key.keysym.sym ==  SDLK_a)
            player.turnDirection = +1;
        if (ev.key.keysym.sym ==  SDLK_d)
            player.turnDirection = -1;
        break;
    }
    case SDL_KEYUP: {
        if (ev.key.keysym.sym == SDLK_w)
            player.walkDirection = 0;
        if (ev.key.keysym.sym == SDLK_s)
            player.walkDirection = 0;
        if (ev.key.keysym.sym ==  SDLK_a)
            player.turnDirection = 0;
        if (ev.key.keysym.sym == SDLK_d)
            player.turnDirection = 0;
        break;
    }
    }
}

void playerConstructor() {
    player.x = WINDOW_WIDTH / 2;
    player.y = WINDOW_HEIGHT / 2;
    player.width = 5;
    player.height = 5;
    player.turnDirection = 0;
    player.walkDirection = 0;
    player.angle = PI / 2;
    player.walkSpeed = 50;
    player.turnSpeed = 45 * (PI / 180);
}

void update() {
    // Waste some time until the goal frame is reached
    int timeToWait = FRAME_TIME_LENGTH - (SDL_GetTicks() - lastFrameT);

    if (timeToWait > 0 && timeToWait <= FRAME_TIME_LENGTH) {
        SDL_Delay(timeToWait);
    }

    float deltaTime = (SDL_GetTicks() - lastFrameT)/1000.0f;
    lastFrameT = SDL_GetTicks();

    movePlayer(deltaTime);
}

void render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    //SDL_Rect rect = { playerX, playerY, 20, 20 };

    renderMap();
    renderPlayer();
    //renderRays();

    //SDL_RenderFillRect(renderer, &rect);
    SDL_RenderPresent(renderer);
}