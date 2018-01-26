#include "c2Broadphase.h"
#include "c2CollisionDetection.h"
#include <cassert>

c2Broadphase::c2Broadphase(c2World* w){
    world = w;
    collisions.clear();
}
c2BroadphaseQT::c2BroadphaseQT(c2World* w): c2Broadphase(w){
    tree = new c2bpQuadTree(world->objects, world->objectCount);
}
c2BroadphaseBF::c2BroadphaseBF(c2World* w): c2Broadphase(w){
    
}

c2Broadphase::~c2Broadphase(){
    
}

c2BroadphaseQT::~c2BroadphaseQT(){
    delete tree;
}
c2BroadphaseBF::~c2BroadphaseBF(){
    
}

std::vector<c2CollisionInfo>* c2BroadphaseQT::getCollisionInfo(){
    tree->update();
    
    c2Object** objects = world->objects;
    c2Object** bounds  = world->bounds;
    int objectCount    = world->objectCount;
    int boundCount     = world->boundCount;
    collisions.clear();
    
    collisions = tree->getCollision();
    
    for(int i=0; i<objectCount; i++){
        for(int j=0; j<boundCount; j++){
            if(c2Collide(objects[i]->bv, bounds[j]->bv)){
                collisions.push_back(c2CollisionInfo(objects[i],bounds[j]));
            }
        }
    }
    
    return &collisions;
}

std::vector<c2CollisionInfo>* c2BroadphaseBF::getCollisionInfo(){
    c2Object** objects = world->objects;
    c2Object** bounds = world->bounds;
    collisions.clear();
    
    int objectCount = world->objectCount;
    int boundCount = world->boundCount;
    
    for(int i=0; i<objectCount; i++){
        for(int j=i+1; j<objectCount; j++){
            if(c2Collide(objects[i]->bv, objects[j]->bv)){
                collisions.push_back(c2CollisionInfo(objects[i],objects[j]));
            }
        }
        for(int j=0; j<boundCount; j++){
            if(c2Collide(objects[i]->bv, bounds[j]->bv)){
                collisions.push_back(c2CollisionInfo(objects[i],bounds[j]));
            }
        }
    }
    
    return &collisions;
}
