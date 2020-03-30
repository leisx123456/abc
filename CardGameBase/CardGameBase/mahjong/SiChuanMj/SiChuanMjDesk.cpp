#include "SiChuanMjDesk.h"
#include "core/LGameDispatcher.h"

CSiChuanMjDesk::CSiChuanMjDesk()
{

	_gameDispatcher = new CLGameDispatcher();
	_gameDispatcher->add(TIME_ID_Ready, 1000, std::bind(&CSiChuanMjDesk::onEventReady, this));
	_gameDispatcher->add(TIME_ID_ROCK_DICE, 3000, std::bind(&CSiChuanMjDesk::onEventCutCards, this));
	_gameDispatcher->add(TIME_ID_FETCH_HANDCARDS, 3000, std::bind(&CSiChuanMjDesk::onEventDealCards, this));
	_gameDispatcher->add(TIME_ID_TBA, 1000, std::bind(&CSiChuanMjDesk::onEventDingQue, this));

	_gameDispatcher->add(TIME_ID_ROUND_FINISH, 500, std::bind(&CSiChuanMjDesk::onEventGameFinshed, this));
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
		if (i == m_nActiveUser)
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
	m_nActiveUser = m_nBanker;
	m_cardOut = CARD_EMPTY;
	updateUser();
	
	T_MsgAppointActiveUser tMsgAppointActiveUser;
	tMsgAppointActiveUser.tActiveUser.nActiveUser = m_nActiveUser;
	onMsgAppointActiveUser(tMsgAppointActiveUser);

	// 2. 获取所有可能的动作信息，没有动作则出牌(首轮为庄家自己)
	onEventJudgeAndExecActNotify(EA_BankerFirstGot);

}


void CSiChuanMjDesk::onEventDrawCard()
{
	// 1. 更新活动玩家
	m_nActiveUser = nextPlayerIndex(m_nActiveUser, playerCount(), false);
	updateUser();

	// 2. 活动用户摸牌
	if (surplusCards() == 0)
	{
		// 如果牌墙剩余为0，结束
		_gameDispatcher->start(TIME_ID_ROUND_FINISH);
		return;
	}
	CLMjCard card = drawCard();
	m_pArrMjPlayer[m_nActiveUser]->drawCard(card);
	
	// 发送消息
	T_MsgAppointActiveUser tMsgAppointActiveUser;
	tMsgAppointActiveUser.tActiveUser.nActiveUser = m_nActiveUser;
	tMsgAppointActiveUser.tActiveUser.nNewCard = card.value();
	onMsgAppointActiveUser(tMsgAppointActiveUser);


	// 2. 获取所有可能的动作信息
	onEventJudgeAndExecActNotify(EA_DarwCard);
}


// 麻将中间是一个摸牌打牌,摸牌打牌的过程，但是由于活动玩家摸牌,打牌会产生动作信息,需要系统判断是否有动作信息
// 如果非活动状态玩家有动作且执行了该动作就发生动作流转，活动状态转移到该玩家
void CSiChuanMjDesk::onEventJudgeAndExecActNotify(E_ActNotifyType eActiveNotifyType)
{
	// 1. 动作限制
	unsigned short usIgnoreFlags = 0;
	switch (eActiveNotifyType)
	{
	case CSiChuanMjDesk::EA_BankerFirstGot:
		usIgnoreFlags |= E_ActionTypeFlags::EA_Kong;	// 首轮不能杠

		usIgnoreFlags |= E_ActionTypeFlags::EA_Eat;
		usIgnoreFlags |= E_ActionTypeFlags::EA_Pong;
		break;
	case CSiChuanMjDesk::EA_DarwCard:
		//自己摸牌肯定不能吃碰
		usIgnoreFlags |= E_ActionTypeFlags::EA_Eat;
		usIgnoreFlags |= E_ActionTypeFlags::EA_Pong;
		break;
	case CSiChuanMjDesk::EA_OutCard:
		break;
	case CSiChuanMjDesk::EA_OtherAct:
		// 只能是胡
		usIgnoreFlags |= E_ActionTypeFlags::EA_Kong;
		usIgnoreFlags |= E_ActionTypeFlags::EA_Eat;
		usIgnoreFlags |= E_ActionTypeFlags::EA_Pong;
		break;
	default:
		break;
	}

	// 2. 判断是否有动作
	if (eActiveNotifyType == EA_BankerFirstGot || eActiveNotifyType == EA_DarwCard)
	{
		if (m_pArrMjPlayer[m_nActiveUser]->isReboot())
		{
			// 如果是机器人直接处理动作信息 并获取机器人的动作请求
			T_ActRequest tActRequest;
			m_pArrMjPlayer[m_nActiveUser]->think(&tActRequest, CARD_EMPTY, usIgnoreFlags);
			if (tActRequest.usActFlags)
			{
				OnUserActRequest(m_nActiveUser, tActRequest);
			}
			else
			{
				onUserOutCardRequest(m_nActiveUser, tActRequest.tMjActOutInfo);
			}
		}
		else
		{
			// 是玩家则把动作信息交给玩家自己处理(包括出牌)
			T_MjActInfo tMjActInfo;
			bool bHaveAct = m_pArrMjPlayer[m_nActiveUser]->selectActInfo(&tMjActInfo, CARD_EMPTY, usIgnoreFlags);
			if (bHaveAct)
			{
				onMsgActNotify(tMjActInfo);
			}

		}
	}
	else if (eActiveNotifyType == EA_OutCard)
	{
		for (int i = 0; i < playerCount(); ++i)
		{
			if (i == m_nActiveUser)
			{
				continue;
			}
			if (m_pArrMjPlayer[i]->isReboot())
			{
				// 如果是机器人直接处理动作信息 并获取机器人的动作请求
				T_ActRequest tActRequest;
				m_pArrMjPlayer[m_nActiveUser]->think(&tActRequest, CARD_EMPTY, usIgnoreFlags);
				if (tActRequest.usActFlags)
				{
					OnUserActRequest(m_nActiveUser, tActRequest);
				}
				else
				{
					onUserOutCardRequest(m_nActiveUser, tActRequest.tMjActOutInfo);
				}
			}
			else
			{
				
				T_MjActInfo tMjActInfo;
				bool bHaveAct = m_pArrMjPlayer[i]->selectActInfo(&tMjActInfo, m_cardOut, 0);
				if (bHaveAct)
				{
					//动作流转向////////////////////////////////////////////////////////////////////////////////////////////
					//即可能会断流，活动状态流转到执行动作的玩家
					onMsgActNotify(tMjActInfo);
				}
				else
				{
					onEventDrawCard();
				}

			}


		}

	}
	else if (eActiveNotifyType == EA_OtherAct)
	{
		// 目前只有抢杠
	}



}



void CSiChuanMjDesk::onEventGameFinshed()
{

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

void CSiChuanMjDesk::onUserTBA(int nChairID, int nCardColor)
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

	// 需要等待所有人定缺完成
	if (playerCount() == nTBANum)
	{
		// 广播定缺结果
		T_MsgDingQue tMsgDingQue;
		for (int i = 0; i < playerCount(); ++i)
		{
			tMsgDingQue.arrCardColor[i] = m_pArrMjPlayer[i]->getDingQueColor();
		}
		onMsgDingQue(tMsgDingQue);
		
		// 指定活动玩家为庄家
		onEventFristBankerActive();
	}
}

void CSiChuanMjDesk::onUserOutCardRequest(int nChairID, T_MjActOutInfo tMjActOutInfo)
{
	// 执行动作
	m_pArrMjPlayer[nChairID]->outCard(tMjActOutInfo.nOutCardValue);
	m_cardOut = tMjActOutInfo.nOutCardValue;

	// 广播出牌信息
	T_MsgOutCard tMsgOutCard;
	tMsgOutCard.nOutCardUser = tMjActOutInfo.nOutCardUser;
	tMsgOutCard.nOutCardValue = tMjActOutInfo.nOutCardValue;
	m_pArrMjPlayer[nChairID]->getOutCardsValue(tMsgOutCard.arrOutCard, tMsgOutCard.nOutedNums);
	onMsgOutCard(tMsgOutCard);

	// 玩家出的牌可能引起其他玩家有动作
	onEventJudgeAndExecActNotify(EA_OutCard);
	

}

void CSiChuanMjDesk::OnUserActRequest(int nChairID, T_ActRequest tActRequest)
{
	// 执行动作
	m_pArrMjPlayer[nChairID]->execAction(tActRequest);
	m_nActiveUser = nChairID;
	// 如果是一炮多项, 活动状态交给点炮者

	// 广播动作信息
	T_MsgActResultInfo tMsgActResultInfo;
	onMsgActResult(tMsgActResultInfo);

	// 玩家的动作也可能引起其他玩家有动作，如补杠别人会抢杠
	onEventJudgeAndExecActNotify(EA_OtherAct);
}









