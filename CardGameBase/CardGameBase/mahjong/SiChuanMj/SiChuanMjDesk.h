#pragma once

#include "core/LClockActive.h"
#include "core/LDicePair.h"

#include "mahjong/LMjPlayer.h"
#include "mahjong/LMjLogic.h"
#include "mahjong/LMjDesk.h"

/************************************************************************/
/* 
	������齫��Ϸ������
*/
/************************************************************************/

//enum E_GameState
//{
//	EGS_Ready,
//	EGS_Begin,
//	EGS_ReadyRolateDice,
//	EGS_RolatingDice,         //ing
//	EGS_TBA,                 //to be assured,��ȱ
//	EGS_Playing,
//	EGS_Over
//};

class CLGameDispatcher;

// ����ͨ�������Ĵ��齫
class CSiChuanMjDesk : public CLMjDesk
{
public:
	CSiChuanMjDesk();
	virtual ~CSiChuanMjDesk();

	//////////////////////////////////////////////////////////////////////////
	// ��Ϸ�ڲ��߼�
	// �����齫�ӣ��Ĵ��齫ֻ������Ͳ
	virtual void allocation();
	virtual int mjNumAllocation() const { return 108; }
	int surplusCards() { return m_nIndexCurrent > m_nIndexStart 
		? (m_nIndexCurrent - m_nIndexStart) : (m_nIndexCurrent + mjNumAllocation() - m_nIndexStart); }

	bool selectActInfo(int nResponseUser, int nActiveUser, T_MjActInfo* pActInfo , CLMjCard cardDest, unsigned short usIgnoreFlags = 0);


	//////////////////////////////////////////////////////////////////////////
	// ϵͳ�¼�
	virtual void onEventReady();
	virtual void onEventGameBegin();		// ��Ϸ��ʼ
	virtual void onEventCutCards();			// ҡ��������
	virtual void onEventDealCards();		// ������
	virtual void onEventDingQue();			// ��ȱ

	virtual void onEventFristGotActiveUser();
	virtual void onEventAppointDrawCardUser();
	virtual void onEventAppointActiveUser();			// ָ����û�����û������Ƴ��ơ�
	//virtual void onEventResponseToActiveUser();			// ��ǰ�ɶԻ��ҵĶ�����Ӧ



	//////////////////////////////////////////////////////////////////////////
	// �㲥�û��ӿ�
	virtual void onMsgReady(int nChairID) = 0;
	virtual void onMsgBegin(/*�û�����*/) = 0;
	virtual void onMsgCutCards(int dice1, int dice2) = 0;
	virtual void onMsgDealCards(T_MsgDealCards tMsgDealCards) = 0;
	virtual void onMsgDingQueBegin() = 0;
	virtual void onMsgDingQue() = 0;
	virtual void onMsgAppointActiveUser(T_MsgAppointActiveUser tMsgAppointActiveUser) = 0;


	//////////////////////////////////////////////////////////////////////////
	//�û������ӿ�
	//void onUserEnter(int nChairID);
	//void onUiSitDown();
	void onUserReady(int nChairID);
	void onUserTBA(int nCardColor, int nChairID);

protected:
	CLGameDispatcher* _gameDispatcher;
	//E_GameState _nGameState;

};

