#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include <Box2D/Box2D.h>
#include "GLES-Render.h"
#include <iostream>
#include "ui/CocosGUI.h"
#include "HelloWorldHub.h"

#define PTM_RATIO 32.0

struct Segment
{
	b2Vec2 p1;
	b2Vec2 p2;
};

class  HelloWorldScene : public cocos2d::Layer, public b2RayCastCallback, public b2ContactListener
{
public:
	static cocos2d::Scene* scene();
	virtual bool init();
	virtual void draw(cocos2d::Renderer* renderer, const cocos2d::Mat4& transform, uint32_t flags) override; 

	void prepareLayers();
	void createFixtures(cocos2d::TMXLayer* layer);
	void createRectangularFixture(cocos2d::TMXLayer* layer, int x, int y, float width, float height);

	//moving hero
	bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	virtual void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);

	void setViewPointCenter(cocos2d::Point position);
	void setPlayerPosition(cocos2d::Point position);
	cocos2d::Point tileCoordForPosition(cocos2d::Point position);
	cocos2d::Vec2 fixscreen;
	bool moving;
	bool upjumpfix;
	b2Body *bodybox;

	//bullets
	cocos2d::DrawNode *bulletdraw;
	b2Body *bullet;
	bool bullet_working;
	bool bullet_destroy_body;

	//slice body
	cocos2d::DrawNode* slicedraw[10]; 
	int table_slice_draw = 0;

	void addNewSpriteWithCoords2();
	bool m_bSliceObject;
	bool m_bIsRayCast;
	bool m_bIsDraw;
	bool m_bIsDrawEnter;
	Segment* laserSegment;
	std::vector<b2Vec2> entryPoint;
	std::vector<b2Body*> affectedByLaser;
	virtual float32 ReportFixture(b2Fixture* fixture, const b2Vec2& point,const b2Vec2& normal, float32 fraction) override;
	b2Vec2 findCenter(std::vector<b2Vec2>& vertices, int count);
	void createSlice(std::vector<b2Vec2>& vertices, int count);
	cocos2d::Vec2 m_startPt;
	cocos2d::Vec2 m_endPoint;
	CREATE_FUNC(HelloWorldScene);

private:
	b2World* world;
	GLESDebugDraw* debugDraw;
	~HelloWorldScene();
	void initPhysics();
	void update(float dt);

	//tile map
	cocos2d::TMXTiledMap *_tileMap;
	cocos2d::TMXLayer *_background;
	cocos2d::TMXLayer *_blockage;
	cocos2d::TMXLayer *_foreground;
	cocos2d::TMXLayer *_collect;
	cocos2d::Sprite *hero;
	cocos2d::DrawNode *drawes;
	b2BodyDef BodyDef;
	b2Body *herobody;

	float withTileMap;
	static HelloWorldHud *_hud;
};

#endif
