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
	// 1����Զ�׼��
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
	//static CLMjCard arrCardHand[4][14]; // ����performFunctionInCocosThread������һ�̣߳���arrCardHandʼ�������ô������ݣ�
	//����arrCardHand��ǰ�ͷţ��ʼ���static����
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
	// �Ƿ�����ʱ���Զ�������һ�¼�
	//_gameDispatcher->start(TIME_ID_TBA);

	// �Զ�Ϊ����ѡ����
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


// ׯ�����ֳ���, һ��ֻ����һ��
void CSiChuanMjDesk::onEventFristBankerActive()
{
	// 1. ָ������
	m_nActiveUser = m_nBanker;
	m_cardOut = CARD_EMPTY;
	updateUser();
	
	T_MsgAppointActiveUser tMsgAppointActiveUser;
	tMsgAppointActiveUser.tActiveUser.nActiveUser = m_nActiveUser;
	onMsgAppointActiveUser(tMsgAppointActiveUser);

	// 2. ��ȡ���п��ܵĶ�����Ϣ��û�ж��������(����Ϊׯ���Լ�)
	onEventJudgeAndExecActNotify(EA_BankerFirstGot);

}


void CSiChuanMjDesk::onEventDrawCard()
{
	// 1. ���»���
	m_nActiveUser = nextPlayerIndex(m_nActiveUser, playerCount(), false);
	updateUser();

	// 2. ��û�����
	if (surplusCards() == 0)
	{
		// �����ǽʣ��Ϊ0������
		_gameDispatcher->start(TIME_ID_ROUND_FINISH);
		return;
	}
	CLMjCard card = drawCard();
	m_pArrMjPlayer[m_nActiveUser]->drawCard(card);
	
	// ������Ϣ
	T_MsgAppointActiveUser tMsgAppointActiveUser;
	tMsgAppointActiveUser.tActiveUser.nActiveUser = m_nActiveUser;
	tMsgAppointActiveUser.tActiveUser.nNewCard = card.value();
	onMsgAppointActiveUser(tMsgAppointActiveUser);


	// 2. ��ȡ���п��ܵĶ�����Ϣ
	onEventJudgeAndExecActNotify(EA_DarwCard);
}


// �齫�м���һ�����ƴ���,���ƴ��ƵĹ��̣��������ڻ�������,���ƻ����������Ϣ,��Ҫϵͳ�ж��Ƿ��ж�����Ϣ
// ����ǻ״̬����ж�����ִ���˸ö����ͷ���������ת���״̬ת�Ƶ������
void CSiChuanMjDesk::onEventJudgeAndExecActNotify(E_ActNotifyType eActiveNotifyType)
{
	// 1. ��������
	unsigned short usIgnoreFlags = 0;
	switch (eActiveNotifyType)
	{
	case CSiChuanMjDesk::EA_BankerFirstGot:
		usIgnoreFlags |= E_ActionTypeFlags::EA_Kong;	// ���ֲ��ܸ�

		usIgnoreFlags |= E_ActionTypeFlags::EA_Eat;
		usIgnoreFlags |= E_ActionTypeFlags::EA_Pong;
		break;
	case CSiChuanMjDesk::EA_DarwCard:
		//�Լ����ƿ϶����ܳ���
		usIgnoreFlags |= E_ActionTypeFlags::EA_Eat;
		usIgnoreFlags |= E_ActionTypeFlags::EA_Pong;
		break;
	case CSiChuanMjDesk::EA_OutCard:
		break;
	case CSiChuanMjDesk::EA_OtherAct:
		// ֻ���Ǻ�
		usIgnoreFlags |= E_ActionTypeFlags::EA_Kong;
		usIgnoreFlags |= E_ActionTypeFlags::EA_Eat;
		usIgnoreFlags |= E_ActionTypeFlags::EA_Pong;
		break;
	default:
		break;
	}

	// 2. �ж��Ƿ��ж���
	if (eActiveNotifyType == EA_BankerFirstGot || eActiveNotifyType == EA_DarwCard)
	{
		if (m_pArrMjPlayer[m_nActiveUser]->isReboot())
		{
			// ����ǻ�����ֱ�Ӵ�������Ϣ ����ȡ�����˵Ķ�������
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
			// �������Ѷ�����Ϣ��������Լ�����(��������)
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
				// ����ǻ�����ֱ�Ӵ�������Ϣ ����ȡ�����˵Ķ�������
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
					//������ת��////////////////////////////////////////////////////////////////////////////////////////////
					//�����ܻ�������״̬��ת��ִ�ж��������
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
		// Ŀǰֻ������
	}



}



void CSiChuanMjDesk::onEventGameFinshed()
{

}



//////////////////////////////////////////////////////////////////////////
// onUser�ﲻ����onUser
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

	// �㲥�û�׼��
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

	// ��Ҫ�ȴ������˶�ȱ���
	if (playerCount() == nTBANum)
	{
		// �㲥��ȱ���
		T_MsgDingQue tMsgDingQue;
		for (int i = 0; i < playerCount(); ++i)
		{
			tMsgDingQue.arrCardColor[i] = m_pArrMjPlayer[i]->getDingQueColor();
		}
		onMsgDingQue(tMsgDingQue);
		
		// ָ������Ϊׯ��
		onEventFristBankerActive();
	}
}

void CSiChuanMjDesk::onUserOutCardRequest(int nChairID, T_MjActOutInfo tMjActOutInfo)
{
	// ִ�ж���
	m_pArrMjPlayer[nChairID]->outCard(tMjActOutInfo.nOutCardValue);
	m_cardOut = tMjActOutInfo.nOutCardValue;

	// �㲥������Ϣ
	T_MsgOutCard tMsgOutCard;
	tMsgOutCard.nOutCardUser = tMjActOutInfo.nOutCardUser;
	tMsgOutCard.nOutCardValue = tMjActOutInfo.nOutCardValue;
	m_pArrMjPlayer[nChairID]->getOutCardsValue(tMsgOutCard.arrOutCard, tMsgOutCard.nOutedNums);
	onMsgOutCard(tMsgOutCard);

	// ��ҳ����ƿ���������������ж���
	onEventJudgeAndExecActNotify(EA_OutCard);
	

}

void CSiChuanMjDesk::OnUserActRequest(int nChairID, T_ActRequest tActRequest)
{
	// ִ�ж���
	m_pArrMjPlayer[nChairID]->execAction(tActRequest);
	m_nActiveUser = nChairID;
	// �����һ�ڶ���, �״̬����������

	// �㲥������Ϣ
	T_MsgActResultInfo tMsgActResultInfo;
	onMsgActResult(tMsgActResultInfo);

	// ��ҵĶ���Ҳ����������������ж������粹�ܱ��˻�����
	onEventJudgeAndExecActNotify(EA_OtherAct);
}









