//
//  HomeLayer.cpp
//  CDianGame
//
//  Created by 余强 on 16/3/26.
//
//

#include "HomeLayer.hpp"
#include "display.hpp"
#include "FightLayer.h"
#include "dataManager.hpp"
#include "GameManager.hpp"
#include "AudioManager.hpp"
#include "com/MessageQueue.h"
#include "ColorBeautifulShader.hpp"
#include "facebookShareTipsLayer.hpp"
#include "inviteShareTipsLayer.hpp"


#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <jni.h>
#include "platform/android/jni/JniHelper.h"
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#endif
USING_NS_CC;


HomeLayer *HomeLayer::create()
{
    HomeLayer *ret = new (std::nothrow)HomeLayer();
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


Vec3 rgb2hsb(float r, float g, float b)
{
    float rgbR = r*255.0;
    float rgbG = g*255.0;
    float rgbB = b*255.0;
    
    float max = rgbR;
    if (max < rgbG)max = rgbG;
    if (max < rgbB)max = rgbB;
    
    float min = rgbR;
    if (min > rgbG)min = rgbG;
    if (min > rgbB)min = rgbB;
    
    float hsbB = max/255.0;;
    float hsbS = 0.0;
    if (0.0 != max)hsbS = (max - min)/max;
    float hsbH = 0.0;
    
    if (max == rgbR && rgbG >= rgbB)
    {
        hsbH = (rgbG - rgbB) * 60.0 / (max - min) + 0.0;
    }
    else if (max == rgbR && rgbG < rgbB)
    {
        hsbH = (rgbG - rgbB) * 60.0 / (max - min) + 360.0;
    }
    else if (max == rgbG)
    {
        hsbH = (rgbB - rgbR) * 60.0 / (max - min) + 120.0;
    }
    else if (max == rgbB)
    {
        hsbH = (rgbR - rgbG) * 60.0 / (max - min) + 240.0;
    };
    
    return Vec3(hsbH, hsbS, hsbB);
}

bool HomeLayer::initEx()
{
    /*
    m_sprtest = Sprite::create("ui/light/001.png");
    m_sprtest->setPosition(300, 500);
    addChild(m_sprtest, 10, 10);
    cocos2d::GLProgram* shader1 = GLProgram::createWithFilenames("shader/lighthide.vsh", "shader/lighthide.fsh");
    auto state1 = GLProgramState::create(shader1);
    m_sprtest->setGLProgramState(state1);
    state1->setUniformFloat("HValue", 205.0);
    state1->setUniformFloat("filterValue", 0.0);
    state1->setUniformInt("fromTop", 0);
    
    
    CCFiniteTimeAction *action4 = cocos2d::Sequence::create(
                                                            CCDelayTime::create(0.05),
                                                            CCCallFuncN::create(this, callfuncN_selector(HomeLayer::testShader)),
                                                            NULL);
    m_sprtest->runAction(action4);
    */
    //////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////
    
    
    
    //
    
    MessageQueue->SendMsg(MSG_ID_SHOW_LAYER, new int(0));

    auto* csbNode = CSLoader::createNode("HomeLayerUI.csb");
    csbNode->setPosition(CDisplay::getInstance()->leftBottom);
    addChild(csbNode);
    
	
    m_audioBtn = (ui::Button*)csbNode->getChildByName("audioBtn");
	
    m_audioBtn->addTouchEventListener(this, ui::SEL_TouchEvent(&HomeLayer::audioTouchCallback));
    m_audioBtn->loadTextureNormal(audioManager->getEnable() ? "ui/shezhi_on.png" : "ui/shezhi_off.png" );
	m_audioBtn->setLocalZOrder(1);
	m_audioBtn->setAnchorPoint(Vec2(1.0, 1.0));
	m_audioBtn->setPosition(Vec2(-5, -5));
	auto audiosz = m_audioBtn->getContentSize();
	auto rtpos = CDisplay::getInstance()->rightTop;
	m_audioBtn->setPosition(Vec2(rtpos.x, rtpos.y));
	
	
    auto* topPanel = csbNode->getChildByName("topPanel");
    topPanel->setPosition(CDisplay::getInstance()->topCenter.x, CDisplay::getInstance()->topCenter.y - 247);
    auto* txt1 = topPanel->getChildByName("txt1");
    auto* txt2 = topPanel->getChildByName("txt2");
    m_txt1 = dynamic_cast<cocos2d::ui::Widget*>(txt1);
    m_txt2 = dynamic_cast<cocos2d::ui::Widget*>(txt2);
    
    float txtMvLen = 800.0f;
    float txtMvTime = 0.50f;
    Vec2 t1Pos = m_txt1->getPosition();
    Vec2 t2Pos = m_txt2->getPosition();
    t1Pos.x += -txtMvLen;
    t2Pos.x += txtMvLen;
    m_txt1->setPosition(t1Pos);
    m_txt2->setPosition(t2Pos);
    m_txt1->runAction(EaseSineOut::create(MoveBy::create(txtMvTime, Vec2(txtMvLen, 0))));
    m_txt2->runAction(EaseSineOut::create(MoveBy::create(txtMvTime, Vec2(-txtMvLen, 0))));
    
    
    auto Image_2 = topPanel->getChildByName("Image_2");
    m_Image_2 = dynamic_cast<cocos2d::ui::Widget*>(Image_2);
    m_Image_2->runAction( CCFadeIn::create(txtMvTime) );
    //colorBeautifulShader->addBode(m_Image_2);
    
    auto animation1 = Animation::create();
    for( int i=1;i<4;++i)
    {
        char szName1[100] = {0};
        sprintf(szName1, "ui/s0%d.png", i);
        animation1->addSpriteFrameWithFile(szName1);
        //animation1->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(szName1));
    }
    // should last A seconds. And there are B frames.
    animation1->setDelayPerUnit(0.3f / 3.0f);
    animation1->setRestoreOriginalFrame(true);
    auto action1 = Animate::create(animation1);
    auto* animNode1 = Sprite::create();
    txt1->addChild(animNode1);
    animNode1->setPosition(txt1->getContentSize().width/2, txt1->getContentSize().height/2);
    animNode1->runAction(RepeatForever::create(action1));
    
    
    auto animation2 = Animation::create();
    for( int i=1;i<4;++i)
    {
        char szName1[100] = {0};
        sprintf(szName1, "ui/x0%d.png", i);
        animation2->addSpriteFrameWithFile(szName1);
        //animation2->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(szName1));
    }
    // should last A seconds. And there are B frames.
    animation2->setDelayPerUnit(0.6f / 6.0f);
    animation2->setRestoreOriginalFrame(true);
    auto action2 = Animate::create(animation2);
    auto* animNode2 = Sprite::create();
    txt2->addChild(animNode2);
     
    animNode2->setPosition(txt2->getContentSize().width/2, txt2->getContentSize().height/2);
    animNode2->runAction(RepeatForever::create(action2));
    //txt2->runAction(RepeatForever::create(action2));
    
    
    
    auto* bottomPanel = csbNode->getChildByName("bottomPanel");
	auto topPanelPos = topPanel->getPosition();
    bottomPanel->setPosition(topPanelPos.x, topPanelPos.y - 770);
    

    ui::Button* startGameBtn = dynamic_cast<ui::Button*>( bottomPanel->getChildByName("startGameBtn") );
    m_startGameBtn = startGameBtn;
    startGameBtn->addTouchEventListener(this, ui::SEL_TouchEvent(&HomeLayer::startGameTouchCallback));

	animation1 = Animation::create();
	for (int i = 0; i<3; ++i)
	{
		char szName1[100] = { 0 };
		sprintf(szName1, "ui/kaishi%02d.png", i);
		animation1->addSpriteFrameWithFile(szName1);
		//animation1->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(szName1));
	}
	// should last A seconds. And there are B frames.
	animation1->setDelayPerUnit(0.3f / 3.0f);
	animation1->setRestoreOriginalFrame(true);
	action1 = Animate::create(animation1);
	animNode1 = Sprite::create();
	startGameBtn->addChild(animNode1);
	animNode1->setPosition(startGameBtn->getContentSize().width / 2, startGameBtn->getContentSize().height / 2);
	animNode1->runAction(RepeatForever::create(action1));

    /*
    auto animation3 = Animation::create();
    for( int i=1;i<4;++i)
    {
        char szName3[100] = {0};
        sprintf(szName3, "ui/kaishi0%d.png", i);
        animation3->addSpriteFrameWithFile(szName3);
    }
    // should last A seconds. And there are B frames.
    animation3->setDelayPerUnit(0.3f / 3.0f);
    animation3->setRestoreOriginalFrame(true);
    auto action3 = Animate::create(animation3);
    auto* animNode3 = Sprite::create();
    startGameBtn->addChild(animNode3);
    animNode3->setPosition(startGameBtn->getContentSize().width/2, startGameBtn->getContentSize().height/2);
    animNode3->runAction(RepeatForever::create(action3));
    */
    
    auto* scorePanel = bottomPanel->getChildByName("scorePanel");
    auto* dianliuNode = Sprite::create("ui/dianji_frame/dianji01.png");
    m_dianliuNode = dianliuNode;
    dianliuNode->setPosition(100, 100);
    scorePanel->addChild(dianliuNode);
    m_originalDianliuPos = m_dianliuNode->getPosition();
    
    

    auto animation = Animation::create();
    for( int i=1;i<7;++i)
    {
        char szName1[100] = {0};
        sprintf(szName1, "ui/dianji_frame/dianji0%d.png", i);
        //single image
        //animation->addSpriteFrameWithFile(szName1);
        
        //plist image
        animation->addSpriteFrameWithFile(szName1);
        //animation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(szName1));
    }
    // should last A seconds. And there are B frames.
    animation->setDelayPerUnit(0.6f / 6.0f);
    animation->setRestoreOriginalFrame(true);
    auto action = Animate::create(animation);
    dianliuNode->runAction(RepeatForever::create(action));
    
    char tmpZi[128] = {0};
    sprintf(tmpZi, "%d", gameManager->getMaxScore());
    LabelAtlas* dazi = LabelAtlas::create(tmpZi, "ui/dazi.png", 58.6, 78, 48);
    dazi->setPosition(95, 180);
    dazi->setAnchorPoint(Vec2(0.5, 0.5));
    scorePanel->addChild(dazi);
    m_dazi = dazi;
    
    m_newScoreNode = CCSprite::create("ui/NEW.png");
    m_newScoreNode->setPosition(Vec2(dazi->getPositionX() + 110, dazi->getPositionY() ));
    scorePanel->addChild(m_newScoreNode);
	m_newScoreNode->setVisible(false);
    
    
    
    sprintf(tmpZi, "0");
    LabelAtlas* xiaozi = LabelAtlas::create(tmpZi, "ui/xiaozi.png", 34, 45, 48);
    xiaozi->setPosition(95, 0);
    xiaozi->setAnchorPoint(Vec2(0.5, 0.5));
    scorePanel->addChild(xiaozi);
    m_xiaozi = xiaozi;
    
	
    audioManager->playMainPageMusic(true, true);
    
    m_fbBtn = dynamic_cast<ui::Button*>( bottomPanel->getChildByName("facebook") );
    m_fbBtn->addTouchEventListener(this, ui::SEL_TouchEvent(&HomeLayer::facebookCallback));
	m_fbBtn->setVisible(false);
    
    m_googleBtn = dynamic_cast<ui::Button*>( bottomPanel->getChildByName("google") );
    m_googleBtn->addTouchEventListener(this, ui::SEL_TouchEvent(&HomeLayer::googleCallback));
	m_googleBtn->setVisible(false);

    //register message
    MessageQueue->RegisterMsg(MSG_ID_FIRST_SCORE_10_NOTIFY, CreateEventEx1(&HomeLayer::first10ScoreNotify) );

	return 0;
}


void HomeLayer::audioTouchCallback(Ref* obj, ui::TouchEventType tp)
{
    printf("HomeLayer::audioTouchCallback:%d\n", tp);
    if (tp != ui::TouchEventType::TOUCH_EVENT_ENDED)return;
    
    bool b = audioManager->getEnable();
    b = !b;
    audioManager->setEnable(b);
    
    m_audioBtn->loadTextureNormal(b ? "ui/shezhi_on.png" : "ui/shezhi_off.png" );
    m_audioBtn->loadTexturePressed(b ? "ui/shezhi_off.png" : "ui/shezhi_off.png" );
    
    if (b)
        audioManager->playMainPageMusic(true, true);
}

void HomeLayer::startGameTouchCallback(Ref* obj, ui::TouchEventType tp)
{
    if (tp != ui::TouchEventType::TOUCH_EVENT_ENDED || m_beStart)
		return;

    m_beStart = true;
    
    m_txt1->runAction(EaseSineIn::create(MoveBy::create(0.5, Vec2(1000, 0))));
    m_txt2->runAction(EaseSineIn::create(MoveBy::create(0.5, Vec2(-1000, 0))));
    
    m_Image_2->runAction( CCFadeOut::create(0.4) );
    
    m_audioBtn->runAction( CCFadeOut::create(0.4) );
    m_audioBtn->setTouchEnabled(false);
    
    m_fbBtn->runAction( CCFadeOut::create(0.4) );
    m_fbBtn->setTouchEnabled(false);
    
    m_googleBtn->runAction( CCFadeOut::create(0.4) );
    m_googleBtn->setTouchEnabled(false);
    
    CCFiniteTimeAction *action1 = cocos2d::Sequence::create(
                                                            CCDelayTime::create(0.6),
                                                            cocos2d::Spawn::create(
                                                                                   CCFadeOut::create(1.0),
                                                                                   ScaleTo::create(1.0, 2.0),
                                                                                   NULL
                                                                                   ),
                                                            NULL);
    m_startGameBtn->runAction(action1);
    m_startGameBtn->setTouchEnabled(false);
    
	auto dianliuWorldPos = m_dianliuNode->convertToWorldSpace(Vec2(0, 0));
    CCFiniteTimeAction *action2 = cocos2d::Sequence::create(
                                                            CCDelayTime::create(0.4),
                                                            EaseSineIn::create(MoveBy::create(0.5, Vec2(0, -dianliuWorldPos.y))),
                                                            EaseSineOut::create(MoveBy::create(0.7, Vec2(0, dianliu_jumpH))),
                                                            CCCallFuncN::create(this, callfuncN_selector(HomeLayer::startGame)),
                                                            NULL);
    m_dianliuNode->runAction(action2);
    
    CCFiniteTimeAction *action3 = cocos2d::Sequence::create(
                                                            CCDelayTime::create(0.4),
                                                            CCFadeOut::create(0.5),
                                                            NULL);
    m_dazi->runAction(action3);
    
    CCFiniteTimeAction *action4 = cocos2d::Sequence::create(
                                                            CCDelayTime::create(0.4),
                                                            CCFadeOut::create(0.5),
                                                            NULL);
    m_xiaozi->runAction(action4);
    
    m_newScoreNode->setVisible(false);
    m_newScoreNode->stopAllActions();
    
    audioManager->playStartgame(true, false);
}

void HomeLayer::comebackCallback()
{
    m_beStart = false;
    //m_audioBtn->setGlobalZOrder(10);//
    m_audioBtn->setTouchEnabled(true);
    m_fbBtn->setTouchEnabled(true);
    m_googleBtn->setTouchEnabled(true);
    m_startGameBtn->setTouchEnabled(true);
    audioManager->playMainPageMusic(true, true);
	
	gameManager->showAd(0);
    /*
    if(gameManager->isTipsGoogleInvite() && NULL == getChildByTag(2314))
    {
        inviteShareTipsLayer* l = inviteShareTipsLayer::create();
        l->setLocalZOrder(4);
        l->setPosition(CDisplay::getInstance()->leftBottom);
        addChild(l);
        gameManager->setTipsGoogleInvite();
    }
    */
    m_visible = true;
}

void HomeLayer::comeback(float delayTime, bool bPassScore)
{
     MessageQueue->SendMsg(MSG_ID_SHOW_LAYER, new int(0));
    
    m_txt1->runAction(EaseSineIn::create(MoveBy::create(0.5, Vec2(-1000, 0))));
    m_txt2->runAction(EaseSineIn::create(MoveBy::create(0.5, Vec2(1000, 0))));
    
    m_Image_2->runAction( CCFadeIn::create(0.4) );
    
    m_audioBtn->runAction( CCFadeIn::create(0.4) );
    
    m_fbBtn->runAction( CCFadeIn::create(0.4) );
    
    m_googleBtn->runAction( CCFadeIn::create(0.4) );
    
    CCFiniteTimeAction *action1 = cocos2d::Sequence::create(
                                                            CCDelayTime::create(0.6),
                                                            cocos2d::Spawn::create(
                                                                                   CCFadeIn::create(1.0),
                                                                                   ScaleTo::create(1.0, 1.0),
                                                                                   CCCallFunc::create(this, callfunc_selector(HomeLayer::comebackCallback)),
                                                                                   NULL
                                                                                   ),
                                                            NULL);
    m_startGameBtn->runAction(action1);
    
    
    CCFiniteTimeAction *action3 = cocos2d::Sequence::create(
                                                            CCDelayTime::create(0.4),
                                                            CCFadeIn::create(0.5),
                                                            NULL);
    m_dazi->runAction(action3);
    
    CCFiniteTimeAction *action4 = cocos2d::Sequence::create(
                                                            CCDelayTime::create(0.4),
                                                            CCFadeIn::create(0.5),
                                                            NULL);
    m_xiaozi->runAction(action4);
    
    m_dianliuNode->runAction(cocos2d::Sequence::create(CCDelayTime::create(delayTime), Show::create(), NULL));
    
    char tmpZi[128] = {0};
    sprintf(tmpZi, "%d", gameManager->getMaxScore());
    m_dazi->setString(tmpZi);
    
    sprintf(tmpZi, "%d", gameManager->getLastScore());
    m_xiaozi->setString(tmpZi);
    
    if(bPassScore)
    {
        m_newScoreNode->setVisible(true);
        m_newScoreNode->stopAllActions();
        m_newScoreNode->runAction(RepeatForever::create(Sequence::create(
                                                                         FadeOut::create(1.0),
                                                                         FadeIn::create(1.0),
                                                                         NULL)));
    }
}


void HomeLayer::startGame(Node* nd)
{
    const Vec2& pos = m_dianliuNode->getPosition();
    Node* parent = m_dianliuNode->getParent();
    Vec2 cpos = parent->convertToWorldSpace(pos);
    
    FightLayer* fightlayer = (FightLayer*)gameManager->getFightLayer();
    if (nullptr == fightlayer)
    {
        fightlayer = FightLayer::create(cpos);
        fightlayer->startGame();
        gameManager->getRootLayer()->addChild(fightlayer);
        gameManager->setFightLayer(fightlayer);
        fightlayer->setLocalZOrder(2);
    }
    else
    {
        fightlayer->startGame();
    }
    
    m_dianliuNode->setVisible(false);
	
	gameManager->hideAd();
    
	//if(gameManager->getPlayTimes() > 0)
		audioManager->playBGMusic(true, true);
	
    m_visible = false;
}



void HomeLayer::showComeback(float delayTime, bool b)
{
    m_dianliuNode->setPosition(m_originalDianliuPos);
    comeback(delayTime, b);
}

void HomeLayer::facebookCallback(Ref* obj, ui::TouchEventType tp)
{
    if (tp != ui::TouchEventType::TOUCH_EVENT_ENDED || m_beStart)return;
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    MessageQueue->SendMsg(MSG_ID_SDK_IOS_FB_SHARE_NOTIFY, NULL);
#endif
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	
    //截图
    Size sz = Director::sharedDirector()->getWinSize();
    RenderTexture* in_texture = RenderTexture::create(sz.width, sz.height,
                                                      kCCTexture2DPixelFormat_RGBA8888);
    Scene* scene = Director::sharedDirector()->getRunningScene();
    in_texture->begin();
    scene->visit();
    in_texture->end();
	
	//std::string wirtePath = FileUtils::getInstance()->getWritablePath();
	
	//CCLOG("*******photo wirtePath :%s", wirtePath.c_str());
	
    in_texture->saveToFile("ecrushFbShare.png", kCCImageFormatPNG);
    
    runAction(Sequence::create(DelayTime::create(0.1f), CCCallFunc::create(this, callfunc_selector(HomeLayer::facebookPhotoShareCallback)),NULL));
    

#endif
    
}

void HomeLayer::googleCallback(Ref* obj, ui::TouchEventType tp)
{
    if (tp != ui::TouchEventType::TOUCH_EVENT_ENDED || m_beStart)return;
	
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    
    JniMethodInfo info;
    bool ret = JniHelper::getStaticMethodInfo(info,"org/cocos2dx/cpp/AppActivity","googleplayShare","()V");
    jobject jobj;
    if(ret)
    {
        info.env->CallStaticVoidMethod(info.classID,info.methodID);
    }
    
#endif
}

void HomeLayer::facebookPhotoShareCallback()
{
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
}

int HomeLayer::first10ScoreNotify(void* lparam)
{
	/*
    facebookShareTipsLayer* l = facebookShareTipsLayer::create();
    l->setLocalZOrder(4);
    l->setPosition(CDisplay::getInstance()->leftBottom);
    addChild(l);
    l->setTag(2314);
    
    first10ScoreSchedule(NULL);
	*/
	return 0;
}

void HomeLayer::first10ScoreSchedule(Node* nd)
{
    if( !m_visible ){
        CCFiniteTimeAction *action = cocos2d::Sequence::create(
                                                                CCDelayTime::create(0.5),
                                                                CCCallFuncN::create(this, callfuncN_selector(HomeLayer::first10ScoreSchedule)),
                                                                NULL);
        this->runAction(action);
        //schedule(schedule_selector(HomeLayer::first10ScoreSchedule), 0.5f);
    }
    else{
        facebookShareTipsLayer* l = (facebookShareTipsLayer*)getChildByTag(2314);
        l->show();
    }
}









