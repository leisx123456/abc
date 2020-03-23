
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




void CLBasicThink::deleteSurplseTwoInSequence(int pai, E_SequenceType sequenceType)
{
	int nPai1, nPai2;
	switch (sequenceType)
	{
	case ES_MiddleCard:
		nPai1 = pai - 1;
		nPai2 = pai + 1;
		break;
	case ES_LeftEdgeCard:
		nPai1 = pai + 1;
		nPai2 = pai + 2;
		break;
	case ES_RightEdgeCard:
		nPai1 = pai - 1;
		nPai2 = pai - 2;
		break;
	default:
		break;
	}
	deleteCard(nPai1, 1);
	deleteCard(nPai2, 1);
}

void CLBasicThink::deleteSurplseTwoInTriplet(int nCard)
{
	deleteCard(nCard, 2);
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

int CLBasicThink::deleteCard(int nCard, int nDeleteCardNum)
{
	int nCurDeleteCount = 0;
	vector<CLMjCard>::iterator iter;
	for (iter = m_arrHandCard.begin(); iter != m_arrHandCard.end();)
	{
		if (iter->getCard() == nCard)
		{
			iter = m_arrHandCard.erase(iter);
			++nCurDeleteCount;
		}
		else
		{
			++iter;
		}
		if (nCurDeleteCount == nDeleteCardNum)
		{
			break;
		}
	}
	return nCurDeleteCount;
}



bool CLBasicThink::isExistSequenceBeforeDelete(int pai, E_SequenceType sequenceType)
{
	int nPai1, nPai2;
	//switch (sequenceType)
	//{
	//case ES_MiddleCard:
	//	nPai1 = pai - 1;
	//	nPai2 = pai + 1;
	//	break;
	//case ES_LeftEdgeCard:
	//	nPai1 = pai + 1;
	//	nPai2 = pai + 2;
	//	break;
	//case ES_RightEdgeCard:
	//	nPai1 = pai - 1;
	//	nPai2 = pai - 2;
	//	break;
	//default:
	//	break;
	//}
	//if (nPai1 / 9 != nPai2 / 9 || nPai1 / 9 != pai / 9
	//	|| nPai1 >= 27 || nPai2 >= 27)	//�ж��Ƿ���ͬ����ʽ��������
	//{
	//	return false;
	//}
	//bool b1 = false, b2 = false;
	//int i, nPaiTemp;
	//for (i = 0; i < m_arrHandCard.size(); i++)
	//{
	//	nPaiTemp = m_arrHandCard.at(i).getCard();
	//	if (nPai1 == nPaiTemp)
	//	{
	//		b1 = true;
	//		break;
	//	}
	//}
	//for (i = 0; i < m_arrHandCard.size(); i++)
	//{
	//	nPaiTemp = m_arrHandCard.at(i).getCard();
	//	if (nPai2 == nPaiTemp)
	//	{
	//		b2 = true;
	//		break;
	//	}
	//}
	//��1��2�����ھ�Ϊ��,���඼Ϊ��
	return (b1 && b2);
}

bool CLBasicThink::isExistTripletBeforeDelete(int pai)
{
	//if (numByCard(pai) > 1)
	//{
	//	return true;
	//}
	//return false;
}








/*************************************************************/
/*�������ܣ��Ƿ���˳��
/*��ڲ�����˳�ӵ�����type
/*�������ͣ�����,�������˳�ӣ��پ���û��
/*************************************************************/
bool CLBasicThink::isExistSequence(int type)
{
	int nPai1, nPai2;
	if (type == 1)
	{
		nPai1 = m_cardOut - 1;
		nPai2 = m_cardOut + 1;
	}
	else if (type == 0)
	{
		nPai1 = m_cardOut + 1;
		nPai2 = m_cardOut + 2;
	}
	else
	{
		nPai1 = m_cardOut - 1;
		nPai2 = m_cardOut - 2;
	}
	int pos1, pos2;
	pos1 = findCardPos(nPai1, m_nStartCard);
	pos2 = findCardPos(nPai2, m_nStartCard);
	//��1��2�����ھ�Ϊ��,���඼Ϊ��
	return (pos1 != -1 && pos2 != -1);
}
bool CLBasicThink::isExistSequence(int nCard, int &pos1, int &pos2)
{
	if (nCard >= 27)
	{
		return false;
	}
	int nCard1 = nCard + 1;
	int nCard2 = nCard + 2;

	//�ж��Ƿ������Ƿ���һ�������
	if (nCard1 / 9 != nCard2 / 9 || nCard1 / 9 != nCard / 9 || nCard1 >= 27 || nCard2 >= 27)
	{
		return false;
	}
	pos1 = findCardPos(nCard1);
	pos2 = findCardPos(nCard2);
	if (pos1 != -1 && pos2 != -1)
	{
		return true;
	}
	else
	{
		return false;
	}
}


bool CLBasicThink::isExistSequence(CLMjCard handCard, int &pos1, int &pos2)
{

	CLMjCard nCard1 = handCard + 1;
	CLMjCard nCard2 = handCard + 2;

	//�ж��Ƿ������Ƿ���һ�������
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
	//������ͬʱ���������������
	CLMjCard handCard1;
	//bool bHaveDoorTwoHead = false;
	//bool bHaveDoorEdge = false;
	//bool bHaveDoorMiddle = false;
	handCard1 = handCard + 1;
	//�ж������Ƿ�ͬɫ�Ƿ�����
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








