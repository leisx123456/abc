//#include "stdafx.h"
//#include "Mahjong.h"
#include "BasicThink.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBasicThink::CBasicThink():m_bHaveDui(false)
{

}

CBasicThink::~CBasicThink()
{

}
/*************************************************************/
/*�������ܣ������齫��
/*��ڲ�����ָ�루�齫���飩�����ͣ�������
/*�������ͣ�void
/*************************************************************/
void CBasicThink::SetMjPaiArray(int *nPailist, int nNum)
{
	for( int i = 0;i < 14;i++ )
	{
		m_bMjSelect[i] = false;
	}
	m_nCheckNum = 0;
	m_nMaxScoreThree = m_nMaxScoreTwo = m_nMaxScoreOne = 0;
	m_nMjNum = nNum;
	m_nScoreThree = m_nScoreTwo = 0;
	m_nBadly = -1;
	
	for(int j = 0;j < nNum;j++)
	{
		m_nMajiang[j] = nPailist[j];
	}

	m_nNeedPai = nPailist[nNum-1];				//������Ҫ����Ϊ���һ��
	SortMj(m_nMajiang,nNum);					//������
}

/*************************************************************/
/*�������ܣ�������
/*��ڲ�����ָ�루�Ƶ����飩�����ͣ�����λ�ã�
/*�������ͣ�void
/*************************************************************/
void CBasicThink::SortMj(int *who,int end)
{
	int i,j,nPai;
	for( i = 0 ; i < end-1 ; i++ )
	{
		for( j = i+1 ; j < end ; j++ )
            if( who[i] > who[j] )
			{
				nPai =  who[i];
				who[i] = who[j];
				who[j] = nPai;
			}
	}
}

/*************************************************************/
/*�������ܣ���ÿ��Ǻ��������
/*��ڲ�������
/*�������ͣ�����
/*************************************************************/
int CBasicThink::GetMaxScore()
{
	return m_nMaxScoreThree + m_nMaxScoreTwo;
}

/*************************************************************/
/*�������ܣ��������������
/*��ڲ�������
/*�������ͣ�����
/*************************************************************/
int CBasicThink::GetBadly()
{
	return m_nBadly;
}

/*************************************************************/
/*�������ܣ���ñ��˳��ƺ���ϵ�������
/*��ڲ��������ͣ����ͣ�����
/*�������ͣ�void
/*************************************************************/
void CBasicThink::GetGoodTeam(int &pai1, int &pai2, int &pai3)
{
	pai1=m_nTotalTeam[0];
	pai2=m_nTotalTeam[1];
	pai3=m_nTotalTeam[2];
}

/*************************************************************/
/*�������ܣ�������
/*��ڲ��������ͣ�Ҫ�ҵ��ǳ��ƣ������ͣ���ʼλ�ã�
/*�������ͣ����ͣ�λ�ã�
/*************************************************************/
int CBasicThink::FindPos(int pai,int nStart)
{
	for( int i = nStart;i < m_nMjNum;i++ )
	{
		if( m_nMajiang[i] == pai && m_bMjSelect[i] == false )
		{
			return i;
		}
	}
	return -1;
}

/*************************************************************/
/*�������ܣ��Ӽ�������ź�ʣ������в�����
/*��ڲ��������ͣ�Ҫ�ҵ��ǳ��ƣ������ͣ���ʼλ�ã�
/*�������ͣ����ͣ�λ�ã�
/*************************************************************/
int CBasicThink::FindPosThree(int pai,int nStart)
{
	for( int i = nStart;i < m_nRemainNumThree;i++ )
	{
		if( m_nRemainThree[i] == pai && m_bMjSelect[i] == false )
		{
			return i;
		}
	}
	return -1;
}

/*************************************************************/
/*�������ܣ��Ƿ���3��һ����
/*��ڲ��������ͣ����ͣ����ͣ�
/*�������ͣ��������������
/*************************************************************/
bool CBasicThink::IsSameTeam(int pai,int &pos1,int &pos2)
{
	pos1 = FindPos(pai);
	if( pos1 == -1 )
	{
		return false;
	}
	pos2 = FindPos(pai,pos1+1);
	if( pos2 == -1 )
	{
		return false;
	}
    return true;
}

/*************************************************************/
/*�������ܣ��Ƿ���3��˳�ӵ�
/*��ڲ��������ͣ����ͣ����ͣ�
/*�������ͣ��������������
/*************************************************************/
bool CBasicThink::IsStepTeam(int pai,int &pos1,int &pos2)
{
    if( pai >= 27 ) 
	{
		return false;
	}
	int nPai1 = pai+1;
	int nPai2 = pai+2;
	//�ж��Ƿ������Ƿ���һ�������
	if( nPai1/9 != nPai2/9 || nPai1/9 != pai/9 || nPai1 >= 27 || nPai2 >= 27 )
	{
		return false;
	}
	pos1 = FindPos(nPai1);
	pos2 = FindPos(nPai2);
	if( pos1 != -1 && pos2 != -1)
	{
		return true;
	}
	else
	{
		return false;
	}
}

/*************************************************************/
/*�������ܣ������������Ƿ��Ǳ�Ե�ģ�0��8��
/*��ڲ��������ͣ�����
/*�������ͣ��������������
/*************************************************************/
bool CBasicThink::IsEdge(int pai1,int pai2)
{
	//pai1 < pai2
	if( pai1%9 == 0 || pai2%9 == 8 )
	{
		return true;
	}
	return false;
}

/*************************************************************/
/*�������ܣ������ż�������Ƿ��ж���
/*��ڲ��������ͣ����ͣ����ͣ�
/*�������ͣ��������������
/*************************************************************/
bool CBasicThink::IsSameTeamThree(int pai,int &pos1,int nStart)
{
	pos1 = FindPosThree(pai,nStart);
	return pos1 == -1 ? false:true;
}

/*************************************************************/
/*�������ܣ���������������Ƿ������������
/*��ڲ��������ͣ����ͣ����ͣ�����
/*�������ͣ��������������
/*************************************************************/
bool CBasicThink::IsStepTeamThree(int pai,int type,int &pos1,int nStart)
{
    if( pai >= 27 ) 
	{
		return false;
	}
	int nPai1;
	if(type == 1)
	{
		nPai1 = pai+1;
	}
	else 
	{
		nPai1 = pai+2;
	}
	//�ж������Ƿ�ͬɫ�Ƿ�����
	if(  nPai1/9 != pai/9 || nPai1 >= 27  )
	{
		return false;
	}
	pos1 = FindPosThree(nPai1,nStart);
	return pos1 == -1 ? false:true;
	
}

/*************************************************************/
/*�������ܣ�����3�ŵ����
/*��ڲ��������ͣ����ࣩ
/*�������ͣ�void
/*************************************************************/
void CBasicThink::SetTeamPai( int type)
{
	switch(type)
	{
	case 0:		//3��һ����
		m_nGoodTeamThree[0] = m_nGoodTeamThree[1] = m_nGoodTeamThree[2] = m_nNeedPai;
		break;
	case 1:
		m_nGoodTeamThree[0] = m_nNeedPai;
		m_nGoodTeamThree[1] = m_nNeedPai+1;
		m_nGoodTeamThree[2] = m_nNeedPai+2;
		break;
	case 2:
		m_nGoodTeamThree[0] = m_nNeedPai-1;
		m_nGoodTeamThree[1] = m_nNeedPai;
		m_nGoodTeamThree[2] = m_nNeedPai+1;
		break;
	case 3:
		m_nGoodTeamThree[0] = m_nNeedPai-2;
		m_nGoodTeamThree[1] = m_nNeedPai-1;
		m_nGoodTeamThree[2] = m_nNeedPai;
		break;
	}
}






///*************************************************************/
///*�������ܣ����Գ���
///*��ڲ��������ͣ��ƣ�
///*�������ͣ����ͣ������ǳ��ƣ�
///*************************************************************/
//int CLMjPlayer::cmpOutCard(int nPai)
//{
//	if (m_CardNew != nPai)
//	{
//		int pos;
//		pos = findCardPos(nPai, m_nStartCard);
//		if (pos != -1)
//		{
//			m_arrHandCard[pos].setCard(m_CardNew);
//		}
//		sortCards(m_nStartCard);
//	}
//	m_lstCardOut.push_back(nPai);
//	m_CardNew = -1;
//	m_nOtherOutCard = -1;
//	m_ePlayerActiveState = E_PlayerActiveState::p_unActive;
//	return nPai;
//}
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


//void CLMjPlayer::selectTBA()
//{
//
//	int nCircleNum = 0;
//	int nCharacterNum = 0;
//	int nBamboo = 0;
//	int nMinNum = 14;
//	int i;
//	//����Ѿ���ȱ�򷵻�
//	if (m_eTBA != EO_Invalid)
//	{
//		return;
//	}
//	//�������ж�ȱ
//	for (i = 0; i < 13; ++i)
//	{
//		if (m_arrHandCard[i] > -1 && m_arrHandCard[i] < 9)
//		{
//			++nCircleNum;
//		}
//		else if (m_arrHandCard[i] > 8 && m_arrHandCard[i] < 18)
//		{
//			++nCharacterNum;
//		}
//		else
//		{
//			++nBamboo;
//		}
//	}
//
//	//�����ׯ���ж�����
//	CHandCard newHandCard;
//	newHandCard = m_CardNew;
//	E_OrderNumCardType type = newHandCard.getONCardType();
//	switch (type)
//	{
//	case EO_Circle:
//		++nCircleNum;
//		break;
//	case EO_Character:
//		++nCharacterNum;
//		break;
//	case EO_Bamboo:
//		++nBamboo;
//		break;
//	case EO_Invalid:
//		break;
//	default:
//		break;
//	}
//
//	nMinNum = nCircleNum;
//	m_eTBA = EO_Circle;
//	if (nCharacterNum < nMinNum)
//	{
//		nMinNum = nCharacterNum;
//		m_eTBA = EO_Character;
//	}
//	if (nBamboo < nMinNum)
//	{
//		nMinNum = nBamboo;
//		m_eTBA = EO_Bamboo;
//	}
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
//int CLMjPlayer::numByCard(int nCard)
//{
//	//int nCardNum = 0;
//	//vector<CHandCard>::iterator iter;
//	//for (iter = m_vecActiveHandCards.begin(); iter != m_vecActiveHandCards.end(); ++iter)
//	//{
//	//	if (iter->getCard() == nCard)
//	//	{
//	//		++nCardNum;
//	//	}
//	//}
//	//return nCardNum;
//}
//
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
//bool CLMjPlayer::isExistTriplet(CHandCard handCard, int &pos1, int &pos2)
//{
//	pos1 = findPosInActiveHandCards(handCard);
//	if (-1 == pos1)
//	{
//		return false;
//	}
//	m_vecActiveHandCards[pos1].setParticipated(true);
//	pos2 = findPosInActiveHandCards(handCard);
//	if (-1 == pos2)
//	{
//		m_vecActiveHandCards[pos1].setParticipated(false);
//		return false;
//	}
//	m_vecActiveHandCards[pos2].setParticipated(true);
//	return true;
//}
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
//bool CLMjPlayer::isExistPair(CHandCard handCard, int & pos1)
//{
//	pos1 = findPosInActiveHandCards(handCard);
//	if (-1 == pos1)
//	{
//		return false;
//	}
//	m_vecActiveHandCards[pos1].setParticipated(true);
//	return true;
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
//
///*************************************************************/
///*�������ܣ�����ָ���Ƶ�λ��
///*��ڲ��������ͣ������ƣ������ͣ������￪ʼ�ң������ͣ��ҵ����������
///*�������ͣ����ͣ��ҵ���λ�ã��Ҳ�������-1��
///*************************************************************/
//int CLMjPlayer::findCardPos(int nCard, int nStart /*= 0*/, int nEnd /*= 13*/)
//{
//	for (int i = nStart; i < nEnd; ++i)
//	{
//		if (m_arrHandCard[i] == nCard)
//		{
//			return i;
//		}
//	}
//	return -1;
//}
//
//
//
//bool CLMjPlayer::isHaveTBA()
//{
//	for (int i = 0; i < m_vecActiveHandCards.size(); ++i)
//	{
//		if (isTBA(m_vecActiveHandCards[i]))
//		{
//			m_nBadlyCard = m_vecActiveHandCards[i].getCard();
//			return true;
//		}
//	}
//	return false;
//}
//
//void CLMjPlayer::markMeld()
//{
//	int pos1, pos2;
//	for (int i = 0; i < m_vecActiveHandCards.size(); ++i)
//	{
//		if (!m_vecActiveHandCards.at(i).isParticipated())
//		{
//			m_vecActiveHandCards.at(i).setParticipated(true);
//			if (isExistTriplet(m_vecActiveHandCards.at(i), pos1, pos2))
//			{
//				//m_vecActiveHandCards.at(pos1).setParticipated(true);
//				//m_vecActiveHandCards.at(pos2).setParticipated(true);
//				m_nScore += SCORE_Triplet;
//				markMeld();
//				m_nScore -= SCORE_Triplet;
//				m_vecActiveHandCards.at(pos1).setParticipated(false);
//				m_vecActiveHandCards.at(pos2).setParticipated(false);
//			}
//			if (isExistSequence(m_vecActiveHandCards.at(i).getCard(), pos1, pos2))
//			{
//				m_vecActiveHandCards.at(pos1).setParticipated(true);
//				m_vecActiveHandCards.at(pos2).setParticipated(true);
//				m_nScore += SCORE_Sequence;
//				markMeld();
//				m_nScore -= SCORE_Sequence;
//				m_vecActiveHandCards.at(pos1).setParticipated(false);
//				m_vecActiveHandCards.at(pos2).setParticipated(false);
//			}
//			m_vecActiveHandCards.at(i).setParticipated(false);
//		}
//	}
//
//	//�ݹ��ȡ�õ�������Ƶ������
//	if (m_nScore > m_nMaxScore)
//	{
//		m_nMaxScore = m_nScore;
//		setMark(EHC_Group);
//	}
//}
//
//
//void CLMjPlayer::markOneToOne()
//{
//	int pos1;
//	for (int i = 0; i < m_vecActiveHandCards.size(); i++)
//	if (!m_vecActiveHandCards[i].isParticipated())
//	{
//		m_vecActiveHandCards.at(i).setParticipated(true);
//		//�Ƿ��ж���
//		if (isExistPair(m_vecActiveHandCards[i], pos1))
//		{
//			int nScorePair = SCORE_Pair;
//			//���û�������ý�
//			if (m_bHavePair == false)
//			{
//				nScorePair = SCORE_Pair_Only;
//				m_bHavePair = true;
//			}
//			m_nScore += nScorePair;
//			m_vecActiveHandCards.at(pos1).setParticipated(true);
//			markOneToOne();
//			m_vecActiveHandCards.at(pos1).setParticipated(false);
//			if (nScorePair == SCORE_Pair_Only)
//			{
//				m_bHavePair = false;
//			}
//			m_nScore -= nScorePair;
//		}
//
//		//�Ƿ�������
//		E_DoorType doorType;
//		if (isExistDoor(m_vecActiveHandCards[i], doorType, pos1))
//		{
//			int nScoreDoor = 0;
//			switch (doorType)
//			{
//			case ED_DoorTwoHead:
//				nScoreDoor = SCORE_Door_TwoHead;
//				break;
//			case ED_DoorMiddle:
//				nScoreDoor = SCORE_Door_Middle;
//				break;
//			case ED_DoorEdge:
//				nScoreDoor = SCORE_Door_Edge;
//				break;
//			default:
//				break;
//			}
//			m_nScore += nScoreDoor;
//			m_vecActiveHandCards.at(pos1).setParticipated(true);
//			markOneToOne();
//			m_vecActiveHandCards.at(pos1).setParticipated(false);
//			m_nScore -= nScoreDoor;
//		}
//		m_vecActiveHandCards.at(i).setParticipated(false);
//	}
//	//int nMarkNum = getMarkNum();
//	if (m_nScore > m_nMaxScore)
//	{
//		//m_nMaxMarkNum = nMarkNum;
//		setMark(EHC_OneToOne);
//	}
//}
//
//
//void CLMjPlayer::thinkNoSingle()
//{
//	bool bHavePair = false;
//	bool bHaveTriplet = false;
//	vector<CHandCard> vecTempCard;
//	for (int i = 0; i < m_vecActiveHandCards.size(); ++i)
//	{
//		if (m_vecActiveHandCards[i].getRelationType() == EHC_OneToOne)
//		{
//			vecTempCard.push_back(m_vecActiveHandCards[i]);
//		}
//	}
//	//���ֻ��������
//	if (vecTempCard.size() == 2)
//	{
//		//�����ʣ����������ͬ�ͺ���
//		if (vecTempCard[0] == vecTempCard[1])
//		{
//			m_bIsHu = true;
//			return;
//		}
//		//��������Ʋ�ͬ�ʹ�һ�Խ�
//		for (int j = 0; m_vecActiveHandCards.size(); j++)
//		{
//			//���Ƿ��а���
//			if (m_vecActiveHandCards[j] == m_vecActiveHandCards[j + 1]
//				&& m_vecActiveHandCards[j] == m_vecActiveHandCards[j + 2])
//			{
//				bHaveTriplet = true;
//				//m_vecActiveHandCards[j].setScore(SCORE_Badly_Zero);
//				m_nBadlyCard = m_vecActiveHandCards[j].getCard();
//				break;
//			}
//		}
//		if (bHaveTriplet == false)
//		{
//			for (int i = 0; i < m_vecActiveHandCards.size(); ++i)
//			{
//				if (m_vecActiveHandCards[i] == vecTempCard[0] || m_vecActiveHandCards[i] == vecTempCard[1])
//				{
//					m_vecActiveHandCards[i].setRelationType(EHC_Single);
//				}
//			}
//		}
//	}
//	else
//	{
//		//����һ�飬�Ƚ��ĸ�������, �ѷ�������ÿһ��ĵ�һ������
//		for (int j = 0; j < vecTempCard.size() / 2; j++)
//		{
//			if (vecTempCard[j * 2] == vecTempCard[j * 2 + 1])
//			{
//				if (bHavePair)
//				{
//					vecTempCard[j].setScore(6);
//				}
//				else
//				{
//					vecTempCard[j].setScore(8);
//					bHavePair = true;
//				}
//			}
//			else if (vecTempCard[j * 2] == vecTempCard[j * 2 + 1] - 1)
//			{
//				if (vecTempCard[j * 2] % 9 == 0 || vecTempCard[j * 2 + 1] % 9 == 8)
//				{
//					vecTempCard[j].setScore(4);
//				}
//				else
//				{
//					vecTempCard[j].setScore(7);
//				}
//			}
//			else
//			{
//				if (vecTempCard[j * 2] % 9 == 0 || vecTempCard[j * 2 + 1] % 9 == 8)
//				{
//					vecTempCard[j].setScore(3);
//				}
//				else
//				{
//					vecTempCard[j].setScore(5);
//				}
//			}
//
//		}
//		int nMinScore = 10;
//		CHandCard handCardBadly;
//		//�ҳ�����һ���飬�����������ҳ�������
//		for (int k = 0; k < vecTempCard.size() / 2; k++)
//		{
//			if (vecTempCard[k].getScore() < nMinScore)
//			{
//				nMinScore = vecTempCard[k].getScore();
//				if (vecTempCard[k * 2] % 9 == 0)
//				{
//					handCardBadly = vecTempCard[k * 2];
//				}
//				else if (vecTempCard[k * 2 + 1] % 9 == 8)
//				{
//					handCardBadly = vecTempCard[k * 2 + 1];
//				}
//				else
//				{
//					handCardBadly = vecTempCard[k * 2 + rand() % 2];
//				}
//			}
//		}
//		m_nBadlyCard = handCardBadly.getCard();
//	}
//}
//
//
//void CLMjPlayer::markSingle()
//{
//	CHandCard badlyHandCrd;
//	badlyHandCrd.setScore(20);
//	for (int i = 0; i < m_vecActiveHandCards.size(); ++i)
//	{
//		if (m_vecActiveHandCards[i].getRelationType() == EHC_Single)
//		{
//			m_vecActiveHandCards[i].setParticipated(true);
//			addScoreBySelf(m_vecActiveHandCards[i]);
//			addScoreByRelation(m_vecActiveHandCards[i]);
//			m_vecActiveHandCards[i].setParticipated(false);
//			if (m_vecActiveHandCards[i].getScore() < badlyHandCrd.getScore())
//			{
//				badlyHandCrd = m_vecActiveHandCards[i];
//			}
//		}
//	}
//	m_nBadlyCard = badlyHandCrd.getCard();
//}
//
//int CLMjPlayer::getMarkNum()
//{
//	int nNum = 0;
//	for (int i = 0; i < m_vecActiveHandCards.size(); ++i)
//	{
//		if (m_vecActiveHandCards.at(i).isParticipated())
//		{
//			++nNum;
//		}
//	}
//	return nNum;
//}
//
//void CLMjPlayer::addScoreBySelf(CHandCard & handCard)
//{
//	�����һ���߾�
//	if (handCard % 9 == 0 || handCard % 9 == 8)
//	{
//		handCard.addScore(SCORE_Single_Edge);
//	}
//	else
//	{
//		handCard.addScore(SCORE_Single_Middle);
//	}
//}
//
//void CLMjPlayer::addScoreByRelation(CHandCard & handCard)
//{
//	//���ʣ��������б�����һ����
//	if (handCard % 9 != 0 && findPosInActiveHandCards(handCard - 1) != -1)
//	{
//		if (handCard % 9 != 1)
//		{
//			handCard.addScore(SCORE_Single_R_TwoHead);
//		}
//		else
//		{
//			handCard.addScore(SCORE_Single_R_Edge);
//		}
//	}
//	//���ʣ��������б����һ������
//	if (handCard % 9 != 8 && findPosInActiveHandCards(handCard + 1) != -1)
//	{
//		if (handCard % 9 != 7)
//		{
//			handCard.addScore(SCORE_Single_R_TwoHead);
//		}
//		else
//		{
//			handCard.addScore(SCORE_Single_R_Edge);
//		}
//	}
//	//���ʣ��������б����ٶ����ƣ���3��5,1_3�ȣ�
//	if (handCard % 9 > 1 && findPosInActiveHandCards(handCard - 2) != -1)
//	{
//		handCard.addScore(SCORE_Single_R_Middle);
//	}
//
//	//���ʣ��������б���������
//	if (handCard % 9 < 7 && findPosInActiveHandCards(handCard + 2) != -1)
//	{
//		handCard.addScore(SCORE_Single_R_Middle);
//	}
//
//}
//
//void CLMjPlayer::setMark(E_HandCardRelationType relationType)
//{
//	for (int i = 0; i < m_vecActiveHandCards.size(); ++i)
//	{
//		//��ϵȼ��͵Ĳ��ܸı���ϵȼ��ߵ�
//		if (relationType > m_vecActiveHandCards[i].getRelationType())
//		{
//			continue;
//		}
//		m_vecActiveHandCards.at(i).setRelationType(EHC_Single);
//		if (m_vecActiveHandCards.at(i).isParticipated())
//		{
//			m_vecActiveHandCards.at(i).setRelationType(relationType);
//		}
//	}
//}
//
//void CPlayer::setScore(E_HandCardRelationType relationType)
//{
//	int nScore;
//	switch (relationType)
//	{
//	case EHC_Group:
//		nScore = 300;
//		break;
//	case EHC_OneToOne:
//		nScore = 100;
//		break;
//	case EHC_Single:
//		nScore = 10;
//		break;
//	default:
//		break;
//	}
//	for (int i = 0; i < m_vecActiveHandCards.size(); ++i)
//	{
//		if (m_vecActiveHandCards.at(i).isParticipated())
//		{
//			m_vecActiveHandCards.at(i).setScore(nScore);
//		}
//	}
//}
//
//
//
//
//void CLMjPlayer::thinkSevenPair()
//{
//	int i, j;
//	int nPairs = 0;
//	for (i = 0; i < 12; ++i)
//	{
//		if (m_arrHandCard[i].isParticipated())
//		{
//			continue;
//		}
//		for (j = i + 1; j < 13; ++j)
//		{
//			if (!m_arrHandCard[j].isParticipated() && (m_arrHandCard[i] == m_arrHandCard[j]))
//			{
//				m_arrHandCard[i].setParticipated(true);
//				m_arrHandCard[j].setParticipated(true);
//				++nPairs;
//				break;
//			}
//		}
//	}
//	if (nPairs == 6)
//	{
//		for (i = 0; i < 13; ++i)
//		{
//			if (!m_arrHandCard[i].isParticipated())
//			{
//				m_vecTing.push_back(m_arrHandCard[i].getCard());
//			}
//		}
//	}
//	resetParticipated();
//}
//
//void CLMjPlayer::resetParticipated()
//{
//	for (int i = 0; i < 13; ++i)
//	{
//		m_arrHandCard[i].setParticipated(false);
//	}
//}
//
//
//int CLMjPlayer::findPosInActiveHandCards(CHandCard handCard)
//{
//	for (int i = 0; i < m_vecActiveHandCards.size(); ++i)
//	{
//		if (!m_vecActiveHandCards[i].isParticipated() && m_vecActiveHandCards[i] == handCard)
//		{
//			return i;
//		}
//	}
//	return -1;
//}
//
//
//void CLMjPlayer::resetParticipatedInActive()
//{
//	for (int i = 0; i < m_vecActiveHandCards.size(); ++i)
//	{
//		m_vecActiveHandCards[i].setParticipated(false);
//	}
//}



////���ø�
//void setKong(int nPai);
////�������������д���λ��
//int findCardPos(int nCard, int nStart = 0, int nEnd = 13);
//void resetParticipated();