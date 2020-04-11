#include "LMjPlayer.h"
#include "ai/AbstractThink.h"
#include <iostream>

CLMjPlayer::CLMjPlayer()
: m_ePlayerType(EP_People)
{
	init();
}


CLMjPlayer::CLMjPlayer(E_PlayerType ePlayerType, int nChairID)
: m_ePlayerType(ePlayerType)
, m_nChairID(nChairID)
{
	init();
}


void CLMjPlayer::init()
{


	//m_CardNew = CARD_EMPTY;
	

	m_nHandNums = 0;
	m_nWeaveItemNums = 0;
	m_ePlayerActiveState = E_PlayerActiveState::p_unActive;
	CLMjCard m_arrOutedCards[MJ_MAX_OUTED_COUNT];
	for (int i = 0; i < MJ_MAX_OUTED_COUNT; i++)
	{
		m_arrOutedCards[i] = CARD_EMPTY;
	}
	m_nOutedTimes = 0;

	m_nDrawTimes = 0;	//抓牌次数

	m_bReady = false;
	m_eColorTBA = CLMjCard::EM_Invalid_Color;
}



CLMjPlayer::~CLMjPlayer()
{

}

// 刚摸的牌放在最后一个位置， 不需要理牌
void CLMjPlayer::drawCard(const CLMjCard & card)
{
	m_arrHandCards[m_nHandNums++] = card;
	m_nDrawTimes++;
}


void CLMjPlayer::sordCards()
{
	if (m_nHandNums < 1)
	{
		return;
	}
	m_pMjLogic->sortCards(m_arrHandCards, m_nHandNums);
}



bool CLMjPlayer::outCard(const CLMjCard & card)
{
	if (!m_pMjLogic->removeCard(m_arrHandCards, m_nHandNums, card))
	{
		return false;
	}
	m_nHandNums--;

		//重新排列手牌
	m_pMjLogic->sortCards(m_arrHandCards, m_nHandNums);

		//向出牌表中添加该牌
	m_vecCardOut.push_back(card);

		//更新出牌计数
	m_nOutedTimes++;

	return true;
}

CLMjCard CLMjPlayer::getLatestOutCard()
{
	return m_vecCardOut.back();
}


void CLMjPlayer::getOutCardsValue(int* pArrOutCardsValue, int & nOutCardNums)
{
	for (int i = 0; i < m_vecCardOut.size(); ++i)
	{
		pArrOutCardsValue[i] = m_vecCardOut[i].value();
	}
	nOutCardNums = m_vecCardOut.size();
}


void CLMjPlayer::removeLatestOutCard()
{
	m_vecCardOut.pop_back();
}





bool CLMjPlayer::execPong(unsigned char byProvideUser, const CLMjCard & cardOut)
{
	// 更新手牌
	if (m_pMjLogic->removeCards(m_arrHandCards, m_nHandNums, cardOut, 2) != 2)
	{
		return false;
	}
	m_nHandNums -= 2;

	// 更新组合牌
	T_WeaveCardsItem tWeaveCardsItem;
	tWeaveCardsItem.byWeaveKind = T_WeaveCardsItem::EW_Triplet;
	tWeaveCardsItem.cardCenter = cardOut;
	tWeaveCardsItem.cardPublic = cardOut;
	tWeaveCardsItem.byProvideUser = byProvideUser;
	for (int i = 0; i < 3; i++)
	{
		tWeaveCardsItem.aCards[i] = cardOut;
	}
	m_arrWeaveCardsItem[m_nWeaveItemNums++] = tWeaveCardsItem;

	return true;
}


bool CLMjPlayer::execKong(unsigned char byProvideUser, const CLMjCard & cardOut, std::vector<int> vetHu, int nPlayNum/*, int nCurSelectIndex*/)
{
	int nIndex = m_tMjActInfo.tMjActKongInfo.nCurSelectIndex;
	E_KongType eKongType = m_tMjActInfo.tMjActKongInfo.arrKongType[nIndex];

	if (eKongType == EK_KongDian)
	{
		// 更新手牌
		if (m_pMjLogic->removeCards(m_arrHandCards, m_nHandNums, cardOut, 3) != 3)
		{
			return false;
		}
		m_nHandNums -= 3;

		// 更新组合牌
		T_WeaveCardsItem tWeaveCardsItem;
		tWeaveCardsItem.byWeaveKind = T_WeaveCardsItem::EW_KongDian;
		tWeaveCardsItem.cardCenter = cardOut;
		tWeaveCardsItem.cardPublic = cardOut;
		tWeaveCardsItem.byProvideUser = byProvideUser;
		for (int i = 0; i < 4; i++)
		{
			tWeaveCardsItem.aCards[i] = cardOut;
		}
		
		for (int i = 0; i < nPlayNum; ++i)
		{
			std::vector<int>::iterator it = std::find(vetHu.begin(), vetHu.end(), i);
			if (it == vetHu.end())
			{
				tWeaveCardsItem.arrNotHuUser[tWeaveCardsItem.nNotHuUserNum++] = *it;
			}
		}
		m_arrWeaveCardsItem[m_nWeaveItemNums++] = tWeaveCardsItem;
		
	}
	else if (eKongType == EK_KongAn)
	{
		//取得要杠的牌
		int byGangCard = m_tMjActInfo.tMjActKongInfo.arrKongSelect[nIndex];

		// 更新手牌
 		if (m_pMjLogic->removeCards(m_arrHandCards, m_nHandNums, byGangCard, 4) != 4)
		{
			return false;
		}
		m_nHandNums -= 4;

		// 更新组合牌
		T_WeaveCardsItem tWeaveCardsItem;
		tWeaveCardsItem.byWeaveKind = T_WeaveCardsItem::EW_KongAn;
		tWeaveCardsItem.cardCenter = byGangCard;
		tWeaveCardsItem.cardPublic = byGangCard;
		for (int i = 0; i < 4; i++)
		{
			tWeaveCardsItem.aCards[i] = byGangCard;
		}
		for (int i = 0; i < nPlayNum; ++i)
		{
			std::vector<int>::iterator it = std::find(vetHu.begin(), vetHu.end(), i);
			if (it == vetHu.end())
			{
				tWeaveCardsItem.arrNotHuUser[tWeaveCardsItem.nNotHuUserNum++] = *it;
			}
		}
		m_arrWeaveCardsItem[m_nWeaveItemNums++] = tWeaveCardsItem;
	}
	else if (eKongType == EK_KongBa)
	{
		//取得要杠的牌
		int byGangCard = m_tMjActInfo.tMjActKongInfo.arrKongSelect[nIndex];
		// 更新手牌
		if (m_pMjLogic->removeCards(m_arrHandCards, m_nHandNums, cardOut, 1) != 1)
		{
			return false;
		}
		m_nHandNums--;

		// 更新组合牌
		T_WeaveCardsItem tWeaveCardsItem;
		tWeaveCardsItem.byWeaveKind = T_WeaveCardsItem::EW_KongBa;
		tWeaveCardsItem.cardCenter = byGangCard;
		tWeaveCardsItem.cardPublic = byGangCard;
		for (int i = 0; i < 4; i++)
		{
			tWeaveCardsItem.aCards[i] = byGangCard;
		}
		for (int i = 0; i < nPlayNum; ++i)
		{
			std::vector<int>::iterator it = std::find(vetHu.begin(), vetHu.end(), i);
			if (it == vetHu.end())
			{
				tWeaveCardsItem.arrNotHuUser[tWeaveCardsItem.nNotHuUserNum++] = *it;
			}
		}
		m_arrWeaveCardsItem[m_nWeaveItemNums++] = tWeaveCardsItem;
	}

	return true;
}


bool CLMjPlayer::execHu(unsigned char byProvideUser, int nHuIndex, const CLMjCard & cardOut)
{
	m_tUserHuInfo.bHu = true;
	m_tUserHuInfo.nHuIndex = nHuIndex;
	if (byProvideUser == m_nChairID)
	{
		m_tUserHuInfo.eMjHuWay = EHW_ZiMo;
	}
	else
	{
		m_tUserHuInfo.eMjHuWay = EHW_JiePao;
		m_tUserHuInfo.byFangPaoUser = byProvideUser;

		// 加入到手牌
		m_arrHandCards[13] = cardOut;
	}
	// 转移胡牌方式
	m_tUserHuInfo.eMjHuWay = m_tMjActInfo.tMjActHuInfo.eMjHuWay;
	m_tUserHuInfo.eMjHuName = m_tMjActInfo.tMjActHuInfo.eMjHuName;
	m_tUserHuInfo.nGeng = m_tMjActInfo.tMjActHuInfo.gen;

	return true;
}


void CLMjPlayer::getWeaveCardsItems(T_WeaveCardsItem* pWeaveCardsItem, int & nWeaveItemNums)
{
	pWeaveCardsItem = m_arrWeaveCardsItem;
	nWeaveItemNums = m_nWeaveItemNums;
}


T_WeaveCardsItem & CLMjPlayer::getLatestWeaveCardsItem()
{
	return m_arrWeaveCardsItem[m_nWeaveItemNums - 1];
}


void CLMjPlayer::dealCards(const CLMjCard arrHandCards[MJ_MAX_HAND_COUNT])
{
	for (int i = 0; i < MJ_MAX_HAND_COUNT; ++i)
	{
		m_arrHandCards[i] = arrHandCards[i];
	}
}

void CLMjPlayer::getHandCards(CLMjCard* pArrHandCards, int & nHandNums)
{
	//pArrHandCards = m_arrHandCards;
	for (int i = 0; i < MJ_MAX_HAND_COUNT; ++i)
	{
		pArrHandCards[i] = m_arrHandCards[i];
	}
	nHandNums = m_nHandNums;
}

void CLMjPlayer::getHandCards(int pArrHandCards[], int & nHandNums)
{
	for (int i = 0; i < MJ_MAX_HAND_COUNT; ++i)
	{
		pArrHandCards[i] = m_arrHandCards[i];
	}
	nHandNums = m_nHandNums;
}


int CLMjPlayer::huIndex()
{
	int nIndex = 255;
	if (m_tUserHuInfo.bHu)
	{
		nIndex = m_tUserHuInfo.nHuIndex;
	}
	return nIndex;
}







//////////////////////////////////////////////////////////////////////////

bool CLMjPlayer::selectActInfo(CLMjCard cardOut, unsigned short usIgnoreFlags /*= 0*/)
{
	// 清除动作信息
	m_tMjActInfo.clear();

	if (isActive())
	{
		//自己摸牌肯定不能吃碰
		usIgnoreFlags |= E_ActionTypeFlags::EA_Eat;
		usIgnoreFlags |= E_ActionTypeFlags::EA_Pong;
	}

	if (!(usIgnoreFlags & E_ActionTypeFlags::EA_Pong))
	{
		if (isCanPong(cardOut))
		{
			m_tMjActInfo.usActFlags |= (E_ActionTypeFlags::EA_Pong | E_ActionTypeFlags::EA_Pass);

		}
	}

	if (!(usIgnoreFlags & E_ActionTypeFlags::EA_Kong))
	{
		if (isCanKong(cardOut, &m_tMjActInfo))
		{
			m_tMjActInfo.usActFlags |= (E_ActionTypeFlags::EA_Kong | E_ActionTypeFlags::EA_Pass);

		}
	}

	if (!(usIgnoreFlags & E_ActionTypeFlags::EA_hu))
	{
		if (isCanHu(cardOut))
		{
			m_tMjActInfo.usActFlags |= (E_ActionTypeFlags::EA_hu | E_ActionTypeFlags::EA_Pass);
		}
	}

	return (m_tMjActInfo.usActFlags > 0);
}


bool CLMjPlayer::isCanPong(CLMjCard cardOut)
{
	return m_pMjLogic->isCanPong(m_arrHandCards, m_nHandNums, cardOut);
}

bool CLMjPlayer::isCanKong(CLMjCard cardOut, T_MjActInfo* pActInfo)
{
	return m_pMjLogic->isCanKong(m_arrHandCards, m_nHandNums, m_arrWeaveCardsItem, m_nWeaveItemNums, cardOut, pActInfo->tMjActKongInfo);
}

bool CLMjPlayer::isCanHu(CLMjCard cardOut)
{
	return m_pMjLogic->isCanHu(m_arrHandCards, m_nHandNums, m_arrWeaveCardsItem, m_nWeaveItemNums, m_tMjActInfo.tMjActHuInfo, cardOut);
}

void CLMjPlayer::checkTing()
{
	m_pMjLogic->isCanTing(m_arrHandCards, m_nHandNums, m_arrWeaveCardsItem, m_nWeaveItemNums, m_tMjActInfo.tMjActHuInfo);
}

bool CLMjPlayer::isTing()
{
	return m_vecTing.size() > 0;
}


//////////////////////////////////////////////////////////////////////////
CLMjCard::E_MjCardColor CLMjPlayer::thinkDingQue()
{
	return (CLMjCard::E_MjCardColor)m_pIAbstractThink->thinkDingQue(m_arrHandCards, m_nHandNums);
}

void CLMjPlayer::think(CLMjCard cardOut, unsigned short usIgnoreFlags)
{
	// 出牌思考
	m_pIAbstractThink->think(&m_tMjActInfo, m_arrHandCards, m_nHandNums, m_arrWeaveCardsItem
		, m_nWeaveItemNums, cardOut, m_eColorTBA, usIgnoreFlags);
}






