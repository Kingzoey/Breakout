#ifndef __GAMEOVERSCENE_H__
#define __GAMEOVERSCENE_H__
#include <string>
#include <vector>
#include "ui/CocosGUI.h"
#include "network/HttpClient.h"

using std::vector;
using std::string;
using namespace cocos2d::ui;
using namespace cocos2d::network;
#include "cocos2d.h"
USING_NS_CC;
class GameOverScene : public cocos2d::Layer
{
public:	
	static cocos2d::Scene* createScene();
	virtual bool init();
	void menuCloseCallback(Ref* pSender);
	void BackMenuCallback(cocos2d::Ref* pSender);
	CREATE_FUNC(GameOverScene);
private:
//
	float visibleHeight;
	float visibleWidth;
	TextField * textField;
};

#endif // __HELLOWORLD_SCENE_H__
