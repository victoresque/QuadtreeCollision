#include <iostream>
#include <cassert>
#include <SDL2/SDL.h>
#include <cmath>
#include "Collision2D.h"
using namespace std;

SDL_Window*     gWindow   = NULL;
SDL_Renderer*   gRenderer = NULL;

c2World*        world     = NULL;
c2CollisionSolver* solver = NULL;
float           deltaTime;

bool init();
void close();

Uint32 frames = 0;
Uint32 timef = 0;
void showFramerate(){
    frames += 1;
    timef += deltaTime;
    
    static int prevTime;
    if(SDL_GetTicks()-prevTime>C2_UPDATERATE){
        prevTime = SDL_GetTicks();
    }else{
        return;
    }
    
    printf("%d fps\n", frames*1000/timef);
    timef = 0;
    frames = 0;
}

int main(){
    srand(time(NULL));
    
    Uint32 prevTime = 0;
    Uint32 currTime = 0;
    
    if( !init() ){
        printf( "Failed to initialize!\n" );
    }else{
        bool quit = false;
        SDL_Event e;
        
        world = new c2World;
        world->setObjectNumber(C2_OBJECTNUM);
        solver = new c2CollisionSolver(world);
        
        SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
        SDL_RenderClear( gRenderer );
        SDL_RenderPresent( gRenderer );
        
        SDL_Delay(1000);
        currTime = SDL_GetTicks();
        
        while( !quit ){
            while( SDL_PollEvent( &e ) != 0 ){
                if( e.type == SDL_QUIT ){
                    quit = true;
                }
            }
            
            SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
            SDL_RenderClear( gRenderer );
            
            prevTime = currTime;
            currTime = SDL_GetTicks();
            
            deltaTime = currTime-prevTime;
            
#ifdef C2_SHOWOBJECTNUMINVIEW
            world->showObjectsInView();
            world->showEnergy();
#endif
#ifdef C2_SHOWFRAMERATE
            showFramerate();
#endif
            solver->solveCollision();
#ifdef C2_TRACETARGET
            world->traceTarget();
#endif
            world->renderWorld(gRenderer);
            SDL_RenderPresent( gRenderer );
            
            
        }
        
    }
    close();
    return 0;
}

bool init(){
    bool success = true;
    
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 ){
        printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
        success = false;
    }else{
        if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) ){
            printf( "Warning: Linear texture filtering not enabled!" );
        }
        
        gWindow = SDL_CreateWindow( "Collision2D", SDL_WINDOWPOS_UNDEFINED, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
        if( gWindow == NULL ){
            printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
            success = false;
        }else{
            gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );
            if( gRenderer == NULL ){
                printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
                success = false;
            }else{
                SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
            }
        }
    }
    return success;
}

void close(){
    SDL_DestroyRenderer( gRenderer );
    SDL_DestroyWindow( gWindow );
    gWindow = NULL;
    gRenderer = NULL;
    
    SDL_Quit();
}

