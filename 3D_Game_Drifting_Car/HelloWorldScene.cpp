#include "HelloWorldScene.h"
#include "AppMacros.h"
#include "GL\glew.h"

USING_NS_CC;

HelloWorldHud *HelloWorld::_hud = NULL;

Scene* HelloWorld::scene()
{
	psychics_scene = Scene::createWithPhysics();
	psychics_scene->getPhysics3DWorld();
	psychics_scene->getPhysics3DWorld()->setGravity(Vec3(0, -20, 0));
	psychics_scene->getPhysicsWorld()->setAutoStep(false);
	psychics_scene->getPhysics3DWorld()->stepSimulate(1 / 100);
	psychics_scene->getPhysics3DWorld()->setDebugDrawEnable(true);

	auto layer = HelloWorld::create();
	layer->SetPhysics3DWOrld(psychics_scene->getPhysics3DWorld());
	psychics_scene->addChild(layer);

	auto hud = HelloWorldHud::create();
	_hud = hud;
	psychics_scene->addChild(_hud, 1);

	return psychics_scene;
}

bool HelloWorld::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();

	camera = Camera::createPerspective(30, (float)visibleSize.width / visibleSize.height, 1.0, 5000);
	camera->setBackgroundBrush(CameraBackgroundBrush::createColorBrush(Color4F(0.0f, 1.0f, 1.0f, 0.5f), 1.0f));
	camera->setPosition3D(Vec3(0.0f, 50.0f, 100.0f));
	camera->lookAt(Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0, 1.0, 0.0));
	camera->setCameraFlag(CameraFlag::USER2);
	this->addChild(camera);

	//Create Plane
	Physics3DRigidBodyDes rbd_plan;
	rbd_plan.mass = 0.0f;
	rbd_plan.shape = Physics3DShape::createBox(Vec3(500, 5.0f, 500));
	auto physics_rbd_plan = Physics3DRigidBody::create(&rbd_plan);
	physics_rbd_plan->setFriction(20);
	auto component_plan = Physics3DComponent::create(physics_rbd_plan);
	_plan = Sprite3D::create("box.c3t");
	_plan->setTexture("plane.png");
	_plan->setScale(30.0f);
	_plan->setPosition3D(Vec3(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y, 0));
	_plan->setScaleX(500);
	_plan->setScaleY(5);
	_plan->setScaleZ(500);
	_plan->setPositionZ(0);
	_plan->setPositionY(0);
	_plan->setPositionX(0);
	_plan->setGlobalZOrder(-1);
	_plan->addComponent(component_plan);
	component_plan->syncNodeToPhysics();
	component_plan->setSyncFlag(Physics3DComponent::PhysicsSyncFlag::NONE);
	_plan->setCameraMask((unsigned short)CameraFlag::USER2);
	this->addChild(_plan, 1);

	//Creat BOX
	/**
	Physics3DRigidBodyDes rbDesxs;
	rbDesxs.disableSleep = true;
	rbDesxs.mass = 1.0f;
	rbDesxs.originalTransform.translate(camera->getPosition3D());
	rbDesxs.shape = Physics3DShape::createBox(Vec3(5, 1.0f, 5));
	rbds = Physics3DRigidBody::create(&rbDesxs);
	auto rbcs = Physics3DComponent::create(rbds);
	robox = Sprite3D::create("box.c3t");
	robox->setTexture("Gun.png");
	robox->setPosition3D(Vec3(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y, 0));
	robox->setPositionZ(0);
	robox->setPositionY(10);
	robox->setPositionX(0);
	robox->setGlobalZOrder(-1);
	robox->addComponent(rbcs);
	rbcs->syncNodeToPhysics();
	rbcs->setSyncFlag(Physics3DComponent::PhysicsSyncFlag::PHYSICS_TO_NODE);
	robox->setCameraMask((unsigned short)CameraFlag::USER2);
	this->addChild(robox);
	**/

/**
	Physics3DRigidBodyDes xrbd_cabine;
	xrbd_cabine.disableSleep = true;
	xrbd_cabine.mass = 50.0f;
	xrbd_cabine.shape = Physics3DShape::createBox(Vec3(20, 20.0f, 20));
	auto xphysics_rbd_cabine = Physics3DRigidBody::create(&xrbd_cabine);
	auto xcabine_component = Physics3DComponent::create(xphysics_rbd_cabine);
	auto xcar_cabine = Sprite3D::create("box.c3t");
	xcar_cabine->setTexture("Gun.png");
	xcar_cabine->setPosition3D(Vec3(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y, 0));
	xcar_cabine->setPositionZ(30);
	xcar_cabine->setPositionY(0);
	xcar_cabine->setPositionX(30);
	xcar_cabine->setGlobalZOrder(-1);
	xcar_cabine->addComponent(xcabine_component);
	xcabine_component->syncNodeToPhysics();
	xcabine_component->setSyncFlag(Physics3DComponent::PhysicsSyncFlag::PHYSICS_TO_NODE);
	xcar_cabine->setCameraMask((unsigned short)CameraFlag::USER2);
	this->addChild(xcar_cabine, 1);

	auto xconstraint5 = Physics3DHingeConstraint::create(xphysics_rbd_cabine, Vec3(0.0f, 0.0, 0.0f), Vec3(0.f, 1.0f, 0.f));
	//xconstraint5->setLimit(-90, 90);
	psychics_scene->getPhysics3DWorld()->addPhysics3DConstraint(xconstraint5);
	auto xconstraint6 = Physics3DHingeConstraint::create(xphysics_rbd_cabine, Vec3(0.0f, 20.0, 00.0f), Vec3(0.f, 1.0f, 0.f));
	//xconstraint5->setLimit(-90, 90);
	psychics_scene->getPhysics3DWorld()->addPhysics3DConstraint(xconstraint6);

**/
	
        //Create Car
	Physics3DRigidBodyDes rbd_cabine;
	rbd_cabine.disableSleep = true;
	rbd_cabine.mass = 500.0f;
	rbd_cabine.shape = Physics3DShape::createBox(Vec3(12, 2.0f, 6));
	physics_rbd_cabine = Physics3DRigidBody::create(&rbd_cabine);
	auto cabine_component = Physics3DComponent::create(physics_rbd_cabine);
	car_cabine = Sprite3D::create("box.c3t");
	car_cabine->setTexture("Gun.png");
	car_cabine->setPosition3D(Vec3(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y, 0));
	car_cabine->setPositionZ(0);
	car_cabine->setPositionY(10);
	car_cabine->setPositionX(0);
	car_cabine->setGlobalZOrder(-1);
	car_cabine->addComponent(cabine_component);
	cabine_component->syncNodeToPhysics();
	cabine_component->setSyncFlag(Physics3DComponent::PhysicsSyncFlag::PHYSICS_TO_NODE);
	car_cabine->setCameraMask((unsigned short)CameraFlag::USER2);
	this->addChild(car_cabine, 1);

	Physics3DRigidBodyDes rbd_wheel1;
	rbd_wheel1.disableSleep = true;
	rbd_wheel1.mass = 15.0f;
	rbd_wheel1.shape = Physics3DShape::createCylinder(3, 1);
	auto physics_rbd_wheel1 = Physics3DRigidBody::create(&rbd_wheel1);
	auto wheel1_component = Physics3DComponent::create(physics_rbd_wheel1);
	wheel1 = Sprite3D::create("box.c3t");
	//wheel1->setTexture("Gun.png");
	wheel1->setPosition3D(Vec3(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y, 0));
	wheel1->setGlobalZOrder(-1);
	wheel1->addComponent(wheel1_component);
	wheel1_component->syncNodeToPhysics();
	wheel1_component->setSyncFlag(Physics3DComponent::PhysicsSyncFlag::PHYSICS_TO_NODE);
	wheel1->setCameraMask((unsigned short)CameraFlag::USER2);
	this->addChild(wheel1, 1);

	Physics3DRigidBodyDes rbd_wheel2;
	rbd_wheel2.disableSleep = true;
	rbd_wheel2.mass = 15.0f;
	rbd_wheel2.shape = Physics3DShape::createCylinder(3, 1);
	auto physics_rbd_wheel2 = Physics3DRigidBody::create(&rbd_wheel2);
	auto wheel2_component = Physics3DComponent::create(physics_rbd_wheel2);
	wheel2 = Sprite3D::create("box.c3t");
	//wheel2->setTexture("Gun.png");
	wheel2->setPosition3D(Vec3(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y, 0));
	wheel2->setGlobalZOrder(-1);
	wheel2->addComponent(wheel2_component);
	wheel2_component->syncNodeToPhysics();
	wheel2_component->setSyncFlag(Physics3DComponent::PhysicsSyncFlag::PHYSICS_TO_NODE);
	wheel2->setCameraMask((unsigned short)CameraFlag::USER2);
	this->addChild(wheel2, 1);

	Physics3DRigidBodyDes rbd_wheel3;
	rbd_wheel3.disableSleep = true;
	rbd_wheel3.mass = 15.0f;
	rbd_wheel3.shape = Physics3DShape::createCylinder(3, 1);
	auto physics_rbd_wheel3 = Physics3DRigidBody::create(&rbd_wheel3);
	auto wheel3_component = Physics3DComponent::create(physics_rbd_wheel3);
	wheel3 = Sprite3D::create("box.c3t");
	//wheel3->setTexture("Gun.png");
	wheel3->setPosition3D(Vec3(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y, 0));
	wheel3->setGlobalZOrder(-1);
	wheel3->addComponent(wheel3_component);
	wheel3_component->syncNodeToPhysics();
	wheel3_component->setSyncFlag(Physics3DComponent::PhysicsSyncFlag::PHYSICS_TO_NODE);
	wheel3->setCameraMask((unsigned short)CameraFlag::USER2);
	this->addChild(wheel3, 1);

	Physics3DRigidBodyDes rbd_wheel4;
	rbd_wheel4.disableSleep = true;
	rbd_wheel4.mass = 15.0f;
	rbd_wheel4.shape = Physics3DShape::createCylinder(3, 1);
	auto physics_rbd_wheel4 = Physics3DRigidBody::create(&rbd_wheel4);
	auto wheel4_component = Physics3DComponent::create(physics_rbd_wheel4);
	wheel4 = Sprite3D::create("box.c3t");
	//wheel3->setTexture("Gun.png");
	wheel4->setPosition3D(Vec3(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y, 0));
	wheel4->setGlobalZOrder(-1);
	wheel4->addComponent(wheel4_component);
	wheel4_component->syncNodeToPhysics();
	wheel4_component->setSyncFlag(Physics3DComponent::PhysicsSyncFlag::PHYSICS_TO_NODE);
	wheel4->setCameraMask((unsigned short)CameraFlag::USER2);
	this->addChild(wheel4, 1);

	Physics3DRigidBodyDes rbd_wheel_bar;
	rbd_wheel_bar.disableSleep = true;
	rbd_wheel_bar.mass = 400.0f;
	rbd_wheel_bar.shape = Physics3DShape::createBox(Vec3(2, 2.0f, 6));
	physics_rbd_wheel_bar = Physics3DRigidBody::create(&rbd_wheel_bar);
	auto car_wheel_bar_component = Physics3DComponent::create(physics_rbd_wheel_bar);
	car_wheel_bar = Sprite3D::create("box.c3t");
	car_wheel_bar->setTexture("Gun.png");
	car_wheel_bar->setPosition3D(Vec3(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y, 0));
	car_wheel_bar->setPositionZ(0);
	car_wheel_bar->setPositionY(10);
	car_wheel_bar->setPositionX(0);
	car_wheel_bar->setGlobalZOrder(-1);
	car_wheel_bar->addComponent(car_wheel_bar_component);
	car_wheel_bar_component->syncNodeToPhysics();
	car_wheel_bar_component->setSyncFlag(Physics3DComponent::PhysicsSyncFlag::PHYSICS_TO_NODE);
	car_wheel_bar->setCameraMask((unsigned short)CameraFlag::USER2);
	this->addChild(car_wheel_bar, 1);

	auto constraint = Physics3DHingeConstraint::create(physics_rbd_cabine, physics_rbd_wheel1, Vec3(6.0f, 0.0, 0.0f), Vec3(0.f, -3.0f, 0.f), Vec3(0.0f, 0.0, 1.0f), Vec3(0.f, 1.0f, 0.f));
	psychics_scene->getPhysics3DWorld()->addPhysics3DConstraint(constraint);
	auto constraint2 = Physics3DHingeConstraint::create(physics_rbd_cabine, physics_rbd_wheel2, Vec3(6.0f, 0.0, 0.0f), Vec3(0.f, 3.0f, 0.f), Vec3(0.0f, 0.0, 1.0f), Vec3(0.f, 1.0f, 0.f));
	psychics_scene->getPhysics3DWorld()->addPhysics3DConstraint(constraint2);


	auto constraint5 = Physics3DHingeConstraint::create(physics_rbd_cabine, physics_rbd_wheel_bar, Vec3(-6.0f, 0.0, 0.0f), Vec3(0.f, 0.0f, 0.f), Vec3(0.0f, 1.0, 0.0f), Vec3(0.f, 1.0f, 0.f));
	//constraint5->setLimit(-10, 10, 0.9f, 0.01f, 0.0f);
	psychics_scene->getPhysics3DWorld()->addPhysics3DConstraint(constraint5);


	auto constraint3 = Physics3DHingeConstraint::create(physics_rbd_wheel_bar, physics_rbd_wheel3, Vec3(-3.0f, 0.0, 0.0f), Vec3(0.f, -3.0f, 0.f), Vec3(0.0f, 0.0, 1.0f), Vec3(0.f, 1.0f, 0.f));
	psychics_scene->getPhysics3DWorld()->addPhysics3DConstraint(constraint3);
	auto constraint4 = Physics3DHingeConstraint::create(physics_rbd_wheel_bar, physics_rbd_wheel4, Vec3(-3.0f, 0.0, 0.0f), Vec3(0.f, 3.0f, 0.f), Vec3(0.0f, 0.0, 1.0f), Vec3(0.f, 1.0f, 0.f));
	psychics_scene->getPhysics3DWorld()->addPhysics3DConstraint(constraint4);

	auto listener = EventListenerTouchAllAtOnce::create();
	listener->onTouchesBegan = CC_CALLBACK_2(HelloWorld::onTouchesBegan, this);
	listener->onTouchesMoved = CC_CALLBACK_2(HelloWorld::onTouchesMoved, this);
	listener->onTouchesEnded = CC_CALLBACK_2(HelloWorld::onTouchesEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);


	psychics_scene->setPhysics3DDebugCamera(camera); 
	schedule(schedule_selector(HelloWorld::myupdate), .005);
	this->scheduleUpdate();
	
    return true;
}

void HelloWorld::myupdate(float dt){
	camera->setPosition3D(car_cabine->getPosition3D() + Vec3(100.0f * sinf(_angle), 50.0f, 100.0f * cosf(_angle)));
	camera->lookAt(car_cabine->getPosition3D(), Vec3(0.0f, 1.0f, 0.0f));

	if (_hud->movingleft == true){
	
		physics_rbd_wheel_bar->setAngularVelocity(Vec3(0.0, 1.0, 0.0));
		physics_rbd_wheel_bar->setAngularFactor(Vec3(0.0, 0.0, 0.0));
	}
	if (_hud->movingright == true){
		physics_rbd_wheel_bar->setAngularVelocity(Vec3(0.0, -1.0, 0.0));
		physics_rbd_wheel_bar->setAngularFactor(Vec3(0.0, 0.0, 0.0));
	}
	//Direction Velocity
	Vec3 mynearP((int)wheel1->getPosition3D().x, 0.0, (int)wheel1->getPosition3D().z), myfarP((int)wheel3->getPosition3D().x, 0.0, (int)wheel3->getPosition3D().z);
	Vec3 mynearP2((int)wheel2->getPosition3D().x, 0.0, (int)wheel2->getPosition3D().z), myfarP2((int)wheel4->getPosition3D().x, 0.0, (int)wheel4->getPosition3D().z);
	Vec3 mydir(myfarP - mynearP);
	Vec3 mydir2(myfarP2 - mynearP2);

	if (_hud->movingup == true){

		Vec3 mylinearVel = (mydir+mydir2)/2;
		mylinearVel.normalize();
		mylinearVel *= 25.0f;
		mylinearVel.y = 0;
		physics_rbd_wheel_bar->setLinearVelocity(mylinearVel);
		physics_rbd_wheel_bar->setAngularVelocity(Vec3(0.0, 0.0, 0.0));
		physics_rbd_wheel_bar->setAngularFactor(Vec3(0.0, 0.0, 0.0));

	}

}

void HelloWorld::onTouchesBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event) {
	shootBox = true;
	event->stopPropagation();
}

void HelloWorld::onTouchesMoved(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event) {
	if (touches.size() && camera)
	{
		auto touch = touches[0];
		auto delta = touch->getDelta();

		_angle -= CC_DEGREES_TO_RADIANS(delta.x);
		camera->setPosition3D(car_cabine->getPosition3D() + Vec3(100.0f * sinf(_angle), 50.0f, 100.0f * cosf(_angle)));
		camera->lookAt(car_cabine->getPosition3D(), Vec3(0.0f, 1.0f, 0.0f));
	
		if (delta.lengthSquared() > 16)
		{
			shootBox = false;
		}
		event->stopPropagation();

		////Moving obj in camera direction
		/**
		auto location = touches[0]->getLocationInView();
		Vec3 nearP((int)location.x, (int)location.y, -1.0f), farP((int)location.x, (int)location.y, 1.0f);
		nearP = camera->unproject(nearP);
		farP = camera->unproject(farP);
		Vec3 dir(farP - nearP);
		vectorcameraforbox = (camera->getPosition3D() + dir) -camera->getPosition3D();
		**/
	}
}

void HelloWorld::onTouchesEnded(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event) {

	if (!shootBox) return;
	if (!touches.empty())
	{
		auto location = touches[0]->getLocationInView();

		Vec3 nearP(location.x, location.y, -1.0f), farP(location.x, location.y, 1.0f);
		nearP = camera->unproject(nearP);
		farP = camera->unproject(farP);
		Vec3 dir(farP - nearP);
		shootBoxfunc(camera->getPosition3D() + dir * 10.0f);
		event->stopPropagation();
	}
}

//Throwing Object
void HelloWorld::shootBoxfunc(const cocos2d::Vec3 &des)
{
	Vec3 linearVel = des - camera->getPosition3D();
	linearVel.normalize();
	linearVel *= 100.0f;

	Physics3DRigidBodyDes rbDes;
	rbDes.originalTransform.translate(camera->getPosition3D());
	rbDes.mass = 1.f;
	rbDes.shape = Physics3DShape::createBox(Vec3(0.5f, 0.5f, 0.5f));
	auto sprite = PhysicsSprite3D::create("box.c3t", &rbDes);
	sprite->setTexture("plane.png");

	auto rigidBody = static_cast<Physics3DRigidBody*>(sprite->getPhysicsObj());
	rigidBody->setLinearFactor(Vec3::ONE);
	rigidBody->setLinearVelocity(linearVel);
	rigidBody->setAngularVelocity(Vec3::ZERO);
	rigidBody->setCcdMotionThreshold(0.5f);
	rigidBody->setCcdSweptSphereRadius(0.4f);

	sprite->setPosition3D(camera->getPosition3D());
	sprite->setScale(0.5f);
	sprite->syncNodeToPhysics();
	sprite->setSyncFlag(Physics3DComponent::PhysicsSyncFlag::PHYSICS_TO_NODE); 
	sprite->setCameraMask((unsigned short)CameraFlag::USER2);
	this->addChild(sprite,1);
}


//Creat Tortoise
/**
Physics3DRigidBodyDes rbDesxs;
rbDesxs.mass = 1.0f;
rbDesxs.shape = Physics3DShape::createBox(Vec3(5, 1.0f, 5));
rbds = Physics3DRigidBody::create(&rbDesxs);
//rbds->setKinematic(true); /// nie dziala nie wiadomo czemu
auto rbcs = Physics3DComponent::create(rbds);
robox = Sprite3D::create("T-90.obj");
robox->setTexture("Gun.png");
//robox->setScale(0.01);
robox->setPosition3D(Vec3(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y, 0));
//robox->setScaleX(5);
//robox->setScaleY(1);
//robox->setScaleZ(5);
robox->setPositionZ(0);
robox->setPositionY(10);
robox->setPositionX(0);
robox->setGlobalZOrder(-1);
robox->addComponent(rbcs);
rbcs->syncNodeToPhysics();
rbcs->setSyncFlag(Physics3DComponent::PhysicsSyncFlag::PHYSICS_TO_NODE);
robox->setCameraMask((unsigned short)CameraFlag::USER2);
this->addChild(robox);
**/

//Create Terrian
/**
Terrain::DetailMap r("dirt.jpg");
Terrain::TerrainData data("heightmap16.jpg", "alphamap.png", r, r, r, r);
auto _terrain = Terrain::create(data, Terrain::CrackFixedType::SKIRT);
_terrain->setScale(5.0f);
_terrain->setPosition3D(Vec3(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y, 0));
_terrain->setPositionZ(0);
_terrain->setPositionY(0);
_terrain->setPositionX(0);
_terrain->setRotation3D(Vec3(0, 0, 0));
_terrain->setGlobalZOrder(-1);
_terrain->setLODDistance(3.2f, 6.4f, 9.6f);
_terrain->setMaxDetailMapAmount(4);
_terrain->setDrawWire(false);
_terrain->setCameraMask((unsigned short)CameraFlag::USER2);
addChild(_terrain);

**/

//create Terrain PHYSICS
/**
Terrain::DetailMap r("dirt.jpg");
Terrain::TerrainData data("heightmap16.jpg", "alphamap.png", r, r, r, r,Size(32, 32), 20.0f, 1.0f);
_terrain = Terrain::create(data, Terrain::CrackFixedType::SKIRT);
_terrain->setPosition3D(Vec3(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y, 0));
_terrain->setPositionZ(0);
_terrain->setPositionY(0);
_terrain->setPositionX(0);
_terrain->setLODDistance(64, 128, 192);
_terrain->setDrawWire(false);
_terrain->setMaxDetailMapAmount(2);
_terrain->setSkirtHeightRatio(2);
_terrain->setCameraMask((unsigned short)CameraFlag::USER2);
//create terrain
std::vector<float> heidata = _terrain->getHeightData();
auto size = _terrain->getTerrainSize();
Physics3DColliderDes colliderDes;
colliderDes.shape = Physics3DShape::createHeightfield(size.width, size.height, &heidata[0], 1.0f, _terrain->getMinHeight(), _terrain->getMaxHeight(), true, false, true);
auto collider = Physics3DCollider::create(&colliderDes);
auto component = Physics3DComponent::create(collider);
_terrain->addComponent(component);
component->syncNodeToPhysics();
component->setSyncFlag(Physics3DComponent::PhysicsSyncFlag::NONE);
this->addChild(_terrain);
**/

//create kinematic thing
/**
Physics3DRigidBodyDes xrbDes;
xrbDes.mass = 1.0f; //kinematic objects. zero mass so that it can not be affected by other dynamic objects
xrbDes.shape = Physics3DShape::createBox(Vec3(2.0f, 2.0f, 2.0f));
auto sprite = PhysicsSprite3D::create("box.c3t", &xrbDes);
sprite->setTexture("plane.png");
sprite->setCameraMask((unsigned short)CameraFlag::USER2);
auto rigidBody = static_cast<Physics3DRigidBody*>(sprite->getPhysicsObj());
rigidBody->setKinematic(true);
sprite->setScale(2.0f);
sprite->setPosition3D(Vec3(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y, 0));
sprite->setPositionZ(0);
sprite->setPositionY(10);
sprite->setPositionX(10);
this->addChild(sprite);
**/

/**
///CREATE MODEL orc
Physics3DRigidBodyDes xrbDesxs;
xrbDesxs.disableSleep = true;
xrbDesxs.mass = 1.0f;
xrbDesxs.shape = Physics3DShape::createBox(Vec3(5, 1.0f, 5));
auto xrbds = Physics3DRigidBody::create(&xrbDesxs);
auto xrbcs = Physics3DComponent::create(xrbds);
auto sprite = Sprite3D::create("orc.c3t");
this->addChild(sprite);
sprite->setPosition3D(Vec3(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y, 0));
sprite->setScale(1);
sprite->setPositionZ(20);
sprite->setPositionY(10);
sprite->setPositionX(20);
sprite->addComponent(xrbcs);
xrbcs->syncNodeToPhysics();
xrbcs->setSyncFlag(Physics3DComponent::PhysicsSyncFlag::PHYSICS_TO_NODE);
sprite->setCameraMask((unsigned short)CameraFlag::USER2);

Physics3DRigidBodyDes xrbDesxs2;
xrbDesxs2.disableSleep = true;
xrbDesxs2.shape = Physics3DShape::createBox(Vec3(5, 1.0f, 5));
auto xrbds2 = Physics3DRigidBody::create(&xrbDesxs2);
auto xrbcs2 = Physics3DComponent::create(xrbds2);
auto sp = Sprite3D::create("axe.c3b");
sp->setScale(1);
sprite->getAttachNode("Bip001 R Hand")->addChild(sp);
sp->addComponent(xrbcs2);
xrbcs2->syncNodeToPhysics();
sp->setCameraMask((unsigned short)CameraFlag::USER2);

**/

//CONSTRAINT BODY
/**
auto visibleSize = Director::getInstance()->getVisibleSize();
auto origin = Director::getInstance()->getVisibleOrigin();

Physics3DRigidBodyDes rigidbodydes;
rigidbodydes.disableSleep = true;
rigidbodydes.mass = 1.0f;
rigidbodydes.shape = Physics3DShape::createBox(Vec3(2, 2.0f, 2));
//rigidbodydes.shape = Physics3DShape::createCylinder(5, 5);
auto rigidbody = Physics3DRigidBody::create(&rigidbodydes);
auto component = Physics3DComponent::create(rigidbody);
auto mybox = Sprite3D::create("box.c3t");
mybox->setTexture("Gun.png");
mybox->setPosition3D(Vec3(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y, 0));
mybox->setPositionZ(10);
mybox->setPositionY(10);
mybox->setPositionX(10);
mybox->setGlobalZOrder(-1);
mybox->addComponent(component);
component->syncNodeToPhysics();
component->setSyncFlag(Physics3DComponent::PhysicsSyncFlag::PHYSICS_TO_NODE);
mybox->setCameraMask((unsigned short)CameraFlag::USER2);
this->addChild(mybox);


Physics3DRigidBodyDes rigidbodydes2;
rigidbodydes2.disableSleep = true;
rigidbodydes2.mass = 1.0f;
//rigidbodydes2.shape = Physics3DShape::createBox(Vec3(5, 1.0f, 5));
rigidbodydes2.shape = Physics3DShape::createCylinder(5, 1);
auto rigidbody2 = Physics3DRigidBody::create(&rigidbodydes2);
auto component2 = Physics3DComponent::create(rigidbody2);
auto mybox2 = Sprite3D::create("box.c3t");
mybox2->setTexture("Gun.png");
mybox2->setPosition3D(Vec3(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y, 0));
mybox2->setPositionZ(5);
mybox2->setPositionY(10);
mybox2->setPositionX(10);
mybox2->setGlobalZOrder(-1);
mybox2->addComponent(component2);
component2->syncNodeToPhysics();
component2->setSyncFlag(Physics3DComponent::PhysicsSyncFlag::PHYSICS_TO_NODE);
mybox2->setCameraMask((unsigned short)CameraFlag::USER2);
this->addChild(mybox2);

Physics3DRigidBodyDes rigidbodydes3;
rigidbodydes3.disableSleep = true;
rigidbodydes3.mass = 1.0f;
//rigidbodydes2.shape = Physics3DShape::createBox(Vec3(5, 1.0f, 5));
rigidbodydes3.shape = Physics3DShape::createCylinder(5, 1);
auto rigidbody3 = Physics3DRigidBody::create(&rigidbodydes3);
auto component3 = Physics3DComponent::create(rigidbody3);
auto mybox3 = Sprite3D::create("orc.c3t");
mybox3->setTexture("Gun.png");
mybox3->setPosition3D(Vec3(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y, 0));
mybox3->setPositionZ(15);
mybox3->setPositionY(10);
mybox3->setPositionX(10);
mybox3->setGlobalZOrder(-1);
mybox3->addComponent(component3);
component3->syncNodeToPhysics();
component3->setSyncFlag(Physics3DComponent::PhysicsSyncFlag::PHYSICS_TO_NODE);
mybox2->setCameraMask((unsigned short)CameraFlag::USER2);
this->addChild(mybox3);

auto constraint = Physics3DHingeConstraint::create(rigidbody, rigidbody2, Vec3(0.0f, 2.0, 0.0f), Vec3(0.f, 0.0f, 0.f), Vec3(0.0f, 1.0, 0.0f), Vec3(0.f, 1.0f, 0.f));
pscene->getPhysics3DWorld()->addPhysics3DConstraint(constraint);

auto constraint2 = Physics3DHingeConstraint::create(rigidbody, rigidbody3, Vec3(0.0f, 2.0, 0.0f), Vec3(0.f, 4.0f, 0.f), Vec3(0.0f, 1.0, 0.0f), Vec3(0.f, 1.0f, 0.f));
pscene->getPhysics3DWorld()->addPhysics3DConstraint(constraint2);
**/
