
#include "SiChuanMjThink.h"
#include <assert.h>



static int s_nCurTotalScore = 0;

CSiChuanMjThink::CSiChuanMjThink()
{


}


//////////////////////////////////////////////////////////////////////////
//  接口实现
int CSiChuanMjThink::thinkDingQue(CLMjCard aCards[], unsigned int unCardCount)
{
	int nColorQue = -1;
	unsigned int arrColorIndex[3];
	memset(arrColorIndex, 0, sizeof(unsigned int)* 3);

	for (unsigned int i = 0; i < unCardCount; ++i)
	{
		assert(aCards[i].color() < 3);
		arrColorIndex[aCards[i].color()]++;
	}

	unsigned int fewest = 14;
	for (int i = 0; i < 3; ++i)
	{
		if (arrColorIndex[i] < fewest)
		{
			fewest = arrColorIndex[i];
			nColorQue = i;
		}
	}

	return nColorQue;
}


void CSiChuanMjThink::think(T_MjActInfo* pMjActInfo, CLMjCard aCards[], unsigned int unCardCount, T_WeaveCardsItem aWeaveItem[]
	, unsigned int unItemSize, CLMjCard cardOut, int nQueColor /*= -1*/, unsigned short usIgnoreFlags /*= 0*/)
{
	// 如果要杠的牌是缺
	if (cardOut.isValid() && cardOut.color() == nQueColor)
	{
		return;
	}
	clear();

	// 拷贝数据
	copyCards(aCards, unCardCount, aWeaveItem, unItemSize, cardOut, nQueColor);
	think();
	s_nCurTotalScore = totalScore();

	// 思考后又重新拷贝
	copyCards(aCards, unCardCount, aWeaveItem, unItemSize, cardOut, nQueColor);

	// 首先先思考能不能胡
	if (!(usIgnoreFlags & E_ActionTypeFlags::EA_hu))
	{
		if (thinkHu(pMjActInfo->tMjActHuInfo))
		{
			pMjActInfo->usActFlags |= (E_ActionTypeFlags::EA_hu);
			return;
		}
	}
	if (!(usIgnoreFlags & E_ActionTypeFlags::EA_Kong))
	{
		if (thinkKong(pMjActInfo->tMjActKongInfo))
		{
			pMjActInfo->usActFlags |= (E_ActionTypeFlags::EA_Kong);
			return;
		}
	}
	if (!(usIgnoreFlags & E_ActionTypeFlags::EA_Pong))
	{
		if (thinkPong())
		{
			// 如果执行碰还要找出碰过后立马要出的牌
			pMjActInfo->usActFlags |= (E_ActionTypeFlags::EA_Pong);
			return;
		}
	}

	pMjActInfo->tMjActOutInfo.nOutCardValue = thinkOutCard();

}

void CSiChuanMjThink::think()
{
	CLScoreBasicThink::think();
}


bool CSiChuanMjThink::thinkHu(T_MjActHuInfo & tMjActHuInfo)
{
	return m_mjLogic.isCanHu(m_arrHandCard, m_nHandNums, m_arrWeaveCardsItem, m_nWeaveCardsItemNum, tMjActHuInfo, m_cardOut);

}

bool CSiChuanMjThink::thinkKong(T_MjActKongInfo & tMjActKongInfo)
{
	T_MjActKongInfo tMjActKongInfoTemp;
	bool bCanKong = m_mjLogic.isCanKong(m_arrHandCard, m_nHandNums, m_arrWeaveCardsItem, m_nWeaveCardsItemNum, m_cardOut, tMjActKongInfoTemp);
	if (!bCanKong)
	{
		return false;
	}

	// 有多个杠时，目前只按顺序思考杠，不考虑在其中选择最佳的杠
	CSiChuanMjThink *pThinkAfter = clone();
	int nAfterKongScore = pThinkAfter->thinkAfterKong(tMjActKongInfoTemp.arrKongSelect[0], tMjActKongInfoTemp.arrKongType[0]);
	E_KongType eKongType = tMjActKongInfoTemp.arrKongType[0];
	int nLostCardNums = 0;	// 由于杠牌原牌中跑到组合牌中的张数
	switch (eKongType)
	{
	case EK_KongAn:
		nLostCardNums = 4;
		break;
	case EK_KongBa:
		nLostCardNums = 1;
		break;
	case EK_KongDian:
		nLostCardNums = 3;
		break;
	default:
		break;
	}
	bool bExecKong = nAfterKongScore >= s_nCurTotalScore - SCORE_Kong * nLostCardNums;
	delete pThinkAfter;

	if (bExecKong)
	{
		tMjActKongInfo.arrKongSelect[0] = tMjActKongInfoTemp.arrKongSelect[0];
		tMjActKongInfo.arrKongType[0] = tMjActKongInfoTemp.arrKongType[0];
		tMjActKongInfo.nCurSelectIndex = 0;
		tMjActKongInfo.nKongSelectNums = 1;
	}
	return bExecKong;
}


int CSiChuanMjThink::thinkAfterKong(CLMjCard cardGangDest, E_KongType eKongType)
{
	switch (eKongType)
	{
	case EK_KongAn:
		m_mjLogic.removeCards(m_arrHandCard, m_nHandNums, cardGangDest, 4);
		m_nHandNums -= 4;
		break;
	case EK_KongBa:
		m_mjLogic.removeCards(m_arrHandCard, m_nHandNums, cardGangDest, 1);
		m_nHandNums--;
		break;
	case EK_KongDian:
		m_mjLogic.removeCards(m_arrHandCard, m_nHandNums, cardGangDest, 3);
		m_nHandNums -= 3;
		break;
	default:
		break;
	}

	think();
	return totalScore();
}



bool CSiChuanMjThink::thinkPong()
{

	if (!m_cardOut.isValid())
	{
		return false;
	}

	bool bCanPong = m_mjLogic.isCanPong(m_arrHandCard, m_nHandNums, m_cardOut);
	if (!bCanPong)
	{
		return false;
	}

	bool bExecPong = false;
	CSiChuanMjThink *pThinkAfter = clone();
	bExecPong = pThinkAfter->thinkAfterPong() >= s_nCurTotalScore - SCORE_Triplet * 2;

	delete pThinkAfter;
	return bExecPong;
}


int CSiChuanMjThink::thinkAfterPong()
{
	m_mjLogic.removeCards(m_arrHandCard, m_nHandNums, m_cardOut, 2);
	m_nHandNums -= 2;
	think();

	return totalScore();
}


CLMjCard CSiChuanMjThink::thinkOutCard()
{
	int pos = isExistQue(m_colorQue);
	if (pos > 0)
	{
		// 如果有缺直接返回
		return m_arrHandCard[pos];
	}

	think();
	return m_cardBadly;
}





