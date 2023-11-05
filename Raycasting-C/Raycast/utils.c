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
    SDL_Quit();
}

void playerConstructor() {
    player.x = WINDOW_WIDTH / 2;
    player.y = WINDOW_HEIGHT / 2;
    player.width = 5;
    player.height = 5;
    player.turnDirection = 0;
    player.walkDirection = 0;
    player.angle = PI / 2;
    player.walkSpeed = 100;
    player.turnSpeed = 45 * (PI / 180);
}

int wallExists(float x, float y) {
    if (x < 0 || x > WINDOW_WIDTH || y < 0 || y > WINDOW_HEIGHT) {
        return TRUE;
    }
    int mapIndexX = floor(x / TILE_SIZE);
    int mapIndexY = floor(y / TILE_SIZE);

    return map[mapIndexY][mapIndexX] != 0;
}

void movePlayer(float deltaTime) {
    player.angle += player.turnDirection * player.turnSpeed * deltaTime;
    float moveStep = player.walkDirection * player.walkSpeed * deltaTime;

    float newPlayerX = player.x + cos(player.angle) * moveStep;
    float newPlayerY = player.y + sin(player.angle) * moveStep;

    // Update position
    if (!wallExists(newPlayerX, newPlayerY)) {
        player.x = newPlayerX;
        player.y = newPlayerY;
    }
    
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

float normalize(float angle) {
    float normalizedAngle = remainder(angle, PI2);
    if (normalizedAngle < 0) {
        angle = PI2 + angle;
    }
    return angle;
}

float distanceBetweenPoints(float x1, float y1, float x2, float y2) {
    return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

// Casts a single ray
void castRay(float rayAngle, int stripId) {
    rayAngle = normalize(rayAngle);
    
    int rayFacingDown = rayAngle > 0 && rayAngle < PI;
    int rayFacingUp = !rayFacingDown;
    
    int rayFacingRight = rayAngle < 0.5 * PI || rayAngle > 1.5 * PI;
    int rayFacingLeft = !rayFacingRight;

    float xIntercept, yIntercept;
    float xStep, yStep;

    int isHorizontalWallHit = FALSE;
    int horizontalWallGrid = 0;
    float horizontalWallHitX = 0;
    float horizontalWallHitY = 0;

    yIntercept = floor(player.y / TILE_SIZE) * TILE_SIZE;
    yIntercept += rayFacingDown ? TILE_SIZE : 0;
     
    xIntercept = player.x + (yIntercept - player.y) / tan(rayAngle);

    yStep = TILE_SIZE;
    yStep *= rayFacingUp ? -1 : 1;

    xStep = TILE_SIZE / tan(rayAngle);
    xStep *= (rayFacingLeft && xStep > 0) ? -1 : 1;
    xStep *= (rayFacingRight && xStep < 0) ? -1 : 1;

    float nextHorizontalTouchX = xIntercept;
    float nextHorizontalTouchY = yIntercept;

    while (nextHorizontalTouchX >= 0 && nextHorizontalTouchX <= WINDOW_WIDTH && nextHorizontalTouchY >= 0 && nextHorizontalTouchY <= WINDOW_HEIGHT) {
        float xToCheck = nextHorizontalTouchX;
        float yToCheck = nextHorizontalTouchY + (rayFacingUp ? -1 : 0);

        if (wallExists(xToCheck, yToCheck)){
            horizontalWallHitX = nextHorizontalTouchX;
            horizontalWallHitY = nextHorizontalTouchY;
            horizontalWallGrid = map[(int)floor(yToCheck / TILE_SIZE)][(int)floor(xToCheck / TILE_SIZE)];
            isHorizontalWallHit = TRUE;
            break;
        }else {
            nextHorizontalTouchX += xStep;
            nextHorizontalTouchY += yStep;
        }
    }

    int isVerticalWallHit = FALSE;
    int verticalWallGrid = 0;
    float verticalWallHitX = 0;
    float verticalWallHitY = 0;

    xIntercept = floor(player.x / TILE_SIZE) * TILE_SIZE;
    xIntercept += rayFacingRight ? TILE_SIZE : 0;

    yIntercept = player.y + (xIntercept - player.x) * tan(rayAngle);

    xStep = TILE_SIZE;
    xStep *= rayFacingLeft ? -1 : 1;

    yStep = TILE_SIZE * tan(rayAngle);
    yStep *= (rayFacingUp && xStep > 0) ? -1 : 1;
    yStep *= (rayFacingDown && xStep < 0) ? -1 : 1;

    float nextVerticalTouchX = xIntercept;
    float nextVerticalTouchY = yIntercept;

    while (nextVerticalTouchX >= 0 && nextVerticalTouchX <= WINDOW_WIDTH && nextVerticalTouchY >= 0 && nextVerticalTouchY <= WINDOW_HEIGHT) {
        float xToCheck = nextVerticalTouchX + (rayFacingLeft ? -1 : 0);
        float yToCheck = nextVerticalTouchY;

        if (wallExists(xToCheck, yToCheck)) {
            verticalWallHitX = nextVerticalTouchX;
            verticalWallHitY = nextVerticalTouchY;
            verticalWallGrid = map[(int)floor(yToCheck / TILE_SIZE)][(int)floor(xToCheck / TILE_SIZE)];
            isVerticalWallHit = TRUE;
            break;
        }
        else {
            nextVerticalTouchX += xStep;
            nextVerticalTouchY += yStep;
        }
    }
    
    float horizontalHitDistance = isHorizontalWallHit ? distanceBetweenPoints(player.x, player.y, horizontalWallHitX, horizontalWallHitY) : INT_MAX;
    float verticalHitDistance = isVerticalWallHit ? distanceBetweenPoints(player.x, player.y, verticalWallHitX, verticalWallHitY) : INT_MAX;

    // Keep the closest distance
    if (verticalHitDistance < horizontalHitDistance) {
        rays[stripId].distance = verticalHitDistance;
        rays[stripId].wallHitX = verticalWallHitX;
        rays[stripId].wallHitY = verticalWallHitY;
        rays[stripId].wallHitGrid = verticalWallGrid;

        rays[stripId].verticalHit = TRUE;
    }else {
        rays[stripId].distance = horizontalHitDistance;
        rays[stripId].wallHitX = horizontalWallHitX;
        rays[stripId].wallHitY = horizontalWallHitY;
        rays[stripId].wallHitGrid = horizontalWallGrid;

        rays[stripId].verticalHit = TRUE;
    }
    rays[stripId].angle = rayAngle;
    rays[stripId].rayFacingUp = rayFacingUp;
    rays[stripId].rayFacingDown = rayFacingDown;
    rays[stripId].rayFacingLeft = rayFacingLeft;
    rays[stripId].rayFacingRight = rayFacingRight;
}

void castRays() {
    float rayAngle = player.angle - (FOV_ANGLE / 2);

    for (int strip = 0; strip < NUM_RAYS; strip++) {
        castRay(rayAngle, strip);

        rayAngle += FOV_ANGLE / NUM_RAYS;
    }
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

void renderRays() {
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    for (int i = 0; i < NUM_RAYS; i++) {
        SDL_RenderDrawLine(
            renderer,
            SCALE * player.x,
            SCALE * player.y,
            SCALE * rays[i].wallHitX,
            SCALE * rays[i].wallHitY
        );
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
        if (ev.key.keysym.sym ==  SDLK_d)
            player.turnDirection = +1;
        if (ev.key.keysym.sym ==  SDLK_a)
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

void update() {
    // Waste some time until the goal frame is reached
    int timeToWait = FRAME_TIME_LENGTH - (SDL_GetTicks() - lastFrameT);

    if (timeToWait > 0 && timeToWait <= FRAME_TIME_LENGTH) {
        SDL_Delay(timeToWait);
    }

    float deltaTime = (SDL_GetTicks() - lastFrameT)/1000.0f;
    lastFrameT = SDL_GetTicks();

    movePlayer(deltaTime);
    castRays();
}

void render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    //SDL_Rect rect = { playerX, playerY, 20, 20 };

    renderMap();
    renderPlayer();
    renderRays();

    //SDL_RenderFillRect(renderer, &rect);
    SDL_RenderPresent(renderer);
}