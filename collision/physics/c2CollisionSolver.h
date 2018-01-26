#ifndef c2CollisionSolver_h
#define c2CollisionSolver_h

#include "c2CollisionInfo.h"
#include "c2World.h"
#include "c2Broadphase.h"

class c2CollisionSolver{
public:
    c2CollisionSolver(c2World*);
    ~c2CollisionSolver();
    void solveCollision();
    
private:
    void solveAABBAABB(c2Object*, c2Object*);
    
    c2World* world;
    c2Broadphase* broadphase;
};

#endif
