﻿#include "MenuSence.h"
#include "BreakoutScene.h"
#include "SimpleAudioEngine.h"
#include "GateScene.h"
#include "AboutScene.h"
#pragma execution_character_set("utf-8")
using namespace CocosDenshion;
USING_NS_CC;

void MenuSence::setPhysicsWorld(PhysicsWorld* world) {  m_world = world; }

Scene* MenuSence::createScene()
{
	auto scene = Scene::createWithPhysics();
	scene->getPhysicsWorld()->setGravity(Point(0, 0));

	auto layer = MenuSence::create(scene->getPhysicsWorld());
//创建场景
	scene->addChild(layer);
	return scene;
}

//预加载及播放背景音乐
void MenuSence::ppreloadMusic()
{
	SimpleAudioEngine::getInstance()->preloadBackgroundMusic("music/beginmusic.mp3");
}

void MenuSence::pplayBgm()
{
	SimpleAudioEngine::getInstance()->playBackgroundMusic("music/beginmusic.mp3", true);
}
// on "init" you need to initialize your instance
bool MenuSence::init(PhysicsWorld* world)
{

    if ( !Layer::init() )
    {
        return false;
    }
    
	this->setPhysicsWorld(world);
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
	//背景图片
	auto bgmap = Sprite::create("Cover.jpg");
	bgmap->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	Size mywinsize = Director::getInstance()->getWinSize();
	float winw = mywinsize.width; //获取屏幕宽度
	float winh = mywinsize.height;//获取屏幕高度
	float spx = bgmap->getTextureRect().getMaxX();
	float spy = bgmap->getTextureRect().getMaxY();
	bgmap->setScaleX(winw / spx); //设置精灵宽度缩放比例
	bgmap->setScaleY(winh / spy);
	this->addChild(bgmap, -1);
//关闭游戏按钮
	auto closeItem = MenuItemImage::create(
		"CloseNormal.png",
		"CloseSelected.png",
		CC_CALLBACK_1(MenuSence::menuCloseCallback, this));

	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width / 2 - 10,
		origin.y + closeItem->getContentSize().height / 2 + 10));
	closeItem->setScale(1.5);
	auto menuu = Menu::create(closeItem, NULL);
	menuu->setPosition(Vec2::ZERO);
	this->addChild(menuu, 1);

//第一关按钮
	auto startItem = MenuItemImage::create(
		"level1.png",
		"level11.png",
		CC_CALLBACK_1(MenuSence::startMenuCallback, this));
	startItem->setPosition(Vec2(visibleSize.width / 2 + origin.x - 10, visibleSize.height / 8 * 5 + origin.y -130));
	startItem->setScale(1.5);
	auto menu = Menu::create(startItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 3);
//第二关按钮
	auto startItem1 = MenuItemImage::create(
		"level2.png",
		"level22.png",
		CC_CALLBACK_1(MenuSence::aboutMenuCallback, this));
	startItem1->setPosition(Vec2(visibleSize.width / 2 + origin.x -10 , visibleSize.height / 8 * 5 + origin.y - 180));
	startItem1->setScale(1.5);
	auto menu1 = Menu::create(startItem1, NULL);
	menu1->setPosition(Vec2::ZERO);
	this->addChild(menu1, 3);
	//第三关按钮
	auto startItem2 = MenuItemImage::create(
		"level3.png",
		"level33.png",
		CC_CALLBACK_1(MenuSence::gateMenuCallback, this));
	startItem2->setPosition(Vec2(visibleSize.width / 2 + origin.x - 10, visibleSize.height / 8 * 5 + origin.y - 230));
	startItem2->setScale(1.5);
	auto menu2= Menu::create(startItem2, NULL);
	menu2->setPosition(Vec2::ZERO);
	this->addChild(menu2, 3);
	ppreloadMusic();
	pplayBgm();
    return true;
}

MenuSence * MenuSence::create(PhysicsWorld * world) {
	MenuSence* pRet = new(std::nothrow) MenuSence();
	if (pRet && pRet->init(world)) {
		pRet->autorelease();
		return pRet;
	}
	delete pRet;
	pRet = NULL;
	return NULL;
}
//跳转到第一关游戏页面
void MenuSence::startMenuCallback(cocos2d::Ref * pSender)
{
	CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(1.5, Breakout::createScene()));
}
//跳转到第二关游戏页面
void MenuSence::aboutMenuCallback(cocos2d::Ref * pSender)
{
	CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(1.5f, About::createScene()));
}
//跳转到第三关游戏页面
void MenuSence::gateMenuCallback(cocos2d::Ref * pSender)
{
	CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(1.5f, Gate::createScene()));
}
//退出游戏
void MenuSence::menuCloseCallback(Ref* pSender)
{
	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}