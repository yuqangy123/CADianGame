//
//  inviteShareTipsLayer.cpp
//  CDianGame
//
//  Created by 余强 on 16/3/23.
//
//

#include "inviteShareTipsLayer.hpp"
#include "com/microDtata.h"
#include "GameManager.hpp"
#include "display.hpp"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <jni.h>
#include "platform/android/jni/JniHelper.h"
#endif

USING_NS_CC;
float stime = 0.3f;

inviteShareTipsLayer *inviteShareTipsLayer::create()
{
    inviteShareTipsLayer *ret = new (std::nothrow) inviteShareTipsLayer();
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
void inviteShareTipsLayer::initEx()
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
    
    auto* csbNode = CSLoader::createNode("InviteShareLayerUI.csb");
    csbNode->setTag(1234);
    csbNode->setPosition(CDisplay::getInstance()->center);
    csbNode->setAnchorPoint(Vec2(0.5, 0.5));
    addChild(csbNode);
    
    
    auto* dialogBG = csbNode->getChildByName("dialogBG");
    m_Panel_enjoy = dynamic_cast<cocos2d::ui::Widget*>(dialogBG->getChildByName("Panel_enjoy"));
    m_Panel_yes = dynamic_cast<cocos2d::ui::Widget*>(dialogBG->getChildByName("Panel_enjoy_yes"));
    m_Panel_no = dynamic_cast<cocos2d::ui::Widget*>(dialogBG->getChildByName("Panel_enjoy_no"));
    m_Panel_enjoy->setVisible(false);
    m_Panel_yes->setVisible(false);
    m_Panel_no->setVisible(false);
    
    auto* yesEnjoyBtn = dynamic_cast<ui::Button*>(m_Panel_enjoy->getChildByName("yesBtn"));
    yesEnjoyBtn->addTouchEventListener(this, ui::SEL_TouchEvent(&inviteShareTipsLayer::yesEnjoyTouchCallback));
    
    auto* noEnjoyBtn = dynamic_cast<ui::Button*>(m_Panel_enjoy->getChildByName("noBtn"));
    noEnjoyBtn->addTouchEventListener(this, ui::SEL_TouchEvent(&inviteShareTipsLayer::noEnjoyTouchCallback));
    
    auto* yesYesBtn = dynamic_cast<ui::Button*>(m_Panel_yes->getChildByName("yesBtn"));
    yesYesBtn->addTouchEventListener(this, ui::SEL_TouchEvent(&inviteShareTipsLayer::yesYesTouchCallback));
    
    auto* noYesBtn = dynamic_cast<ui::Button*>(m_Panel_yes->getChildByName("noBtn"));
    noYesBtn->addTouchEventListener(this, ui::SEL_TouchEvent(&inviteShareTipsLayer::noYesTouchCallback));
    
    auto* yesNoBtn = dynamic_cast<ui::Button*>(m_Panel_no->getChildByName("yesBtn"));
    yesNoBtn->addTouchEventListener(this, ui::SEL_TouchEvent(&inviteShareTipsLayer::yesNoTouchCallback));
    
    auto* noNoBtn = dynamic_cast<ui::Button*>(m_Panel_no->getChildByName("noBtn"));
    noNoBtn->addTouchEventListener(this, ui::SEL_TouchEvent(&inviteShareTipsLayer::noNoTouchCallback));
    
    
    
    csbNode->setScale(0.0);
    csbNode->runAction(CCEaseSineOut::create(ScaleTo::create(stime, 1.0)));
    m_Panel_enjoy->setVisible(true);
}


void inviteShareTipsLayer::showYesCallback(Node* tp)
{
    CCFiniteTimeAction *action2 = cocos2d::Sequence::create(
                                                            CCEaseSineOut::create(ScaleTo::create(stime, 1.0)),
                                                            NULL);
    auto* csbNode = this->getChildByTag(1234)->runAction(action2);
    
    m_Panel_enjoy->setVisible(false);
    m_Panel_yes->setVisible(true);
}

void inviteShareTipsLayer::showNoCallback(Node* tp)
{
    CCFiniteTimeAction *action2 = cocos2d::Sequence::create(
                                                            CCEaseSineOut::create(ScaleTo::create(stime, 1.0)),
                                                            NULL);
    auto* csbNode = this->getChildByTag(1234)->runAction(action2);
    
    m_Panel_enjoy->setVisible(false);
    m_Panel_no->setVisible(true);
}

void inviteShareTipsLayer::yesEnjoyTouchCallback(Ref* obj, ui::TouchEventType tp)
{
    if (tp != ui::TouchEventType::TOUCH_EVENT_ENDED )return;
    
    CCFiniteTimeAction *action2 = cocos2d::Sequence::create(
                                                            CCEaseSineOut::create(ScaleTo::create(stime, 0.0)),
                                                            CCCallFuncN::create(this, callfuncN_selector(inviteShareTipsLayer::showYesCallback)),
                                                            NULL);
    this->getChildByTag(1234)->runAction(action2);
}

void inviteShareTipsLayer::noEnjoyTouchCallback(Ref* obj, ui::TouchEventType tp)
{
    if (tp != ui::TouchEventType::TOUCH_EVENT_ENDED )return;
    
    CCFiniteTimeAction *action2 = cocos2d::Sequence::create(
                                                            CCEaseSineOut::create(ScaleTo::create(stime, 0.0)),
                                                            CCCallFuncN::create(this, callfuncN_selector(inviteShareTipsLayer::showNoCallback)),
                                                            NULL);
    this->getChildByTag(1234)->runAction(action2);
}


void inviteShareTipsLayer::yesYesTouchCallback(Ref* obj, ui::TouchEventType tp)
{
    if (tp != ui::TouchEventType::TOUCH_EVENT_ENDED )return;
    //google商店评价
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo info;
    bool ret = JniHelper::getStaticMethodInfo(info,"org/cocos2dx/cpp/AppActivity","openURL_googleEval","()V");
    jobject jobj;
    if(ret)
    {
        //log("call void getObj() succeed");
        info.env->CallStaticVoidMethod(info.classID,info.methodID);
    }
#endif

    destory(NULL);
}

void inviteShareTipsLayer::noYesTouchCallback(Ref* obj, ui::TouchEventType tp)
{
    if (tp != ui::TouchEventType::TOUCH_EVENT_ENDED )return;
    
    CCFiniteTimeAction *action2 = cocos2d::Sequence::create(
                                                            CCEaseSineOut::create(ScaleTo::create(stime, 0.0)),
                                                            CCCallFuncN::create(this, callfuncN_selector(inviteShareTipsLayer::destory)),
                                                            NULL);
    this->getChildByTag(1234)->runAction(action2);
}

void inviteShareTipsLayer::yesNoTouchCallback(Ref* obj, ui::TouchEventType tp)
{
    if (tp != ui::TouchEventType::TOUCH_EVENT_ENDED )return;
    //fb评论
    
    CCFiniteTimeAction *action2 = cocos2d::Sequence::create(
                                                            CCEaseSineOut::create(ScaleTo::create(stime, 0.0)),
                                                            CCCallFuncN::create(this, callfuncN_selector(inviteShareTipsLayer::destory)),
                                                            NULL);
    this->getChildByTag(1234)->runAction(action2);
}

void inviteShareTipsLayer::noNoTouchCallback(Ref* obj, ui::TouchEventType tp)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo info;
    bool ret = JniHelper::getStaticMethodInfo(info,"org/cocos2dx/cpp/AppActivity","openURL_facebookEval","()V");
    jobject jobj;
    if(ret)
    {
        //log("call void getObj() succeed");
        info.env->CallStaticVoidMethod(info.classID,info.methodID);
    }
#endif
    
    destory(NULL);
}







void inviteShareTipsLayer::destory(Node* nd)
{
    this->removeFromParentAndCleanup(true);
}

