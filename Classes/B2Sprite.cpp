//
//  B2Sprite.cpp
//  CDianGame
//
//  Created by 余强 on 16/3/21.
//
//

#ifndef B2Sprite_cpp
#define B2Sprite_cpp

#include "B2Sprite.hpp"
#include "com/microDtata.h"

 


B2Sprite* B2Sprite::createSprite()
{
	B2Sprite *sprite = new (std::nothrow) B2Sprite();
	if (sprite && sprite->init())
	{
		sprite->autorelease();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}

void B2Sprite::initEx()
{

}

void B2Sprite::setPositionEx(float x, float y)
{
    setPosition(x,y);
    //m_b2Body->SetTransform(b2Vec2(x/PTM_RATIO, y/PTM_RATIO),0);
    //m_b2Body->SetAwake(true);
}
void B2Sprite::offsetPosition(float x, float y)
{
    const cocos2d::Vec2& pos = getPosition();
    setPosition(pos.x + x, pos.y + y);
    //m_b2Body->SetTransform(b2Vec2((pos.x + x)/PTM_RATIO, (pos.y + y)/PTM_RATIO),0);
    //m_b2Body->SetAwake(true);
}







#endif /* B2Sprite_cpp */
