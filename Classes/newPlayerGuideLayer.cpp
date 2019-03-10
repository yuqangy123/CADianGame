//
//  NewPlayerGuideLayer.cpp
//  CDianGame
//
//  Created by 余强 on 16/3/23.
//
//

#include "newPlayerGuideLayer.hpp"
#include "com/microDtata.h"
#include "GameManager.hpp"
#include "display.hpp"
USING_NS_CC;

NewPlayerGuideLayer *NewPlayerGuideLayer::create()
{
    NewPlayerGuideLayer *ret = new (std::nothrow) NewPlayerGuideLayer();
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
void NewPlayerGuideLayer::initEx()
{
    auto quan = Sprite::create("ui/zhiying01.png");
    quan->setAnchorPoint(Vec2(0.5, 0.5));
    //quan->setPosition(pt.x, pt.y+500);
    quan->setScale(2);
    addChild(quan);
    
    
    
    float t1 = 1.5;
    auto action1 = cocos2d::Sequence::create(
                                            ScaleTo::create(t1, 3.0),
                                            ScaleTo::create(t1, 2.0),
                                            NULL);
    quan->runAction(RepeatForever::create(action1));
    
    
    
    this->setTouchEnabled(true);
    m_touchListener = EventListenerTouchOneByOne::create();
    m_touchListener->setSwallowTouches(false);
    
    m_touchListener->onTouchBegan = CC_CALLBACK_2(NewPlayerGuideLayer::onTouchBegan, this);
    m_touchListener->onTouchMoved = CC_CALLBACK_2(NewPlayerGuideLayer::onTouchMoved, this);
    m_touchListener->onTouchEnded = CC_CALLBACK_2(NewPlayerGuideLayer::onTouchEnded, this);
    m_touchListener->onTouchCancelled = CC_CALLBACK_2(NewPlayerGuideLayer::onTouchCancelled, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(m_touchListener, this);
    _touchListener = m_touchListener;
}


bool NewPlayerGuideLayer::onTouchBegan(Touch *touch, Event *event)
{
    auto target = event->getCurrentTarget();
    //touchPos = target->convertToNodeSpace(touch->getLocation());
    
    return true;
}

void NewPlayerGuideLayer::onTouchMoved(Touch *touch, Event *event)
{
    auto target = event->getCurrentTarget();
    Vec2 touchtmp = target->convertToNodeSpace(touch->getLocation());
    
}

void NewPlayerGuideLayer::onTouchEnded(Touch *touch, Event *event)
{
    //this->runAction(CCMoveBy::create(5.0f,Vec2(0, -500)));
}

