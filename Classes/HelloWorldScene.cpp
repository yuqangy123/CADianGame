#include "HelloWorldScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "FightLayer.h"
#include "HomeLayer.hpp"
#include "SplashLayer.hpp"
#include "BackgroundLayer.hpp"
#include "GameManager.hpp"
#include "display.hpp"
#include "exitgameTipsLayer.hpp"

USING_NS_CC;

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
    if ( !Layer::init() )
    {
        return false;
    }
	FileUtils::getInstance()->addSearchPath("res");

    CDisplay::getInstance()->setDesignResolution();



#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    //set android back key callback
    auto listener = EventListenerKeyboard::create();
    listener->onKeyReleased = CC_CALLBACK_2(HelloWorld::onAndroidKeyReleased, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
#endif

	
    //Configuration *conf = Configuration::getInstance();
    //conf->setValue("cocos2d.x.display_fps", Value(false));
    
    
    
    gameManager->setRootLayer(this);
    
    CCFiniteTimeAction *action = cocos2d::Sequence::create(
                                                            CCDelayTime::create(0.1),
                                                            CCCallFuncN::create(this, callfuncN_selector(HelloWorld::delayCallback)),
                                                            NULL);
    runAction(action);
    
    /*
     cocos2d::GLProgram* shader = GLProgram::createWithFilenames("shader/lighthide.vsh", "shader/lighthide.fsh");
     shader->use();
     shader->setUniformsForBuiltins();
     spr->setGLProgram(shader);    
    
    spr = cocos2d::Sprite::create("044.png");
    spr->setPosition(400, 800);
    addChild(spr);
    
    cocos2d::GLProgram* shader = GLProgram::createWithFilenames("shader/lighthide.vsh", "shader/lighthide.fsh");
    
    auto state = GLProgramState::create(shader);
    spr->setGLProgramState(state);
    
    state->setUniformFloat("filterValue", m_filterV);
    
    schedule(schedule_selector(HelloWorld::checkDistorySchedule), .05f);
    
    //spr->setOpacity(0);
    */
    return true;
}


void HelloWorld::delayCallback(CCNode* nd)
{
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    SplashLayer *ret = SplashLayer::create();
    gameManager->getRootLayer()->addChild(ret);
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    SplashLayer::showHomelayer();
#endif
	
    return;
}

void HelloWorld::onAndroidKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
    switch(keyCode)
    {
            //监听返回键
        case EventKeyboard::KeyCode::KEY_ESCAPE:
        {
			if(NULL == getChildByTag(100))
			{
				exitgameTipsLayer* l = exitgameTipsLayer::create();
				l->setLocalZOrder(4);
				l->setPosition(CDisplay::getInstance()->leftBottom);
				l->setTag(100);
				addChild(l);
			}
            
        }break;
            //监听menu键
        case EventKeyboard::KeyCode::KEY_MENU:
            break;
    }
}







