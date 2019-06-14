#ifndef __GAMEOVERSCENE_H__
#define __GAMEOVERSCENE_H__
 
#include "ui/CocosGUI.h"
#include "network/HttpClient.h"
#include <string>
#include <vector>
using std::vector;
using std::string;
using namespace cocos2d::ui;
using namespace cocos2d::network;
#include "cocos2d.h"
USING_NS_CC;
class GameOverScene : public cocos2d::Layer
{
public:
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene* createScene();

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();
	void menuCloseCallback(Ref* pSender);
	// implement the "static create()" method manually
	CREATE_FUNC(GameOverScene);

private:
//创建六个人
	Sprite* mouse;
	Sprite* mouse1;
	Sprite* mouse2;
	Sprite* mouse3;
	Sprite* mouse4;
	Sprite* mouse5;
	float visibleHeight;
	float visibleWidth;
	TextField * textField;
};

#endif // __HELLOWORLD_SCENE_H__
