//
//  FightLayer.h
//  CDianGame
//
//  Created by 余强 on 16/3/21.
//
//

#ifndef lightSprite_h
#define lightSprite_h

#include "cocos2d.h"
#include "B2Sprite.hpp"
#include <string>

class CLightFilter;
class FightLayer;
class lightSprite : public B2Sprite
{
public:
    lightSprite();
    ~lightSprite();
    virtual void initEx(int);
    void eat();
    void beEated();
    void beDead(int type);
    void update(float f);
    void setFilterBody(CLightFilter* f);
    void setFightLayer(FightLayer* f){m_fightlayer = f;};
    void showDeadEffect();
    void delSelf();
    void setContacted(bool b){m_contacted = b;};
    bool isLife(){return m_isLife;}
    bool isEatingNormal();
    
    void setLightID(int n){m_lightID = n;}
    int getLightID(){return m_lightID;}
	
	void setImageName(const std::string& name){m_imageName = name;}
    
protected:
    bool  m_beEatting = false;
    bool  m_isLife = true;
    int  m_fromTop = 0;
    
    bool m_contacted = false;
    CLightFilter* m_filter;
    FightLayer* m_fightlayer;
    
    float m_filterV = 0.0f;
    
    int m_lightID = 0;
	
	std::string m_imageName;
};


#endif /* lightSprite_h */
