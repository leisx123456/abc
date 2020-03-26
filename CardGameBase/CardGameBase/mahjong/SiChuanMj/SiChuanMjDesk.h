#pragma once

#include "core/LClockActive.h"
#include "core/LDicePair.h"

#include "mahjong/LMjPlayer.h"
#include "mahjong/LMjLogic.h"
#include "mahjong/LMjDesk.h"

/************************************************************************/
/* 
	具体的麻将游戏桌面类
*/
/************************************************************************/

//enum E_GameState
//{
//	EGS_Ready,
//	EGS_Begin,
//	EGS_ReadyRolateDice,
//	EGS_RolatingDice,         //ing
//	EGS_TBA,                 //to be assured,定缺
//	EGS_Playing,
//	EGS_Over
//};

class CLGameDispatcher;

// 最普通的四人四川麻将
class CSiChuanMjDesk : public CLMjDesk
{
public:
	CSiChuanMjDesk();
	virtual ~CSiChuanMjDesk();

	//////////////////////////////////////////////////////////////////////////
	// 游戏内部逻辑
	// 分配麻将子，四川麻将只需万条筒
	virtual void allocation();
	virtual int mjNumAllocation() const { return 108; }
	int surplusCards() { return m_nIndexCurrent > m_nIndexStart 
		? (m_nIndexCurrent - m_nIndexStart) : (m_nIndexCurrent + mjNumAllocation() - m_nIndexStart); }

	bool selectActInfo(int nResponseUser, int nActiveUser, T_MjActInfo* pActInfo , CLMjCard cardDest, unsigned short usIgnoreFlags = 0);


	//////////////////////////////////////////////////////////////////////////
	// 系统事件
	virtual void onEventReady();
	virtual void onEventGameBegin();		// 游戏开始
	virtual void onEventCutCards();			// 摇骰子切牌
	virtual void onEventDealCards();		// 发手牌
	virtual void onEventDingQue();			// 定缺

	virtual void onEventFristGotActiveUser();
	virtual void onEventAppointDrawCardUser();
	virtual void onEventAppointActiveUser();			// 指定活动用户，活动用户可摸牌出牌。
	//virtual void onEventResponseToActiveUser();			// 当前可对活动玩家的动作响应



	//////////////////////////////////////////////////////////////////////////
	// 广播用户接口
	virtual void onMsgReady(int nChairID) = 0;
	virtual void onMsgBegin(/*用户数据*/) = 0;
	virtual void onMsgCutCards(int dice1, int dice2) = 0;
	virtual void onMsgDealCards(T_MsgDealCards tMsgDealCards) = 0;
	virtual void onMsgDingQueBegin() = 0;
	virtual void onMsgDingQue() = 0;
	virtual void onMsgAppointActiveUser(T_MsgAppointActiveUser tMsgAppointActiveUser) = 0;


	//////////////////////////////////////////////////////////////////////////
	//用户操作接口
	//void onUserEnter(int nChairID);
	//void onUiSitDown();
	void onUserReady(int nChairID);
	void onUserTBA(int nCardColor, int nChairID);

protected:
	CLGameDispatcher* _gameDispatcher;
	//E_GameState _nGameState;

};

