#ifndef _L_MJ_PLAYER_H_
#define _L_MJ_PLAYER_H_

/************************************************************************/
/*
对手牌的管理及相关的记录
*/
/************************************************************************/
#include <vector>
#include <algorithm>
#include "core/LMjDef.h"
#include "core/LMjCard.h"
#include "LMjLogic.h"

using namespace std;

class CLMjPlayer
{
public:
	CLMjPlayer();
	~CLMjPlayer();

	//玩家状态
	enum E_PlayerStatus
	{
		EPS_Waiting,
		EPS_ThinkOutCard,
		EPS_Choosing,
		EPS_Hu,
	};

	enum E_PlayerActiveState
	{
		P_Active,
		p_unActive
	};

	void dealCards(const CLMjCard arrHandCards[MAX_HAND_COUNT]);
	void getHandCards(CLMjCard* pArrHandCards);

	//摸牌牌
	void drawCard(const CLMjCard & card);

	//设置别人出的牌
	void setOtherOutCard(int nCard);

	//在已出牌队列中删除一张最近打出的牌
	void removeListOutCardAtLast();

	//玩家出牌
	int outCard(int nPlace);



	//设置胡
	void setHu(int nCard)
	{
		m_vecHu.push_back(nCard);
	}

	//获得出了的牌队列
	//vector<int> getOutCard();

	// 返回chair逆时针转1的玩家
	int antiClockWise(int chair)
	{
		return (chair + PLAYER_NUM - 1) % PLAYER_NUM;
	}

	// 返回chair顺时针转1的玩家
	int clockwise(int chair)
	{
		return (chair + 1) % PLAYER_NUM;
	}



private:
	CLMjLogic m_mjLogic;
	T_WeaveCardsItem m_arrWeaveCardsItem[4];	// 用户的组合牌

	CLMjCard m_arrHandCards[MAX_HAND_COUNT];	//用户的手牌
	int m_nHandNums;					//用户手牌数量

	CLMjCard m_CardNew;	// 刚抓起来的牌

	CLMjCard m_arrOutedCards[MJ_MAX_OUTED];	//已出牌表(不包含别人拿过去吃碰杠胡的)
	int m_nOutedTimes;					//玩家已出牌的次数(包含别人拿过去吃碰杠胡的)
	//int m_nOutedNums;					//已出的牌数(不包含别人拿过去吃碰杠胡的)

	vector<CLMjCard> m_vecTing;	//听牌队列
	vector<CLMjCard> m_vecHu;	//胡牌队列，扩展到血流成河
	vector<CLMjCard> m_lstCardOut;	//出牌列
	vector<CLMjCard> m_vecCardsLouHu;	// 漏胡的牌
	

	int m_nDrawTimes;	//抓牌次数
	
	E_PlayerActiveState m_ePlayerActiveState;
	E_PlayerStatus m_ePlayerStatus;

	CLMjCard::E_MjCardColor m_eColorTBA;	//定缺所用

	
	int	m_nOtherOutCard;	//别人出的牌

	bool m_bIsHu;						//是否已经胡了
	
	



	// 当player设置为机器人时使用AI出牌
	//CLMjAI m_mjAI;
};

#endif //_L_MJ_PLAYER_H_
