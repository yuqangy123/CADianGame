//
//  NewPlayerGuideLayer.hpp
//  CDianGame
//
//  Created by 余强 on 16/3/23.
//
//

#ifndef NewPlayerGuideLayer_hpp
#define NewPlayerGuideLayer_hpp

#include <stdio.h>
#include "cocos2d.h"


class NewPlayerGuideLayer : public cocos2d::CCLayer
{
public:
    static NewPlayerGuideLayer* create();
    void initEx();
    
protected:
    bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event);
    void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event);
    void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event);
    
private:
    cocos2d::EventListenerTouchOneByOne* m_touchListener;
};

#endif /* NewPlayerGuideLayer_hpp */
