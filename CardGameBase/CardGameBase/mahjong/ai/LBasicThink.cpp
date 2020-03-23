
#include "LBasicThink.h"
#include <iostream>

CLBasicThink::CLBasicThink()
{

}

CLBasicThink::~CLBasicThink()
{

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
	m_arrHandCard[pos2].lock();
	return true;
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





bool CLBasicThink::isExistPair(CLMjCard handCard, int & pos1)
{
	pos1 = findPosInActiveHandCards(handCard);
	if (-1 == pos1)
	{
		return false;
	}
	m_arrHandCard[pos1].lock();
	return true;
}






bool CLBasicThink::isExistSequence(CLMjCard handCard, int &pos1, int &pos2)
{

	CLMjCard nCard1 = handCard + 1;
	CLMjCard nCard2 = handCard + 2;

	//判断是否有字是否在一个类别中
	if (nCard1 / 9 != nCard2 / 9 || nCard1 / 9 != handCard / 9 || nCard1 > 26 || nCard2 > 26)
	{
		return false;
	}
	pos1 = findPosInActiveHandCards(handCard);
	pos2 = findPosInActiveHandCards(handCard);
	if (pos1 != -1 && pos2 != -1)
	{
		return true;
	}
	else
	{
		return false;
	}
}



bool CLBasicThink::isExistDoor(CLMjCard handCard, E_HandCardRelationType & doorType, int &pos1)
{
	//不可能同时存在任意两种情况
	CLMjCard handCard1;
	//bool bHaveDoorTwoHead = false;
	//bool bHaveDoorEdge = false;
	//bool bHaveDoorMiddle = false;
	handCard1 = handCard + 1;
	//判断两牌是否同色是否是字
	if (handCard1 / 9 == handCard / 9 && handCard1 < 27)
	{
		pos1 = findPosInActiveHandCards(handCard1);
		if (pos1 != -1)
		{
			if (handCard % 9 == 0 || handCard1 % 9 == 8)
			{
				//bHaveDoorEdge = true;
				doorType = EHC_Door_Edge;
				return true;
			}
			else
			{
				//bHaveDoorTwoHead = true;
				doorType = EHC_Door_TwoHead;
				return true;
			}
		}
	}

	handCard1 = handCard + 2;
	if (handCard1 / 9 == handCard / 9 && handCard1 < 27)
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








