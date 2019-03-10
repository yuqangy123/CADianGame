//
//  CHomeLayer.hpp
//  CDianGame
//
//  Created by 余强 on 16/3/26.
//
//

#ifndef CHomeLayer_hpp
#define CHomeLayer_hpp

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/UIButton.h"
//#include "ui/widgets/UIImageView.h"

class HomeLayer : public cocos2d::Layer
{
public:
    static HomeLayer *create();
    virtual bool initEx();
    void showComeback(float delayTime, bool b);
    void startGame(Node* nd);
    void startGameTouchCallback(cocos2d::Ref* obj, cocos2d::ui::TouchEventType tp);
    void audioTouchCallback(Ref* obj, cocos2d::ui::TouchEventType tp);
    
    void facebookCallback(cocos2d::Ref* obj, cocos2d::ui::TouchEventType tp);
    void googleCallback(cocos2d::Ref* obj, cocos2d::ui::TouchEventType tp);
    void facebookPhotoShareCallback();
    
    
protected:
    void comeback(float delayTime, bool bPassScore);
    void comebackCallback();
    int first10ScoreNotify(void* lparam);
    void first10ScoreSchedule(Node* nd);
    
protected:
    cocos2d::ui::Widget* m_txt1;
    cocos2d::ui::Widget* m_txt2;
    cocos2d::ui::Widget* m_Image_2;
    cocos2d::ui::Widget* m_startGameBtn;
    cocos2d::Sprite* m_dianliuNode;
    cocos2d::LabelAtlas* m_dazi;
    cocos2d::LabelAtlas* m_xiaozi;
    cocos2d::ui::Button* m_audioBtn;
    cocos2d::ui::Button*  m_fbBtn;
    cocos2d::ui::Button*  m_googleBtn;
    cocos2d::Vec2 m_originalDianliuPos;
    cocos2d::Sprite* m_newScoreNode;
    
    
    bool m_beStart=false;
    
    float m_hvaluet = 1.0;
    
    float m_visible = true;
    
};

#endif /* CHomeLayer_hpp */
