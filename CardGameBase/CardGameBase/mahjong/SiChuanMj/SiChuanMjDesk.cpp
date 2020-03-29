#include "SiChuanMjDesk.h"
#include "core/LGameDispatcher.h"

CSiChuanMjDesk::CSiChuanMjDesk()
{

	_gameDispatcher = new CLGameDispatcher();
	_gameDispatcher->add(TIME_ID_Ready, 1000, std::bind(&CSiChuanMjDesk::onEventReady, this));
	_gameDispatcher->add(TIME_ID_ROCK_DICE, 3000, std::bind(&CSiChuanMjDesk::onEventCutCards, this));
	_gameDispatcher->add(TIME_ID_FETCH_HANDCARDS, 3000, std::bind(&CSiChuanMjDesk::onEventDealCards, this));
	_gameDispatcher->add(TIME_ID_TBA, 1000, std::bind(&CSiChuanMjDesk::onEventDingQue, this));

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
	m_tActiveUser.nActiveUser = m_nBanker;
	m_tActiveUser.eActiveUserType = EA_FristGot;
	T_MsgAppointActiveUser tMsgAppointActiveUser;
	tMsgAppointActiveUser.tActiveUser = m_tActiveUser;
	onMsgAppointActiveUser(tMsgAppointActiveUser);

	// 2. ��ȡ���п��ܵĶ�����Ϣ��û�ж��������(����Ϊׯ���Լ�)
	unsigned short usIgnoreFlags = 0;
	usIgnoreFlags |= E_ActionTypeFlags::EA_Kong;	// ���ֲ��ܸ�

	if (m_pArrMjPlayer[m_nBanker]->isReboot())
	{
		// ����ǻ�����ֱ�Ӵ�������Ϣ ����ȡ�����˵Ķ�������
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
		// �������Ѷ�����Ϣ��������Լ�����
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
		// �����ǽʣ��Ϊ0������
		return;
	}
	CLMjCard card = drawCard();
	m_pArrMjPlayer[m_tActiveUser.nActiveUser]->drawCard(card);
	m_tActiveUser.nNewCard = card.value();

	// 1. ָ������
	m_tActiveUser.nActiveUser = m_nBanker;
	m_tActiveUser.eActiveUserType = EA_FristGot;
	T_MsgAppointActiveUser tMsgAppointActiveUser;
	tMsgAppointActiveUser.tActiveUser = m_tActiveUser;
	onMsgAppointActiveUser(tMsgAppointActiveUser);

	// 2. ��ȡ���п��ܵĶ�����Ϣ��û�ж��������(����Ϊׯ���Լ�)
	unsigned short usIgnoreFlags = 0;
	usIgnoreFlags |= E_ActionTypeFlags::EA_Kong;	// ���ֲ��ܸ�

	if (m_pArrMjPlayer[m_nBanker]->isReboot())
	{
		// ����ǻ�����ֱ�Ӵ�������Ϣ ����ȡ�����˵Ķ�������
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
		// �������Ѷ�����Ϣ��������Լ�����
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
			// �����ǽʣ��Ϊ0������
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
			// �����ǽʣ��Ϊ0������
			return;
		}
		CLMjCard card = drawCard();
		m_pArrMjPlayer[m_tActiveUser.nActiveUser]->drawCard(card);
		m_tActiveUser.nNewCard = card.value();

		nResponseUser = m_tActiveUser.nActiveUser;
	}
	else
	{
		// �Զ�Ϊ����ѡ������ܺ�
		for (int i = 0; i < playerCount(); i++)
		{
			if (m_pArrMjPlayer[i]->isReboot())
			{
				// ����ǻ�����ֱ�Ӵ�������Ϣ ����ȡ�����˵Ķ�������
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
				// ���ҽ���������ж�����Ϣʱ�����ҷ��Ͷ���֪ͨ����������Ҵ���
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

	// �㲥�û���ȱ���
	onMsgDingQue();

	if (playerCount() == nTBANum)
	{
		
		m_nActiveUser = m_nBanker;
		m_cardOut = CARD_EMPTY;
		updateUser();

		// ��Ҫ�ȴ������˶�ȱ���
		//onEventAppointActiveUser(); // Ҫ��Ҫ������ɳ���
		onEventFristBankerActive();
	}
}

void CSiChuanMjDesk::onUserOutCard(int nChairID, T_MjActOutInfo tMjActOutInfo)
{
	// ִ�ж������㲥������Ϣ
	m_pArrMjPlayer[nChairID]->outCard(tMjActOutInfo.nOutCardValue);
	m_cardOut = tMjActOutInfo.nOutCardValue;
	onMsgOutCard(tMjActOutInfo);

	// ��ҳ����ƿ���������������ж��������ж��Ƿ�������Ҿ߱�����
	//����������bDeskStation������ƶ������Ķ�����Ϣ\ �����Ķ�����Ϣ�����������i������m_tagActInfo��
	bool bHaveAct = false;
	for (int i = 0; i < playerCount(); ++i)
	{
		if (i != m_nActiveUser) //�ǳ��ƶ��ſ����ж���
		{
			if (m_pArrMjPlayer[i]->selectActInfo(&tMjActInfo, m_cardOut, 0))
			{
				onMsgActNotify(tMjActInfo);
				bHaveAct = true;
			}
		}
	}

	//������ת��////////////////////////////////////////////////////////////////////////////////////////////

	//��ҳ������,������£�\
			1.���û�����ж���,ϵͳΪ�¸���Ҹ�����\
			2.���ж������������ȴ����ѡ��������Ҫ�����̱���

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








