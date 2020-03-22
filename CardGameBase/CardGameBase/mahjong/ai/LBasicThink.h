#pragma once
#include "AbstractThink.h"



class CLBasicThink : public IAbstractThink
{
public:
	CLBasicThink();
	virtual ~CLBasicThink();


	virtual int thinkDingQue(CLMjCard aCards[], unsigned int unCardCount, T_WeaveCardsItem aWeaveItem[], unsigned int unItemSize) = 0;

	virtual void thinkHu(CLMjCard aCards[], unsigned int unCardCount
		, T_WeaveCardsItem aWeaveItem[], unsigned int unItemSize, CLMjCard cardDest) = 0;

	virtual CLMjCard thinkOutCard(CLMjCard aCards[], unsigned int unCardCount
		, T_WeaveCardsItem aWeaveItem[], unsigned int unItemSize) = 0;

	bool isExistTriplet(CLMjCard aCards, int &pos1, int &pos2);


protected:

	//手牌
	CLMjCard m_arrHandCard[13];
	int m_nHandNums;

	T_WeaveCardsItem m_arrWeaveCardsItem[4];
	int m_nWeaveCardsItemNum;

	CLMjCard m_cardDest;

	bool m_bHavePair;

	//当前手牌分数
	int m_nScore;

	//
	int m_nMaxScore;

	//
	bool m_bIsHu;
};

