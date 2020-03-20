
#include "LMjLogic.h"
#include "math.h"
#include <assert.h>


CLMjLogic::CLMjLogic()
{

}


bool CLMjLogic::isCanPong(CLMjCard aCards[], unsigned int unCardCount, const CLMjCard & cardDest)
{
	return getCardsNum(aCards, unCardCount, cardDest) >= 2;
}

bool CLMjLogic::isCanKong(CLMjCard aCards[], unsigned int unCardCount, T_WeaveCardsItem aWeaveItem[], unsigned int unItemSize,
	CLMjCard cardDest)
{
	if (getCardsNum(aCards, unCardCount, cardDest) == 3)
	{
		return true;
	}
	return false;
	//�Ҳ���
	//for (int i = 0; i < iCGPNums; ++i)
	//{
	//	if (lstCGP[i].nType == emType::Peng) //������
	//	{
	//		//if (byFetchCard == lstCGP[i].val[0])//תת�齫©�ܲ��ܸ�
	//		if (GetCardNum(byCards, iCardNums, lstCGP[i].val[0])>0)
	//		{
	//			//��Ӹ�ѡ���
	//			pActInfo->byGangSels[pActInfo->iGangNums] = lstCGP[i].val[0];
	//			pActInfo->nGType[pActInfo->iGangNums] = emGangType::gt_BuGang;
	//			pActInfo->nBuGangInPeng[pActInfo->iGangNums] = i;
	//			pActInfo->iGangNums++;
	//		}
	//	}
	//}
}




// Ҫ��֤�����Ѿ���С��������
// �Ƴ�����
bool CLMjLogic::removeTriplet(CLMjCard aCards[], unsigned int unCardCount, int nBeginPos /*= 0*/)
{
	CLMjCard cardBegin = aCards[nBeginPos];
	int nMagicNum = getMagicNum(aCards, unCardCount);
	int nMagicUsed = 0;

	// ���������
	if (cardBegin.isMagicCard())
	{
		return false;
	}

	// �Ƴ���ʼ��
	if (!removeCard(aCards, unCardCount--, cardBegin))
	{
		return false;
	}

	// �Ƴ�ʧ��ʱû�и���ʱ����Ӳ�
	if (!removeCard(aCards, unCardCount--, cardBegin))
	{
		++nMagicUsed;
		// �����Ӷ�����������ֱ�ӷ���
		if (nMagicUsed > nMagicNum)
		{
			return false;
		}
	}

	if (!removeCard(aCards, unCardCount, cardBegin))
	{
		++nMagicUsed;
		if (nMagicUsed > nMagicNum)
		{
			return false;
		}
	}
		return true;
}


bool CLMjLogic::removeSequence(CLMjCard aCards[], unsigned int unCardCount, int nBeginPos /*= 0*/)
{
	CLMjCard cardBegin = aCards[nBeginPos];
	int nMagicNum = getMagicNum(aCards, unCardCount);
	int nMagicUsed = 0;

	if (cardBegin.isMagicCard())
	{
		return false;
	}

	if (!removeCard(aCards, unCardCount--, cardBegin))
	{
		return false;
	}

	/* ��ͷ�� �г� β��*/

	// û�и���ʱ����ͬһ��ɫ ����Ӳ�
	if (!removeCard(aCards, unCardCount--, cardBegin + 1) || ((cardBegin + 1) / 10 != cardBegin / 10))
	{
		++nMagicUsed;
		// �����Ӷ�����������ֱ�ӷ���
		if (nMagicUsed > nMagicNum)
		{
			return false;
		}
	}

	if (!removeCard(aCards, unCardCount--, cardBegin + 2) || ((cardBegin + 2) / 10 != cardBegin / 10))
	{
		++nMagicUsed;
		// �����Ӷ�����������ֱ�ӷ���
		if (nMagicUsed > nMagicNum)
		{
			return false;
		}
	}
	return true;
}

int CLMjLogic::getMagicNum(CLMjCard aCards[], unsigned int unCardCount)
{
	int num = 0;
	for (int i = 0; i < unCardCount; ++i)
	{
		if (aCards[i].isMagicCard())
		{
			++num;
		}
	}
	return num;
}


// 
bool CLMjLogic::isCanHu(CLMjCard aCards[]
	, unsigned int unCardCount
	, T_WeaveCardsItem aWeaveItem[]
	, unsigned int unItemSize
	, const CLMjCard & cardOtherOut/* = CARD_EMPTY*/
	, const CLMjCard & cardGet/* = CARD_EMPTY*/
	, bool bMust258Pair /*= false*/)
{
	if (cardOtherOut < 0)
	{
		return false;
	}

	// ��������
	unsigned int unCardNumTemp = unCardCount;
	CLMjCard aCardsTemp[MAX_HAND_COUNT];
	copyCards(aCardsTemp, unCardNumTemp, aCards, unCardCount);

	// ����
	sortCards(aCardsTemp, unCardNumTemp);

	bool bCanHu3x2 = isCanHu_3x2(aCardsTemp, unCardNumTemp, bMust258Pair);
	return bCanHu3x2;
}

// �ݹ�ɾ����
bool CLMjLogic::isCanHu_3x2(CLMjCard aCards[], unsigned int unCardCount, bool bMust258Pair /*= false*/)
{
	assert(unCardCount > MAX_HAND_COUNT || unCardCount <= 0);
	if (unCardCount % 3 != 2)
	{
		return false;
	}

	// ʣ��һ�Խ��Ϳ��Ժ���
	if (unCardCount == 2)
	{
		if (getMagicNum(aCards, unCardCount) > 1)
		{
			return true;
		}
		if (aCards[0] == aCards[1])
		{
			return true;
		}
		return false;
	}

	// ���¿���һ�ݣ��Դ˵ݹ�ʧ��ʱ�ָ���ʼ��
	CLMjCard aCardsTemp[MAX_HAND_COUNT];
	copyCards(aCardsTemp, unCardCount, aCards, unCardCount);

	// �޳�����
	if (removeTriplet(aCardsTemp, unCardCount))
	{
		isCanHu_3x2(aCardsTemp, unCardCount - 3); // �޳��ɹ���ʹ��aCardsTemp����
	}

	// �޳�ʧ�������¿���
	copyCards(aCardsTemp, unCardCount, aCards, unCardCount);

	// �޳�˳��
	if (removeSequence(aCardsTemp, unCardCount))
	{
		isCanHu_3x2(aCardsTemp, unCardCount - 3);
	}

	return false;
}

bool CLMjLogic::isCanHu_7Pair(CLMjCard aCards[], unsigned int unCardCount, bool bMust258Pair /*= false*/)
{
	assert(unCardCount > MAX_HAND_COUNT || unCardCount <= 0);
	if (unCardCount % 3 != 2)
	{
		return false;
	}
		//int i, j;
		//int nPairs = 0;
		//for (i = 0; i < 12; ++i)
		//{
		//	if (m_arrHandCard[i].isParticipated())
		//	{
		//		continue;
		//	}
		//	for (j = i + 1; j < 13; ++j)
		//	{
		//		if (!m_arrHandCard[j].isParticipated() && (m_arrHandCard[i] == m_arrHandCard[j]))
		//		{
		//			m_arrHandCard[i].setParticipated(true);
		//			m_arrHandCard[j].setParticipated(true);
		//			++nPairs;
		//			break;
		//		}
		//	}
		//}
		//if (nPairs == 6)
		//{
		//	for (i = 0; i < 13; ++i)
		//	{
		//		if (!m_arrHandCard[i].isParticipated())
		//		{
		//			m_vecTing.push_back(m_arrHandCard[i].getCard());
		//		}
		//	}
		//}
		//resetParticipated();
	return true;
}



