#ifndef c2CollisionDetection_h
#define c2CollisionDetection_h
#include "c2Object.h"

bool c2Collide(const c2BoundingVolume*, const c2BoundingVolume*);

bool c2CollideAABBAABB(const c2bvAABB*, const c2bvAABB*);
bool c2CollideAABBCircle(const c2bvAABB*, const c2bvCircle*);
bool c2CollideCircleCircle(const c2bvCircle*, const c2bvCircle*);

#endif /* c2CollisionDetection_h */
