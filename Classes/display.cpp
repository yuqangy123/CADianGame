//
//  display.cpp
//  CDianGame
//
//  Created by 余强 on 16/3/22.
//
//

#include "display.hpp"
#include "com/microDtata.h"
USING_NS_CC;

Director* director ;
GLView* glview ;
Size framesize;
TextureCache* textureCache;
SpriteFrameCache* spriteFrameCache;
AnimationCache* animationChache;

CDisplay::CDisplay()
{
    director = Director::getInstance();
    glview = director->getOpenGLView();
    if (!glview)
    {
        glview = GLViewImpl::createWithRect("Cocos2d-c", Rect(0, 0, stage_width, stage_height));
        director->setOpenGLView(glview);
    }
    
    
    framesize = glview->getFrameSize();
    textureCache = director->getTextureCache();
    spriteFrameCache = SpriteFrameCache::getInstance();
    animationChache = AnimationCache::getInstance();
}

CDisplay::~CDisplay()
{
    
}

CDisplay* CDisplay::getInstance()
{
    static CDisplay* handler = nullptr;
    if (handler == nullptr){
        handler = new CDisplay();
        return handler;
    }
    else{
        return handler;
    }
}

void CDisplay::setAutoScale()
{
    
}

void CDisplay::setDesignResolution()
{
    glview->setDesignResolutionSize(stage_width, stage_height,
                                        ResolutionPolicy::NO_BORDER);
    
    auto sizeInPixels = glview->getFrameSize();
    auto viewsize = director->getWinSize();
    
	float scaleX = glview->getScaleX();
	float scaleY = glview->getScaleY();
    
	m_visibleOrigin = glview->getVisibleOrigin();

    m_frameSize.width = sizeInPixels.width/scaleX;
    m_frameSize.height = sizeInPixels.height/scaleY;
    m_contentScaleFactor = director->getContentScaleFactor();
    m_winSize.width = viewsize.width;
    //m_winSize.height = viewsize.height;
	m_winSize.height = (viewsize.height*scaleY - (viewsize.height*scaleY - sizeInPixels.height) )/scaleY;
	


    //实际可视屏幕显示Rect(即适配后，中间可以显示到屏幕的区域)
    leftBottom = Vec2(0, 0);
	leftTop = Vec2(0, m_winSize.height + m_visibleOrigin.y);
	rightTop = Vec2(m_winSize.width + m_visibleOrigin.x, m_winSize.height + m_visibleOrigin.y);
	rightBottom = Vec2(rightTop.x, 0);
	leftCenter = Vec2(leftTop.x, leftTop.y / 2);
	rightCenter = Vec2(rightBottom.x, leftCenter.y);
	topCenter = Vec2(rightTop.x/2, rightTop.y);
	bottomCenter = Vec2(rightTop.x / 2, 0);
	center = Vec2(rightCenter.x/2, topCenter.y/2);

	m_fLineOffsetY = 10.0f;

	CCLOG("display,%f,%f - %f,%f - %f,%f - %f,%f,%f ", sizeInPixels.width, sizeInPixels.height, 
		viewsize.width, viewsize.height, m_visibleOrigin.x, m_visibleOrigin.y, scaleX, scaleY, m_contentScaleFactor);
}



