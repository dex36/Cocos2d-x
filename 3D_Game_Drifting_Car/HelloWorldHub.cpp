
#include "HelloWorldHub.h"
#include "HelloWorldScene.h"
USING_NS_CC;

bool HelloWorldHud::init()
{
	Size winSize = Director::sharedDirector()->getWinSize();
	int margin = 10;

	buttonup = ui::Button::create("up.png");
	buttonup->setPosition(ccp(winSize.width - (buttonup->getContentSize().width / 4) - 2*margin, buttonup->getContentSize().height / 4 + margin));
	buttonup->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type){
		switch (type)
		{
		case ui::Widget::TouchEventType::BEGAN:
			movingup = true;
			break;
		case ui::Widget::TouchEventType::MOVED:
			movingup = false;
			break;
		case ui::Widget::TouchEventType::ENDED:
			movingup = false;
			break;
		default:
			movingup = false;
			break;
		}
	});
	this->addChild(buttonup,1);

	buttonleft = ui::Button::create("left.png");
	buttonleft->setPosition(ccp((buttonleft->getContentSize().width / 4) + margin, buttonleft->getContentSize().height / 4 + margin));
	buttonleft->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type){
		switch (type)
		{
		case ui::Widget::TouchEventType::BEGAN:
			movingleft = true;
			movingright = false;
			break;
		case ui::Widget::TouchEventType::MOVED:
			movingleft = true;
			movingright = false;
			break;
		case ui::Widget::TouchEventType::ENDED:
			movingleft = false;
			break;
		default:
			movingleft = false;
			break;
		}
	});
	this->addChild(buttonleft,1);

	buttonright = ui::Button::create("right.png");
	buttonright->setPosition(ccp((buttonright->getContentSize().width / 4) + 8*margin, buttonright->getContentSize().height / 4 + margin));
	buttonright->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type){
		switch (type)
		{
		case ui::Widget::TouchEventType::BEGAN:
			movingright = true;
			movingleft= false;
			break;
		case ui::Widget::TouchEventType::MOVED:
			movingright = true;
			movingleft = false;
			break;
		case ui::Widget::TouchEventType::ENDED:
			movingright = false;
			break;
		default:
			movingright = false;
			break;
		}
	});
	this->addChild(buttonright,1);


	return true;
}
