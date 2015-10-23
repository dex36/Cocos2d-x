#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include <vector>
#include "physics3d/CCPhysics3D.h"
#include "HelloWorldHub.h"

static cocos2d::Scene *psychics_scene;

class HelloWorld : public cocos2d::Layer
{
public:
    virtual bool init() override;
	static cocos2d::Scene* scene();

    virtual void onTouchesBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event);
    virtual void onTouchesMoved(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event);
    virtual void onTouchesEnded(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event);

	void myupdate(float dt);
	CREATE_FUNC(HelloWorld);

	void shootBoxfunc(const cocos2d::Vec3 &des);

	cocos2d::Physics3DWorld *sceneWorld;
	void SetPhysics3DWOrld(cocos2d::Physics3DWorld *world){ sceneWorld = world; }

private:
	//cocos2d::Terrain *_terrain;
	cocos2d::Sprite3D *_plan;

	cocos2d::Sprite3D *car_cabine;
	cocos2d::Physics3DRigidBody *physics_rbd_cabine;
	cocos2d::Physics3DRigidBody *physics_rbd_wheel_bar;
	cocos2d::Sprite3D *wheel1;
	cocos2d::Sprite3D *wheel2;
	cocos2d::Sprite3D *wheel3;
	cocos2d::Sprite3D *wheel4;
	cocos2d::Sprite3D *car_wheel_bar;
	float speedup = 0;



	cocos2d::Camera *camera;
	cocos2d::Vec3 vector_camera_for_box;
	float _angle = 0;

	bool shootBox;

	static HelloWorldHud *_hud;
};

#endif 


//cocos2d::Camera* camera;
//cocos2d::Vec3 labelPos;
//float pos = 0;
