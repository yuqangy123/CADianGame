//
//  CLineParticle.cpp
//  CDianGame
//
//  Created by 余强 on 16/3/29.
//
//

#include "CLineParticle.hpp"
#include "ColorBeautifulShader.hpp"

CLineParticle::CLineParticle()
{
    
}

CLineParticle::~CLineParticle()
{
    colorBeautifulShader->delBode(m_particle1);
    colorBeautifulShader->delBode(m_particle2);
}

CLineParticle *CLineParticle::create()
{
    CLineParticle *ret = new (std::nothrow)CLineParticle();
    if (ret && ret->init())
    {
        ret->autorelease();
        ret->initEx();
        return ret;
    }
    else
    {
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
}

void CLineParticle::initEx()
{
    cocos2d::ParticleSystem* particle1 = cocos2d::CCParticleSystemQuad::create("ui/texiao/00.plist");
    addChild(particle1);
    m_particle1 = particle1;
    colorBeautifulShader->addBode(m_particle1);
    
    cocos2d::ParticleSystem* particle2 = cocos2d::CCParticleSystemQuad::create("ui/texiao/01.plist");
    addChild(particle2);
    m_particle2 = particle2;
    colorBeautifulShader->addBode(m_particle2);
    




    //ccBlendFunc cbl = {GL_DST_COLOR, GL_ONE};
    //lightSprite->setBlendFunc(cbl);
    
    //particle1->initWithTotalParticles(150);
    //particle1->stopSystem();
    //particle1->setTotalParticles(particle1->getTotalParticles()/2);
    
}

void CLineParticle::stopSystem()
{
    if(!m_beLife)
        return;
    m_beLife = false;
    m_particle1->stopSystem();
    m_particle2->stopSystem();
    
	runAction(Sequence::create(DelayTime::create(2.0f), CCCallFunc::create(this, callfunc_selector(CLineParticle::delSelfCallback)), NULL));
}
void CLineParticle::delSelfCallback()
{
	this->removeFromParent();
}

void CLineParticle::playSystem()
{
    if(!m_beLife)
        return;
    m_beLife = true;
    
    //m_particle1->setTotalParticles(50);
    m_particle1->resetSystem();
    m_particle2->resetSystem();
}


void CLineParticle::releaseSelf(Node* nd)
{
    removeFromParentAndCleanup(true);
}







