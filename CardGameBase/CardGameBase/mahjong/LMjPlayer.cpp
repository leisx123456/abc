#include "LMjPlayer.h"
#include "ai/AbstractThink.h"
#include "ai/LCmpThink.h"

CLMjPlayer::CLMjPlayer()
: m_ePlayerType(EP_People)
{
	init();
}


CLMjPlayer::CLMjPlayer(E_PlayerType ePlayerType)
: m_ePlayerType(ePlayerType)
{
	init();
}


void CLMjPlayer::init()
{
	switch (m_ePlayerType)
	{
	case CLMjPlayer::EP_People:
		m_pIAbstractThink = new CLCmpThink();
		break;
	case CLMjPlayer::EP_CmpEasy:
		m_pIAbstractThink = nullptr;
		break;
	case CLMjPlayer::EP_CmpNormal:
		m_pIAbstractThink = new CLCmpThink();
		break;
	case CLMjPlayer::EP_CmpClever:
		m_pIAbstractThink = nullptr;
		break;
	default:
		break;
	}

	//m_CardNew = CARD_EMPTY;
	
		m_bIsHu = false;

	m_nHandNums = 0;
	m_ePlayerActiveState = E_PlayerActiveState::p_unActive;
	CLMjCard m_arrOutedCards[MJ_MAX_OUTED];
	for (int i = 0; i < MJ_MAX_OUTED; i++)
	{
		m_arrOutedCards[i] = CARD_EMPTY;
	}
	m_nOutedTimes = 0;
	m_nOutedNums = 0;

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








void CLMjPlayer::removeListOutCardAtLast()
{
	m_lstCardOut.pop_back();
}



//bool CLMjPlayer::isAlreadyHu()
//{
//	return m_bIsHu;
//}



//bool CLMjPlayer::isTBA(CHandCard handCard)
//{
//	if (handCard.getONCardType() == m_eTBA)
//	{
//		return true;
//	}
//	return false;
//}


bool CLMjPlayer::outCard(const CLMjCard & card)
{
	if (!m_mjLogic.removeCard(m_arrHandCards, m_nHandNums, card))
	{
		return false;
	}
	m_nHandNums--;

		//重新排列手牌
	m_mjLogic.sortCards(m_arrHandCards, m_nHandNums);

		//向出牌表中添加该牌
	m_arrOutedCards[m_nOutedNums++] = card;

		//更新出牌计数
	m_nOutedTimes++;

	return true;
}

void CLMjPlayer::execAction(T_ActRequest tActRequest)
{
	if (EA_Pong == tActRequest.usActFlags)
	{
		//为玩家执行过操作
		//ExecuteGuoAct(bDeskStation);

	}
	else if (EA_Kong == tActRequest.usActFlags)
	{
		//为玩家执行杠操作
		//ExecuteGangAct(m_byTokenUser, byCanActUsers[0]);
	}
	else if (EA_hu == tActRequest.usActFlags)
	{
		//为玩家执行胡操作
		//ExecuteHuAct(m_byTokenUser, byCanActUsers, iAtOnceCounter);
	}

}


bool CLMjPlayer::execPong(const CLMjCard & cardOut)
{
	if (m_mjLogic.removeCards(m_arrHandCards, m_nHandNums, cardOut, 2) != 2)
	{
		return false;
	}
	m_nHandNums -= 2;

	T_WeaveCardsItem tWeaveCardsItem;
	tWeaveCardsItem.byWeaveKind = T_WeaveCardsItem::EW_Triplet;
	tWeaveCardsItem.cardCenter = cardOut;
	tWeaveCardsItem.cardPublic = cardOut;
	//tWeaveCardsItem.byProvideUser = 0;
	m_arrWeaveCardsItem[m_nWeaveItemNums++] = tWeaveCardsItem;
}



void CLMjPlayer::dealCards(const CLMjCard arrHandCards[MAX_HAND_COUNT])
{
	for (int i = 0; i < MAX_HAND_COUNT; ++i)
	{
		m_arrHandCards[i] = arrHandCards[i];
	}
}

void CLMjPlayer::getHandCards(CLMjCard* pArrHandCards)
{
	//pArrHandCards = m_arrHandCards;
	for (int i = 0; i < MAX_HAND_COUNT; ++i)
	{
		pArrHandCards[i] = m_arrHandCards[i];
	}
}









//////////////////////////////////////////////////////////////////////////

bool CLMjPlayer::selectActInfo(T_MjActInfo* pActInfo, CLMjCard cardOut, unsigned short usIgnoreFlags /*= 0*/)
{
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
			pActInfo->usActFlags |= (E_ActionTypeFlags::EA_Pong | E_ActionTypeFlags::EA_Pass);

		}
	}

	if (!(usIgnoreFlags & E_ActionTypeFlags::EA_Kong))
	{
		if (isCanKong(cardOut, pActInfo))
		{
			pActInfo->usActFlags |= (E_ActionTypeFlags::EA_Kong | E_ActionTypeFlags::EA_Pass);

		}
	}

	if (!(usIgnoreFlags & E_ActionTypeFlags::EA_hu))
	{
		if (isCanHu(cardOut))
		{
			pActInfo->usActFlags |= (E_ActionTypeFlags::EA_hu | E_ActionTypeFlags::EA_Pass);
		}
	}

	return (pActInfo->usActFlags > 0);
}


bool CLMjPlayer::isCanPong(CLMjCard cardOut)
{
	return m_mjLogic.isCanPong(m_arrHandCards, m_nHandNums, cardOut);
}

bool CLMjPlayer::isCanKong(CLMjCard cardOut, T_MjActInfo* pActInfo)
{
	return m_mjLogic.isCanKong(m_arrHandCards, m_nHandNums, m_arrWeaveCardsItem, m_nWeaveItemNums, cardOut, pActInfo->tMjActKongInfo);
}

bool CLMjPlayer::isCanHu(CLMjCard cardOut)
{
	return m_mjLogic.isCanHu(m_arrHandCards, m_nHandNums, m_arrWeaveCardsItem, m_nWeaveItemNums, cardOut);
}



//////////////////////////////////////////////////////////////////////////
CLMjCard::E_MjCardColor CLMjPlayer::thinkDingQue()
{
	return (CLMjCard::E_MjCardColor)m_pIAbstractThink->thinkDingQue(m_arrHandCards, m_nHandNums);
}

void CLMjPlayer::think(T_ActRequest* pActRequest, CLMjCard cardDest, unsigned short usIgnoreFlags)
{
	// 出牌思考
	
	m_pIAbstractThink->think(pActRequest, m_arrHandCards, m_nHandNums, m_arrWeaveCardsItem, m_nWeaveItemNums, cardDest, m_eColorTBA);
}






