#ifndef _L_MJ_LOGIC_H_
#define _L_MJ_LOGIC_H_

#include <iostream>
#include "../core/LMjCard.h"
#include "../core/LCardOperate.h"
	
class CLMjLogic : public CLCardOperate<CLMjCard>
{
public:
	CLMjLogic();

	//bool isExistPair(CLMjCard aCards[]);

	// //转换
	int switchToCardData(const unsigned int arrCardIndex[MJ_MAX_INDEX], CLMjCard aCards[MJ_MAX_HAND_COUNT]);
	int switchToCardIndex(CLMjCard aCards[], unsigned int unCardCount, unsigned int arrCardIndex[MJ_MAX_INDEX]);

	
	// 是否能碰
	bool isCanPong(CLMjCard aCards[], unsigned int unCardCount, const CLMjCard & cardDest);

	// 是否能杠
	bool isCanKong(CLMjCard aCards[], unsigned int unCardCount, T_WeaveCardsItem aWeaveItem[], unsigned int unItemSize, CLMjCard cardDest, T_MjActKongInfo & tMjActKongInfo);
	bool isCanDianKong(CLMjCard aCards[], unsigned int unCardCount, T_WeaveCardsItem aWeaveItem[], unsigned int unItemSize,
		CLMjCard cardDest);
	bool isCanAnKong(CLMjCard aCards[], unsigned int unCardCount, CLMjCard & cardResult);
	bool isCanBuKong(CLMjCard aCards[], unsigned int unCardCount, T_WeaveCardsItem aWeaveItem[], unsigned int unItemSize, CLMjCard & cardResult);

	// 消除一组刻子(包含起始位置的牌)
	bool removeTriplet(CLMjCard aCards[], unsigned int unCardCount, int nBeginPos = 0);

	// 消除一组顺子
	bool removeSequence(CLMjCard aCards[], unsigned int unCardCount, int nBeginPos = 0);

	// 手牌所含的癞子数
	int getMagicNum(CLMjCard aCards[], unsigned int unCardCount);
	// 手牌和组合牌所含的花色种类
	int getColorCount(CLMjCard aCards[], unsigned int unCardCount, T_WeaveCardsItem aWeaveItem[], unsigned int unItemSize);

	//是否花猪
	bool isFlowerPig(CLMjCard aCards[], unsigned int unCardCount, T_WeaveCardsItem aWeaveItem[], unsigned int unItemSize, int colorQue = -1);


	/* 胡牌判断 */
	bool isCanHu(CLMjCard aCards[]
		, unsigned int unCardCount
		, T_WeaveCardsItem aWeaveItem[]
		, unsigned int unItemSize
		, const CLMjCard & cardDest = CARD_EMPTY
		, bool bMust258Pair = false);

	bool isCanHu_3x2(CLMjCard aCards[], unsigned int unCardCount, bool bMust258Pair = false);
	bool isCanHu_7Pair(CLMjCard aCards[], unsigned int unCardCount, bool bMust258Pair = false);

	// 基于可胡牌条件的胡牌种类
	bool isPengPeng(CLMjCard aCards[], unsigned int unCardCount, T_WeaveCardsItem aWeaveItem[], unsigned int unItemSize);
	bool isQingYiSe(CLMjCard aCards[], unsigned int unCardCount, T_WeaveCardsItem aWeaveItem[], unsigned int unItemSize);


};



#endif // _L_MJ_LOGIC_H_
