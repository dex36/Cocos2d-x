

#ifndef __HELLOWORLDHUB_SCENE_H__
#define __HELLOWORLDHUB_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

class HelloWorldHud : public cocos2d::Layer
{
public:
	cocos2d::ui::Button *buttonup;
	cocos2d::ui::Button *buttonleft;
	cocos2d::ui::Button *buttonright;
	virtual bool init();
	bool movingup;
	bool movingleft;
	bool movingright;
	CREATE_FUNC(HelloWorldHud);
};

#endif
