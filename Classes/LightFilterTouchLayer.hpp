//
//  LightFilterTouchLayer.hpp
//  CDianGame
//
//  Created by 余强 on 16/3/23.
//
//

#ifndef LightFilterTouchLayer_hpp
#define LightFilterTouchLayer_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "LightFilter.hpp"


class LightFilterTouchLayer : public cocos2d::CCLayer
{
public:
    static LightFilterTouchLayer* create();
    void setLightFilter(CLightFilter* l);
    void initEx();
    void setEnableTouch(bool l);
    void showNewGuide();
    bool isNewGuideTouch(){return m_newGuideTouch;};
    
protected:
    bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event);
    void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event);
    void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event);
    
private:
    CLightFilter* m_lightFilter;
    cocos2d::EventListenerTouchOneByOne* m_touchListener = NULL;
    cocos2d::Sprite*  m_newGuideSpr = NULL;
    int m_touchIndexCnt = 0;
    bool m_newGuideTouch = false;
};

#endif /* LightFilterTouchLayer_hpp */
