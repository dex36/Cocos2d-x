ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "Enemy.h"
#include "Hero.h"

class HelloWorld : public cocos2d::Layer
{
public:
	static cocos2d::Scene* scene();
    virtual bool init();  

	bool onContactBegin(cocos2d::PhysicsContact &contact);

	virtual bool onTouchBegan(cocos2d::Touch*, cocos2d::Event*);
	virtual void onTouchEnded(cocos2d::Touch*, cocos2d::Event*);
	virtual void onTouchMoved(cocos2d::Touch*, cocos2d::Event*);
	virtual void onTouchCancelled(cocos2d::Touch*, cocos2d::Event*);

	//void setViewPointCenter(cocos2d::Point position);

    CREATE_FUNC(HelloWorld);

private:
	cocos2d::Label* labelTouchInfo; // touch
	
	cocos2d::PhysicsWorld *sceneWorld;
	void SetPhysicsWOrld(cocos2d::PhysicsWorld *world){sceneWorld = world;}

	
	void SpawnEnemy(float dt);

	Hero *myheroes;
	Enemy enemy;

	cocos2d::TMXTiledMap *tilemap;
	cocos2d::TMXLayer *background;
	cocos2d::Sprite *_player;
};

#endif
