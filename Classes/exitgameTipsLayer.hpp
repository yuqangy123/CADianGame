//
//  exitgameTipsLayer.hpp
//  CDianGame
//
//  Created by 余强 on 16/3/23.
//
//

#ifndef exitgameTipsLayer_hpp
#define exitgameTipsLayer_hpp

#include "cocostudio/CocoStudio.h"
#include "ui/UIButton.h"
#include "cocos2d.h"


class exitgameTipsLayer : public cocos2d::CCLayer{
public:
    static exitgameTipsLayer* create();
    void initEx();
    
protected:
    void noTouchCallback(cocos2d::Ref* obj, cocos2d::ui::TouchEventType tp);
    void yesTouchCallback(cocos2d::Ref* obj, cocos2d::ui::TouchEventType tp);
    void destory(cocos2d::Node* nd);
};

#endif /* exitgameTipsLayer_hpp */
