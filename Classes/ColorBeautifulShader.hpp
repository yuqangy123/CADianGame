//
//  ColorBeautifulLayer.hpp
//  CDianGame
//
//  Created by 余强 on 16/3/29.
//
//

#ifndef ColorBeautifulLayer_hpp
#define ColorBeautifulLayer_hpp


#include "cocos2d.h"
#include "ui/UIImageView.h"

#endif /* ColorBeautifulLayer_hpp */


#define colorBeautifulShader CColorBeautifulShader::getInstance()

class CColorBeautifulShader : public cocos2d::Node
{
    
public:
    CColorBeautifulShader();
    ~CColorBeautifulShader();
    static CColorBeautifulShader* getInstance();
    void initEx();
    void addBode(cocos2d::Node* nd);
    void addImageviewBode(cocos2d::ui::ImageView* nd);
    void addLightBode(cocos2d::Node* nd);
    void delBode(cocos2d::Node* nd);
    void updateColor(Node*);
    
protected:
    float m_hvaluet = 1.0;
    float m_addv = 1.0;
    std::vector<cocos2d::Node*> m_nodeList;
};