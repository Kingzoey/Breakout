#ifndef __MENU_SEBCE_H__
#define __MENU_SEBCE_H__

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
USING_NS_CC;

class MenuSence : public cocos2d::Layer
{
public:
	void setPhysicsWorld(PhysicsWorld * world);
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene* createScene();
	void ppreloadMusic();
	void pplayBgm();
	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init(PhysicsWorld* world);

	// implement the "static create()" method manually
	static MenuSence* create(PhysicsWorld* world);

	virtual void startMenuCallback(cocos2d::Ref * pSender);
    void aboutMenuCallback(cocos2d::Ref* pSender);
	void menuCloseCallback(Ref* pSender);
private:
	PhysicsWorld* m_world;
};

#endif // __MENU_SEBCE_H__
