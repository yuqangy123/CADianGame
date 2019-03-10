//
//  LightFilterTouchLayer.cpp
//  CDianGame
//
//  Created by 余强 on 16/3/23.
//
//

#include "LightFilterTouchLayer.hpp"
#include "com/microDtata.h"
#include "GameManager.hpp"
#include "display.hpp"
#include "com/MessageQueue.h"
USING_NS_CC;
Vec2 touchPos;

LightFilterTouchLayer *LightFilterTouchLayer::create()
{
    LightFilterTouchLayer *ret = new (std::nothrow) LightFilterTouchLayer();
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

void LightFilterTouchLayer::setEnableTouch(bool l)
{
    if(l)
    {
        this->setTouchEnabled(true);
        
        m_touchListener = EventListenerTouchOneByOne::create();
        m_touchListener->setSwallowTouches(false);
        
        m_touchListener->onTouchBegan = CC_CALLBACK_2(LightFilterTouchLayer::onTouchBegan, this);
        m_touchListener->onTouchMoved = CC_CALLBACK_2(LightFilterTouchLayer::onTouchMoved, this);
        m_touchListener->onTouchEnded = CC_CALLBACK_2(LightFilterTouchLayer::onTouchEnded, this);
        m_touchListener->onTouchCancelled = CC_CALLBACK_2(LightFilterTouchLayer::onTouchCancelled, this);
        
        _eventDispatcher->addEventListenerWithSceneGraphPriority(m_touchListener, this);
        _touchListener = m_touchListener;
        
        m_touchIndexCnt = 0;
    }
    else
    {
        this->setTouchEnabled(false);
        
        _eventDispatcher->removeEventListener(m_touchListener);
        
        m_touchIndexCnt = 0;
    }
}

void LightFilterTouchLayer::initEx()
{
}

void LightFilterTouchLayer::showNewGuide()
{
    cocos2d::Point pt = CDisplay::getInstance()->bottomCenter;
    m_newGuideSpr = Sprite::create("ui/zhiying02.png");
    m_newGuideSpr->setAnchorPoint(Vec2(0.5, 0.5));
    m_newGuideSpr->setPosition(pt.x, pt.y+175);
    m_newGuideSpr->setScale(2);
    m_newGuideSpr->setLocalZOrder(3);
    addChild(m_newGuideSpr);
    
    float t2 = 0.8;
    auto action2 = cocos2d::Sequence::create(
                                             CCEaseSineInOut::create(MoveBy::create(t2, Vec2(0, 200))),
                                             CCEaseSineInOut::create(MoveBy::create(t2, Vec2(0, -200))),
                                             NULL);
    m_newGuideSpr->runAction(RepeatForever::create(action2));
}


void LightFilterTouchLayer::setLightFilter(CLightFilter* l)
{
    m_lightFilter = l;
    
}

bool LightFilterTouchLayer::onTouchBegan(Touch *touch, Event *event)
{
    if (gameManager->isPauseGame() || !gameManager->isPlaying() || m_touchIndexCnt > 0)
        return false;
    
    auto target = event->getCurrentTarget();
    touchPos = target->convertToNodeSpace(touch->getLocation());
    
    CCLOG("onTouchBegan\n");
    
    if(m_newGuideSpr)
    {
        m_newGuideSpr->removeFromParentAndCleanup(true);
        MessageQueue->SendMsg(MSG_ID_NEWPLAYER_GUIDE_TOUCHMOVE_NOTIFY, 0);
        m_newGuideSpr = NULL;
        m_newGuideTouch = true;
    };
    
    ++m_touchIndexCnt;
    return true;
}

void LightFilterTouchLayer::onTouchMoved(Touch *touch, Event *event)
{
    auto target = event->getCurrentTarget();
    Vec2 touchtmp = target->convertToNodeSpace(touch->getLocation());
    
    m_lightFilter->movePosition(touchtmp.x - touchPos.x, touchtmp.y - touchPos.y);
    
    touchPos.x = touchtmp.x;
    touchPos.y = touchtmp.y;
}

void LightFilterTouchLayer::onTouchEnded(Touch *touch, Event *event)
{
    m_lightFilter->unoffsetPosition();
    //this->runAction(CCMoveBy::create(5.0f,Vec2(0, -500)));
    
    --m_touchIndexCnt;
    
}

