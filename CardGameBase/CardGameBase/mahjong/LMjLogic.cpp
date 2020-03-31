
#include "LMjLogic.h"
#include "math.h"
#include <assert.h>


CLMjLogic::CLMjLogic()
{

}





int CLMjLogic::switchToCardData(const unsigned int arrCardIndex[MJ_MAX_INDEX], CLMjCard aCards[MJ_MAX_HAND_COUNT])
{
	int cbPosition = 0;

	for (int index = 0; index < MJ_MAX_INDEX; index++)
	{
		if (aCards[index] != 0)	// aCards[index] ���λ4��
		{
			for (int j = 0; j < arrCardIndex[index]; j++)
			{
				assert(cbPosition < MJ_MAX_HAND_COUNT);
				aCards[cbPosition++] = CLMjCard::switchToCardValue(index);
			}
		}
	}

	return cbPosition;
}


int CLMjLogic::switchToCardIndex(CLMjCard aCards[], unsigned int unCardCount, unsigned int arrCardIndex[MJ_MAX_INDEX])
{
	memset(arrCardIndex, 0, sizeof(unsigned int) * MJ_MAX_INDEX);

	for (int i = 0; i < unCardCount; i++)
	{
		assert(aCards[i].isValid());
		arrCardIndex[aCards[i].switchToCardIndex()]++;
	}

	return unCardCount;
}



bool CLMjLogic::isCanPong(CLMjCard aCards[], unsigned int unCardCount, const CLMjCard & cardDest)
{
	return getCardsNum(aCards, unCardCount, cardDest) >= 2;
}


bool CLMjLogic::isCanKong(CLMjCard aCards[], unsigned int unCardCount, T_WeaveCardsItem aWeaveItem[]
	, unsigned int unItemSize, CLMjCard cardOut, T_MjActKongInfo & tMjActKongInfo)
{
	if (cardOut.isValid())
	{
		if (getCardsNum(aCards, unCardCount, cardOut) == 3)
		{
			tMjActKongInfo.arrKongSelect[tMjActKongInfo.nKongSelectNums] = cardOut;
			tMjActKongInfo.arrKongType[tMjActKongInfo.nKongSelectNums] = EK_KongDian;
			tMjActKongInfo.nKongSelectNums++;
		}
	}
	else
	{
		unsigned int arrCardIndexTemp[MJ_MAX_INDEX];
		switchToCardIndex(aCards, unCardCount, arrCardIndexTemp);
		for (int i = 0; i < MJ_MAX_INDEX; i++)
		{
			if (arrCardIndexTemp[i] == 4)
			{
				tMjActKongInfo.arrKongSelect[tMjActKongInfo.nKongSelectNums] = CLMjCard::switchToCardValue(i).value();
				tMjActKongInfo.arrKongType[tMjActKongInfo.nKongSelectNums] = EK_KongAn;
				tMjActKongInfo.nKongSelectNums++;
			}
		}
		for (int i = 0; i < unItemSize; i++)
		{
			if (aWeaveItem[i].byWeaveKind == T_WeaveCardsItem::EW_Triplet)
			{
				if (arrCardIndexTemp[aWeaveItem[i].cardCenter.switchToCardIndex()] == 1)
				{
					tMjActKongInfo.arrKongSelect[tMjActKongInfo.nKongSelectNums] = CLMjCard::switchToCardValue(i).value();
					tMjActKongInfo.arrKongType[tMjActKongInfo.nKongSelectNums] = EK_KongBa;
					tMjActKongInfo.nKongSelectNums++;
				}
			}
		}
	}
	return tMjActKongInfo.nKongSelectNums > 0;
	
}


bool CLMjLogic::isCanDianKong(CLMjCard aCards[], unsigned int unCardCount, T_WeaveCardsItem aWeaveItem[], unsigned int unItemSize, CLMjCard cardDest)
{
	if (getCardsNum(aCards, unCardCount, cardDest) == 3)
	{
		return true;
	}
	return false;
}


bool CLMjLogic::isCanAnKong(CLMjCard aCards[], unsigned int unCardCount, CLMjCard & cardResult)
{
	unsigned int arrCardIndexTemp[MJ_MAX_INDEX];
	switchToCardIndex(aCards, unCardCount, arrCardIndexTemp);

	for (int i = 0; i < MJ_MAX_INDEX; i++)
	{
		if (arrCardIndexTemp[i] == 4)
		{
			cardResult = CLMjCard::switchToCardValue(i);
			return true;
		}
	}

	return false;
}

bool CLMjLogic::isCanBuKong(CLMjCard aCards[], unsigned int unCardCount, T_WeaveCardsItem aWeaveItem[], unsigned int unItemSize, CLMjCard & cardResult)
{
	//�Ҳ���
	//for (int i = 0; i < unItemSize; ++i)
	//{
	//	if (aWeaveItem[i].byWeaveKind == T_WeaveCardsItem::EW_Triplet)
	//	{
	//		if (aWeaveItem[i].cardCenter == cardDest)
	//		{
	//			return true;
	//		}
	//	}
	//}
	unsigned int arrCardIndexTemp[MJ_MAX_INDEX];
	switchToCardIndex(aCards, unCardCount, arrCardIndexTemp);

	for (int i = 0; i < unItemSize; i++)
	{
		if (aWeaveItem[i].byWeaveKind == T_WeaveCardsItem::EW_Triplet)
		{
			if (arrCardIndexTemp[aWeaveItem[i].cardCenter.switchToCardIndex()] == 1)
			{
				cardResult = CLMjCard::switchToCardValue(i);
				return true;
			}
		}
	}

	return false;
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


int CLMjLogic::getColorCount(CLMjCard aCards[], unsigned int unCardCount, T_WeaveCardsItem aWeaveItem[], unsigned int unItemSize)
{
	const int c_nColorCount = CLMjCard::EM_Max_Color;	// ��ɫ�������
	unsigned int arrColorIndex[c_nColorCount];			// ���廨����������
	int nColorBiggerZero = 0;							// ������������0�Ļ�ɫ�ж�����
	memset(arrColorIndex, 0, sizeof(unsigned int)* c_nColorCount);

	// ����
	for (int i = 0; i < unCardCount; ++i)
	{
		arrColorIndex[aCards[i].color()]++;
	}

	// �����
	for (int i = 0; i < unItemSize; ++i)
	{
		arrColorIndex[aWeaveItem[i].cardCenter.color()]++;
	}

	// 
	for (int i = 0; i < c_nColorCount; ++i)
	{
		if (arrColorIndex[i] > 0)
		{
			nColorBiggerZero++;
		}
	}

	return nColorBiggerZero;
}




bool CLMjLogic::isFlowerPig(CLMjCard aCards[], unsigned int unCardCount, T_WeaveCardsItem aWeaveItem[], unsigned int unItemSize, int colorQue)
{
	// ע:����ͨ�������ƵĻ�ɫ�������2���ǻ���,��Ϊ�п���2����ɫ����һ������ȱ
	// ֱ���ж��Ƿ��ж�ȱ��
	if (colorQue < 0 || colorQue > 2)
	{
		return false;
	}
	
	// ����
	for (int i = 0; i < unCardCount; ++i)
	{
		if (aCards[i].color() == colorQue)
		{
			return true;
		}
	}

	// �����
	for (int i = 0; i < unItemSize; ++i)
	{
		if (aWeaveItem[i].cardCenter.color() == colorQue)
		{
			return true;
		}
	}

	return false;
}



// 
bool CLMjLogic::isCanHu(CLMjCard aCards[]
	, unsigned int unCardCount
	, T_WeaveCardsItem aWeaveItem[]
	, unsigned int unItemSize
	, const CLMjCard & cardDest/* = CARD_EMPTY*/
	, bool bMust258Pair /*= false*/)
{
	if (cardDest < 0)
	{
		return false;
	}

	// ��������
	unsigned int unCardNumTemp = unCardCount;
	CLMjCard aCardsTemp[MJ_MAX_HAND_COUNT];
	copyCards(aCardsTemp, unCardNumTemp, aCards, unCardCount);

	// ����
	sortCards(aCardsTemp, unCardNumTemp);

	bool bCanHu3x2 = isCanHu_3x2(aCardsTemp, unCardNumTemp, bMust258Pair);
	return bCanHu3x2;
}

// �ݹ�ɾ����
bool CLMjLogic::isCanHu_3x2(CLMjCard aCards[], unsigned int unCardCount, bool bMust258Pair /*= false*/)
{
	assert(unCardCount > 0 && unCardCount <= MJ_MAX_HAND_COUNT);
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
	CLMjCard aCardsTemp[MJ_MAX_HAND_COUNT];
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
	assert(unCardCount > MJ_MAX_HAND_COUNT || unCardCount <= 0);
	if (unCardCount % 3 != 2)
	{
		return false;
	}

	//������Ŀ
	int cbReplaceCount = 0;

	//��ʱ����
	unsigned int arrCardIndexTemp[MJ_MAX_INDEX];
	switchToCardIndex(aCards, unCardCount, arrCardIndexTemp);

	//���㵥��
	for (int i = 0; i < MJ_MAX_INDEX; i++)
	{
		int cbCardCount = arrCardIndexTemp[i];

		//����ͳ��
		if (cbCardCount == 1 || cbCardCount == 3)
		{
			cbReplaceCount++;
		}
	}

	if (cbReplaceCount > 0)
	{
		return false;
	}
		
	return true;
}

bool CLMjLogic::isPengPeng(CLMjCard aCards[], unsigned int unCardCount, T_WeaveCardsItem aWeaveItem[], unsigned int unItemSize)
{
	if (unCardCount != 2 || unItemSize != 4)
	{
		return false;
	}
	
	for (int i = 0; i < unItemSize; i++)
	{
		if (aWeaveItem[i].byWeaveKind  & (EA_EatLeft | EA_EatCenter | EA_EatRight))
			return false;
	}
	return true;
}

// �ҵ�˼·��ֻ��һ�ֻ�ɫ����������0��������ɫ������Ϊ0��������һɫ 20200321 leisx
bool CLMjLogic::isQingYiSe(CLMjCard aCards[], unsigned int unCardCount, T_WeaveCardsItem aWeaveItem[], unsigned int unItemSize)
{
	return getColorCount(aCards, unCardCount, aWeaveItem, unItemSize) == 1;
}





