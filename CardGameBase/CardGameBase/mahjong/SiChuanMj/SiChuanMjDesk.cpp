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





void CSiChuanMjDesk::updateUser()
{
	for (int i = 0; i < playerCount(); i++)
	{
		if (i == m_tActiveUser.nActiveUser)
		{
			m_pArrMjPlayer[i]->setActive(CLMjPlayer::P_Active);
		}
		else
		{
			m_pArrMjPlayer[i]->cancelActive();
		}
	}
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


// 庄家首轮出牌, 一局只调用一次
void CSiChuanMjDesk::onEventFristBankerActive()
{
	// 1. 指定活动玩家
	m_tActiveUser.nActiveUser = m_nBanker;
	m_tActiveUser.eActiveUserType = EA_FristGot;
	T_MsgAppointActiveUser tMsgAppointActiveUser;
	tMsgAppointActiveUser.tActiveUser = m_tActiveUser;
	onMsgAppointActiveUser(tMsgAppointActiveUser);

	// 2. 获取所有可能的动作信息，没有动作则出牌(首轮为庄家自己)
	unsigned short usIgnoreFlags = 0;
	usIgnoreFlags |= E_ActionTypeFlags::EA_Kong;	// 首轮不能杠

	if (m_pArrMjPlayer[m_nBanker]->isReboot())
	{
		// 如果是机器人直接处理动作信息 并获取机器人的动作请求
		T_ActRequest tActRequest;
		m_pArrMjPlayer[m_nBanker]->think(&tActRequest, CARD_EMPTY, usIgnoreFlags);
		if (tActRequest.usActFlags)
		{
			OnUserActRequest(m_nBanker, tActRequest);
		}
		else
		{
			onUserOutCard(m_nBanker, tActRequest.tMjActOutInfo);
		}
	}
	else
	{
		// 是玩家则把动作信息交给玩家自己处理
		T_MjActInfo tMjActInfo;
		bool bHaveAct = m_pArrMjPlayer[m_nBanker]->selectActInfo(&tMsgAppointActiveUser.tMjActInfo, CARD_EMPTY, usIgnoreFlags);
		if (bHaveAct)
		{
			onMsgActNotify(tMjActInfo);
		}

	}

}



void CSiChuanMjDesk::onEventDrawCard()
{
	if (surplusCards() == 0)
	{
		// 如果牌墙剩余为0，结束
		return;
	}
	CLMjCard card = drawCard();
	m_pArrMjPlayer[m_tActiveUser.nActiveUser]->drawCard(card);
	m_tActiveUser.nNewCard = card.value();

	// 1. 指定活动玩家
	m_tActiveUser.nActiveUser = m_nBanker;
	m_tActiveUser.eActiveUserType = EA_FristGot;
	T_MsgAppointActiveUser tMsgAppointActiveUser;
	tMsgAppointActiveUser.tActiveUser = m_tActiveUser;
	onMsgAppointActiveUser(tMsgAppointActiveUser);

	// 2. 获取所有可能的动作信息，没有动作则出牌(首轮为庄家自己)
	unsigned short usIgnoreFlags = 0;
	usIgnoreFlags |= E_ActionTypeFlags::EA_Kong;	// 首轮不能杠

	if (m_pArrMjPlayer[m_nBanker]->isReboot())
	{
		// 如果是机器人直接处理动作信息 并获取机器人的动作请求
		T_ActRequest tActRequest;
		m_pArrMjPlayer[m_nBanker]->think(&tActRequest, CARD_EMPTY, usIgnoreFlags);
		if (tActRequest.usActFlags)
		{
			OnUserActRequest(m_nBanker, tActRequest);
		}
		else
		{
			onUserOutCard(m_nBanker, tActRequest.tMjActOutInfo);
		}
	}
	else
	{
		// 是玩家则把动作信息交给玩家自己处理
		T_MjActInfo tMjActInfo;
		bool bHaveAct = m_pArrMjPlayer[m_nBanker]->selectActInfo(&tMsgAppointActiveUser.tMjActInfo, CARD_EMPTY, usIgnoreFlags);
		if (bHaveAct)
		{
			onMsgActNotify(tMjActInfo);
		}

	}
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


void CSiChuanMjDesk::onEventResponseToActiveUser()
{
	int nResponseUser;
	if (m_tActiveUser.eActiveUserType == EA_FristGot)
	{
		nResponseUser = m_tActiveUser.nActiveUser;
		
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

		nResponseUser = m_tActiveUser.nActiveUser;
	}
	else
	{
		// 自动为电脑选择吃碰杠胡
		for (int i = 0; i < playerCount(); i++)
		{
			if (m_pArrMjPlayer[i]->isReboot())
			{
				// 如果是机器人直接处理动作信息 并获取机器人的动作请求
				T_ActRequest tActRequest;
				m_pArrMjPlayer[i]->think(&tActRequest, m_cardOut, 0);
				if (tActRequest.usActFlags)
				{
					OnUserActRequest(i, tActRequest);
				}
				else
				{
					onUserOutCard(i, tActRequest.tMjActOutInfo);
				}
			}
			else
			{
				// 当且仅当该玩家有动作信息时向该玩家发送动作通知，交给该玩家处理
				T_MjActInfo tMjActInfo;
				bool bHaveAct = m_pArrMjPlayer[i]->selectActInfo(&tMjActInfo, CARD_EMPTY, 0);
				if (bHaveAct)
				{
					onMsgActNotify(tMjActInfo);
				}
				
			}
		}
	}



}




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
		
		m_nActiveUser = m_nBanker;
		m_cardOut = CARD_EMPTY;
		updateUser();

		// 需要等待所有人定缺完成
		//onEventAppointActiveUser(); // 要不要吧他变成出牌
		onEventFristBankerActive();
	}
}

void CSiChuanMjDesk::onUserOutCard(int nChairID, T_MjActOutInfo tMjActOutInfo)
{
	// 执行动作并广播出牌信息
	m_pArrMjPlayer[nChairID]->outCard(tMjActOutInfo.nOutCardValue);
	m_cardOut = tMjActOutInfo.nOutCardValue;
	onMsgOutCard(tMjActOutInfo);

	// 玩家出的牌可能引起其他玩家有动作，先判断是否其他玩家具备动作
	//计算各玩家因bDeskStation打出的牌而产生的动作信息\ 产生的动作信息将保存在玩家i的数据m_tagActInfo中
	bool bHaveAct = false;
	for (int i = 0; i < playerCount(); ++i)
	{
		if (i != m_nActiveUser) //非出牌都才可能有动作
		{
			if (m_pArrMjPlayer[i]->selectActInfo(&tMjActInfo, m_cardOut, 0))
			{
				onMsgActNotify(tMjActInfo);
				bHaveAct = true;
			}
		}
	}

	//动作流转向////////////////////////////////////////////////////////////////////////////////////////////

	//玩家出牌完成,情况如下：\
			1.如果没有人有动作,系统为下个玩家赋令牌\
			2.如有动作，即断流等待玩家选择动作。需要有流程保护

	if (!bHaveAct)
	{
		//m_tagParam.bCanFetch = true;
		//m_tagParam.byNextUser = 255;
		//SetTimer(TIME_GIVE_NEXT_TOKEN, 500);
		onEventDrawCard();
	}

}

void CSiChuanMjDesk::OnUserActRequest(int nChairID, T_ActRequest tActRequest)
{
	m_pArrMjPlayer[nChairID]->execAction(tActRequest);

	T_MsgActResultInfo tMsgActResultInfo;
	onMsgActResult(tMsgActResultInfo);

	m_nActiveUser = nChairID;
}








