#include "c2CollisionSolver.h"
#include "c2Broadphase.h"
#include "c2Time.h"
#include "c2Def.h"
#include <cassert>
#include <vector>
#include <cmath>
#include <algorithm>
#include <iostream>

c2CollisionSolver::c2CollisionSolver(c2World* w){
    world = w;
    broadphase = new c2BroadphaseQT(world);
}

c2CollisionSolver::~c2CollisionSolver(){
    delete broadphase;
}

void c2CollisionSolver::solveCollision(){
    world->update();
    for(int iter=0; iter<world->iterNum; iter++){
        world->updatePosition();
        int objCount = world->objectCount;
        
#ifdef C2_USEBRUTEFORCE
        c2BroadphaseBF broadphase = c2BroadphaseBF(world); std::vector<c2CollisionInfo>& collisions = *(broadphase.getCollisionInfo());
#else
        std::vector<c2CollisionInfo>& collisions = *(broadphase->getCollisionInfo());
#endif
        
        bool* vis = new bool[objCount];
        for(int i=0; i<objCount; i++){
            vis[i] = false;
        }
        
        for(int i=0; i<collisions.size(); i++){
            c2Object* obj1 = collisions[i].obj1;
            c2Object* obj2 = collisions[i].obj2;
            obj1->velOld[0] = obj1->vel[0];
            obj1->velOld[1] = obj1->vel[1];
            obj2->velOld[0] = obj2->vel[0];
            obj2->velOld[1] = obj2->vel[1];
        }
        
        for(int i=0; i<collisions.size(); i++){
            c2Object* obj1 = collisions[i].obj1;
            c2Object* obj2 = collisions[i].obj2;
            
            solveAABBAABB(obj1, obj2);
#ifdef C2_SHOWCOLLISION
            if(obj1->type==c2_DYNAMIC){
                obj1->shape->R = 0xFF;
                obj1->shape->G = 0x00;
                obj1->shape->B = 0x00;
                obj1->shape->A = 0xFF;
            }
            if(obj2->type==c2_DYNAMIC){
                obj2->shape->R = 0xFF;
                obj2->shape->G = 0x00;
                obj2->shape->B = 0x00;
                obj2->shape->A = 0xFF;
            }
#endif
        }
    }
}

void c2CollisionSolver::solveAABBAABB(c2Object* obj1, c2Object* obj2){
    assert(obj1->bv->type==c2_BVAABB);
    assert(obj2->bv->type==c2_BVAABB);
    
    c2bvAABB* aabb1 = (c2bvAABB*)obj1->bv;
    c2bvAABB* aabb2 = (c2bvAABB*)obj2->bv;
    
    // top/bot/... of aabb1
    bool collideTop   = false;
    bool collideBot   = false;
    bool collideRight = false;
    bool collideLeft  = false;
    
    float disRight = fabs((aabb1->c[0]+aabb1->r[0]) - (aabb2->c[0]-aabb2->r[0]));
    float disLeft  = fabs((aabb2->c[0]+aabb2->r[0]) - (aabb1->c[0]-aabb1->r[0]));
    float disTop   = fabs((aabb1->c[1]+aabb1->r[1]) - (aabb2->c[1]-aabb2->r[1]));
    float disBot   = fabs((aabb2->c[1]+aabb2->r[1]) - (aabb1->c[1]-aabb1->r[1]));
    float mindis   = std::min( std::min(disRight,disLeft), std::min(disTop, disBot) );
    
    if(mindis==disRight)        collideRight = true;
    else if(mindis==disLeft)    collideLeft  = true;
    else if(mindis==disTop)     collideTop   = true;
    else if(mindis==disBot)     collideBot   = true;
    else                        assert(0);
    
    if(obj1->type==c2_DYNAMIC){
        if(collideRight){
            obj1->transform(-disRight-C2_SKINDEPTH, 0);
        }
        else if(collideLeft){
            obj1->transform(disLeft+C2_SKINDEPTH, 0);
        }
        else if(collideTop){
            obj1->transform(0, -disTop-C2_SKINDEPTH);
        }
        else if(collideBot){
            obj1->transform(0, disBot+C2_SKINDEPTH);
        }
    }
    else if(obj2->type==c2_DYNAMIC){
        if(collideRight){
            obj1->transform(disRight+C2_SKINDEPTH, 0);
        }
        else if(collideLeft){
            obj1->transform(-disLeft-C2_SKINDEPTH, 0);
        }
        else if(collideTop){
            obj1->transform(0, disTop+C2_SKINDEPTH);
        }
        else if(collideBot){
            obj1->transform(0, -disBot-C2_SKINDEPTH);
        }
    }
    
    if(obj1->type==c2_DYNAMIC){
        if(obj2->type==c2_DYNAMIC){
            if(collideTop || collideBot){
                std::swap(obj1->vel[1], obj2->vel[1]);
            }
            else if(collideRight || collideLeft){
                std::swap(obj1->vel[0], obj2->vel[0]);
            }
            else{
                
            }
        }
        else if(obj2->type==c2_STATIC){
            if(collideTop || collideBot){
                obj1->vel[1] *= -1;
            }
            else if(collideRight || collideLeft){
                obj1->vel[0] *= -1;
            }
            else{
                
            }
        }
        else{
            assert(0);
        }
    }
    else if(obj1->type==c2_STATIC){
        if(obj2->type==c2_DYNAMIC){
            if(collideTop || collideBot){
                obj2->vel[1] *= -1;
            }
            else if(collideRight || collideLeft){
                obj2->vel[0] *= -1;
            }
            else{
                
            }
        }
        else if(obj2->type==c2_STATIC){
            // DO NOTHING
        }
        else{
            assert(0);
        }
    }
    else{
        assert(0);
    }
}
