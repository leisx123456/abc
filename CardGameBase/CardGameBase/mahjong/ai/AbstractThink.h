#pragma once
#include "../../core/LMjCard.h"



class IAbstractThink
{
public:
	IAbstractThink();


	virtual ~IAbstractThink();
	

	//接口定义
	//////////////////////////////////////////////////////////////////////////

	// 思考定缺，返回缺的花色
	virtual int thinkDingQue(CLMjCard aCards[], unsigned int unCardCount) = 0;

	// 思考打牌整个过程
	virtual void think(T_MjActInfo* pMjActInfo, CLMjCard aCards[], unsigned int unCardCount, T_WeaveCardsItem aWeaveItem[]
		, unsigned int unItemSize, CLMjCard cardOut, int nQueColor = -1, unsigned short usIgnoreFlags = 0) = 0;


};
