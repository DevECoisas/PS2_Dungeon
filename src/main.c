#include "./constants.h"
#include "./object.h"
#include "./image.h"
#include "port.h"

//"UNIX" = 1, "PS2" = 2 // every time it tries to see from what it's going to port


///////////////////////////////////////////////////////////////////////////////
// Global variables
///////////////////////////////////////////////////////////////////////////////
int game_is_running = false;
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
int last_frame_time = 0;

SDL_GameController* controller;
SDL_Joystick* joystick;
float delta_time;

SDL_Texture* Tsala1 = NULL;

float rockbottom; 
Room = 1;

///////////////////////////////////////////////////////////////////////////////
// Declare the objects (a type that I made to keep things simple)
///////////////////////////////////////////////////////////////////////////////

object player;

object roof1;
object key1;
object door1;
object sqr1;
bool dooropen1 = false;

////////////////////////////////////////////////////////////////////////////////
// Function to fix the improper TEXA value set by gsKit internally //by: F0bes//
////////////////////////////////////////////////////////////////////////////////
#ifdef PORTING_TO
    #if PORTING_TO == 2
        #include <tamtypes.h>
        void fix_texa_value(void)
        {
    qword_t draw_packet[2] __aligned(16);
    qword_t* q = draw_packet;

    q->dw[0] = 0x1000000000008001u; // GIF_TAG
    q->dw[1] = 0x0E; // Write to AD register
    q++;
    // TEXA
    q->dw[0] = (u64)0x80 << 32; // TA1 = 0x80 AEM = 0 TA0 = 0 
    q->dw[1] = 0x3B; // TEXA register
    q++;

    *((vu32*)0x1000a010) = (u32)&draw_packet[0]; // Point the channel to our data
    *((vu32*)0x1000a020) = 2; // # of qwords (2)

    *((vu32*)0x1000a000) = 0x100; // Run the DMA channel

    while(*((vu32*)0x1000a000) & 0x100); // Wait for DMA transfer
    }
    #endif
#endif

///////////////////////////////////////////////////////////////////////////////
// Function to initialize the SDL window
///////////////////////////////////////////////////////////////////////////////
int initialize_window(void) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        fprintf(stderr, "Error initializing SDL.\n");
        return false;
    }
    window = SDL_CreateWindow(
        NULL,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_BORDERLESS
    );
    if (!window) {
        fprintf(stderr, "Error creating SDL Window.\n");
        return false;
    }
    renderer = SDL_CreateRenderer(window, -1, 0);
    if (!renderer) {
        fprintf(stderr, "Error creating SDL Renderer.\n");
        return false;
    }
    controller = SDL_GameControllerOpen(0);
    if(controller == NULL){ 
        printf("Failed to open the game controller!\n");
    }else 
        printf("Controller OK!\n");

    joystick = SDL_JoystickOpen(0);
    if (joystick == NULL) {
        printf("Failed to open joystick\n");
    }else {
        printf("Joystick opened successfully\n");
    }        
    return true;
}

/////////////////////////////////////////////////////////////////////////////////////
// Function to read the events that are happening and interprets it in diferent ways
/////////////////////////////////////////////////////////////////////////////////////
void process_input(void) {
    SDL_Event event;
    SDL_PollEvent(&event);
    switch (event.type) {
        case SDL_QUIT:
            game_is_running = false;
            break;
    }

    //const SDL_GameControllerButton UpState = SDL_GameControllerGetButton(controller,11);
    //const SDL_GameControllerButton DownState = SDL_GameControllerGetButton(controller,12);
    const SDL_GameControllerButton LeftState = SDL_GameControllerGetButton(controller,13);
    const SDL_GameControllerButton RightState = SDL_GameControllerGetButton(controller,14);

    const SDL_GameControllerButton AState = SDL_GameControllerGetButton(controller,0); 

    const float JoyLeftXState = SDL_JoystickGetAxis(joystick,0);

    int x_axis = SDL_JoystickGetAxis(joystick, 0);
    int y_axis = SDL_JoystickGetAxis(joystick, 1);
    player.x += player.speedX * delta_time * (x_axis / 32767);

    if(RightState == SDL_PRESSED)
        player.x += player.speedX * delta_time;
    if(LeftState == SDL_PRESSED)
        player.x -= player.speedX * delta_time;
    if(AState == SDL_PRESSED && player.onfloor){
        player.gravity = -3;
    }else if(!player.onfloor){
        player.gravity = 0.2;
    }

}

///////////////////////////////////////////////////////////////////////////////
// Setup function that runs once at the beginning of the program
///////////////////////////////////////////////////////////////////////////////
void Nsetup(void) {
    player = initObject(200,200,48,48,false);
    sqr1   = initObject(540,160,40,40,false);
    door1  = initObject(520,204,80,80,false);
    roof1  = initObject(0 ,100,640,50,false);
    key1   = initObject(350,240,20,40,false);
    //the object type needs some variables that can be put here to stay more simple
    player.speedX = 200;
    //speedX it's in object type but will be removed soon
    
    player.text  = loadBMPText("Assets/player.bmp",renderer);
    key1.text    = loadBMPText("Assets/key.bmp"   ,renderer);
    door1.text   = loadBMPText("Assets/door.bmp"  ,renderer);
    Tsala1       = loadBMPText("Assets/room1.bmp" ,renderer);

    sqr1    = setAnimation(sqr1,3,renderer,(char*[]){"Assets/sqrbtn0.bmp","Assets/sqrbtn1.bmp","Assets/sqrbtn2.bmp"});
    //I also made a texture-from-path

}

///////////////////////////////////////////////////////////////////////////////
// Update function with a fixed time step 
///////////////////////////////////////////////////////////////////////////////
void update(void) {
    int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - last_frame_time);
    if (time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME)
        SDL_Delay(time_to_wait);
    delta_time = (SDL_GetTicks() - last_frame_time) / 1000.0;
    last_frame_time = SDL_GetTicks();
    //it waits a little if is needed to
    if(Room == 1){
        rockbottom = WINDOW_HEIGHT - player.height - 164;
        if(hasCollision(player,key1)){
            dooropen1 = true;
            SDL_DestroyTexture(key1.text);
            key1.y = 999;
            SDL_DestroyTexture(door1.text);
            door1.text = loadBMPText("Assets/dooro.bmp"  ,renderer);
        }
    }

    if (player.y > rockbottom || hasCollision(player,roof1)) {
        if (player.y > rockbottom)
        player.y = rockbottom;
        player.onfloor = true;
    } else {
        player.onfloor = false;
    }

    player = backInCollision(player,roof1);
    //checks if it's colliding and block it (on the more above code)


    /////////////////////////////////////////////////////////////////////////////////
    if (player.onfloor)
        player.gravitySpeed = 0;

    player.gravitySpeed += player.gravity;
    player.y += player.gravitySpeed;
    //some gravity to the player
}

///////////////////////////////////////////////////////////////////////////////
// Render function to draw game objects in the SDL window
///////////////////////////////////////////////////////////////////////////////
int u=30;
void render(void) {
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderClear(renderer);

    if(Room == 1){
        SDL_Rect Rsala1 = {0,0, WINDOW_WIDTH, WINDOW_HEIGHT};
        SDL_RenderCopy(renderer, Tsala1, NULL, &Rsala1);

        blitObject(roof1,renderer);
        blitObject(key1,renderer);
        blitObject(door1,renderer);

        if(dooropen1 && hasCollision(player,door1)){
            sqr1.text = sqr1.anim[0];
            blitAnimateObject(sqr1,renderer,u);
            u++;
            if (u > 120)
                u = 30;
        }
    }

    blitObject(player,renderer);


    SDL_RenderPresent(renderer);
    //it renders some images and present it
}

///////////////////////////////////////////////////////////////////////////////
// Function to destroy SDL window and renderer
///////////////////////////////////////////////////////////////////////////////
void destroy_window(void) {    
    SDL_DestroyTexture(player.text);

    SDL_DestroyTexture(roof1.text);
    SDL_DestroyTexture(key1.text);
    SDL_DestroyTexture(door1.text);
    SDL_DestroyTexture(Tsala1);

    SDL_JoystickClose(joystick);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    //Destroy some things that was imported
}

///////////////////////////////////////////////////////////////////////////////
// Main function
///////////////////////////////////////////////////////////////////////////////
int main(int argc, char* args[]) {
    game_is_running = initialize_window();// init window and some other important things

    Nsetup();// set up some things (is Nsetup and not Setup because it's causing conflict with another function)

    #ifdef PORTING_TO
    #if PORTING_TO == 2
        fix_texa_value();
    #endif
    #endif
    //again, only if is been ported to ps2 that this will work

    while (game_is_running) {
        process_input();// process the input values
        update(); // update some things (and slow down the code if it's too fast)
        render(); // render images
    }

    destroy_window(); // destroy some variables

    return 0;// end
}
//