#pragma once
#include "../../core/LMjCard.h"



class IAbstractThink
{
public:
	IAbstractThink();

	//////////////////////////////////////////////////////////////////////////
	//�ӿڶ���
	virtual ~IAbstractThink();

	// ˼����ȱ������ȱ�Ļ�ɫ
	virtual int thinkDingQue(CLMjCard aCards[], unsigned int unCardCount) = 0;

	virtual void thinkHu(CLMjCard aCards[], unsigned int unCardCount
		, T_WeaveCardsItem aWeaveItem[], unsigned int unItemSize, CLMjCard cardDest) = 0;

	virtual CLMjCard thinkOutCard(CLMjCard aCards[], unsigned int unCardCount
		, T_WeaveCardsItem aWeaveItem[], unsigned int unItemSize) = 0;
};
