#include "Breakout3Scene.h"
#include <string>
#include "SimpleAudioEngine.h"
#include "GameOverScene.h"

using namespace std;
using namespace CocosDenshion;

//初始化积分
int Breakout3::Score = 0;
void Breakout3::setPhysicsWorld(PhysicsWorld* world) { m_world = world; }

Scene* Breakout3::createScene()
{
	auto scene = Scene::createWithPhysics();
	scene->getPhysicsWorld()->setGravity(Point(0, 0));
	auto layer = Breakout3::create(scene->getPhysicsWorld());
	scene->addChild(layer);
	return scene;
}

bool Breakout3::init(PhysicsWorld* world)
{
	if (!Layer::init())
	{
		return false;
	}
	Score = 0;
	this->setPhysicsWorld(world);
	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();
	//调用加载函数
	preloadMusic();
	playBgm();
	addBackground();
	addball();
	addContactListener();
	addKeyboardListener();
	this->schedule(schedule_selector(Breakout3::limitv), 0.1f, kRepeatForever, 0);
	return true;
}

//预加载以及播放音乐
void Breakout3::preloadMusic()
{
	SimpleAudioEngine::getInstance()->preloadBackgroundMusic("music/backgroundmusic3.mp3");
}

void Breakout3::playBgm()
{
	SimpleAudioEngine::getInstance()->playBackgroundMusic("music/backgroundmusic3.mp3", true);
}

//加载地图背景等
void Breakout3::addBackground()
{
	//加载砖块地图
	TMXTiledMap* tmx = TMXTiledMap::create("rose.tmx");
	tmx->setPosition(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y);
	tmx->setAnchorPoint(Vec2(0.5, 0.5));
	this->addChild(tmx, 0);

	TMXObjectGroup *objGroup = tmx->objectGroupNamed("objects");
	ValueVector tmxBlocks = objGroup->getObjects();
	blocknum = tmxBlocks.size();

	//两种砖块
	for (int i = 0; i < blocknum; i++)
	{
		ValueMap message = tmxBlocks.at(i).asValueMap();
		float position_x = message.at("x").asFloat();
		float position_y = message.at("y").asFloat();
		if (i == 60 || i == 14 || i == 8) 
		{
			Sprite* block = Sprite::create("block1.png");
			block->setAnchorPoint(Vec2(0.5, 0.5));
			block->setPhysicsBody(PhysicsBody::createBox(block->getContentSize(), PhysicsMaterial(0.0f, 1.0f, 0.0f)));
			block->setPosition(origin + Vec2(position_x + 150, position_y + 30) + tmx->getContentSize() / 7);
			block->getPhysicsBody()->setTag(9);
			block->setColor(ccc3(26, 185, 16));
			block->getPhysicsBody()->setDynamic(false);
			block->getPhysicsBody()->setContactTestBitmask(0x01);
			addChild(block);
		}
		else 
		{
			Sprite* block = Sprite::create("block1.png");
			block->setAnchorPoint(Vec2(0.5, 0.5));
			block->setPhysicsBody(PhysicsBody::createBox(block->getContentSize(), PhysicsMaterial(0.0f, 1.0f, 0.0f)));
			block->setPosition(origin + Vec2(position_x + 150, position_y + 30) + tmx->getContentSize() / 7);
			block->getPhysicsBody()->setTag(4);
			block->getPhysicsBody()->setDynamic(false);
			block->getPhysicsBody()->setContactTestBitmask(0x01);
			addChild(block);
		}
	}
	//记分
	TTFConfig ttfConfig;
	ttfConfig.fontFilePath = "fonts/arial.ttf";
	ttfConfig.fontSize = 24;
	CCString *str = CCString::createWithFormat("Score : %d", Score);
	std::string ScoreStr = str->getCString();
	ScoreLabel = Label::createWithTTF(ttfConfig, ScoreStr);
	ScoreLabel->setPosition(Vec2(origin.x + visibleSize.width - ScoreLabel->getContentSize().width * 2,
		origin.y + visibleSize.height - ScoreLabel->getContentSize().height));
	addChild(ScoreLabel);

	auto edgeSp = Sprite::create();
	auto boundBody = PhysicsBody::createEdgeBox(visibleSize, PhysicsMaterial(0.0f, 1.0f, 0.0f));
	boundBody->setDynamic(false);
	boundBody->setTag(0);
	edgeSp->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	edgeSp->setPhysicsBody(boundBody);
	edgeSp->getPhysicsBody()->setTag(3);
	edgeSp->getPhysicsBody()->setContactTestBitmask(0x10);
	this->addChild(edgeSp);

	//背景图片
	auto bgmap = Sprite::create("background3.jpg");
	bgmap->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	Size mywinsize = Director::getInstance()->getWinSize();
	//获取屏幕宽度
	float windowswidth = mywinsize.width; 
	//获取屏幕高度
	float windowsheight = mywinsize.height;
	float spx = bgmap->getTextureRect().getMaxX();
	float spy = bgmap->getTextureRect().getMaxY();
	bgmap->setScaleX(windowswidth / spx); //设置精灵宽度缩放比例
	bgmap->setScaleY(windowsheight / spy);
	this->addChild(bgmap, -1);

	//底边
	auto edge = Sprite::create("deadbar.png");
	edge->setAnchorPoint(Vec2(0.5, 0.5));
	edge->setScaleX(visibleSize.width / edge->getContentSize().width);
	edge->setPhysicsBody(PhysicsBody::createBox(edge->getContentSize()));
	edge->setPosition(origin.x + visibleSize.width / 2, origin.y + edge->getContentSize().height);
	edge->getPhysicsBody()->setDynamic(false);
	edge->getPhysicsBody()->setTag(5);
	edge->getPhysicsBody()->setContactTestBitmask(0x01);
	addChild(edge);

	//板
	player = Sprite::create("player.jpg");
	player->setScaleX(1.5f);
	player->setAnchorPoint(Vec2(0.5, 0.5));
	player->setPhysicsBody(PhysicsBody::createBox(player->getContentSize(), PhysicsMaterial(0.0f, 1.0f, 0.0f)));
	player->setPosition(origin.x + visibleSize.width / 2, origin.y + player->getContentSize().height / 2);
	player->getPhysicsBody()->setTag(1);
	player->getPhysicsBody()->setAngularVelocityLimit(0);
	player->getPhysicsBody()->setRotationEnable(false);
	player->getPhysicsBody()->setMass(10000);
	player->getPhysicsBody()->setContactTestBitmask(0x10);
	addChild(player);
}

//球
void Breakout3::addball()
{
	ball = Sprite::create("irolball.png");
	ball->setPosition(origin.x + visibleSize.width / 2, origin.y + ball->getContentSize().height);
	ball->setPhysicsBody(PhysicsBody::createCircle(ball->getContentSize().height / 2, PhysicsMaterial(0.0f, 1.0f, 0.0f)));
	ball->getPhysicsBody()->setTag(2);
	ball->getPhysicsBody()->setVelocity(Vec2(0, 350));
	ball->getPhysicsBody()->applyImpulse(Vec2(50, 10));
	ball->getPhysicsBody()->setMass(10);
	ball->getPhysicsBody()->setContactTestBitmask(0x01);
	addChild(ball);
}

void Breakout3::fireBoom() 
{
	/* 创建背景精灵 */
	Sprite* bkSnow = Sprite::create("bkSky.jpg");
	bkSnow->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	this->addChild(bkSnow);

	//烟花效果  
	CCParticleSystem* particleSystem1 = CCParticleFireworks::create();
	particleSystem1->setTexture(CCTextureCache::sharedTextureCache()->addImage("fire.png"));
	particleSystem1->setPosition(Point(80, 0));
	addChild(particleSystem1);

	CCParticleSystem* particleSystem2 = CCParticleFireworks::create();
	particleSystem2->setTexture(CCTextureCache::sharedTextureCache()->addImage("fire.png"));
	particleSystem2->setPosition(Point(240, 0));
	addChild(particleSystem2);

	CCParticleSystem* particleSystem3 = CCParticleFireworks::create();
	particleSystem3->setTexture(CCTextureCache::sharedTextureCache()->addImage("fire.png"));
	particleSystem3->setPosition(Point(350, 0));
	addChild(particleSystem3);

	return;
}

void Breakout3::addContactListener()
{
	auto touchListener = EventListenerPhysicsContact::create();
	touchListener->onContactBegin = CC_CALLBACK_1(Breakout3::onConcactBegan, this);
	_eventDispatcher->addEventListenerWithFixedPriority(touchListener, 1);
}

void Breakout3::addKeyboardListener()
{
	auto keboardListener = EventListenerKeyboard::create();
	keboardListener->onKeyPressed = CC_CALLBACK_2(Breakout3::onKeyPressed, this);
	keboardListener->onKeyReleased = CC_CALLBACK_2(Breakout3::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keboardListener, this);
}

//通过Tag碰撞检测
bool Breakout3::onConcactBegan(PhysicsContact& contact) 
{
	auto node1 = contact.getShapeA()->getBody();
	auto node2 = contact.getShapeB()->getBody();
	auto explosion = ParticleExplosion::create();
	if (node1 && node2)
	{
		if ((node1->getTag() == 2 && node2->getTag() == 4) || (node1->getTag() == 4 && node2->getTag() == 2))
		{
			if (node1->getTag() == 2 && node2->getTag() == 4)
			{
				//爆炸
				explosion->setPosition(node2->getPosition());
				addChild(explosion);			
				node2->getNode()->removeFromParentAndCleanup(true);
			}
			else if (node1->getTag() == 4 && node2->getTag() == 2)
			{
				//爆炸
				explosion->setPosition(node1->getPosition());
				addChild(explosion);
				node1->getNode()->removeFromParentAndCleanup(true);
			}
			blocknum = blocknum - 1;
			Score = Score + 12;
			CCString *strss = CCString::createWithFormat("Score : %d", Score);
			std::string ScoreStrrr = strss->getCString();
			ScoreLabel->setString(ScoreStrrr);
			if (blocknum == 0)
			{
				CCDirector::sharedDirector()->replaceScene(GameOverScene::createScene());
			}
		}
		if ((node1->getTag() == 2 && node2->getTag() == 9) || (node1->getTag() == 9 && node2->getTag() == 2)) 
		{
			if (node1->getTag() == 2 && node2->getTag() == 9) 
			{
				//爆炸
				explosion->setPosition(node2->getPosition());
				addChild(explosion);
				node2->getNode()->removeFromParentAndCleanup(true);
			}
			else if (node1->getTag() == 9 && node2->getTag() == 2)
			{
				//爆炸
				explosion->setPosition(node1->getPosition());
				addChild(explosion);
				node1->getNode()->removeFromParentAndCleanup(true);
			}
			blocknum =  blocknum - 1;
			Score = Score + 50;
			CCString *strsqw = CCString::createWithFormat("Score : %d", Score);
			std::string ScoreStrqw = strsqw->getCString();
			ScoreLabel->setString(ScoreStrqw);
			if (blocknum == 0)
			{
				CCDirector::sharedDirector()->replaceScene(GameOverScene::createScene());
			}
		}

		if ((node1->getTag() == 1 && node2->getTag() == 3) || (node1->getTag() == 3 && node2->getTag() == 1)) 
		{
			if (node1->getTag() == 1 && node2->getTag() == 3)
				node1->setVelocity(Vec2(0, 0));
			else if (node1->getTag() == 3 && node2->getTag() == 1)
				node2->setVelocity(Vec2(0, 0));
		}
		if ((node1->getTag() == 2 && node2->getTag() == 5) || (node1->getTag() == 5 && node2->getTag() == 2)) 
		{
			CCDirector::sharedDirector()->replaceScene(GameOverScene::createScene());
		}
	}

	return true;
}


//键盘按下
void Breakout3::onKeyPressed(EventKeyboard::KeyCode code, Event* event) {
	switch (code) {
	case cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW:
		player->getPhysicsBody()->setVelocity(Point(-300, player->getPhysicsBody()->getVelocity().y));
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		player->getPhysicsBody()->setVelocity(Point(300, player->getPhysicsBody()->getVelocity().y));
		break;

	default:
		break;
	}
}

//释放键盘
void Breakout3::onKeyReleased(EventKeyboard::KeyCode code, Event* event) 
{
	player->getPhysicsBody()->setVelocity(Vec2(0, 0));
}

//限制速度
void Breakout3::limitv(float f)
{
	float velx = ball->getPhysicsBody()->getVelocity().x;
	float vely = ball->getPhysicsBody()->getVelocity().y;
	if (velx > 350 || vely > 350)
	{
		if (velx > 350)
			ball->getPhysicsBody()->setVelocity(Vec2(350, vely));
		else if (vely > 350)
			ball->getPhysicsBody()->setVelocity(Vec2(velx, 350));
	}
	else if (velx < -350 || vely < -350)
	{
		if (velx < -350)
			ball->getPhysicsBody()->setVelocity(Vec2(-350, vely));
		else if (vely < -350)
			ball->getPhysicsBody()->setVelocity(Vec2(velx, -350));
	}
	else if (velx > -100 && velx < 100)
	{
		ball->getPhysicsBody()->setVelocity(Vec2(3 * velx, vely));
	}
	else if (vely > -100 && vely < 100)
	{
		ball->getPhysicsBody()->setVelocity(Vec2(velx, 3 * vely));
	}

}

//构建物理世界
Breakout3 * Breakout3::create(PhysicsWorld * world) 
{
	Breakout3* pRet = new(std::nothrow) Breakout3();
	if (pRet && pRet->init(world)) 
	{
		pRet->autorelease();
		return pRet;
	}
	delete pRet;
	pRet = NULL;
	return NULL;
}
