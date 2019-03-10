//
//  javaCallControl.hpp
//  CDianGame
//
//  Created by 余强 on 16/3/23.
//
//

#ifndef javaCallControl_hpp
#define javaCallControl_hpp

#include "cocos2d.h"

#define javaCall javaCallControl::InstanceEx()
class javaCallControl
{
public:
    static javaCallControl* InstanceEx();
    
	void setGooglePayBase64EncodedPublicKey(const char* key);
	void setGooglePayPremiumMenuList(const char* list);
	
	void buyCoin(const char* price, cocos2d::CallFunc* successCallback, cocos2d::CallFunc* failedCallback);
	void showAd(const char* type);
	void kochavaEvent(const char* eventname, const char* lparam = "");
	
	void fbsharing();
	
protected:
	void init();
	
public:
	cocos2d::CallFunc* m_paySuccessCallback = NULL;
	cocos2d::CallFunc* m_payFailedCallback = NULL;
};

#endif /* dataManager_hpp */
