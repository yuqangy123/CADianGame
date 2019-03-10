#include "AppDelegate.h"
#include "HelloWorldScene.h"
#include "MessageQueue.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "Adjust/Adjust2dx.h"
#include "MobClickCpp.h"
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "CAppController.h"
#endif

USING_NS_CC;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
    #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	umeng::MobClickCpp::end();
    #endif
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
void attributionCallbackMethod(AdjustAttribution2dx attribution) {
}
#endif

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLViewImpl::createWithRect("HelloCpp", cocos2d::Rect(0, 0, 720, 1280));
        director->setOpenGLView(glview);
    }

    director->getOpenGLView()->setDesignResolutionSize(720, 1280, ResolutionPolicy::SHOW_ALL);

    // turn on display FPS
    director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

	#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    //add adjust
	std::string appToken = "crxiwbkpxj40";
	std::string environment = AdjustEnvironmentSandbox2dx;//sendbox//AdjustEnvironmentProduction2dx
	
	AdjustConfig2dx adjustConfig = AdjustConfig2dx(appToken, environment);
	adjustConfig.setAttributionCallback(attributionCallbackMethod);
	Adjust2dx::start(adjustConfig);

	//Ôö¼Óadjust log
	adjustConfig.setLogLevel(AdjustLogLevel2dxVerbose); // enable all logging
	adjustConfig.setLogLevel(AdjustLogLevel2dxDebug);   // enable more logging
	adjustConfig.setLogLevel(AdjustLogLevel2dxInfo);    // the default
	adjustConfig.setLogLevel(AdjustLogLevel2dxWarn);    // disable info logging
	adjustConfig.setLogLevel(AdjustLogLevel2dxError);   // disable warnings as well
	adjustConfig.setLogLevel(AdjustLogLevel2dxAssert);  // disable errors as well
	
	//umeng
	MOBCLICKCPP_START_WITH_APPKEY_AND_CHANNEL("5706332767e58e2414000ddf", "gg1001");
	//MOBCLICKCPP_START_WITH_APPKEY("5706332767e58e2414000ddf");
	#endif
	
    // create a scene. it's an autorelease object
    auto scene = HelloWorld::createScene();

    // run
    director->runWithScene(scene);

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    //ios register
    AppDelegateIOS::getInstance();
#endif
    
    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

	#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		Adjust2dx::onPause();
	
		umeng::MobClickCpp::applicationDidEnterBackground();
	#endif

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

	#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		Adjust2dx::onResume();
	
		umeng::MobClickCpp::applicationWillEnterForeground();
	#endif

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}

static AppDelegateIOS* g_AppDelegateIOS = NULL;
AppDelegateIOS* AppDelegateIOS::getInstance()
{
    if(g_AppDelegateIOS == NULL)
    {
        AppDelegateIOS* in = new AppDelegateIOS();
        in->initEx();
    }
    return g_AppDelegateIOS;
}

void AppDelegateIOS::initEx()
{
    MessageQueue->RegisterMsg(MSG_ID_SDK_IOS_FB_SHARE_NOTIFY, CreateEventEx1(&AppDelegateIOS::fbShareNotify) );
}

int AppDelegateIOS::fbShareNotify(void* args)
{
    CAppController::getInstance()->sdkLogin();
}









