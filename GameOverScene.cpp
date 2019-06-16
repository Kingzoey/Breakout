#include "GameOverScene.h"
#include "cocostudio/CocoStudio.h"
#include "BreakoutScene.h"
#include "Breakout2Scene.h"
#include "Breakout3Scene.h"
#include "MenuScene.h"
#include <regex>

#define database UserDefault::getInstance()
#pragma execution_character_set("utf-8")
using std::to_string;
using std::regex;
using std::match_results;
using std::regex_match;
using std::cmatch;
using namespace rapidjson;
using namespace CocosDenshion;
USING_NS_CC;

//创建场景
Scene* GameOverScene::createScene()
{
	auto scene = Scene::create();
	auto layer = GameOverScene::create();
	scene->addChild(layer);
	return scene;
}

bool GameOverScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	//积分数据库
	if (!database->getBoolForKey("isExist")) 
	{
		database->setBoolForKey("isExist", true);
		database->setIntegerForKey("record", 0);
	}
	
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	//添加背景图片
	auto backgroundmap = Sprite::create("end.jpg");
	backgroundmap->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	Size windowssize = Director::getInstance()->getWinSize();

	//获取屏幕宽or高度
	float windowswidth = windowssize.width; 
	float windowsheight = windowssize.height;
	float spx = backgroundmap->getTextureRect().getMaxX();
	float spy = backgroundmap->getTextureRect().getMaxY();

	//根据屏幕宽度高度设置精灵宽度缩放比例
	backgroundmap->setScaleX(windowswidth / spx); 
	backgroundmap->setScaleY(windowsheight / spy);
	this->addChild(backgroundmap, -1);

	//计分数据库
	int record = database->getIntegerForKey("record");

	Size size = Director::getInstance()->getVisibleSize();
	Point originn = Director::getInstance()->getVisibleOrigin();
	visibleHeight = size.height;
	visibleWidth = size.width;
	
	//加载游戏结束图片
	auto over = Sprite::create("GameOver.png");
	over->setPosition(Vec2(size.width / 2 + originn.x, size.height / 2 + originn.y + 150));
	this->addChild(over, 0);

	TTFConfig ttfConfig;
	ttfConfig.fontFilePath = "fonts/arial.ttf";
	ttfConfig.fontSize = 26;

	//加载第一关分数
	CCString *str = CCString::createWithFormat("%d", Breakout::Score);
	std::string ScoreStr = str->getCString();
	Label* yourScoreLabel = Label::createWithTTF(ttfConfig, "Level 1 Score: " + ScoreStr);
	yourScoreLabel->setPosition(Vec2(origin.x + size.width / 2,
		origin.y + size.height / 2 ));
	addChild(yourScoreLabel);

	//加载第二关分数
	CCString *str1 = CCString::createWithFormat("%d", Breakout2::Score);
	std::string ScoreStr1 = str1->getCString();
	Label* yourScoreLabel1 = Label::createWithTTF(ttfConfig, "Level 2 Score: " + ScoreStr1);
	yourScoreLabel1->setPosition(Vec2(origin.x + size.width / 2,
		origin.y + size.height / 2 - 30));
	addChild(yourScoreLabel1);

	//加载第三关分数
	CCString *str2 = CCString::createWithFormat("%d", Breakout3::Score);
	std::string ScoreStr2 = str2->getCString();
	Label* yourScoreLabel2 = Label::createWithTTF(ttfConfig, "Level 3 Score: " + ScoreStr2);
	yourScoreLabel2->setPosition(Vec2(origin.x + size.width / 2,
		origin.y + size.height / 2 - 60));
	addChild(yourScoreLabel2);

	//更新最高分
	if (Breakout::Score > record)
	{
		record = Breakout::Score;
		database->setIntegerForKey("record", record);
	}
	if (Breakout2::Score > record)
	{
		record = Breakout2::Score;
		database->setIntegerForKey("record", record);
	}
	if (Breakout3::Score > record)
	{
		record = Breakout3::Score;
		database->setIntegerForKey("record", record);
	}

	//最高分
	CCString *strr = CCString::createWithFormat("%d", record);
	std::string recordStr = strr->getCString();
	Label* recordLabel = Label::createWithTTF(ttfConfig, "Top Score: " + recordStr);
	recordLabel->setPosition(Vec2(origin.x + size.width / 2,
		origin.y + size.height / 2 + recordLabel->getContentSize().height * 2));
	addChild(recordLabel);

	//返回菜单
	auto backItem = MenuItemImage::create(
		"backmenu.png",
		"backmenuu.png",
		CC_CALLBACK_1(GameOverScene::BackMenuCallback, this));
	backItem->setPosition(Vec2(visibleWidth / 2 + origin.x + 0, visibleHeight / 4 + origin.y +10));
	backItem->setScale(1.5);
	auto back0 = Menu::create(backItem, NULL);
	back0->setPosition(Vec2::ZERO);
	this->addChild(back0, 1);


	//退出游戏
	auto agn = MenuItemImage::create(
		"CloseNormal.png",
		"CloseSelected.png",
		CC_CALLBACK_1(GameOverScene::menuCloseCallback, this));
	agn->setScale(2);
	agn->setPosition(Vec2(visibleWidth / 2 + origin.x + 0, visibleHeight / 4 + origin.y - 50));

	auto menu = Menu::create(agn, NULL);
	menu->setPosition(Vec2::ZERO);
	addChild(menu);
	Size visibleSizee = Director::getInstance()->getVisibleSize();
}

//返回菜单
void GameOverScene::BackMenuCallback(cocos2d::Ref* pSender) 
{
	CCDirector::sharedDirector()->replaceScene(CCTransitionFadeTR::create(1.5, MenuScene::createScene()));
}

//退出游戏
void GameOverScene::menuCloseCallback(Ref* pSender) 
{
	Director::getInstance()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}
