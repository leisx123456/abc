
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
//			//�˴����ֿ������ˣ����ӣ�˳��ͳһ����͵ĸ�
//			m_vecActiveHandCards[i].setScore(SCORE_Sequence);
//		}
//	}
//	markOneToOne();
//	for (int i = 0; i < m_vecActiveHandCards.size(); ++i)
//	{
//		if (m_vecActiveHandCards[i].getRelationType() == EHC_OneToOne)
//		{
//			m_vecActiveHandCards[i].setParticipated(true);
//			//�˴����ֿ�������
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
//	//���ѭ������ֻʣ��4�žͿ���
//	if (m_vecActiveHandCards.size() <= 4)
//	{
//		thinkSuplseFour();
//		return;
//	}
//	vector<CHandCard>::iterator iter;
//	//�жϳ�Ϊ������п��ܵķ�ʽ���޳���
//	for (i = 0; i < m_vecActiveHandCards.size(); i++)
//	{
//		iter = m_vecActiveHandCards.begin();
//		CHandCard nPai = (*iter);
//		m_vecActiveHandCards.erase(iter);
//		if (isExistTripletBeforeDelete(nPai.getCard()))
//		{
//			deleteSurplseTwoInTriplet(nPai.getCard());
//			think32();
//			m_vecActiveHandCards.push_back(nPai);//�����������Ա�������ˣ�������ûʲôӰ��
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
//	//��û��ʣ���ţ�Ҳ�����ҳ�һ��Ҳ����
//}
//
//
///*************************************************************/
///*�������ܣ�ѭ��ɾ����ֻ�����žͿ��Կ��Ǻ���
///*��ڲ�������
///*�������ͣ�void
///*************************************************************/
//void CLMjPlayer::thinkSuplseFour()
//{
//	CHandCard nPai1, nPai2, nPai3, nPaiTemp;
//	int i, j;
//
//	//����һ���ƶ���������3+2
//	//��2223, 2334, 2234ͬ�� 7789,7889,7899
//	//�Ƿ�����һ�飨���ӣ�˳�ӣ�
//	vector<CHandCard>::iterator iter;
//	for (i = 0; i < 4; ++i) //�˴���Ϊiter = m_vecActiveHandCards.begin(); iter != m_vecActiveHandCards.end();���Ϊ��ѭ��
//	{
//		iter = m_vecActiveHandCards.begin();
//		nPai1 = (*iter);
//		m_vecActiveHandCards.erase(iter);
//		if (isExistTripletBeforeDelete(nPai1.getCard()))
//		{
//			deleteSurplseTwoInTriplet(nPai1.getCard());
//			addTingCard(m_vecActiveHandCards.at(0).getCard());
//			m_vecActiveHandCards.push_back(nPai1);//�����������Ա�������ˣ�������ûʲôӰ��
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
//	//ѭ������4�������Ƿ��У�һ�Խ������ӵĺ�����
//	for (i = 0; i < 4; i++)
//	{
//		iter = m_vecActiveHandCards.begin();
//		nPai1 = (*iter);
//		m_vecActiveHandCards.erase(iter);
//		vector<CHandCard>::iterator iter2;
//		for (j = 0; j < 3; j++)
//		{
//			iter2 = m_vecActiveHandCards.begin();
//			nPaiTemp = *iter2;//ִ�г���
//			//�ж��Ƿ���һ�Խ��������ж��Ƿ�������
//			if (nPai1 == nPaiTemp)
//			{
//				iter2 = m_vecActiveHandCards.erase(iter2); //Ϊ����vc5��vc12�Ĳ�ͬ����Ϊm_vecActiveHandCards.erase(iter2);
//				//ʣ���������
//				nPai2 = (*iter2);
//				nPai3 = (*(iter2 + 1));
//				//���ʣ�����������ͬ�����Ǻ���������
//				if (nPai2 == nPai3)
//				{
//					addTingCard(nPai1.getCard());
//					addTingCard(nPai2.getCard());
//				}
//				//���ʣ��������Ʋ�ͬ������һ����ʽ���ڣ����Ҷ�������
//				if (nPai2.getCard() / 9 == nPai3.getCard() / 9)
//				{
//					//�ж������Ƶ�����������
//					switch (nPai2.getCard() - nPai3.getCard())
//					{
//					case 1://����������6-5��7-6��
//						if ((nPai2.getCard() - 8) % 9)//�ж��Ƿ�߽�8
//						{
//							addTingCard(nPai2.getCard() + 1);
//						}
//						if ((nPai2.getCard() - 1) % 9)//�ж��Ƿ�߽�1
//						{
//							addTingCard(nPai2.getCard() - 2);
//						}
//						break;
//					case -1://˳��������5-6��7-8��
//						if (nPai2.getCard() % 9)//�ж��Ƿ�߽�0
//						{
//							addTingCard(nPai2.getCard() - 1);
//						}
//						if ((nPai2.getCard() + 1 - 8) % 9)//�ж��Ƿ�߽�7
//						{
//							addTingCard(nPai2.getCard() + 2);
//						}
//						break;
//					case 2://������һ�ŵ�������4-2��6-4��
//						if (nPai2.getCard() % 9)
//						{
//							addTingCard(nPai2.getCard() - 1);
//						}
//						break;
//					case -2://˳����һ�ŵ�������2-4��4-6��
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
//	//	|| nPai1 >= 27 || nPai2 >= 27)	//�ж��Ƿ���ͬ����ʽ��������
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
//	//��1��2�����ھ�Ϊ��,���඼Ϊ��
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
///*�������ܣ������Ƿ��ܺ���
///*��ڲ�������
///*�������ͣ��������������
///*************************************************************/
//bool CLMjPlayer::isHu()
//{
//	int i;
//	//����ֻ���ǻ�����ƣ��̶��������Ʋ���
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
//	if (m_vecActiveHandCards.size() == 13)			//�����߶Ե����
//	{
//		thinkSevenPair();
//	}
//	if (m_vecActiveHandCards.size() == 1)			//���ֻ��һ�ž�ֻ��ӵ�ǰ��һֻ
//	{
//		m_vecTing.push_back(m_vecActiveHandCards.at(0).getCard());
//		return true;
//	}
//	think32();
//
//	for (i = 0; i < (int)m_vecTing.size(); i++)
//	{
//		//����״̬�ж��Ƿ����
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
///*�������ܣ��Ƿ���˳��
///*��ڲ�����˳�ӵ�����type
///*�������ͣ�����,�������˳�ӣ��پ���û��
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
//	//��1��2�����ھ�Ϊ��,���඼Ϊ��
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
//	//�ж��Ƿ������Ƿ���һ�������
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
//	//�ж��Ƿ������Ƿ���һ�������
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
//	//������ͬʱ���������������
//	CHandCard handCard1;
//	//bool bHaveDoorTwoHead = false;
//	//bool bHaveDoorEdge = false;
//	//bool bHaveDoorMiddle = false;
//	handCard1 = handCard + 1;
//	//�ж������Ƿ�ͬɫ�Ƿ�����
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






