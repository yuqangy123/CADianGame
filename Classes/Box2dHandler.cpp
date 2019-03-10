//
//  Box2dHandle2.cpp
//  CDianGame
//
//  Created by 余强 on 16/3/21.
//
//

#include "Box2dHandler.h"
#include "com/microDtata.h"
#include "com/MessageQueue.h"
#include "lightSprite.h"
#include "display.hpp"
USING_NS_CC;


Box2dHandler::Box2dHandler()
{
}
Box2dHandler::~Box2dHandler()
{
    
}



Box2dHandler *Box2dHandler::create()
{
    Box2dHandler *ret = new (std::nothrow) Box2dHandler();
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

void Box2dHandler::initEx()
{
    //start();
    
    Sprite* spr = Sprite::create("ui/light/pengzhuang/001_zhangai.png");
    m_light1Size = spr->getContentSize();
    
    spr = Sprite::create("ui/light/pengzhuang/002_zhangai.png");
    m_light2Size = spr->getContentSize();
    
    spr = Sprite::create("ui/light/pengzhuang/dianji_a.png");
    m_ballSize = spr->getContentSize();
    
    spr = Sprite::create("ui/light/pengzhuang/dianji_b.png");
    m_lineSize = spr->getContentSize();
}


void Box2dHandler::update(float dt)
{
    linepos = m_filterLine.sprite->getPosition();
    linepos.x -= m_filterLine.size.width/2;
    linepos.y -= m_filterLine.size.height/2;
	linepos.y += CDisplay::getInstance()->m_fLineOffsetY;

    ballLpos = m_filterBallL.sprite->getPosition();
    ballLpos.x -= m_filterBallL.size.width/2;
    ballLpos.y -= m_filterBallL.size.height/2;
    
    ballRpos = m_filterBallR.sprite->getPosition();
    ballRpos.x -= m_filterBallR.size.width/2;
    ballRpos.y -= m_filterBallR.size.height/2;
    
    for(auto itr = m_body.begin(); itr != m_body.end(); ++itr)
    {
		if (NULL == m_contactLightBody.sprite)
		{
			bool intersect = !(itr->pos.x + itr->size.width < linepos.x ||
				linepos.x + m_filterLine.size.width < itr->pos.x ||
				itr->pos.y + itr->size.height < linepos.y ||
				linepos.y + m_filterLine.size.height < itr->pos.y);
			if (intersect && intersect != m_lineContacting)//first contact
			{
				MessageQueue->SendMsg(MSG_ID_LIGHTFILTER_LINE_CONTACT, itr->sprite->getUserData());
				m_contactLightBody.copy(*itr);
				m_lineContacting = intersect;
				break;
			}
		}else
		{
			if (m_contactLightBody.sprite == itr->sprite)//contact finish
			{
				bool intersect = !(itr->pos.x + itr->size.width < linepos.x ||
					linepos.x + m_filterLine.size.width < itr->pos.x ||
					itr->pos.y + itr->size.height < linepos.y ||
					linepos.y + m_filterLine.size.height < itr->pos.y);
				//CCLOG("%f,%f,%f,%f,%f,%f,%f,%f,%d\n", itr->pos.x , itr->size.width, itr->pos.y , itr->size.height, linepos.x, m_filterLine.size.width, linepos.y,m_filterLine.size.height, intersect );
				if (!intersect && intersect != m_lineContacting)
				{
					lightSprite* light = (lightSprite*)m_contactLightBody.sprite;
					MessageQueue->SendMsg(MSG_ID_LIGHTFILTER_LINE_CONTACT_FINISH, light->getUserData());
					m_contactLightBody.sprite = NULL;
					m_lineContacting = intersect;
					break;
				}
			}
		}
        
        
        //contact both sides
		bool intersectBallL = !(itr->pos.x + itr->size.width < ballLpos.x ||
                                ballLpos.x + m_filterBallL.size.width < itr->pos.x ||
                                itr->pos.y + itr->size.height < ballLpos.y ||
                                ballLpos.y + m_filterBallL.size.height < itr->pos.y);
        if(intersectBallL)
        {
            MessageQueue->SendMsg(MSG_ID_LIGHTFILTER_BALL_CONTACT, m_contactLightBody.sprite!=NULL ? m_contactLightBody.sprite->getUserData() : itr->sprite->getUserData());
            stop();
            break;
        }
        
        bool intersectBallR = !(itr->pos.x + itr->size.width < ballRpos.x ||
                                ballRpos.x + m_filterBallR.size.width < itr->pos.x ||
                                itr->pos.y + itr->size.height < ballRpos.y ||
                                ballRpos.y + m_filterBallR.size.height < itr->pos.y);
        if(intersectBallR)
        {
            MessageQueue->SendMsg(MSG_ID_LIGHTFILTER_BALL_CONTACT, m_contactLightBody.sprite!=NULL ? m_contactLightBody.sprite->getUserData() : itr->sprite->getUserData());
            stop();
            break;
        }
    }
    
}

void Box2dHandler::stop()
{
    this->unscheduleUpdate();
    initState();
}

void Box2dHandler::start()
{
    this->scheduleUpdate();
    initState();
}

void Box2dHandler::initState()
{
    m_contactLightBody.sprite = NULL;
    m_lineContacting = false;
}

void Box2dHandler::creatDynamicbody(B2Sprite *sp, int shape, void* userData)
{
    sp->setUserData(userData);
    
    body bd;
    bd.sprite = sp;
    bd.type = (BodyType)shape;
    bd.size = sp->getContentSize();
    
    if (shape == BodyType::lightFilterBall)
    {
        bd.size = m_ballSize;
        if (NULL == m_filterBallL.sprite)
            m_filterBallL = bd;
        else
            m_filterBallR = bd;
    }
    else if(shape == BodyType::lightFilterLine)
    {
        bd.size = m_lineSize;
        m_filterLine = bd;
    }
    else
    {
        lightSprite* light = (lightSprite*)bd.sprite;
        bd.size = light->getLightID() == 1 ? m_light1Size : m_light2Size;
        bd.pos = Vec2(light->getPositionX() - bd.size.width/2, light->getPositionY() - bd.size.height/2);
        m_body.push_back(bd);
    }
    
    return;
    
}

void Box2dHandler::createStaticbody(B2Sprite* sp, int shape, void* userData)
{
    sp->setUserData(userData);
    
    body bd;
    bd.sprite = sp;
    bd.type = (BodyType)shape;
    bd.size = sp->getContentSize();
    
    if (shape == BodyType::lightFilterBall)
    {
        bd.size = m_ballSize;
        if (NULL == m_filterBallL.sprite)
            m_filterBallL = bd;
        else
            m_filterBallR = bd;
    }
    else if(shape == BodyType::lightFilterLine)
    {
        bd.size = m_lineSize;
        m_filterLine = bd;
    }
    else
    {
        lightSprite* light = (lightSprite*)bd.sprite;
        bd.size = light->getLightID() == 1 ? m_light1Size : m_light2Size;
        bd.pos = Vec2(light->getPositionX() - bd.size.width/2, light->getPositionY() - bd.size.height/2);
        m_body.push_back(bd);
    }
}

void Box2dHandler::destoryBody(Sprite *b)
{
    if (b)
    {
        for(auto itr = m_body.begin(); itr != m_body.end(); itr++)
        {
            if((*itr).sprite == b)
            {
                m_body.erase(itr);
                break;
            }
        }
        //m_world->DestroyBody(b);
    }
}





