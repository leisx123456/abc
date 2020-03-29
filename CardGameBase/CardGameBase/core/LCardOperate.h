#pragma once
#include "LCard.h"
#include <iostream>
#include <vector>
#include <algorithm>


#define INVALID_CARD		0
/************************************************************************/
/* 
CLCardOperate 牌操作容器类， 即对多数量的牌管理操作, 由于是用的数组而非stl容器，需要自己实现一些函数
*/
/************************************************************************/

// 声明为类模板，因为还不知道对确切的哪一种牌操作
template<class CLCard>
class CLCardOperate
{
public:
	CLCardOperate();
	virtual ~CLCardOperate();

	// 洗牌
	void shuffleCards(CLCard aCards[], unsigned int unCardCount);

	// 理牌
	void sortCards(CLCard aCards[], unsigned int unCardCount);

	// 复制-(由于对象不能memcpy,需要自己定义)
	void copyCards(CLCard aCardsDest[], unsigned int unDestCount, CLCard aCardsSrc[], unsigned int unSrcCount);
	void copyCards(CLCard aCardsDest[], unsigned int unDestCount, int aCardsValueSrc[], unsigned int unSrcCount);
	void copyCards(int aCardsValueDest[], unsigned int unDestCount, CLCard aCardsSrc[], unsigned int unSrcCount);
	void emptyCards(CLCard aCards[], unsigned int unCardCount);

	// 删除操作
	// 根据牌值表删除
	//bool RemoveCard(byte cbCardIndex[MAX_INDEX], byte cbRemoveCard);
	//bool RemoveCard(byte cbCardIndex[MAX_INDEX], const byte cbRemoveCard[], byte cbRemoveCount);

	// 普通删除
	bool removeCard(CLCard aCards[], unsigned int unCardCount, CLCard cardRemove);
	bool removeCards(CLCard aCards[], unsigned int unCardCount, const CLCard aRemoveCards[], unsigned int unRemoveCount);
	unsigned char removeCards(CLCard aCards[], unsigned int unCardCount, CLCard cardRemove, unsigned int unRemoveCount = 1);

	// 查找
	// 返回找到该牌的数组下标
	int findCardIndex(CLCard aCards[], unsigned int unCardCount, CLCard card);

	// 从牌数组中，获取某一牌值得具体个数
	int getCardsNum(CLCard aCards[], unsigned int unCardCount, CLCard card);

	// other
	bool isCardInArray(CLCard const aCards[], unsigned int unCardCount, CLCard card);
	bool isCardsInArray(CLCard const aCards[], unsigned int unCardCount, CLCard const aSubsetCards[], unsigned int unSubsetCardCount);

};



/* 定义*/
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

	 //方法2
	//混乱准备
	CLCard *cbCardDataTemp = new CLCard[unCardCount];
	for (int i = 0; i < unCardCount; ++i)
	{
		cbCardDataTemp[i] = aCards[i];
	}

	//混乱扑克
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

	// 方法2
	// 从小到大选择排序
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
	// 检验数据
	if (unRemoveCount > unCardCount)
	{
		return false;
	}

	// 判断要删除的数组，是否存在源数组里面
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



// aSubsetCards-子集
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