#ifndef BackgroundLayer_hpp
#define BackgroundLayer_hpp

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
//#include "ui/widgets/UIImageView.h"

class backgroundMoveLayer : public cocos2d::Layer
{
public:
	static backgroundMoveLayer *create(int id);
	virtual bool initEx(int id);
	void addMoveBg();
	void move(bool b);
	void setSpeed(float f);

protected:
	std::list<cocos2d::Sprite*> m_list;
	float m_speed = 1.0f;
	int m_id = 0;
	cocos2d::Action * m_moveAction = NULL;
};

class BackgroundLayer : public cocos2d::Layer
{
public:
    static BackgroundLayer *create();
    virtual bool initEx();
    void image3RandomMoveCallback(Node* nd);
    void initsize();
    int showLayerNotify(void* lparam);
    void BgChangeCallback(Node* nd);
    
protected:
    cocos2d::Sprite* m_image3;
    cocos2d::CCRect m_screenSize;
	backgroundMoveLayer* m_bg1;
	backgroundMoveLayer* m_bg2;
	backgroundMoveLayer* m_bg3;
	cocos2d::Action* m_action = NULL;
    int m_bgChangeIndex = 1;
    bool m_bChange = false;
};

#endif /* BackgroundLayer_hpp */
