#pragma once

#include "cocos2d.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

class Breakout3 :public Layer 
{
public:
	static int Score;
	void setPhysicsWorld(PhysicsWorld * world);
	static cocos2d::Scene* createScene();
	virtual bool init(PhysicsWorld* world);
	static Breakout3* create(PhysicsWorld* world);

private:
	Sprite* player;
	Sprite* ball;
	PhysicsWorld* m_world;
	Size visibleSize;
	Point origin;
	int blocknum;
	int flag = 0;
	cocos2d::Label* ScoreLabel;
	//相关加载的函数
	void preloadMusic();
	void playBgm();
	void addBackground();
	void addball();
	void fireBoom();
	//监听器
	void addContactListener();
	void addKeyboardListener();
	//限速
	void limitv(float f);

	bool onConcactBegan(PhysicsContact& contact);
	void onKeyPressed(EventKeyboard::KeyCode code, Event* event);
	void onKeyReleased(EventKeyboard::KeyCode code, Event* event);
};