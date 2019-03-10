//
//  NewPlayerGuideLayer.hpp
//  CDianGame
//
//  Created by 余强 on 16/3/23.
//
//

#ifndef inviteShareTipsLayer_hpp
#define inviteShareTipsLayer_hpp

#include "cocostudio/CocoStudio.h"
#include "ui/UIButton.h"
#include "cocos2d.h"


class inviteShareTipsLayer : public cocos2d::CCLayer
{
public:
    static inviteShareTipsLayer* create();
    void initEx();
    
protected:
    void showYesCallback(Node* tp);
    void showNoCallback(Node* tp);
    
    void yesEnjoyTouchCallback(Ref* obj, cocos2d::ui::TouchEventType tp);
    void noEnjoyTouchCallback(Ref* obj, cocos2d::ui::TouchEventType tp);
    
    void yesYesTouchCallback(Ref* obj, cocos2d::ui::TouchEventType tp);
    void noYesTouchCallback(Ref* obj, cocos2d::ui::TouchEventType tp);
    
    void yesNoTouchCallback(Ref* obj, cocos2d::ui::TouchEventType tp);
    void noNoTouchCallback(Ref* obj, cocos2d::ui::TouchEventType tp);
    
    
    void destory(Node* nd);
    
protected:
    cocos2d::ui::Widget* m_Panel_enjoy;
    cocos2d::ui::Widget* m_Panel_yes;
    cocos2d::ui::Widget* m_Panel_no;
};

#endif /* inviteShareTipsLayer_hpp */
