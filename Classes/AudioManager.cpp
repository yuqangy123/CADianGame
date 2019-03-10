//
//  AudioManager.cpp
//  CDianGame
//
//  Created by 余强 on 16/3/31.
//
//

#include "AudioManager.hpp"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#import "SimpleAudioEngine.h"
#else
#include "SimpleAudioEngine.h"
#endif

using namespace cocos2d;

const char* musicBG = "res/audio/bg.mp3";
const char* mainPageBG = "res/audio/mainpage.mp3";
const char* musicBGLoop = "res/audio/bg_loop.mp3";
const char* soundFilterlight = "res/audio/filterlight.mp3";
const char* soundGamelose = "res/audio/gamelose.wav";
const char* soundStartgame = "res/audio/startgame.wav";
const char* soundMaxScore = "res/audio/maxscore.wav";


#define audioEngine CocosDenshion::SimpleAudioEngine::getInstance()
AudioManager* AudioManager::getInstance()
{
    static AudioManager* pRet = NULL;
    if (NULL == pRet)
    {
        pRet = new(std::nothrow) AudioManager();
        pRet->initEx();
		
		Director::getInstance()->getRunningScene()->addChild(pRet);
    }
    return pRet;
}

void AudioManager::initEx()
{
    audioEngine->preloadBackgroundMusic(musicBG);
	audioEngine->preloadBackgroundMusic(mainPageBG);
	audioEngine->preloadBackgroundMusic(musicBGLoop);
    audioEngine->preloadEffect(soundFilterlight);
    audioEngine->preloadEffect(soundGamelose);
    audioEngine->preloadEffect(soundStartgame);
	audioEngine->preloadEffect(soundMaxScore);


    m_able = cocos2d::UserDefault::sharedUserDefault()->getBoolForKey("isaudio", true);
}

void AudioManager::playBGMusic(bool p, bool loop = true)
{
    if(!m_able && p)return;
	audioEngine->stopBackgroundMusic();
    if(m_bgMusic2Action){
        this->stopAction(m_bgMusic2Action);
        m_bgMusic2Action = NULL;
    };
	
    if(p)
    {
        {
			audioEngine->playBackgroundMusic(musicBG, false);
            if(loop)
            {
                m_bgMusic2Action = this->runAction(Sequence::create(DelayTime::create(32.0f), CCCallFunc::create(this, callfunc_selector(AudioManager::bgMusicLoopPlayCallback)),NULL));
            }
        }
    }
}

void AudioManager::playMainPageMusic(bool p, bool loop = true)
{
    if(!m_able && p)return;
	
	audioEngine->stopBackgroundMusic();

    if(m_bgMusic2Action){
        this->stopAction(m_bgMusic2Action);
        m_bgMusic2Action = NULL;
    };
	
    if(p)
    {
		audioEngine->playBackgroundMusic(mainPageBG, loop);
    }
}

void AudioManager::playFilter(bool p, bool loop = true)
{
    if(!m_able && p)return;
    
    if(p)
    {
        audioEngine->stopEffect(m_soundFilterID);
        m_soundFilterID = audioEngine->playEffect(soundFilterlight, loop);
    }
    else
        audioEngine->stopEffect(m_soundFilterID);
}

void AudioManager::playStartgame(bool p, bool loop = true)
{
    if(!m_able && p)return;
    
    if(p)
    {
        audioEngine->stopEffect(m_soundStartGameID);
        m_soundStartGameID = audioEngine->playEffect(soundStartgame, loop);
    }
        else
            audioEngine->stopEffect(m_soundStartGameID);
}

void AudioManager::playGamelose(bool p, bool loop = true)
{
    if(!m_able && p)return;
    
    if(p)
    {
        audioEngine->stopEffect(m_soundGameloseID);
        m_soundGameloseID = audioEngine->playEffect(soundGamelose, loop);
    }
    else
        audioEngine->stopEffect(m_soundGameloseID);
}

void AudioManager::playMaxScore(bool p, bool loop)
{
    if(!m_able && p)return;
    
    if(p)
    {
		if (m_able) audioEngine->setEffectsVolume(1.0);
        audioEngine->stopEffect(m_soundMaxScoreID);
		m_soundMaxScoreID = audioEngine->playEffect(soundMaxScore, loop);
		m_maxScorePlaying = true;
		audioManager->playFilter(false);

		auto callback = CallFunc::create([this]() {
			if (m_able) audioEngine->setEffectsVolume(0.0);
			audioManager->playFilter(true);
			m_maxScorePlaying = false;
		});
		this->runAction(Sequence::create(
			DelayTime::create(1.9),
			callback,
			NULL));
    }
	else
	{
		audioEngine->stopEffect(m_soundMaxScoreID);
		m_maxScorePlaying = false;
	}
}

void AudioManager::setEnable(bool p)
{
    if(m_able != p)
    {
        cocos2d::UserDefault::sharedUserDefault()->setBoolForKey("isaudio", p);
        m_able = p;
    }
    
    if (!p)
    {
        playBGMusic(false);
        playFilter(false);
        playGamelose(false);
        playStartgame(false);

    }
}

void AudioManager::setBackgroundMusicVolume(float v)
{
	
    if (m_able)
        audioEngine->setBackgroundMusicVolume(v);
}

void AudioManager::setEffectsVolume(float v)
{
    if (m_able && !m_maxScorePlaying)
        audioEngine->setEffectsVolume(v);
}

void AudioManager::pauseAllEffects()
{
    audioEngine->pauseAllEffects();
}

void AudioManager::resumeAllEffects()
{
    audioEngine->resumeAllEffects();
}

void AudioManager::bgMusicLoopPlayCallback()
{
	audioEngine->stopBackgroundMusic();

	audioEngine->playBackgroundMusic(musicBGLoop, true);
    
    m_bgMusic2Action = NULL;
}




