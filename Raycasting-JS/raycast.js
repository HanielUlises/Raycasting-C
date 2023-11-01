const TILE_SIZE = 32;
// MxN size Matrix 
const MAP_NUM_ROWS = 11;
const MAP_NUM_COLS = 15;
// This defines the window proportions
const WINDOW_WIDTH = MAP_NUM_COLS*TILE_SIZE;
const WINDOW_HEIGHT = MAP_NUM_ROWS*TILE_SIZE;
// Field of view angle
const FOV_ANGLE =  60 * (Math.PI/180);

const WALL_STRIP_WIDTH = 1;
// Rays to be <<casted>> per space
const NUM_RAYS = WINDOW_WIDTH/WALL_STRIP_WIDTH;

// Proportion of minimap display in window
const MINIMAP_SCALE_FACTOR = 0.3;

class Map{
    constructor(){
        // Matrix with the wall distribution
        this.walls = [
            [1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1],
            [1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1],
            [1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1],
            [1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 1, 0, 1],
            [1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1],
            [1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 1],
            [1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1],
            [1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1],
            [1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1],
            [1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1],
            [1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1]
        ];
    }
    //Function that tells if in the actual position there's a wall
    wallExists(x,y){
        if(x < 0 || x > WINDOW_WIDTH || y < 0 || y > WINDOW_HEIGHT){
            return true;
        }
        let mapGridIndexX = Math.floor(x / TILE_SIZE);
        let mapGridIndexY = Math.floor(y / TILE_SIZE);
        return this.walls[mapGridIndexY][mapGridIndexX] != 0;
    }
    // Map renderer
    render(){
        for(let i = 0; i < MAP_NUM_ROWS;i++){
            for(let j=0; j<MAP_NUM_COLS; j++){
                let tileX = j * TILE_SIZE;
                let tileY = i * TILE_SIZE;
                // Sets the colors for the <<walls>>
                let tileColor = this.walls[i][j] == 1 ? "#222" : "#fff";
                
                // p5.js functions
                stroke("#222");
                fill(tileColor);
                rect(
                    MINIMAP_SCALE_FACTOR *tileX,
                    MINIMAP_SCALE_FACTOR *tileY, 
                    MINIMAP_SCALE_FACTOR *TILE_SIZE, 
                    MINIMAP_SCALE_FACTOR *TILE_SIZE);
            }
        }
    }
}
class Player{
    constructor(){
        this.x = WINDOW_WIDTH/2;
        this.y = WINDOW_HEIGHT/2;
        this.radius = 6;
        this.turnDirection = 0; // -1 for left, +1 for right
        this.walkDirection = 0; // -1 for back, +1 for front
        this.rotationAngle = Math.PI/2;
        this.moveSpeed = 2.0;
        this.rotationSpeed = 2 * (Math.PI/180);
    }
    update(){
        // Updates the player position based on <<turnDirection>> and <<walkDirection>>
        this.rotationAngle += this.turnDirection * this.rotationSpeed;

        // Increasing or decreasing the moving pixels
        let moveStep = this.walkDirection*this.moveSpeed;
        
        let newX = this.x + Math.cos(this.rotationAngle)*moveStep;
        let newY = this.y + Math.sin(this.rotationAngle)*moveStep;
        // Collision with walls
        if(!grid.wallExists(newX, newY)){
           this.x = newX;
           this.y = newY; 
        }
    }
    // Player renderer
    render(){
        // p5.js functions
        noStroke();
        fill("red");
        circle(
            MINIMAP_SCALE_FACTOR *this.x, 
            MINIMAP_SCALE_FACTOR *this.y, 
            MINIMAP_SCALE_FACTOR *this.radius);
        
        stroke("red");
        // Line that shows where the player is facing
        line(MINIMAP_SCALE_FACTOR *this.x,
            MINIMAP_SCALE_FACTOR *this.y,
            MINIMAP_SCALE_FACTOR *(this.x + Math.cos(this.rotationAngle)*30),
            MINIMAP_SCALE_FACTOR *(this.y + Math.sin(this.rotationAngle)*30)
            );
    }
}

class Ray{
    constructor(rayAngle){
        this.rayAngle = normalizeAngle(rayAngle);
        this.wallHitX = 0;
        this.wallHitY = 0;
        this.distance = 0;
        this.wasHitVertical = false;

        this.rayFacingDown = this.rayAngle > 0 && this.rayAngle < Math.PI;
        this.rayFacingUp = !this.rayFacingDown;

        this.rayFacingRight = this.rayAngle < 0.5 * Math.PI || this.rayAngle > 1.5 * Math.PI;
        this.rayFacingLeft = !this.rayFacingRight;
    }
    // Casting based on the current column
    cast (){
        let xstep, ystep;
        let xintercept, yintercept;

        let foundHorizWallHit = false;
        let horizWallHitX = 0;
        let horizWallHitY = 0;
        
        // x & y coordinates of the closest horizontal grid
        yintercept = Math.floor(player.y/TILE_SIZE) * TILE_SIZE;
        yintercept += this.rayFacingDown ? TILE_SIZE : 0;

        xintercept = player.x + ((yintercept - player.y)/Math.tan(this.rayAngle));

        // Increment of both xstep and ystep
        ystep = TILE_SIZE;
        // Inversion if needed
        ystep *= this.rayFacingUp ? -1 : 1;

        xstep = TILE_SIZE / Math.tan(this.rayAngle);
        xstep *= (this.rayFacingLeft && xstep > 0) ? -1 : 1;
        xstep *= (this.rayFacingRight && xstep < 0)? -1 : 1;

        let nextHorizTouchX = xintercept;
        let nextHorizTouchY = yintercept;

        // if (this.rayFacingUp) nextHorizTouchY--;

        // Incremente XStep and Ystep unitl a wall is found

        while (nextHorizTouchX >= 0 && nextHorizTouchX <= WINDOW_WIDTH && nextHorizTouchY >= 0 && nextHorizTouchY <= WINDOW_HEIGHT){
            if (grid.wallExists(nextHorizTouchX, nextHorizTouchY - (this.rayFacingUp ? 1 : 0))) {
                // A wall is found
                foundHorizWallHit = true;
                horizWallHitX = nextHorizTouchX;
                horizWallHitY = nextHorizTouchY;

                // stroke("red");
                // line (player.x, player.y, horizWallHitX, horizWallHitY);
                break;
            }else{
                nextHorizTouchX += xstep;
                nextHorizTouchY += ystep;
            }
        }

        // Vertical intersection
        let foundVertWallHit = false;
        let verticalWallHitX = 0;
        let verticalWallHitY = 0;

        
        // x & y coordinates of the closest vertical grid intersection
        xintercept = Math.floor(player.x/TILE_SIZE) * TILE_SIZE;
        xintercept += this.rayFacingRight ? TILE_SIZE : 0;

        yintercept = player.y + ((xintercept - player.x) * Math.tan(this.rayAngle));

        // Increment of both xstep and ystep
        xstep = TILE_SIZE;
        // Inversion if needed
        xstep *= this.rayFacingLeft ? -1 : 1;

        ystep = TILE_SIZE * Math.tan(this.rayAngle);
        ystep *= (this.rayFacingUp && ystep > 0) ? -1 : 1;
        ystep *= (this.rayFacingDown && ystep < 0)? -1 : 1;

        let nextVertTouchX = xintercept;
        let nextVertTouchY = yintercept;

        // if (this.rayFacingLeft) nextVertTouchX--;

        // Incremente XStep and Ystep unitl a wall is found

        while (nextVertTouchX >= 0 && nextVertTouchX <= WINDOW_WIDTH && nextHorizTouchY >= 0 && nextHorizTouchY <= WINDOW_HEIGHT){
            if (grid.wallExists(nextVertTouchX - (this.rayFacingLeft ? 1 : 0), nextVertTouchY)) {
                // A wall is found
                foundVertWallHit = true;
                verticalWallHitX = nextVertTouchX;
                verticalWallHitY = nextVertTouchY;

                // stroke("red");
                // line (player.x, player.y, verticalWallHitX, verticalWallHitY);

                break;

            }else{
                nextVertTouchX += xstep;
                nextVertTouchY += ystep;
            }
        }
        // Calculate both horizontal and vertical distances and take the smallest value
        let horzHitDistance = (foundHorizWallHit) 
        ? pointsDistance(player.x, player.y, horizWallHitX, horizWallHitY)
        : Number.MAX_VALUE;

        let vertHitDistance = (foundVertWallHit) 
        ? pointsDistance(player.x, player.y, verticalWallHitX, verticalWallHitY)
        : Number.MAX_VALUE;

        if(vertHitDistance < horzHitDistance){
            this.wallHitX = verticalWallHitX;
            this.wallHitY = verticalWallHitY;
            this.distance = vertHitDistance;
            this.wasHitVertical = true;
        }else{
            this.wallHitX = horizWallHitX;
            this.wallHitY = horizWallHitY;
            this.distance = horzHitDistance;
            this.wasHitVertical = false;
        }

        // // Just store the smallest of the distance
        // this.wallHitX = (horzHitDistance < vertHitDistance) ? horizWallHitX : verticalWallHitX;
        // this.wallHitY = (horzHitDistance < vertHitDistance) ? horizWallHitY  : verticalWallHitY;
        // // Distance between two points
        // this.distance = (horzHitDistance < vertHitDistance) ? horzHitDistance : vertHitDistance;

        // this.wasHitVertical = (vertHitDistance < horzHitDistance);
    }

    render(){
        stroke("rgba(225,0,0,0.1)");
        line(
            MINIMAP_SCALE_FACTOR * player.x, 
            MINIMAP_SCALE_FACTOR * player.y, 
            MINIMAP_SCALE_FACTOR * this.wallHitX,
            MINIMAP_SCALE_FACTOR * this.wallHitY    
        );
    }   
}

let grid = new Map();
let player = new Player();
let rays = [];

function projection3D(){
    // Looping through every ray in the array
    for (let i = 0; i < NUM_RAYS; i++){
        let ray = rays[i];
        let rayDistance = ray.distance * Math.cos(ray.rayAngle - player.rotationAngle);
        // Player's distance from the projection plane
        let distanceProjectionPlane = (WINDOW_WIDTH / 2) / Math.tan(FOV_ANGLE/2);
        let wallStripHeight = (TILE_SIZE / rayDistance) * distanceProjectionPlane;

        // Distance of a wall from the player 
        let alpha = 90 / rayDistance;

        let intensity = ray.wasHitVertical ? 255 : 180;

        fill("rgba("+ intensity + "," + intensity +","+intensity + "," + alpha + ")");
        noStroke();
        rect(
            i * WALL_STRIP_WIDTH,
            (WINDOW_HEIGHT / 2) - (wallStripHeight / 2),
            WALL_STRIP_WIDTH,
            wallStripHeight
        );
    }
}

function normalizeAngle(angle){
    angle = angle % (2 * Math.PI);
    if (angle < 0){
        angle += (2* Math.PI);
    }
    return angle;
}
// Euclidean distance
function pointsDistance(x1, y1, x2, y2){
    return Math.sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1));
}

// Moving the player
function keyPressed(){
    if (key === 'W' || key === 'w' || keyCode === UP_ARROW){
        player.walkDirection = +1;  // Going forward
    } else if (key === 'S' || key === 's' || keyCode === DOWN_ARROW){
        player.walkDirection = -1;  // Going back
    } else if (key === 'D' || key === 'd' || keyCode === RIGHT_ARROW){
        player.turnDirection = +1;  // Going right
    } else if (key === 'A' || key === 'a' || keyCode === LEFT_ARROW){
        player.turnDirection = -1;  // Going left
    }
}

// Stop moving when the keys are no longer pressed
function keyReleased(){
    if (key === 'W' || key === 'w' || keyCode === UP_ARROW ||
        key === 'S' || key === 's' || keyCode === DOWN_ARROW){
        player.walkDirection = 0;  
    }
    if (key === 'D' || key === 'd' || keyCode === RIGHT_ARROW ||
        key === 'A' || key === 'a' || keyCode === LEFT_ARROW){
        player.turnDirection = 0;  
    }
}

function castAllRays() {

    // Start first ray subtracting half of the given field of view
    let rayAngle = player.rotationAngle - (FOV_ANGLE / 2);
    rays = [];
    // Loop all columns casting the rays
    for (let currCol = 0; currCol < NUM_RAYS; currCol++) {
        let ray = new Ray(rayAngle);
        ray.cast();
        rays.push(ray);

        rayAngle += FOV_ANGLE / NUM_RAYS;
    }
}

function setup(){
    // Initializing all objects to be used
    createCanvas(WINDOW_WIDTH, WINDOW_HEIGHT);
}

function update(){
    // Updates all game objects before the next frame is rendered
    player.update();
    castAllRays();
}

function draw(){
    // Clearing the walls so they wont stuck in frame :p
    clear("#212121");
    update();
    // Renders the objects frame by frame
    projection3D();
    grid.render();
    

    for(ray of rays){
        ray.render();
    }

    player.render();

    castAllRays();
}