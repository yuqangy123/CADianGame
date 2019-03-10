


#include "LightFilter.hpp"
#include "B2Sprite.hpp"
#include "lightSprite.h"
#include "FightLayer.h"
#include "CLineParticle.hpp"
#include "com/microDtata.h"
#include "com/MessageQueue.h"
#include "GameManager.hpp"
#include "display.hpp"
#include "LightFactory.hpp"

CLightFilter::CLightFilter()
{
    
}


CLightFilter::~CLightFilter()
{
    
}

CLightFilter *CLightFilter::create()
{
    CLightFilter *ret = new (std::nothrow) CLightFilter();
    if (ret && ret->init())
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

bool CLightFilter::initEx(FightLayer* fightLayer)
{
    m_ballSize.width = 55;
    m_ballSize.height = 40;
    m_lineSize.width = 148;
    m_lineSize.height = 23;
    
    m_leftBall = B2Sprite::createSprite();
    m_leftBall->initWithFile("ui/light/dianji_a.png");
    m_leftBall->setPosition(m_ballSize.width/2, 0);
    m_leftBall->setLocalZOrder(2);
    fightLayer->addDynamicbody(m_leftBall, BodyType::lightFilterBall, (void*)BodyUserData::create(m_leftBall, BodyType::lightFilterBall, 0));
    
    m_rightBall = B2Sprite::createSprite();
    m_rightBall->initWithFile("ui/light/dianji_a.png");
    m_rightBall->setPosition(m_lineSize.width + m_ballSize.width*3/2, 0);
    m_rightBall->setLocalZOrder(2);
    fightLayer->addDynamicbody(m_rightBall, BodyType::lightFilterBall, (void*)BodyUserData::create(m_rightBall, BodyType::lightFilterBall, 0));
    
    m_filterLine = B2Sprite::createSprite();
    m_filterLine->initWithFile("ui/light/dianji_b.png");
    m_filterLine->setPosition(m_ballSize.width/2 + m_lineSize.width/2, 0);
    m_filterLine->setLocalZOrder(3);
    fightLayer->addDynamicbody(m_filterLine, BodyType::lightFilterLine, (void*)BodyUserData::create(m_filterLine, BodyType::lightFilterLine, 0));
     
    auto animation = Animation::create();
    for( int i=1;i<7;++i)
    {
        char szName1[100] = {0};
        sprintf(szName1, "ui/dianji_frame/dianji%02d.png", i);
        animation->addSpriteFrameWithFile(szName1);
        //animation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(szName1));
    }
    // should last A seconds. And there are B frames.
    animation->setDelayPerUnit(0.6f / 6.0f);
    animation->setRestoreOriginalFrame(true);
    
    auto action = Animate::create(animation);
    m_filterLine->runAction(RepeatForever::create(action));
    

    ////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////
    
    
    MessageQueue->RegisterMsg(MSG_ID_LIGHTFILTER_BALL_CONTACT,
                              CreateEventEx1(&CLightFilter::lightFilterBallContact) );
    MessageQueue->RegisterMsg(MSG_ID_LIGHTFILTER_LINE_CONTACT,
                              CreateEventEx1(&CLightFilter::lightFilterLineContactStart) );
    MessageQueue->RegisterMsg(MSG_ID_LIGHTFILTER_LINE_CONTACT_FINISH,
                              CreateEventEx1(&CLightFilter::lightFilterLineContactFinish) );
    
    
    cocos2d::Vec2 pos(325/2, 545);
    setPositionEx(pos);
     
    auto deadSpr = cocos2d::Sprite::create();
    auto animation1 = Animation::create();
    for( int i=1;i<9;++i)
    {
        char szName1[100] = {0};
        sprintf(szName1, "ui/siwang1_frame/%03d.png", i);
        animation1->addSpriteFrameWithFile(szName1);
        //animation1->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(szName1));
    }
    // should last A seconds. And there are B frames.
    animation1->setDelayPerUnit(0.6f / 15.0f);
    animation1->setRestoreOriginalFrame(true);
    
    auto action1 = Animate::create(animation1);
    deadSpr->runAction(RepeatForever::create(action1));
    deadSpr->setPosition(m_filterLine->getContentSize().width + 50, m_filterLine->getContentSize().height/2 + 40);
    m_filterLine->addChild(deadSpr);
	deadSpr->setLocalZOrder(10);
    m_deadAnimSpr = deadSpr;
    m_deadAnimSpr->setVisible(false);
    
    m_fightLayer = fightLayer;

	return 0;
}


void CLightFilter::setBroundSize(const cocos2d::Size& s)
{
    m_broundSize = s;
}

void CLightFilter::lightdeadByContact(void *lparam)
{
    lightSprite* spr = (lightSprite*)lparam;
    
    spr->beDead(lightDead::ballContact);
    
    unplayParticle(NULL);
    
    m_deadAnimSpr->setVisible(true);
}

int CLightFilter::lightFilterBallContact(void* lparam)
{
    BodyUserData* udata = (BodyUserData*)lparam;
    if (udata)
        lightdeadByContact(udata->target);
    
    return 1;
}
int CLightFilter::lightFilterLineContactStart(void* lparam)
{
    if(lparam)
    {
        BodyUserData* puserdata = (BodyUserData*)lparam;
        m_fightLayer->contactLight(puserdata->tag);
        playParticle(puserdata->target);
    }
    
    return 1;
}
int CLightFilter::lightFilterLineContactFinish(void* lparam)
{
    BodyUserData* puserdata = (BodyUserData*)lparam;
    
    if(m_fightLayer->getLightFactory()->haveLight((lightSprite*)puserdata->target))
    {
        unplayParticle(puserdata->target);
        lightSprite* bspr = (lightSprite*)puserdata->target;
        if( bspr->isEatingNormal() )
        {
            lightFilterBallContact(lparam);//Midway collision end
        }
        else
        {
            bspr->beEated();
            m_fightLayer->getLightFactory()->delLight(bspr->getTag());
        }
    }
    
    
    return 1;
}

int CLightFilter::playParticle(void* lparam)
{
    if(!gameManager->isPlaying())return 1;
    
    lightSprite* spr = (lightSprite*)lparam;
    for(auto itr = m_parList.begin(); itr != m_parList.end(); itr++)
    {
        if ((*itr)->getTag() == spr->getTag())
            return 1;
    }
     
    auto par = CLineParticle::create();
    m_filterLine->addChild(par);
    par->setPosition(247, 40);
    par->setLightID(spr->getTag());
    par->playSystem();
    par->setTag(spr->getTag());
    
    m_parList.push_back(par);
    
    return 1;
}

int CLightFilter::unplayParticle(void* lparam)
{
    lightSprite* spr = (lightSprite*)lparam;
    
    for(auto itr = m_parList.begin(); itr != m_parList.end(); ++itr)
    {
        CLineParticle* par = (CLineParticle*)(*itr);
        par->stopSystem();
    }
    
    m_parList.clear();
    return 1;
}



const cocos2d::Vec2& CLightFilter::getPosition()
{
    return m_pos;
}

void CLightFilter::setPositionEx(const cocos2d::Vec2& vc)
{
    
    float x = vc.x - m_lineSize.width/2 - m_ballSize.width/2;
    float y = vc.y;
    m_leftBall->setPositionEx(x,y);
    
    
    x = vc.x;
    m_filterLine->setPositionEx(x,y);
    
    
    x = vc.x + m_lineSize.width/2 + m_ballSize.width/2;
    m_rightBall->setPositionEx(x,y);
    
    m_pos = vc;
}
void CLightFilter::movePosition(float x, float y)
{
    offsetPosition(x,y);
}
void CLightFilter::adjustPosition(float x, float y)
{
    offsetPosition(x,y);
}
void CLightFilter::offsetPosition(float x, float y)
{
    float newx = m_pos.x + x;
    float newy = m_pos.y + y;
    float box2dPos = m_fightLayer->getBox2dLayerPos();
    
    
    if ( newx <= (m_lineSize.width+m_ballSize.width)/2  )
    {
        x = m_pos.x - (m_lineSize.width+m_ballSize.width)/2.0;
        x = -x;
    }
    else if(newx >= (m_broundSize.width - (m_lineSize.width+m_ballSize.width)/2))
    {
        x = (m_broundSize.width - (m_lineSize.width+m_ballSize.width)/2) - m_pos.x;
    }
    else if(newy <= -box2dPos)
    {
        y = -box2dPos - newy;
    }
    else if(newy >= (-box2dPos+m_broundSize.height))
    {
        y = (-box2dPos+m_broundSize.height) - newy;
    }
    
    
    /*
    else if(newy < (m_ballSize.height)/2 )
    {
        //y = m_pos.y - m_ballSize.height/2;
    }
    else if(newy > (m_broundSize.height - (m_ballSize.width)/2))
    {
        //y = ( m_broundSize.height - (m_ballSize.width)/2 ) - m_pos.y;
    }
    */
    m_pos.x += x;
    m_pos.y += y;
    
    
    
    m_leftBall->offsetPosition(x, y);
    
    m_filterLine->offsetPosition(x, y);
   
    m_rightBall->offsetPosition(x, y);
}


void CLightFilter::unoffsetPosition()
{
}

float CLightFilter::comeback(cocos2d::Vec2 &pos, float speed)
{
    Vec2 vlen(pos.x-m_pos.x, pos.y-m_pos.y);
    float t = vlen.length()/speed;
    m_leftBall->runAction(cocos2d::Sequence::create(
                                                       MoveTo::create(t, Vec2(pos.x - m_lineSize.width/2 - m_ballSize.width/2, pos.y )),
                                                       //CallFuncN::create(this, callfuncN_selector(FightLayer::showHomeLayer)),
                                                       NULL));

    m_rightBall->runAction(cocos2d::Sequence::create(
                                                    MoveTo::create(t, Vec2(pos.x + m_lineSize.width/2 + m_ballSize.width/2, pos.y )),
                                                    //CallFuncN::create(this, callfuncN_selector(FightLayer::showHomeLayer)),
                                                    NULL));
    
    m_filterLine->runAction(cocos2d::Sequence::create(
                                                    MoveTo::create(t, Vec2(pos.x, pos.y )),
                                                    //CallFuncN::create(this, callfuncN_selector(FightLayer::showHomeLayer)),
                                                    NULL));
    return t;
}

void CLightFilter::setVisibleEx(bool bl)
{
    m_leftBall->setVisible(bl);
    m_rightBall->setVisible(bl);
    m_filterLine->setVisible(bl);
    setVisible(bl);
    
    if(!bl)
    {
        unplayParticle(NULL);
        m_deadAnimSpr->setVisible(false);
    }
}

void CLightFilter::checkNewplayerTouch(const Vec2& checkPos)
{
    m_newplayerCheckPos = checkPos;
    
    checkNewplayerTouchSchedule();
}

void CLightFilter::checkNewplayerTouchSchedule()
{
    Vec2 pos = this->getPosition();
    
    float dis = pos.distance(m_newplayerCheckPos);
    
    if (dis <= 320.f)
    {
        MessageQueue->SendMsg(MSG_ID_NEWPLAYER_CHECK_LEN_NOTIFY, 0);
    }
    else
    {
        m_filterLine->runAction(cocos2d::Sequence::create(
                                                  DelayTime::create(0.2),
                                                  CCCallFunc::create(this, callfunc_selector(CLightFilter::checkNewplayerTouchSchedule)),
                                                  NULL));
    }
}









