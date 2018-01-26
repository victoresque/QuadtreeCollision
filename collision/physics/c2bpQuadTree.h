#ifndef c2bpQuadTree_h
#define c2bpQuadTree_h
#include <SDL2/SDL.h>
#include "c2Def.h"
#include "c2Object.h"
#include "c2BoundingVolumes.h"
#include "c2CollisionInfo.h"
#include "c2CollisionDetection.h"
#include <list>
#include <cassert>
#include <iostream>
#include <vector>

extern SDL_Renderer* gRenderer;

class c2bpQuadTree{
public:
    c2bpQuadTree(c2Object** objs, int objCount): QTMaxDepth(C2_QUADDEPTH){
        root = new node(0,0,SCREEN_WIDTH,SCREEN_HEIGHT,NULL);
        for(int i=0; i<objCount; i++){
            root->addObject(objs[i]);
        }
        build(root, 0, NULL);
        
        objects = objs;
        objectsCount = root->objectNum;
    }
    ~c2bpQuadTree(){
        clearTree(root);
    }
    
    struct node{
        node(float xmin, float ymin, float xmax, float ymax, node* _par){
            x[0] = xmin, x[1] = xmax;
            y[0] = ymin, y[1] = ymax;
            
            aabb = new c2bvAABB((xmin+xmax)/2, (ymin+ymax)/2, (xmax-xmin)/2, (ymax-ymin)/2);
            
            objects.clear();
            
            for(int i=0; i<4; i++){
                child[i] = NULL;
            }
            par = _par;
            objectNum = 0;
        }
        ~node(){
            for(int i=0; i<4; i++)
                delete child[i];
            delete aabb;
        }
        
        void addObject(c2Object* obj){
            objects.push_back(obj);
            objectNum += 1;
        }
        bool encapsule(c2Object* obj){
            assert(obj->bv->type==c2_BVAABB);
            assert(aabb->type==c2_BVAABB);
            c2bvAABB* aabb1 = (c2bvAABB*)obj->bv;
            return ((aabb1->c[0]+aabb1->r[0])<(aabb->c[0]+aabb->r[0]))
                && ((aabb1->c[0]-aabb1->r[0])>(aabb->c[0]-aabb->r[0]))
                && ((aabb1->c[1]+aabb1->r[1])<(aabb->c[1]+aabb->r[1]))
                && ((aabb1->c[1]-aabb1->r[1])>(aabb->c[1]-aabb->r[1]));
        }
        bool overlap(c2Object* obj){
            assert(obj->bv->type==c2_BVAABB);
            c2bvAABB* aabb1 = (c2bvAABB*)obj->bv;
            return c2Collide(aabb1, aabb);
        }
        
        std::list<c2Object*> objects;
        node* child[4];
        node* par;
        float x[2], y[2];
        int objectNum;
        c2bvAABB* aabb;
    };
    std::vector<c2CollisionInfo>& getCollision(){
        static std::vector<c2CollisionInfo> cinfoTemp;
        cinfoTemp.clear();
        for(int i=0; i<objectsCount; i++){
            checkObjectCollision(root, objects[i], cinfoTemp);
        }
        return cinfoTemp;
    }
    void update(){
        updateTraversalFloat(root, 0);
        updateTraversalSink(root, 0);
        
#ifdef C2_DRAWQUADTREE
        printTraversal(root);
#endif
    }
    
private:
    void printTraversal(node* n){
        if(n==NULL) return;
        
        SDL_Rect outlineRect = { (int)n->x[0], (int)n->y[0], (int)n->x[1]-(int)n->x[0], (int)n->y[1]-(int)n->y[0] };
        SDL_SetRenderDrawColor( gRenderer, 0x00, 0xFF, 0xFF, 0xFF );
        SDL_RenderDrawRect( gRenderer, &outlineRect );
        
        for(int i=0; i<4; i++){
            printTraversal(n->child[i]);
        }
    }
    
    void build(node* n, int depth, node* par){
        if(depth==QTMaxDepth) return;
        assert(n!=NULL);
        n->par = par;
        
        std::list<c2Object*>::iterator it;
        
        for(int i=0; i<4; i++){
            float xmin, xmax, ymin, ymax;
            if(i&1) xmin = n->x[0], xmax = (n->x[1]+n->x[0])/2;
            else    xmin = (n->x[1]+n->x[0])/2, xmax = n->x[1];
            if(i&2) ymin = n->y[0], ymax = (n->y[1]+n->y[0])/2;
            else    ymin = (n->y[1]+n->y[0])/2, ymax = n->y[1];
            
            if(n->child[i]==NULL){
                n->child[i] = new node(xmin,ymin,xmax,ymax,par);
            }
        
            for(it=n->objects.begin(); it!=n->objects.end(); ++it){
                if(n->child[i]->encapsule(*it)){
                    n->child[i]->addObject(*it);
                    it = n->objects.erase(it);
                    --it;
                }
            }
            if(n->child[i]->objects.size()<=0)
                continue;
            else
                build(n->child[i], depth+1, n);
        }
    }
    
    void updateTraversalFloat(node* n, int depth){
        if(n==NULL) return;
        
        for(int i=0; i<4; i++){
            updateTraversalFloat(n->child[i], depth+1);
        }
        if(n->objects.size()>0){
            std::list<c2Object*>::iterator it;
            for(it=n->objects.begin(); it!=n->objects.end(); ){
                if(!n->encapsule(*it)){
                    if(n->par!=NULL){
                        n->par->addObject(*it);
                        n->par->objectNum--;
                    }
                    it = n->objects.erase(it);
                    n->objectNum--;
                    
                    continue;
                }
                ++it;
            }
        }
        for(int i=0; i<4; i++){
            if(n->child[i]==NULL) continue;
            if(n->child[i]->objectNum==0){
                delete n->child[i];
                n->child[i] = NULL;
            }
        }
    }
    void updateTraversalSink(node* n, int depth){
        if(depth==QTMaxDepth) return;
        if(n==NULL) return;
        
        for(int i=0; i<4; i++){
            float xmin, xmax, ymin, ymax;
            if(i&1) xmin = n->x[0], xmax = (n->x[1]+n->x[0])/2;
            else    xmin = (n->x[1]+n->x[0])/2, xmax = n->x[1];
            if(i&2) ymin = n->y[0], ymax = (n->y[1]+n->y[0])/2;
            else    ymin = (n->y[1]+n->y[0])/2, ymax = n->y[1];
            
            if(n->child[i]==NULL){
                n->child[i] = new node(xmin,ymin,xmax,ymax,n);
            }
            
            if(n->objects.size()>0){
                std::list<c2Object*>::iterator it;
                for(it=n->objects.begin(); it!=n->objects.end(); ++it){
                    assert(n->aabb!=NULL);
                    
                    if(n->child[i]->encapsule(*it)){
                        n->child[i]->addObject(*it);
                        it = n->objects.erase(it);
                        
                        --it;
                    }
                    if(n->objects.size()==0) return;
                }
            }
            
            int childObjNum = n->child[i]->objectNum;
            assert(childObjNum>=0);
            if(childObjNum==0){
                ////////////////
                delete n->child[i];
                n->child[i] = NULL;
                ////////////////
            }else{
                updateTraversalSink(n->child[i], depth+1);
            }
        }
    }
    
    void checkObjectCollision(node* n, c2Object* obj, std::vector<c2CollisionInfo>& cinfoTemp){
        if(n==NULL) return;
        
        std::list<c2Object*>::iterator it;
        for(it=n->objects.begin(); it!=n->objects.end(); ++it){
            if( (*it)->objectID<=obj->objectID ) continue;
            if( c2Collide( (*it)->bv, obj->bv) ){
                cinfoTemp.push_back(c2CollisionInfo(*it, obj));
            }
        }
        for(int i=0; i<4; i++){
            if(n->child[i]!=NULL){
                if(n->child[i]->overlap(obj)){
                    checkObjectCollision(n->child[i], obj, cinfoTemp);
                }
            }
        }
    }
    
    void clearTree(node* n){
        if(n==NULL) return;
        for(int i=0; i<4; i++){
            clearTree(n->child[i]);
            if(n->child[i]!=NULL){
                delete n->child[i];
                n->child[i] = NULL;
            }
        }
    }
    
    node* root;
    const int QTMaxDepth;
    c2Object** objects;
    int objectsCount;
};

#endif
