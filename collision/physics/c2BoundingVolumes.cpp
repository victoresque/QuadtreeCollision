#include "c2BoundingVolumes.h"
#include "c2CollisionDetection.h"

c2bvAABB::c2bvAABB(float x, float y, float rx, float ry){
    type = c2_BVAABB;
    c[0] = x;
    c[1] = y;
    r[0] = rx;
    r[1] = ry;
}

c2bvCircle::c2bvCircle(float x, float y, float r){
    type = c2_BVCIRCLE;
    c[0] = x;
    c[1] = y;
    r    = r;
}

bool c2BoundingVolume::collideWith(const c2BoundingVolume * bv){
    return c2Collide(this, bv);
}

void c2bvAABB::transform(float dx, float dy){
    c[0] += dx;
    c[1] += dy;
}

void c2bvCircle::transform(float dx, float dy){
    c[0] += dx;
    c[1] += dy;
}

c2BoundingVolume::~c2BoundingVolume(){
    
}

c2bvAABB::~c2bvAABB(){
    
}

c2bvCircle::~c2bvCircle(){
    
}
