#ifndef c2BoundingVolumes_h
#define c2BoundingVolumes_h
#include "c2Shapes.h"

enum c2_BVTYPE{
    c2_BVUNDEF,
    c2_BVAABB,
    c2_BVCIRCLE
};

class c2BoundingVolume{
public:
    virtual ~c2BoundingVolume();
    
    c2_BVTYPE type;
    bool collideWith(const c2BoundingVolume*);
    virtual void transform(float dx, float dy) = 0;
};

class c2bvAABB: public c2BoundingVolume{
public:
    ~c2bvAABB();
    
    c2bvAABB(float x, float y, float rx, float ry);
    
    void transform(float dx, float dy);
    
    float r[2];
    float c[2];
};

class c2bvCircle: public c2BoundingVolume{
public:
    ~c2bvCircle();
    
    c2bvCircle(float x, float y, float r);
    
    void transform(float dx, float dy);
    
    float r;
    float c[2];
};


#endif
