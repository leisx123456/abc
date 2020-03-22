
#include "LCmpThink.h"




CLCmpThink::CLCmpThink()
{
	
}

void CLCmpThink::markMeld()
{
	int pos1, pos2;
	for (int i = 0; i < m_nHandNums; ++i)
	{
		if (!m_arrHandCard[i].isLocked())
		{
			m_arrHandCard[i].lock();
			if (isExistTriplet(m_arrHandCard[i], pos1, pos2))
			{
				//m_vecActiveHandCards[pos1].lock();
				//m_vecActiveHandCards[pos2].lock();
				m_nScore += SCORE_Triplet;
				markMeld();
				m_nScore -= SCORE_Triplet;
				m_arrHandCard[pos1].unLock();
				m_arrHandCard[pos2].unLock();
			}
			if (isExistSequence(m_arrHandCard[i].getCard(), pos1, pos2))
			{
				m_arrHandCard[pos1].lock();
				m_arrHandCard[pos2].lock();
				m_nScore += SCORE_Sequence;
				markMeld();
				m_nScore -= SCORE_Sequence;
				m_arrHandCard[pos1].unLock();
				m_arrHandCard[pos2].unLock();
			}
			m_arrHandCard[i].unLock();
		}
	}

	//�ݹ��ȡ�õ�������Ƶ������
	if (m_nScore > m_nMaxScore)
	{
		m_nMaxScore = m_nScore;
		setMark(EHC_Group);
	}
}


void CLCmpThink::markOneToOne()
{
	int pos1;
	for (int i = 0; i < m_nHandNums; i++)
	if (!m_arrHandCard[i].isLocked())
	{
		m_arrHandCard[i].lock();
		//�Ƿ��ж���
		if (isExistPair(m_arrHandCard[i], pos1))
		{
			int nScorePair = SCORE_Pair_Only;
			//���û�������ý�
			if (m_bHavePair == false)
			{
				nScorePair = SCORE_Pair_Only;
				m_bHavePair = true;
			}
			m_nScore += nScorePair;
			m_arrHandCard[pos1].lock();
			markOneToOne();
			m_arrHandCard[pos1].unLock();
			if (nScorePair == SCORE_Pair_Only)
			{
				m_bHavePair = false;
			}
			m_nScore -= nScorePair;
		}

		//�Ƿ�������
		E_DoorType doorType;
		if (isExistDoor(m_arrHandCard[i], doorType, pos1))
		{
			int nScoreDoor = 0;
			switch (doorType)
			{
			case ED_DoorTwoHead:
				nScoreDoor = SCORE_Door_TwoHead;
				break;
			case ED_DoorMiddle:
				nScoreDoor = SCORE_Door_Middle;
				break;
			case ED_DoorEdge:
				nScoreDoor = SCORE_Door_Edge;
				break;
			default:
				break;
			}
			m_nScore += nScoreDoor;
			m_arrHandCard[pos1].lock();
			markOneToOne();
			m_arrHandCard[pos1].unLock();
			m_nScore -= nScoreDoor;
		}
		m_arrHandCard[i].unLock();
	}
	//int nMarkNum = getMarkNum();
	if (m_nScore > m_nMaxScore)
	{
		//m_nMaxMarkNum = nMarkNum;
		setMark(EHC_OneToOne);
	}
}


void CLCmpThink::thinkNoSingle()
{
	bool bHavePair = false;
	bool bHaveTriplet = false;
	vector<CLMjCard> vecTempCard;
	for (int i = 0; i < m_nHandNums; ++i)
	{
		if (m_arrHandCard[i].getRelationType() == EHC_OneToOne)
		{
			vecTempCard.push_back(m_arrHandCard[i]);
		}
	}
	//���ֻ��������
	if (vecTempCard.size() == 2)
	{
		//�����ʣ����������ͬ�ͺ���
		if (vecTempCard[0] == vecTempCard[1])
		{
			m_bIsHu = true;
			return;
		}
		//��������Ʋ�ͬ�ʹ�һ�Խ�
		for (int j = 0; m_nHandNums; j++)
		{
			//���Ƿ��а���
			if (m_arrHandCard[j] == m_arrHandCard[j + 1]
				&& m_arrHandCard[j] == m_arrHandCard[j + 2])
			{
				bHaveTriplet = true;
				//m_vecActiveHandCards[j].setScore(SCORE_Badly_Zero);
				m_nBadlyCard = m_arrHandCard[j].getCard();
				break;
			}
		}
		if (bHaveTriplet == false)
		{
			for (int i = 0; i < m_nHandNums; ++i)
			{
				if (m_arrHandCard[i] == vecTempCard[0] || m_arrHandCard[i] == vecTempCard[1])
				{
					m_arrHandCard[i].setRelationType(EHC_Single);
				}
			}
		}
	}
	else
	{
		//����һ�飬�Ƚ��ĸ�������, �ѷ�������ÿһ��ĵ�һ������
		for (int j = 0; j < vecTempCard.size() / 2; j++)
		{
			if (vecTempCard[j * 2] == vecTempCard[j * 2 + 1])
			{
				if (bHavePair)
				{
					vecTempCard[j].setScore(6);
				}
				else
				{
					vecTempCard[j].setScore(8);
					bHavePair = true;
				}
			}
			else if (vecTempCard[j * 2] == vecTempCard[j * 2 + 1] - 1)
			{
				if (vecTempCard[j * 2] % 9 == 0 || vecTempCard[j * 2 + 1] % 9 == 8)
				{
					vecTempCard[j].setScore(4);
				}
				else
				{
					vecTempCard[j].setScore(7);
				}
			}
			else
			{
				if (vecTempCard[j * 2] % 9 == 0 || vecTempCard[j * 2 + 1] % 9 == 8)
				{
					vecTempCard[j].setScore(3);
				}
				else
				{
					vecTempCard[j].setScore(5);
				}
			}

		}
		int nMinScore = 10;
		CLMjCard handCardBadly;
		//�ҳ�����һ���飬�����������ҳ�������
		for (int k = 0; k < vecTempCard.size() / 2; k++)
		{
			if (vecTempCard[k].getScore() < nMinScore)
			{
				nMinScore = vecTempCard[k].getScore();
				if (vecTempCard[k * 2] % 9 == 0)
				{
					handCardBadly = vecTempCard[k * 2];
				}
				else if (vecTempCard[k * 2 + 1] % 9 == 8)
				{
					handCardBadly = vecTempCard[k * 2 + 1];
				}
				else
				{
					handCardBadly = vecTempCard[k * 2 + rand() % 2];
				}
			}
		}
		m_nBadlyCard = handCardBadly.getCard();
	}
}


void CLCmpThink::markSingle()
{
	CLMjCard badlyHandCrd;
	badlyHandCrd.setScore(20);
	for (int i = 0; i < m_nHandNums; ++i)
	{
		if (m_arrHandCard[i].getRelationType() == EHC_Single)
		{
			m_arrHandCard[i].lock();
			addScoreBySelf(m_arrHandCard[i]);
			addScoreByRelation(m_arrHandCard[i]);
			m_arrHandCard[i].unLock();
			if (m_arrHandCard[i].getScore() < badlyHandCrd.getScore())
			{
				badlyHandCrd = m_arrHandCard[i];
			}
		}
	}
	m_nBadlyCard = badlyHandCrd.getCard();
}

int CLCmpThink::getMarkNum()
{
	int nNum = 0;
	for (int i = 0; i < m_nHandNums; ++i)
	{
		if (m_arrHandCard[i].isLocked())
		{
			++nNum;
		}
	}
	return nNum;
}

void CLCmpThink::addScoreBySelf(CLMjCard & handCard)
{
	//�����һ���߾�
	if (handCard % 9 == 0 || handCard % 9 == 8)
	{
		handCard.addScore(SCORE_Single_Edge);
	}
	else
	{
		handCard.addScore(SCORE_Single_Middle);
	}
}

void CLCmpThink::addScoreByRelation(CLMjCard & handCard)
{
	//���ʣ��������б�����һ����
	if (handCard % 9 != 0 && findPosInActiveHandCards(handCard - 1) != -1)
	{
		if (handCard % 9 != 1)
		{
			handCard.addScore(SCORE_Single_R_TwoHead);
		}
		else
		{
			handCard.addScore(SCORE_Single_R_Edge);
		}
	}
	//���ʣ��������б����һ������
	if (handCard % 9 != 8 && findPosInActiveHandCards(handCard + 1) != -1)
	{
		if (handCard % 9 != 7)
		{
			handCard.addScore(SCORE_Single_R_TwoHead);
		}
		else
		{
			handCard.addScore(SCORE_Single_R_Edge);
		}
	}
	//���ʣ��������б����ٶ����ƣ���3��5,1_3�ȣ�
	if (handCard % 9 > 1 && findPosInActiveHandCards(handCard - 2) != -1)
	{
		handCard.addScore(SCORE_Single_R_Middle);
	}

	//���ʣ��������б���������
	if (handCard % 9 < 7 && findPosInActiveHandCards(handCard + 2) != -1)
	{
		handCard.addScore(SCORE_Single_R_Middle);
	}

}

void CLCmpThink::setMark(E_HandCardRelationType relationType)
{
	for (int i = 0; i < m_nHandNums; ++i)
	{
		//��ϵȼ��͵Ĳ��ܸı���ϵȼ��ߵ�
		if (relationType > m_arrHandCard[i].getRelationType())
		{
			continue;
		}
		m_arrHandCard[i].setRelationType(EHC_Single);
		if (m_arrHandCard[i].isLocked())
		{
			m_arrHandCard[i].setRelationType(relationType);
		}
	}
}

void CLCmpThink::setScore(E_HandCardRelationType relationType)
{
	int nScore;
	switch (relationType)
	{
	case EHC_Group:
		nScore = 300;
		break;
	case EHC_OneToOne:
		nScore = 100;
		break;
	case EHC_Single:
		nScore = 10;
		break;
	default:
		break;
	}
	for (int i = 0; i < m_nHandNums; ++i)
	{
		if (m_arrHandCard[i].isLocked())
		{
			m_arrHandCard[i].setScore(nScore);
		}
	}
}





void CLCmpThink::resetParticipated()
{
	for (int i = 0; i < 13; ++i)
	{
		m_arrHandCard[i].unLock();
	}
}


int CLCmpThink::findPosInActiveHandCards(CLMjCard handCard)
{
	for (int i = 0; i < m_nHandNums; ++i)
	{
		if (!m_arrHandCard[i].isLocked() && m_arrHandCard[i] == handCard)
		{
			return i;
		}
	}
	return -1;
}


void CLCmpThink::resetParticipatedInActive()
{
	for (int i = 0; i < m_nHandNums; ++i)
	{
		m_arrHandCard[i].unLock();
	}
}