#include "c2CollisionDetection.h"

bool c2Collide(const c2BoundingVolume* a, const c2BoundingVolume* b){
    bool collide = false;
    
    c2_BVTYPE BVType1 = a->type;
    c2_BVTYPE BVType2 = b->type;
    
    if(BVType1 == c2_BVAABB){
        if(BVType2 == c2_BVAABB){
            collide = c2CollideAABBAABB((const c2bvAABB*)a, (const c2bvAABB*)b);
        }else if(BVType2 == c2_BVCIRCLE){
            collide = c2CollideAABBCircle((const c2bvAABB*)a, (const c2bvCircle*)b);
        }
            
    }else if(BVType1 == c2_BVCIRCLE){
        if(BVType2 == c2_BVAABB){
            collide = c2CollideAABBCircle((const c2bvAABB*)b, (const c2bvCircle*)a);
        }else if(BVType2 == c2_BVCIRCLE){
            collide = c2CollideCircleCircle((const c2bvCircle*)a, (const c2bvCircle*)b);
        }
        
    }
    
    return collide;
}

bool c2CollideAABBAABB(const c2bvAABB* a, const c2bvAABB* b){
    if( fabs(a->c[0]-b->c[0]) > a->r[0]+b->r[0] ) return false;
    if( fabs(a->c[1]-b->c[1]) > a->r[1]+b->r[1] ) return false;
    return true;
}

bool c2CollideAABBCircle(const c2bvAABB* a, const c2bvCircle* b){
    float AABBL = a->c[0] - a->r[0];
    float AABBR = a->c[0] + a->r[0];
    float AABBD = a->c[1] - a->r[1];
    float AABBU = a->c[1] + a->r[1];
    
    c2Point closestPoint;
    
    if(b->c[0]<=AABBL){
        closestPoint.c[0] = AABBL;
        if(b->c[1]>=AABBU){
            closestPoint.c[1] = AABBU;
        }else if(b->c[1]<=AABBD){
            closestPoint.c[1] = AABBD;
        }else{
            closestPoint.c[1] = b->c[1];
        }
    }else if(b->c[0]>=AABBR){
        closestPoint.c[0] = AABBR;
        if(b->c[1]>=AABBU){
            closestPoint.c[1] = AABBU;
        }else if(b->c[1]<=AABBD){
            closestPoint.c[1] = AABBD;
        }else{
            closestPoint.c[1] = b->c[1];
        }
    }else{
        if(b->c[1]>=AABBU){
            closestPoint.c[0] = b->c[0];
            closestPoint.c[1] = AABBU;
        }else if(b->c[1]<=AABBD){
            closestPoint.c[0] = b->c[0];
            closestPoint.c[1] = AABBD;
        }else{
            closestPoint.c[0] = b->c[0];
            closestPoint.c[1] = b->c[1];
        }
    }
    float dx    = closestPoint.c[0] - b->c[0];
    float dy    = closestPoint.c[1] - b->c[1];
    
    return dx*dx+dy*dy <= b->r*b->r;
}

bool c2CollideCircleCircle(const c2bvCircle* a, const c2bvCircle* b){
    float dx    = a->c[0] - b->c[0];
    float dy    = a->c[1] - b->c[1];
    float dr    = a->r + b->r;
    return dx*dx+dy*dy <= dr*dr;
}




