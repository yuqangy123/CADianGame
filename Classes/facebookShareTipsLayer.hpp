//
//  NewPlayerGuideLayer.hpp
//  CDianGame
//
//  Created by 余强 on 16/3/23.
//
//

#ifndef facebookShareTipsLayer_hpp
#define facebookShareTipsLayer_hpp

#include "cocostudio/CocoStudio.h"
#include "ui/UIButton.h"
#include "cocos2d.h"


class facebookShareTipsLayer : public cocos2d::CCLayer{
public:
    static facebookShareTipsLayer* create();
    void initEx();
    void show();
    
protected:
    void noTouchCallback(cocos2d::Ref* obj, cocos2d::ui::TouchEventType tp);
    void yesTouchCallback(cocos2d::Ref* obj, cocos2d::ui::TouchEventType tp);
    void destory(cocos2d::Node* nd);
};

#endif /* facebookShareTipsLayer_hpp */
