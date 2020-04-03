#ifndef  _SI_CHUAN_MJ_LOGIC_H_
#define  _SI_CHUAN_MJ_LOGIC_H_

#include "mahjong/LMjLogic.h"

// 具体游戏逻辑类
class CSiChuanMjLogic : public CLMjLogic
{
public:
	virtual bool isCanHu(CLMjCard aCards[]
		, unsigned int unCardCount
		, T_WeaveCardsItem aWeaveItem[]
		, unsigned int unItemSize
		, T_MjActHuInfo & tMjActHuInfo
		, const CLMjCard & cardOut = CARD_EMPTY
		, bool bMust258Pair = false);
};


#endif // _SI_CHUAN_MJ_LOGIC_H_

