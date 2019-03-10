//
//  AudioManager.hpp
//  CDianGame
//
//  Created by 余强 on 16/3/31.
//
//

#ifndef AudioManager_hpp
#define AudioManager_hpp

#include <cocos2d.h>

#define audioManager AudioManager::getInstance()
class AudioManager : cocos2d::Node
{
public:
    static AudioManager* getInstance();
    void initEx();
    
	void playMainPageMusic(bool p, bool loop);
    void playBGMusic(bool p, bool loop);
    void playFilter(bool p, bool loop);
    void playStartgame(bool p, bool loop);
    void playGamelose(bool p, bool loop);
    void playMaxScore(bool p, bool loop);
    
    void setEnable(bool p);
    bool getEnable(){return m_able;};
    void setBackgroundMusicVolume(float v);
    void setEffectsVolume(float v);
    
    void pauseAllEffects();
    void resumeAllEffects();
    
    void bgMusicLoopPlayCallback();
    
protected:
    int m_soundFilterID = 0;
    int m_soundStartGameID = 0;
    int m_soundGameloseID = 0;
    int m_soundMaxScoreID = 0;
    bool m_able = true;
	bool m_maxScorePlaying = false;
    cocos2d::Action* m_bgMusic2Action = NULL;
};

#endif /* AudioManager_hpp */
