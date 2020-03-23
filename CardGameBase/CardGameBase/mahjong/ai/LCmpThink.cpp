
#include "LCmpThink.h"


CLCmpThink::CLCmpThink()
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
	m_mapScore[EHC_Tba] = EHC_Tba;
	m_mapScore[EHC_Relation_Invaild] = SCORE_Badly;

}



// һ����õݹ�ɾ����
void CLCmpThink::thinkHu(CLMjCard aCards[], unsigned int unCardCount, T_WeaveCardsItem aWeaveItem[], unsigned int unItemSize, CLMjCard cardDest)
{
	//m_mjLogic.isCanHu(aCards, unCardCount, aWeaveItem, unItemSize, m_cardOut, m_cardNew);

}

CLMjCard CLCmpThink::thinkOutCard(CLMjCard aCards[], unsigned int unCardCount, T_WeaveCardsItem aWeaveItem[], unsigned int unItemSize, int nCardColor)
{

	if (nCardColor > 0 && nCardColor < 3)
	{
		markDingQue();
	}

	//
	m_nMaxScore = 0;
	m_nScore = 0;
	markThree();
	// �ѱ�������״̬���鿽������
	m_mjLogic.copyCards(m_arrHandCard, 14, m_arrHandCardTemp, 14);
	m_mjLogic.emptyCards(m_arrHandCardTemp, 14);

	//
	m_nMaxScore = 0;
	m_nScore = 0;
	markTwo();
	m_mjLogic.copyCards(m_arrHandCard, 14, m_arrHandCardTemp, 14);
	m_mjLogic.emptyCards(m_arrHandCardTemp, 14);

	if (getMarkNum() == 14)
	{
		thinkNoSingle();
	}

	markOne();
}


void CLCmpThink::markDingQue(int nCardColor)
{
	for (int i = 0; i < m_nHandNums; ++i)
	{
		if (m_arrHandCard[i].color() == nCardColor)
		{
			m_arrHandCard->lock(EHC_Tba);
		}
	}
}

void CLCmpThink::markThree()
{
	int pos1, pos2;
	for (int i = 0; i < m_nHandNums; ++i)
	{
		if (!m_arrHandCard[i].isLocked())
		{
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
	}

	//�ݹ��ȡ�õ�������Ƶ������
	if (m_nScore > m_nMaxScore)
	{
		m_nMaxScore = m_nScore;
		
		// �������״̬
		m_mjLogic.copyCards(m_arrHandCardTemp, 14, m_arrHandCard, 14);
	}
}


void CLCmpThink::markTwo()
{
	int pos1;
	for (int i = 0; i < m_nHandNums; i++)
	if (!m_arrHandCard[i].isLocked())
	{
		m_arrHandCard[i].lock(EHC_Pair);
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
			m_arrHandCard[pos1].lock(EHC_Pair);
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


void CLCmpThink::thinkNoSingle()
{
	bool bHavePair = false;
	

	CLMjCard arrCardsRelation2Temp[14];
	int nCardsRelation2Num = 0;

	for (int i = 0; i < m_nHandNums; ++i)
	{
		if (m_arrHandCard[i].isRelation2())
		{
			arrCardsRelation2Temp[nCardsRelation2Num++] = m_arrHandCard[i];
		}
	}

	//���ֻ��������
	if (nCardsRelation2Num == 2)
	{
		//�����ʣ����������ͬ�ͺ���
		if (arrCardsRelation2Temp[0] == arrCardsRelation2Temp[1])
		{
			m_CardBadly = CARD_EMPTY;
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
				//m_vecActiveHandCards[j].setScore(SCORE_Badly_Zero);
				m_CardBadly = m_arrHandCard[j];
				break;
			}
		}

		if (bHaveTriplet == false)
		{
			for (int i = 0; i < m_nHandNums; ++i)
			{
				if (m_arrHandCard[i] == arrCardsRelation2Temp[0] || m_arrHandCard[i] == arrCardsRelation2Temp[1])
				{
					m_arrHandCard[i].lock(EHC_Single);
				}
			}
		}
	}
	else
	{
		//����һ�飬�Ƚ��ĸ�������, �ѷ�������ÿһ��ĵ�һ������
		for (int j = 0; j < nCardsRelation2Num / 2; j++)
		{
			if (arrCardsRelation2Temp[j * 2] == arrCardsRelation2Temp[j * 2 + 1])
			{
				if (bHavePair)
				{
					arrCardsRelation2Temp[j].setScore(6);
				}
				else
				{
					arrCardsRelation2Temp[j].setScore(8);
					bHavePair = true;
				}
			}
			else if (arrCardsRelation2Temp[j * 2] == arrCardsRelation2Temp[j * 2 + 1] - 1)
			{
				if (arrCardsRelation2Temp[j * 2] % 9 == 0 || arrCardsRelation2Temp[j * 2 + 1] % 9 == 8)
				{
					arrCardsRelation2Temp[j].setScore(4);
				}
				else
				{
					arrCardsRelation2Temp[j].setScore(7);
				}
			}
			else
			{
				if (arrCardsRelation2Temp[j * 2] % 9 == 0 || arrCardsRelation2Temp[j * 2 + 1] % 9 == 8)
				{
					arrCardsRelation2Temp[j].setScore(3);
				}
				else
				{
					arrCardsRelation2Temp[j].setScore(5);
				}
			}

		}

		int nMinScore = 10;
		CLMjCard handCardBadly;
		//�ҳ�����һ���飬�����������ҳ�������
		for (int k = 0; k < nCardsRelation2Num / 2; k++)
		{
			if (arrCardsRelation2Temp[k].getScore() < nMinScore)
			{
				nMinScore = arrCardsRelation2Temp[k].getScore();
				if (arrCardsRelation2Temp[k * 2] % 9 == 0)
				{
					handCardBadly = arrCardsRelation2Temp[k * 2];
				}
				else if (arrCardsRelation2Temp[k * 2 + 1] % 9 == 8)
				{
					handCardBadly = arrCardsRelation2Temp[k * 2 + 1];
				}
				else
				{
					handCardBadly = arrCardsRelation2Temp[k * 2 + rand() % 2];
				}
			}
		}
		m_CardBadly = handCardBadly;
	}
}


void CLCmpThink::markOne()
{

	for (int i = 0; i < m_nHandNums; ++i)
	{
		if (!m_arrHandCard[i].isLocked())
		{

			markOne(m_arrHandCard[i]);
			addExtraScoreToSingleCard(m_arrHandCard[i]);
		}
	}


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

void CLCmpThink::markOne(CLMjCard & card)
{
	//�����һ���߾�
	if ((card.value() & MASK_VALUE == 1) || (card.value() & MASK_VALUE == 9))
	{
		card.setRelation(SCORE_Single_Edge);
	}
	else if ((card.value() & MASK_VALUE == 2) || (card.value() & MASK_VALUE == 8))
	{
		card.setRelation(SCORE_Single_Middle);
	}
	else
	{
		card.setRelation(SCORE_Single_TwoHead);
	}
}

void CLCmpThink::addExtraScoreToSingleCard(CLMjCard & card)
{
	//���ʣ��������б�����һ����
	if (card.value() & MASK_VALUE != 1 && findPosInActiveHandCards(card - 1) != -1)
	{
		if (card.value() & MASK_VALUE != 2)
		{
			card.addScore(SCORE_SingleToSingle_TwoHead);
		}
		else
		{
			card.addScore(SCORE_SingleToSingle_Middle);
		}
	}
	//���ʣ��������б����һ������
	if (card.value() & MASK_VALUE != 9 && findPosInActiveHandCards(card + 1) != -1)
	{
		if (card.value() & MASK_VALUE != 8)
		{
			card.addScore(SCORE_SingleToSingle_TwoHead);
		}
		else
		{
			card.addScore(SCORE_SingleToSingle_Middle);
		}
	}
	//���ʣ��������б����ٶ����ƣ���3��5,1_3�ȣ�
	if (card.value() & MASK_VALUE > 2 && findPosInActiveHandCards(card - 2) != -1)
	{
		card.addScore(SCORE_SingleToSingle_Middle);
	}

	//���ʣ��������б���������
	if (card.value() & MASK_VALUE < 8 && findPosInActiveHandCards(card + 2) != -1)
	{
		card.addScore(SCORE_SingleToSingle_Middle);
	}

}










void CLCmpThink::unLockAll()
{
	for (int i = 0; i < m_nHandNums; ++i)
	{
		m_arrHandCard[i].unLock();
	}
}

void CLCmpThink::commitScore()
{
	for (int i = 0; i < m_nHandNums; ++i)
	{
		if (m_arrHandCard[i].isLocked())
		{
			m_arrHandCard[i].setScore(m_mapScore[m_arrHandCard[i].getRelation()]);
			m_arrHandCard[i].unLock();
		}
	}
}







