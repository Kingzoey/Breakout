#ifndef __ABOUT_SEBCE_H__
#define __ABOUT_SEBCE_H__

#include "cocos2d.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

class About : public cocos2d::Layer
{
public:
	
	static cocos2d::Scene* createScene();
	
	virtual bool init();


	void CancelMenuCallback(cocos2d::Ref* pSender);

	CREATE_FUNC(About);
};

#endif // __MENU_SEBCE_H__