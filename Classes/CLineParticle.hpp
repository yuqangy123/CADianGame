//
//  CLineParticle.hpp
//  CDianGame
//
//  Created by 余强 on 16/3/29.
//
//

#ifndef CLineParticle_hpp
#define CLineParticle_hpp

#include "cocos2d.h"
using namespace cocos2d;

class CLineParticle : public Sprite
{
public:
    CLineParticle();
    ~CLineParticle();
    static CLineParticle *create();
    void initEx();
    void stopSystem();
    void playSystem();
    void setLightID(int n){m_lightID = n;};
    int getLightID(){return m_lightID;};
    void releaseSelf(Node* nd);
    
	void delSelfCallback();

protected:
    cocos2d::ParticleSystem* m_particle1;
    cocos2d::ParticleSystem* m_particle2;
    int m_lightID = 0;
    bool m_beLife = true;
};


#endif /* CLineParticle_hpp */
