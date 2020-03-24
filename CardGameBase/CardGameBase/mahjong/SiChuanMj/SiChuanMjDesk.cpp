#include "SiChuanMjDesk.h"
#include "core/LGameDispatcher.h"

CSiChuanMjDesk::CSiChuanMjDesk()
{

	_gameDispatcher = new CLGameDispatcher();
	_gameDispatcher->add(TIME_ID_ROCK_DICE, 3000, std::bind(&CSiChuanMjDesk::onEventCutCards, this));
	_gameDispatcher->add(TIME_ID_FETCH_HANDCARDS, 3000, std::bind(&CSiChuanMjDesk::onEventDealCards, this));
	_gameDispatcher->add(TIME_ID_TBA, 1000, std::bind(&CSiChuanMjDesk::onEventDingQue, this));


}


CSiChuanMjDesk::~CSiChuanMjDesk()
{
	delete _gameDispatcher;
}


void CSiChuanMjDesk::allocation()
{
	for (int i = 0; i < mjNum(); ++i)
	{
		m_arrMjCard[i] = g_arrMjCardPair[i];
	}

}


//////////////////////////////////////////////////////////////////////////
void CSiChuanMjDesk::onEventGameBegin()
{
	allocation();

	ruffle();

	initWall();

	onMsgBegin();
	_gameDispatcher->start(TIME_ID_ROCK_DICE);
}

void CSiChuanMjDesk::onEventCutCards()
{
	rockDice();

	onMsgCutCards(m_mjDicePair.dice1(), m_mjDicePair.dice2());
	_gameDispatcher->start(TIME_ID_FETCH_HANDCARDS);
}


void CSiChuanMjDesk::onEventDealCards()
{
	//static CLMjCard arrCardHand[4][14]; // 由于performFunctionInCocosThread是在另一线程，而arrCardHand始终是引用传递数据，
										//导致arrCardHand提前释放，故加入static修饰
	T_MsgDealCards tMsgDealCards;
	for (int i = 0; i < 4; i++)
	{
		m_mjLogic.emptyCards(tMsgDealCards.arrCardHand[i], 14);
	}
	dealCards(tMsgDealCards.arrCardHand);
	tMsgDealCards.nPlayerCount = playerCount();
	tMsgDealCards.nIndexStart = m_nIndexStart;
	tMsgDealCards.nIndexCurrent = m_nIndexCurrent;

	onMsgDealCards(tMsgDealCards);
	_gameDispatcher->start(TIME_ID_TBA);
}


void CSiChuanMjDesk::onEventDingQue()
{
	for (int i = 0; i < playerCount(); i++)
	{
		if (m_pArrMjPlayer[i]->isReboot())
		{
			CLMjCard::E_MjCardColor color = m_pArrMjPlayer[i]->thinkDingQue();
			m_pArrMjPlayer[i]->selectTBA(color);
		}
	}
	
	// 广播定缺结果
	onMsgDingQue();
	//_gameDispatcher->start(TIME_ID_TBA);
}

void CSiChuanMjDesk::onEventAppointActiveUser()
{
	T_MsgAppointActiveUser tMsgAppointActiveUser;
	tMsgAppointActiveUser.nChairID = m_nBanker;
	tMsgAppointActiveUser.nDrawCardValue = CARD_EMPTY;
	tMsgAppointActiveUser.nIndexStart = m_nIndexStart;
	tMsgAppointActiveUser.nIndexCurrent = m_nIndexStart;
}



//////////////////////////////////////////////////////////////////////////

void CSiChuanMjDesk::onUserEnter(int nChairID)
{
	if (m_pArrMjPlayer[nChairID]->isReboot())
	{
		onUserReady(nChairID);
	}
}


void CSiChuanMjDesk::onUserReady(int nChairID)
{
	if (m_pArrMjPlayer[nChairID]->isReady())
	{
		return;
	}
	m_pArrMjPlayer[nChairID]->ready();

	int nReadyNum = 0;
	for (int i = 0; i < playerCount(); ++i)
	{
		if (m_pArrMjPlayer[i]->isReady())
		{
			nReadyNum++;
		}
	}

	// 广播用户准备
	onMsgReady(nChairID);

	if (playerCount() == nReadyNum)
	{
		onEventGameBegin();
	}
}

void CSiChuanMjDesk::onUserTBA(int nCardColor, int nChairID)
{
	if (m_pArrMjPlayer[nChairID]->isAlreadyTBA())
	{
		return;
	}
	m_pArrMjPlayer[nChairID]->selectTBA(CLMjCard::E_MjCardColor(nCardColor));
	int nTBANum = 0;
	for (int i = 0; i < playerCount(); ++i)
	{
		if (m_pArrMjPlayer[i]->isAlreadyTBA())
		{
			nTBANum++;
		}
	}

	if (playerCount() == nTBANum)
	{
		onEventAppointActiveUser();
	}
}






