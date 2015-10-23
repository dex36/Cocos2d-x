
#ifndef Cut_the_Bomb_RayCastCallback_h
#define Cut_the_Bomb_RayCastCallback_h

#include <Box2D/Box2D.h>

class RaycastCallback : public b2RayCastCallback
{
public:
    RaycastCallback(){
    }
    
    float32 ReportFixture(b2Fixture *fixture,const b2Vec2 &point,const b2Vec2 &normal,float32 fraction)
    {
        return 1;
    }
    
 
};

#endif
