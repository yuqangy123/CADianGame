//
//  Box2dHandle.h
//  CDianGame
//
//  Created by 余强 on 16/3/21.
//
//

#ifndef Box2dHandle_h
#define Box2dHandle_h

#include "cocos2d.h"
#include "B2Sprite.hpp"
#include <vector>
#include "com/microDtata.h"

typedef struct body {
    cocos2d::Sprite* sprite;
    BodyType type;
    cocos2d::Size size;
    cocos2d::Vec2 pos;
    
    body()
    {
        sprite = NULL;
        type = BodyType::none;
    }
    
    inline body& operator =(const body& v)
    {
        sprite = v.sprite;
        type = v.type;
        size = v.size;
        pos = v.pos;
		return *this;
    }
    
    inline void copy(const body& v)
    {
        sprite = v.sprite;
        type = v.type;
        size = v.size;
        pos = v.pos;
    }
};
class Box2dHandler :  public cocos2d::Layer
{
public:
    Box2dHandler();
    ~Box2dHandler();
    static Box2dHandler* create();
    
    void initEx();
    
    
    //将物理世界的精灵创建Box2D物体
    void creatDynamicbody(B2Sprite *sp, int shape, void* userData = NULL);
    void createStaticbody(B2Sprite* sp, int shape, void* userData = NULL);
    
    void start();
    void stop();
    
    void update(float dt);
    
    void destoryBody(cocos2d::Sprite *b);
    
    void initState();
    
public:
    std::vector<body> m_body;
    body m_filterLine;
    body m_filterBallL;
    body m_filterBallR;
    
    bool m_lineContacting = false;
    body m_contactLightBody;
    
    cocos2d::Vec2 linepos;
    cocos2d::Vec2 ballLpos;
    cocos2d::Vec2 ballRpos;
    //cocos2d::Vec2 lightpos;
    
    cocos2d::Size m_ballSize;
    cocos2d::Size m_lineSize;
    cocos2d::Size m_light1Size;
    cocos2d::Size m_light2Size;
};



#endif /* Box2dHandle_h */
