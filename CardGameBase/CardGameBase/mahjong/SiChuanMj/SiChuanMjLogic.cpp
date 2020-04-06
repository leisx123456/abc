#include "SiChuanMjLogic.h"
// 
bool CSiChuanMjLogic::isCanHu(CLMjCard aCards[]
	, unsigned int unCardCount
	, T_WeaveCardsItem aWeaveItem[]
	, unsigned int unItemSize
	, T_MjActHuInfo & tMjActHuInfo
	, const CLMjCard & cardOut/* = CARD_EMPTY*/
	, bool bMust258Pair /*= false*/)
{
	unsigned short unHuNameFlags = 0;
	// 复制牌组
	unsigned int unCardNumTemp = unCardCount;
	CLMjCard aCardsTemp[MJ_MAX_HAND_COUNT];
	copyCards(aCardsTemp, unCardNumTemp, aCards, unCardCount);

	// 加入出的牌
	if (cardOut.isValid())
	{
		aCardsTemp[unCardNumTemp++] = cardOut;
	}

	// 理牌
	sortCards(aCardsTemp, unCardNumTemp);
	//EHN_Ping = 0x0001,			//平胡
	//	EHN_Pong = 0x0002,	             //碰碰胡,大对子
	//	EHN_Qing = 0x0004,	    // 清一色
	//	//EHN_All_19 = 0x0008,	              //幺九
	//	EHN_SevenPair = 0x0010,     	      //七对
	//	EHN_JinGouDiao = 0x0020,			// 金钩钓
	//	EHN_DragonSevenPair = 0x0040,	     //龙七对
	bool bCanHu3x2 = isCanHu_3x2(aCardsTemp, unCardNumTemp, bMust258Pair);
	bool bCanHu7Pair = isCanHu_7Pair(aCardsTemp, unCardNumTemp);
	if (!bCanHu3x2 && !bCanHu7Pair)
	{
		return false;
	}
	bool bPongPongHu = isPongPong(aCardsTemp, unCardNumTemp, aWeaveItem, unItemSize);
	bool bJinGouDiao = false;
	if (bPongPongHu && unCardNumTemp == 2)
	{
		bJinGouDiao = true;
	}
	bool bDragonSevenPair = isDragonSevenPair(aCardsTemp, unCardNumTemp);

	// 组合牌型判断
	unHuNameFlags = 0;
	int gen = genNum(aCardsTemp, unCardNumTemp, aWeaveItem, unItemSize);
	bool bQingYiSe = isQingYiSe(aCardsTemp, unCardNumTemp, aWeaveItem, unItemSize);
	if (bQingYiSe)
	{
		unHuNameFlags = EHN_Qing;
	}

	if (bDragonSevenPair)
	{
		unHuNameFlags |= EHN_DragonSevenPair;
		gen--;
	}
	else if (bCanHu7Pair)
	{
		unHuNameFlags |= EHN_SevenPair;
	}
	else if (bJinGouDiao)
	{
		unHuNameFlags |= EHN_JinGouDiao;
	}
	else if (bPongPongHu)
	{
		unHuNameFlags |= EHN_Pong;
	}

	if (0 == unHuNameFlags)
	{
		unHuNameFlags = EHN_Ping;
	}

	tMjActHuInfo.eMjHuName = E_MjHuNameFlags(unHuNameFlags);
	tMjActHuInfo.gen = gen;
	return true;
}
