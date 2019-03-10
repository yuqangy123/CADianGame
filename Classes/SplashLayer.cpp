//
//  SplashLayer.cpp
//  CDianGame
//
//  Created by 余强 on 16/3/26.
//
//

#include "SplashLayer.hpp"
#include "display.hpp"
#include "FightLayer.h"
#include "dataManager.hpp"
#include "GameManager.hpp"
#include "AudioManager.hpp"
#include "com/MessageQueue.h"

#include "HomeLayer.hpp"
#include "BackgroundLayer.hpp"

USING_NS_CC;


SplashLayer *SplashLayer::create()
{
    SplashLayer *ret = new (std::nothrow)SplashLayer();
    if (ret && ret->init())
    {
        ret->initEx();
        ret->autorelease();
        return ret;
    }
    else
    {
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
}



bool SplashLayer::initEx()
{
	//bg = Sprite::create("ui/bg.png");
    bg = LayerColor::create(Color4B(32,50,69,255));
	bg->setAnchorPoint(Vec2(0, 0));
	bg->setPosition(CDisplay::getInstance()->leftBottom);
    bg->setScale(4.0);
	addChild(bg);
	
	logo = Sprite::create("ui/logo.png");
	logo->setAnchorPoint(Vec2(0.5, 0.5));
	logo->setPosition(CDisplay::getInstance()->center);
	addChild(logo);
	
	float time = 0.8f;
    float delaytime = 1.0f;
	CCFiniteTimeAction *action = cocos2d::Sequence::create(
                                                                CCFadeIn::create(time),
																CCDelayTime::create(delaytime),
																CCCallFunc::create(this, callfunc_selector(SplashLayer::comebackCallback)),
																NULL
																);
    
    bg->runAction(action);
    bg->setOpacity(0.0);
	
	logo->runAction(CCFadeIn::create(time));
    logo->setOpacity(0.0);
	
    return true;
}

void SplashLayer::comebackCallback()
{
    float time = 0.6f;
	CCFiniteTimeAction *action = cocos2d::Sequence::create(
                                                                CCFadeOut::create(time),
																CCCallFunc::create(this, callfunc_selector(SplashLayer::homelayerCallback)),
																NULL);
    bg->runAction(action);
	
	logo->runAction(CCFadeOut::create(time));
}

void SplashLayer::homelayerCallback()
{
    showHomelayer();
}

void SplashLayer::showHomelayer()
{
    auto* csbBg = BackgroundLayer::create();
    csbBg->setPosition(CDisplay::getInstance()->leftBottom);
    gameManager->getRootLayer()->addChild(csbBg);
    gameManager->setBgLayer(csbBg);
    csbBg->setLocalZOrder(0);
    
    HomeLayer* homelayer = HomeLayer::create();
    homelayer->setPosition(CDisplay::getInstance()->leftBottom);
    homelayer->initEx();
    gameManager->getRootLayer()->addChild(homelayer);
    gameManager->setHomeLayer(homelayer);
    homelayer->setLocalZOrder(1);
}

