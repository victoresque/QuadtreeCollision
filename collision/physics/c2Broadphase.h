#ifndef c2Broadphase_h
#define c2Broadphase_h
#include "c2World.h"
#include "c2Object.h"
#include "c2bpQuadTree.h"
#include "c2bpAABBTree.h"
#include "c2CollisionInfo.h"
#include <vector>

class c2Broadphase{
public:
    c2Broadphase(c2World*);
    virtual ~c2Broadphase();
    
    virtual std::vector<c2CollisionInfo>* getCollisionInfo() = 0;
    std::vector<c2CollisionInfo> collisions;
    
    c2World* world;
};

class c2BroadphaseQT: public c2Broadphase{
public:
    c2BroadphaseQT(c2World*);
    ~c2BroadphaseQT();
    
    c2bpQuadTree* tree;
    std::vector<c2CollisionInfo>* getCollisionInfo();
};

class c2BroadphaseBF: public c2Broadphase{
public:
    c2BroadphaseBF(c2World*);
    ~c2BroadphaseBF();
    
    std::vector<c2CollisionInfo>* getCollisionInfo();
};

class c2BroadphaseAT: public c2Broadphase{
public:
    c2bpAABBTree* tree;
};

#endif
