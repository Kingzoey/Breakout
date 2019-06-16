#ifndef __MENU_SCENE_H__
#define __MENU_SCENE_H__

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
USING_NS_CC;

class MenuScene : public cocos2d::Layer
{
public:
	void setPhysicsWorld(PhysicsWorld * world);
	static cocos2d::Scene* createScene();
	void ppreloadMusic();   //预加载音乐
	void pplayBgm();   //播放背景音乐
	virtual bool init(PhysicsWorld* world);
	static MenuScene* create(PhysicsWorld* world);
	//第一二三关场景
	virtual void BreakoutMenuCallback(cocos2d::Ref * pSender);
    virtual void Breakout2MenuCallback(cocos2d::Ref* pSender);
	virtual void Breakout3MenuCallback(cocos2d::Ref* pSender);
	void menuCloseCallback(Ref* pSender);
private:
	PhysicsWorld* m_world;
};

#endif 
// __MENU_SCENE_H__
