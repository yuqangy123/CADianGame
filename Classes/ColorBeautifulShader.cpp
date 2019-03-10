//
//  ColorBeautifulLayer.cpp
//  CDianGame
//
//  Created by 余强 on 16/3/29.
//
//

#include "ColorBeautifulShader.hpp"



using namespace cocos2d;

CColorBeautifulShader::CColorBeautifulShader()
{
    
}

CColorBeautifulShader::~CColorBeautifulShader()
{
    
}


CColorBeautifulShader *CColorBeautifulShader::getInstance()
{
    static CColorBeautifulShader *ret = nullptr;
    
    if (nullptr == ret)
    {
        ret = new (std::nothrow)CColorBeautifulShader();
        Director::getInstance()->getRunningScene()->addChild(ret);//限制在当前场景
        ret->initEx();
    }
    return ret;
}

void CColorBeautifulShader::initEx()
{
    Sequence *actiontest = cocos2d::Sequence::create(
                                                               DelayTime::create(0.2),
                                                               CallFuncN::create(this, callfuncN_selector(CColorBeautifulShader::updateColor)),
                                                               NULL);
    this->runAction( RepeatForever::create(actiontest) );
}

void CColorBeautifulShader::addBode(cocos2d::Node* nd)
{
    if (nullptr == nd)
        return;
    
    cocos2d::GLProgram* shader = GLProgram::createWithFilenames("shader/hsb.vsh", "shader/hsb.fsh");
    auto state = GLProgramState::create(shader);
    nd->setGLProgramState(state);
    state->setUniformFloat("HValue", m_hvaluet);
    
    m_nodeList.push_back(nd);
}

void CColorBeautifulShader::addLightBode(cocos2d::Node* nd)
{
    cocos2d::GLProgram* shader = GLProgram::createWithFilenames("shader/lighthide.vsh", "shader/lighthide.fsh");
    auto state = GLProgramState::create(shader);
    nd->setGLProgramState(state);
    
    state->setUniformFloat("filterValue", 0.0);
    state->setUniformFloat("HValue", m_hvaluet);
    
    m_nodeList.push_back(nd);
}

void CColorBeautifulShader::addImageviewBode(cocos2d::ui::ImageView* nd)
{
    
}



void CColorBeautifulShader::delBode(cocos2d::Node* nd)
{
    for (auto itr = m_nodeList.begin(); itr != m_nodeList.end(); ++itr)
    {
        if ( (*itr) == nd )
        {
            m_nodeList.erase(itr);
            break;
        }
    }
}

void CColorBeautifulShader::updateColor(Node* nd)
{
    m_hvaluet = m_hvaluet + m_addv;
    if (m_hvaluet > 360.0)
    {
        m_addv = -m_addv;
        m_hvaluet = 360.0;
    }
    else if(m_hvaluet < 0.0)
    {
        m_addv = -m_addv;
        m_hvaluet = 0.0;
    }
    
    for (auto itr = m_nodeList.begin(); itr != m_nodeList.end(); ++itr)
    {
        auto state = (*itr)->getGLProgramState();
        state->setUniformFloat("HValue", m_hvaluet);
    }
}











