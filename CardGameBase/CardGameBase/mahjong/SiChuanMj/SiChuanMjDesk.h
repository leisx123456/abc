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
	virtual int mjNum() const { return 108; }




	//////////////////////////////////////////////////////////////////////////
	// 系统事件
	virtual void onEventGameBegin();		// 游戏开始
	virtual void onEventCutCards();			// 摇骰子切牌
	virtual void onEventDealCards();		// 发手牌
	virtual void onEventDingQue();			// 定缺
	virtual void onEventAppointActiveUser();			// 指定活动用户，活动用户可摸牌出牌。



	//////////////////////////////////////////////////////////////////////////
	// 广播用户接口
	virtual void onMsgBegin(/*用户数据*/) = 0;
	virtual void onMsgCutCards(int dice1, int dice2) = 0;
	virtual void onMsgDealCards(CLMjCard pArrCardHand[][14], int nPlayerCount) = 0;
	virtual void onMsgDingQue() = 0;


	//////////////////////////////////////////////////////////////////////////
	//用户操作接口
	void onUserEnter(int nChairID);
	void onUserReady(int nChairID);
	void onUserTBA(int nCardColor, int nChairID);

protected:
	CLGameDispatcher* _gameDispatcher;
	//E_GameState _nGameState;

};

