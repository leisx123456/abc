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


bool CSiChuanMjDesk::selectActInfo(int nResponseUser, int nActiveUser, T_MjActInfo* pActInfo, CLMjCard cardDest, unsigned short usIgnoreFlags /*= 0*/)
{
	CLMjPlayer* pPlayerResponse = m_pArrMjPlayer[nResponseUser];
	CLMjPlayer* pPlayerActive = m_pArrMjPlayer[nActiveUser];
	// 1. ���cardDest�ǿ��ƣ���ʾׯ�����ֳ���
	if (!cardDest.isValid())
	{
		//�Լ����ƿ϶����ܳ���
		usIgnoreFlags |= E_ActionTypeFlags::EA_Eat;
		usIgnoreFlags |= E_ActionTypeFlags::EA_Pong;
		// ���ֲ��ܸ�
		usIgnoreFlags |= E_ActionTypeFlags::EA_Kong;
	}
	
	
	//2.���nResponseUser == nActiveUser˵�������ƣ�cardDest����������
	if (nResponseUser == nActiveUser)
	{
		//�Լ����ƿ϶����ܳ���
		usIgnoreFlags |= E_ActionTypeFlags::EA_Eat;
		usIgnoreFlags |= E_ActionTypeFlags::EA_Pong;
	}
	else 
	{
		//3.���nResponseUser != nActiveUser˵���Ǳ������ڳ��ƣ�cardDest���������
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


// ����� ��Ϊһ��ֻ����һ��
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
//			// �����ǽʣ��Ϊ0������
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
//	// �Զ�Ϊ����ѡ������ܺ�
//	for (int i = 0; i < playerCount(); i++)
//	{
//		if (m_pArrMjPlayer[i]->isReboot())
//		{
//
//		}
//	}
//}




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

		//m_tActiveUser.nActiveUser = m_nBanker;
		//m_tActiveUser.eActiveUserType = EA_FristGot;

		// ��Ҫ�ȴ������˶�ȱ���
		//onEventAppointActiveUser(); // Ҫ��Ҫ������ɳ���
		onEventFristGotActiveUser();
	}
}








