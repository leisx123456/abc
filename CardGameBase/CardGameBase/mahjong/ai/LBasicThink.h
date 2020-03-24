#pragma once
#include "AbstractThink.h"
#include "../LMjLogic.h"


class CLBasicThink : public IAbstractThink
{
public:
	CLBasicThink();
	virtual ~CLBasicThink();

	int findPosInActiveHandCards(CLMjCard handCard);

	// ���溯��ȷ��cardΪ����״̬
	bool isExistTriplet(CLMjCard card, int &pos1, int &pos2);
	bool isExistSequence(CLMjCard card, int &pos1, int &pos2);
	bool isExistPair(CLMjCard card, int & pos1);
	bool isExistDoor(CLMjCard card, E_HandCardRelationType & doorType, int &pos1);


protected:

	//����
	CLMjCard m_arrHandCard[14];
	int m_nHandNums;

	// ��ʱ��������״̬,���ڵݹ�
	CLMjCard m_arrHandCardTemp[14];

	T_WeaveCardsItem m_arrWeaveCardsItem[4];
	int m_nWeaveCardsItemNum;

	CLMjCard m_cardOut;
	CLMjCard m_cardNew;
	CLMjCard m_cardBadly;

	bool m_bHavePair;

	//��ǰ����
	int m_nScore;
	int m_nMaxScore;

};

