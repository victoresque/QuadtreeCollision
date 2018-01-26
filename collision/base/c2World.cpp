#include "c2World.h"
#include "c2Time.h"

c2World::c2World(){
    objects = NULL;
    iterNum = C2_ITERNUM;
}

void c2World::setObjectNumber(int num){
    delete [] objects;
    
    objects = new c2Object*[num];
    
    int sqnum = sqrt(num);
    int    objSize = C2_OBJECTSIZE;
    float  speed   = C2_OBJECTSPEED;
    float  gap     = C2_OBJECTGAP;
    
    for(int i=0; i<num; i++){
        objects[i] = new c2Object(new c2Rect((i%sqnum)*objSize*gap+100,
                                             (i/sqnum)*objSize*gap+100,objSize,objSize),
                                  c2_DYNAMIC);
        double theta = rand()%360;
        if(C2_ARRANGESTYLE==1)
            objects[i]->setVelocity(speed*cos(theta*M_PI/180), speed*sin(theta*M_PI/180));
        else if(C2_ARRANGESTYLE==2){
            float speed2 = (rand()%2001-1000)/1000.0f*speed;
            objects[i]->setVelocity(speed2*cos(theta*M_PI/180), speed2*sin(theta*M_PI/180));
        }else
            objects[i]->setVelocity(0, 0);
        objects[i]->objectID = i;
    }
    objectCount = num;
    
    bounds = new c2Object*[4];
    int W = 4;
    bounds[0] = new c2Object(new c2Rect(W,SCREEN_HEIGHT/2,W,SCREEN_HEIGHT/2), c2_STATIC);
    bounds[1] = new c2Object(new c2Rect(SCREEN_WIDTH/2,W,SCREEN_WIDTH/2,W), c2_STATIC);
    bounds[2] = new c2Object(new c2Rect(SCREEN_WIDTH/2,SCREEN_HEIGHT-W,SCREEN_WIDTH/2,W), c2_STATIC);
    bounds[3] = new c2Object(new c2Rect(SCREEN_WIDTH-W,SCREEN_HEIGHT/2,W,SCREEN_HEIGHT/2), c2_STATIC);
    for(int i=0; i<4; i++){
        bounds[i]->objectID = num+i;
    }
    boundCount = 4;
}

void c2World::update(){
    for(int i=0; i<objectCount; i++){
        objects[i]->shape->R = 0x00;
        objects[i]->shape->G = 0xFF;
        objects[i]->shape->B = 0x00;
        objects[i]->shape->A = 0xFF;
    }
}

void c2World::updatePosition(){
    for(int i=0; i<objectCount; i++){
        objects[i]->transform(objects[i]->vel[0]*deltaTime/iterNum,
                              objects[i]->vel[1]*deltaTime/iterNum);
    }
}

void c2World::solveCollision(){
    
}

void c2World::showObjectsInView(){
    static int prevTime;
    if(SDL_GetTicks()-prevTime>C2_UPDATERATE){
        prevTime = SDL_GetTicks();
    }else{
        return;
    }
    int objectsInView = 0;
    for(int i=0; i<objectCount; i++){
        if(objects[i]->pos[0]>=0&&objects[i]->pos[0]<=SCREEN_WIDTH
           &&objects[i]->pos[1]>=0&&objects[i]->pos[1]<=SCREEN_HEIGHT)
            objectsInView++;
    }
    printf("%d objects\n", objectsInView);
}

void c2World::renderWorld(SDL_Renderer* renderer){
    for(int i=0; i<objectCount; i++){
        objects[i]->draw(renderer);
    }
    for(int i=0; i<boundCount; i++){
        bounds[i]->draw(renderer);
    }
}

void c2World::traceTarget(){
    objects[C2_TRACEID]->shape->R = 0xFF;
    objects[C2_TRACEID]->shape->G = 0x00;
    objects[C2_TRACEID]->shape->B = 0x00;
    objects[C2_TRACEID]->shape->A = 0xFF;
}

void c2World::showEnergy(){
    static int prevTime;
    if(SDL_GetTicks()-prevTime>C2_UPDATERATE){
        prevTime = SDL_GetTicks();
    }else{
        return;
    }
    float totalEnergy   = 0;
    for(int i=0; i<objectCount; i++){
        float energy = 0.5f * ( objects[i]->vel[0]*objects[i]->vel[0]
                               +objects[i]->vel[1]*objects[i]->vel[1]);
        totalEnergy += energy;
    }
    printf("Total energy: %f\n", totalEnergy);
}

c2World::~c2World(){
    delete [] objects;
}
