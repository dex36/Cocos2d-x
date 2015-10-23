#include "AppMacros.h"
#include "HelloWorldScene.h"
#include "GLES-Render.h"
#include <Box2D/Box2D.h>
USING_NS_CC;

HelloWorldHud *HelloWorldScene::_hud = NULL;

Scene* HelloWorldScene::scene()
{
	//Create Scene
	auto scene = Scene::create();
	auto layer = HelloWorldScene::create();
	scene->addChild(layer);

	//Create HUB
	auto hud = HelloWorldHud::create();
	_hud = hud;
	scene->addChild(_hud);

	return scene;
}

bool  HelloWorldScene::init()
{
	if (!Layer::init()){
		return false;
	}
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	//Create Touch Listener
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(HelloWorldScene::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(HelloWorldScene::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(HelloWorldScene::onTouchEnded, this);
	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	//Initialize Physics
	this->initPhysics();

	//Initialize TileMap
	this->_tileMap = TMXTiledMap::create("worldmap.tmx");
	_tileMap->setPosition(0, 0);
	this->_background = _tileMap->layerNamed("ground");
	this->addChild(_tileMap);

	//Initialize TileMap Equipment
	this->_blockage = _tileMap->layerNamed("meta01");
	_blockage->setVisible(false);
	this->_collect = _tileMap->layerNamed("meta02");
	_collect->setVisible(false);
	this->_foreground = _tileMap->getLayer("collec");
	withTileMap = _tileMap->getMapSize().width * _tileMap->getTileSize().width; //size of tile map

	//Create Range of TileMap
	CCSize s = CCDirector::sharedDirector()->getVisibleSize();
	b2BodyDef groundBodyDef;
	groundBodyDef.type = b2_staticBody;
	groundBodyDef.position.Set(0, 0);
	b2Body* groundBody = world->CreateBody(&groundBodyDef);
	b2EdgeShape groundBox;
	groundBox.Set(b2Vec2(0, 0), b2Vec2(withTileMap / PTM_RATIO, 0));
	groundBody->CreateFixture(&groundBox, 0);
	groundBox.Set(b2Vec2(0, s.height / PTM_RATIO), b2Vec2(s.width / PTM_RATIO, s.height / PTM_RATIO));
	groundBody->CreateFixture(&groundBox, 0);
	groundBox.Set(b2Vec2(0, s.height / PTM_RATIO), b2Vec2(0, 0));
	groundBody->CreateFixture(&groundBox, 0);
	groundBox.Set(b2Vec2(withTileMap / PTM_RATIO, s.height / PTM_RATIO), b2Vec2(withTileMap / PTM_RATIO, 0));
	groundBody->CreateFixture(&groundBox, 0);

	//Create Player
	TMXObjectGroup *objects = _tileMap->getObjectGroup("player");
	auto playerShowUpPoint = objects->getObject("spawn");
	{
		int x = playerShowUpPoint["x"].asInt();
		int y = playerShowUpPoint["y"].asInt();
		hero = Sprite::create("monster.png", Rect(14, 0, 16, 15));
		hero->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
		addChild(hero);
		BodyDef.type = b2_dynamicBody;
		BodyDef.position.Set(50/ PTM_RATIO, 50 / PTM_RATIO);
		BodyDef.userData = hero;
		BodyDef.fixedRotation = true;
		herobody = world->CreateBody(&BodyDef);
		b2FixtureDef fixtureDef;
		b2CircleShape circleShape;
		circleShape.m_p.Set(0, 0); //position, relative to body position
		circleShape.m_radius = 0.2;
		fixtureDef.shape = &circleShape;
		fixtureDef.density = 1.0f;
		herobody->CreateFixture(&fixtureDef);

		//circleShape.m_radius = 0.1;
		//circleShape.m_p.Set(0, -0.2);
		//fixtureDef.isSensor = true;
		//b2Fixture* footSensorFixture = herobody->CreateFixture(&fixtureDef);
		//footSensorFixture->SetUserData((int*)3);
	}
	
	//Create Sliced Box
	{
		drawes = DrawNode::create();
		Vec2 vertices3[] = { Vec2(-20, -20), Vec2(-20, 20), Vec2(20, 20), Vec2(20, -20) };
		drawes->drawPolygon(vertices3, 4, Color4F(1, 1, 0, 1), 0.5, Color4F(1, 1, 0, 1));
		addChild(drawes);
		
		b2BodyDef bodyDef;
		bodyDef.type = b2_dynamicBody;
		bodyDef.position.Set(100 / PTM_RATIO, 100 / PTM_RATIO);
		bodyDef.userData = drawes;
		b2PolygonShape dynamicBox;
		dynamicBox.SetAsBox(20 / PTM_RATIO, 20 / PTM_RATIO);
		bodybox = world->CreateBody(&bodyDef);
		b2FixtureDef fixtureDef;
		fixtureDef.shape = &dynamicBox;
		fixtureDef.density = 1.0f;
		bodybox->CreateFixture(&fixtureDef);
	}

	//Create Bullet
	{
		bulletdraw = DrawNode::create();
		//Vec2 vertices3[] = { Vec2(-2, -2), Vec2(-2, 2), Vec2(2, 2), Vec2(2, -2) };
		//bulletdraw->drawPolygon(vertices3, 4, Color4F(1, 1, 0, 1), 0.5, Color4F(1, 1, 0, 1));
		bulletdraw->drawSolidCircle(Vec2(0, 0), 2, 360, 10, Color4F(1, 1, 0, 1));
		addChild(bulletdraw);

		b2BodyDef bDef;
		bDef.type = b2_dynamicBody;
		bDef.position.Set(hero->getPosition().x, hero->getPosition().y + 0.3);
		bDef.userData = bulletdraw;
		bullet = world->CreateBody(&bDef);
		b2FixtureDef fixturebDef;
		b2CircleShape circlebShape;
		circlebShape.m_radius = 0.05;
		fixturebDef.shape = &circlebShape;
		bullet->CreateFixture(&fixturebDef);
	}

	//Prepare TileMap
	this->prepareLayers();

	//Update Game
	this->scheduleUpdate();
	return true;
}

void HelloWorldScene::update(float dt)
{
	//A little CHEAT ;(
	herobody->SetLinearVelocity(b2Vec2(herobody->GetLinearVelocity().x + 0.0001, herobody->GetLinearVelocity().y));

	//Camera Follow
	this->setViewPointCenter(Point(hero->getPosition().x, hero->getPosition().y));
	this->setPlayerPosition(Point(hero->getPosition().x, hero->getPosition().y));

	//Player Animation
	Vector<SpriteFrame*> animFrames2;
	animFrames2.reserve(3);
	animFrames2.pushBack(SpriteFrame::create("monster.png", Rect(0, 15, 16, 15)));
	animFrames2.pushBack(SpriteFrame::create("monster.png", Rect(28, 15, 16, 15)));
	animFrames2.pushBack(SpriteFrame::create("monster.png", Rect(14, 15, 16, 15)));
	Animation* animation2 = Animation::createWithSpriteFrames(animFrames2, 0.1f);
	Animate* animate2 = Animate::create(animation2);

	Vector<SpriteFrame*> animFrames3;
	animFrames3.reserve(3);
	animFrames3.pushBack(SpriteFrame::create("monster.png", Rect(0, 30, 16, 15)));
	animFrames3.pushBack(SpriteFrame::create("monster.png", Rect(28, 30, 16, 15)));
	animFrames3.pushBack(SpriteFrame::create("monster.png", Rect(14, 30, 16, 15)));
	Animation* animation3 = Animation::createWithSpriteFrames(animFrames3, 0.1f);
	Animate* animate3 = Animate::create(animation3);

	/**
	if (_hud->movingup == true && herobody->GetLinearVelocity().y == 0){
		//herobody->ApplyLinearImpulse(b2Vec2(herobody->GetLinearVelocity().x, 0.1), herobody->GetPosition(), true);	
		herobody->SetLinearVelocity(b2Vec2(herobody->GetLinearVelocity().x, 9));
		///herobody->ApplyLinearImpulse(b2Vec2(0, 0.9), herobody->GetPosition(), true);
		_hud->movingup = false;
	}
	**/

	//Moving Player Left/Right
	if (_hud->movingleft == true){
		auto adam = hero->numberOfRunningActions();
		if (hero->numberOfRunningActions() == 32){
			hero->runAction(animate2);
		}
		herobody->SetLinearVelocity(b2Vec2(-1.5, herobody->GetLinearVelocity().y));
	}
	else
	{
		hero->stopAction(animate2);
	}

	if (_hud->movingright == true){
		auto adam = hero->numberOfRunningActions();
		if (hero->numberOfRunningActions() == 32){
			hero->runAction(animate3);
		}
		herobody->SetLinearVelocity(b2Vec2(1.5, herobody->GetLinearVelocity().y));
	}
	else
	{
		hero->stopAction(animate3);
	}

	//Fixing Player Body
	for (b2Body* b = world->GetBodyList(); b; b = b->GetNext())
	{
		if (b->GetUserData() != NULL && b->GetUserData() == hero) {
			hero = (CCSprite*)b->GetUserData();
			hero->setPosition(CCPointMake(b->GetPosition().x * PTM_RATIO, b->GetPosition().y * PTM_RATIO));
			hero->setRotation(-1 * CC_RADIANS_TO_DEGREES(b->GetAngle()));
	}
		if (b->GetUserData() != NULL && b->GetUserData() == drawes){
			drawes = (DrawNode*)b->GetUserData();
			drawes->setPosition(CCPointMake(b->GetPosition().x * PTM_RATIO, b->GetPosition().y * PTM_RATIO));
			drawes->setRotation(-1 * CC_RADIANS_TO_DEGREES(b->GetAngle()));
		}	

		if (b->GetUserData() != NULL && b->GetUserData() == bulletdraw){
			bulletdraw = (DrawNode*)b->GetUserData();
			bulletdraw->setPosition(CCPointMake(b->GetPosition().x * PTM_RATIO, b->GetPosition().y * PTM_RATIO));
			bulletdraw->setRotation(-1 * CC_RADIANS_TO_DEGREES(b->GetAngle()));
		}
		for (int i = 0; i < table_slice_draw; i++){
			if (b->GetUserData() != NULL && b->GetUserData() == slicedraw[i]){
				slicedraw[i] = (DrawNode*)b->GetUserData();
				slicedraw[i]->setPosition(CCPointMake(b->GetPosition().x * PTM_RATIO, b->GetPosition().y * PTM_RATIO));
				slicedraw[i]->setRotation(-1 * CC_RADIANS_TO_DEGREES(b->GetAngle()));
			}
		}

	}
	//Bullet destory box
	//if (bullet_destroy_body == true){
	//	world->DestroyBody(bodybox);
	//}

	//Update Bullet
	if (bullet_working == true){
		bullet->SetTransform(b2Vec2(herobody->GetPosition().x, herobody->GetPosition().y + 0.3), 0);
	}

	//Update World
	int velocityIterations = 8;
	int positionIterations = 1;
	world->Step(dt, velocityIterations, positionIterations);
	
}

//Initialize World
void HelloWorldScene::initPhysics()
{
	b2Vec2 gravity;
	gravity.Set(0.0f, -9.81f);
	this->world = new b2World(gravity);
	this->debugDraw = new GLESDebugDraw(PTM_RATIO);
	this->world->SetDebugDraw(debugDraw);
	world->SetContactListener(this);
	uint32 flags = 0;
	flags += b2Draw::e_shapeBit;
	this->debugDraw->SetFlags(flags);
	//    this->addChild(B2DebugDrawLayer::create(this->world, PTM_RATIO), 9999);
}

//Draw Function
void HelloWorldScene::draw(cocos2d::Renderer* renderer, const cocos2d::Mat4& transform, uint32_t transformUpdated) {
	Layer::draw(renderer, transform, transformUpdated);
	Director* director = Director::getInstance();
	GL::enableVertexAttribs(cocos2d::GL::VERTEX_ATTRIB_FLAG_POSITION);
	director->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
	world->DrawDebugData();
	director->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);

	if (m_bIsDraw) {
		ccDrawLine(m_startPt, m_endPoint);
	}
}

//Prepare Tile Map
void HelloWorldScene::prepareLayers()
{
	this->createFixtures(_background);
}
void HelloWorldScene::createFixtures(TMXLayer* layer)
{
	Size layerSize = layer->getLayerSize();
	for (int y = 0; y < layerSize.height; y++)
	{
		for (int x = 0; x < layerSize.width; x++)
		{
			Sprite* tileSprite = layer->tileAt(ccp(x, y));
			if (tileSprite)
				createRectangularFixture(layer, x, y, 0.95f, 0.95f);
		}
	}
}

void HelloWorldScene::createRectangularFixture(TMXLayer* layer, int x, int y, float width, float height)
{
	Point p = layer->positionAt(ccp(x, y));
	auto tileSize = _tileMap->getTileSize();
	const float pixelsPerMeter = 32.0f;

	//Create the body
	b2BodyDef bodyDef;
	bodyDef.type = b2_staticBody; 
	bodyDef.position.Set((p.x + (tileSize.width / 4.0f)) / pixelsPerMeter,(p.y + (tileSize.height / 4.0f)) / pixelsPerMeter);
	b2Body *bodytiles = world->CreateBody(&bodyDef);

	//Define the shape
	b2PolygonShape shape;
	shape.SetAsBox((tileSize.width / pixelsPerMeter) * 0.25f * width,(tileSize.width / pixelsPerMeter) * 0.25f * height);

	//Create the fixture
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.6f;
	fixtureDef.restitution = 0.0f;
	//fixtureDef.filter.categoryBits = kFilterCategoryLevel;
	fixtureDef.filter.maskBits = 0xffff;
	bodytiles->CreateFixture(&fixtureDef);
}

//Delete World
HelloWorldScene::~HelloWorldScene() {
	delete this->debugDraw;
	this->debugDraw = NULL;

	delete this->world;
	this->world = NULL;
}

//Contact Listener
void HelloWorldScene::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse){
	b2Body *bodyA = contact->GetFixtureA()->GetBody();
	b2Body *bodyB = contact->GetFixtureB()->GetBody();

	if (_hud->movingup == true){
	if ((bodyA == herobody && bodyB != bodybox) || (bodyB == herobody && bodyA != bodybox)) {
			herobody->SetLinearVelocity(b2Vec2(herobody->GetLinearVelocity().x, 7));
			_hud->movingup = false;
		}
	}

	//if ((bodyA == bullet && bodyB == bodybox) || (bodyB == bullet && bodyA == bodybox)) {
		//bullet_destroy_body = true;
	//}
}

//Touch Listener
bool HelloWorldScene::onTouchBegan(Touch *touch, Event *unused_event)
{
	bullet_working = true;
	m_bSliceObject = false;
	laserSegment = new Segment;
	m_bIsRayCast = false;
	m_bIsDraw = false;
	m_bIsDrawEnter = false;
	m_startPt = CCDirector::sharedDirector()->convertToGL(touch->getLocationInView() - fixscreen);
	laserSegment->p1 = b2Vec2(m_startPt.x / PTM_RATIO, m_startPt.y / PTM_RATIO);
	return true;
}

//Touch Listener
void HelloWorldScene::onTouchMoved(Touch *touch, Event *unused_event)
{
	m_endPoint = CCDirector::sharedDirector()->convertToGL(touch->getLocationInView() - fixscreen);
	m_bIsDraw = true;
}

//Touch Listener
void HelloWorldScene::onTouchEnded(Touch *touch, Event *unused_event)
{
	m_endPoint = CCDirector::sharedDirector()->convertToGL(touch->getLocationInView() - fixscreen);
	laserSegment->p2 = b2Vec2(m_endPoint.x / PTM_RATIO, m_endPoint.y / PTM_RATIO);
	m_bIsDraw = false;
	m_bIsRayCast = true;
	if (m_bIsRayCast && NULL != laserSegment && (laserSegment->p2 - laserSegment->p1).LengthSquared() > 0) {
		entryPoint.clear();
		affectedByLaser.clear();
		world->RayCast(this, laserSegment->p1, laserSegment->p2);
		world->RayCast(this, laserSegment->p2, laserSegment->p1);
		delete laserSegment;
		laserSegment = NULL;
	}
	//THROW BALL
	bullet->SetLinearVelocity(b2Vec2((m_startPt.x - herobody->GetPosition().x*PTM_RATIO) / PTM_RATIO, m_startPt.y / PTM_RATIO));
	//b->ApplyLinearImpulse(b2Vec2((m_startPt.x - herobody->GetPosition().x*PTM_RATIO) / PTM_RATIO, m_startPt.y / PTM_RATIO), b->GetWorldCenter(), true);
	bullet_working = false;
	//b->SetTransform(b2Vec2(herobody->GetPosition().x + 0.4, herobody->GetPosition().y), 0);
}

//
void HelloWorldScene::setPlayerPosition(Point position)
{
	Point tileCoord = this->tileCoordForPosition(position);
	int tileGid = this->_blockage->tileGIDAt(tileCoord);
	if (tileGid) {
		Value props = this->_tileMap->propertiesForGID(tileGid);
		ValueMap map = props.asValueMap();
		int type_int = map.at("collision").asInt();
		if (1 == type_int)
		{
			return;
		}
	}
	int tileGid2 = this->_collect->tileGIDAt(tileCoord);
	if (tileGid2) {
		Value props = this->_tileMap->propertiesForGID(tileGid2);
		ValueMap map = props.asValueMap();
		int type_int = map.at("collect").asInt();
		if (2 == type_int)
		{
			//_numCollected++;
			//_hud->numCollectedChanged(_numCollected);
			_collect->removeTileAt(tileCoord);
			_foreground->removeTileAt(tileCoord);
		}
	}
	hero->runAction(MoveTo::create(0.5, position));
}

//Camera Follow
void HelloWorldScene::setViewPointCenter(Point position) {
	auto winSize = Director::getInstance()->getWinSize();
	int x = MAX(position.x, winSize.width / 2);
	int y = MAX(position.y, winSize.height / 2);
	x = MIN(x, (_tileMap->getMapSize().width * _tileMap->getTileSize().width) - winSize.width / 2);
	y = MIN(y, (_tileMap->getMapSize().height * _tileMap->getTileSize().height) - winSize.height / 2);
	auto actualPosition = Point(x, y);
	auto centerOfView = Point(winSize.width / 2, winSize.height / 2);
	auto viewPoint = centerOfView - actualPosition;
	fixscreen = viewPoint;
	this->setPosition(viewPoint);
}

Point HelloWorldScene::tileCoordForPosition(Point position)
{
	int x = CC_CONTENT_SCALE_FACTOR() * position.x / this->_tileMap->getTileSize().width;
	int y = ((this->_tileMap->getMapSize().height * this->_tileMap->getTileSize().height) - CC_CONTENT_SCALE_FACTOR()*position.y) / this->_tileMap->getTileSize().height;
	return Point(x, y);
}

//Sliced Box
float32 HelloWorldScene::ReportFixture(b2Fixture *fixture, const b2Vec2 &point, const b2Vec2 &normal, float32 fraction)
{
	b2Body *affectedBody = fixture->GetBody();
	b2PolygonShape* affectedPolygon = (b2PolygonShape*)fixture->GetShape();

	if (affectedBody == herobody){
		return 1;
	}
	int fixtureIndex = -1;
	for (int index = 0; index < affectedByLaser.size(); ++index) {
		if (affectedBody == affectedByLaser[index]) {
			fixtureIndex = index;
			break;
		}
	}
	if (-1 == fixtureIndex) {
		affectedByLaser.push_back(affectedBody);
		entryPoint.push_back(point);
	}
	else{

		b2Vec2 rayCenter = b2Vec2((point.x + entryPoint[fixtureIndex].x) / 2.0f,(point.y + entryPoint[fixtureIndex].y) / 2.0f);
		float rayAngle = b2Atan2(entryPoint[fixtureIndex].y - point.y,entryPoint[fixtureIndex].x - point.x);
		std::vector<b2Vec2> polyVertices;
		b2Transform xf = affectedBody->GetTransform();
		int32 vertexCount = affectedPolygon->m_count; /// FIXED
		if (vertexCount > 8){
			return 1;
		}
		b2Assert(vertexCount <= b2_maxPolygonVertices); // b2_max from 8 to 20 b2_maxPolygonVertices
		for (int32 i = 0; i < vertexCount; ++i)
		{
			b2Vec2 vertice = b2Mul(xf, affectedPolygon->m_vertices[i]);
			polyVertices.push_back(vertice);
		}
		std::vector<b2Vec2> newPolyVertices1;
		std::vector<b2Vec2> newPolyVertices2;
		int currentPoly = 0;
		bool cutPlaced1 = false;
		bool cutPlaced2 = false;
		for (int i = 0; i<polyVertices.size(); i++) {
			// b2Vec2 worldPoint2 = affectedBody->GetWorldPoint(polyVertices[i]);
			b2Vec2 worldPoint = polyVertices[i];
			float cutAngle = b2Atan2(worldPoint.y - rayCenter.y, worldPoint.x - rayCenter.x) - rayAngle;
			if (cutAngle < M_PI * -1) {
				cutAngle += 2 * M_PI;
			}
			if (cutAngle > 0 && cutAngle <= M_PI) {
				if (currentPoly == 2) {
					cutPlaced1 = true;
					newPolyVertices1.push_back(point);
					newPolyVertices1.push_back(entryPoint[fixtureIndex]);
				}
				newPolyVertices1.push_back(worldPoint);
				currentPoly = 1;
			}
			else {
				if (currentPoly == 1) {
					cutPlaced2 = true;
					newPolyVertices2.push_back(entryPoint[fixtureIndex]);
					newPolyVertices2.push_back(point);
				}
				newPolyVertices2.push_back(worldPoint);
				currentPoly = 2;
			}
		}
		if (!cutPlaced1) {
			newPolyVertices1.push_back(point);
			newPolyVertices1.push_back(entryPoint[fixtureIndex]);
		}
		if (!cutPlaced2) {
			newPolyVertices2.push_back(entryPoint[fixtureIndex]);
			newPolyVertices2.push_back(point);
		}
		this->createSlice(newPolyVertices1, newPolyVertices1.size());
		this->createSlice(newPolyVertices2, newPolyVertices2.size());
		world->DestroyBody(affectedBody);
		drawes->clear();

	}
	return 1;

}

//Creating Sliced Boxes
void HelloWorldScene::createSlice(std::vector<b2Vec2> &vertices, int count) 
{
	//centre.x < 0.01 || centre.y < 0.01
	b2Vec2 centre = this->findCenter(vertices, vertices.size());
	if (centre.x < 0.05 || centre.y < 0.05 || centre.x < -50 || centre.x > 50 || centre.y > 50 || centre.y < -50 || centre.y != centre.y
		|| count > b2_maxPolygonVertices) {
		return;
	}
	for (int i = 0; i< count; ++i) {
		vertices[i] -= centre;
	}
	///////////////////////////
	slicedraw[table_slice_draw] = DrawNode::create();
	std::vector<b2Vec2>::iterator it;
	cocos2d::Vec2 draw_verts[b2_maxPolygonVertices];
	static int looper = 0;
	for (auto it = vertices.begin(); it != vertices.end(); it++){
		draw_verts[looper] = Vec2(PTM_RATIO*it->x, PTM_RATIO*it->y);
		looper++;
	}
	looper = 0;
	slicedraw[table_slice_draw]->drawPolygon(draw_verts, count, Color4F(1, 1, 0, 1), 0.5, Color4F(1, 1, 0, 1));
	addChild(slicedraw[table_slice_draw]);
	///////////////////////
	
	b2BodyDef sliceBody;
	sliceBody.position.Set(centre.x, centre.y);
	sliceBody.type = b2_dynamicBody;
	sliceBody.userData = slicedraw[table_slice_draw]; // <----
	b2PolygonShape slicePoly;
	b2Vec2 arrVertices[b2_maxPolygonVertices];
	std::copy(vertices.begin(), vertices.end(), arrVertices);
	
	slicePoly.Set(arrVertices, count);
	b2FixtureDef sliceFixture;
	sliceFixture.shape = &slicePoly;
	sliceFixture.density = 1;
	b2Body *pSliceBody;
	pSliceBody = world->CreateBody(&sliceBody);
	pSliceBody->CreateFixture(&sliceFixture);
	for (int i = 0; i < count; ++i) {
		vertices[i] += centre;
	}
	table_slice_draw++;
}

//Finding Area
b2Vec2 HelloWorldScene::findCenter(std::vector<b2Vec2>& vertices, int count) 
{
	b2Vec2 c;
	c.Set(0, 0); 
	float area = 0.0f;
	float p1x = 0.0f;
	float p1y = 0.0f;
	float inv3 = 1.0f / 3.0f;
	for (int i = 0; i < count; ++i) {
		b2Vec2 p2 = vertices[i];
		b2Vec2 p3 = (i + 1) < count ? vertices[i + 1] : vertices[0];
		float e1x = p2.x - p1x;
		float e1y = p2.y - p1y;
		float e2x = p3.x - p1x;
		float e2y = p3.y - p1y;
		float d = (e1x * e2y - e1y * e2x);
		float triangleArea = 0.5f * d;
		area += fabs(triangleArea);
		//area += triangleArea;
		c.x += triangleArea * inv3 * (p1x + p2.x + p3.x);
		c.y += triangleArea * inv3 * (p1y + p2.y + p3.y);
	}
	c.x *= 1.0 / area;
	c.y *= 1.0 / area;
	return c;
}
