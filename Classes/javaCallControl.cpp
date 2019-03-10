//
//  javaCallControl.cpp
//  CDianGame
//
//  Created by 余强 on 16/3/23.
//
//

#include "javaCallControl.hpp"
#include <jni.h>
#include "platform/android/jni/JniHelper.h"
#include "com/MessageQueue.h"

javaCallControl* javaCallControl::InstanceEx() 
{ 
	static javaCallControl* ptr = nullptr; 
	if (nullptr == ptr) 
	{ 
		ptr = new javaCallControl(); 
		ptr->init(); 
	} 
	return ptr; 
}

void javaCallControl::init()
{
    
    
}

void javaCallControl::setGooglePayBase64EncodedPublicKey(const char* key)
{
	return;
	cocos2d::JniMethodInfo info;
    bool ret = cocos2d::JniHelper::getStaticMethodInfo(info,"org/cocos2dx/cpp/AppActivity","setGooglePayBase64EncodedPublicKey","(Ljava/lang/String;Ljava/lang/String;)I");
    if(ret)
    {
		jstring jkey = info.env->NewStringUTF(key);
		jstring jval = info.env->NewStringUTF("");
		info.env->CallStaticIntMethod(info.classID,info.methodID, jkey, jval);
    }
}

void javaCallControl::setGooglePayPremiumMenuList(const char* list)
{
	return;
	cocos2d::JniMethodInfo info;
    bool ret = cocos2d::JniHelper::getStaticMethodInfo(info,"org/cocos2dx/cpp/AppActivity","setGooglePayPremiumMenuList","(Ljava/lang/String;Ljava/lang/String;)I");
    if(ret)
    {
		jstring jkey = info.env->NewStringUTF(list);
		jstring jval = info.env->NewStringUTF("");
		info.env->CallStaticIntMethod(info.classID,info.methodID, jkey, jval);
    }
}




void javaCallControl::buyCoin(const char* price, cocos2d::CallFunc* successCallback, cocos2d::CallFunc* failedCallback)
{
	cocos2d::JniMethodInfo info;
    bool ret = cocos2d::JniHelper::getStaticMethodInfo(info,"org/cocos2dx/cpp/AppActivity","buyCoin","(Ljava/lang/String;Ljava/lang/String;)I");
    if(ret)
    {
		jstring jkey = info.env->NewStringUTF(price);
		jstring jval = info.env->NewStringUTF("");
		info.env->CallStaticIntMethod(info.classID,info.methodID, jkey, jval);
    }
	
	m_paySuccessCallback = successCallback;
	m_paySuccessCallback->retain();
	m_payFailedCallback =  failedCallback;
	m_payFailedCallback->retain();
}

void javaCallControl::kochavaEvent(const char* eventname, const char* lparam)
{
	cocos2d::JniMethodInfo info;
    bool ret = cocos2d::JniHelper::getStaticMethodInfo(info,"org/cocos2dx/cpp/AppActivity","kochavaEvent","(Ljava/lang/String;Ljava/lang/String;)I");
    if(ret)
    {
		jstring jkey = info.env->NewStringUTF(eventname);
		jstring jval = info.env->NewStringUTF(lparam);
		info.env->CallStaticIntMethod(info.classID,info.methodID, jkey, jval);
    }
}

void javaCallControl::showAd(const char* type)
{
	cocos2d::JniMethodInfo info; 
    bool ret = cocos2d::JniHelper::getStaticMethodInfo(info,"org/cocos2dx/cpp/AppActivity","showAd","(Ljava/lang/String;Ljava/lang/String;)I");
    if(ret)
    {
		jstring jkey = info.env->NewStringUTF(type);
		jstring jval = info.env->NewStringUTF("");
		info.env->CallStaticIntMethod(info.classID,info.methodID, jkey, jval);
    }
}

void javaCallControl::fbsharing()
{
	cocos2d::JniMethodInfo info; 
    bool ret = cocos2d::JniHelper::getStaticMethodInfo(info,"org/cocos2dx/cpp/AppActivity","fbsharing","(Ljava/lang/String;Ljava/lang/String;)I");
    if(ret)
    {
		jstring jkey = info.env->NewStringUTF("");
		jstring jval = info.env->NewStringUTF("");
		info.env->CallStaticIntMethod(info.classID,info.methodID, jkey, jval);
    }
}

extern "C" 
{
    JNIEXPORT void JNICALL  Java_org_cocos2dx_cpp_AppActivity_PaySuccess(JNIEnv *env, jobject thiz, jstring jstr)
    {
        auto strOpenId = env->GetStringUTFChars(jstr, NULL);
        env->ReleaseStringUTFChars(jstr, strOpenId);

		  //支付成功
			if(javaCall->m_paySuccessCallback)
			{
				javaCall->m_paySuccessCallback->execute();
				javaCall->m_paySuccessCallback->release();
				javaCall->m_paySuccessCallback = NULL;
			}	
			if(javaCall->m_payFailedCallback)
			{
				javaCall->m_payFailedCallback->release();
				javaCall->m_payFailedCallback = NULL;
			}	
    }
	JNIEXPORT void JNICALL  Java_org_cocos2dx_cpp_AppActivity_PayFailed(JNIEnv *env, jobject thiz, jstring jstr)
    {
        auto strOpenId = env->GetStringUTFChars(jstr, NULL);
        env->ReleaseStringUTFChars(jstr, strOpenId);

		  //支付失败
			if(javaCall->m_payFailedCallback)
			{
				javaCall->m_payFailedCallback->execute();
				javaCall->m_payFailedCallback->release();
				javaCall->m_payFailedCallback = NULL;
			}	
			if(javaCall->m_paySuccessCallback)
			{
				javaCall->m_paySuccessCallback->release();
				javaCall->m_paySuccessCallback = NULL;
			}	
	}
	
	JNIEXPORT void JNICALL  Java_org_cocos2dx_cpp_AppActivity_WatchShopVideoSuccess(JNIEnv *env, jobject thiz, jstring jstr)
    {
        auto strOpenId = env->GetStringUTFChars(jstr, NULL);
        env->ReleaseStringUTFChars(jstr, strOpenId);

		  //商店视频成功
			//MessageQueue->SendMsg(MSG_ID_VIDEO_WATCH_SHOP_SUCCESS, NULL);
    }
	JNIEXPORT void JNICALL  Java_org_cocos2dx_cpp_AppActivity_WatchShopVideoFailed(JNIEnv *env, jobject thiz, jstring jstr)
    {
        auto strOpenId = env->GetStringUTFChars(jstr, NULL);
        env->ReleaseStringUTFChars(jstr, strOpenId);

		  //商店视频失败
			//MessageQueue->SendMsg(MSG_ID_VIDEO_WATCH_SHOP_FAILED, NULL);	
	}
	
	JNIEXPORT void JNICALL  Java_org_cocos2dx_cpp_AppActivity_WatchResurreVideoSuccess(JNIEnv *env, jobject thiz, jstring jstr)
    {
        auto strOpenId = env->GetStringUTFChars(jstr, NULL);
        env->ReleaseStringUTFChars(jstr, strOpenId);

		  //复活视频成功
			//MessageQueue->SendMsg(MSG_ID_VIDEO_WATCH_RESURRE_SUCCESS, NULL);
    }
	JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_AppActivity_WatchResurreVideoFailed(JNIEnv *env, jobject thiz, jstring jstr)
    {
        auto strOpenId = env->GetStringUTFChars(jstr, NULL);
        env->ReleaseStringUTFChars(jstr, strOpenId);

		  //复活视频失败
			//MessageQueue->SendMsg(MSG_ID_VIDEO_WATCH_RESURRE_FAILED, NULL);	
	}
}
