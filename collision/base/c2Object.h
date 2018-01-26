#ifndef c2Object_h
#define c2Object_h
#include "c2BoundingVolumes.h"
#include <SDL2/SDL.h>

enum c2_OBJECTTYPE{
    c2_OBJECTUNDEF,
    c2_DYNAMIC,
    c2_STATIC
};

class c2Object{
public:
    c2_OBJECTTYPE type;
    
    c2Object();
    c2Object(const c2Shape* s, c2_OBJECTTYPE);
    ~c2Object();
    
    void transform(float dx, float dy);
    void setVelocity(float vx, float vy);
    void draw(SDL_Renderer* renderer);
    
    c2Shape* shape;
    c2BoundingVolume* bv;
    float pos[2];
    float vel[2];
    float velOld[2];
    int objectID;
};

#endif
