//
//  FightLayer.h
//  CDianGame
//
//  Created by 余强 on 16/3/21.
//
//

#ifndef B2Sprite_h
#define B2Sprite_h

#include "cocos2d.h"
#include <Box2D/Box2D.h>


class B2Sprite :  public cocos2d::CCSprite
{
public:
	static B2Sprite* createSprite(); 

    virtual void initEx();
    void setPositionEx(float x, float y);
    void offsetPosition(float x, float y);
    
	cocos2d::PointArray* getRoadPathfunName(void) const { return m_pRoadPath; }
	void setRoadPathfunName(cocos2d::PointArray* var){ m_pRoadPath = var; }

protected:
	cocos2d::PointArray* m_pRoadPath;
};


#endif /* B2Sprite_h */
