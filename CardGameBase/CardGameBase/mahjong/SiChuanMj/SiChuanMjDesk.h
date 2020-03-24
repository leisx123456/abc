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
	virtual int mjNum() const { return 108; }




	//////////////////////////////////////////////////////////////////////////
	// ϵͳ�¼�
	virtual void onEventGameBegin();		// ��Ϸ��ʼ
	virtual void onEventCutCards();			// ҡ��������
	virtual void onEventDealCards();		// ������
	virtual void onEventDingQue();			// ��ȱ
	virtual void onEventAppointActiveUser();			// ָ����û�����û������Ƴ��ơ�



	//////////////////////////////////////////////////////////////////////////
	// �㲥�û��ӿ�
	virtual void onMsgBegin(/*�û�����*/) = 0;
	virtual void onMsgCutCards(int dice1, int dice2) = 0;
	virtual void onMsgDealCards(CLMjCard pArrCardHand[][14], int nPlayerCount) = 0;
	virtual void onMsgDingQue() = 0;


	//////////////////////////////////////////////////////////////////////////
	//�û������ӿ�
	void onUserEnter(int nChairID);
	void onUserReady(int nChairID);
	void onUserTBA(int nCardColor, int nChairID);

protected:
	CLGameDispatcher* _gameDispatcher;
	//E_GameState _nGameState;

};

