//
//  exitgameTipsLayer.cpp
//  CDianGame
//
//  Created by 余强 on 16/3/23.
//
//

#include "facebookShareTipsLayer.hpp"
#include "com/microDtata.h"
#include "GameManager.hpp"
#include "display.hpp"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <jni.h>
#include "platform/android/jni/JniHelper.h"
#endif

USING_NS_CC;

facebookShareTipsLayer *facebookShareTipsLayer::create()
{
    facebookShareTipsLayer *ret = new (std::nothrow) facebookShareTipsLayer();
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
void facebookShareTipsLayer::initEx()
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
    
    auto* csbNode = CSLoader::createNode("FbShareLayerUI.csb");
    csbNode->setTag(1234);
    csbNode->setPosition(CDisplay::getInstance()->center);
    csbNode->setAnchorPoint(Vec2(0.5, 0.5));
    addChild(csbNode);
    
    
    auto* yesBtn = dynamic_cast<ui::Button*>(csbNode->getChildByName("dialogBG")->getChildByName("yesBtn"));
    yesBtn->addTouchEventListener(this, ui::SEL_TouchEvent(&facebookShareTipsLayer::yesTouchCallback));
    
    auto* noBtn = dynamic_cast<ui::Button*>(csbNode->getChildByName("dialogBG")->getChildByName("noBtn"));
    noBtn->addTouchEventListener(this, ui::SEL_TouchEvent(&facebookShareTipsLayer::noTouchCallback));
    
    setVisible(false);
    csbNode->setScale(0.0);
    
}

void facebookShareTipsLayer::show()
{
    setVisible(true);
    this->getChildByTag(1234)->runAction(CCEaseSineOut::create(ScaleTo::create(0.3, 1.0)));
}

void facebookShareTipsLayer::noTouchCallback(Ref* obj, ui::TouchEventType tp)
{
    if (tp != ui::TouchEventType::TOUCH_EVENT_ENDED)return;
    
    CCFiniteTimeAction *action2 = cocos2d::Sequence::create(
                                                            CCEaseSineOut::create(ScaleTo::create(0.3, 0.0)),
                                                            CCCallFuncN::create(this, callfuncN_selector(facebookShareTipsLayer::destory)),
                                                            NULL);
    auto* csbNode = this->getChildByTag(1234);
    csbNode->runAction(action2);
}

void facebookShareTipsLayer::yesTouchCallback(Ref* obj, ui::TouchEventType tp)
{
    if (tp != ui::TouchEventType::TOUCH_EVENT_ENDED)return;
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo info;
    bool ret = JniHelper::getStaticMethodInfo(info,"org/cocos2dx/cpp/AppActivity","facebookShare","()V");
    jobject jobj;
    if(ret)
    {
        //log("call void getObj() succeed");
        info.env->CallStaticVoidMethod(info.classID,info.methodID);
    }
#endif

    destory(NULL);
}

void facebookShareTipsLayer::destory(Node* nd)
{
    this->removeFromParentAndCleanup(true);
}

