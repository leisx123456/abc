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

	m_nDrawTimes = 0;	//抓牌次数

	m_bReady = false;
	//E_PlayerStatus m_ePlayerStatus;
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



void CLMjPlayer::setOtherOutCard(int nCard)
{
	m_nOtherOutCard = nCard;
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


int CLMjPlayer::outCard(int nPlace)
{
	//if (nPlace == 14)
	//{
	//	m_lstCardOut.push_back(m_CardNew);
	//}
	//else
	//{
	//	m_lstCardOut.push_back(m_arrHandCard[nPlace].getCard());
	//	m_arrHandCard[nPlace].setCard(m_CardNew);
	//}
	//sortCards(m_nStartCard);
	//m_CardNew = -1;
	//m_nOtherOutCard = -1;
	//m_ePlayerActiveState = E_PlayerActiveState::p_unActive;
	return m_lstCardOut.back();
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




void CLMjPlayer::selectTBA(CLMjCard::E_MjCardColor eColorTBA)
{
	m_eColorTBA = eColorTBA;
}





//////////////////////////////////////////////////////////////////////////
bool CLMjPlayer::isCanPong()
{
	return m_mjLogic.isCanPong(m_arrHandCards, m_nHandNums, CARD_EMPTY);
}

bool CLMjPlayer::isCanKong()
{
	return m_mjLogic.isCanDianKong(m_arrHandCards, m_nHandNums, m_arrWeaveCardsItem, m_nWeaveItemNums, CARD_EMPTY);
}

bool CLMjPlayer::isCanHu()
{
	return m_mjLogic.isCanHu(m_arrHandCards, m_nHandNums, m_arrWeaveCardsItem, m_nWeaveItemNums);
}



//////////////////////////////////////////////////////////////////////////
CLMjCard::E_MjCardColor CLMjPlayer::thinkDingQue()
{
	return (CLMjCard::E_MjCardColor)m_pIAbstractThink->thinkDingQue(m_arrHandCards, m_nHandNums);
}

void CLMjPlayer::think(T_MjActInfo* pActInfo, CLMjCard cardDest)
{
	// 出牌思考
	if (isActive())
	{
		m_pIAbstractThink->think(m_arrHandCards, m_nHandNums, m_arrWeaveCardsItem, m_nWeaveItemNums, cardDest, m_eColorTBA);
	}
	else
	{
		
	}
}




