//
//  FightLayer.h
//  CDianGame
//
//  Created by 余强 on 16/3/21.
//
//

#ifndef FightLayer_h
#define FightLayer_h

#include "cocos2d.h"
#include "Box2dHandler.h"
#include "LightFilter.hpp"
#include "LightFactory.hpp"
#include "LightFilterTouchLayer.hpp"
#include "HomeLayer.hpp"


#define common_speed -320.f
class FightLayer : public cocos2d::CCLayer
{
public:
    FightLayer();
    ~FightLayer();
    static FightLayer *create(const cocos2d::Vec2& dianliuPos);
    
    void initEx(const cocos2d::Vec2& dianliuPos);
    void initUI();
    void hided();
    void addDynamicbody(B2Sprite* spr, int shape, void* userdata);
    void addStaticbody(B2Sprite* spr, int shape, void* userdata);
    void initRoadData();
    
    void startGame();
    void startGameNow();
    void update(float dt);
    void makeLightSchedule();
	void makeLightScheduleFirst();
    float getBox2dLayerPos();
    void destoryBody(cocos2d::Sprite *b);
    void contactLight(int tag);
    float getSpeed(){return -m_curSpeed;}
    CLightFilter* getFilterBody(){return m_lightFilter;}
    void showDeadEffect(float pos);
    
    int gameDeadNotify(void* lparam);
    
    int newplayerTouchmoveNotify(void* lparam);
    
    int newplayerCheckLenNotify(void* lparam);
    
    
    void movingGap(Node* nd);
    void showHomeLayer(Node* nd);
    
    //finish break a light
    void filterLightFinsih(int nID);
    
    //play max speed effect
    void playMaxSpeedParticle();
    
    //get original light position
    const cocos2d::Vec2& getOriginalDianliuPos(){return m_dianliuPos;};
    
    void setSpeed(float speed);
    
    LightFactory* getLightFactory(){return m_lightFactory;}
    
protected:
    void pauseBtnTouchCallback(cocos2d::Ref* obj, cocos2d::ui::TouchEventType tp);
    void resumeBtnTouchCallback(cocos2d::Ref* obj, cocos2d::ui::TouchEventType tp);
    
public:
    CC_SYNTHESIZE(Box2dHandler*, m_box2d, Box2D);
    CLightFilter*   m_lightFilter;
    LightFactory*    m_lightFactory;
    
    cocos2d::CCLayer*    m_box2dLayer;
    cocos2d::Action* m_movingGapAction = NULL;
    LightFilterTouchLayer*   m_lightFilterTouchLayer;
    cocos2d::CCSprite*  m_deadLineSpr;
    cocos2d::Sprite*    m_maxSpeedSprite = NULL;
    cocos2d::LabelAtlas* m_scoreText = NULL;
    cocos2d::LabelAtlas* m_scoreTextScale = NULL;
    cocos2d::ui::Button* m_pauseBtn;
    cocos2d::ui::Button* m_resumeBtn;
    cocos2d::LayerColor* m_resumeHideView;
    cocos2d::Sprite*  m_newGuideSpr = NULL;
    float           m_curSpeed = common_speed;
    float           m_moveSize = 0.f;
    float           m_showH = .0f;//当前的高度
    
    cocos2d::Vec2    m_r1Pos;
    cocos2d::Vec2    m_r2Pos;
    cocos2d::Vec2    m_r3Pos;
    cocos2d::Vec2    m_dianliuPos;
    cocos2d::CCRect m_screenSize;
    int             m_score = 0;
    bool            m_maxScoreIndex = false;
    int             m_lastType = 0;
};


#endif /* FightLayer_h */
