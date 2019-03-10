//
//  exitgameTipsLayer.cpp
//  CDianGame
//
//  Created by 余强 on 16/3/23.
//
//

#include "exitgameTipsLayer.hpp"
#include "com/microDtata.h"
#include "GameManager.hpp"
#include "display.hpp"


USING_NS_CC;

exitgameTipsLayer *exitgameTipsLayer::create()
{
    exitgameTipsLayer *ret = new (std::nothrow) exitgameTipsLayer();
    if (ret && ret->init())
    {
        ret->autorelease();
        ret->initEx();
        return ret;
    }
    else
    {
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
}
void exitgameTipsLayer::initEx()
{
    
    LayerColor* l = LayerColor::create(Color4B(0,0,0,0));
    l->setLocalZOrder(-1);
    addChild(l);
    auto callback = [](Touch * ,Event *)
    {
        return true;
    };
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = callback;
    listener->setSwallowTouches(true);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener,this);
    
    
    
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    auto* csbNode = CSLoader::createNode("exitDialogLayerUI.csb");
    csbNode->setTag(1234);
    csbNode->setPosition(CDisplay::getInstance()->center);
    csbNode->setAnchorPoint(Vec2(0.5, 0.5));
    addChild(csbNode);
    
    
    auto* yesBtn = dynamic_cast<ui::Button*>(csbNode->getChildByName("dialogBG")->getChildByName("yesBtn"));
    yesBtn->addTouchEventListener(this, ui::SEL_TouchEvent(&exitgameTipsLayer::yesTouchCallback));
    
    auto* noBtn = dynamic_cast<ui::Button*>(csbNode->getChildByName("dialogBG")->getChildByName("noBtn"));
    noBtn->addTouchEventListener(this, ui::SEL_TouchEvent(&exitgameTipsLayer::noTouchCallback));
    
    csbNode->setScale(0.0);
    
    csbNode->runAction(ScaleTo::create(0.3, 1.0));
}


void exitgameTipsLayer::noTouchCallback(Ref* obj, ui::TouchEventType tp)
{
    if (tp != ui::TouchEventType::TOUCH_EVENT_ENDED)return;
    
    CCFiniteTimeAction *action2 = cocos2d::Sequence::create(
                                                            CCEaseSineOut::create(ScaleTo::create(0.3, 0.0)),
                                                            CCCallFuncN::create(this, callfuncN_selector(exitgameTipsLayer::destory)),
                                                            NULL);
    auto* csbNode = this->getChildByTag(1234);
    csbNode->runAction(action2);
}

void exitgameTipsLayer::yesTouchCallback(Ref* obj, ui::TouchEventType tp)
{
    if (tp != ui::TouchEventType::TOUCH_EVENT_ENDED)return;
    
    Director::getInstance()->end();
}

void exitgameTipsLayer::destory(Node* nd)
{
    this->removeFromParentAndCleanup(true);
}

