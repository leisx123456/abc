#ifndef _L_MJ_LOGIC_H_
#define _L_MJ_LOGIC_H_

#include <iostream>
#include "../core/LMjCard.h"
#include "../core/LCardOperate.h"
	







class CLMjLogic : public CLCardOperate<CLMjCard>
{
public:
	CLMjLogic();
	virtual bool isValidCard(const CLMjCard & card);

	//virtual void initMjCards() = 0;	// 每种麻将的玩法和麻将数目不一样，需要自己实现

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

private:
	unsigned char* m_pByMjCards;	// 麻将牌
	int m_nTotalCards;				// 麻将牌总数
	unsigned int m_nMaxHandCards;	// 最大手牌数
	int m_nMagicNum;				// 癞子数
};



#endif // _L_MJ_LOGIC_H_
