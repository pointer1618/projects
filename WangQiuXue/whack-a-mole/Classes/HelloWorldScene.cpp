#include "HelloWorldScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "GameScene.h"
#include "AudioEngine.h"
USING_NS_CC;
using namespace cocos2d::experimental;
using namespace cocostudio::timeline;

Scene* HelloWorld::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = HelloWorld::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
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

	if (!Layer::init())
	{
		return false;
	}
	auto d = Director::getInstance();
	srand(time(0));
	//Color4B 蓝紫色
	Color4B c = Color4B::Color4B(128, 128, 255, 255);//四个参数—红 绿 蓝 透明度
	//使用xml文件作为汉字查询的字典
	//auto dy = Dictionary::createWithContentsOfFile("Dictionary_Strings.xml");

//地鼠

	//背景及适应屏幕
	auto back = Sprite::create("whack-a-mole/board.png");
	back->setZOrder(-2);
	Size size = Director::getInstance()->getWinSize();
	Size bsize = back->getContentSize();
	back->setPosition(size.width / 2, size.height / 2);
	back->setScale(size.width / bsize.width, size.height / bsize.height);
	addChild(back);

	//打地鼠标题
	auto title = Sprite::create("whack-a-mole/icon/title.png");
	addChild(title);
	title->setPosition(700, 530);

	

	//鼠标图片
	Sprite* mouse = Sprite::create("mouse.png");
	mouse->setAnchorPoint(Vec2(0, 1));
	mouse->setScale(0.2);
	mouse->setZOrder(3);
	addChild(mouse);
	ShowCursor(false);

	auto b1 = MenuItemImage::create("whack-a-mole/icon/44.png", "whack-a-mole/icon/43.png", [this](Ref* r){
		//新游戏
		Scene* newScene = GameScene::createScene();
		auto t = TransitionFadeBL::create(1, newScene);
		Director::getInstance()->pushScene(t);
	});

	//创建说明且隐藏
	auto help = Sprite::create("whack-a-mole/help.png");
	help->setPosition(Director::getInstance()->getWinSize() / 2);
	help->setTag(5);
	help->setVisible(false);
	addChild(help);
	auto b2 = MenuItemImage::create("whack-a-mole/icon/42.png", "whack-a-mole/icon/41.png", [=](Ref* r){
		//游戏说明
		help->setVisible(true);
		help->setZOrder(2);
	});
	

	auto b3 = MenuItemImage::create("whack-a-mole/icon/40.png", "whack-a-mole/icon/39.png", [this](Ref* r){
		//设置
	});
	auto b4 = MenuItemImage::create("whack-a-mole/icon/47.png", "whack-a-mole/icon/46.png", [this](Ref* r){
		Director::getInstance()->popScene();	//退出
	});

	auto menu_b = Menu::create(b1, b2, b3, b4, 0);		//创建菜单，最后一个参数为0！
	menu_b->setZOrder(-1);
	menu_b->setPosition(700, 280);
	menu_b->alignItemsVertically();		//默认间距垂直排列
	addChild(menu_b);

	auto et = EventListenerTouchOneByOne::create();		//创建触摸事件监听器
	et->onTouchBegan = [=](cocos2d::Touch* t, cocos2d::Event* e){
		mouse->setScale(0.25);
		Node* is_help = getChildByTag(5);
		if (is_help && is_help->getBoundingBox().containsPoint(t->getLocation()))
		{
			
			help->setVisible(false);
			
			return true;
		}
		else
			et->setSwallowTouches(false);
			return false;
	};
	et->onTouchEnded = [=](cocos2d::Touch* t, cocos2d::Event* e){
		mouse->setScale(0.2);
	};
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(et, help);

	//创建鼠标事件监听器
	auto em = EventListenerMouse::create();
	em->onMouseMove = [=](EventMouse* event){
		auto dr = Director::getInstance()->getWinSize();
		mouse->setPosition(event->getLocation().x, dr.height - event->getLocation().y);
	};
	getEventDispatcher()->addEventListenerWithSceneGraphPriority(em, mouse);

	return true;
}


