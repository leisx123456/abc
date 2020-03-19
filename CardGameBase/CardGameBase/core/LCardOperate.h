#pragma once
#include "LCard.h"
#include <iostream>
#include <vector>
#include <algorithm>

#define INVALID_CARD		0

// ����Ϊ��ģ�壬��Ϊ����֪����ȷ�е���һ���Ʋ���
template<class CLCard>
class CLCardOperate
{
public:
	CLCardOperate();
	virtual ~CLCardOperate();

	// ��Ч���ж�
	//virtual bool isValidCard(CLCard card) = 0;
	virtual bool isValidCard(CLCard card) { return false; }

	// ϴ��
	void shuffleCard(CLCard aCards[], unsigned int unCardCount);

	// ����
	void sortCard(CLCard aCards[], unsigned int unCardCount);

	// ɾ������
	// ������ֵ��ɾ��
	//bool RemoveCard(byte cbCardIndex[MAX_INDEX], byte cbRemoveCard);
	//bool RemoveCard(byte cbCardIndex[MAX_INDEX], const byte cbRemoveCard[], byte cbRemoveCount);

	// ��ͨɾ��
	bool removeCard(CLCard aCards[], unsigned int unCardCount, CLCard cardRemove);
	bool removeCards(CLCard aCards[], unsigned int unCardCount, const CLCard aRemoveCards[], unsigned int unRemoveCount);
	unsigned char removeAllCards(CLCard aCards[], unsigned int unCardCount, CLCard cardRemove);

	// ����
	// �����ҵ����Ƶ������±�
	int findCardIndex(CLCard aCards[], unsigned int unCardCount, CLCard card);

	// ���������У���ȡĳһ��ֵ�þ������
	int getCardNum(CLCard aCards[], unsigned int unCardCount, CLCard card);

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
void CLCardOperate<CLCard>::shuffleCard(CLCard aCards[], unsigned int unCardCount)
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
void CLCardOperate<CLCard>::sortCard(CLCard aCards[], unsigned int unCardCount)
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

			aCards[unCardCount - 1] = INVALID_CARD;
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
	CLCard* cardTemp = new CLCard[unCardCount];
	int nTempSize = unCardCount;
	if (NULL == cardTemp)
	{
		return false;
	}
	memcpy(cardTemp, aCard, sizeof(CLCard)* unCardCount);

	for (int i = 0; i < unSubsetCardCount; i++)
	{
		if (!removeCard(cardTemp, nTempSize--, aSubsetCards[i]))
		{
			delete cardTemp;
			cardTemp = NULL;
			return false;
		}
	}

	delete cardTemp;
	cardTemp = NULL;
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
int CLCardOperate<CLCard>::getCardNum(CLCard aCards[], unsigned int unCardCount, CLCard card)
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