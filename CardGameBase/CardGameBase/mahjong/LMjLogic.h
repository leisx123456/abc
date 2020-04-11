#ifndef _L_MJ_LOGIC_H_
#define _L_MJ_LOGIC_H_

#include <iostream>
#include "../core/LMjCard.h"
#include "../core/LCardOperate.h"

//////////////////////////////////////////////////////////////////////////
// 麻将逻辑抽象类
class CLMjLogic : public CLCardOperate<CLMjCard>
{
public:
	CLMjLogic();
	//bool isExistPair(CLMjCard aCards[]);

	// //转换
	int switchToCardData(const unsigned int arrCardIndex[MJ_MAX_INDEX], CLMjCard aCards[MJ_MAX_HAND_COUNT]);
	int switchToCardIndex(CLMjCard aCards[], unsigned int unCardCount, unsigned int arrCardIndex[MJ_MAX_INDEX]);

	// 是否能碰
	bool isCanPong(CLMjCard aCards[], unsigned int unCardCount, const CLMjCard & cardOut);

	// 是否能杠
	bool isCanKong(CLMjCard aCards[], unsigned int unCardCount, T_WeaveCardsItem aWeaveItem[], unsigned int unItemSize, CLMjCard cardOut, T_MjActKongInfo & tMjActKongInfo);
	
	// 消除一组刻子(包含起始位置的牌)
	// bool removeTriplet(CLMjCard aCards[], unsigned int unCardCount, int nBeginPos = 0);
	// 不考虑癞子的情况
	bool removeTriplet(CLMjCard aCards[], unsigned int unCardCount, int nBeginPos = 0);

	// 消除一组顺子
	bool removeSequence(CLMjCard aCards[], unsigned int unCardCount, int nBeginPos = 0);

	// 手牌所含的癞子数
	int getMagicNum(CLMjCard aCards[], unsigned int unCardCount);
	// 手牌和组合牌所含的花色种类
	int getColorCount(CLMjCard aCards[], unsigned int unCardCount, T_WeaveCardsItem aWeaveItem[], unsigned int unItemSize);

	//是否花猪
	bool isFlowerPig(CLMjCard aCards[], unsigned int unCardCount, T_WeaveCardsItem aWeaveItem[], unsigned int unItemSize, int colorQue = -1);
	virtual bool isCanTing(CLMjCard aCards[]
		, unsigned int unCardCount
		, T_WeaveCardsItem aWeaveItem[]
		, unsigned int unItemSize
		, T_MjActHuInfo & tMjActHuInfo
		, const CLMjCard & cardOut = CARD_EMPTY
		, bool bMust258Pair = false);

	// 胡牌算法还有问题
	bool isCanHu_3x2(CLMjCard aCards[], unsigned int unCardCount, bool bMust258Pair = false);
	bool isCanHu_7Pair(CLMjCard aCards[], unsigned int unCardCount, bool bMust258Pair = false);

	// 基于可胡牌条件的胡牌种类
	bool isPongPong(CLMjCard aCards[], unsigned int unCardCount, T_WeaveCardsItem aWeaveItem[], unsigned int unItemSize);
	bool isQingYiSe(CLMjCard aCards[], unsigned int unCardCount, T_WeaveCardsItem aWeaveItem[], unsigned int unItemSize);
	bool isDragonSevenPair(CLMjCard aCards[], unsigned int unCardCount, bool bMust258Pair = false);
	int genNum(CLMjCard aCards[], unsigned int unCardCount, T_WeaveCardsItem aWeaveItem[], unsigned int unItemSize);

	/* 胡牌判断, 这里只判断最基本的3x2,没有考虑7对等， 具体怎样胡看各地方规则由子类实现*/
	virtual bool isCanHu(CLMjCard aCards[]
		, unsigned int unCardCount
		, T_WeaveCardsItem aWeaveItem[]
		, unsigned int unItemSize
		, T_MjActHuInfo & tMjActHuInfo
		, const CLMjCard & cardOut = CARD_EMPTY
		, bool bMust258Pair = false);
};



#endif // _L_MJ_LOGIC_H_
