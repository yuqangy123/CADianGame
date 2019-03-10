//
//  BackgroundLayer.cpp
//  CDianGame
//
//  Created by 余强 on 16/3/26.
//
//

#include "BackgroundLayer.hpp"
#include "display.hpp"
#include "ColorBeautifulShader.hpp"
#include "com/MessageQueue.h"

#include "ui/UIImageView.h"
#include "ui/UIScale9Sprite.h"

USING_NS_CC;


backgroundMoveLayer *backgroundMoveLayer::create(int id)
{
	backgroundMoveLayer *ret = new (std::nothrow)backgroundMoveLayer();
	if (ret && ret->init())
	{
		ret->autorelease();
		ret->initEx(id);
		return ret;
	}
	else
	{
		CC_SAFE_DELETE(ret);
		return nullptr;
	}
}

bool backgroundMoveLayer::initEx(int id)
{
	m_id = id;
	return true;
}

void backgroundMoveLayer::addMoveBg()
{
	CCLOG("addMoveBg");
	char name[32];
	for (int n = 1; n <= 3; ++n)
	{
		sprintf(name, "ui/bg%02d_%02d.jpg", m_id, n);
		cocos2d::Sprite* spr = Sprite::create(name);
		auto cnt = m_list.size();
		auto h = 0.0f;
		if (cnt > 0)
		{
			auto backspr = m_list.back();
			auto sh = backspr->getContentSize().height;
			auto pos = backspr->getPosition();
			h = pos.y + sh;
		}
		spr->setPosition(0, h);
		spr->setAnchorPoint(Vec2(0, 0));
		addChild(spr);

		m_list.push_back(spr);
	}
	
	
}

void backgroundMoveLayer::move(bool b)
{
	if (!b && m_moveAction != NULL)
	{
		this->stopAction(m_moveAction);
		m_moveAction = NULL;
		return;
	}
	auto callback = CallFunc::create([this]() {
		auto layerpos = this->getPosition();
		do
		{
			auto cnt = m_list.size();
			if (cnt <= 0)break;
			auto frontspr = m_list.front();
			auto sprpos = frontspr->getPosition();
			auto sprh = frontspr->getContentSize().height;
			sprpos.y += sprh;
			if (abs(layerpos.y) > sprpos.y)
			{
				CCLOG("pop_front");
				m_list.pop_front();
				frontspr->removeFromParent();
			}
			else
			{
				break;
			}
		} while (true);

		do
		{
			auto showedlen = 0.0f;
			if (m_list.size() > 0)
			{
				auto backspr = m_list.back();
				showedlen = (backspr->getPosition().y - backspr->getContentSize().height*2);
			}

			if (showedlen <= abs(layerpos.y))
			{
				addMoveBg();
			}
			else
			{
				break;
			}
		} while (true);
	});
	Sequence *action1 = cocos2d::Sequence::create(
		callback,
		MoveBy::create(m_speed, Vec2(0, -100)),
		NULL);
	m_moveAction = this->runAction(RepeatForever::create(action1));
}

BackgroundLayer *BackgroundLayer::create()
{
    BackgroundLayer *ret = new (std::nothrow)BackgroundLayer();
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


bool BackgroundLayer::initEx()
{
    MessageQueue->RegisterMsg(MSG_ID_SHOW_LAYER,CreateEventEx1(&BackgroundLayer::showLayerNotify) );
    
    auto* csbNode = CSLoader::createNode("BackgroundLayerUI.csb");
    csbNode->setPosition(0,0);
    addChild(csbNode);
    
    auto* rightBottomPanel = csbNode->getChildByName("rightBottomPanel");
    rightBottomPanel->setLocalZOrder(10);
    rightBottomPanel->setPosition(CDisplay::getInstance()->rightBottom);
    auto* leftBottomPanel = csbNode->getChildByName("leftBottomPanel");
    leftBottomPanel->setLocalZOrder(10);
    leftBottomPanel->setPosition(CDisplay::getInstance()->leftBottom);
    
    auto image1 = dynamic_cast<cocos2d::ui::ImageView*>(rightBottomPanel->getChildByName("image1"));
    auto image2 = dynamic_cast<cocos2d::ui::ImageView*>(leftBottomPanel->getChildByName("image2"));
    auto image3 = dynamic_cast<cocos2d::ui::ImageView*>(leftBottomPanel->getChildByName("image3"));
    
    Sprite* i1 = Sprite::create("ui/sekuai3.png");
    i1->setPosition(image1->getPosition());
    i1->setAnchorPoint(image1->getAnchorPoint());
    i1->setScale(image1->getScale());
    rightBottomPanel->addChild(i1);
    image1->removeFromParentAndCleanup(true);
    colorBeautifulShader->addBode(i1);
   
    Sprite* i2 = Sprite::create("ui/sekuai2.png");
    i2->setPosition(image2->getPosition());
    i2->setAnchorPoint(image2->getAnchorPoint());
    i2->setScale(image2->getScale());
    leftBottomPanel->addChild(i2);
    image2->removeFromParentAndCleanup(true);
    colorBeautifulShader->addBode(i2);
    
    Sprite* i3 = Sprite::create("ui/sekuai1.png");
    i3->setPosition(image3->getPosition());
    i3->setAnchorPoint(image3->getAnchorPoint());
    i3->setScale(image3->getScale());
    leftBottomPanel->addChild(i3);
    m_image3 = i3;
    image3->removeFromParentAndCleanup(true);
    colorBeautifulShader->addBode(i3);
    
    Sequence *action1 = cocos2d::Sequence::create(
                                                  MoveBy::create(3.2, Vec2(0, 300)),
                                                  MoveBy::create(3.2, Vec2(0, -300)),
                                                  NULL);
    i1->runAction(RepeatForever::create(action1));
    i2->runAction(RepeatForever::create(CCRotateBy::create(2.0,90.0)));
    i3->runAction(RepeatForever::create(CCRotateBy::create(1.0,90.0)));
    
	initsize();
	image3RandomMoveCallback(nullptr);
    
	m_bg1 = backgroundMoveLayer::create(1);
    m_bg2 = backgroundMoveLayer::create(2);
	m_bg3 = backgroundMoveLayer::create(3);
	
	m_bg1->move(true);
	m_bg2->move(true);
	m_bg3->move(true);

	m_bg1->setAnchorPoint(Vec2(0.0, 0.0));
	m_bg2->setAnchorPoint(Vec2(0.0, 0.0));
	m_bg3->setAnchorPoint(Vec2(0.0, 0.0));
	
    m_bg1->setPosition(0, 0);
    m_bg2->setPosition(0 ,0);
    m_bg3->setPosition(0, 0);
    m_bg1->setLocalZOrder(1);
    m_bg2->setLocalZOrder(2);
    m_bg3->setLocalZOrder(3);
    m_bg1->setTag(1);
    m_bg2->setTag(2);
    m_bg3->setTag(3);
    csbNode->addChild(m_bg1);
	csbNode->addChild(m_bg2);
	csbNode->addChild(m_bg3);
	BgChangeCallback(nullptr);

	return true;
}

void BackgroundLayer::initsize()
{
    //模型中心点为基准
    Point lefttop = CDisplay::getInstance()->leftTop;
    Point leftbottom = CDisplay::getInstance()->leftBottom;
    Point righttop = CDisplay::getInstance()->rightTop;
    Point topcenter = CDisplay::getInstance()->topCenter;
    Point bottomcenter = CDisplay::getInstance()->bottomCenter;
    
    float width = righttop.x - lefttop.x;
    float rw = width/5;
    
    m_screenSize.origin.x = leftbottom.x;
    m_screenSize.origin.y = leftbottom.y;
    m_screenSize.size.width = righttop.x - lefttop.x;
    m_screenSize.size.height = topcenter.y - bottomcenter.y;
}

void BackgroundLayer::image3RandomMoveCallback(Node* nd)
{
    Size sz = m_image3->getContentSize();
    sz.width *= 2;
    sz.height *= 2;
    Vec2 pos = m_image3->getPosition();
    
    if ( pos.x < m_screenSize.size.width/2 )
        pos.x = m_screenSize.origin.x + m_screenSize.size.width - sz.width;
    else
        pos.x = sz.width;
    pos.y = cocos2d::random(0.0 + sz.height/2, double(m_screenSize.size.height) - sz.height/2);
    
    float speed = 300;
    float len = pos.length();
    float time = len/speed;
    CCFiniteTimeAction *action2 = cocos2d::Sequence::create(
                                                            MoveTo::create(time, pos),
                                                            CCCallFuncN::create(this, callfuncN_selector(BackgroundLayer::image3RandomMoveCallback)),
                                                            NULL);
    m_image3->runAction(action2);
    
    
}

int BackgroundLayer::showLayerNotify(void* lparam)
{
	int* n = (int*)lparam;
	   
    if ( *n == 0 )
    {
		/*
        m_bChange = false;
		if(NULL == m_action)
		{
			stopAction(m_action);
			m_action = NULL;
		}
		*/
    }
    else
    {
		if(!m_bChange)
		{
			m_action = runAction( cocos2d::Sequence::create(DelayTime::create(3), CCCallFuncN::create(this, callfuncN_selector(BackgroundLayer::BgChangeCallback)), NULL));
			m_bChange = true;
		}
    }
    
    delete n;

	return 0;
}


void BackgroundLayer::BgChangeCallback(Node* nd)
{
    if(!m_bChange)return;
	CCLOG("BgChangeCallback,%d", m_bgChangeIndex);
    float delayTime1 = 3.0;
    float fadeOutTime = 12.0;
    m_action = cocos2d::Sequence::create(
                                                            CCFadeOut::create(delayTime1),
                                                            CCDelayTime::create(fadeOutTime),
                                                            CCCallFuncN::create(this, callfuncN_selector(BackgroundLayer::BgChangeCallback)),
                                                            NULL);
    m_bg1->setOpacity(255);
	m_bg2->setOpacity(255);
	m_bg3->setOpacity(255);
    if(m_bgChangeIndex == 1)
    {
        m_bg3->setLocalZOrder(3);
        m_bg3->setVisible(true);
        m_bg1->setLocalZOrder(2);
        m_bg1->setVisible(true);
        m_bg2->setLocalZOrder(1);
        m_bg2->setVisible(false);        
        
        m_bg3->runAction(m_action);
        
    }
    else if(m_bgChangeIndex == 2)
    {
        m_bg1->setLocalZOrder(3);
        m_bg1->setVisible(true);
        m_bg2->setLocalZOrder(2);
        m_bg2->setVisible(true);
        m_bg3->setLocalZOrder(1);
        m_bg3->setVisible(false);
        
        m_bg1->runAction(m_action);
    }
    else if(m_bgChangeIndex == 3)
    {
        m_bg2->setLocalZOrder(3);
        m_bg2->setVisible(true);
        m_bg3->setLocalZOrder(2);
        m_bg3->setVisible(true);
        m_bg1->setLocalZOrder(1);
        m_bg1->setVisible(false);
        
        m_bg2->runAction(m_action);
    }
    m_bgChangeIndex = MAX( 1, (m_bgChangeIndex+1)%4 );
}









