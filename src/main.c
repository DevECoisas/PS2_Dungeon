#include "./constants.h"
#include "./object.h"
#include "./image.h"
#include <unistd.h>
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

float rockbottom; 
Room = 1;

///////////////////////////////////////////////////////////////////////////////
// Declare the objects (a type that I made to keep things simple)
///////////////////////////////////////////////////////////////////////////////

object player;
SDL_Texture* Background = NULL;

object roof1;
object key1;
object door1;
object sqr1;
bool dooropen1 = false;
int time1=30;

object wall1;
object wall2;

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

    const SDL_GameControllerButton XState = SDL_GameControllerGetButton(controller,0); 
    const SDL_GameControllerButton OState = SDL_GameControllerGetButton(controller,1); 
    const SDL_GameControllerButton SQRState = SDL_GameControllerGetButton(controller,2);

    const float JoyLeftXState = SDL_JoystickGetAxis(joystick,0);

    int x_axis = SDL_JoystickGetAxis(joystick, 0);
    int y_axis = SDL_JoystickGetAxis(joystick, 1);
    player.x += player.speedX * delta_time * (x_axis / 32767);

    if(RightState == SDL_PRESSED){
        player.x += player.speedX * delta_time;
    }

    if(LeftState == SDL_PRESSED){
        player.x -= player.speedX * delta_time;
    }

    if(XState == SDL_PRESSED && player.onfloor){
        player.gravity = -3;
    }else if(!player.onfloor){
        player.gravity = 0.2;
    }

    if (OState == SDL_PRESSED)
    {
        printf("%f ... %f \n",player.x,player.y);
    }

    if (Room == 1)
        if(dooropen1 && hasCollision(player,door1) && SQRState == SDL_PRESSED){
            Background = loadBMPText("Assets/Room2.bmp",renderer);

            door1.text = loadBMPText("Assets/door.bmp"  ,renderer);
            dooropen1 = false;
            printf("ROOM 2\n");
            Room = 2;

            player.x = 45;
            player.y = 236;

            key1.x = 371;
            key1.y = 212;

        } 

    if (Room == 2)
        if(dooropen1 && hasCollision(player,door1) && SQRState == SDL_PRESSED){
            Background = loadBMPText("Assets/Room3.bmp",renderer);

            door1.text = loadBMPText("Assets/door.bmp"  ,renderer);
            dooropen1 = false;
            printf("ROOM 3\n");
            Room = 3;

            player.x = 45;
            player.y = 236;

            key1.x = 371;
            key1.y = 212;

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

    wall1   = initObject(200,300,100,50,false);
    wall2   = initObject(350,250,100,50,false);
    //the object type needs some variables that can be put here to stay more simple
    player.speedX = 200;
    //speedX it's in object type but will be removed soon
    
    player.text  = loadBMPText("Assets/player.bmp",renderer);
    key1.text    = loadBMPText("Assets/key.bmp"   ,renderer);
    door1.text   = loadBMPText("Assets/door.bmp"  ,renderer);
    wall1.text   = loadBMPText("Assets/plat.bmp"  ,renderer);
    wall2.text   = loadBMPText("Assets/plat.bmp"  ,renderer);
    Background       = loadBMPText("Assets/room1.bmp" ,renderer);

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
    //it waits a little if it needs to

    if(Room == 1){
        rockbottom = WINDOW_HEIGHT - player.height - 164;
        if(hasCollision(player,key1)){
            dooropen1 = true;
            key1.y = 999;
            SDL_DestroyTexture(door1.text);
            door1.text = loadBMPText("Assets/dooro.bmp"  ,renderer);
        }
    
        if (player.y > rockbottom || hasCollision(player,roof1)) {
            if (player.y > rockbottom)
            player.y = rockbottom;
            player.onfloor = true;
        } else {
            player.onfloor = false;
        }

        
    }

    if(Room == 2){
        rockbottom = WINDOW_HEIGHT - player.height - 164;
        if(hasCollision(player,key1)){
            dooropen1 = true;
            key1.y = 999;
            SDL_DestroyTexture(door1.text);
            door1.text = loadBMPText("Assets/dooro.bmp"  ,renderer);
        }
    
        if ((player.y > rockbottom && player.y < 280 && player.x < 120)|| (player.y > rockbottom && player.y < 280 && player.x > 450) || hasCollision(player,roof1)) {
            if (player.y > rockbottom)
            player.y = rockbottom;
            player.onfloor = true;
        } else {
            player.onfloor = false;
        }

        player = backInCollision(player,wall1);
        player = backInCollision(player,wall2);
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
void render(void) {
    if (player.text == NULL)
    {SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
     player.text = loadBMPText("Assets/player.bmp",renderer);}
    else{SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);}
    
    SDL_RenderClear(renderer);

    SDL_Rect RBack = {0,0, WINDOW_WIDTH, WINDOW_HEIGHT};
        SDL_RenderCopy(renderer, Background, NULL, &RBack);

    if(Room == 1){

        blitObject(roof1,renderer);
        blitObject(key1,renderer);
        blitObject(door1,renderer);

        if(dooropen1 && hasCollision(player,door1)){
            sqr1.text = sqr1.anim[0];
            blitAnimateObject(sqr1,renderer,time1);
            time1++;
            if (time1 > 120)
                time1 = 30;
        }
    }
    if(Room == 2){
        blitObject(key1,renderer);
        blitObject(door1,renderer);
        blitObject(wall1,renderer);
        blitObject(wall2,renderer);

        if(dooropen1 && hasCollision(player,door1)){
            sqr1.text = sqr1.anim[0];
            blitAnimateObject(sqr1,renderer,time1);
            time1++;
            if (time1 > 120)
                time1 = 30;
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
    SDL_DestroyTexture(wall1.text);
    SDL_DestroyTexture(Background);

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
    sleep(5);

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