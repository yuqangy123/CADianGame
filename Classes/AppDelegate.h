#ifndef  _APP_DELEGATE_H_
#define  _APP_DELEGATE_H_

#include "cocos2d.h"



/**
@brief    The cocos2d Application.

The reason for implement as private inheritance is to hide some interface call by Director.
*/
class  AppDelegate : private cocos2d::Application
{
public:
    AppDelegate();
    virtual ~AppDelegate();

    virtual void initGLContextAttrs();

    /**
    @brief    Implement Director and Scene init code here.
    @return true    Initialize success, app continue.
    @return false   Initialize failed, app terminate.
    */
    virtual bool applicationDidFinishLaunching();

    /**
    @brief  The function be called when the application enter background
    @param  the pointer of the application
    */
    virtual void applicationDidEnterBackground();

    /**
    @brief  The function be called when the application enter foreground
    @param  the pointer of the application
    */
    virtual void applicationWillEnterForeground();
    
    //void setAppController(CAppController* app){};
    
};

class AppDelegateIOS : cocos2d::Node
{
public:
    static AppDelegateIOS* getInstance();
    
protected:
    void initEx();
    int fbShareNotify(void* args);
};

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <jni.h>

extern "C"  
{  
	//JNIEXPORT void JNICALL Java_com_mc_ecrush_JniTestHelper_backActionA(JNIEnv *env, jobject thiz, jstring key, jstring message);
}
#endif 

#endif /* _APP_DELEGATE_H_ */