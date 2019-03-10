#pragma once
//#include "common.h"
#include <map>
#include "cocos2d.h"

typedef enum{
	MSG_ID_NONE = 0,
    MSG_ID_SHOW_LAYER,
    MSG_ID_LIGHTFILTER_BALL_CONTACT,
    MSG_ID_LIGHTFILTER_LINE_CONTACT,
    MSG_ID_LIGHTFILTER_LINE_CONTACT_FINISH,
    MSG_ID_GAME_LOSE_NOTIFY,
    MSG_ID_SHOCK_SCREEN_NOTIFY,
    MSG_ID_NEWPLAYER_GUIDE_TOUCHMOVE_NOTIFY,
    MSG_ID_NEWPLAYER_CHECK_LEN_NOTIFY,
    MSG_ID_FIRST_SCORE_10_NOTIFY,
    MSG_ID_SDK_IOS_FB_SHARE_NOTIFY,
	MSG_ID_MAX
}CUSTOM_MESSAGE_ID;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#define CNotifyEventEx1 CNotifyEvent<cocos2d::Node, void*>

#define CreateEventEx1(NotifyFunc) CNotifyEventEx1(this, static_cast<CNotifyEventEx1::pNotifyFunc>(NotifyFunc))

template<class classType, class pParam>
class CNotifyEvent
{
public:
	typedef int (classType::*pNotifyFunc)(pParam lParam);
	CNotifyEvent(classType* Owner=NULL, const pNotifyFunc pFun=NULL);
	void Notify(pParam nParam);
	void operator=(const CNotifyEvent &pEvent);

private:
	pNotifyFunc m_pFunc;
	classType* m_pOwner;
};

#define MessageQueue CYMessageQueue::InstanceEx()
class CYMessageQueue
{
public:
	CYMessageQueue();
	~CYMessageQueue();
    static CYMessageQueue* InstanceEx(){static CYMessageQueue* ptr = nullptr; if(nullptr == ptr)ptr = new CYMessageQueue(); return ptr;}

	void RegisterMsg(CUSTOM_MESSAGE_ID nID, const CNotifyEventEx1& NotifyEvent);
	void UnRegisterMsg(CUSTOM_MESSAGE_ID nID, const CNotifyEventEx1& NotifyEvent);
	void SendMsg(CUSTOM_MESSAGE_ID nID, void* param1);
	void Clear();

private:
	std::map<int,CNotifyEventEx1*>	m_MsgList;
};


template<class classType, class param>
CNotifyEvent<classType, param>::CNotifyEvent(classType *Owner/*=NULL*/, const pNotifyFunc pFun/*=NULL*/)
{
	m_pOwner = Owner;
	m_pFunc = pFun;
}

template<class classType, class param>
void CNotifyEvent<classType, param>::Notify(param nParam)
{
	if(m_pOwner && m_pFunc)
		(m_pOwner->*m_pFunc)(nParam);
}

template<class classType, class param>
void CNotifyEvent<classType, param>::operator=(const CNotifyEvent &pEvent)
{
	this->m_pOwner = pEvent.m_pOwner;
	this->m_pFunc = pEvent.m_pFunc;
}
