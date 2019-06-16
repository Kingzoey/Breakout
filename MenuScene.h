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
	void ppreloadMusic();   //Ԥ��������
	void pplayBgm();   //���ű�������
	virtual bool init(PhysicsWorld* world);
	static MenuScene* create(PhysicsWorld* world);
	//��һ�����س���
	virtual void BreakoutMenuCallback(cocos2d::Ref * pSender);
    virtual void Breakout2MenuCallback(cocos2d::Ref* pSender);
	virtual void Breakout3MenuCallback(cocos2d::Ref* pSender);
	void menuCloseCallback(Ref* pSender);
private:
	PhysicsWorld* m_world;
};

#endif 
// __MENU_SCENE_H__
