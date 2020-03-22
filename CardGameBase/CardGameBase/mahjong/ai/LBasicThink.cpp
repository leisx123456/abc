
#include "LBasicThink.h"
#include <iostream>

CLBasicThink::CLBasicThink()
{

}

CLBasicThink::~CLBasicThink()
{

}

bool CLBasicThink::isExistTriplet(CLMjCard aCards, int &pos1, int &pos2)
{
	//for
}











//
//void CLMjPlayer::thinkOutCard()
//{
//	m_vecActiveHandCards.push_back(m_CardNew);
//	if (isHaveTBA())
//	{
//		return;
//	}
//	markMeld();
//	//resetParticipated();
//	for (int i = 0; i < m_vecActiveHandCards.size(); ++i)
//	{
//		if (m_vecActiveHandCards[i].getRelationType() == EHC_Group)
//		{
//			m_vecActiveHandCards[i].setParticipated(true);
//			//此处不分开给分了，刻子，顺子统一以最低的给
//			m_vecActiveHandCards[i].setScore(SCORE_Sequence);
//		}
//	}
//	markOneToOne();
//	for (int i = 0; i < m_vecActiveHandCards.size(); ++i)
//	{
//		if (m_vecActiveHandCards[i].getRelationType() == EHC_OneToOne)
//		{
//			m_vecActiveHandCards[i].setParticipated(true);
//			//此处不分开给分了
//			m_vecActiveHandCards[i].setScore(SCORE_Door_Edge);
//		}
//	}
//	if (getMarkNum() == 14)
//	{
//		thinkNoSingle();
//	}
//	resetParticipatedInActive();
//	markSingle();
//}





//void CLMjPlayer::think32()
//{
//	int nPai, i;
//	//如果循环遍历只剩下4张就考虑
//	if (m_vecActiveHandCards.size() <= 4)
//	{
//		thinkSuplseFour();
//		return;
//	}
//	vector<CHandCard>::iterator iter;
//	//判断成为组的所有可能的方式，剔除组
//	for (i = 0; i < m_vecActiveHandCards.size(); i++)
//	{
//		iter = m_vecActiveHandCards.begin();
//		CHandCard nPai = (*iter);
//		m_vecActiveHandCards.erase(iter);
//		if (isExistTripletBeforeDelete(nPai.getCard()))
//		{
//			deleteSurplseTwoInTriplet(nPai.getCard());
//			think32();
//			m_vecActiveHandCards.push_back(nPai);//里面的其他成员变量变了，但最终没什么影响
//			m_vecActiveHandCards.push_back(nPai);
//		}
//		if (isExistSequenceBeforeDelete(nPai.getCard(), ES_MiddleCard))
//		{
//			deleteSurplseTwoInSequence(nPai.getCard(), ES_MiddleCard);
//			think32();
//			m_vecActiveHandCards.push_back(nPai - 1);
//			m_vecActiveHandCards.push_back(nPai + 1);
//		}
//		if (isExistSequenceBeforeDelete(nPai.getCard(), ES_LeftEdgeCard))
//		{
//			deleteSurplseTwoInSequence(nPai.getCard(), ES_LeftEdgeCard);
//			think32();
//			m_vecActiveHandCards.push_back(nPai + 1);
//			m_vecActiveHandCards.push_back(nPai + 2);
//		}
//		if (isExistSequenceBeforeDelete(nPai.getCard(), ES_RightEdgeCard))
//		{
//			deleteSurplseTwoInSequence(nPai.getCard(), ES_RightEdgeCard);
//			think32();
//			m_vecActiveHandCards.push_back(nPai - 1);
//			m_vecActiveHandCards.push_back(nPai - 2);
//		}
//		m_vecActiveHandCards.push_back(nPai);
//	}
//	//既没有剩四张，也不能找出一组也返回
//}
//
//
///*************************************************************/
///*函数功能：循环删除牌只有四张就可以考虑胡了
///*入口参数：无
///*返回类型：void
///*************************************************************/
//void CLMjPlayer::thinkSuplseFour()
//{
//	CHandCard nPai1, nPai2, nPai3, nPaiTemp;
//	int i, j;
//
//	//填补最后一张牌都必须满足3+2
//	//如2223, 2334, 2234同， 7789,7889,7899
//	//是否还能有一组（刻子，顺子）
//	vector<CHandCard>::iterator iter;
//	for (i = 0; i < 4; ++i) //此处若为iter = m_vecActiveHandCards.begin(); iter != m_vecActiveHandCards.end();会成为死循环
//	{
//		iter = m_vecActiveHandCards.begin();
//		nPai1 = (*iter);
//		m_vecActiveHandCards.erase(iter);
//		if (isExistTripletBeforeDelete(nPai1.getCard()))
//		{
//			deleteSurplseTwoInTriplet(nPai1.getCard());
//			addTingCard(m_vecActiveHandCards.at(0).getCard());
//			m_vecActiveHandCards.push_back(nPai1);//里面的其他成员变量变了，但最终没什么影响
//			m_vecActiveHandCards.push_back(nPai1);
//		}
//		if (isExistSequenceBeforeDelete(nPai1.getCard(), ES_MiddleCard))
//		{
//			deleteSurplseTwoInSequence(nPai1.getCard(), ES_MiddleCard);
//			addTingCard(m_vecActiveHandCards.at(0).getCard());
//			m_vecActiveHandCards.push_back(nPai1 - 1);
//			m_vecActiveHandCards.push_back(nPai1 + 1);
//		}
//		if (isExistSequenceBeforeDelete(nPai1.getCard(), ES_LeftEdgeCard))
//		{
//			deleteSurplseTwoInSequence(nPai1.getCard(), ES_LeftEdgeCard);
//			addTingCard(m_vecActiveHandCards.at(0).getCard());
//			m_vecActiveHandCards.push_back(nPai1 + 1);
//			m_vecActiveHandCards.push_back(nPai1 + 2);
//		}
//		if (isExistSequenceBeforeDelete(nPai1.getCard(), ES_RightEdgeCard))
//		{
//			deleteSurplseTwoInSequence(nPai1.getCard(), ES_RightEdgeCard);
//			addTingCard(m_vecActiveHandCards.at(0).getCard());
//			m_vecActiveHandCards.push_back(nPai1 - 1);
//			m_vecActiveHandCards.push_back(nPai1 - 2);
//		}
//		m_vecActiveHandCards.push_back(nPai1);
//		//++iter;
//	}
//
//	//循环遍历4张牌中是否有（一对将带门子的胡法）
//	for (i = 0; i < 4; i++)
//	{
//		iter = m_vecActiveHandCards.begin();
//		nPai1 = (*iter);
//		m_vecActiveHandCards.erase(iter);
//		vector<CHandCard>::iterator iter2;
//		for (j = 0; j < 3; j++)
//		{
//			iter2 = m_vecActiveHandCards.begin();
//			nPaiTemp = *iter2;//执行出错
//			//判断是否有一对将，有则判断是否有门子
//			if (nPai1 == nPaiTemp)
//			{
//				iter2 = m_vecActiveHandCards.erase(iter2); //为保险vc5和vc12的不同不能为m_vecActiveHandCards.erase(iter2);
//				//剩余的两张牌
//				nPai2 = (*iter2);
//				nPai3 = (*(iter2 + 1));
//				//如果剩余的两张牌相同（即是胡两个将）
//				if (nPai2 == nPai3)
//				{
//					addTingCard(nPai1.getCard());
//					addTingCard(nPai2.getCard());
//				}
//				//如果剩余的两张牌不同并且在一种样式以内，并且都不是字
//				if (nPai2.getCard() / 9 == nPai3.getCard() / 9)
//				{
//					//判断两张牌的门子是那种
//					switch (nPai2.getCard() - nPai3.getCard())
//					{
//					case 1://倒序门子如6-5，7-6等
//						if ((nPai2.getCard() - 8) % 9)//判断是否边界8
//						{
//							addTingCard(nPai2.getCard() + 1);
//						}
//						if ((nPai2.getCard() - 1) % 9)//判断是否边界1
//						{
//							addTingCard(nPai2.getCard() - 2);
//						}
//						break;
//					case -1://顺序门子如5-6，7-8等
//						if (nPai2.getCard() % 9)//判断是否边界0
//						{
//							addTingCard(nPai2.getCard() - 1);
//						}
//						if ((nPai2.getCard() + 1 - 8) % 9)//判断是否边界7
//						{
//							addTingCard(nPai2.getCard() + 2);
//						}
//						break;
//					case 2://倒序间隔一张的门子如4-2，6-4等
//						if (nPai2.getCard() % 9)
//						{
//							addTingCard(nPai2.getCard() - 1);
//						}
//						break;
//					case -2://顺序间隔一张的门子如2-4，4-6等
//						if ((nPai2.getCard() - 8) % 9)
//						{
//							addTingCard(nPai2.getCard() + 1);
//						}
//						break;
//					default:
//						break;
//					}
//				}
//				m_vecActiveHandCards.push_back(nPai1);
//			}
//		}
//		m_vecActiveHandCards.push_back(nPai1);
//	}
//}
//
//void CLMjPlayer::deleteSurplseTwoInSequence(int pai, E_SequenceType sequenceType)
//{
//	int nPai1, nPai2;
//	switch (sequenceType)
//	{
//	case ES_MiddleCard:
//		nPai1 = pai - 1;
//		nPai2 = pai + 1;
//		break;
//	case ES_LeftEdgeCard:
//		nPai1 = pai + 1;
//		nPai2 = pai + 2;
//		break;
//	case ES_RightEdgeCard:
//		nPai1 = pai - 1;
//		nPai2 = pai - 2;
//		break;
//	default:
//		break;
//	}
//	deleteCard(nPai1, 1);
//	deleteCard(nPai2, 1);
//}
//
//void CLMjPlayer::deleteSurplseTwoInTriplet(int nCard)
//{
//	deleteCard(nCard, 2);
//}
//
//int CLMjPlayer::deleteCard(int nCard, int nDeleteCardNum)
//{
//	int nCurDeleteCount = 0;
//	vector<CHandCard>::iterator iter;
//	for (iter = m_vecActiveHandCards.begin(); iter != m_vecActiveHandCards.end();)
//	{
//		if (iter->getCard() == nCard)
//		{
//			iter = m_vecActiveHandCards.erase(iter);
//			++nCurDeleteCount;
//		}
//		else
//		{
//			++iter;
//		}
//		if (nCurDeleteCount == nDeleteCardNum)
//		{
//			break;
//		}
//	}
//	return nCurDeleteCount;
//}
//
//
//
//bool CLMjPlayer::isExistSequenceBeforeDelete(int pai, E_SequenceType sequenceType)
//{
//	int nPai1, nPai2;
//	//switch (sequenceType)
//	//{
//	//case ES_MiddleCard:
//	//	nPai1 = pai - 1;
//	//	nPai2 = pai + 1;
//	//	break;
//	//case ES_LeftEdgeCard:
//	//	nPai1 = pai + 1;
//	//	nPai2 = pai + 2;
//	//	break;
//	//case ES_RightEdgeCard:
//	//	nPai1 = pai - 1;
//	//	nPai2 = pai - 2;
//	//	break;
//	//default:
//	//	break;
//	//}
//	//if (nPai1 / 9 != nPai2 / 9 || nPai1 / 9 != pai / 9
//	//	|| nPai1 >= 27 || nPai2 >= 27)	//判断是否是同种样式或者是字
//	//{
//	//	return false;
//	//}
//	//bool b1 = false, b2 = false;
//	//int i, nPaiTemp;
//	//for (i = 0; i < m_vecActiveHandCards.size(); i++)
//	//{
//	//	nPaiTemp = m_vecActiveHandCards.at(i).getCard();
//	//	if (nPai1 == nPaiTemp)
//	//	{
//	//		b1 = true;
//	//		break;
//	//	}
//	//}
//	//for (i = 0; i < m_vecActiveHandCards.size(); i++)
//	//{
//	//	nPaiTemp = m_vecActiveHandCards.at(i).getCard();
//	//	if (nPai2 == nPaiTemp)
//	//	{
//	//		b2 = true;
//	//		break;
//	//	}
//	//}
//	//牌1牌2都存在就为真,其余都为假
//	return (b1 && b2);
//}
//
//bool CLMjPlayer::isExistTripletBeforeDelete(int pai)
//{
//	//if (numByCard(pai) > 1)
//	//{
//	//	return true;
//	//}
//	//return false;
//}
//

//
///*************************************************************/
///*函数功能：考虑是否能胡牌
///*入口参数：无
///*返回类型：布尔，真就是有
///*************************************************************/
//bool CLMjPlayer::isHu()
//{
//	int i;
//	//胡牌只考虑活动区的牌，固定区的手牌不用
//	m_vecActiveHandCards.clear();
//	for (i = 0; i < 13 - m_nStartCard; i++)
//		m_vecActiveHandCards.push_back(m_arrHandCard[i + m_nStartCard]);
//	m_vecTing.clear();
//
//	if (isHaveTBA())
//	{
//		return false;
//	}
//
//	if ((m_vecActiveHandCards.size() - 1) % 3)
//	{
//		return false;
//	}
//	if (m_vecActiveHandCards.size() == 13)			//考虑七对的情况
//	{
//		thinkSevenPair();
//	}
//	if (m_vecActiveHandCards.size() == 1)			//如果只有一张就只添加当前这一只
//	{
//		m_vecTing.push_back(m_vecActiveHandCards.at(0).getCard());
//		return true;
//	}
//	think32();
//
//	for (i = 0; i < (int)m_vecTing.size(); i++)
//	{
//		//根据状态判断是否胡牌
//		if ((m_nOtherOutCard == (int)m_vecTing.at(i) && m_ePlayerActiveState == p_unActive)
//			|| (m_CardNew == (int)m_vecTing.at(i) && m_ePlayerActiveState == P_Active))
//			return true;
//	}
//	return false;
//}
//
//bool CLMjPlayer::isTing()
//{
//	return m_vecTing.size() > 0 ? true : false;
//}
//
//void CLMjPlayer::addTingCard(int nCard)
//{
//	if (nCard < 0 || nCard > 26)
//	{
//		return;
//	}
//	int i;
//	for (i = 0; i < m_vecTing.size(); i++)
//	{
//		if (nCard == m_vecTing.at(i))
//		{
//			break;
//		}
//	}
//	if (i == m_vecTing.size())
//	{
//		m_vecTing.push_back(nCard);
//	}
//	// using default comparison (operator <):
//	sort(m_vecTing.begin(), m_vecTing.end());
//}
//
//bool CLMjPlayer::isExistTriplet(int nCard, int &pos1, int &pos2)
//{
//	pos1 = findCardPos(nCard);
//	if (pos1 == -1)
//	{
//		return false;
//	}
//	pos2 = findCardPos(nCard, pos1 + 1);
//	if (pos2 == -1)
//	{
//		return false;
//	}
//	return true;
//}
//

//
//
///*************************************************************/
///*函数功能：是否有顺子
///*入口参数：顺子的种类type
///*返回类型：布尔,真就是有顺子，假就是没有
///*************************************************************/
//bool CLMjPlayer::isExistSequence(int type)
//{
//	int nPai1, nPai2;
//	if (type == 1)
//	{
//		nPai1 = m_nOtherOutCard - 1;
//		nPai2 = m_nOtherOutCard + 1;
//	}
//	else if (type == 0)
//	{
//		nPai1 = m_nOtherOutCard + 1;
//		nPai2 = m_nOtherOutCard + 2;
//	}
//	else
//	{
//		nPai1 = m_nOtherOutCard - 1;
//		nPai2 = m_nOtherOutCard - 2;
//	}
//	int pos1, pos2;
//	pos1 = findCardPos(nPai1, m_nStartCard);
//	pos2 = findCardPos(nPai2, m_nStartCard);
//	//牌1牌2都存在就为真,其余都为假
//	return (pos1 != -1 && pos2 != -1);
//}
//bool CLMjPlayer::isExistSequence(int nCard, int &pos1, int &pos2)
//{
//	if (nCard >= 27)
//	{
//		return false;
//	}
//	int nCard1 = nCard + 1;
//	int nCard2 = nCard + 2;
//
//	//判断是否有字是否在一个类别中
//	if (nCard1 / 9 != nCard2 / 9 || nCard1 / 9 != nCard / 9 || nCard1 >= 27 || nCard2 >= 27)
//	{
//		return false;
//	}
//	pos1 = findCardPos(nCard1);
//	pos2 = findCardPos(nCard2);
//	if (pos1 != -1 && pos2 != -1)
//	{
//		return true;
//	}
//	else
//	{
//		return false;
//	}
//}
//
//
//bool CLMjPlayer::isExistSequence(CHandCard handCard, int &pos1, int &pos2)
//{
//
//	CHandCard nCard1 = handCard + 1;
//	CHandCard nCard2 = handCard + 2;
//
//	//判断是否有字是否在一个类别中
//	if (nCard1 / 9 != nCard2 / 9 || nCard1 / 9 != handCard / 9 || nCard1 > 26 || nCard2 > 26)
//	{
//		return false;
//	}
//	pos1 = findPosInActiveHandCards(handCard);
//	pos2 = findPosInActiveHandCards(handCard);
//	if (pos1 != -1 && pos2 != -1)
//	{
//		return true;
//	}
//	else
//	{
//		return false;
//	}
//}
//

//
//bool CLMjPlayer::isExistDoor(CHandCard handCard, E_DoorType & doorType, int &pos1)
//{
//	//不可能同时存在任意两种情况
//	CHandCard handCard1;
//	//bool bHaveDoorTwoHead = false;
//	//bool bHaveDoorEdge = false;
//	//bool bHaveDoorMiddle = false;
//	handCard1 = handCard + 1;
//	//判断两牌是否同色是否是字
//	if (handCard1 / 9 == handCard / 9 && handCard1 < 27)
//	{
//		pos1 = findPosInActiveHandCards(handCard1);
//		if (pos1 != -1)
//		{
//			if (handCard % 9 == 0 || handCard1 % 9 == 8)
//			{
//				//bHaveDoorEdge = true;
//				doorType = ED_DoorEdge;
//				return true;
//			}
//			else
//			{
//				//bHaveDoorTwoHead = true;
//				doorType = ED_DoorTwoHead;
//				return true;
//			}
//		}
//	}
//
//	handCard1 = handCard + 2;
//	if (handCard1 / 9 == handCard / 9 && handCard1 < 27)
//	{
//		pos1 = findPosInActiveHandCards(handCard1);
//		if (pos1 != -1)
//		{
//			doorType = ED_DoorMiddle;
//			return true;
//		}
//	}
//	return false;
//}
//
//






