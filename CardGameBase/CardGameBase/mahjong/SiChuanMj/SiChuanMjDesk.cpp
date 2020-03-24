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
	static CLMjCard arrCardHand[4][14]; // 由于performFunctionInCocosThread是在另一线程，而arrCardHand始终是引用传递数据，
										//导致arrCardHand提前释放，故加入static修饰
	for (int i = 0; i < 4; i++)
	{
		m_mjLogic.emptyCards(arrCardHand[i], 14);
	}
	dealCards(arrCardHand);

	onMsgDealCards(arrCardHand, playerCount());
	_gameDispatcher->start(TIME_ID_TBA);
}


void CSiChuanMjDesk::onEventDingQue()
{
	for (int i = 0; i < playerCount(); i++)
	{
		if (m_arrMjPlayer[i].isReboot())
		{
			CLMjCard::E_MjCardColor color = m_arrMjPlayer[i].thinkDingQue();
			m_arrMjPlayer[i].selectTBA(color);
		}
	}
	
	// 广播定缺结果
	onMsgDingQue();
	//_gameDispatcher->start(TIME_ID_TBA);
}

void CSiChuanMjDesk::onEventAppointActiveUser()
{

}



//////////////////////////////////////////////////////////////////////////

void CSiChuanMjDesk::onUserEnter(int nChairID)
{
	if (m_arrMjPlayer[nChairID].isReboot())
	{
		m_arrMjPlayer[nChairID].ready();
	}
}


void CSiChuanMjDesk::onUserReady(int nChairID)
{
	if (m_arrMjPlayer[nChairID].isReady())
	{
		return;
	}
	m_arrMjPlayer[nChairID].ready();

	int nReadyNum = 0;
	for (int i = 0; i < playerCount(); ++i)
	{
		if (m_arrMjPlayer[i].isReady())
		{
			nReadyNum++;
		}
	}

	if (playerCount() == nReadyNum)
	{
		onEventGameBegin();
	}
}

void CSiChuanMjDesk::onUserTBA(int nCardColor, int nChairID)
{
	if (m_arrMjPlayer[nChairID].isAlreadyTBA())
	{
		return;
	}
	m_arrMjPlayer[nChairID].selectTBA(CLMjCard::E_MjCardColor(nCardColor));
	int nTBANum = 0;
	for (int i = 0; i < playerCount(); ++i)
	{
		if (m_arrMjPlayer[i].isAlreadyTBA())
		{
			nTBANum++;
		}
	}

	if (playerCount() == nTBANum)
	{
		onEventAppointActiveUser();
	}
}






