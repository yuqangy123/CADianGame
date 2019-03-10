//
//  display.hpp
//  CDianGame
//
//  Created by 余强 on 16/3/22.
//
//

#ifndef display_hpp
#define display_hpp

#include "cocos2d.h"



class CDisplay
{
    
public:
    CDisplay();
    ~CDisplay();
    static CDisplay* getInstance();
    void setDesignResolution();
    
    
private:
    void setAutoScale();
    
    
public:
	cocos2d::Vec2 leftTop;
	cocos2d::Vec2 leftBottom;
	cocos2d::Vec2 rightTop;
	cocos2d::Vec2 rightBottom;
	cocos2d::Vec2 leftCenter;
	cocos2d::Vec2 rightCenter;
	cocos2d::Vec2 topCenter;
	cocos2d::Vec2 bottomCenter;
	cocos2d::Vec2 center;

    cocos2d::Size m_frameSize;
    float m_contentScaleFactor;
    cocos2d::Size m_winSize;
	cocos2d::Vec2 m_visibleOrigin;

	float m_fLineOffsetY = 0.0;
};

#define sdisplay CDisplay::getInstance()
#endif /* display_hpp */



