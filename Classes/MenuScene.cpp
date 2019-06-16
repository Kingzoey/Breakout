#include "MenuScene.h"
#include "BreakoutScene.h"
#include "Breakout3Scene.h"
#include "Breakout2Scene.h"
#include "SimpleAudioEngine.h"

#pragma execution_character_set("utf-8")
using namespace CocosDenshion;
USING_NS_CC;

void MenuScene::setPhysicsWorld(PhysicsWorld* world) {  m_world = world; }

Scene* MenuScene::createScene()
{
	//创建菜单场景
	auto scene = Scene::createWithPhysics();
	scene->getPhysicsWorld()->setGravity(Point(0, 0));
	auto layer = MenuScene::create(scene->getPhysicsWorld());
	scene->addChild(layer);
	return scene;
}

//预加载开始界面音乐
void MenuScene::ppreloadMusic()
{
	SimpleAudioEngine::getInstance()->preloadBackgroundMusic("music/beginmusic.mp3");
}

//播放开始界面音乐
void MenuScene::pplayBgm()
{
	SimpleAudioEngine::getInstance()->playBackgroundMusic("music/beginmusic.mp3", true);
}

//物理世界
bool MenuScene::init(PhysicsWorld* world)
{

    if ( !Layer::init() )
    {
        return false;
    }
    
	this->setPhysicsWorld(world);
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	//添加背景图片
	auto backgroundmap = Sprite::create("Cover.jpg");
	backgroundmap->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	Size mywinsize = Director::getInstance()->getWinSize();
	//获取屏幕宽or高度
	float windowswidth = mywinsize.width; 
	float windowsheight = mywinsize.height; 
	float spx = backgroundmap->getTextureRect().getMaxX();
	float spy = backgroundmap->getTextureRect().getMaxY();
	//由屏幕宽or高度设置对应精灵宽度缩放比例
	backgroundmap->setScaleX(windowswidth / spx); 
	backgroundmap->setScaleY(windowsheight / spy);
	this->addChild(backgroundmap, -1);

//关闭游戏按钮
	auto closeItem = MenuItemImage::create(
		"CloseNormal.png",
		"CloseSelected.png",
		CC_CALLBACK_1(MenuScene::menuCloseCallback, this));

	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width / 2 - 10,
		origin.y + closeItem->getContentSize().height / 2 + 10));
	closeItem->setScale(1.5);
	auto menuu = Menu::create(closeItem, NULL);
	menuu->setPosition(Vec2::ZERO);
	this->addChild(menuu, 1);

//第一关按钮
	auto startItem0 = MenuItemImage::create(
		"level1.png",
		"level11.png",
		CC_CALLBACK_1(MenuScene::BreakoutMenuCallback, this));
	startItem0->setPosition(Vec2(visibleSize.width / 2 + origin.x - 10, visibleSize.height / 8 * 5 + origin.y -130));
	startItem0->setScale(1.5);
	auto menu = Menu::create(startItem0, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 3);

//第二关按钮
	auto startItem1 = MenuItemImage::create(
		"level2.png",
		"level22.png",
		CC_CALLBACK_1(MenuScene::Breakout2MenuCallback, this));
	startItem1->setPosition(Vec2(visibleSize.width / 2 + origin.x -10 , visibleSize.height / 8 * 5 + origin.y - 180));
	startItem1->setScale(1.5);
	auto menu1 = Menu::create(startItem1, NULL);
	menu1->setPosition(Vec2::ZERO);
	this->addChild(menu1, 3);

	//第三关按钮
	auto startItem2 = MenuItemImage::create(
		"level3.png",
		"level33.png",
		CC_CALLBACK_1(MenuScene::Breakout3MenuCallback, this));
	startItem2->setPosition(Vec2(visibleSize.width / 2 + origin.x - 10, visibleSize.height / 8 * 5 + origin.y - 230));
	startItem2->setScale(1.5);
	auto menu2= Menu::create(startItem2, NULL);
	menu2->setPosition(Vec2::ZERO);
	this->addChild(menu2, 3);
	ppreloadMusic();
	pplayBgm();
    return true;
}

//构建物理世界
MenuScene * MenuScene::create(PhysicsWorld * world) 
{
	MenuScene* pRet = new(std::nothrow) MenuScene();
	if (pRet && pRet->init(world)) 
	{
		pRet->autorelease();
		return pRet;
	}
	delete pRet;
	pRet = NULL;
	return NULL;
}

//跳转到第一关游戏页面
void MenuScene::BreakoutMenuCallback(cocos2d::Ref * pSender)
{
	CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(1.5, Breakout::createScene()));
}
//跳转到第二关游戏页面
void MenuScene::Breakout2MenuCallback(cocos2d::Ref * pSender)
{
	CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(1.5f, Breakout2::createScene()));
}
//跳转到第三关游戏页面
void MenuScene::Breakout3MenuCallback(cocos2d::Ref * pSender)
{
	CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(1.5f, Breakout3::createScene()));
}

//执行退出游戏
void MenuScene::menuCloseCallback(Ref* pSender)
{
	Director::getInstance()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}