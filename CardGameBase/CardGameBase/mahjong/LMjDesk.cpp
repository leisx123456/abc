#include "LMjDesk.h"
#include "LMjMachine.h"
#include <time.h>
#include <iostream>
#include <stdlib.h>
#include <assert.h>

CLMjDesk::CLMjDesk()
: m_nBanker(EW_South)
{
	m_pArrMjPlayer[0] = new CLMjPlayer(CLMjPlayer::EP_People);
	m_pArrMjPlayer[1] = new CLMjPlayer(CLMjPlayer::EP_CmpNormal);
	m_pArrMjPlayer[2] = new CLMjPlayer(CLMjPlayer::EP_CmpNormal);
	m_pArrMjPlayer[3] = new CLMjPlayer(CLMjPlayer::EP_CmpNormal);

m_nMjNumSurplus = GAME_MJ_CARD_COUNT_MAX;
m_nIndexStart = 0;
m_nIndexCurrent = 0;
m_nBanker = 0;
m_nActiveUser = 0;

}


CLMjDesk::~CLMjDesk()
{
}


int CLMjDesk::nextPlayerIndex(int nCurIndex, int nPlayerCount /*= GAME_MJ_PLAYER_MAX*/)
{
	int nNextIndex = (nCurIndex + 1) % nPlayerCount;
	return nNextIndex;
}


int CLMjDesk::getSurplusCardsOnWall()
{
	return m_nMjNumSurplus;
}

// 最原始的方法是先从108张中随机抽再从新的牌堆里随机抽出一张，直到抽完出一张，剩下的牌组成新的牌堆
void CLMjDesk::ruffle()
{
	assert(1 < mjNumAllocation());
	m_mjLogic.shuffleCards(m_arrMjCardsPair, mjNumAllocation());
}

/*************************************************************/
/*函数功能：把牌分派到4面牌墙
/*入口参数：无
/*返回类型：void
/*************************************************************/
void CLMjDesk::initWall()
{
	/* step1 先确定每个牌墙的麻将数量 */
	// 由于麻将牌墙以双为单位，确定麻将有多少双(顿)
	assert((mjNumAllocation() % 2) == 0); // 麻将不能为奇数
	int nMjNumPair = mjNumAllocation() / 2;

	// 四个牌墙中每个牌墙得到的多少双 和 余下多少双
	int nMjNumPairAWall = nMjNumPair / 4;
	int nMjNumPairMod = nMjNumPair % 4;

	// 每个牌墙的实际有多少双
	int nMjNumPairWall1st = nMjNumPairAWall + (nMjNumPairMod-- > 1 ? 1 : 0);
	int nMjNumPairWall2nd = nMjNumPairAWall + (nMjNumPairMod-- > 1 ? 1 : 0);
	int nMjNumPairWall3rd = nMjNumPairAWall + (nMjNumPairMod-- > 1 ? 1 : 0);
	int nMjNumPairWall4th = nMjNumPairAWall + (nMjNumPairMod-- > 1 ? 1 : 0) ;

	int nMjNumWall1st = nMjNumPairWall1st * 2;
	int nMjNumWall2nd = nMjNumPairWall1st * 2;
	int nMjNumWall3rd = nMjNumPairWall1st * 2;
	int nMjNumWall4th = nMjNumPairWall1st * 2;

	// 从庄家开始 顺时针 分配麻将到4个牌墙
	int nIndexPlayer = m_nBanker;
	int nIndexCard = 0;
	for (int i = 0; i < nMjNumWall1st; ++i)
	{
		m_arrMjCardWall[nIndexPlayer][i] = m_arrMjCardsPair[nIndexCard++];
	}

	nIndexPlayer = nextPlayerIndex(nIndexPlayer);
	for (int i = 0; i < nMjNumWall2nd; ++i)
	{
		m_arrMjCardWall[nIndexPlayer][i] = m_arrMjCardsPair[nIndexCard++];
	}

	nIndexPlayer = nextPlayerIndex(nIndexPlayer);
	for (int i = 0; i < nMjNumWall3rd; ++i)
	{
		m_arrMjCardWall[nIndexPlayer][i] = m_arrMjCardsPair[nIndexCard++];
	}

	nIndexPlayer = nextPlayerIndex(nIndexPlayer);
	for (int i = 0; i < nMjNumWall4th; ++i)
	{
		m_arrMjCardWall[nIndexPlayer][i] = m_arrMjCardsPair[nIndexCard++];
	}
}


void CLMjDesk::rockDice()
{
	// 摇骰子
	m_mjDicePair.rock();

	/*确定牌墙*/
	// 由于麻将牌墙以双为单位，确定麻将有多少双(顿)
	assert((mjNumAllocation() % 2) == 0); // 麻将不能为奇数
	int nMjNumPair = mjNumAllocation() / 2;

	// 四个牌墙中每个牌墙得到的多少双 和 余下多少双
	int nMjNumPairAWall = nMjNumPair / 4;
	int nMjNumPairMod = nMjNumPair % 4;

	// 每个牌墙的实际有多少双
	int nMjNumPairWall1st = nMjNumPairAWall + (nMjNumPairMod-- > 1 ? 1 : 0);
	int nMjNumPairWall2nd = nMjNumPairAWall + (nMjNumPairMod-- > 1 ? 1 : 0);
	int nMjNumPairWall3rd = nMjNumPairAWall + (nMjNumPairMod-- > 1 ? 1 : 0);
	int nMjNumPairWall4th = nMjNumPairAWall + (nMjNumPairMod-- > 1 ? 1 : 0);
	int arrWall[4] = { nMjNumPairWall1st, nMjNumPairWall2nd, nMjNumPairWall3rd, nMjNumPairWall4th };

	
	int nDirection = (m_mjDicePair.total() - 1) % 4;// 0， 1， 2， 3， 0-庄 
	int nIndexAWall = (m_mjDicePair.min() - 1) * 2; // 0, 2, 4...
	int nIndex = 0;
	for (int i = 0; i < 4; ++i)
	{
		if (nDirection-- > 0)
		{
			nIndex += arrWall[i] * 2;
		}
	}
	m_nIndexStart = nIndex + nIndexAWall;
	m_nIndexCurrent = m_nIndexStart;

}


CLMjCard CLMjDesk::drawCard()
{
	CLMjCard card = m_arrMjCardsPair[m_nIndexCurrent];
	m_arrMjCardsPair[m_nIndexCurrent] = CARD_EMPTY;
	m_nIndexCurrent++;
	if (m_nIndexCurrent >= mjNumAllocation())
	{
		m_nIndexCurrent = 0;
	}
	return card;
}






//void CLMjDesk::SortCardData(byte wviewid)
//{
//	MjLogic::getInstance()->SortCardList(m_nHandCard[wviewid], CountArray(m_nHandCard[wviewid]));
//}
//
//
//
//void CLMjDesk::outCard(byte cbViewId, byte cbCard)
//{
//	assert(cbViewId >= playerCount());
//	MjLogic::getInstance()->RemoveValueCardOne(m_nHandCard[cbViewId], m_nHandCardCount[cbViewId], cbCard);
//	m_vecOutCard[cbViewId].push_back(cbCard);
//}

// 发手牌
void CLMjDesk::dealCards(CLMjCard pArrCardHand[][14])
{
	int nIndex = m_nBanker;
	int nPlayerNum = playerCount();
	std::cout << nPlayerNum;
	// 开始三轮麻将四张四张的拿，从庄家开始(前12张)
	for (int i = 0; i < 3; ++i)
	{
		nIndex = m_nBanker;
		for (int i = 0; i < nPlayerNum; ++i)
		{
			m_pArrMjPlayer[nIndex]->drawCard(drawCard());
			m_pArrMjPlayer[nIndex]->drawCard(drawCard());
			m_pArrMjPlayer[nIndex]->drawCard(drawCard());
			m_pArrMjPlayer[nIndex]->drawCard(drawCard());
			nIndex = nextPlayerIndex(nIndex, nPlayerNum);
		}
	}

	//  --跳牌
	// (第13张)
	nIndex = m_nBanker;
	for (int i = 0; i < nPlayerNum; ++i)
	{
		m_pArrMjPlayer[nIndex]->drawCard(drawCard());
		nIndex = nextPlayerIndex(nIndex, nPlayerNum);
	}
	// 仅限庄家（第14张）
	m_pArrMjPlayer[m_nBanker]->drawCard(drawCard());

	//
	for (int i = 0; i < playerCount(); ++i)
	{
		m_pArrMjPlayer[i]->getHandCards(pArrCardHand[i]);
	}
}
