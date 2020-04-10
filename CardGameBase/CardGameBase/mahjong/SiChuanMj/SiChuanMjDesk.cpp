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

// �������ҵĶ�����Ϣ
void CSiChuanMjDesk::clearAllUserActInfo()
{
	for (int i = 0; i < playerCount(); ++i)
	{
		m_pArrMjPlayer[i]->actInfo()->clear();
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
	clearAllUserActInfo();

	if (bGuoQGang)
	{
		_gameDispatcher->start(TIME_DRAW_CARD);
		return true;
	}

	// ��һ����û�����
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

	//�����ҹ㲥������Ϣ////////////////////////////////////////////////
	T_MsgActResultInfo tMsgActResultInfo;
	tMsgActResultInfo.usActFlags = EA_Pong;
	tMsgActResultInfo.byFromUser = nFromUser;
	tMsgActResultInfo.arrActUsers[0] = nToUser;
	tMsgActResultInfo.nActUserNums = 1;
	tMsgActResultInfo.nFromOutCardNums = m_pArrMjPlayer[nFromUser]->outCardNums();
	tMsgActResultInfo.tWeaveCardsValueItem = m_pArrMjPlayer[nToUser]->getLatestWeaveCardsItem();
	tMsgActResultInfo.nWeaveCardsItemNum = m_pArrMjPlayer[nToUser]->weaveItemNums();
	//������ҵ���������
	 m_pArrMjPlayer[nToUser]->getHandCards(tMsgActResultInfo.byHands, tMsgActResultInfo.iHandNums);
	 onMsgActResult(tMsgActResultInfo);

	 clearAllUserActInfo();

	// ָ������
	onSysAppointActiveUser(nToUser);

	if (m_pArrMjPlayer[nToUser]->isReboot())
	{
		// ����ǻ�����˼������
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

	// �����ҹ㲥������Ϣ/////////////////////////////////////////////////////////////////////////////////////////////////////////
	T_MsgActResultInfo tMsgActResultInfo;
	tMsgActResultInfo.usActFlags = EA_Kong;
	tMsgActResultInfo.byFromUser = nFromUser;
	tMsgActResultInfo.arrActUsers[0] = nToUser;
	tMsgActResultInfo.nActUserNums = 1;
	tMsgActResultInfo.nFromOutCardNums = m_pArrMjPlayer[nFromUser]->outCardNums();
	tMsgActResultInfo.tWeaveCardsValueItem = m_pArrMjPlayer[nToUser]->getLatestWeaveCardsItem();
	tMsgActResultInfo.nWeaveCardsItemNum = m_pArrMjPlayer[nToUser]->weaveItemNums();
	// ������ҵ���������
	m_pArrMjPlayer[nToUser]->getHandCards(tMsgActResultInfo.byHands, tMsgActResultInfo.iHandNums);
	onMsgActResult(tMsgActResultInfo);
	
	// ����ұ��ֵĶ�����Ϣ�����///////////////////////////////
	clearAllUserActInfo();

	//��Ҳ��ܵ��ƿ�������������Һ���/////////////////////////////////////////////////////////////////////////////////////////////////////////
	onSysJudgeAndExecActNotify(EA_OtherAct);

	// ��ǰ��¼���ֵĶ�����Ϣ,����m_nCurExecActUser�ܷ�����Ȱ������������Ϊ���Ͽ�����������ȷ���Ƿ�ת��T_UserHuInfo��
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

	//�����ҹ㲥������Ϣ///////////////////////////////////////////////////////////////////////////
	T_MsgActResultInfo tMsgActResultInfo;
	tMsgActResultInfo.usActFlags = EA_hu;
	tMsgActResultInfo.byFromUser = nFromUser;
	memcpy(tMsgActResultInfo.arrActUsers, arrToUser, sizeof(arrToUser));
	tMsgActResultInfo.nActUserNums = nUserNums;
	tMsgActResultInfo.nFromOutCardNums = m_pArrMjPlayer[nFromUser]->outCardNums();
	// ������ҵ���������
	for (int i = 0; i < nUserNums; ++i)
	{
		m_pArrMjPlayer[arrToUser[i]]->getHandCards(tMsgActResultInfo.byHands, tMsgActResultInfo.iHandNums);
	}
	onMsgActResult(tMsgActResultInfo);

	//����ҵĶ�����Ϣ�����///////////////////////////////
	clearAllUserActInfo();

	// ���ֻʣһ��δ���򱾾ֽ���
	if (_vecHu.size() >= playerCount() - 1)
	{
		_gameDispatcher->start(TIME_ID_ROUND_FINISH);
		return true;
	}

	// �����һ�ڶ���, �״̬����������
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
	//static CLMjCard arrCardHand[4][14]; // ����performFunctionInCocosThread������һ�̣߳���arrCardHandʼ�������ô������ݣ�
	//����arrCardHand��ǰ�ͷţ��ʼ���static����
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

	// ����
	for (int i = 0; i < playerCount(); i++)
	{
		m_pArrMjPlayer[i]->sordCards();
	}

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
		// �����ǽʣ��Ϊ0����ׯ������
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

	// 2. ��ȡ���п��ܵĶ�����Ϣ
	onSysJudgeAndExecActNotify(EA_DarwCard);
}


void CSiChuanMjDesk::onEventChaJiao()
{
	m_bHuangZhuang = true;
	// ����û�������������,��ʵ���ǲ�һ������ƣ�Ȼ��õ������ͷ���ȡ����һ��
}

void CSiChuanMjDesk::onEventGameFinshed()
{
	T_MsgResult tMsgResult;
	tMsgResult.bHuangZhuang = m_bHuangZhuang;

	// ����Ϣ�ṹ�帳ֵ����������Ҽ�����Ʒ�
	for (int i = 0; i < playerCount(); ++i)
	{
		tMsgResult.arrUserHuInfo[i] = m_pArrMjPlayer[i]->userHuInfo();
		tMsgResult.arrUserHuInfo[i].calculate(m_tDeskConfig);
	}

	// �������뱻�������֮���������������ȸ�һ������ҽ��㣬�ٸ���������ҽ���,�������
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
		usIgnoreFlags = ~EA_hu;
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
			// �������Ѷ�����Ϣ��������Լ�����(��������)
			bool bHaveAct = m_pArrMjPlayer[m_nActiveUser]->selectActInfo(CARD_EMPTY, usIgnoreFlags);
			if (bHaveAct)
			{
				onMsgActNotify(*m_pArrMjPlayer[m_nActiveUser]->actInfo());
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
			m_nActiveUser = nextPlayerIndex(m_nActiveUser, playerCount(), false);
			onSysAppointActiveUser(m_nActiveUser);
			_gameDispatcher->start(TIME_DRAW_CARD);
		}

	}
	else if (eActiveNotifyType == EA_OtherAct)
	{
		/*Ŀǰֻ������*/
		// ���û�в���
		if (m_pArrMjPlayer[m_nCurExecActUser]->getLatestWeaveCardsItem().byWeaveKind != T_WeaveCardsItem::EW_KongBa)
		{
			onSysAppointActiveUser(m_nCurExecActUser);
			_gameDispatcher->start(TIME_DRAW_CARD);
			return;
		}

		// �в������ж���û�����ܺ�
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
				// ����ǻ�����ֱ�Ӵ�������Ϣ ����ȡ�����˵Ķ�������
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
					//������ת��////////////////////////////////////////////////////////////////////////////////////////////
					//�����ܻ�������״̬��ת��ִ�ж��������
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

	if (usActFlagsRequest == EA_Kong)
	{
		m_pArrMjPlayer[nChairID]->actInfo()->tMjActKongInfo.nCurSelectIndex = tActRequest.nKongSelectIndex;
	}

	//������ҵĶ�����־��ֻ����������ض����ı�־
	m_pArrMjPlayer[nChairID]->actInfo()->usActFlags &= usActFlagsRequest;

	//����ҵĶ�����־�ó����ڵȴ�״̬
	m_pArrMjPlayer[nChairID]->actInfo()->usActFlags |= EA_Wait;

	int byCanActUsers[MJ_MAX_PLAYER];
	unsigned short usAct = 0;

	m_nCurExecActUser = nChairID;

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

	m_nCurExecActUser = -1;

}













