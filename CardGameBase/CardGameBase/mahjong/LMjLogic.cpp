
#include "LMjLogic.h"
#include "math.h"
#include <assert.h>


CLMjLogic::CLMjLogic()
{

}


int CLMjLogic::switchToCardData(const unsigned int arrCardIndex[MAX_INDEX], CLMjCard aCards[MAX_HAND_COUNT])
{
	int cbPosition = 0;

	for (int index = 0; index < MAX_INDEX; index++)
	{
		if (aCards[index] != 0)	// aCards[index] 最大位4张
		{
			for (int j = 0; j < arrCardIndex[index]; j++)
			{
				assert(cbPosition < MAX_HAND_COUNT);
				aCards[cbPosition++] = CLMjCard::switchToCardValue(index);
			}
		}
	}

	return cbPosition;
}


int CLMjLogic::switchToCardIndex(CLMjCard aCards[], unsigned int unCardCount, unsigned int arrCardIndex[MAX_INDEX])
{
	memset(arrCardIndex, 0, sizeof(unsigned int) * MAX_INDEX);

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

bool CLMjLogic::isCanKong(CLMjCard aCards[], unsigned int unCardCount, T_WeaveCardsItem aWeaveItem[], unsigned int unItemSize,
	CLMjCard cardDest)
{
	if (getCardsNum(aCards, unCardCount, cardDest) == 3)
	{
		return true;
	}
	return false;
	//找补杠
	//for (int i = 0; i < iCGPNums; ++i)
	//{
	//	if (lstCGP[i].nType == emType::Peng) //找碰牌
	//	{
	//		//if (byFetchCard == lstCGP[i].val[0])//转转麻将漏杠不能杠
	//		if (GetCardNum(byCards, iCardNums, lstCGP[i].val[0])>0)
	//		{
	//			//添加杠选择表
	//			pActInfo->byGangSels[pActInfo->iGangNums] = lstCGP[i].val[0];
	//			pActInfo->nGType[pActInfo->iGangNums] = emGangType::gt_BuGang;
	//			pActInfo->nBuGangInPeng[pActInfo->iGangNums] = i;
	//			pActInfo->iGangNums++;
	//		}
	//	}
	//}
}




// 要保证手牌已经从小到大排序
// 移除刻子
bool CLMjLogic::removeTriplet(CLMjCard aCards[], unsigned int unCardCount, int nBeginPos /*= 0*/)
{
	CLMjCard cardBegin = aCards[nBeginPos];
	int nMagicNum = getMagicNum(aCards, unCardCount);
	int nMagicUsed = 0;

	// 跳过癞子牌
	if (cardBegin.isMagicCard())
	{
		return false;
	}

	// 移除起始牌
	if (!removeCard(aCards, unCardCount--, cardBegin))
	{
		return false;
	}

	// 移除失败时没有该牌时用癞子补
	if (!removeCard(aCards, unCardCount--, cardBegin))
	{
		++nMagicUsed;
		// 如果癞子都不够用了则直接返回
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

	/* 分头吃 中吃 尾吃*/

	// 没有该牌时或不是同一花色 用癞子补
	if (!removeCard(aCards, unCardCount--, cardBegin + 1) || ((cardBegin + 1) / 10 != cardBegin / 10))
	{
		++nMagicUsed;
		// 如果癞子都不够用了则直接返回
		if (nMagicUsed > nMagicNum)
		{
			return false;
		}
	}

	if (!removeCard(aCards, unCardCount--, cardBegin + 2) || ((cardBegin + 2) / 10 != cardBegin / 10))
	{
		++nMagicUsed;
		// 如果癞子都不够用了则直接返回
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
	const int c_nColorCount = CLMjCard::EM_Max_Color;	// 花色最大种类
	unsigned int arrColorIndex[c_nColorCount];			// 定义花牌种类索引
	int nColorBiggerZero = 0;							// 定义牌数大于0的花色有多少种
	memset(arrColorIndex, 0, sizeof(unsigned int)* c_nColorCount);

	// 手牌
	for (int i = 0; i < unCardCount; ++i)
	{
		arrColorIndex[aCards[i].color()]++;
	}

	// 组合牌
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




bool CLMjLogic::isFlowerPig(CLMjCard aCards[], unsigned int unCardCount, T_WeaveCardsItem aWeaveItem[], unsigned int unItemSize)
{
	// 所有牌的花色种类大于2就是花猪
	return getColorCount(aCards, unCardCount, aWeaveItem, unItemSize) > 2;
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

	// 复制牌组
	unsigned int unCardNumTemp = unCardCount;
	CLMjCard aCardsTemp[MAX_HAND_COUNT];
	copyCards(aCardsTemp, unCardNumTemp, aCards, unCardCount);

	// 理牌
	sortCards(aCardsTemp, unCardNumTemp);

	bool bCanHu3x2 = isCanHu_3x2(aCardsTemp, unCardNumTemp, bMust258Pair);
	return bCanHu3x2;
}

// 递归删除法
bool CLMjLogic::isCanHu_3x2(CLMjCard aCards[], unsigned int unCardCount, bool bMust258Pair /*= false*/)
{
	assert(unCardCount > MAX_HAND_COUNT || unCardCount <= 0);
	if (unCardCount % 3 != 2)
	{
		return false;
	}

	// 剩下一对将就可以胡了
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

	// 重新拷贝一份，以此递归失败时恢复起始点
	CLMjCard aCardsTemp[MAX_HAND_COUNT];
	copyCards(aCardsTemp, unCardCount, aCards, unCardCount);

	// 剔除刻子
	if (removeTriplet(aCardsTemp, unCardCount))
	{
		isCanHu_3x2(aCardsTemp, unCardCount - 3); // 剔除成功则使用aCardsTemp传参
	}

	// 剔除失败则重新拷贝
	copyCards(aCardsTemp, unCardCount, aCards, unCardCount);

	// 剔除顺子
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

	//单牌数目
	int cbReplaceCount = 0;

	//临时数据
	unsigned int arrCardIndexTemp[MAX_INDEX];
	switchToCardIndex(aCards, unCardCount, arrCardIndexTemp);

	//计算单牌
	for (int i = 0; i < MAX_INDEX; i++)
	{
		int cbCardCount = arrCardIndexTemp[i];

		//单牌统计
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
		if (aWeaveItem[i].byWeaveKind  & (ACTION_LEFT | ACTION_CENTER | ACTION_RIGHT))
			return false;
	}
	return true;
}

// 我的思路，只有一种花色的牌数大于0，其他花色的牌数为0，就是清一色 20200321 leisx
bool CLMjLogic::isQingYiSe(CLMjCard aCards[], unsigned int unCardCount, T_WeaveCardsItem aWeaveItem[], unsigned int unItemSize)
{
	return getColorCount(aCards, unCardCount, aWeaveItem, unItemSize) == 1;
}





