
#include "LScoreBasicThink.h"
#include <assert.h>

CLScoreBasicThink::CLScoreBasicThink()
{
	m_mapScore.insert(std::pair<int, int>(EHC_Hu, SCORE_Hu));
	m_mapScore[EHC_Kong] = SCORE_Kong;
	m_mapScore[EHC_Triplet] = SCORE_Triplet;
	m_mapScore[EHC_Sequence] = SCORE_Sequence;
	m_mapScore[EHC_Pair_only] = SCORE_Pair_Only;
	m_mapScore[EHC_Pair] = SCORE_Pair;
	m_mapScore[EHC_Door_TwoHead] = SCORE_Door_TwoHead;
	m_mapScore[EHC_Door_Middle] = SCORE_Door_Middle;
	m_mapScore[EHC_Door_Edge] = SCORE_Door_Edge;
	m_mapScore[EHC_Single_34567] = SCORE_Single_TwoHead;
	m_mapScore[EHC_Single_28] = SCORE_Single_Middle;
	m_mapScore[EHC_Single_19] = SCORE_Single_Edge;
	m_mapScore[EHC_SingleToSingle_TwoHead] = SCORE_SingleToSingle_TwoHead;
	m_mapScore[EHC_SingleToSingle_Middle] = SCORE_SingleToSingle_Middle;
	m_mapScore[EHC_ingleToSingle_Edge] = SCORE_SingleToSingle_Edge;
	m_mapScore[EHC_Tba] = SCORE_TBA;
	m_mapScore[EHC_Badly] = SCORE_Badly;
}



void CLScoreBasicThink::think()
{
	m_nMaxScore = 0;
	m_nScore = 0;
	markThree();
	m_mjLogic.copyCards(m_arrHandCard, 14, m_arrHandCardTemp, 14); // �ѱ�������״̬���鿽������

	//
	m_nMaxScore = 0;
	m_nScore = 0;
	markTwo();
	m_mjLogic.copyCards(m_arrHandCard, 14, m_arrHandCardTemp, 14);

	if (getMarkNum() == 14)
	{
		thinkNoSingle();
	}
	markOne();

	//
	commitScore();
	saveBadlyCardByScore();
}


int CLScoreBasicThink::isExistQue(int nCardColor)
{
	for (int i = 0; i < m_nHandNums; ++i)
	{
		if (m_arrHandCard[i].color() == nCardColor)
		{
			m_arrHandCard->lock(EHC_Tba);
			return i;
		}
	}
	return -1;
}

void CLScoreBasicThink::markThree()
{
	int pos1, pos2;
	for (int i = 0; i < m_nHandNums; ++i)
	{
		if (m_arrHandCard[i].isLocked())
		{
			continue;
		}
		m_arrHandCard[i].lock(EHC_Triplet);
		if (isExistTriplet(m_arrHandCard[i], pos1, pos2))
		{
			m_arrHandCard[pos1].lock(EHC_Triplet);
			m_arrHandCard[pos2].lock(EHC_Triplet);
			m_nScore += SCORE_Triplet;
			markThree();
			m_nScore -= SCORE_Triplet;
			m_arrHandCard[pos1].unLock(true);
			m_arrHandCard[pos2].unLock(true);
		}
		m_arrHandCard[i].unLock(true);

		m_arrHandCard[i].lock(EHC_Sequence);
		if (isExistSequence(m_arrHandCard[i], pos1, pos2))
		{
			m_arrHandCard[pos1].lock(EHC_Sequence);
			m_arrHandCard[pos2].lock(EHC_Sequence);
			m_nScore += SCORE_Sequence;
			markThree();
			m_nScore -= SCORE_Sequence;
			m_arrHandCard[pos1].unLock(true);
			m_arrHandCard[pos2].unLock(true);
		}
		m_arrHandCard[i].unLock(true);
	}

	//�ݹ��ȡ�õ�������Ƶ������
	if (m_nScore > m_nMaxScore)
	{
		m_nMaxScore = m_nScore;

		// �������״̬
		m_mjLogic.copyCards(m_arrHandCardTemp, 14, m_arrHandCard, 14);
	}
}


void CLScoreBasicThink::markTwo()
{
	int pos1;
	for (int i = 0; i < m_nHandNums; i++)
	{
		if (m_arrHandCard[i].isLocked())
		{
			continue;
		}
		m_arrHandCard[i].lock();
		//�Ƿ��ж���
		if (isExistPair(m_arrHandCard[i], pos1))
		{
			int nScorePair = SCORE_Pair;
			//���û�������ý�
			if (m_bHavePair == false)
			{
				nScorePair = SCORE_Pair_Only;
				m_bHavePair = true;
			}
			m_nScore += nScorePair;
			m_arrHandCard[i].lock(nScorePair);
			m_arrHandCard[pos1].lock(nScorePair);
			markTwo();
			m_arrHandCard[pos1].unLock(true);
			if (nScorePair == SCORE_Pair_Only)
			{
				m_bHavePair = false;
			}
			m_nScore -= nScorePair;
		}
		m_arrHandCard[i].unLock(true);

		//�Ƿ�������
		m_arrHandCard[i].lock();
		E_HandCardRelationType doorType;
		if (isExistDoor(m_arrHandCard[i], doorType, pos1))
		{
			int nScoreDoor = 0;
			switch (doorType)
			{
			case EHC_Door_TwoHead:
				nScoreDoor = SCORE_Door_TwoHead;
				break;
			case EHC_Door_Middle:
				nScoreDoor = SCORE_Door_Middle;
				break;
			case EHC_Door_Edge:
				nScoreDoor = SCORE_Door_Edge;
				break;
			default:
				break;
			}
			m_arrHandCard[i].lock(doorType);
			m_nScore += nScoreDoor;
			m_arrHandCard[pos1].lock(doorType);
			markTwo();
			m_arrHandCard[pos1].unLock(true);
			m_nScore -= nScoreDoor;
		}
		m_arrHandCard[i].unLock(true);
	}


	if (m_nScore > m_nMaxScore)
	{
		m_nMaxScore = m_nScore;
		m_mjLogic.copyCards(m_arrHandCardTemp, 14, m_arrHandCard, 14);
	}
}


void CLScoreBasicThink::thinkNoSingle()
{

	CLMjCard arrCardsRelation2Temp[14];
	int nCardsRelation2Num = 0;

	for (int i = 0; i < m_nHandNums; ++i)
	{
		if (m_arrHandCard[i].isRelation2())
		{
			// �����˸�ֵ��������ֵʱ���״̬�����ƹ�ȥ
			arrCardsRelation2Temp[nCardsRelation2Num++] = m_arrHandCard[i];
		}
	}

	//���ֻ��������
	if (nCardsRelation2Num == 2)
	{
		//�����ʣ����������ͬ�ͺ���
		if (arrCardsRelation2Temp[0] == arrCardsRelation2Temp[1])
		{
			return;
		}

		//��������Ʋ�ͬ�ʹ�һ�Խ�
		bool bHaveTriplet = false;
		for (int j = 0; m_nHandNums; j++)
		{
			//���Ƿ��а���
			if (m_arrHandCard[j] == m_arrHandCard[j + 1] && m_arrHandCard[j] == m_arrHandCard[j + 2])
			{
				bHaveTriplet = true;
				// ����ƾͽ��� �����Ϲ�ϵ �����䵽�����ж���
				m_arrHandCard[j].unLock(true);
				break;
			}
		}

		if (bHaveTriplet == false)
		{
			for (int i = 0; i < m_nHandNums; ++i)
			{
				if (m_arrHandCard[i] == arrCardsRelation2Temp[0])
				{
					m_arrHandCard[i].unLock(true);
				}
				if (m_arrHandCard[i] == arrCardsRelation2Temp[1])
				{
					m_arrHandCard[i].unLock(true);
				}
			}
		}
	}
	else
	{
		/*�������2��*/
		// �ҳ��������Ž������䵽�����ж���
		// ѭ��2��
		for (int count = 0; count < 2; count++)
		{
			int posBadly = -1;
			int nBadRelation = EHC_Hu;
			for (int i = 0; i < m_nHandNums; ++i)
			{
				if (m_arrHandCard[i].isLocked())
				{
					if (m_arrHandCard[i].getRelation() > nBadRelation)
					{
						nBadRelation = m_arrHandCard[i].getRelation();
						posBadly = i;
					}
				}
			}
			m_arrHandCard[posBadly].unLock(true);
		}
	}
}


void CLScoreBasicThink::markOne()
{
	// �������������û�е��ţ�˵������
	if (getMarkNum() == 14)
	{
		return;
	}

	// �����Ӷ����
	for (int i = 0; i < m_nHandNums; ++i)
	{
		if (!m_arrHandCard[i].isLocked())
		{
			addExtraScoreToSingleCard(m_arrHandCard[i]);
		}
	}

	//
	for (int i = 0; i < m_nHandNums; ++i)
	{
		if (!m_arrHandCard[i].isLocked())
		{
			markOne(m_arrHandCard[i]);
		}
	}


}

int CLScoreBasicThink::getMarkNum()
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

void CLScoreBasicThink::markOne(CLMjCard & card)
{
	//�����һ���߾�
	if (card.isYaoJiuOrderNum_19())
	{
		card.lock(SCORE_Single_Edge);
	}
	else if (card.isJaing_28())
	{
		card.lock(SCORE_Single_Middle);
	}
	else
	{
		card.lock(SCORE_Single_TwoHead);
	}
}

void CLScoreBasicThink::addExtraScoreToSingleCard(CLMjCard & card)
{
	//���ʣ��������б�����һ����
	if ((card.value() & MASK_VALUE) != 1 && findPosInHandCards(card - 1) != -1)
	{
		if ((card.value() & MASK_VALUE) != 2)
		{
			card.addScore(SCORE_SingleToSingle_TwoHead);
		}
		else
		{
			card.addScore(SCORE_SingleToSingle_Middle);
		}
	}
	//���ʣ��������б����һ������
	if ((card.value() & MASK_VALUE) != 9 && findPosInHandCards(card + 1) != -1)
	{
		if ((card.value() & MASK_VALUE) != 8)
		{
			card.addScore(SCORE_SingleToSingle_TwoHead);
		}
		else
		{
			card.addScore(SCORE_SingleToSingle_Middle);
		}
	}
	//���ʣ��������б����ٶ����ƣ���3��5,1_3�ȣ�
	if ((card.value() & MASK_VALUE) > 2 && findPosInHandCards(card - 2) != -1)
	{
		card.addScore(SCORE_SingleToSingle_Middle);
	}

	//���ʣ��������б���������
	if ((card.value() & MASK_VALUE) < 8 && findPosInHandCards(card + 2) != -1)
	{
		card.addScore(SCORE_SingleToSingle_Middle);
	}

}



void CLScoreBasicThink::unLockAll()
{
	for (int i = 0; i < m_nHandNums; ++i)
	{
		m_arrHandCard[i].unLock();
	}
}

void CLScoreBasicThink::commitScore()
{
	for (int i = 0; i < m_nHandNums; ++i)
	{
		if (m_arrHandCard[i].isLocked())
		{
			m_arrHandCard[i].addScore(m_mapScore[m_arrHandCard[i].getRelation()]);
			m_arrHandCard[i].unLock();
		}
	}

	m_nTotalScore = 0;
	for (int i = 0; i < m_nHandNums; ++i)
	{
		if (!m_arrHandCard[i].isLocked())
		{
			m_nTotalScore += m_arrHandCard[i].getScore();
		}
	}
}

// ȷ���Ѿ��ύ�����������������״̬������Ϊ��δ�ύ����
void CLScoreBasicThink::saveBadlyCardByScore()
{
	int pos = 0;
	int badlySore = SCORE_Hu;
	for (int i = 0; i < m_nHandNums; ++i)
	{
		if (!m_arrHandCard[i].isLocked())
		{
			if (m_arrHandCard[i].getScore() < badlySore)
			{
				badlySore = m_arrHandCard[i].getScore();
				pos = i;
			}

		}
	}
	m_cardBadly = m_arrHandCard[pos];
}

int CLScoreBasicThink::totalScore()
{
	return m_nTotalScore;
}









