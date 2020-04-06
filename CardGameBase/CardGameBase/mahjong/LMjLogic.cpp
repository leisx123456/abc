
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
		//assert(aCards[i].isValid());
		if (!aCards[i].isValid())
		{
			std::cout << "errer";
		}
		arrCardIndex[aCards[i].switchToCardIndex()]++;
	}

	return unCardCount;
}



bool CLMjLogic::isCanPong(CLMjCard aCards[], unsigned int unCardCount, const CLMjCard & cardOut)
{
	return getCardsNum(aCards, unCardCount, cardOut) >= 2;
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


//bool CLMjLogic::isCanDianKong(CLMjCard aCards[], unsigned int unCardCount, T_WeaveCardsItem aWeaveItem[], unsigned int unItemSize, CLMjCard cardOut)
//{
//	if (getCardsNum(aCards, unCardCount, cardOut) == 3)
//	{
//		return true;
//	}
//	return false;
//}
//
//
//bool CLMjLogic::isCanAnKong(CLMjCard aCards[], unsigned int unCardCount, CLMjCard & cardResult)
//{
//	unsigned int arrCardIndexTemp[MJ_MAX_INDEX];
//	switchToCardIndex(aCards, unCardCount, arrCardIndexTemp);
//
//	for (int i = 0; i < MJ_MAX_INDEX; i++)
//	{
//		if (arrCardIndexTemp[i] == 4)
//		{
//			cardResult = CLMjCard::switchToCardValue(i);
//			return true;
//		}
//	}
//
//	return false;
//}
//
//bool CLMjLogic::isCanBuKong(CLMjCard aCards[], unsigned int unCardCount, T_WeaveCardsItem aWeaveItem[], unsigned int unItemSize, CLMjCard & cardResult)
//{
//	//�Ҳ���
//	//for (int i = 0; i < unItemSize; ++i)
//	//{
//	//	if (aWeaveItem[i].byWeaveKind == T_WeaveCardsItem::EW_Triplet)
//	//	{
//	//		if (aWeaveItem[i].cardCenter == cardOut)
//	//		{
//	//			return true;
//	//		}
//	//	}
//	//}
//	unsigned int arrCardIndexTemp[MJ_MAX_INDEX];
//	switchToCardIndex(aCards, unCardCount, arrCardIndexTemp);
//
//	for (int i = 0; i < unItemSize; i++)
//	{
//		if (aWeaveItem[i].byWeaveKind == T_WeaveCardsItem::EW_Triplet)
//		{
//			if (arrCardIndexTemp[aWeaveItem[i].cardCenter.switchToCardIndex()] == 1)
//			{
//				cardResult = CLMjCard::switchToCardValue(i);
//				return true;
//			}
//		}
//	}
//
//	return false;
//}






// Ҫ��֤�����Ѿ���С��������
// �Ƴ����ӣ��������
//bool CLMjLogic::removeTriplet(CLMjCard aCards[], unsigned int unCardCount, int nBeginPos /*= 0*/)
//{
//	CLMjCard cardBegin = aCards[nBeginPos];
//	int nMagicNum = getMagicNum(aCards, unCardCount);
//	int nMagicUsed = 0;
//
//	// ���������
//	if (cardBegin.isMagicCard())
//	{
//		return false;
//	}
//
//	// �Ƴ���ʼ��
//	if (!removeCard(aCards, unCardCount--, cardBegin))
//	{
//		return false;
//	}
//
//	// �Ƴ�ʧ��ʱû�и���ʱ����Ӳ�
//	if (!removeCard(aCards, unCardCount--, cardBegin))
//	{
//		++nMagicUsed;
//		// �����Ӷ�����������ֱ�ӷ���
//		if (nMagicUsed > nMagicNum)
//		{
//			return false;
//		}
//	}
//
//	if (!removeCard(aCards, unCardCount, cardBegin))
//	{
//		++nMagicUsed;
//		if (nMagicUsed > nMagicNum)
//		{
//			return false;
//		}
//	}
//		return true;
//}

bool CLMjLogic::removeTriplet(CLMjCard aCards[], unsigned int unCardCount, int nBeginPos /*= 0*/)
{
	CLMjCard cardBegin = aCards[nBeginPos];
	if (!removeCards(aCards, unCardCount, cardBegin, 3))
	{
			return false;
	}
	return true;
}

//bool CLMjLogic::removeSequence(CLMjCard aCards[], unsigned int unCardCount, int nBeginPos /*= 0*/)
//{
//	CLMjCard cardBegin = aCards[nBeginPos];
//	int nMagicNum = getMagicNum(aCards, unCardCount);
//	int nMagicUsed = 0;
//
//	if (cardBegin.isMagicCard())
//	{
//		return false;
//	}
//
//	if (!removeCard(aCards, unCardCount--, cardBegin))
//	{
//		return false;
//	}
//
//	/* ��ͷ�� �г� β��*/
//	// û�и���ʱ����ͬһ��ɫ ����Ӳ�
//	if (!removeCard(aCards, unCardCount--, cardBegin + 1) || ((cardBegin + 1) / 10 != cardBegin / 10))
//	{
//		++nMagicUsed;
//		// �����Ӷ�����������ֱ�ӷ���
//		if (nMagicUsed > nMagicNum)
//		{
//			return false;
//		}
//	}
//
//	if (!removeCard(aCards, unCardCount--, cardBegin + 2) || ((cardBegin + 2) / 10 != cardBegin / 10))
//	{
//		++nMagicUsed;
//		// �����Ӷ�����������ֱ�ӷ���
//		if (nMagicUsed > nMagicNum)
//		{
//			return false;
//		}
//	}
//	return true;
//}

// Ϊ�˱�֤ԭ���ԣ��ȼ���Ƿ���˳����ɾ��
bool CLMjLogic::removeSequence(CLMjCard aCards[], unsigned int unCardCount, int nBeginPos /*= 0*/)
{
	CLMjCard cardBegin = aCards[nBeginPos];
	CLMjCard card2 = cardBegin + 1;
	CLMjCard card3 = cardBegin + 2;
	if (!card2.isValid() || card2.color() != cardBegin.color())
	{
		return false;
	}
	if (!card3.isValid() || card3.color() != cardBegin.color())
	{
		return false;
	}
	if (!isCardInArray(aCards, unCardCount, card2) || !isCardInArray(aCards, unCardCount, card3))
	{
		return false;
	}

	removeCard(aCards, unCardCount--, cardBegin);
	removeCard(aCards, unCardCount--, card2);
	removeCard(aCards, unCardCount--, card3);
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
		if (aCards[0] == aCards[1])
		{
			return true;
		}
		return false;
	}

	// ���¿���һ�ݣ��Դ˵ݹ�ʧ��ʱ�ָ���ʼ��
	CLMjCard aCardsTemp[MJ_MAX_HAND_COUNT];
	copyCards(aCardsTemp, unCardCount, aCards, unCardCount);

	// ������Ҫѭ������
	for (int i = 0; i < unCardCount; ++i)
	{
		// �޳�����
		if (removeTriplet(aCardsTemp, unCardCount, i))
		{
			if (isCanHu_3x2(aCardsTemp, unCardCount - 3))
			{
				return true; // ע����뷵��ֵ����㷵��true
			}
		}

		// �޳�ʧ�������¿���
		copyCards(aCardsTemp, unCardCount, aCards, unCardCount);

		// �޳�˳��
		if (removeSequence(aCardsTemp, unCardCount, i))
		{
			if (isCanHu_3x2(aCardsTemp, unCardCount - 3))
			{
				return true;
			}
		}
		copyCards(aCardsTemp, unCardCount, aCards, unCardCount);
	}


	return false;
}

bool CLMjLogic::isCanHu_7Pair(CLMjCard aCards[], unsigned int unCardCount, bool bMust258Pair /*= false*/)
{
	if (unCardCount != 14)
	{
		return false;
	}
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

bool CLMjLogic::isPongPong(CLMjCard aCards[], unsigned int unCardCount, T_WeaveCardsItem aWeaveItem[], unsigned int unItemSize)
{

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


bool CLMjLogic::isDragonSevenPair(CLMjCard aCards[], unsigned int unCardCount, bool bMust258Pair /*= false*/)
{
	if (!isCanHu_7Pair(aCards, unCardCount))
	{
		return false;
	}
	//��ʱ����
	unsigned int arrCardIndexTemp[MJ_MAX_INDEX];
	switchToCardIndex(aCards, unCardCount, arrCardIndexTemp);

	//���㵥��
	for (int i = 0; i < MJ_MAX_INDEX; i++)
	{
		if (arrCardIndexTemp[i] == 4)
		{
			return true;
		}
	}

	return false;
}


int CLMjLogic::genNum(CLMjCard aCards[], unsigned int unCardCount, T_WeaveCardsItem aWeaveItem[], unsigned int unItemSize)
{
	unsigned int arrCardIndexTemp[MJ_MAX_INDEX];
	switchToCardIndex(aCards, unCardCount, arrCardIndexTemp);
	int gen = 0;

	//���㵥��
	for (int i = 0; i < MJ_MAX_INDEX; i++)
	{
		if (arrCardIndexTemp[i] == 4)
		{
			gen++;
		}
	}

	for (int i = 0; i < unItemSize; i++)
	{
		if (aWeaveItem[i].isKong())
		{
			gen++;
		}
	}
	return gen;
}


bool CLMjLogic::isCanHu(CLMjCard aCards[], unsigned int unCardCount, T_WeaveCardsItem aWeaveItem[]
	, unsigned int unItemSize, T_MjActHuInfo & tMjActHuInfo, const CLMjCard & cardOut /*= CARD_EMPTY */, bool bMust258Pair /*= false*/)
{
	// ��������
	unsigned int unCardNumTemp = unCardCount;
	CLMjCard aCardsTemp[MJ_MAX_HAND_COUNT];
	copyCards(aCardsTemp, unCardNumTemp, aCards, unCardCount);

	// ���������
	aCardsTemp[unCardNumTemp++] = cardOut;

	// ����
	sortCards(aCardsTemp, unCardNumTemp);

	bool bCanHu3x2 = isCanHu_3x2(aCardsTemp, unCardNumTemp, bMust258Pair);
	if (bCanHu3x2)
	{
		tMjActHuInfo.eMjHuWay = cardOut.isValid() ? EHW_JiePao : EHW_ZiMo;
		tMjActHuInfo.eMjHuName = EHN_Ping;
		//tMjActHuInfo.nHuIndex = 0;
		//tMjActHuInfo.nHuNameNums = 1;
	}
	return bCanHu3x2;
}





