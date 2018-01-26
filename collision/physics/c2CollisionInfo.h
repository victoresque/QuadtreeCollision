#ifndef c2CollisionInfo_h
#define c2CollisionInfo_h

#include "c2Object.h"

class c2CollisionInfo{
public:
    c2CollisionInfo(c2Object*, c2Object*);
    
    c2Object* obj1;
    c2Object* obj2;
};

#endif
