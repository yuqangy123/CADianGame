//
//  FightLayer.cpp
//  CDianGame
//
//  Created by 余强 on 16/3/21.
//
//

#ifndef FightLayer_cpp
#define FightLayer_cpp

#include "FightLayer.h"
#include "B2Sprite.hpp"
#include "com/microDtata.h"
#include "com/MessageQueue.h"
#include "display.hpp"
#include "lightSprite.h"
#include "newPlayerGuideLayer.hpp"
#include "GameManager.hpp"
#include "dataManager.hpp"
#include "ColorBeautifulShader.hpp"
#include "AudioManager.hpp"

#include "ui/UIImageView.h"
#include "ui/UIScale9Sprite.h"



USING_NS_CC;

float tmpMoveY = .0f;
float moveY = .0f;

FightLayer::FightLayer()
{
    m_box2d = nullptr;
}
FightLayer::~FightLayer()
{
    m_lightFilter->release();
}

FightLayer *FightLayer::create(const Vec2& dianliuPos)
{
    FightLayer *ret = new (std::nothrow)FightLayer();
    if (ret && ret->init())
    {
        ret->autorelease();
        ret->initEx(dianliuPos);
        return ret;
    }
    else
    {
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
}

void FightLayer::initEx(const Vec2& dianliuPos)
{
    initUI();
    
    m_dianliuPos = dianliuPos;
    
    m_box2dLayer = CCLayer::create();
    addChild(m_box2dLayer);
    
    m_box2d = Box2dHandler::create();
    m_box2d:setPosition(0,0);
    m_box2dLayer->addChild(m_box2d);
    
    
    m_lightFilter = CLightFilter::create();
    m_lightFilter->initEx(this);
    m_lightFilter->setPositionEx(dianliuPos);
    m_lightFilter->retain();
    
    
    m_lightFilterTouchLayer = LightFilterTouchLayer::create();
    m_lightFilterTouchLayer->setLightFilter(m_lightFilter);
    this->addChild(m_lightFilterTouchLayer);

    
    m_lightFactory = LightFactory::create();
    this->addChild(m_lightFactory);
    m_lightFactory->init(this);
    
    initRoadData();
    
    m_deadLineSpr = cocos2d::Sprite::create();
    auto animation = Animation::create();
    for( int i=1;i<9;++i)
    {
        char szName1[100] = {0};
        sprintf(szName1, "ui/siwang1_frame/%03d.png", i);
        animation->addSpriteFrameWithFile(szName1);
        //animation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(szName1));
    }
    // should last A seconds. And there are B frames.
    animation->setDelayPerUnit(0.6f / 15.0f);
    animation->setRestoreOriginalFrame(true);
    
    auto action = Animate::create(animation);
    m_deadLineSpr->runAction(RepeatForever::create(action));
    m_deadLineSpr->setVisible(false);
	m_deadLineSpr->setScale(2.0f);
	m_deadLineSpr->setLocalZOrder(10);
    addChild(m_deadLineSpr);
    
    MessageQueue->RegisterMsg(MSG_ID_GAME_LOSE_NOTIFY, CreateEventEx1(&FightLayer::gameDeadNotify) );
    MessageQueue->RegisterMsg(MSG_ID_NEWPLAYER_CHECK_LEN_NOTIFY, CreateEventEx1(&FightLayer::newplayerCheckLenNotify ));
    MessageQueue->RegisterMsg(MSG_ID_NEWPLAYER_GUIDE_TOUCHMOVE_NOTIFY, CreateEventEx1(&FightLayer::newplayerTouchmoveNotify ));
}

void FightLayer::initUI()
{
    auto* csbNode = CSLoader::createNode("FightLayerUI.csb");
    csbNode->setPosition(0,0);
    addChild(csbNode);
    
    
    auto leftTopPanel = csbNode->getChildByName("leftTopPanel");
    leftTopPanel->setLocalZOrder(1);
    m_pauseBtn = ui::Button::create("ui/suspend.png", "ui/suspend.png", "ui/suspend.png");
	auto pauseBtnSz = m_pauseBtn->getContentSize();
    m_pauseBtn->setAnchorPoint(Vec2(0.0, 1.0));
	m_pauseBtn->setPosition(Vec2(5, -5));
    leftTopPanel->addChild(m_pauseBtn);
	leftTopPanel->setPosition(CDisplay::getInstance()->leftTop);
    
    
    auto rightTopPanel = csbNode->getChildByName("rightTopPanel");
    rightTopPanel->setLocalZOrder(1);
	rightTopPanel->setPosition(CDisplay::getInstance()->rightTop);
    LabelAtlas* xiaozi = LabelAtlas::create("00", "ui/xiaozi.png", 34, 45, 48);
    xiaozi->setAnchorPoint(Vec2(1.0, 1.0));
	xiaozi->setPosition(Vec2(-5, -5));
    rightTopPanel->addChild(xiaozi);
	xiaozi->setScale(1.4);
    m_scoreText = xiaozi;
    
    /*
    LabelAtlas* xiaozi1 = LabelAtlas::create("00", "ui/xiaozi.png", 34, 45, 48);
    xiaozi1->setPosition(130, 130);
    xiaozi1->setAnchorPoint(Vec2(0.5, 0.5));
    rightTopPanel->addChild(xiaozi1);
    m_scoreTextScale = xiaozi1;
    */
    
    
    
    m_resumeHideView = LayerColor::create(Color4B(128, 128, 128, 128));
    m_resumeHideView->setPosition(CDisplay::getInstance()->leftBottom);
    m_resumeHideView->setLocalZOrder(3);
    m_resumeHideView->setVisible(false);
    this->addChild(m_resumeHideView);
    
    m_resumeBtn = ui::Button::create("ui/start.png", "ui/start.png", "ui/start.png");
    m_resumeBtn->setPosition(CDisplay::getInstance()->center);
    m_resumeBtn->setPositionX(m_resumeBtn->getPositionX() + 10);
    m_resumeBtn->setLocalZOrder(3);
    m_resumeBtn->setVisible(false);
    m_resumeBtn->setScale(2);
    this->addChild(m_resumeBtn);
    m_pauseBtn->addTouchEventListener(this, ui::SEL_TouchEvent(&FightLayer::pauseBtnTouchCallback));
    m_resumeBtn->addTouchEventListener(this, ui::SEL_TouchEvent(&FightLayer::resumeBtnTouchCallback));
    
    
    
    //最高分粒子
    m_maxSpeedSprite = Sprite::create();
    cocos2d::Point pt = CDisplay::getInstance()->topCenter;
    pt.y += 100;
    m_maxSpeedSprite->setPosition(pt);
    addChild(m_maxSpeedSprite);
    
    
    cocos2d::ParticleSystem* particle = cocos2d::CCParticleSystemQuad::create("ui/texiao/02.plist");
    particle->stopSystem();
    particle->setTag(2016);
    m_maxSpeedSprite->addChild(particle);
    particle->stopSystem();
}

int FightLayer::newplayerTouchmoveNotify(void* lparam)
{
    if(NULL == m_movingGapAction)
    {
        movingGap(NULL);
    }

	return 0;
}

int FightLayer::newplayerCheckLenNotify(void* lparam)
{
    
    Vec2 dPos = m_lightFilter->getPositionEx();
    
    
    if(!m_lightFilterTouchLayer->isNewGuideTouch())
    {
        if(m_movingGapAction)
        {
            m_box2dLayer->stopAction(m_movingGapAction);
            m_movingGapAction = NULL;
        }
        
    }

	return 0;
}

int FightLayer::gameDeadNotify(void* lparam)
{
    if(!gameManager->isPlaying())
        return 1;
    
    m_box2d->stop();
    
    gameManager->dead();
    this->stopAllActions();
    m_box2dLayer->stopAllActions();
    m_lightFilterTouchLayer->setEnableTouch(false);
    //m_lightFactory->stopAllLight();
    
    
    CCFiniteTimeAction *actionN = cocos2d::Sequence::create(
                                                            CCDelayTime::create(1.0),
                                                            CCCallFunc::create(this, callfunc_selector(FightLayer::hided)),
                                                            NULL);
    runAction(actionN);
    
    gameManager->setLastScore(m_score);
    
    audioManager->playFilter(false, false);

    audioManager->setEffectsVolume(1.0);
    audioManager->playGamelose(true, false);
    
    MessageQueue->SendMsg(MSG_ID_SHOCK_SCREEN_NOTIFY, 0);
    
    return 0;
}


void FightLayer::addDynamicbody(B2Sprite* spr, int shape, void* userdata)
{
    m_box2d->creatDynamicbody(spr, shape, userdata);
    m_box2dLayer->addChild(spr);
}

void FightLayer::addStaticbody(B2Sprite* spr, int shape, void* userdata)
{
    m_box2d->createStaticbody(spr, shape, userdata);
    m_box2dLayer->addChild(spr);
}

float FightLayer::getBox2dLayerPos()
{
    return m_box2dLayer->getPositionY();
}

void FightLayer::destoryBody(Sprite *b)
{
    m_box2d->destoryBody(b);
}

void FightLayer::contactLight(int tag)
{
    m_lightFactory->contactLight(tag);
}

void FightLayer::initRoadData()
{
    //模型中心点为基准
    Point lefttop = CDisplay::getInstance()->leftTop;
    Point leftbottom = CDisplay::getInstance()->leftBottom;
    Point righttop = CDisplay::getInstance()->rightTop;
    Point topcenter = CDisplay::getInstance()->topCenter;
    Point bottomcenter = CDisplay::getInstance()->bottomCenter;
    
    float width = righttop.x - lefttop.x;
    float rw = width/5;
    
    m_r1Pos = Point((rw*3.0)/2.0, topcenter.y);
    m_r2Pos = Point((rw*5.0)/2.0, topcenter.y);
    m_r3Pos = Point((rw*7.0)/2.0, topcenter.y);
    
    m_screenSize.origin.x = leftbottom.x;
    m_screenSize.origin.y = leftbottom.y;
    m_screenSize.size.width = righttop.x - lefttop.x;
    m_screenSize.size.height = topcenter.y - bottomcenter.y;
    
    m_lightFilter->setBroundSize(m_screenSize.size);
}

void FightLayer::startGame()
{
    
        startGameNow();
    
}

void FightLayer::startGameNow()
{
    MessageQueue->SendMsg(MSG_ID_SHOW_LAYER, new int(1));
    
    m_box2d->start();
    
    gameManager->playing();
    
    audioManager->playFilter(true, true);
    audioManager->setEffectsVolume(0.0);

    moveY = 0.f;
    
    m_score = 0.0f;
    
    m_maxScoreIndex = false;
    
    m_showH = m_screenSize.size.height;
    
    m_box2dLayer->setPosition(0,0);
    
    m_deadLineSpr->setVisible(false);
    
    m_lightFilter->setVisibleEx(true);
    
	m_scoreText->stopAllActions();
    m_scoreText->runAction(FadeIn::create(0.5));
    //m_scoreTextScale->runAction(FadeIn::create(0.5));
    m_pauseBtn->runAction(FadeIn::create(0.5));
    
    m_pauseBtn->setTouchEnabled(true);
    m_resumeBtn->setTouchEnabled(true);
    
    m_lightFilterTouchLayer->setEnableTouch(true);
    
    m_maxSpeedSprite->runAction(FadeIn::create(0.5));
    m_maxSpeedSprite->setVisible(false);
    
    m_lightFilter->setPositionEx(m_dianliuPos);
    m_curSpeed = common_speed - 23;
    
    
    
	if( gameManager->isFirstPlay() && 1 == gameManager->getPlayTimes() )
	{
        movingGap(NULL);
		makeLightScheduleFirst();
        this->scheduleUpdate();
        m_lightFilterTouchLayer->showNewGuide();
	}
	else
	{
        movingGap(NULL);
		makeLightSchedule();
        this->scheduleUpdate();
	}
    
    setVisible(true);
}

void FightLayer::makeLightScheduleFirst()
{
	do{
        float fBox2dLayerMoveY = m_box2dLayer->getPositionY();
        CC_BREAK_IF(m_showH > ( -fBox2dLayerMoveY + (m_screenSize.size.height)*1.0) );
        cocos2d::Vec2 vc = m_r2Pos;
        
        int type = 1;
        int gapH = 384;
        B2Sprite* spr = m_lightFactory->addLightBody(vc.x, m_showH + gapH, type);
        lightSprite* light = (lightSprite*)spr;
        
        m_lightFilter->checkNewplayerTouch(light->getPosition());
        
        m_showH += (gapH + spr->getContentSize().height);
    }while(false);
    
    runAction(Sequence::create(DelayTime::create(0.1f), CCCallFunc::create(this, callfunc_selector(FightLayer::makeLightSchedule)),NULL));
}

void FightLayer::makeLightSchedule()
{
    do{
        float fBox2dLayerMoveY = m_box2dLayer->getPositionY();
        CC_BREAK_IF(m_showH > ( -fBox2dLayerMoveY + (m_screenSize.size.height)*1.0) );
        cocos2d::Vec2 vc;
        
        int ran = cocos2d::random(1, 3);
        switch (ran) {
            case 1:
                vc = m_r1Pos;
                break;
            case 2:
                vc = m_r2Pos;
                break;
            case 3:
                vc = m_r3Pos;
                break;
        }
        
        int type = cocos2d::random(1,3);
        int gapH = 384 + (m_lastType == type ? 0 : (type == 1 ? -120 : 120));
        m_lastType = type;
        B2Sprite* spr = m_lightFactory->addLightBody(vc.x, m_showH + gapH, type);
        m_showH += (gapH + spr->getContentSize().height);
    }while(true);
    
    runAction(Sequence::create(DelayTime::create(0.2f), CCCallFunc::create(this, callfunc_selector(FightLayer::makeLightSchedule)),NULL));
}

void FightLayer::update(float dt)
{
    moveY = m_box2dLayer->getPositionY();
    tmpMoveY = moveY - m_moveSize;
    m_moveSize = moveY;
    
    m_lightFilter->adjustPosition(0, -tmpMoveY);
}

void FightLayer::movingGap(Node* nd)
{
    if ( !gameManager->isPlaying() )
        return;
    
    float accelerationSpeed = -23;
    m_curSpeed += accelerationSpeed;
    if (m_curSpeed < -1280.0)
    {
        playMaxSpeedParticle();
        m_curSpeed = -1280.0;
    }
    
    CCFiniteTimeAction *actionN = cocos2d::Sequence::create(
                                                            CCMoveBy::create(1, Vec2(0, m_curSpeed)),
                                                            CCCallFuncN::create(this, callfuncN_selector(FightLayer::movingGap)),
                                                            NULL);
    m_movingGapAction = m_box2dLayer->runAction(actionN);
}

void FightLayer::showHomeLayer(Node* nd)
{
    m_lightFilter->setVisibleEx(false);
    m_box2dLayer->setPosition(0,0);
    
    setVisible(false);
}
void FightLayer::hided()
{
    stopAllActions();
    
    audioManager->playFilter(false, false);
    audioManager->setEffectsVolume(1.0);
    audioManager->setBackgroundMusicVolume(1.0);

    moveY = 0.f;
    
    m_deadLineSpr->setVisible(false);
    
	m_scoreText->stopAllActions();
    m_scoreText->setString("00");
    m_scoreText->runAction(FadeOut::create(0.5));
    //m_scoreTextScale->setString("00");
    //m_scoreTextScale->runAction(FadeOut::create(0.5));
    m_pauseBtn->runAction(FadeOut::create(0.5));
    
    m_pauseBtn->setTouchEnabled(false);
    m_resumeBtn->setTouchEnabled(false);
    
    
    cocos2d::ParticleSystem* s = (cocos2d::ParticleSystem*)m_maxSpeedSprite->getChildByTag(2016);
    s->stopSystem();
    m_maxSpeedSprite->runAction(CCFadeIn::create(0.5));
    
    m_lightFactory->removeAllLight(true);
    
    
    Vec2 lpos(0 + m_dianliuPos.x, -getBox2dLayerPos() + m_dianliuPos.y + (585.0-dianliu_jumpH));
    float t = m_lightFilter->comeback(lpos, 400);
    
    CCFiniteTimeAction *actionN = cocos2d::Sequence::create(
                                                            CCDelayTime::create(t),
                                                            CCCallFuncN::create(this, callfuncN_selector(FightLayer::showHomeLayer)),
                                                            NULL);
    runAction(actionN);
    
    HomeLayer* l = (HomeLayer*)gameManager->getHomeLayer();
    l->showComeback(t, m_maxScoreIndex);
}

void FightLayer::showDeadEffect(float pos)
{
    m_deadLineSpr->setVisible(true);
    m_deadLineSpr->setPosition(pos, 86/2);
}

void FightLayer::filterLightFinsih(int nID)
{
    int curMaxScore = gameManager->getMaxScore();
    int score = dataManager->getScoreWithCustomId(nID);
    m_score += score;
	m_scoreText->stopAllActions();

    char tmp[16] = {0};
    sprintf(tmp, "%02d", m_score);
    m_scoreText->setString(tmp);
    
    m_scoreText->setScale(2.0);
    
    /*
    m_scoreText->runAction(cocos2d::Sequence::create(
                                                     ScaleTo::create(0.1, 2.4),
                                                     ScaleTo::create(0.0, 1.5),
                                                     NULL));
     */
    float time = 0.3;
    m_scoreText->setOpacity(0);
    m_scoreText->setScale(2.4);
    m_scoreText->runAction(Spawn::create(EaseSineOut::create(FadeIn::create(time)), EaseSineOut::create(ScaleTo::create(time, 1.5)), NULL));
    
                                
    if(m_scoreTextScale != NULL)
    {
		m_scoreTextScale->stopAllActions();
        m_scoreTextScale->setScale(1.0);
        m_scoreTextScale->setOpacity(255);
        m_scoreTextScale->setString(tmp);
        CCFiniteTimeAction *action1 = cocos2d::Spawn::create(
                                                             ScaleTo::create(1.0, 3),
                                                             CCFadeOut::create(1.0),
                                                             NULL);
        m_scoreTextScale->runAction(cocos2d::Sequence::create(action1, ScaleTo::create(0, 1), NULL));
    }
    
    
    if (!m_maxScoreIndex && curMaxScore < m_score)
    {
        audioManager->playMaxScore(true, false);
        m_maxScoreIndex = true;
    }
}


void FightLayer::playMaxSpeedParticle()
{
    if (m_maxSpeedSprite->isVisible()) return;
    
    m_maxSpeedSprite->setVisible(true);
    cocos2d::ParticleSystem* par = (cocos2d::ParticleSystem*)m_maxSpeedSprite->getChildByTag(2016);
    par->resetSystem();
}

void FightLayer::setSpeed(float speed)
{
    m_curSpeed = 0;
}

void FightLayer::pauseBtnTouchCallback(Ref* obj, ui::TouchEventType tp)
{
    printf("FightLayer::pauseBtnTouchCallback:%d\n", tp);
    if (tp != ui::TouchEventType::TOUCH_EVENT_ENDED)return;
    
    Director::getInstance()->getScheduler()->setTimeScale(0.0f);
    
    m_resumeBtn->setVisible(true);
    m_resumeHideView->setVisible(true);
    gameManager->pauseGame(true);
	
	gameManager->showAd(1);
	
	audioManager->pauseAllEffects();
}

void FightLayer::resumeBtnTouchCallback(Ref* obj, ui::TouchEventType tp)
{
    if (tp != ui::TouchEventType::TOUCH_EVENT_ENDED)return;
    
    Director::getInstance()->getScheduler()->setTimeScale(1.0f);
    
    m_resumeBtn->setVisible(false);
    m_resumeHideView->setVisible(false);
    gameManager->pauseGame(false);
	
	gameManager->hideAd();
	
	audioManager->resumeAllEffects();
}




#endif /* FightLayer_cpp */
