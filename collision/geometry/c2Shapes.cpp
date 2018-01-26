#include "c2Shapes.h"

c2Shape::c2Shape(){
    R = 0x00;
    G = 0x00;
    B = 0x00;
    A = 0xFF;
}

c2Point::c2Point(float x, float y){
    type = C2_POINT;
    c[0] = x;
    c[1] = y;
}

c2Circle::c2Circle(float x, float y, float r){
    type   = c2_CIRCLE;
    o.c[0] = x;
    o.c[1] = y;
    r      = r;
}

c2Rect::c2Rect(float x, float y, float rx, float ry){
    type   = c2_RECT;
    o.c[0] = x;
    o.c[1] = y;
    r[0]   = rx;
    r[1]   = ry;
}

void c2Point::transform(float dx, float dy){
    c[0] += dx;
    c[1] += dy;
}
void c2Circle::transform(float dx, float dy){
    o.transform(dx, dy);
}

void c2Rect::transform(float dx, float dy){
    o.transform(dx, dy);
}

void c2Point::draw(SDL_Renderer* renderer){
    
}

void c2Circle::draw(SDL_Renderer* renderer){
}

void c2Rect::draw(SDL_Renderer* renderer){
    SDL_Rect outlineRect = { (int)(o.c[0]-r[0]), (int)(o.c[1]-r[1]), (int)(r[0]*2), (int)(r[1]*2) };
    SDL_SetRenderDrawColor( renderer, R, G, B, A );
    SDL_RenderFillRect( renderer, &outlineRect );
}


c2Shape::~c2Shape(){
    
}
c2Point::~c2Point(){
    
}
c2Circle::~c2Circle(){
    
}
c2Rect::~c2Rect(){
    
}
