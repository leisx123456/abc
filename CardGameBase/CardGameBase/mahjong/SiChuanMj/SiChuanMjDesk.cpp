#include "SiChuanMjDesk.h"
#include "core/LGameDispatcher.h"
#include "SiChuanMjPlayer.h"

CSiChuanMjDesk::CSiChuanMjDesk()
{
	m_pArrMjPlayer[0] = new CSiChuanMjPlayer(CLMjPlayer::EP_People, 0);
	m_pArrMjPlayer[1] = new CSiChuanMjPlayer(CLMjPlayer::EP_CmpNormal, 1);
	m_pArrMjPlayer[2] = new CSiChuanMjPlayer(CLMjPlayer::EP_CmpNormal, 2);
	m_pArrMjPlayer[3] = new CSiChuanMjPlayer(CLMjPlayer::EP_CmpNormal, 3);

	_gameDispatcher = new CLGameDispatcher();
	_gameDispatcher->add(TIME_ID_Ready, 1000, std::bind(&CSiChuanMjDesk::onEventReady, this));
	_gameDispatcher->add(TIME_ID_Begin, 500, std::bind(&CSiChuanMjDesk::onEventGameBegin, this));
	_gameDispatcher->add(TIME_ID_ROCK_DICE, 3000, std::bind(&CSiChuanMjDesk::onEventCutCards, this));
	_gameDispatcher->add(TIME_ID_FETCH_HANDCARDS, 3000, std::bind(&CSiChuanMjDesk::onEventDealCards, this));
	_gameDispatcher->add(TIME_ID_TBA, 1000, std::bind(&CSiChuanMjDesk::onEventDingQue, this));
	_gameDispatcher->add(TIME_DRAW_CARD, 1000, std::bind(&CSiChuanMjDesk::onEventDrawCard, this));
	_gameDispatcher->add(TIME_ID_CHA_JIAO, 500, std::bind(&CSiChuanMjDesk::onEventChaJiao, this));
	_gameDispatcher->add(TIME_ID_ROUND_FINISH, 500, std::bind(&CSiChuanMjDesk::onEventGameFinshed, this));
	// 电脑AI延迟处理
	_gameDispatcher->addDelayFun(TIME_ID_COMPUTER_THINK_ACT, 1000, std::bind(&CSiChuanMjDesk::onDelayExecActRequest, this, std::placeholders::_1));
	_gameDispatcher->addDelayFun(TIME_ID_COMPUTER_THINK_OUT_CARD, 1000, std::bind(&CSiChuanMjDesk::onDelayExecOutCardRequest, this, std::placeholders::_1));
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

// 清除各玩家的动作信息
void CSiChuanMjDesk::clearAllUserActInfo()
{
	for (int i = 0; i < playerCount(); ++i)
	{
		m_pArrMjPlayer[i]->actInfo()->clear();
	}
}

// 获取优先级最高可立刻执行的玩家
int CSiChuanMjDesk::selectActAtOnceUsers(int byUsers[], unsigned short & usActFlag)
{
	//定义未申请动作的玩家中最大动作玩家，及动作
	unsigned short usMax1 = 0;
	int byMax1 = 255;

	//定义已申请动作的玩家中最大动作玩家，及动作
	unsigned short usMaxWaitFlags = 0;
	int nMaxWaitUser = 255;

	for (int i = 0; i < playerCount(); ++i)
	{
		unsigned short usFlags = m_pArrMjPlayer[i]->actInfo()->usActFlags;

		if (usFlags & 0x0003) //有动作的玩家00000011
		{
			unsigned short usTemp = usFlags & 0xFFFE;//11111110 // 去除等待状态后的权位

			if (usFlags & 0x0001)
			{
				if (usTemp > usMaxWaitFlags)
				{
					usMaxWaitFlags = usTemp;
					nMaxWaitUser = i;
				}
			}
			else
			{
				if (usTemp > usMax1)
				{
					usMax1 = usTemp;
					byMax1 = i;
				}
			}
		}
	}

	int iCounter = 0;


	// 通炮所有能胡的选择完以后 弹出结算界面(点过的人不让胡), 暂时不考虑一炮单响
	if (usMaxWaitFlags > usMax1)
	{
		for (int i = 0; i < playerCount(); ++i)
		{
			unsigned short usFlags = m_pArrMjPlayer[i]->actInfo()->usActFlags;

			if ((usFlags & 0x0001) && (usFlags & usMaxWaitFlags))
			{
				byUsers[iCounter++] = i;
			}
		}
		usActFlag = usMaxWaitFlags;
	}

	return iCounter;
}

bool CSiChuanMjDesk::execActPass(int nFromUser)
{
	//看看这次有没有过掉抢杠
	bool bGuoQGang = false;
	if (EHW_QiangGang == m_pArrMjPlayer[nFromUser]->actInfo()->tMjActHuInfo.eMjHuWay)
	{
		bGuoQGang = true;
	}

	//各玩家的动作信息可清除///////////
	clearAllUserActInfo();

	if (bGuoQGang)
	{
		_gameDispatcher->start(TIME_DRAW_CARD);
		return true;
	}

	// 下一个活动用户摸牌
	if (nFromUser != m_nActiveUser)
	{
		m_nActiveUser = nextPlayerIndex(m_nActiveUser, playerCount(), false);
		onSysAppointActiveUser(m_nActiveUser);
		_gameDispatcher->start(TIME_DRAW_CARD);
	}
	return true;
}

bool CSiChuanMjDesk::execActPong(int nFromUser, int nToUser)
{
	m_pArrMjPlayer[nFromUser]->removeLatestOutCard();
	m_pArrMjPlayer[nToUser]->execPong(nFromUser, m_cardOut);

	//向各玩家广播动作消息////////////////////////////////////////////////
	T_MsgActResultInfo tMsgActResultInfo;
	tMsgActResultInfo.usActFlags = EA_Pong;
	tMsgActResultInfo.byFromUser = nFromUser;
	tMsgActResultInfo.arrActUsers[0] = nToUser;
	tMsgActResultInfo.nActUserNums = 1;
	tMsgActResultInfo.nFromOutCardNums = m_pArrMjPlayer[nFromUser]->outCardNums();
	tMsgActResultInfo.tWeaveCardsValueItem = m_pArrMjPlayer[nToUser]->getLatestWeaveCardsItem();
	tMsgActResultInfo.nWeaveCardsItemNum = m_pArrMjPlayer[nToUser]->weaveItemNums();
	//更新玩家的手牌数据
	 m_pArrMjPlayer[nToUser]->getHandCards(tMsgActResultInfo.byHands, tMsgActResultInfo.iHandNums);
	 onMsgActResult(tMsgActResultInfo);

	 clearAllUserActInfo();

	// 指定活动玩家
	onSysAppointActiveUser(nToUser);

	if (m_pArrMjPlayer[nToUser]->isReboot())
	{
		// 如果是机器人思考出牌
		unsigned int usIgnoreFlags = ~0;
		m_pArrMjPlayer[m_nActiveUser]->think(CARD_EMPTY, usIgnoreFlags);
		_gameDispatcher->delayExec(TIME_ID_COMPUTER_THINK_OUT_CARD, m_nActiveUser);
	}
	return true;
}

bool CSiChuanMjDesk::execActKong(int nFromUser, int nToUser)
{
	m_pArrMjPlayer[nFromUser]->removeLatestOutCard();
	m_pArrMjPlayer[nToUser]->execKong(nFromUser, m_cardOut);

	// 向各玩家广播动作消息/////////////////////////////////////////////////////////////////////////////////////////////////////////
	T_MsgActResultInfo tMsgActResultInfo;
	tMsgActResultInfo.usActFlags = EA_Kong;
	tMsgActResultInfo.byFromUser = nFromUser;
	tMsgActResultInfo.arrActUsers[0] = nToUser;
	tMsgActResultInfo.nActUserNums = 1;
	tMsgActResultInfo.nFromOutCardNums = m_pArrMjPlayer[nFromUser]->outCardNums();
	tMsgActResultInfo.tWeaveCardsValueItem = m_pArrMjPlayer[nToUser]->getLatestWeaveCardsItem();
	tMsgActResultInfo.nWeaveCardsItemNum = m_pArrMjPlayer[nToUser]->weaveItemNums();
	// 更新玩家的手牌数据
	m_pArrMjPlayer[nToUser]->getHandCards(tMsgActResultInfo.byHands, tMsgActResultInfo.iHandNums);
	onMsgActResult(tMsgActResultInfo);
	
	// 各玩家本轮的动作信息可清除///////////////////////////////
	clearAllUserActInfo();

	//玩家补杠的牌可能引起其它玩家胡牌/////////////////////////////////////////////////////////////////////////////////////////////////////////
	onSysJudgeAndExecActNotify(EA_OtherAct);

	// 提前记录下轮的动作信息,不管m_nCurExecActUser能否胡，先把其特殊胡牌置为杠上开花，下轮再确定是否转到T_UserHuInfo中
	T_WeaveCardsItem::E_WeaveCardsType eWeaveCardsType = (T_WeaveCardsItem::E_WeaveCardsType)m_pArrMjPlayer[m_nCurExecActUser]->getLatestWeaveCardsItem().byWeaveKind;
	if (eWeaveCardsType == T_WeaveCardsItem::EW_KongAn)
	{
		m_pArrMjPlayer[m_nCurExecActUser]->actInfo()->tMjActHuInfo.eMjHuWay = EHW_AnGangKai;
	}
	else if (eWeaveCardsType == T_WeaveCardsItem::EW_KongDian)
	{
		m_pArrMjPlayer[m_nCurExecActUser]->actInfo()->tMjActHuInfo.eMjHuWay = EHW_DianGangKai;
	}
	else if (eWeaveCardsType == T_WeaveCardsItem::EW_KongBa)
	{
		m_pArrMjPlayer[m_nCurExecActUser]->actInfo()->tMjActHuInfo.eMjHuWay = EHW_BuGangKai;
	}

	return true;
}

bool CSiChuanMjDesk::execActHu(int nFromUser, int arrToUser[], int nUserNums)
{
	for (int i = 0; i < nUserNums; ++i)
	{
		m_pArrMjPlayer[arrToUser[i]]->execHu(nFromUser, m_nHuOrderNum, m_cardOut);
		_vecHu.push_back(arrToUser[i]);
	}
	m_nHuOrderNum++;

	//向各玩家广播动作消息///////////////////////////////////////////////////////////////////////////
	T_MsgActResultInfo tMsgActResultInfo;
	tMsgActResultInfo.usActFlags = EA_hu;
	tMsgActResultInfo.byFromUser = nFromUser;
	memcpy(tMsgActResultInfo.arrActUsers, arrToUser, sizeof(arrToUser));
	tMsgActResultInfo.nActUserNums = nUserNums;
	tMsgActResultInfo.nFromOutCardNums = m_pArrMjPlayer[nFromUser]->outCardNums();
	// 更新玩家的手牌数据
	for (int i = 0; i < nUserNums; ++i)
	{
		m_pArrMjPlayer[arrToUser[i]]->getHandCards(tMsgActResultInfo.byHands, tMsgActResultInfo.iHandNums);
	}
	onMsgActResult(tMsgActResultInfo);

	//各玩家的动作信息可清除///////////////////////////////
	clearAllUserActInfo();

	// 如果只剩一家未胡则本局结束
	if (_vecHu.size() >= playerCount() - 1)
	{
		_gameDispatcher->start(TIME_ID_ROUND_FINISH);
		return true;
	}

	// 如果是一炮多项, 活动状态交给点炮者
	if (nUserNums > 1)
	{
		m_nActiveUser = nFromUser;
	}
	else
	{
		m_nActiveUser = nextPlayerIndex(arrToUser[0], playerCount(), false);
	}
	onSysAppointActiveUser(m_nActiveUser);
	_gameDispatcher->start(TIME_DRAW_CARD);

	return true;
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
	for (int i = 0; i < playerCount(); ++i)
	{
		m_mjLogic.copyCards(tMsgDealCards.arrCardHand[i], 14, arrCardHand[i], 14);
	}
	m_mjLogic.copyCards(tMsgDealCards.arrMjCardsPair, MJ_MAX_CARD_COUNT, m_arrMjCardsPair, MJ_MAX_CARD_COUNT);

	onMsgDealCards(tMsgDealCards);

	// 理牌
	for (int i = 0; i < playerCount(); i++)
	{
		m_pArrMjPlayer[i]->sordCards();
	}

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
	onSysAppointActiveUser(m_nBanker);

	// 2. 获取所有可能的动作信息，没有动作则出牌(首轮为庄家自己)
	onSysJudgeAndExecActNotify(EA_BankerFirstGot);

}


void CSiChuanMjDesk::onEventDrawCard()
{
	// 1. 活动用户摸牌
	if (surplusCards() == 0)
	{
		// 如果牌墙剩余为0，荒庄，查大叫
		_gameDispatcher->start(TIME_ID_CHA_JIAO);
		return;
	}
	CLMjCard card = drawCard();
	m_pArrMjPlayer[m_nActiveUser]->drawCard(card);
	T_MsgDrawCard tMsgDrawCard;
	tMsgDrawCard.nDrawCardUser = m_nActiveUser;
	tMsgDrawCard.nNewCard = card;
	tMsgDrawCard.nIndexCurrent = m_nIndexCurrent;
	tMsgDrawCard.nIndexStart = m_nIndexStart;
	onMsgDrawCard(tMsgDrawCard);

	// 2. 获取所有可能的动作信息
	onSysJudgeAndExecActNotify(EA_DarwCard);
}


void CSiChuanMjDesk::onEventChaJiao()
{
	m_bHuangZhuang = true;
	// 查叫用户需计算最大牌型,其实就是补一张癞子牌，然后得到的牌型分中取最大的一个
}

void CSiChuanMjDesk::onEventGameFinshed()
{
	T_MsgResult tMsgResult;
	tMsgResult.bHuangZhuang = m_bHuangZhuang;

	// 给消息结构体赋值并给胡牌玩家计算胡牌分
	for (int i = 0; i < playerCount(); ++i)
	{
		tMsgResult.arrUserHuInfo[i] = m_pArrMjPlayer[i]->userHuInfo();
		tMsgResult.arrUserHuInfo[i].calculate(m_tDeskConfig);
	}

	// 给胡牌与被胡牌玩家之间结算分数，首先先给一胡的玩家结算，再给二胡的玩家结算,最后三胡
	for (int i = 0; i < _vecHu.size(); i++)
	{
		int nHuUser = _vecHu.at(i);
		if (tMsgResult.arrUserHuInfo[nHuUser].isZiMoType())
		{
			for (int i = 0; i < playerCount(); ++i)
			{
				if (i == nHuUser)
				{
					continue;
				}
				if (tMsgResult.arrUserHuInfo[i].nHuIndex > tMsgResult.arrUserHuInfo[nHuUser].nHuIndex)
				{
					tMsgResult.arrSettlementList[nHuUser].nGetScore += tMsgResult.arrUserHuInfo[nHuUser].nFinalScore;

					T_HuLostItem tLostScoreItem(nHuUser
						, tMsgResult.arrUserHuInfo[nHuUser].eMjHuWay
						, tMsgResult.arrUserHuInfo[nHuUser].nFinalScore);
					tMsgResult.arrSettlementList[i].addLostScoreItem(tLostScoreItem);



				}
			}
		}
		else
		{

		}
	}

	onMsgGameResult(tMsgResult);
}


void CSiChuanMjDesk::onSysAppointActiveUser(int nChairID)
{
	m_nActiveUser = nChairID;
	while (m_pArrMjPlayer[m_nActiveUser]->isAlreadyHu())
	{
		m_nActiveUser = nextPlayerIndex(m_nActiveUser, playerCount(), false);
	}
	
	m_cardOut = CARD_EMPTY;
	updateUser();

	T_MsgAppointActiveUser tMsgAppointActiveUser;
	tMsgAppointActiveUser.nActiveUser = m_nActiveUser;
	onMsgAppointActiveUser(tMsgAppointActiveUser);
}


// 麻将中间是一个摸牌打牌,摸牌打牌的过程，但是由于活动玩家摸牌,打牌会产生动作信息,需要系统判断是否有动作信息
// 如果非活动状态玩家有动作且执行了该动作就发生动作流转，活动状态转移到该玩家
void CSiChuanMjDesk::onSysJudgeAndExecActNotify(E_ActNotifyType eActiveNotifyType)
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
		usIgnoreFlags = ~EA_hu;
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

			m_pArrMjPlayer[m_nActiveUser]->think(CARD_EMPTY, usIgnoreFlags);
			if (m_pArrMjPlayer[m_nActiveUser]->actInfo()->usActFlags)
			{
				_gameDispatcher->delayExec(TIME_ID_COMPUTER_THINK_ACT, m_nActiveUser);
				//OnUserActRequest(m_nActiveUser, tActRequest);
			}
			else
			{
				_gameDispatcher->delayExec(TIME_ID_COMPUTER_THINK_OUT_CARD, m_nActiveUser);
				// onUserOutCardRequest(m_nActiveUser, tActRequest.tMjActOutInfo);
			}
		}
		else
		{
			// 是玩家则把动作信息交给玩家自己处理(包括出牌)
			bool bHaveAct = m_pArrMjPlayer[m_nActiveUser]->selectActInfo(CARD_EMPTY, usIgnoreFlags);
			if (bHaveAct)
			{
				onMsgActNotify(*m_pArrMjPlayer[m_nActiveUser]->actInfo());
			}

		}
	}
	else if (eActiveNotifyType == EA_OutCard)
	{
		// 检测是否有动作，此处不必检测各玩家的动作优先级，等玩家响应动作发起请求后一起处理
		bool bHaveAct = false;
		for (int i = 0; i < playerCount(); ++i)
		{
			if (m_pArrMjPlayer[i]->isAlreadyHu())
			{
				continue;
			}
			if (i == m_nActiveUser)
			{
				continue;
			}
			if (m_pArrMjPlayer[i]->isReboot())
			{
				// 如果是机器人直接处理动作信息 并获取机器人的动作请求
				m_pArrMjPlayer[i]->think(m_cardOut, usIgnoreFlags);
				if (m_pArrMjPlayer[i]->actInfo()->usActFlags)
				{
					bHaveAct = true;
					_gameDispatcher->delayExec(TIME_ID_COMPUTER_THINK_ACT, i);
				}
			}
			else
			{
				if (m_pArrMjPlayer[i]->selectActInfo(m_cardOut, 0))
				{
					//动作流转向////////////////////////////////////////////////////////////////////////////////////////////
					//即可能会断流，活动状态流转到执行动作的玩家
					bHaveAct = true;
					onMsgActNotify(*m_pArrMjPlayer[i]->actInfo());
				}
			}

		}

		// 都没有动作信息
		if (!bHaveAct)
		{
			m_nActiveUser = nextPlayerIndex(m_nActiveUser, playerCount(), false);
			onSysAppointActiveUser(m_nActiveUser);
			_gameDispatcher->start(TIME_DRAW_CARD);
		}

	}
	else if (eActiveNotifyType == EA_OtherAct)
	{
		/*目前只有抢杠*/
		// 如果没有补杠
		if (m_pArrMjPlayer[m_nCurExecActUser]->getLatestWeaveCardsItem().byWeaveKind != T_WeaveCardsItem::EW_KongBa)
		{
			onSysAppointActiveUser(m_nCurExecActUser);
			_gameDispatcher->start(TIME_DRAW_CARD);
			return;
		}

		// 有补杠则判断有没有抢杠胡
		bool bHaveQGHu = false;
		for (int i = 0; i < playerCount(); ++i)
		{
			if (i == m_nCurExecActUser)
			{
				continue;
			}
			if (m_pArrMjPlayer[i]->isAlreadyHu())
			{
				continue;
			}
			if (m_pArrMjPlayer[i]->isReboot())
			{
				// 如果是机器人直接处理动作信息 并获取机器人的动作请求
				m_pArrMjPlayer[i]->think(m_cardOut, usIgnoreFlags);
				if (m_pArrMjPlayer[i]->actInfo()->usActFlags)
				{
					bHaveQGHu = true;
					_gameDispatcher->delayExec(TIME_ID_COMPUTER_THINK_ACT, i);
				}
			}
			else
			{
				if (m_pArrMjPlayer[i]->selectActInfo(m_cardOut, usIgnoreFlags))
				{
					//动作流转向////////////////////////////////////////////////////////////////////////////////////////////
					//即可能会断流，活动状态流转到执行动作的玩家
					m_pArrMjPlayer[i]->actInfo()->tMjActHuInfo.eMjHuWay = EHW_QiangGang;
					bHaveQGHu = true;
					onMsgActNotify(*m_pArrMjPlayer[i]->actInfo());
				}
			}
		}

		if (!bHaveQGHu)
		{
			onSysAppointActiveUser(m_nCurExecActUser);
			_gameDispatcher->start(TIME_DRAW_CARD);
		}


	}

}




void CSiChuanMjDesk::onDelayExecActRequest(int nChairID)
{
	T_ActRequest tActRequest;
	tActRequest.usActFlags = m_pArrMjPlayer[nChairID]->actInfo()->usActFlags;
	tActRequest.nKongCardValue = m_pArrMjPlayer[nChairID]->actInfo()->tMjActKongInfo.arrKongSelect[0];
	tActRequest.nKongSelectIndex = 0;
	tActRequest.tMjActOutInfo = m_pArrMjPlayer[nChairID]->actInfo()->tMjActOutInfo;
	OnUserActRequest(nChairID, tActRequest);
}


void CSiChuanMjDesk::onDelayExecOutCardRequest(int nChairID)
{
	T_MjActOutInfo tMjActOutInfo = m_pArrMjPlayer[nChairID]->actInfo()->tMjActOutInfo;
	tMjActOutInfo.nOutCardUser = nChairID;
	onUserOutCardRequest(nChairID, tMjActOutInfo);
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
		_gameDispatcher->start(TIME_ID_Begin);
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
	onSysJudgeAndExecActNotify(EA_OutCard);
	

}

void CSiChuanMjDesk::OnUserActRequest(int nChairID, T_ActRequest tActRequest)
{
	// 验证玩家动作
	
	unsigned short usActFlagsRequest = tActRequest.usActFlags;
	unsigned short usActFlags = m_pArrMjPlayer[nChairID]->actInfo()->usActFlags;

	//玩家没有动作,或已请求
	if (usActFlags == 0 || (usActFlags & EA_Wait))
	{
		return;
	}
	//玩家没有些动作
	if (!(usActFlags & usActFlagsRequest))
	{
		return;
	}
	if (usActFlagsRequest == EA_Pass)
	{
		//玩家执行了过的操作 看看是否漏胡
		if (EA_hu & usActFlags)
		{
			//m_UserData[bDeskStation].m_bLouHu = true;
			//记录漏胡哪张牌
			//m_UserData[bDeskStation].addALouHuCard(m_byLastOutCard);
		}
	}

	if (usActFlagsRequest == EA_Kong)
	{
		m_pArrMjPlayer[nChairID]->actInfo()->tMjActKongInfo.nCurSelectIndex = tActRequest.nKongSelectIndex;
	}

	//清理玩家的动作标志，只留下请求相关动作的标志
	m_pArrMjPlayer[nChairID]->actInfo()->usActFlags &= usActFlagsRequest;

	//把玩家的动作标志置成正在等待状态
	m_pArrMjPlayer[nChairID]->actInfo()->usActFlags |= EA_Wait;

	int byCanActUsers[MJ_MAX_PLAYER];
	unsigned short usAct = 0;

	m_nCurExecActUser = nChairID;

	//获取有哪些玩家可马上执行动作
	int iAtOnceCounter = selectActAtOnceUsers(byCanActUsers, usAct);
	if (iAtOnceCounter > 0)
	{
		//如果iAtOnceCounter == 1,那么只有byCanActUsers[0]执行动作
		//如果iAtOnceCounter > 1,表明有多个玩家可执行同一个动作,那么根据动作进行优先处理
		if (EA_Pass == usAct)
		{
			execActPass(nChairID);
		}
		else if (EA_Pong == usAct)
		{
			execActPong(m_nActiveUser, byCanActUsers[0]);
		}
		else if (EA_Kong == usAct)
		{
			execActKong(m_nActiveUser, byCanActUsers[0]);
		}
		else if (EA_hu == usAct)
		{
			//为玩家执行胡操作
			execActHu(m_nActiveUser, byCanActUsers, iAtOnceCounter);
		}
	}

	m_nCurExecActUser = -1;

}













