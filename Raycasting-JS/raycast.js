const TILE_SIZE = 32;
// MxN size Matrix 
const MAP_NUM_ROWS = 11;
const MAP_NUM_COLS = 15;
// This defines the window proportions
const WINDOW_WIDTH = MAP_NUM_COLS*TILE_SIZE;
const WINDOW_HEIGHT = MAP_NUM_ROWS*TILE_SIZE;
// Field of view angle
const FOV_ANGLE =  60 * (Math.PI/180);

const WALL_STRIP_WIDTH = 4;
// Rays to be <<casted>> per space
const NUM_RAYS = WINDOW_WIDTH/WALL_STRIP_WIDTH;

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
                rect(tileX,tileY, TILE_SIZE, TILE_SIZE);
            }
        }
    }
}
class Player{
    constructor(){
        this.x = WINDOW_WIDTH/2;
        this.y = WINDOW_HEIGHT/2;
        this.radius = 6;
        this.turnDirection = 0; // -1 if left, +1 if right
        this.walkDirection = 0; // -1 if back, +1 if front
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
        circle(this.x, this.y, this.radius);
        stroke("red");
        // Line that shows where the player is facing
        line(this.x,
            this.y,
            this.x + Math.cos(this.rotationAngle)*30,
            this.y + Math.sin(this.rotationAngle)*30
            );
    }
}

class Ray{
    constructor(rayAngle){
        this.rayAngle = rayAngle;
    }
    render(){
        stroke("orange");
        line(player.x, player.y, 
            player.x + Math.cos(this.rayAngle)*30,
            player.y + Math.sin(this.rayAngle)*30    
        );
    }
}

let grid = new Map();
let player = new Player();
let rays = [];

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

function castAllRays(){
    let columnId = 0;
    // Start first ray substracting half of the given field of view
    let rayAngle = player.rotationAngle - (FOV_ANGLE/2);
    rays = [];
    // Loop all columns casting the rays
    for(let i = 0; i < NUM_RAYS; i++){
        let ray = new Ray (rayAngle);
        // TODO: ray.cast();
        rays.push(ray);

        rayAngle += FOV_ANGLE / NUM_RAYS;

        columnId++;
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
    update();
    // Renders the objects frame by frame
    
    grid.render();
    player.render();

    for(ray of rays){
        ray.render();
    }
}