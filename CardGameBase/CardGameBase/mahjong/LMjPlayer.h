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

enum E_OperatorStatus
{
	EO_Choosing,         //选牌中
	EO_Choosed,				//已选定
	EO_TBA_Choosing,		//定缺中
	EO_TBA_Choosed,			//已定缺
	EO_NULL					//未操作
};

struct T_UserInfo
{
	string strNickname;
	unsigned short cbScore;
	bool bOffline;
	string strHeadPath;
	bool bBanker;
	bool bHouseOwner;
	//		img_dingque
	bool bReady;
	bool bBaoJiao;
	bool bHu;
	bool bBuyCard;
	CLMjCard::E_MjCardColor eTBACardColor;
	E_OperatorStatus eOperatorStatus;

	T_UserInfo()
		: strNickname("12345")
		, cbScore(0)
		, bOffline(false)
		, strHeadPath("")
		, bBanker(false)
		, bHouseOwner(false)
		, bReady(false)
		, bBaoJiao(false)
		, bHu(false)
		, bBuyCard(false)
		, eTBACardColor(CLMjCard::EM_Invalid_Color)
		, eOperatorStatus(EO_NULL)
	{

	}
};

// 手牌基本数据
struct T_UserCardsData
{
	CLMjCard arrHandCards[MAX_HAND_COUNT];
	int nHandNums;

	T_WeaveCardsItem arrWeaveCardsItem[4];
	int nWeaveItemNums;

	int nColorQue;
	CLMjCard cardNew;
};

class IAbstractThink;

class CLMjPlayer
{
public:
	enum E_PlayerType
	{
		EP_People,
		EP_CmpEasy,
		EP_CmpNormal,
		EP_CmpClever
	};

	//玩家状态
	enum E_PlayerStatus
	{
		EPS_Waiting,
		EPS_ThinkOutCard,
		EPS_Choosing,
		EPS_Hu,
	};

	//玩家活动状态
	enum E_PlayerActiveState
	{
		P_Active,
		p_unActive
	};



	enum E_Sex
	{
		S_Boy,
		S_Girl
	};

	CLMjPlayer();
	CLMjPlayer(E_PlayerType ePlayerType);
	~CLMjPlayer();

	void init();
	//////////////////////////////////////////////////////////////////////////
	// 玩家属性
	bool isReboot() {return m_ePlayerType != EP_People; }

	void ready() { m_bReady = true; }
	void cancelReady(){ m_bReady = false; }
	bool isReady() { return m_bReady; }
	

	// 摸手牌
	void dealCards(const CLMjCard arrHandCards[MAX_HAND_COUNT]);
	void getHandCards(CLMjCard* pArrHandCards);

	// 定缺
	void selectTBA(CLMjCard::E_MjCardColor eColorTBA);
	bool isAlreadyTBA(){ return m_eColorTBA > -1 && m_eColorTBA < 3; }


	//摸牌
	void drawCard(const CLMjCard & card);

	//玩家出牌
	int outCard(int nPlace);

	//设置别人出的牌
	void setOtherOutCard(int nCard);

	//在已出牌队列中删除一张最近打出的牌
	void removeListOutCardAtLast();

	bool isActive() { return m_ePlayerActiveState == P_Active; }
	void setActive(E_PlayerActiveState ePlayerActiveState){ m_ePlayerActiveState = ePlayerActiveState; }
	void cancelActive(){ m_ePlayerActiveState = p_unActive; }


	//设置胡
	void setHu(int nCard)
	{
		m_vecHu.push_back(nCard);
	}

	//获得出了的牌队列
	//vector<int> getOutCard();
//////////////////////////////////////////////////////////////////////////
// 逻辑判断
	bool isCanPong();
	bool isCanKong();
	bool isCanHu();


	//////////////////////////////////////////////////////////////////////////
	// ai思考
	CLMjCard::E_MjCardColor thinkDingQue();
	void think(T_MjActInfo* pActInfo, CLMjCard cardDest);


private:
	E_PlayerType m_ePlayerType;
	IAbstractThink* m_pIAbstractThink;

	CLMjLogic m_mjLogic;
	T_WeaveCardsItem m_arrWeaveCardsItem[4];	// 用户的组合牌
	int m_nWeaveItemNums;

	CLMjCard m_arrHandCards[MAX_HAND_COUNT];	//用户的手牌
	int m_nHandNums;					//用户手牌数量

	// --取消，因为玩家手牌设置最大14，新牌已加入到手牌中
	//CLMjCard m_CardNew;	// 刚抓起来的牌

	CLMjCard m_arrOutedCards[MJ_MAX_OUTED];	//已出牌表(不包含别人拿过去吃碰杠胡的)
	int m_nOutedTimes;					//玩家已出牌的次数(包含别人拿过去吃碰杠胡的)
	//int m_nOutedNums;					//已出的牌数(不包含别人拿过去吃碰杠胡的)

	vector<CLMjCard> m_vecTing;	//听牌队列
	vector<CLMjCard> m_vecHu;	//胡牌队列，扩展到血流成河
	vector<CLMjCard> m_lstCardOut;	//出牌列
	vector<CLMjCard> m_vecCardsLouHu;	// 漏胡的牌
	

	int m_nDrawTimes;	//抓牌次数
	
	E_PlayerActiveState m_ePlayerActiveState; // 玩家活动状态
	E_PlayerStatus m_ePlayerStatus;

	CLMjCard::E_MjCardColor m_eColorTBA;	//定缺所用

	
	int	m_nOtherOutCard;	//别人出的牌

	bool m_bIsHu;						//是否已经胡了
	
	
	//////////////////////////////////////////////////////////////////////////
	bool m_bReady;


	// 当player设置为机器人时使用AI出牌
	//CLMjAI m_mjAI;
};

#endif //_L_MJ_PLAYER_H_
