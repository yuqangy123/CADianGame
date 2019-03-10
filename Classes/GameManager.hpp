//
//  GameManager.hpp
//  CDianGame
//
//  Created by 余强 on 16/3/26.
//
//

#ifndef GameManager_hpp
#define GameManager_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "com/microDtata.h"

typedef enum{
    ready,
    playing,
    dead
}GAME_STATE_ID;

#define gameManager CGameManager::handler()
class CGameManager : cocos2d::Node
{
public:
    CGameManager();
    ~CGameManager();
    static CGameManager* handler();
    void initEx();
    bool isPlaying(){return m_gameState == GAME_STATE_ID::playing;};
    void playing(){m_gameState = GAME_STATE_ID::playing; ++m_startTimes;};
    void dead(){m_gameState = GAME_STATE_ID::dead;};
    void setLastScore(int n);
    int getLastScore(){return m_lastScore;};
    void setMaxScore(int);
    int getMaxScore(){return m_maxScore;};
    int shockScreen(void* lparam);
    void pauseGame(bool p){m_pause = p;};
    bool isPauseGame(){return m_pause;};
    //bool isFirstPlay(){return m_firstPlay;};
    
    bool isFirstPlay(){return m_startGlobalTimes == 1;};
    
    //The number of the start game to play
    int getPlayTimes(){return m_startTimes;}
    
    //show ad
	void showAd(int type);
	void hideAd();
    
    bool isTipsGoogleInvite();
    void setTipsGoogleInvite();
    
protected:
    CC_SYNTHESIZE(int, m_gameState, GameState);
    int m_lastScore = 0;
    int m_maxScore = 0;
    bool m_pause = false;
    int  m_startGlobalTimes = 0;
    int m_startTimes = 0;
    
    int m_playDays = 0;
    int m_playYear = 0;
    int m_playMonth = 0;
    int m_playDay = 0;
    bool m_tipsGgInvite = false;
    
    CC_SYNTHESIZE(cocos2d::Layer*, m_rootLayer, RootLayer);
    CC_SYNTHESIZE(cocos2d::Layer*, m_bgLayer, BgLayer);
    CC_SYNTHESIZE(cocos2d::Layer*, m_homeLayer, HomeLayer);
    CC_SYNTHESIZE(cocos2d::Layer*, m_fightLayer, FightLayer);
    CC_SYNTHESIZE(cocos2d::Layer*, m_newguideLayer, NewGuideLayer);
};
#endif /* GameManager_hpp */
