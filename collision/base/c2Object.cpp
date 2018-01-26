#include "c2Object.h"
#include <cassert>

c2Object::c2Object(){
    shape   = NULL;
    bv      = NULL;
    pos[0] = pos[1] = vel[0] = vel[1] = 0;
    objectID = 0;
    type = c2_OBJECTUNDEF;
}

c2Object::c2Object(const c2Shape* s, c2_OBJECTTYPE _type){
    vel[0] = vel[1] = 0;
    
    c2_BVTYPE bvType = c2_BVUNDEF;
    
    if(s->type==c2_RECT){
        shape = new c2Rect(*(c2Rect*)s);
        bvType = c2_BVAABB;
    }else if(s->type==c2_CIRCLE){
        shape = new c2Circle(*(c2Circle*)s);
        bvType = c2_BVCIRCLE;
    }else{
        assert(0);
    }
    
    if(bvType==c2_BVAABB){
        c2Rect* rect = (c2Rect*) shape;
        pos[0] = rect->o.c[0];
        pos[1] = rect->o.c[1];
        bv = new c2bvAABB(pos[0],pos[1],rect->r[0], rect->r[1]);
    }else if(bvType==c2_BVCIRCLE){
        c2Circle* circle = (c2Circle*) shape;
        pos[0] = circle->o.c[0];
        pos[1] = circle->o.c[1];
        bv = new c2bvCircle(pos[0],pos[1],circle->r);
    }else{
        assert(0);
    }
    
    objectID = 0;
    type = _type;
}

c2Object::~c2Object(){
    delete shape;
    delete bv;
}

void c2Object::transform(float dx, float dy){
    pos[0] += dx;
    pos[1] += dy;

    shape->transform(dx, dy);
    bv->transform(dx, dy);
    
}
void c2Object::setVelocity(float vx, float vy){
    vel[0] = vx;
    vel[1] = vy;
}

void c2Object::draw(SDL_Renderer* renderer){
    shape->draw(renderer);
}

