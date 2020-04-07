#include "LMjDesk.h"

#include <time.h>
#include <iostream>
#include <stdlib.h>
#include <assert.h>

CLMjDesk::CLMjDesk()
: m_nBanker(EW_South)
{


m_nMjNumSurplus = MJ_MAX_CARD_COUNT;
m_nIndexStart = 0;
m_nIndexCurrent = 0;
m_nBanker = 0;
m_nActiveUser = 0;
m_nCurExecActUser = -1;
m_bHuangZhuang = false;
m_nHuOrderNum = 1;
//m_nAlreadyHuNum = 0;

}


CLMjDesk::~CLMjDesk()
{
}

// 获取下nUnit个单位的索引 
//@param-in nCurIndex 当前的索引
//@param-in nPlayerCount为实际参与的玩家数
//@param-in bClockWise顺时针, antiClockWise 逆时针
//@param-in nUnit多少个单位
int CLMjDesk::nextPlayerIndex(int nCurIndex, int nPlayerCount /*= GAME_MJ_PLAYER_MAX*/, bool bClockWise /*= true*/, int nUnit /*= 1*/)
{
	if (nUnit < 1)
	{
		return nCurIndex;
	}
	int nNextIndex = nCurIndex;
	if (bClockWise)
	{
		for (int i = 0; i < nUnit; ++i)
		{
			nNextIndex = (nPlayerCount + nNextIndex - 1) % nPlayerCount;
		}
	}
	else
	{
		for (int i = 0; i < nUnit; ++i)
		{
			nNextIndex = (nPlayerCount + nNextIndex + 1) % nPlayerCount;
		}
	}

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
#ifdef TEST_LOCAL_LX
		// 测试手牌

		// 定义庄家玩家的手牌测试
		//int arrBanker[] = { 0x21, 0x21, 0x21, 0x22, 0x22, 0x22, 0x23, 0x03, 0x04, 0x05, 0x06, 0x07, 0x16, 0x08 };
		int arrBanker[] = { 0x01, 0x02, 0x03, 0x03, 0x03, 0x13, 0x14, 0x15, 0x15, 0x16, 0x17, 0x18, 0x19, 0x17 };

		// 定义各玩家的手牌测试 // 测试胡
		//int arrTest[][14] =
		//{ 
		//	{ 0x01, 0x01, 0x01, 0x02, 0x02, 0x02, 0x03, 0x03, 0x03, 0x04, 0x04, 0x04, 0x21, 0 },
		//	{ 0x11, 0x11, 0x12, 0x12, 0x13, 0x13, 0x14, 0x14, 0x15, 0x15, 0x16, 0x16, 0x17, 0 },
		//	{ 0x09, 0x09, 0x09, 0x08, 0x08, 0x08, 0x07, 0x07, 0x07, 0x06, 0x06, 0x06, 0x05, 0 },
		//	{ 0x21, 0x22, 0x23, 0x23, 0x24, 0x25, 0x25, 0x26, 0x27, 0x27, 0x27, 0x16, 0x17, 0 }	
		//};

		// 测试杠牌
		int arrTest[][14] =
		{ 
			{ 0x01, 0x01, 0x01, 0x02, 0x02, 0x02, 0x03, 0x03, 0x03, 0x04, 0x04, 0x04, 0x21, 0 },
			{ 0x11, 0x11, 0x11, 0x12, 0x13, 0x13, 0x14, 0x14, 0x14, 0x16, 0x16, 0x16, 0x17, 0 },
			{ 0x09, 0x09, 0x09, 0x08, 0x08, 0x08, 0x07, 0x07, 0x07, 0x06, 0x06, 0x05, 0x02, 0 },
			{ 0x21, 0x22, 0x22, 0x23, 0x24, 0x25, 0x25, 0x25, 0x27, 0x27, 0x27, 0x17, 0x17, 0 }	
		};

		// 定义紧接着摸完手牌后要摸的牌
		int arrDrawCard[] = { 0x17, 0x05, 0x17, 0x21 };

		std::memcpy(arrTest[m_nBanker], arrBanker, sizeof(arrBanker));
		int pos = 2; // 保证骰子摇的是1+4或1+8

		// 前面3次是4张四张的拿
		int k = 0;
		for (int j = 0; j < 3; ++j)
		{
			for (int i = 0; i < playerCount(); ++i)
			{
				m_arrMjCardsPair[pos++] = arrTest[i][k];
				m_arrMjCardsPair[pos++] = arrTest[i][k + 1];
				m_arrMjCardsPair[pos++] = arrTest[i][k + 2];
				m_arrMjCardsPair[pos++] = arrTest[i][k + 3];
			}
			k += 4;
		}
		k = 12;
		for (int i = 0; i < playerCount(); ++i)
		{
			m_arrMjCardsPair[pos++] = arrTest[i][k];
		}
		m_arrMjCardsPair[pos++] = arrBanker[13];

		int nDrawNums = sizeof(arrDrawCard) / sizeof(int);
		for (int i = 0; i < nDrawNums; ++i)
		{
			m_arrMjCardsPair[pos++] = arrDrawCard[i];
		}
	
		//int nTotalFetchHandCards = playerCount() * 13 + 1;
		//int *pHand = new int [nTotalFetchHandCards * sizeof(int)];
		//int nOffsetHand = 0;
		//for (int i = 0; i < playerCount(); ++i)
		//{
		//	int nLengthHand = arrTest[i][13] == 0 ? 13 : 14;
		//	std::memcpy(pHand + nOffsetHand, arrTest[i], nLengthHand * sizeof(int));
		//	nOffsetHand += nLengthHand;
		//}
		//delete []pHand;
#else
	m_mjLogic.shuffleCards(m_arrMjCardsPair, mjNumAllocation());
#endif // TEST_HU
	
	
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
	int nMjNumPairWall1st = nMjNumPairAWall + (nMjNumPairMod-- > 0 ? 1 : 0);
	int nMjNumPairWall2nd = nMjNumPairAWall + (nMjNumPairMod-- > 0 ? 1 : 0);
	int nMjNumPairWall3rd = nMjNumPairAWall + (nMjNumPairMod-- > 0 ? 1 : 0);
	int nMjNumPairWall4th = nMjNumPairAWall + (nMjNumPairMod-- > 0 ? 1 : 0) ;

	int nMjNumWall1st = nMjNumPairWall1st * 2;
	int nMjNumWall2nd = nMjNumPairWall2nd * 2;
	int nMjNumWall3rd = nMjNumPairWall3rd * 2;
	int nMjNumWall4th = nMjNumPairWall4th * 2;

	// 从庄家开始 顺时针 分配麻将到4个牌墙
	int nIndexPlayer = m_nBanker;
	int nIndexCard = 0;
	for (int i = 0; i < nMjNumWall1st; ++i)
	{
		m_arrMjCardWall[nIndexPlayer][i] = m_arrMjCardsPair[nIndexCard++];
	}

	nIndexPlayer = nextPlayerIndex(nIndexPlayer, playerCount());
	for (int i = 0; i < nMjNumWall2nd; ++i)
	{
		m_arrMjCardWall[nIndexPlayer][i] = m_arrMjCardsPair[nIndexCard++];
	}

	nIndexPlayer = nextPlayerIndex(nIndexPlayer, playerCount());
	for (int i = 0; i < nMjNumWall3rd; ++i)
	{
		m_arrMjCardWall[nIndexPlayer][i] = m_arrMjCardsPair[nIndexCard++];
	}

	nIndexPlayer = nextPlayerIndex(nIndexPlayer, playerCount());
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
	int nMjNumPairWall1st = nMjNumPairAWall + (nMjNumPairMod-- > 0 ? 1 : 0);
	int nMjNumPairWall2nd = nMjNumPairAWall + (nMjNumPairMod-- > 0 ? 1 : 0);
	int nMjNumPairWall3rd = nMjNumPairAWall + (nMjNumPairMod-- > 0 ? 1 : 0);
	int nMjNumPairWall4th = nMjNumPairAWall + (nMjNumPairMod-- > 0 ? 1 : 0);
	int arrWall[4] = { nMjNumPairWall1st, nMjNumPairWall2nd, nMjNumPairWall3rd, nMjNumPairWall4th };

	// 确定起牌座位-相对于庄家
	int nStartChair = nextPlayerIndex(m_nBanker, 4, false, m_mjDicePair.total() - 1);

	// 起牌序号-双
	int nIndexAWall = m_mjDicePair.min() * 2; // 0, 2, 4...

	// 由于抓牌是顺时针方向，庄家为0，arrWall[0], 庄家的上家为3，arrWall[1], 具体取那个牌墙用顺时针得出牌墙序号nIndex
	int nIndex = nextPlayerIndex(m_nBanker, 4, true, m_mjDicePair.total() - 1);
	for (int i = 0; i < nIndex; ++i)
	{
		m_nIndexStart += arrWall[i] * 2;
	}
	m_nIndexStart += nIndexAWall;
	m_nIndexCurrent = m_nIndexStart;

}


CLMjCard CLMjDesk::drawCard()
{
	if (m_nIndexCurrent >= mjNumAllocation())
	{
		m_nIndexCurrent = 0;
	}
	CLMjCard card = m_arrMjCardsPair[m_nIndexCurrent];
	m_arrMjCardsPair[m_nIndexCurrent] = CARD_EMPTY;
	m_nIndexCurrent++;

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
	std::cout << playerCount();
	// 开始三轮麻将四张四张的拿，从庄家开始(前12张)
	for (int i = 0; i < 3; ++i)
	{
		nIndex = m_nBanker;
		for (int i = 0; i < playerCount(); ++i)
		{
			m_pArrMjPlayer[nIndex]->drawCard(drawCard());
			m_pArrMjPlayer[nIndex]->drawCard(drawCard());
			m_pArrMjPlayer[nIndex]->drawCard(drawCard());
			m_pArrMjPlayer[nIndex]->drawCard(drawCard());
			nIndex = nextPlayerIndex(nIndex, playerCount(), false);
		}
	}

	//  --跳牌
	// (第13张)
	nIndex = m_nBanker;
	for (int i = 0; i < playerCount(); ++i)
	{
		m_pArrMjPlayer[nIndex]->drawCard(drawCard());
		nIndex = nextPlayerIndex(nIndex, playerCount(), false);
	}
	// 仅限庄家（第14张）
	m_pArrMjPlayer[m_nBanker]->drawCard(drawCard());

	//
	int nHandNums;
	for (int i = 0; i < playerCount(); ++i)
	{
		m_pArrMjPlayer[i]->getHandCards(pArrCardHand[i], nHandNums);
	}
}


