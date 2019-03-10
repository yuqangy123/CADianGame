//
//  LightFactory.cpp
//  CDianGame
//
//  Created by 余强 on 16/3/23.
//
//

#include "LightFactory.hpp"
#include "FightLayer.h"
#include "B2Sprite.hpp"
#include "com/microDtata.h"
#include "display.hpp"
#include "lightSprite.h"


#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
USING_NS_CC;

LightFactory::LightFactory()
{
    m_fightLayer = nullptr;
    m_serialTag = 1;
    m_lastLight = nullptr;
}

LightFactory::~LightFactory()
{
    
}

LightFactory *LightFactory::create()
{
    LightFactory *ret = new (std::nothrow) LightFactory();
    if (ret)
    {
        ret->autorelease();
        return ret;
    }
    else
    {
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
}

bool LightFactory::init(FightLayer* fightLayer)
{
    m_fightLayer = fightLayer;
	return true;
}

void LightFactory::destoryBody(B2Sprite *spr)
{
    if (nullptr == spr)
        return;
    
    m_fightLayer->destoryBody(spr);
    spr->removeFromParent();
}

B2Sprite* LightFactory::addLightBody(float x, float y, int body)
{
    lightSprite* spr = new lightSprite();
    
    char uiName[256] = {0};
    sprintf(uiName, "ui/light/%03d.png", body);
    char id[8] = {0};
    sprintf(id, "%03d", body);
    
    spr->autorelease();
    spr->initWithFile(uiName);
	spr->setImageName(uiName);
    spr->setLightID(body);
    spr->setFilterBody(m_fightLayer->getFilterBody());
    spr->setFightLayer(m_fightLayer);
    spr->setLocalZOrder(1);
    spr->setPositionEx(x, y);
    spr->initEx(++m_serialTag);
    m_fightLayer->addStaticbody(spr, BodyType::light, BodyUserData::create(spr, BodyType::light, m_serialTag));
    
    
    m_addBodyList.push_back(spr);
    return spr;
}

void LightFactory::contactLight(int tag)
{
    for(auto itr = m_addBodyList.begin(); itr != m_addBodyList.end(); ++itr)
    {
        if( tag == (*itr)->getTag() )
        {
            eatLight(*itr);
            break;
        }
    }
}

void LightFactory::eatLight(B2Sprite* spr)
{
    if(nullptr == spr)
        return;
    
    lightSprite* lightSpr = (lightSprite*) spr;
    if(!lightSpr->isEatingNormal())
        lightSpr->eat();
    
    
}

void LightFactory::stopAllLight()
{
    this->stopAllActions();
    for(auto itr = m_addBodyList.begin(); itr != m_addBodyList.end(); ++itr)
    {
        (*itr)->unscheduleUpdate();
        (*itr)->stopAllActions();
    }
}

void LightFactory::removeAllLight(bool fadeOut)
{
    for(auto itr = m_addBodyList.begin(); itr != m_addBodyList.end(); ++itr)
    {
        if (fadeOut)
        {
            lightSprite* light = (lightSprite*)(*itr);
            light->delSelf();
        }
        else
        {
            destoryBody((*itr));
        }
    }
    m_addBodyList.clear();
}

void LightFactory::delLight(int nID)
{
    for(auto itr = m_addBodyList.begin(); itr != m_addBodyList.end(); ++itr)
    {
        if ( nID == (*itr)->getTag() )
        {
            destoryBody((*itr));
            m_addBodyList.erase(itr);
            break;
        }
    }
}

bool LightFactory::haveLight(B2Sprite* spr)
{
    for(auto itr = m_addBodyList.begin(); itr != m_addBodyList.end(); ++itr)
    {
        if(spr == *itr)
            return true;
    }
    return false;
}


