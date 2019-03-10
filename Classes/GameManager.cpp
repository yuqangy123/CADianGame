//
//  GameManager.cpp
//  CDianGame
//
//  Created by 余强 on 16/3/26.
//
//

#include "GameManager.hpp"
//#include "com/GB2ShapeCache-x.h"
#include "FightLayer.h"
#include "HomeLayer.hpp"
#include "dataManager.hpp"
#include "com/MessageQueue.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <jni.h>
#include "platform/android/jni/JniHelper.h"
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#endif

using namespace cocos2d;
CGameManager::CGameManager()
{
    m_gameState = GAME_STATE_ID::ready;
    
    m_bgLayer = nullptr;
    m_homeLayer = nullptr;
    m_fightLayer = nullptr;
}

CGameManager::~CGameManager()
{
    
}



CGameManager* CGameManager::handler()
{
    static CGameManager* handler = nullptr;
    if (handler == nullptr){
        handler = new CGameManager();
        handler->initEx();
        return handler;
    }
    else{
        return handler;
    }
}

void CGameManager::initEx()
{
    retain();
    
    m_maxScore = cocos2d::UserDefault::sharedUserDefault()->getIntegerForKey("maxscore");
    m_lastScore = cocos2d::UserDefault::sharedUserDefault()->getIntegerForKey("lastscore");
    m_tipsGgInvite = cocos2d::UserDefault::sharedUserDefault()->getBoolForKey("tipsGgInvite");
    
    {
        m_playDays = cocos2d::UserDefault::sharedUserDefault()->getIntegerForKey("playDays");
        m_playYear = cocos2d::UserDefault::sharedUserDefault()->getIntegerForKey("playYear");
        m_playMonth = cocos2d::UserDefault::sharedUserDefault()->getIntegerForKey("playMonth");
        m_playDay = cocos2d::UserDefault::sharedUserDefault()->getIntegerForKey("playDay");
        
        struct timeval nowTimeval;
        gettimeofday(&nowTimeval, NULL);
        struct tm * tm;
        time_t time_sec;
        time_sec = nowTimeval.tv_sec;
        tm = localtime(&time_sec);
        int nYear = tm->tm_year;
        int nMonth = tm->tm_mon;
        int nDay = tm->tm_mday;
        
        int score_old = m_playYear* 10000 + m_playMonth*100 + m_playDay*1;
        int score_now = nYear* 10000 + nMonth*100 + nDay*1;
        
        if(score_now > score_old)
        {
            ++m_playDays;
            cocos2d::UserDefault::sharedUserDefault()->setIntegerForKey("playDays", m_playDays);
            cocos2d::UserDefault::sharedUserDefault()->setIntegerForKey("playYear", nYear);
            cocos2d::UserDefault::sharedUserDefault()->setIntegerForKey("playMonth", nMonth);
            cocos2d::UserDefault::sharedUserDefault()->setIntegerForKey("playDay", nDay);
        }
        else if(score_now < score_old)
        {
            m_playDays = 1;
            cocos2d::UserDefault::sharedUserDefault()->setIntegerForKey("playDays", m_playDays);
            cocos2d::UserDefault::sharedUserDefault()->setIntegerForKey("playYear", nYear);
            cocos2d::UserDefault::sharedUserDefault()->setIntegerForKey("playMonth", nMonth);
            cocos2d::UserDefault::sharedUserDefault()->setIntegerForKey("playDay", nDay);
        }
    }
    
    
    
    
    
    m_startGlobalTimes = cocos2d::UserDefault::sharedUserDefault()->getIntegerForKey("startGlobalTimes", 0);
    cocos2d::UserDefault::sharedUserDefault()->setIntegerForKey("startTimes", ++m_startGlobalTimes);
    cocos2d::UserDefault::sharedUserDefault()->setIntegerForKey("startGlobalTimes", m_startGlobalTimes);
    
    dataManager->init();
    
    MessageQueue->RegisterMsg(MSG_ID_SHOCK_SCREEN_NOTIFY,CreateEventEx1(&CGameManager::shockScreen) );
}

void CGameManager::setMaxScore(int n)
{
    cocos2d::UserDefault::sharedUserDefault()->setIntegerForKey("maxscore", n);
    m_maxScore = n;
}
void CGameManager::setLastScore(int n)
{
    cocos2d::UserDefault::sharedUserDefault()->setIntegerForKey("lastscore", n);
    
    m_lastScore = n;
        
    //update local data
    
    if (n > m_maxScore)
    {
        int old = m_maxScore;
        
        setMaxScore(n);
        
        
        //post message then score more than 10
        if(old < 10 && n >= 10)
        {
            MessageQueue->SendMsg(MSG_ID_FIRST_SCORE_10_NOTIFY, (void*)n);
        }
    }
}

int CGameManager::shockScreen(void* lparam)
{
    float fz_w = 20.0f;
    float fz_h = 15.0f;
    float ft = 0.020f;
    CCFiniteTimeAction *action1 = cocos2d::Sequence::create(
                                                            MoveBy::create(ft, Vec2(fz_w, 0.0)),
                                                            MoveBy::create(ft, Vec2(-fz_w, fz_h)),
                                                            MoveBy::create(ft, Vec2(-fz_w, -fz_h)),
                                                            MoveBy::create(ft, Vec2(fz_w, -fz_h)),
                                                            MoveBy::create(ft, Vec2(fz_w, fz_h)),
                                                            MoveBy::create(ft, Vec2(-fz_w, 0.0)),
                                                            NULL);
    
    fz_w -= 9.f;
    fz_h -= 8.f;
    CCFiniteTimeAction *action2 = cocos2d::Sequence::create(
                                                            MoveBy::create(ft, Vec2(fz_w, 0.0)),
                                                            MoveBy::create(ft, Vec2(-fz_w, fz_h)),
                                                            MoveBy::create(ft, Vec2(-fz_w, -fz_h)),
                                                            MoveBy::create(ft, Vec2(fz_w, -fz_h)),
                                                            MoveBy::create(ft, Vec2(fz_w, fz_h)),
                                                            MoveBy::create(ft, Vec2(-fz_w, 0.0)),
                                                            NULL);
    m_rootLayer->runAction(cocos2d::Sequence::create(action1, action2, NULL));

	return 0;
}



void CGameManager::showAd(int type)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	JniMethodInfo info;  
	std::string functionName = "showAdWithGameOver";
	if (type == 1)functionName = "showAdWithPause";
	
	bool ret = JniHelper::getStaticMethodInfo(info,"org/cocos2dx/cpp/AppActivity",functionName.c_str(),"()V");
	jobject jobj;  
	if(ret)  
	{  
		//log("call void getObj() succeed");  
		info.env->CallStaticVoidMethod(info.classID,info.methodID);  
	}  
#endif
}
void CGameManager::hideAd()
{
	return;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	JniMethodInfo info;  
	bool ret = JniHelper::getStaticMethodInfo(info,"org/cocos2dx/cpp/AppActivity","closeAd","()V");
	jobject jobj;  
	if(ret)  
	{  
		//log("call void getObj() succeed");  
		info.env->CallStaticVoidMethod(info.classID,info.methodID);  
	}  
#endif	
}

bool CGameManager::isTipsGoogleInvite()
{
	return false;
    if(!m_tipsGgInvite && getPlayTimes() == 1 && m_playDays == 2)
        return true;
    
    return false;
}

void CGameManager::setTipsGoogleInvite()
{
	return;
    m_tipsGgInvite = true;
    cocos2d::UserDefault::sharedUserDefault()->setBoolForKey("tipsGgInvite", m_tipsGgInvite);
}








