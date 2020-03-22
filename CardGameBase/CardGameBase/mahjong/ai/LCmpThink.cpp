
#include "LCmpThink.h"




CLCmpThink::CLCmpThink()
{
	
}

/*************************************************************/
/*�������ܣ������Ƿ��ܺ���
/*��ڲ�������
/*�������ͣ��������������
/*************************************************************/
bool CLBasicThink::isHu()
{
	int i;
	//����ֻ���ǻ�����ƣ��̶��������Ʋ���
	m_arrHandCard.clear();
	for (i = 0; i < 13 - m_nStartCard; i++)
		m_arrHandCard.push_back(m_arrHandCard[i + m_nStartCard]);
	m_vecTing.clear();

	if (isHaveTBA())
	{
		return false;
	}

	if ((m_arrHandCard.size() - 1) % 3)
	{
		return false;
	}
	if (m_arrHandCard.size() == 13)			//�����߶Ե����
	{
		thinkSevenPair();
	}
	if (m_arrHandCard.size() == 1)			//���ֻ��һ�ž�ֻ��ӵ�ǰ��һֻ
	{
		m_vecTing.push_back(m_arrHandCard.at(0).getCard());
		return true;
	}
	think32();

	for (i = 0; i < (int)m_vecTing.size(); i++)
	{
		//����״̬�ж��Ƿ����
		if ((m_nOtherOutCard == (int)m_vecTing.at(i) && m_ePlayerActiveState == p_unActive)
			|| (m_CardNew == (int)m_vecTing.at(i) && m_ePlayerActiveState == P_Active))
			return true;
	}
	return false;
}

bool CLBasicThink::isTing()
{
	return m_vecTing.size() > 0 ? true : false;
}

void CLBasicThink::addTingCard(int nCard)
{
	if (nCard < 0 || nCard > 26)
	{
		return;
	}
	int i;
	for (i = 0; i < m_vecTing.size(); i++)
	{
		if (nCard == m_vecTing.at(i))
		{
			break;
		}
	}
	if (i == m_vecTing.size())
	{
		m_vecTing.push_back(nCard);
	}
	// using default comparison (operator <):
	sort(m_vecTing.begin(), m_vecTing.end());
}


void CLCmpThink::thinkHu(CLMjCard aCards[], unsigned int unCardCount, T_WeaveCardsItem aWeaveItem[], unsigned int unItemSize, CLMjCard cardDest)
{
	void CLBasicThink::think32()
	{
		int nPai, i;
		//���ѭ������ֻʣ��4�žͿ���
		if (m_arrHandCard.size() <= 4)
		{
			thinkSuplseFour();
			return;
		}
		vector<CHandCard>::iterator iter;
		//�жϳ�Ϊ������п��ܵķ�ʽ���޳���
		for (i = 0; i < m_arrHandCard.size(); i++)
		{
			iter = m_arrHandCard.begin();
			CHandCard nPai = (*iter);
			m_arrHandCard.erase(iter);
			if (isExistTripletBeforeDelete(nPai.getCard()))
			{
				deleteSurplseTwoInTriplet(nPai.getCard());
				think32();
				m_arrHandCard.push_back(nPai);//�����������Ա�������ˣ�������ûʲôӰ��
				m_arrHandCard.push_back(nPai);
			}
			if (isExistSequenceBeforeDelete(nPai.getCard(), ES_MiddleCard))
			{
				deleteSurplseTwoInSequence(nPai.getCard(), ES_MiddleCard);
				think32();
				m_arrHandCard.push_back(nPai - 1);
				m_arrHandCard.push_back(nPai + 1);
			}
			if (isExistSequenceBeforeDelete(nPai.getCard(), ES_LeftEdgeCard))
			{
				deleteSurplseTwoInSequence(nPai.getCard(), ES_LeftEdgeCard);
				think32();
				m_arrHandCard.push_back(nPai + 1);
				m_arrHandCard.push_back(nPai + 2);
			}
			if (isExistSequenceBeforeDelete(nPai.getCard(), ES_RightEdgeCard))
			{
				deleteSurplseTwoInSequence(nPai.getCard(), ES_RightEdgeCard);
				think32();
				m_arrHandCard.push_back(nPai - 1);
				m_arrHandCard.push_back(nPai - 2);
			}
			m_arrHandCard.push_back(nPai);
		}
		//��û��ʣ���ţ�Ҳ�����ҳ�һ��Ҳ����
	}

}

CLMjCard CLCmpThink::thinkOutCard(CLMjCard aCards[], unsigned int unCardCount, T_WeaveCardsItem aWeaveItem[], unsigned int unItemSize)
{


	m_arrHandCard.push_back(m_CardNew);
	if (isHaveTBA())
	{
		return;
	}
	markMeld();
	//resetParticipated();
	for (int i = 0; i < m_arrHandCard.size(); ++i)
	{
		if (m_arrHandCard[i].getRelationType() == EHC_Group)
		{
			m_arrHandCard[i].setParticipated(true);
			//�˴����ֿ������ˣ����ӣ�˳��ͳһ����͵ĸ�
			m_arrHandCard[i].setScore(SCORE_Sequence);
		}
	}
	markOneToOne();
	for (int i = 0; i < m_arrHandCard.size(); ++i)
	{
		if (m_arrHandCard[i].getRelationType() == EHC_OneToOne)
		{
			m_arrHandCard[i].setParticipated(true);
			//�˴����ֿ�������
			m_arrHandCard[i].setScore(SCORE_Door_Edge);
		}
	}
	if (getMarkNum() == 14)
	{
		thinkNoSingle();
	}
	resetParticipatedInActive();
	markSingle();
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
				m_arrHandCard[pos1].lock();
				m_arrHandCard[pos2].lock();
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





void CLCmpThink::resetParticipatedInActive()
{
	for (int i = 0; i < m_nHandNums; ++i)
	{
		m_arrHandCard[i].unLock();
	}
}




/*************************************************************/
/*�������ܣ�ѭ��ɾ����ֻ�����žͿ��Կ��Ǻ���
/*��ڲ�������
/*�������ͣ�void
/*************************************************************/
void CLBasicThink::thinkSuplseFour()
{
	CHandCard nPai1, nPai2, nPai3, nPaiTemp;
	int i, j;

	//����һ���ƶ���������3+2
	//��2223, 2334, 2234ͬ�� 7789,7889,7899
	//�Ƿ�����һ�飨���ӣ�˳�ӣ�
	vector<CHandCard>::iterator iter;
	for (i = 0; i < 4; ++i) //�˴���Ϊiter = m_arrHandCard.begin(); iter != m_arrHandCard.end();���Ϊ��ѭ��
	{
		iter = m_arrHandCard.begin();
		nPai1 = (*iter);
		m_arrHandCard.erase(iter);
		if (isExistTripletBeforeDelete(nPai1.getCard()))
		{
			deleteSurplseTwoInTriplet(nPai1.getCard());
			addTingCard(m_arrHandCard.at(0).getCard());
			m_arrHandCard.push_back(nPai1);//�����������Ա�������ˣ�������ûʲôӰ��
			m_arrHandCard.push_back(nPai1);
		}
		if (isExistSequenceBeforeDelete(nPai1.getCard(), ES_MiddleCard))
		{
			deleteSurplseTwoInSequence(nPai1.getCard(), ES_MiddleCard);
			addTingCard(m_arrHandCard.at(0).getCard());
			m_arrHandCard.push_back(nPai1 - 1);
			m_arrHandCard.push_back(nPai1 + 1);
		}
		if (isExistSequenceBeforeDelete(nPai1.getCard(), ES_LeftEdgeCard))
		{
			deleteSurplseTwoInSequence(nPai1.getCard(), ES_LeftEdgeCard);
			addTingCard(m_arrHandCard.at(0).getCard());
			m_arrHandCard.push_back(nPai1 + 1);
			m_arrHandCard.push_back(nPai1 + 2);
		}
		if (isExistSequenceBeforeDelete(nPai1.getCard(), ES_RightEdgeCard))
		{
			deleteSurplseTwoInSequence(nPai1.getCard(), ES_RightEdgeCard);
			addTingCard(m_arrHandCard.at(0).getCard());
			m_arrHandCard.push_back(nPai1 - 1);
			m_arrHandCard.push_back(nPai1 - 2);
		}
		m_arrHandCard.push_back(nPai1);
		//++iter;
	}

	//ѭ������4�������Ƿ��У�һ�Խ������ӵĺ�����
	for (i = 0; i < 4; i++)
	{
		iter = m_arrHandCard.begin();
		nPai1 = (*iter);
		m_arrHandCard.erase(iter);
		vector<CHandCard>::iterator iter2;
		for (j = 0; j < 3; j++)
		{
			iter2 = m_arrHandCard.begin();
			nPaiTemp = *iter2;//ִ�г���
			//�ж��Ƿ���һ�Խ��������ж��Ƿ�������
			if (nPai1 == nPaiTemp)
			{
				iter2 = m_arrHandCard.erase(iter2); //Ϊ����vc5��vc12�Ĳ�ͬ����Ϊm_arrHandCard.erase(iter2);
				//ʣ���������
				nPai2 = (*iter2);
				nPai3 = (*(iter2 + 1));
				//���ʣ�����������ͬ�����Ǻ���������
				if (nPai2 == nPai3)
				{
					addTingCard(nPai1.getCard());
					addTingCard(nPai2.getCard());
				}
				//���ʣ��������Ʋ�ͬ������һ����ʽ���ڣ����Ҷ�������
				if (nPai2.getCard() / 9 == nPai3.getCard() / 9)
				{
					//�ж������Ƶ�����������
					switch (nPai2.getCard() - nPai3.getCard())
					{
					case 1://����������6-5��7-6��
						if ((nPai2.getCard() - 8) % 9)//�ж��Ƿ�߽�8
						{
							addTingCard(nPai2.getCard() + 1);
						}
						if ((nPai2.getCard() - 1) % 9)//�ж��Ƿ�߽�1
						{
							addTingCard(nPai2.getCard() - 2);
						}
						break;
					case -1://˳��������5-6��7-8��
						if (nPai2.getCard() % 9)//�ж��Ƿ�߽�0
						{
							addTingCard(nPai2.getCard() - 1);
						}
						if ((nPai2.getCard() + 1 - 8) % 9)//�ж��Ƿ�߽�7
						{
							addTingCard(nPai2.getCard() + 2);
						}
						break;
					case 2://������һ�ŵ�������4-2��6-4��
						if (nPai2.getCard() % 9)
						{
							addTingCard(nPai2.getCard() - 1);
						}
						break;
					case -2://˳����һ�ŵ�������2-4��4-6��
						if ((nPai2.getCard() - 8) % 9)
						{
							addTingCard(nPai2.getCard() + 1);
						}
						break;
					default:
						break;
					}
				}
				m_arrHandCard.push_back(nPai1);
			}
		}
		m_arrHandCard.push_back(nPai1);
	}
}