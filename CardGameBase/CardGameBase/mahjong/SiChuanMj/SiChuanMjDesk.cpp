#include "SiChuanMjDesk.h"
#include "core/LGameDispatcher.h"

CSiChuanMjDesk::CSiChuanMjDesk()
{
	
	_gameDispatcher = new CLGameDispatcher();
	_gameDispatcher->add(TIME_ID_Ready, 1000, std::bind(&CSiChuanMjDesk::onEventReady, this));
	_gameDispatcher->add(TIME_ID_ROCK_DICE, 3000, std::bind(&CSiChuanMjDesk::onEventCutCards, this));
	_gameDispatcher->add(TIME_ID_FETCH_HANDCARDS, 3000, std::bind(&CSiChuanMjDesk::onEventDealCards, this));
	_gameDispatcher->add(TIME_ID_TBA, 1000, std::bind(&CSiChuanMjDesk::onEventDingQue, this));

	// 1秒后自动准备
	_gameDispatcher->start(TIME_ID_Ready);
}


CSiChuanMjDesk::~CSiChuanMjDesk()
{
	delete _gameDispatcher;
}


void CSiChuanMjDesk::allocation()
{
	for (int i = 0; i < mjNumAllocation(); ++i)
	{
		m_arrMjCardsPair[i] = g_arrMjCardPair[i];
	}

}


bool CSiChuanMjDesk::selectActInfo(int nResponseUser, int nActiveUser, T_MjActInfo* pActInfo, CLMjCard cardDest, unsigned short usIgnoreFlags /*= 0*/)
{
	CLMjPlayer* pPlayerResponse = m_pArrMjPlayer[nResponseUser];
	CLMjPlayer* pPlayerActive = m_pArrMjPlayer[nActiveUser];
	// 1. 如果cardDest是空牌，表示庄家首轮出牌
	if (!cardDest.isValid())
	{
		//自己摸牌肯定不能吃碰
		usIgnoreFlags |= E_ActionTypeFlags::EA_Eat;
		usIgnoreFlags |= E_ActionTypeFlags::EA_Pong;
		// 首轮不能杠
		usIgnoreFlags |= E_ActionTypeFlags::EA_Kong;
	}
	
	
	//2.如果nResponseUser == nActiveUser说明是摸牌，cardDest代表摸的牌
	if (nResponseUser == nActiveUser)
	{
		//自己摸牌肯定不能吃碰
		usIgnoreFlags |= E_ActionTypeFlags::EA_Eat;
		usIgnoreFlags |= E_ActionTypeFlags::EA_Pong;
	}
	else 
	{
		//3.如果nResponseUser != nActiveUser说明是别人正在出牌，cardDest代表出的牌
	}

	//
	if (!(usIgnoreFlags & E_ActionTypeFlags::EA_Pong))
	{
		if (pPlayerResponse->isCanPong())
		{
			pActInfo->usActFlags |= (E_ActionTypeFlags::EA_Pong | E_ActionTypeFlags::EA_Pass);

		}
	}

	if (!(usIgnoreFlags & E_ActionTypeFlags::EA_Kong))
	{
		if (pPlayerResponse->isCanKong())
		{
			pActInfo->usActFlags |= (E_ActionTypeFlags::EA_Kong | E_ActionTypeFlags::EA_Pass);

		}
	}

	if (!(usIgnoreFlags & E_ActionTypeFlags::EA_hu))
	{
		if (pPlayerResponse->isCanHu())
			{
				pActInfo->usActFlags |= (E_ActionTypeFlags::EA_hu | E_ActionTypeFlags::EA_Pass);
			}
	}

	return (pActInfo->usActFlags > 0);
}



//////////////////////////////////////////////////////////////////////////

void CSiChuanMjDesk::onEventReady()
{
	for (int i = 0; i < playerCount(); i++)
	{
		if (m_pArrMjPlayer[i]->isReboot())
		{
			onUserReady(i);
		}
	}
}


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
	memset(&tMsgDealCards, 0, sizeof(T_MsgDealCards));

	CLMjCard arrCardHand[4][14];
	dealCards(arrCardHand);

	tMsgDealCards.nPlayerCount = playerCount();
	tMsgDealCards.nMjNumAllocation = mjNumAllocation();
	for (int i = 0; i < 4; ++i)
	{
		m_mjLogic.copyCards(tMsgDealCards.arrCardHand[i], 14, arrCardHand[i], 14);
	}
	m_mjLogic.copyCards(tMsgDealCards.arrMjCardsPair, GAME_MJ_CARD_COUNT_MAX, m_arrMjCardsPair, GAME_MJ_CARD_COUNT_MAX);
	
	onMsgDealCards(tMsgDealCards);
	_gameDispatcher->start(TIME_ID_TBA);
}


void CSiChuanMjDesk::onEventDingQue()
{
	onMsgDingQueBegin();
	// 是否设置时间自动进行下一事件
	//_gameDispatcher->start(TIME_ID_TBA);

	// 自动为电脑选择结果
	for (int i = 0; i < playerCount(); i++)
	{
		if (m_pArrMjPlayer[i]->isReboot())
		{
			CLMjCard::E_MjCardColor color = m_pArrMjPlayer[i]->thinkDingQue();
			m_pArrMjPlayer[i]->selectTBA(color);
			// ai
			onUserTBA(i, color);
		}
	}
}


// 提出来 因为一局只调用一次
void CSiChuanMjDesk::onEventFristGotActiveUser()
{
	T_MjActInfo arrMjActInfo;
	CLMjCard cardOut;
		// 
		if (m_pArrMjPlayer[m_nBanker]->isReboot())
		{
			m_pArrMjPlayer[m_nBanker]->think(&arrMjActInfo, CARD_EMPTY, cardOut);
		}
		else
		{
			m_tActiveUser.nActiveUser = m_nBanker;
			m_tActiveUser.eActiveUserType = EA_FristGot;
			T_MsgAppointActiveUser tMsgAppointActiveUser;
			tMsgAppointActiveUser.tActiveUser = m_tActiveUser;
			
			bool bHaveAct = selectActInfo(m_tActiveUser.nActiveUser, m_tActiveUser.nActiveUser, &tMsgAppointActiveUser.tMjActInfo, CARD_EMPTY);
			//onMsgAppointActiveUser(tMsgAppointActiveUser)
		}
		
}



void CSiChuanMjDesk::onEventAppointDrawCardUser()
{

}


void CSiChuanMjDesk::onEventAppointActiveUser()
{

	if (m_tActiveUser.eActiveUserType == EA_FristGot)
	{
		// nothing to do
	}
	else if (m_tActiveUser.eActiveUserType == EA_Inherit)
	{
		if (surplusCards() == 0)
		{
			// 如果牌墙剩余为0，结束
			return;
		}
		CLMjCard card = drawCard();
		m_pArrMjPlayer[m_tActiveUser.nActiveUser]->drawCard(card);
		m_tActiveUser.nNewCard = card.value();
	}
	else
	{

	}
	
	T_MsgAppointActiveUser tMsgAppointActiveUser;
	tMsgAppointActiveUser.tActiveUser = m_tActiveUser;
	tMsgAppointActiveUser.nMjNumAllocation = mjNumAllocation();
	m_mjLogic.copyCards(tMsgAppointActiveUser.arrMjCardsPair, GAME_MJ_CARD_COUNT_MAX, m_arrMjCardsPair, GAME_MJ_CARD_COUNT_MAX);

	onMsgAppointActiveUser(tMsgAppointActiveUser);
	//onEventResponseToActiveUser();
	
}


//void CSiChuanMjDesk::onEventResponseToActiveUser()
//{
//	int nResponseUser;
//	if (m_tActiveUser.eActiveUserType == EA_FristGot)
//	{
//		nResponseUser = m_tActiveUser.nActiveUser;
//		
//	}
//	else if (m_tActiveUser.eActiveUserType == EA_Inherit)
//	{
//		if (surplusCards() == 0)
//		{
//			// 如果牌墙剩余为0，结束
//			return;
//		}
//		CLMjCard card = drawCard();
//		m_pArrMjPlayer[m_tActiveUser.nActiveUser]->drawCard(card);
//		m_tActiveUser.nNewCard = card.value();
//
//		nResponseUser = m_tActiveUser.nActiveUser;
//	}
//	else
//	{
//
//	}
//
//	T_MjActInfo arrMjActInfo[4];
//	T_MjActInfo* pMjActInfo = &arrMjActInfo[nResponseUser];
//	bool bHaveAct = selectActInfo(nResponseUser, m_tActiveUser.nActiveUser, pMjActInfo, )
//	
//	// 自动为电脑选择吃碰杠胡
//	for (int i = 0; i < playerCount(); i++)
//	{
//		if (m_pArrMjPlayer[i]->isReboot())
//		{
//
//		}
//	}
//}




//////////////////////////////////////////////////////////////////////////
// onUser里不能有onUser
//void CSiChuanMjDesk::onUserEnter(int nChairID)
//{
//
//}


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

	// 广播用户定缺结果
	onMsgDingQue();

	if (playerCount() == nTBANum)
	{

		//m_tActiveUser.nActiveUser = m_nBanker;
		//m_tActiveUser.eActiveUserType = EA_FristGot;

		// 需要等待所有人定缺完成
		//onEventAppointActiveUser(); // 要不要吧他变成出牌
		onEventFristGotActiveUser();
	}
}








