#include "BreakoutScene.h"
#include <string>
#include "SimpleAudioEngine.h"

#include "GameOverScene.h"
using namespace std;
using namespace CocosDenshion;

int Breakout::Score = 0;
void Breakout::setPhysicsWorld(PhysicsWorld* world) { m_world = world; }
//��������
Scene* Breakout::createScene() 
{
    auto scene = Scene::createWithPhysics();
    scene->getPhysicsWorld()->setGravity(Point(0, 0));

    auto layer = Breakout::create(scene->getPhysicsWorld());
//�����������糡��
    scene->addChild(layer);
    return scene;
}

bool Breakout::init(PhysicsWorld* world) 
{
    if (!Layer::init()) 
	{
        return false;
    }
	Score = 0;
	this->setPhysicsWorld(world);
    visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();
//���ü��غ���
    preloadMusic();
    playBgm();
    addBackground();
    addball();
	addContactListener();
    addKeyboardListener();

	this->schedule(schedule_selector(Breakout::update), 0.1f, kRepeatForever, 0);
//ÿ0.1�����һ��
    return true;
}

//Ԥ�����Լ���������
void Breakout::preloadMusic() 
{
    SimpleAudioEngine::getInstance()->preloadBackgroundMusic("music/backgroundmusic2.mp3");
}

void Breakout::playBgm() 
{
    SimpleAudioEngine::getInstance()->playBackgroundMusic("music/backgroundmusic2.mp3", true);
}

void Breakout::addBackground() 
{
//ͨ��TMXTiledMap�����Լ�������ש���ͼ
	TMXTiledMap* tmx = TMXTiledMap::create("cpp.tmx");
	tmx->setPosition(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y);
	tmx->setAnchorPoint(Vec2(0.5, 0.5));
	this->addChild(tmx, 0);

	TMXObjectGroup *objGroup = tmx->objectGroupNamed("objects");
	ValueVector tmxBlocks = objGroup->getObjects();
	blocknum = tmxBlocks.size();
//��Ϊ����ש�飬��������ɫ��ש�����ӷ֣�Tag��ͬ��
	for (int i = 0; i < blocknum; i++)
	{
		ValueMap objInfo = tmxBlocks.at(i).asValueMap();
		float posx = objInfo.at("x").asFloat();
		float posy = objInfo.at("y").asFloat();
		if (i == 60 || i == 14 || i == 8) {
			Sprite* block = Sprite::create("block2.png");
			block->setAnchorPoint(Vec2(0.5, 0.5));
			block->setPhysicsBody(PhysicsBody::createBox(block->getContentSize(), PhysicsMaterial(0.0f, 1.0f, 0.0f)));
			block->setPosition(origin + Vec2(posx + 150, posy + 30) + tmx->getContentSize() / 7);
			block->getPhysicsBody()->setTag(9);
			block->setColor(ccc3(27,193,13));
			block->getPhysicsBody()->setDynamic(false);
			block->getPhysicsBody()->setContactTestBitmask(0x01);
			addChild(block);
		}
		else {
			Sprite* block = Sprite::create("block2.png");
			block->setAnchorPoint(Vec2(0.5, 0.5));
			block->setPhysicsBody(PhysicsBody::createBox(block->getContentSize(), PhysicsMaterial(0.0f, 1.0f, 0.0f)));
			block->setPosition(origin + Vec2(posx + 150, posy + 30) + tmx->getContentSize() / 7);
			block->getPhysicsBody()->setTag(4);
			block->getPhysicsBody()->setDynamic(false);
			block->getPhysicsBody()->setContactTestBitmask(0x01);
			addChild(block);
		}
		
	}


//�Ƿ�
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

	//����ͼƬ
	auto bgmap = Sprite::create("background2.jpg");
	bgmap->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	Size mywinsize = Director::getInstance()->getWinSize();
	float winw = mywinsize.width; //��ȡ��Ļ����
	float winh = mywinsize.height;//��ȡ��Ļ�߶�
	float spx = bgmap->getTextureRect().getMaxX();
	float spy = bgmap->getTextureRect().getMaxY();
	bgmap->setScaleX(winw / spx); //���þ���������ű���
	bgmap->setScaleY(winh / spy);
	this->addChild(bgmap, -1);
	
//���µı߽�
	auto deadSp = Sprite::create("deadbar.png");
	deadSp->setAnchorPoint(Vec2(0.5, 0.5));
	deadSp->setScaleX(visibleSize.width / deadSp->getContentSize().width);
	deadSp->setPhysicsBody(PhysicsBody::createBox(deadSp->getContentSize()));
	deadSp->setPosition(origin.x + visibleSize.width / 2, origin.y + deadSp->getContentSize().height);
	deadSp->getPhysicsBody()->setDynamic(false);
	deadSp->getPhysicsBody()->setTag(5);
	deadSp->getPhysicsBody()->setContactTestBitmask(0x01);
	addChild(deadSp);
//��
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

//��
void Breakout::addball() 
{
	ball = Sprite::create("basketball.png");
	ball->setPosition(origin.x + visibleSize.width / 2, origin.y + ball->getContentSize().height);
	ball->setPhysicsBody(PhysicsBody::createCircle(ball->getContentSize().height / 2,PhysicsMaterial(0.0f,1.0f,0.0f)));
	ball->getPhysicsBody()->setTag(2);
	ball->getPhysicsBody()->setVelocity(Vec2(0,300));
	ball->getPhysicsBody()->applyImpulse(Vec2(50, 10));
	ball->getPhysicsBody()->setMass(10);
	ball->getPhysicsBody()->setContactTestBitmask(0x01);
	addChild(ball);
}

void Breakout::fireBoom() {

	/* ������������ */
	Sprite* bkSnow = Sprite::create("bkSky.jpg");
	bkSnow->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	this->addChild(bkSnow);

	//�̻�Ч��  
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
	particleSystem3->setPosition(Point(400, 0));
	addChild(particleSystem3);

	return;
}

void Breakout::addContactListener() 
{
	auto touchListener = EventListenerPhysicsContact::create();
	touchListener->onContactBegin = CC_CALLBACK_1(Breakout::onConcactBegan, this);
	_eventDispatcher->addEventListenerWithFixedPriority(touchListener, 1);
}

void Breakout::addKeyboardListener() 
{
    auto keboardListener = EventListenerKeyboard::create();
    keboardListener->onKeyPressed = CC_CALLBACK_2(Breakout::onKeyPressed, this);
    keboardListener->onKeyReleased = CC_CALLBACK_2(Breakout::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(keboardListener, this);
}
//ͨ��Tag��ײ���
bool Breakout::onConcactBegan(PhysicsContact& contact) {
	
	

	auto node1 = contact.getShapeA()->getBody();
	auto node2 = contact.getShapeB()->getBody();
	if (node1 && node2)
	{
		if ((node1->getTag() == 2 && node2->getTag() == 4) || (node1->getTag() == 4 && node2->getTag() == 2))
		{
			if (node1->getTag() == 2 && node2->getTag() == 4)
			{
				node2->getNode()->removeFromParentAndCleanup(true);
			}
			else if (node1->getTag() == 4 && node2->getTag() == 2)
			{
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
		if ((node1->getTag() == 2 && node2->getTag() == 9) || (node1->getTag() == 9 && node2->getTag() == 2)) {
			if (node1->getTag() == 2 && node2->getTag() == 9) {
				node2->getNode()->removeFromParentAndCleanup(true);
			}
			else if (node1->getTag() == 9 && node2->getTag() == 2) 
			{
				node1->getNode()->removeFromParentAndCleanup(true);
			}
			blocknum -= 1;
			Score = Score + 50;
			CCString *strsqw = CCString::createWithFormat("Score : %d", Score);
			std::string ScoreStrqw = strsqw->getCString();
			ScoreLabel->setString(ScoreStrqw);
			if (blocknum == 0) 
			{
				CCDirector::sharedDirector()->replaceScene(GameOverScene::createScene());
			}
		}

		if ((node1->getTag() == 1 && node2->getTag() == 3) || (node1->getTag() == 3 && node2->getTag() == 1)){
			if(node1->getTag() == 1 && node2->getTag() == 3)
			    node1->setVelocity(Vec2(0,0));
			else if(node1->getTag() == 3 && node2->getTag() == 1)
				node2->setVelocity(Vec2(0, 0));
		}
		if ((node1->getTag() == 2 && node2->getTag() == 5) || (node1->getTag() == 5 && node2->getTag() == 2)){
			CCDirector::sharedDirector()->replaceScene(GameOverScene::createScene());
		}
	}

	return true;
}
//���º���
void Breakout::update(float f) 
{
	float velx = ball->getPhysicsBody()->getVelocity().x;
	float vely = ball->getPhysicsBody()->getVelocity().y;
	if (velx > 300 || vely > 300)
	{
		if(velx > 300)
		ball->getPhysicsBody()->setVelocity(Vec2(300, vely));
		else if(vely > 300)
			ball->getPhysicsBody()->setVelocity(Vec2(velx, 300));
	}
	else if (velx < -300 || vely < -300)
	{
		if(velx < -300)
		ball->getPhysicsBody()->setVelocity(Vec2(-300, vely));
		else if(vely < -300)
			ball->getPhysicsBody()->setVelocity(Vec2(velx, -300));
	}
	else if (velx > -50 && velx < 50)
	{
		ball->getPhysicsBody()->setVelocity(Vec2(3 * velx, vely));
	}
	else if (vely > -50 && vely < 50)
	{
		ball->getPhysicsBody()->setVelocity(Vec2(velx, 3 * vely));
	}
	
}
//���̰���
void Breakout::onKeyPressed(EventKeyboard::KeyCode code, Event* event) {
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
//���̷ſ�
 void Breakout::onKeyReleased(EventKeyboard::KeyCode code, Event* event) {
	 player->getPhysicsBody()->setVelocity(Vec2(0,0));
}
 

Breakout * Breakout::create(PhysicsWorld * world) {
	Breakout* pRet = new(std::nothrow) Breakout();
	if (pRet && pRet->init(world)) {
		pRet->autorelease();
		return pRet;
	}
	delete pRet;
	pRet = NULL;
	return NULL;
}