#include "GameOverScene.h"
#include "cocostudio/CocoStudio.h"
#include "BreakoutScene.h"
#include "AboutScene.h"
#include "MenuSence.h"
#include "GateScene.h"
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


Scene* GameOverScene::createScene()
{

	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = GameOverScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;


}

bool GameOverScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	if (!database->getBoolForKey("isExist")) {
		database->setBoolForKey("isExist", true);
		database->setIntegerForKey("record", 0);
	}
	
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	//����ͼƬ
	auto bgmap = Sprite::create("end.jpg");
	bgmap->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	Size mywinsize = Director::getInstance()->getWinSize();
	float winw = mywinsize.width; //��ȡ��Ļ���
	float winh = mywinsize.height;//��ȡ��Ļ�߶�
	float spx = bgmap->getTextureRect().getMaxX();
	float spy = bgmap->getTextureRect().getMaxY();
	bgmap->setScaleX(winw / spx); //���þ��������ű���
	bgmap->setScaleY(winh / spy);
	this->addChild(bgmap, -1);

	//�Ʒ����ݿ�
	int record = database->getIntegerForKey("record");

	Size size = Director::getInstance()->getVisibleSize();
	Point originn = Director::getInstance()->getVisibleOrigin();
	visibleHeight = size.height;
	visibleWidth = size.width;
	
	//������Ӧ��ͼƬ
	auto over = Sprite::create("GameOver.png");
	over->setPosition(Vec2(size.width / 2 + originn.x, size.height / 2 + originn.y + 150));
	this->addChild(over, 0);

	TTFConfig ttfConfig;
	ttfConfig.fontFilePath = "fonts/arial.ttf";
	ttfConfig.fontSize = 24;

	CCString *str = CCString::createWithFormat("%d", Breakout::Score);
	std::string ScoreStr = str->getCString();
	Label* yourScoreLabel = Label::createWithTTF(ttfConfig, "Score: " + ScoreStr);
	yourScoreLabel->setPosition(Vec2(origin.x + size.width / 2,
		origin.y + size.height / 2));
	addChild(yourScoreLabel);
	//���»���
	if (Breakout::Score > record)
	{
		record = Breakout::Score;
		database->setIntegerForKey("record", record);
	}
	if (About::Score > record)
	{
		record = About::Score;
		database->setIntegerForKey("record", record);
	}
	if (Gate::Score > record)
	{
		record = Gate::Score;
		database->setIntegerForKey("record", record);
	}
	CCString *strr = CCString::createWithFormat("%d", record);
	std::string recordStr = strr->getCString();
	Label* recordLabel = Label::createWithTTF(ttfConfig, "Top Score: " + recordStr);
	recordLabel->setPosition(Vec2(origin.x + size.width / 2,
		origin.y + size.height / 2 + recordLabel->getContentSize().height * 2));
	addChild(recordLabel);

	//�˳���Ϸ
	auto agn = MenuItemImage::create(
		"CloseNormal.png",
		"CloseSelected.png",
		CC_CALLBACK_1(GameOverScene::menuCloseCallback, this));
	agn->setScale(2);
	agn->setPosition(Vec2(visibleWidth / 2 + origin.x + 0, visibleHeight / 4 + origin.y - 50));

	auto menu = Menu::create(agn, NULL);
	menu->setPosition(Vec2::ZERO);
	addChild(menu);

	/*auto agn1 = MenuItemImage::create("f1.png", "f2.png", [&](Ref *sender) 
	{
		Director::sharedDirector()->replaceScene(Breakout::createScene());
	});
	agn1->setScale(2);
	agn1->setPosition(Vec2(visibleWidth / 2 + origin.x + 150, visibleHeight / 4 + origin.y - 50));

	auto menu1 = Menu::create(agn1, NULL);
	menu1->setPosition(Vec2::ZERO);
	addChild(menu1);*/

	Size visibleSizee = Director::getInstance()->getVisibleSize();
}

void GameOverScene::menuCloseCallback(Ref* pSender) 
{
	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}
