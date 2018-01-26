#ifndef c2Shapes_h
#define c2Shapes_h
#include <SDL2/SDL.h>
#include "c2Def.h"

enum c2_SHAPETYPE{
    c2_SHAPEUNDEF,
    C2_POINT,
    c2_CIRCLE,
    c2_RECT
};

class c2Shape{
public:
    c2_SHAPETYPE type;
    
    c2Shape();
    virtual ~c2Shape();
    
    virtual void transform(float dx, float dy) = 0;
    virtual void draw(SDL_Renderer* renderer) = 0;
    
    Uint8 R, G, B, A;
};

class c2Point: public c2Shape{
public:
    c2Point(float x=0, float y=0);
    ~c2Point();
    
    void transform(float dx, float dy);
    void draw(SDL_Renderer* renderer);
    
    float c[2];
};

class c2Circle: public c2Shape{
public:
    c2Circle(float x, float y, float r);
    ~c2Circle();
    
    void transform(float dx, float dy);
    void draw(SDL_Renderer* renderer);
    
    c2Point o;
    float r;
};

class c2Rect: public c2Shape{
public:
    c2Rect(float x, float y, float rx, float ry);
    ~c2Rect();
    
    void transform(float dx, float dy);
    void draw(SDL_Renderer* renderer);
    
    c2Point o;
    float r[2];
};

#endif
