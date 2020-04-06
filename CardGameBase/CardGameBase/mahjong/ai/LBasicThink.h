#pragma once
#include "AbstractThink.h"
#include "../SiChuanMj/SiChuanMjLogic.h"


class CLBasicThink : public IAbstractThink
{
public:
	CLBasicThink();
	virtual ~CLBasicThink();

	void copyCards(CLMjCard aCards[], unsigned int unCardCount, T_WeaveCardsItem aWeaveItem[], unsigned int unItemSize, CLMjCard cardOut, int nQueColor = -1);
	int findPosInHandCards(const CLMjCard & card);

	// 下面函数确保card为锁定状态
	bool isExistTriplet(const CLMjCard & card, int &pos1, int &pos2);
	bool isExistSequence(const CLMjCard & card, int &pos1, int &pos2);
	bool isExistPair(const CLMjCard & card, int & pos1);
	bool isExistDoor(const CLMjCard & card, E_HandCardRelationType & doorType, int &pos1);

	

protected:
	CSiChuanMjLogic m_mjLogic;
	//手牌
	CLMjCard m_arrHandCard[14];
	int m_nHandNums;

	// 临时保存手牌状态,用于递归
	CLMjCard m_arrHandCardTemp[14];

	T_WeaveCardsItem m_arrWeaveCardsItem[4];
	int m_nWeaveCardsItemNum;

	int m_colorQue;

	CLMjCard m_cardOut;
	//CLMjCard m_cardNew;
	CLMjCard m_cardBadly;

	bool m_bHavePair;

	//当前分数
	int m_nScore;
	int m_nMaxScore;

};

