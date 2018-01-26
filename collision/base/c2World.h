#ifndef c2World_h
#define c2World_h
#include "c2Def.h"
#include "c2Object.h"
#include <SDL2/SDL.h>
#include <cstdlib>

class c2World{
public:
    c2World();
    ~c2World();
    
    void setObjectNumber(int num);
    void update();
    void updatePosition();
    void solveCollision();
    void renderWorld(SDL_Renderer* renderer);
    void traceTarget();
    void showObjectsInView();
    void showEnergy();
    
    c2Object** objects;
    c2Object** bounds;
    
    int objectCount;
    int boundCount;
    int iterNum;
};

#endif
