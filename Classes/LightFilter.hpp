#ifndef LightFilterBody_hpp
#define LightFilterBody_hpp

#include <stdio.h>
#include "cocos2d.h"

class B2Sprite;
class FightLayer;
class CLightFilter : public cocos2d::CCLayer
{
public:
    CLightFilter();
    ~CLightFilter();
    static CLightFilter* create();
    bool initEx(FightLayer* fightLayer);
    const cocos2d::Vec2& getPosition();
    void setPositionEx(const cocos2d::Vec2&);
    void offsetPosition(float x, float y);
    void movePosition(float x, float y);
    void adjustPosition(float x, float y);
    void unoffsetPosition();
    int lightFilterBallContact(void* lparam);
    
    int playParticle(void* lparam);
    int unplayParticle(void* lparam);
    
    int lightFilterLineContactStart(void* lparam);
    int lightFilterLineContactFinish(void* lparam);
    void setBroundSize(const cocos2d::Size& s);
    const cocos2d::Vec2& getPositionEx(){return m_pos;};
    float comeback(cocos2d::Vec2& pos, float speed);
    void setVisibleEx(bool bl);
    
    //deay
    void lightdeadByContact(void* lparam);
    
    //newGuide
    void checkNewplayerTouch(const cocos2d::Vec2& checkPos);
    void checkNewplayerTouchSchedule();
    
protected:
    B2Sprite* m_leftBall;
    
    B2Sprite* m_rightBall;
    
    B2Sprite* m_filterLine;
    
    cocos2d::Sprite* m_deadAnimSpr = NULL;
    
    cocos2d::Vec2 m_pos;
    
    cocos2d::Size m_broundSize;
    
    FightLayer* m_fightLayer = nullptr;
    
    std::vector<cocos2d::Sprite*> m_parList;
    
    cocos2d::Size m_ballSize;
    cocos2d::Size m_lineSize;
    
    cocos2d::Vec2 m_newplayerCheckPos;
};

#endif /* LightFilterBody_hpp */;
