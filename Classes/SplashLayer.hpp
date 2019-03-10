//
//  SplashLayer.hpp
//  CDianGame
//
//  Created by 余强 on 16/3/26.
//
//

#ifndef SplashLayer_hpp
#define SplashLayer_hpp

#include "cocos2d.h"
class SplashLayer : public cocos2d::Layer
{
public:
    static SplashLayer *create();
    virtual bool initEx();
    static void showHomelayer();
	
protected:
	void comebackCallback();
	void homelayerCallback();
	
    cocos2d::Sprite* logo;
	cocos2d::LayerColor* bg;
    
};

#endif /* SplashLayer_hpp */
