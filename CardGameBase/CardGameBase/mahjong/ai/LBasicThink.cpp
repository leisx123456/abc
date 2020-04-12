
#include "LBasicThink.h"
#include <iostream>

CLBasicThink::CLBasicThink()
{
	clear();
}

CLBasicThink::~CLBasicThink()
{

}


void CLBasicThink::clear()
{
	m_mjLogic.emptyCards(m_arrHandCard, 14);
	m_nHandNums = 14;
	m_mjLogic.emptyCards(m_arrHandCardTemp, 14);
	for (int i = 0; i < 4; ++i)
	{
		m_arrWeaveCardsItem[i].clear();
	}
	m_nWeaveCardsItemNum = 0;
	m_colorQue = -1;
	m_cardOut.empty();
	m_cardBadly.empty();
	m_bHavePair = false;
	m_nScore = 0;
	m_nMaxScore = 0;
}


void CLBasicThink::copyCards(CLMjCard aCards[], unsigned int unCardCount, T_WeaveCardsItem aWeaveItem[], unsigned int unItemSize, CLMjCard cardOut, int nQueColor)
{
	m_nHandNums = unCardCount;
	m_nWeaveCardsItemNum = unItemSize;
	m_mjLogic.copyCards(m_arrHandCard, m_nHandNums, aCards, unCardCount);
	for (int i = 0; i < unItemSize; ++i)
	{
		m_arrWeaveCardsItem[i] = aWeaveItem[i];
	}
	m_cardOut = cardOut;
	m_colorQue = nQueColor;
}


int CLBasicThink::findPosInHandCards(const CLMjCard & card)
{
	for (int i = 0; i < m_nHandNums; ++i)
	{
		if (m_arrHandCard[i].isLocked())
		{
			continue;
		}
		if (m_arrHandCard[i] == card)
		{
			return i;
		}
	}
	return -1;
}


bool CLBasicThink::isExistTriplet(const CLMjCard & card, int &pos1, int &pos2)
{
	pos1 = findPosInHandCards(card);
	if (-1 == pos1)
	{
		return false;
	}

	m_arrHandCard[pos1].lock();
	pos2 = findPosInHandCards(card);
	if (-1 == pos2)
	{
		m_arrHandCard[pos1].unLock();
		return false;
	}
	m_arrHandCard[pos1].unLock();
	return true;
}


bool CLBasicThink::isExistSequence(const CLMjCard & card, int &pos1, int &pos2)
{

	CLMjCard card1 = card + 1;
	CLMjCard card2 = card + 2;

	if (card.color() != card1.color()
		|| card.color() != card2.color()
		|| !card1.isOrderNumCard() 
		|| !card2.isOrderNumCard())
	{
		return false;
	}

	pos1 = findPosInHandCards(card1);
	pos2 = findPosInHandCards(card2);
	if (pos1 != -1 && pos2 != -1)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool CLBasicThink::isExistPair(const CLMjCard & card, int & pos1)
{
	pos1 = findPosInHandCards(card);
	if (-1 == pos1)
	{
		return false;
	}
	return true;
}

// 要求牌是从小到大排列的
bool CLBasicThink::isExistDoor(const CLMjCard & card, E_HandCardRelationType & doorType, int &pos1)
{
	//不可能同时存在任意两种情况
	CLMjCard card1 = card + 1;
	//判断两牌是否同色是否是字
	if (card1.color() == card.color() && card1.isOrderNumCard())
	{
		pos1 = findPosInHandCards(card1);
		if (pos1 != -1)
		{
			if (card.orderNumValue() == 1 || card1.orderNumValue()== 9)
			{
				doorType = EHC_Door_Edge;
				return true;
			}
			else
			{
				doorType = EHC_Door_TwoHead;
				return true;
			}
		}
	}

	card1 = card + 2;
	if (card1.color() == card.color() && card1.isOrderNumCard())
	{
		pos1 = findPosInHandCards(card1);
		if (pos1 != -1)
		{
			doorType = EHC_Door_Middle;
			return true;
		}
	}
	return false;
}








