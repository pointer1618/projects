#ifndef __GameScene_SCENE_H__
#define __GameScene_SCENE_H__

#include "cocos2d.h"

class GameScene : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    // implement the "static create()" method manually
	CREATE_FUNC(GameScene);

	int mid;
	
	cocos2d::Vector <cocos2d::Sprite*>molesVector;
	std::vector <cocos2d::Vec2> hpos;
	
};

#endif // __HELLOWORLD_SCENE_H__
