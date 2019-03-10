//
//  LightFactory.hpp
//  CDianGame
//
//  Created by 余强 on 16/3/23.
//
//

#ifndef LightFactory_hpp
#define LightFactory_hpp

#include "cocos2d.h"


class B2Sprite;
class FightLayer;
class LightFactory : public cocos2d::CCNode
{
public:
    static LightFactory *create();
    LightFactory();
    ~LightFactory();
    bool init(FightLayer* fightLayer);
    B2Sprite* addLightBody(float x, float y, int body);
    void destoryBody(B2Sprite* spr);
    void contactLight(int tag);
    void eatLight(B2Sprite* spr);
    void stopAllLight();
    void removeAllLight(bool fadeOut);
    void delLight(int nID);
    
    //查询是否存在light
    bool haveLight(B2Sprite* spr);
    
    
protected:
    FightLayer* m_fightLayer;
    int m_serialTag;
    
    B2Sprite* m_lastLight;
    std::vector<B2Sprite*> m_addBodyList;
};


#endif /* LightFactory_hpp */
