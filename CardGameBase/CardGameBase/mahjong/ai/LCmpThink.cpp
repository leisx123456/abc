
#include "LCmpThink.h"
#include <assert.h>

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
	m_mapScore[EHC_Tba] = SCORE_TBA;
	m_mapScore[EHC_Badly] = SCORE_Badly;

}


int CLCmpThink::thinkDingQue(CLMjCard aCards[], unsigned int unCardCount)
{
	int nColorQue = -1;
	unsigned int arrColorIndex[3];
	memset(arrColorIndex, 0, sizeof(unsigned int)* 3);

	for (int i = 0; i < unCardCount; ++i)
	{
		assert(aCards[i].color() < 3);
		arrColorIndex[aCards[i].color()]++;
	}

	int fewest = 14;
	for (int i = 0; i < 3; ++i)
	{
		if (arrColorIndex[i] < fewest)
		{
			fewest = arrColorIndex[i];
			nColorQue = i;
		}
	}

	return nColorQue;
}

// 一般采用递归删除法
bool CLCmpThink::thinkHu(CLMjCard aCards[], unsigned int unCardCount, T_WeaveCardsItem aWeaveItem[], unsigned int unItemSize, CLMjCard cardDest)
{
	//m_mjLogic.isCanHu(aCards, unCardCount, aWeaveItem, unItemSize, m_cardOut, m_cardNew);
	return false;
}

bool CLCmpThink::thinkKong(CLMjCard aCards[], unsigned int unCardCount, T_WeaveCardsItem aWeaveItem[], unsigned int unItemSize, CLMjCard cardDest)
{
	return false;
}

bool CLCmpThink::thinkPong(CLMjCard aCards[], unsigned int unCardCount, T_WeaveCardsItem aWeaveItem[], unsigned int unItemSize, CLMjCard cardDest)
{
	return false;
}

CLMjCard CLCmpThink::thinkOutCard(CLMjCard aCards[], unsigned int unCardCount, T_WeaveCardsItem aWeaveItem[], unsigned int unItemSize, int nCardColor)
{
	if (nCardColor > -1 && nCardColor < 3)
	{
		int pos = isExistQue(nCardColor);
		if (pos > 0)
		{
			// 如果有缺直接返回
			return m_arrHandCard[pos];
		}

	}

	//
	m_nMaxScore = 0;
	m_nScore = 0;
	markThree();
	// 把保存的最佳状态数组拷贝回来
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

	//
	commitScore();
	saveBadlyCardByScore();

	return m_cardBadly;
}


int CLCmpThink::isExistQue(int nCardColor)
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

	//递归获取得到最大优势的牌组合
	if (m_nScore > m_nMaxScore)
	{
		m_nMaxScore = m_nScore;
		
		// 保存这个状态
		m_mjLogic.copyCards(m_arrHandCardTemp, 14, m_arrHandCard, 14);
	}
}


void CLCmpThink::markTwo()
{
	int pos1;
	for (int i = 0; i < m_nHandNums; i++)
	if (!m_arrHandCard[i].isLocked())
	{
		m_arrHandCard[i].lock();
		//是否有对子
		if (isExistPair(m_arrHandCard[i], pos1))
		{
			int nScorePair = SCORE_Pair;
			//如果没将就设置将
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

		//是否有门子
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

	CLMjCard arrCardsRelation2Temp[14];
	int nCardsRelation2Num = 0;

	for (int i = 0; i < m_nHandNums; ++i)
	{
		if (m_arrHandCard[i].isRelation2())
		{
			// 重载了赋值函数，赋值时会把状态都复制过去
			arrCardsRelation2Temp[nCardsRelation2Num++] = m_arrHandCard[i];
		}
	}

	//如果只有两张牌
	if (nCardsRelation2Num == 2)
	{
		//如果仅剩的两张牌相同就胡牌
		if (arrCardsRelation2Temp[0] == arrCardsRelation2Temp[1])
		{
			return;
		}

		//如果两张牌不同就凑一对将
		bool bHaveTriplet = false;
		for (int j = 0; m_nHandNums; j++)
		{
			//看是否有暗刻
			if (m_arrHandCard[j] == m_arrHandCard[j + 1] && m_arrHandCard[j] == m_arrHandCard[j + 2])
			{
				bHaveTriplet = true;
				// 差的牌就解锁 清除组合关系 并分配到单牌判断中
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
		/*如果大于2张*/
		// 找出最差的两张解锁分配到单牌判断中
		// 循环2次
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


void CLCmpThink::markOne()
{
	// 如果从上面下来没有单张，说明胡了
	if (getMarkNum() == 14)
	{
		return;
	}

	// 先添加额外分
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
	//如果是一或者九
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

void CLCmpThink::addExtraScoreToSingleCard(CLMjCard & card)
{
	//如果剩余的牌中有比其少一的牌
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
	//如果剩余的牌中有比起多一个的牌
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
	//如果剩余的牌中有比其少二的牌（如3—5,1_3等）
	if (card.value() & MASK_VALUE > 2 && findPosInActiveHandCards(card - 2) != -1)
	{
		card.addScore(SCORE_SingleToSingle_Middle);
	}

	//如果剩余的牌中有比其多二的牌
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
			m_arrHandCard[i].addScore(m_mapScore[m_arrHandCard[i].getRelation()]);
			m_arrHandCard[i].unLock();
		}
	}
}

// 确保已经提交分数解除锁定，锁定状态的牌认为还未提交分数
void CLCmpThink::saveBadlyCardByScore()
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









