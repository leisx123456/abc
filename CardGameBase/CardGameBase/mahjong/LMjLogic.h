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

	// 是否能碰
	bool isCanPong(CLMjCard aCards[], unsigned int unCardCount, const CLMjCard & cardDest);

	// 是否能杠
	bool isCanKong(CLMjCard aCards[], unsigned int unCardCount, T_WeaveCardsItem aWeaveItem[], unsigned int unItemSize,
		CLMjCard cardDest);

	// 消除一组刻子(包含起始位置的牌)
	bool removeTriplet(CLMjCard aCards[], unsigned int unCardCount, int nBeginPos = 0);

	// 消除一组顺子
	bool removeSequence(CLMjCard aCards[], unsigned int unCardCount, int nBeginPos = 0);

	// 手牌所含的癞子数
	int getMagicNum(CLMjCard aCards[], unsigned int unCardCount);


	/* 胡牌判断 */
	bool isCanHu(CLMjCard aCards[]
		, unsigned int unCardCount
		, T_WeaveCardsItem aWeaveItem[]
		, unsigned int unItemSize
		, const CLMjCard & cardOtherOut = CARD_EMPTY
		, const CLMjCard & cardGet = CARD_EMPTY
		, bool bMust258Pair = false);

	bool isCanHu_3x2(CLMjCard aCards[], unsigned int unCardCount, bool bMust258Pair = false);
	bool isCanHu_7Pair(CLMjCard aCards[], unsigned int unCardCount, bool bMust258Pair = false);


};



#endif // _L_MJ_LOGIC_H_
