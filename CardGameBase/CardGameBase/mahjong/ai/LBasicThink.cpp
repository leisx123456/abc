
#include "LBasicThink.h"
#include <iostream>

CLBasicThink::CLBasicThink()
{

}

CLBasicThink::~CLBasicThink()
{

}


void CLBasicThink::copyCards(CLMjCard aCards[], unsigned int unCardCount, T_WeaveCardsItem aWeaveItem[], unsigned int unItemSize, CLMjCard cardDest, int nQueColor)
{
	m_nHandNums = unCardCount;
	m_nWeaveCardsItemNum = unItemSize;
	m_mjLogic.copyCards(m_arrHandCard, m_nHandNums, aCards, unCardCount);
	for (int i = 0; i < unItemSize; ++i)
	{
		m_arrWeaveCardsItem[i] = aWeaveItem[i];
	}
	m_cardOut = cardDest;
	m_colorQue = nQueColor;
}


int CLBasicThink::findPosInActiveHandCards(CLMjCard card)
{
	for (int i = 0; i < m_nHandNums; ++i)
	{
		if (!m_arrHandCard[i].isLocked() && m_arrHandCard[i] == card)
		{
			return i;
		}
	}
	return -1;
}


bool CLBasicThink::isExistTriplet(CLMjCard card, int &pos1, int &pos2)
{
	pos1 = findPosInActiveHandCards(card);
	if (-1 == pos1)
	{
		return false;
	}

	m_arrHandCard[pos1].lock();
	pos2 = findPosInActiveHandCards(card);
	if (-1 == pos2)
	{
		m_arrHandCard[pos1].unLock();
		return false;
	}
	m_arrHandCard[pos1].unLock();
	return true;
}


bool CLBasicThink::isExistSequence(CLMjCard card, int &pos1, int &pos2)
{

	CLMjCard card1 = card + 1;
	CLMjCard card2 = card + 2;

	if (card1.color() != card2.color() 
		|| card1.color() != card.color()
		|| !card1.isOrderNumCard() 
		|| !card2.isOrderNumCard())
	{
		return false;
	}

	pos1 = findPosInActiveHandCards(card);
	pos2 = findPosInActiveHandCards(card);
	if (pos1 != -1 && pos2 != -1)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool CLBasicThink::isExistPair(CLMjCard handCard, int & pos1)
{
	pos1 = findPosInActiveHandCards(handCard);
	if (-1 == pos1)
	{
		return false;
	}
	return true;
}

// 要求牌是从小到大排列的
bool CLBasicThink::isExistDoor(CLMjCard card, E_HandCardRelationType & doorType, int &pos1)
{
	//不可能同时存在任意两种情况
	CLMjCard handCard1;

	handCard1 = card + 1;
	//判断两牌是否同色是否是字
	if (handCard1.color() == card.color() && handCard1.isOrderNumCard())
	{
		pos1 = findPosInActiveHandCards(handCard1);
		if (pos1 != -1)
		{
			if (card.orderNumValue() == 1 || handCard1.orderNumValue()== 9)
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

	handCard1 = card + 2;
	if (handCard1.color() == card.color() && handCard1.isOrderNumCard())
	{
		pos1 = findPosInActiveHandCards(handCard1);
		if (pos1 != -1)
		{
			doorType = EHC_Door_Middle;
			return true;
		}
	}
	return false;
}








