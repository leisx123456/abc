#pragma once
#include "../core/LClockActive.h"
#include "LMjPlayer.h"
#include "../core/LDicePair.h"
#include "LMjMachine.h"
#include "LMjLogic.h"
/************************************************************************/
/* 
是一个管理类，即一桌麻将几个玩家为单位的游戏
*/
/************************************************************************/
// 麻将问题:
//由于各类麻将总数不同，如四川麻将只要万条筒，还没有具体的规律总结，所以先不用此类
// 麻将子使用数组还是std::array, std::vector? 先使用数组吧，一是有最大固定值，二是高效, 后期可以用std::array(c++11)
class CLMjDesk
{
public:
	CLMjDesk();
	virtual ~CLMjDesk();

	// 常用函数
	//////////////////////////////////////////////////////////////////////////
	// 获取下个玩家的索引 nPlayerCount为实际参与的玩家数
	static int nextPlayerIndex(int nCurIndex, int nPlayerCount = GAME_MJ_PLAYER_MAX);
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

	
	virtual int playerCount() const { return GAME_MJ_PLAYER_MAX; }	// 参与游戏的实际人数， 表示2人玩  3 表示3人玩  4表示4人



	// shuffle洗牌,cut切牌,deal发牌,sort理牌,draw摸牌,play打出,discard弃牌shuffle：本意是指调换顺序,这里调换牌的顺序,也就是“洗牌”.
	//cut：切牌.作弊的时候常用一招“假切牌”,看起来好像切了,实际上没有改过牌的...
	virtual void allocation() = 0; // 分配麻将子，这里的数量是具体麻将游戏实际加入的牌,如四川麻将只需万条筒
	virtual int mjNumAllocation() const { return GAME_MJ_CARD_COUNT_MAX; }	// 分配麻将子数，这里的数量是具体麻将游戏实际加入的牌数，如有的只加入万条筒就是108

	void ruffle();	//洗牌
	void initWall(); //按四个方向摆放牌墙
	void rockDice(); // 摇骰子，实质为cut切牌
	void dealCards(CLMjCard pArrCardHand[][14]);	// deal发牌, 发手牌
	void sortCard();	//sort理牌

	// shuffle洗牌,cut切牌,deal发牌,sort理牌,draw摸牌,play打出,discard弃牌

	// 摸牌，获取当前麻将
	CLMjCard drawCard();



	//设置起始位置（庄家和色子数）
	void beginGetCardPoint();

	//获得剩余麻将数量
	int getSurplusCardsOnWall();



	////add
	//void initHandCard();
	//void SortCardData(byte wviewid);
	//void outCard(byte cbViewId, byte cbCard);
	////void actionOperator(byte cbViewId);

	// 具体怎么管理， 管理哪些对象让子类去实现
protected:
	CLMjLogic m_mjLogic;

	CLMjCard m_arrMjCardsPair[GAME_MJ_CARD_COUNT_MAX];	// 一副麻将
	CLMjPlayer* m_pArrMjPlayer[GAME_MJ_PLAYER_MAX];	// 麻将玩家 最多4人

	CLDicePair m_mjDicePair;	// 2个麻将骰子

	CLMjCard m_arrMjCardWall[4][34];	// 牌墙，从m_arrMjCard的下标0开始派牌，庄家最先派牌
	CLMjCard m_cardOut;



	int m_nMjNumSurplus;								//剩余牌墙麻将数量
	int m_nIndexStart;	// 根据所摇的骰子确定起牌的地方index，index对应m_arrMjCard的下标，小于m_nMjNum
	int m_nIndexCurrent;	// 当前抓牌的位置index，index对应m_arrMjCard的下标，小于m_nMjNum

	int m_nBanker;	//庄家 0-3
	int m_nActiveUser;	//当前活动用户 0-3


	T_ActiveUser m_tActiveUser;
	
	//int m_nWhere;								//麻将取到那个位置
	//int m_nWho;									//该取哪个方向的麻将
	//int m_nTailWho;								//最开始拿的哪家的牌,用于记录

	//CLMjCard m_nHandCard[4][MAX_HAND_COUNT];
	//int m_nHandCardCount[4];
	//T_WeaveItem m_nWeaveItemArray[4][MAX_WEAVE];		//组合扑克
	//byte m_nWeaveCount[4];					//组合数目
	//std::vector<CLMjCard>  m_vecOutCard[4];
};

