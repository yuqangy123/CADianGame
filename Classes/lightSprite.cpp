//
//  lightSprite.cpp
//  lightSprite
//
//  Created by 余强 on 16/3/21.
//
//

#ifndef lightSprite_cpp
#define lightSprite_cpp

#include "lightSprite.h"
#include "com/microDtata.h"
#include "lightSprite.h"
#include "LightFilter.hpp"
#include "FightLayer.h"
#include "com/MessageQueue.h"
#include "ColorBeautifulShader.hpp"
#include "AudioManager.hpp"
#include "display.hpp"

USING_NS_CC;

lightSprite::lightSprite()
{
}
lightSprite::~lightSprite()
{
    colorBeautifulShader->delBode(this);
}

void lightSprite::initEx(int tag)
{
    colorBeautifulShader->addLightBode(this);
    
    setTag(tag);
    
    this->scheduleUpdateWithPriority(-tag);
    
    m_fromTop = 0;
}

void lightSprite::eat()
{
    if (m_beEatting)
        return;
    runAction(FadeOut::create(2.0));
    //m_pRoadPath
    //CardinalSplineBy* action = CardinalSplineBy::create();
    
    audioManager->setEffectsVolume(1.0);
    audioManager->setBackgroundMusicVolume(0.8);
    
	
    // Error is half the height of the light
    float topPosY = getPositionY();
    float filterPosY = m_filter->getPosition().y + CDisplay::getInstance()->m_fLineOffsetY;
    
    m_fromTop = topPosY <= filterPosY ? 1 : 0;
    //auto state = this->getGLProgramState();
    //state->setUniformInt("fromTop", m_fromTop);
    
    m_beEatting = true;
    m_contacted = true;
}

void lightSprite::beEated()
{
    if(!m_isLife)
        return;
    
    m_fightlayer->filterLightFinsih(1);
    
    this->unscheduleUpdate();
    audioManager->setEffectsVolume(0.0);
    audioManager->setBackgroundMusicVolume(1.0);
    this->setVisible(false);
    m_beEatting = false;
    m_isLife = false;
}


void lightSprite::beDead(int type)
{
    if(!m_isLife)
        return;
    
    this->unscheduleUpdate();
    MessageQueue->SendMsg(MSG_ID_GAME_LOSE_NOTIFY, (void*)NULL);
    m_beEatting = false;
    m_isLife = false;
    
    switch(type)
    {
        case lightDead::ballContact:
        {
            showDeadEffect();
            break;
        }
        case lightDead::screenBottom:
        {
            m_fightlayer->showDeadEffect(this->getPositionX());
            break;
        }
    }

	update(0.0);
}

void lightSprite::update(float f)
{
    
    if (m_beEatting)
    {
        cocos2d::Vec2 fpos = m_filter->getPosition();
		fpos.y += CDisplay::getInstance()->m_fLineOffsetY;
        float posY = this->getPositionY() - this->getContentSize().height/2;
        float myY = m_fromTop == 0 ? posY : (posY + getContentSize().height);
        m_filterV = ( m_fromTop == 0 ? (fpos.y - myY) : (myY - fpos.y) )/getContentSize().height;//消失点上移offsetY
        m_filterV = MAX( 0.0, MIN(1.0, m_filterV));
        
        auto state = this->getGLProgramState();
        state->setUniformFloat("filterValue", m_filterV);
        
        //CCLOG("m_filterV=%f\n", m_filterV);
        
        if(m_filterV >= 1.0f)
        {
            beEated();
        }
        else if(m_filterV <= 0.0f)
        {
            float layerPosY = m_fightlayer->getBox2dLayerPos();
            if(posY < -layerPosY)
            {
                beDead(lightDead::ballContact);
            }
        };
    }
    else
    {
        float myY = this->getPositionY() - this->getContentSize().height/2;
        float layerPosY = m_fightlayer->getBox2dLayerPos();
        if(myY < -layerPosY)
        {
            beDead(lightDead::screenBottom);
        }
    }
}

bool lightSprite::isEatingNormal(){
    if(!m_beEatting)
        return false;
    
    //帧率误差
    //CCLOG("isEatingNormal.m_filterV=%f\n", m_filterV);
    if(m_filterV <= 0.0 || m_filterV >= 0.90)
        return false;
    return true;
}

void lightSprite::setFilterBody(CLightFilter* f)
{
    m_filter = f;
}

void lightSprite::showDeadEffect()
{
    if(!m_isLife)return;
    m_isLife = false;
    
    Sprite* spr = Sprite::create();
    spr->initWithFile(m_imageName);
    spr->setPosition(spr->getContentSize().width/2,spr->getContentSize().height/2);
    this->addChild(spr);
    spr->setOpacity(128);
    
    float t = 0.5f;
    CCFiniteTimeAction *action1 = cocos2d::Spawn::create(
                                                            ScaleTo::create(t, 2),
                                                            CCFadeOut::create(t),
                                                            NULL);
    spr->runAction(action1);

	update(0.0f);
}

void lightSprite::delSelf()
{
    this->setVisible(false);
    m_fightlayer->destoryBody(this);
    this->removeFromParent();
}





#endif /* lightSprite_cpp */
