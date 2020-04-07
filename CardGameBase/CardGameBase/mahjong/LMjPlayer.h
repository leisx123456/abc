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




// 手牌基本数据
struct T_UserCardsData
{
	CLMjCard arrHandCards[MJ_MAX_HAND_COUNT];
	int nHandNums;

	T_WeaveCardsItem arrWeaveCardsItem[4];
	int nWeaveItemNums;

	int nColorQue;
	CLMjCard cardNew;
};

class IAbstractThink;

//////////////////////////////////////////////////////////////////////////
// 麻将玩家抽象类
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


	CLMjPlayer();
	CLMjPlayer(E_PlayerType ePlayerType, int nChairID);
	virtual ~CLMjPlayer();

	void init();
	//////////////////////////////////////////////////////////////////////////
	// 玩家属性
	bool isReboot() {return m_ePlayerType != EP_People; }
	virtual void initLogic() = 0;

	//////////////////////////////////////////////////////////////////////////
	// 玩家状态
	void ready() { m_bReady = true; }
	void cancelReady(){ m_bReady = false; }
	bool isReady() { return m_bReady; }
	
	bool isActive() { return m_ePlayerActiveState == P_Active; }
	void setActive(E_PlayerActiveState ePlayerActiveState){ m_ePlayerActiveState = ePlayerActiveState; }
	void cancelActive(){ m_ePlayerActiveState = p_unActive; }
	bool isAlreadyHu() { return m_tUserHuInfo.bHu; }


	//////////////////////////////////////////////////////////////////////////
	// 玩家动作及操作

	// 摸手牌
	void dealCards(const CLMjCard arrHandCards[MJ_MAX_HAND_COUNT]);
	void getHandCards(CLMjCard* pArrHandCards, int & nHandNums);
	void getHandCards(int pArrHandCards[], int & nHandNums);

	// 定缺
	void selectTBA(CLMjCard::E_MjCardColor eColorTBA) { m_eColorTBA = eColorTBA; }
	bool isAlreadyTBA(){ return m_eColorTBA > -1 && m_eColorTBA < 3; }
	int getDingQueColor() { return m_eColorTBA; }


	//摸牌
	void drawCard(const CLMjCard & card);

	// 理牌
	void sordCards();

	//玩家出牌
	bool outCard(const CLMjCard & card);
	CLMjCard getLatestOutCard();
	void getOutCardsValue(int* pArrOutCardsValue, int & nOutCardNums);
	void removeLatestOutCard();
	int outCardNums() { return m_vecCardOut.size(); }

	// 玩家动作

	bool execPong(unsigned char byProvideUser, const CLMjCard & cardOut);
	bool execKong(unsigned char byProvideUser, const CLMjCard & cardOut/*, int nCurSelectIndex*/);
	bool execHu(unsigned char byProvideUser, int nHuIndex, const CLMjCard & cardOut);
	void getWeaveCardsItems(T_WeaveCardsItem* pWeaveCardsItem, int & nWeaveItemNums);
	T_WeaveCardsItem & getLatestWeaveCardsItem();
	int weaveItemNums() { return m_nWeaveItemNums; }


	//设置胡
	void setHu(int nCard)
	{
		m_vecHu.push_back(nCard);
	}
	T_UserHuInfo & userHuInfo()
	{
		return m_tUserHuInfo;
	}
	int huIndex();

	//获得出了的牌队列
	//vector<int> getOutCard();
//////////////////////////////////////////////////////////////////////////
// 逻辑判断

	// 获取玩家对于自己手牌(自己是活动状态) 或别人出的牌(别人是活动状态) 可以选择的动作
	bool selectActInfo(CLMjCard cardOut, unsigned short usIgnoreFlags = 0);
	T_MjActInfo* actInfo() { return &m_tMjActInfo; }

	bool isCanPong(CLMjCard cardOut);
	bool isCanKong(CLMjCard cardOut, T_MjActInfo* pActInfo);
	bool isCanHu(CLMjCard cardOut);


	//////////////////////////////////////////////////////////////////////////
	// ai思考
	CLMjCard::E_MjCardColor thinkDingQue();
	void think(CLMjCard cardOut, unsigned short usIgnoreFlags = 0);

protected:
	//////////////////////////////////////////////////////////////////////////


	T_WeaveCardsItem m_arrWeaveCardsItem[4];	// 用户的组合牌
	int m_nWeaveItemNums;

	CLMjCard m_arrHandCards[MJ_MAX_HAND_COUNT];	//用户的手牌
	int m_nHandNums;					//用户手牌数量

	// --取消，因为玩家手牌设置最大14，新牌已加入到手牌中
	//CLMjCard m_CardNew;	// 刚抓起来的牌

	vector<CLMjCard> m_vecCardOut;	//已出牌表(不包含别人拿过去吃碰杠胡的)
	int m_nOutedTimes;					//已出的牌数(不包含别人拿过去吃碰杠胡的)

	vector<CLMjCard> m_vecTing;	//听牌队列
	vector<CLMjCard> m_vecHu;	//胡牌队列，扩展到血流成河
	vector<CLMjCard> m_vecCardsLouHu;	// 漏胡的牌
	

	int m_nDrawTimes;	//抓牌次数
	
	E_PlayerActiveState m_ePlayerActiveState; // 玩家活动状态
	E_PlayerStatus m_ePlayerStatus;

	CLMjCard::E_MjCardColor m_eColorTBA;	//定缺所用

	CLMjLogic* m_pMjLogic;

	// 玩家性质 ai or people
	E_PlayerType m_ePlayerType;

	// ai
	IAbstractThink* m_pIAbstractThink;

	// 保存玩家当前可以执行的那些动作(电脑玩家通用)
	T_MjActInfo m_tMjActInfo;

	//  保存玩家本局胡牌信息
	T_UserHuInfo m_tUserHuInfo;
	
	//////////////////////////////////////////////////////////////////////////
	bool m_bReady;


	//////////////////////////////////////////////////////////////////////////
	// 玩家属性
	int m_nChairID;

	//bool bOffline;
	//bool bHouseOwner;

	bool bBanker;
	//bool bQingUue;
	//bool bBaoJiao;
	bool bHu;
	//bool bBuyCard;
	CLMjCard::E_MjCardColor eTBACardColor;
	E_OperatorStatus eOperatorStatus;

};

#endif //_L_MJ_PLAYER_H_
