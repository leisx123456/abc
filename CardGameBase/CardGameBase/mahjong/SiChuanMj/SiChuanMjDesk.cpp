#include "SiChuanMjDesk.h"
#include "core/LGameDispatcher.h"

CSiChuanMjDesk::CSiChuanMjDesk()
{

	_gameDispatcher = new CLGameDispatcher();
	_gameDispatcher->add(TIME_ID_Ready, 1000, std::bind(&CSiChuanMjDesk::onEventReady, this));
	_gameDispatcher->add(TIME_ID_ROCK_DICE, 3000, std::bind(&CSiChuanMjDesk::onEventCutCards, this));
	_gameDispatcher->add(TIME_ID_FETCH_HANDCARDS, 3000, std::bind(&CSiChuanMjDesk::onEventDealCards, this));
	_gameDispatcher->add(TIME_ID_TBA, 1000, std::bind(&CSiChuanMjDesk::onEventDingQue, this));
	_gameDispatcher->add(TIME_DRAW_CARD, 1000, std::bind(&CSiChuanMjDesk::onEventDrawCard, this));
	_gameDispatcher->add(TIME_ID_ROUND_FINISH, 500, std::bind(&CSiChuanMjDesk::onEventGameFinshed, this));
	// ����AI�ӳٴ���
	_gameDispatcher->addDelayFun(TIME_ID_COMPUTER_THINK_ACT, 1000, std::bind(&CSiChuanMjDesk::onDelayExecActRequest, this, std::placeholders::_1));
	_gameDispatcher->addDelayFun(TIME_ID_COMPUTER_THINK_OUT_CARD, 1000, std::bind(&CSiChuanMjDesk::onDelayExecOutCardRequest, this, std::placeholders::_1));
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

// ��ȡ���ȼ���߿�����ִ�е����
int CSiChuanMjDesk::selectActAtOnceUsers(int byUsers[], unsigned short & usActFlag)
{
	//����δ���붯����������������ң�������
	unsigned short usMax1 = 0;
	int byMax1 = 255;

	//���������붯����������������ң�������
	unsigned short usMaxWaitFlags = 0;
	int nMaxWaitUser = 255;

	for (int i = 0; i < playerCount(); ++i)
	{
		unsigned short usFlags = m_pArrMjPlayer[i]->actInfo()->usActFlags;

		if (usFlags & 0x0003) //�ж��������00000011
		{
			unsigned short usTemp = usFlags & 0xFFFE;//11111110 // ȥ���ȴ�״̬���Ȩλ

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


	// ͨ�������ܺ���ѡ�����Ժ� �����������(������˲��ú�), ��ʱ������һ�ڵ���
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
	//���������û�й�������
	bool bGuoQGang = false;
		if (EHW_QiangGang == m_pArrMjPlayer[nFromUser]->actInfo()->tMjActHuInfo.eMjHuWay)
		{
			bGuoQGang = true;
		}


	//����ҵĶ�����Ϣ�����///////////
	for (int i = 0; i < playerCount(); ++i)
	{
		m_pArrMjPlayer[i]->actInfo()->clear();
	}

	if (bGuoQGang)
	{
		//m_tagParam.bCanFetch = true;
		//m_tagParam.byNextUser = m_byTokenUser;
		//SetTimer(TIME_GIVE_NEXT_TOKEN, 500);
		// ��û����䣬ֱ�Ӹܺ���
		_gameDispatcher->start(TIME_DRAW_CARD);
		return true;
	}

	// ��һ����û�����
	onSysAppointActiveUser(nextPlayerIndex(m_nActiveUser, playerCount(), false));
	_gameDispatcher->start(TIME_DRAW_CARD);
	return true;
}

bool CSiChuanMjDesk::execActPong(int nFromUser, int nToUser)
{

	m_pArrMjPlayer[nToUser]->execPong(nFromUser, m_cardOut);

	//�����ҹ㲥������Ϣ////////////////////////////////////////////////

	for (int i = 0; i < playerCount(); ++i)
	{
		if (nToUser == i)
		{
			//ֻ�ж���ִ���ߵĿͻ��˲�֪����������
			//::memcpy(tagActInfo.byHands, pUserT->m_byHandCards, sizeof(int)*pUserT->m_iHandNums);
		}
		else
		{
		//::memset(tagActInfo.byHands, 0, sizeof(tagActInfo.byHands));
		}

		//Ϊ��ҷ�������

	}


	//����ҵĶ�����Ϣ�����///////////////////////////////
	for (int i = 0; i < playerCount(); ++i)
	{
		m_pArrMjPlayer[i]->actInfo()->clear();
	}

	//��������������Ϊ��һ����ҷ����Ƶ����̡�����������ó����Ƶ����,����Ҫץ�ơ�
	//m_tagParam.bCanFetch = false;
	//m_tagParam.byNextUser = byToUser;
	//SetTimer(TIME_GIVE_NEXT_TOKEN, 500);
	// ָ������
	onSysAppointActiveUser(nToUser);
	return true;
}

bool CSiChuanMjDesk::execActKong(int nFromUser, int nToUser)
{
	m_pArrMjPlayer[nFromUser]->removeLatestOutCard();
	//m_pArrMjPlayer[nToUser]->execKong(nFromUser, )

	//�����ҹ㲥������Ϣ/////////////////////////////////////////////////////////////////////////////////////////////////////////
	for (int i = 0; i < playerCount(); ++i)
	{
		//Ϊ��ҷ�������

	}
	
	//����ҵĶ�����Ϣ�����///////////////////////////////
	for (int i = 0; i < playerCount(); ++i)
	{
		m_pArrMjPlayer[i]->actInfo()->clear();
	}

	//��Ҳ��ܵ��ƿ�������������Һ���/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//����������bDeskStation������ƶ������Ķ�����Ϣ\
			//�����Ķ�����Ϣ�����������i������m_tagActInfo��
	bool bHaveQGHu = false;
	bool bBuGang = true;

	if (bBuGang)
	{
		for (int i = 0; i < playerCount(); ++i)
		{

			if (i == nToUser)
			{
				continue;
			}
			{
				unsigned usIgnoreFlags = ~EA_hu;
				if (m_pArrMjPlayer[i]->selectActInfo(m_cardOut, usIgnoreFlags))
				{
					m_pArrMjPlayer[i]->actInfo()->tMjActHuInfo.eMjHuWay = EHW_QiangGang;
				}

			}
		}
	}

	//���û�����ܣ���Ӧ�õ��������byToUserץ�ƣ����ץ���ܺ���Ϊ���Ͽ���
			//����byToUser�ܷ�����Ȱ������������Ϊ���Ͽ�������IDEV_GIVE_NEXT_TOKEN�¼�����ȷ��
			//�Ƿ��CUser::m_nHuSpecWayת��CUser::m_tagActInfo��
	//if (!bHaveQGHu)
	//{
	//	//pUserT->m_nHuSpecWay[1] = emHuSpecialWay::hsw_GangKai;
	//	if (tagNode.nType == emType::AnGang)
	//	{
	//		pUserT->m_nHuSpecWay[1] = emHuSpecialWay::hsw_AnGangKai;
	//	}
	//	else if (tagNode.nType == emType::MingGang)
	//	{
	//		pUserT->m_nHuSpecWay[1] = emHuSpecialWay::hsw_DianGangKai;
	//	}
	//	else if (tagNode.nType == emType::BuGang)
	//	{
	//		pUserT->m_nHuSpecWay[1] = emHuSpecialWay::hsw_BuGangKai;

	//	}
	//}

	//���̴���/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//��������ܣ��ȴ������Ӧ����������Ҫ�����̱���
	//û�����ܣ�������Ϊ��һ����ҷ����Ƶ�����

	//if (bHaveQGHu)
	//{
	//	if (bYouJinCanQiangGang)
	//	{
	//		//���̱���
	//		m_byTokenUser = byToUser;
	//	}
	//	else
	//	{
	//		//�´ε���������ó�ִ�и��Ƶ���ң���ʱ��ץ��
	//		m_tagParam.bCanFetch = true;
	//		m_tagParam.byNextUser = byToUser;
	//		m_bHaveGang = true;
	//		SetTimer(TIME_GIVE_NEXT_TOKEN, 500);
	//	}
	//}
	//else
	//{
	//	//�´ε���������ó�ִ�и��Ƶ���ң���ʱ��ץ��
	//	m_tagParam.bCanFetch = true;
	//	m_tagParam.byNextUser = byToUser;
	//	m_bHaveGang = true;
	//	SetTimer(TIME_GIVE_NEXT_TOKEN, 500);
	//}

	return true;
}

bool CSiChuanMjDesk::execActHu(int nFromUser, int arrToUser[], int nUserNums)
{
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
	m_mjLogic.copyCards(tMsgDealCards.arrMjCardsPair, MJ_MAX_CARD_COUNT, m_arrMjCardsPair, MJ_MAX_CARD_COUNT);

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
	onSysAppointActiveUser(m_nBanker);

	// 2. ��ȡ���п��ܵĶ�����Ϣ��û�ж��������(����Ϊׯ���Լ�)
	onSysJudgeAndExecActNotify(EA_BankerFirstGot);

}


void CSiChuanMjDesk::onEventDrawCard()
{
	// 1. ��û�����
	if (surplusCards() == 0)
	{
		// �����ǽʣ��Ϊ0������
		_gameDispatcher->start(TIME_ID_ROUND_FINISH);
		return;
	}
	CLMjCard card = drawCard();
	m_pArrMjPlayer[m_nActiveUser]->drawCard(card);
	T_MsgDrawCard tMsgDrawCard;
	tMsgDrawCard.nDrawCardUser = m_nActiveUser;
	tMsgDrawCard.nNewCard = card;
	onMsgDrawCard(tMsgDrawCard);

	// 2. ��ȡ���п��ܵĶ�����Ϣ
	onSysJudgeAndExecActNotify(EA_DarwCard);
}



void CSiChuanMjDesk::onSysAppointActiveUser(int nChairID)
{
	m_nActiveUser = nChairID;
	m_cardOut = CARD_EMPTY;
	updateUser();

	T_MsgAppointActiveUser tMsgAppointActiveUser;
	tMsgAppointActiveUser.nActiveUser = m_nActiveUser;
	onMsgAppointActiveUser(tMsgAppointActiveUser);
}


// �齫�м���һ�����ƴ���,���ƴ��ƵĹ��̣��������ڻ�������,���ƻ����������Ϣ,��Ҫϵͳ�ж��Ƿ��ж�����Ϣ
// ����ǻ״̬����ж�����ִ���˸ö����ͷ���������ת���״̬ת�Ƶ������
void CSiChuanMjDesk::onSysJudgeAndExecActNotify(E_ActNotifyType eActiveNotifyType)
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

			m_pArrMjPlayer[m_nActiveUser]->think(CARD_EMPTY, usIgnoreFlags);
			if (m_pArrMjPlayer[m_nActiveUser]->aiActRequest()->usActFlags)
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
			// �������Ѷ�����Ϣ��������Լ�����(��������)
			T_MjActInfo tMjActInfo;
			bool bHaveAct = m_pArrMjPlayer[m_nActiveUser]->selectActInfo(CARD_EMPTY, usIgnoreFlags);
			if (bHaveAct)
			{
				onMsgActNotify(tMjActInfo);
			}

		}
	}
	else if (eActiveNotifyType == EA_OutCard)
	{
		// ����Ƿ��ж������˴����ؼ�����ҵĶ������ȼ����������Ӧ�������������һ����
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
				// ����ǻ�����ֱ�Ӵ�������Ϣ ����ȡ�����˵Ķ�������
				m_pArrMjPlayer[i]->think(m_cardOut, usIgnoreFlags);
				if (m_pArrMjPlayer[i]->aiActRequest()->usActFlags)
				{
					bHaveAct = true;
					//OnUserActRequest(m_nActiveUser, tActRequest);
				}
			}
			else
			{
				if (m_pArrMjPlayer[i]->selectActInfo(m_cardOut, 0))
				{
					//������ת��////////////////////////////////////////////////////////////////////////////////////////////
					//�����ܻ�������״̬��ת��ִ�ж��������
					bHaveAct = true;
					onMsgActNotify(*m_pArrMjPlayer[i]->actInfo());
				}
			}

		}

		// ��û�ж�����Ϣ
		if (!bHaveAct)
		{
			onSysAppointActiveUser(nextPlayerIndex(m_nActiveUser, playerCount(), false));
			_gameDispatcher->start(TIME_DRAW_CARD);
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


void CSiChuanMjDesk::onDelayExecActRequest(int nChairID)
{
	OnUserActRequest(nChairID, *m_pArrMjPlayer[nChairID]->aiActRequest());
}


void CSiChuanMjDesk::onDelayExecOutCardRequest(int nChairID)
{
	T_MjActOutInfo tMjActOutInfo = m_pArrMjPlayer[nChairID]->aiActRequest()->tMjActOutInfo;
	onUserOutCardRequest(nChairID, tMjActOutInfo);
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
	onSysJudgeAndExecActNotify(EA_OutCard);
	

}

void CSiChuanMjDesk::OnUserActRequest(int nChairID, T_ActRequest tActRequest)
{
	// ��֤��Ҷ���
	unsigned short usActFlagsRequest = tActRequest.usActFlags;
	unsigned short usActFlags = m_pArrMjPlayer[nChairID]->actInfo()->usActFlags;

	//���û�ж���,��������
	if (usActFlags == 0 || (usActFlags & EA_Wait))
	{
		return;
	}
	//���û��Щ����
	if (!(usActFlags & usActFlagsRequest))
	{
		return;
	}
	if (usActFlagsRequest == EA_Pass)
	{
		//���ִ���˹��Ĳ��� �����Ƿ�©��
		if (EA_hu & usActFlags)
		{
			//m_UserData[bDeskStation].m_bLouHu = true;
			//��¼©��������
			//m_UserData[bDeskStation].addALouHuCard(m_byLastOutCard);
		}
	}

	//������ҵĶ�����־��ֻ����������ض����ı�־
	m_pArrMjPlayer[nChairID]->actInfo()->usActFlags &= usActFlagsRequest;

	//����ҵĶ�����־�ó����ڵȴ�״̬
	m_pArrMjPlayer[nChairID]->actInfo()->usActFlags |= EA_Wait;

	int byCanActUsers[MJ_MAX_PLAYER];
	unsigned short usAct = 0;

	//��ȡ����Щ��ҿ�����ִ�ж���
	int iAtOnceCounter = selectActAtOnceUsers(byCanActUsers, usAct);
	if (iAtOnceCounter > 0)
	{
		//���iAtOnceCounter == 1,��ôֻ��byCanActUsers[0]ִ�ж���
		//���iAtOnceCounter > 1,�����ж����ҿ�ִ��ͬһ������,��ô���ݶ����������ȴ���
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
			//Ϊ���ִ�к�����
			execActHu(m_nActiveUser, byCanActUsers, iAtOnceCounter);
		}

	}

	//// ִ�ж���
	//m_nActiveUser = nChairID;
	//// �����һ�ڶ���, �״̬����������

	//// �㲥������Ϣ
	//T_MsgActResultInfo tMsgActResultInfo;
	//onMsgActResult(tMsgActResultInfo);

	//// ��ҵĶ���Ҳ����������������ж������粹�ܱ��˻�����
	//onSysJudgeAndExecActNotify(EA_OtherAct);
}













