
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



bool CLBasicThink::isTing()
{
	
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


// 一般采用递归删除法
void CLCmpThink::thinkHu(CLMjCard aCards[], unsigned int unCardCount, T_WeaveCardsItem aWeaveItem[], unsigned int unItemSize, CLMjCard cardDest)
{
	//m_mjLogic.isCanHu(aCards, unCardCount, aWeaveItem, unItemSize, m_cardOut, m_cardNew);/*************************************************************/
	/*函数功能：循环删除牌只有四张就可以考虑胡了
	/*入口参数：无
	/*返回类型：void
	/*************************************************************/
	//void CLBasicThink::thinkSuplseFour()
	//{
	//	CLMjCard nPai1, nPai2, nPai3, nPaiTemp;
	//	int i, j;

	//	//填补最后一张牌都必须满足3+2
	//	//如2223, 2334, 2234同， 7789,7889,7899
	//	//是否还能有一组（刻子，顺子）
	//	vector<CLMjCard>::iterator iter;
	//	for (i = 0; i < 4; ++i) //此处若为iter = m_arrHandCard.begin(); iter != m_arrHandCard.end();会成为死循环
	//	{
	//		iter = m_arrHandCard.begin();
	//		nPai1 = (*iter);
	//		m_arrHandCard.erase(iter);
	//		if (isExistTripletBeforeDelete(nPai1))
	//		{
	//			deleteSurplseTwoInTriplet(nPai1);
	//			addTingCard(m_arrHandCard.at(0));
	//			m_arrHandCard.push_back(nPai1);//里面的其他成员变量变了，但最终没什么影响
	//			m_arrHandCard.push_back(nPai1);
	//		}
	//		if (isExistSequenceBeforeDelete(nPai1, ES_MiddleCard))
	//		{
	//			deleteSurplseTwoInSequence(nPai1, ES_MiddleCard);
	//			addTingCard(m_arrHandCard.at(0));
	//			m_arrHandCard.push_back(nPai1 - 1);
	//			m_arrHandCard.push_back(nPai1 + 1);
	//		}
	//		if (isExistSequenceBeforeDelete(nPai1, ES_LeftEdgeCard))
	//		{
	//			deleteSurplseTwoInSequence(nPai1, ES_LeftEdgeCard);
	//			addTingCard(m_arrHandCard.at(0));
	//			m_arrHandCard.push_back(nPai1 + 1);
	//			m_arrHandCard.push_back(nPai1 + 2);
	//		}
	//		if (isExistSequenceBeforeDelete(nPai1, ES_RightEdgeCard))
	//		{
	//			deleteSurplseTwoInSequence(nPai1, ES_RightEdgeCard);
	//			addTingCard(m_arrHandCard.at(0));
	//			m_arrHandCard.push_back(nPai1 - 1);
	//			m_arrHandCard.push_back(nPai1 - 2);
	//		}
	//		m_arrHandCard.push_back(nPai1);
	//		//++iter;
	//	}

	//	//循环遍历4张牌中是否有（一对将带门子的胡法）
	//	for (i = 0; i < 4; i++)
	//	{
	//		iter = m_arrHandCard.begin();
	//		nPai1 = (*iter);
	//		m_arrHandCard.erase(iter);
	//		vector<CLMjCard>::iterator iter2;
	//		for (j = 0; j < 3; j++)
	//		{
	//			iter2 = m_arrHandCard.begin();
	//			nPaiTemp = *iter2;//执行出错
	//			//判断是否有一对将，有则判断是否有门子
	//			if (nPai1 == nPaiTemp)
	//			{
	//				iter2 = m_arrHandCard.erase(iter2); //为保险vc5和vc12的不同不能为m_arrHandCard.erase(iter2);
	//				//剩余的两张牌
	//				nPai2 = (*iter2);
	//				nPai3 = (*(iter2 + 1));
	//				//如果剩余的两张牌相同（即是胡两个将）
	//				if (nPai2 == nPai3)
	//				{
	//					addTingCard(nPai1);
	//					addTingCard(nPai2);
	//				}
	//				//如果剩余的两张牌不同并且在一种样式以内，并且都不是字
	//				if (nPai2 / 9 == nPai3 / 9)
	//				{
	//					//判断两张牌的门子是那种
	//					switch (nPai2 - nPai3)
	//					{
	//					case 1://倒序门子如6-5，7-6等
	//						if ((nPai2 - 8) % 9)//判断是否边界8
	//						{
	//							addTingCard(nPai2 + 1);
	//						}
	//						if ((nPai2 - 1) % 9)//判断是否边界1
	//						{
	//							addTingCard(nPai2 - 2);
	//						}
	//						break;
	//					case -1://顺序门子如5-6，7-8等
	//						if (nPai2 % 9)//判断是否边界0
	//						{
	//							addTingCard(nPai2 - 1);
	//						}
	//						if ((nPai2 + 1 - 8) % 9)//判断是否边界7
	//						{
	//							addTingCard(nPai2 + 2);
	//						}
	//						break;
	//					case 2://倒序间隔一张的门子如4-2，6-4等
	//						if (nPai2 % 9)
	//						{
	//							addTingCard(nPai2 - 1);
	//						}
	//						break;
	//					case -2://顺序间隔一张的门子如2-4，4-6等
	//						if ((nPai2 - 8) % 9)
	//						{
	//							addTingCard(nPai2 + 1);
	//						}
	//						break;
	//					default:
	//						break;
	//					}
	//				}
	//				m_arrHandCard.push_back(nPai1);
	//			}
	//		}
	//		m_arrHandCard.push_back(nPai1);
	//	}
	//}

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
		m_arrHandCard[i].lock(EHC_Pair);
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

	//如果只有两张牌
	if (nCardsRelation2Num == 2)
	{
		//如果仅剩的两张牌相同就胡牌
		if (arrCardsRelation2Temp[0] == arrCardsRelation2Temp[1])
		{
			m_bIsHu = true;
			m_CardBadly = CARD_EMPTY;
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
		//两两一组，比较哪个组合最差, 把分数存在每一组的第一张牌中
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
		//找出最差的一个组，再在组中最找出最差的牌
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
	//如果是一或者九
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

void CLCmpThink::setMark(E_HandCardRelationType relationType)
{
	for (int i = 0; i < m_nHandNums; ++i)
	{
		//组合等级低的不能改变组合等级高的
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
	case EHC_Sequence:
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

void CLCmpThink::commitScore()
{

}








