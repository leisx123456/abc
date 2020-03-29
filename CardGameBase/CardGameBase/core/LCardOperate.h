#pragma once
#include "LCard.h"
#include <iostream>
#include <vector>
#include <algorithm>


#define INVALID_CARD		0
/************************************************************************/
/* 
CLCardOperate �Ʋ��������࣬ ���Զ��������ƹ������, �������õ��������stl��������Ҫ�Լ�ʵ��һЩ����
*/
/************************************************************************/

// ����Ϊ��ģ�壬��Ϊ����֪����ȷ�е���һ���Ʋ���
template<class CLCard>
class CLCardOperate
{
public:
	CLCardOperate();
	virtual ~CLCardOperate();

	// ϴ��
	void shuffleCards(CLCard aCards[], unsigned int unCardCount);

	// ����
	void sortCards(CLCard aCards[], unsigned int unCardCount);

	// ����-(���ڶ�����memcpy,��Ҫ�Լ�����)
	void copyCards(CLCard aCardsDest[], unsigned int unDestCount, CLCard aCardsSrc[], unsigned int unSrcCount);
	void copyCards(CLCard aCardsDest[], unsigned int unDestCount, int aCardsValueSrc[], unsigned int unSrcCount);
	void copyCards(int aCardsValueDest[], unsigned int unDestCount, CLCard aCardsSrc[], unsigned int unSrcCount);
	void emptyCards(CLCard aCards[], unsigned int unCardCount);

	// ɾ������
	// ������ֵ��ɾ��
	//bool RemoveCard(byte cbCardIndex[MAX_INDEX], byte cbRemoveCard);
	//bool RemoveCard(byte cbCardIndex[MAX_INDEX], const byte cbRemoveCard[], byte cbRemoveCount);

	// ��ͨɾ��
	bool removeCard(CLCard aCards[], unsigned int unCardCount, CLCard cardRemove);
	bool removeCards(CLCard aCards[], unsigned int unCardCount, const CLCard aRemoveCards[], unsigned int unRemoveCount);
	unsigned char removeCards(CLCard aCards[], unsigned int unCardCount, CLCard cardRemove, unsigned int unRemoveCount = 1);

	// ����
	// �����ҵ����Ƶ������±�
	int findCardIndex(CLCard aCards[], unsigned int unCardCount, CLCard card);

	// ���������У���ȡĳһ��ֵ�þ������
	int getCardsNum(CLCard aCards[], unsigned int unCardCount, CLCard card);

	// other
	bool isCardInArray(CLCard const aCards[], unsigned int unCardCount, CLCard card);
	bool isCardsInArray(CLCard const aCards[], unsigned int unCardCount, CLCard const aSubsetCards[], unsigned int unSubsetCardCount);

};



/* ����*/
template<class CLCard>
CLCardOperate<CLCard>::CLCardOperate()
{
}

template<class CLCard>
CLCardOperate<CLCard>::~CLCardOperate()
{
}

template<class CLCard>
void CLCardOperate<CLCard>::shuffleCards(CLCard aCards[], unsigned int unCardCount)
{
	//std::random_shuffle(aCards[0], aCards[unCardCount]);

	 //����2
	//����׼��
	CLCard *cbCardDataTemp = new CLCard[unCardCount];
	for (int i = 0; i < unCardCount; ++i)
	{
		cbCardDataTemp[i] = aCards[i];
	}

	//�����˿�
	int cbRandCount = 0;
	int cbPosition = 0;
	do
	{
		cbPosition = rand() % (unCardCount - cbRandCount);
		aCards[cbRandCount++] = cbCardDataTemp[cbPosition];
		cbCardDataTemp[cbPosition] = cbCardDataTemp[unCardCount - cbRandCount];
	} while (cbRandCount < unCardCount);

	delete []cbCardDataTemp;
}

template<class CLCard>
void CLCardOperate<CLCard>::sortCards(CLCard aCards[], unsigned int unCardCount)
{
	//std::sort(aCards[0], aCards[unCardCount]);

	// ����2
	// ��С����ѡ������
	CLCard cardTemp;
	for (int i = 0; i < unCardCount - 1; ++i)
	{
		for (int j = i + 1; j < unCardCount; ++j)
		{
			if (aCards[i] > aCards[j])
			{
				cardTemp = aCards[i];
				aCards[i] = aCards[j];
				aCards[j] = cardTemp;
			}
		}
	}
}


template<class CLCard>
void CLCardOperate<CLCard>::copyCards(CLCard aCardsDest[], unsigned int unDestCount, CLCard aCardsSrc[], unsigned int unSrcCount)
{
	unsigned int unCopyCount = unDestCount > unSrcCount ? unSrcCount : unDestCount;
	for (unsigned int i = 0; i < unCopyCount; ++i)
	{
		aCardsDest[i] = aCardsSrc[i];
	}
}


template<class CLCard>
void CLCardOperate<CLCard>::copyCards(CLCard aCardsDest[], unsigned int unDestCount, int aCardsValueSrc[], unsigned int unSrcCount)
{
	unsigned int unCopyCount = unDestCount > unSrcCount ? unSrcCount : unDestCount;
	for (unsigned int i = 0; i < unCopyCount; ++i)
	{
		aCardsDest[i] = aCardsValueSrc[i];
	}
}


template<class CLCard>
void CLCardOperate<CLCard>::copyCards(int aCardsValueDest[], unsigned int unDestCount, CLCard aCardsSrc[], unsigned int unSrcCount)
{
	unsigned int unCopyCount = unDestCount > unSrcCount ? unSrcCount : unDestCount;
	for (unsigned int i = 0; i < unCopyCount; ++i)
	{
		aCardsValueDest[i] = aCardsSrc[i].value();
	}
}


template<class CLCard>
void CLCardOperate<CLCard>::emptyCards(CLCard aCards[], unsigned int unCardCount)
{
	for (int i = 0; i < unCardCount; ++i)
	{
		aCards[i].empty();
	}
}



template<class CLCard>
bool CLCardOperate<CLCard>::removeCard(CLCard aCards[], unsigned int unCardCount, CLCard cardRemove)
{
	for (int i = 0; i < unCardCount; ++i)
	{
		if (aCards[i] == cardRemove)
		{
			for (int j = i + 1; j < unCardCount; ++j)
			{
				aCards[j - 1] = aCards[j];
			}

			aCards[unCardCount - 1].empty();
			return true;
		}
	}
	return false;
}

template<class CLCard>
bool CLCardOperate<CLCard>::removeCards(CLCard aCards[], unsigned int unCardCount, const CLCard aRemoveCards[], unsigned int unRemoveCount)
{
	// ��������
	if (unRemoveCount > unCardCount)
	{
		return false;
	}

	// �ж�Ҫɾ�������飬�Ƿ����Դ��������
	if (!isCardsInArray(aCards, unCardCount, aRemoveCards, unRemoveCount))
	{
		return false;
	}

	for (int i = 0; i < unRemoveCount; i++)
	{
		removeCard(aCards, unCardCount--, aRemoveCards[i]);
	}

	return true;
}


template<class CLCard>
unsigned char CLCardOperate<CLCard>::removeCards(CLCard aCards[], unsigned int unCardCount, CLCard cardRemove, unsigned int unRemoveCount/* =1*/)
{
	int unRemoveCountTotal = getCardsNum(aCards, unCardCount, cardRemove);
	if (unRemoveCountTotal < unRemoveCount)
	{
		return 0;
	}
	
	for (int i = 0; i < unRemoveCount; i++)
	{
		removeCard(aCards, unCardCount--, cardRemove);
	}
	return unRemoveCount;
}



// aSubsetCards-�Ӽ�
template<class CLCard>
bool CLCardOperate<CLCard>::isCardsInArray(CLCard const aCard[], unsigned int unCardCount, CLCard const aSubsetCards[], unsigned int unSubsetCardCount)
{
	if (0 >= unCardCount || NULL == aCard)
	{
		return false;
	}

	if (unSubsetCardCount <= 0 || NULL == aSubsetCards)
	{
		return false;
	}
	CLCard* pArrCardTemp = new CLCard[unCardCount];
	int nTempSize = unCardCount;
	if (NULL == pArrCardTemp)
	{
		return false;
	}
	copyCards(pArrCardTemp, unCardCount, aCard, unCardCount);

	for (int i = 0; i < unSubsetCardCount; i++)
	{
		if (!removeCard(pArrCardTemp, nTempSize--, aSubsetCards[i]))
		{
			delete[] pArrCardTemp;
			return false;
		}
	}

	delete [] pArrCardTemp;
	return true;
}

template<class CLCard>
int CLCardOperate<CLCard>::findCardIndex(CLCard aCards[], unsigned int unCardCount, CLCard card)
{
	for (int i = 0; i < unCardCount; ++i)
	{
		if (aCards[i] == card)
		{
			return i;
		}
	}

	return -1;
}

template<class CLCard>
int CLCardOperate<CLCard>::getCardsNum(CLCard aCards[], unsigned int unCardCount, CLCard card)
{
	int num = 0;
	for (int i = 0; i < unCardCount; ++i)
	{
		if (aCards[i] == card)
		{
			++num;
		}
	}
	return num;
}

template<class CLCard>
bool CLCardOperate<CLCard>::isCardInArray(CLCard const aCards[], unsigned int unCardCount, CLCard card)
{
	if (0 >= unCardCount || NULL == aCards)
	{
		return false;
	}

	for (int i = 0; i < unCardCount; i++)
	{
		if (aCards[i] == card)
		{
			return true;
		}
	}
	return false;
}