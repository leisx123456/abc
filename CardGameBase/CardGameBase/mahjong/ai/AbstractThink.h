#pragma once
#include "../../core/LMjCard.h"



class IAbstractThink
{
public:
	IAbstractThink();

	//////////////////////////////////////////////////////////////////////////
	//接口定义
	virtual ~IAbstractThink();

	// 思考定缺，返回缺的花色
	virtual int thinkDingQue(CLMjCard aCards[], unsigned int unCardCount) = 0;

	virtual void thinkHu(CLMjCard aCards[], unsigned int unCardCount
		, T_WeaveCardsItem aWeaveItem[], unsigned int unItemSize, CLMjCard cardDest) = 0;

	virtual CLMjCard thinkOutCard(CLMjCard aCards[], unsigned int unCardCount
		, T_WeaveCardsItem aWeaveItem[], unsigned int unItemSize) = 0;
};
