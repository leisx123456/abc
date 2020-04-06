#pragma once

#include "core/LClockActive.h"
#include "core/LDicePair.h"

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

	enum E_ActNotifyType
	{
		EA_BankerFirstGot,
		EA_DarwCard,
		EA_OutCard,
		EA_OtherAct
	};

	//////////////////////////////////////////////////////////////////////////
	// ��Ϸ�ڲ��߼�
	// �����齫�ӣ��Ĵ��齫ֻ������Ͳ
	virtual void allocation();
	virtual int mjNumAllocation() const { return 108; }
	int surplusCards() { return m_nIndexCurrent >= m_nIndexStart 
		? (m_nIndexCurrent - m_nIndexStart) : (m_nIndexCurrent + mjNumAllocation() - m_nIndexStart); }
	void updateUser();
	void clearAllUserActInfo();
	int selectActAtOnceUsers(int byUsers[], unsigned short & usActFlag);
	bool execActPass(int nFromUser);
	bool execActPong(int nFromUser, int nToUser);
	bool execActKong(int nFromUser, int nToUser);
	bool execActHu(int nFromUser, int arrToUser[], int nUserNums);

	//////////////////////////////////////////////////////////////////////////
	// ϵͳ�¼�
	virtual void onEventReady();
	virtual void onEventGameBegin();		// ��Ϸ��ʼ
	virtual void onEventCutCards();			// ҡ��������
	virtual void onEventDealCards();		// ������
	virtual void onEventDingQue();			// ��ȱ
	virtual void onEventFristBankerActive();
	virtual void onEventDrawCard();	// ����
	//virtual void onEventAppointActiveUser();			// ָ����û�
	//virtual void onEventAppointResponseUsers();			// ָ����Ӧ�û�
	virtual void onEventGameFinshed();
	// ���ڵ���˼��ִ�ж����ȽϿ죬��Ҫ�ӳ�ִ��
	void onDelayExecActRequest(int nChairID);
	void onDelayExecOutCardRequest(int nChairID);
	//////////////////////////////////////////////////////////////////////////
	// ϵͳ�ж�
	void onSysAppointActiveUser(int nChairID);
	void onSysJudgeAndExecActNotify(E_ActNotifyType eActiveNotifyType);


	//////////////////////////////////////////////////////////////////////////
	// �㲥�û��ӿ�
	virtual void onMsgReady(int nChairID) = 0;
	virtual void onMsgBegin(/*�û�����*/) = 0;
	virtual void onMsgCutCards(int dice1, int dice2) = 0;
	virtual void onMsgDealCards(T_MsgDealCards tMsgDealCards) = 0;
	virtual void onMsgDingQueBegin() = 0;
	virtual void onMsgDingQue(T_MsgDingQue tMsgDingQue) = 0;
	virtual void onMsgAppointActiveUser(T_MsgAppointActiveUser tMsgAppointActiveUser) = 0;
	virtual void onMsgDrawCard(T_MsgDrawCard tMsgDrawCard) = 0;
	virtual void onMsgActNotify(T_MjActInfo tMjActInfo) = 0;
	virtual void onMsgOutCard(T_MsgOutCard tMsgOutCard) = 0;
	virtual void onMsgActResult(T_MsgActResultInfo tMsgActResultInfo) = 0;
	virtual void onMsgGameResult(T_MsgResult tMsgResult) = 0;

	//////////////////////////////////////////////////////////////////////////
	//�û������ӿ�
	//void onUserEnter(int nChairID);
	//void onUiSitDown();
	void onUserReady(int nChairID);
	void onUserTBA(int nChairID, int nCardColor);
	void onUserOutCardRequest(int nChairID, T_MjActOutInfo tMjActOutInfo);
	void OnUserActRequest(int nChairID, T_ActRequest tActRequest);

protected:
	CLGameDispatcher* _gameDispatcher;
	//E_GameState _nGameState;

};

