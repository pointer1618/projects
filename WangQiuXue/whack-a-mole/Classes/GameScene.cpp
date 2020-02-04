#include "GameScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "AudioEngine.h"
USING_NS_CC;
using namespace cocos2d::experimental;
using namespace cocostudio::timeline;

Scene* GameScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
	auto layer = GameScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool GameScene::init()
{
    /**  you can create scene with following comment code instead of using csb file.
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    
    auto label = Label::createWithTTF("Hello World", "fonts/Marker Felt.ttf", 24);
    
    // position the label on the center of the screen
    label->setPosition(Vec2(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(label, 1);

    // add "HelloWorld" splash screen"
    auto sprite = Sprite::create("HelloWorld.png");

    // position the sprite on the center of the screen
    sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

    // add the sprite as a child to this layer
    this->addChild(sprite, 0);
    **/
    
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
	AudioEngine::preload("backmusic.mp3");
	Size size = Director::getInstance()->getWinSize();
	auto back = Sprite::create("whack-a-mole/back.png");
	back->setPosition(size / 2);
	addChild(back);
		
	auto title = Sprite::create("whack-a-mole/icon/title.png");
	title->setRotation(25);
	addChild(title);
	title->setPosition(800, 520);
	//背景音乐
	mid = AudioEngine::play2d("backmusic.mp3");
	auto b1 = MenuItemImage::create("whack-a-mole/icon/49.png", "whack-a-mole/icon/48.png", [this](Ref* r){
		AudioEngine::stopAll();
		Director::getInstance()->popScene();	//返回主菜单
	});
	auto b2 = MenuItemImage::create("whack-a-mole/icon/45_1_2.png", "whack-a-mole/icon/45_1_1.png", [this](Ref* r){
		//重新开始
		AudioEngine::stopAll();
		Scene* newScene = GameScene::createScene();
		//场景从中间到四周消失同时另一场景出现
		Director::getInstance()->replaceScene(TransitionProgressInOut::create(1, newScene)); 	
	});
	auto pauseItem = MenuItemImage::create("whack-a-mole/icon/13.png", "whack-a-mole/icon/13.png");//按下暂停
	auto resumeItem = MenuItemImage::create("whack-a-mole/icon/12.png", "whack-a-mole/icon/12.png");//按下恢复
	//组合按钮控制场景暂停与恢复
	auto setPauseResume = MenuItemToggle::createWithCallback([](Ref* r){
	
	},pauseItem, resumeItem, NULL);

	auto openMusicItem = MenuItemImage::create("whack-a-mole/icon/10.png", "whack-a-mole/icon/10.png");
	auto closeMusicItem = MenuItemImage::create("whack-a-mole/icon/9.png", "whack-a-mole/icon/9.png");
	//组合按钮控制声音开关
	auto setMusicMenu = MenuItemToggle::createWithCallback([=](Ref* r){
		auto state = AudioEngine::getState(mid);
		switch (state)
		{
			case cocos2d::experimental::AudioEngine::AudioState::ERROR:
				mid = AudioEngine::play2d("backmusic.mp3");
				break;
			case cocos2d::experimental::AudioEngine::AudioState::INITIALZING:
				mid = AudioEngine::play2d("backmusic.mp3");
				break;
			case cocos2d::experimental::AudioEngine::AudioState::PLAYING:
				AudioEngine::pause(mid);
				break;
			case cocos2d::experimental::AudioEngine::AudioState::PAUSED:
				AudioEngine::resume(mid);
				break;
			default:
				break;
		}
	},openMusicItem, closeMusicItem, NULL);
	
	b1->setScale(0.75);
	b2->setScale(0.75);
	setPauseResume->setScale(0.8);
	auto m_button = Menu::create(b1, b2, setPauseResume, setMusicMenu, 0);		//创建菜单
	m_button->alignItemsHorizontally();
	m_button->setPosition(200, 600);
	addChild(m_button);
	auto b5 = Sprite::create("whack-a-mole/icon/11.png");
	b5->setPosition(80,540);
	b5->setScale(1.5);
	addChild(b5);

//鼠标-小锤子
	Sprite* mouse = Sprite::create("whack-a-mole/hammer.png");
	mouse->setAnchorPoint(Vec2(0.23437,0.45312));
	mouse->setZOrder(3);

	addChild(mouse);
	ShowCursor(false);
	auto em = EventListenerMouse::create();
	em->onMouseDown = [=](EventMouse* event){
		mouse->setScale(1.2);
	};
	em->onMouseUp = [=](EventMouse* event){
		mouse->setScale(1);
	};
	em->onMouseMove = [=](EventMouse* event){
		auto dr = Director::getInstance()->getWinSize();
		mouse->setPosition(event->getLocation().x, dr.height - event->getLocation().y);
	};
	getEventDispatcher()->addEventListenerWithSceneGraphPriority(em, mouse);
	srand(time(0));
//放置老鼠0-19的位置 精灵鼠0-3 地鼠4-7 地鼠王8-11 龙猫12-15 流氓兔16-19 
//洞0	1	2	3	4	5	6	7	8
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("MousePlist.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("HolePlist.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("IconPlist.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("StarPlist.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("TimePlist.plist");
	auto s = Sprite::create();
	this->hpos.push_back(Vec2(230, 381));
	this->hpos.push_back(Vec2(451, 384));
	this->hpos.push_back(Vec2(696, 380));
	//
	auto ma = Animation::create();
	for (int i = 0; i <= 4; i++)
	{
		for (int j = 1; i <= 4; i++)
		{
			char buff[50];
			sprintf(buff, "mouse/%d%d.png", i, j);
			ma->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(buff));
		}
		ma->setDelayPerUnit(1);
		ma->setLoops(4);
		int k = rand() % 2;
		s->setAnchorPoint(Vec2(0.5, 0));
		s->setPosition(hpos[k]);
		s->runAction(Animate::create(ma));
		addChild(s);
	}

	//auto m0 = Sprite::create("whack-a-mole/mouse/3.png");
	//m0->setPosition(230, 381);
	//addChild(m0);
	//m0->setAnchorPoint(Vec2(0.5, 0));
	//auto m1 = Sprite::create("whack-a-mole/mouse/7.png");
	//m1->setPosition(451, 384);
	//addChild(m1);
	//m1->setAnchorPoint(Vec2(0.5, 0));
	//auto m2 = Sprite::create("whack-a-mole/mouse/11.png");
	//m2->setPosition(696, 380);
	//addChild(m2);
	//m2->setAnchorPoint(Vec2(0.5, 0));
	//auto m3 = Sprite::create("whack-a-mole/mouse/15.png");
	//m3->setPosition(195, 265);
	//addChild(m3);
	//m3->setAnchorPoint(Vec2(0.5, 0));
	//auto m4 = Sprite::create("whack-a-mole/mouse/19.png");
	//m4->setPosition(461, 265);
	//addChild(m4);
	//m4->setAnchorPoint(Vec2(0.5, 0));
	//auto m5 = Sprite::create("whack-a-mole/mouse/12.png");
	//m5->setPosition(695, 265);
	//addChild(m5);
	//m5->setAnchorPoint(Vec2(0.5, 0));
	//auto m6 = Sprite::create("whack-a-mole/mouse/14.png");
	//m6->setPosition(188, 135);
	//addChild(m6);
	//m6->setAnchorPoint(Vec2(0.5, 0));
	//auto m7 = Sprite::create("whack-a-mole/mouse/8.png");
	//m7->setPosition(457, 131);
	//addChild(m7);
	//m7->setAnchorPoint(Vec2(0.5, 0));
	//auto m8 = Sprite::create("whack-a-mole/mouse/5.png");
	//m8->setPosition(723, 125);
	//addChild(m8);
	//m8->setAnchorPoint(Vec2(0.5, 0));


    return true;
}
