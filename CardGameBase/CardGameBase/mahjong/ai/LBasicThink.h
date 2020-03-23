#pragma once
#include "AbstractThink.h"
#include "../LMjLogic.h"


class CLBasicThink : public IAbstractThink
{
public:
	CLBasicThink();
	virtual ~CLBasicThink();


	bool isExistTriplet(CLMjCard card, int &pos1, int &pos2);
	int findPosInActiveHandCards(CLMjCard handCard);
	bool isExistSequence(CLMjCard handCard, int &pos1, int &pos2);
	bool isExistPair(CLMjCard handCard, int & pos1);
	bool isExistDoor(CLMjCard handCard, E_HandCardRelationType & doorType, int &pos1);


protected:
	CLMjLogic m_mjLogic;

	//手牌
	CLMjCard m_arrHandCard[14];
	int m_nHandNums;

	T_WeaveCardsItem m_arrWeaveCardsItem[4];
	int m_nWeaveCardsItemNum;

	CLMjCard m_cardOut;
	CLMjCard m_cardNew;
	CLMjCard m_CardBadly;

	// 临时保存手牌状态
	CLMjCard m_arrHandCardTemp[14];

	bool m_bHavePair;

	//当前手牌分数
	int m_nScore;

	//
	int m_nMaxScore;

	//
	bool m_bIsHu;

	
};

