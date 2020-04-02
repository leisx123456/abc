#include "CMJDesk.h"
#include "CommonIniFile.h"
#include <atlstr.h>
#include <algorithm>
#include "../XMjTester/Message.h"
#include <math.h>
#include "LogSys/LogMgr.h"
#include "CMJLogic.h"

CMJDesk::CMJDesk(): CGameDesk(BEGIN_BY_GAME)
{
	Init(NULL);
	m_iPiaoCount = 0;
	//��ʼ����Ϸ״̬
	m_nGameStation = GS_WAIT_SETGAME;
	m_bAllAgreeDismissRoom = false; 
	m_iPlayerCount = 4;

	m_bReserveSeat=true;

}
//��������
CMJDesk::~CMJDesk(void)
{

}

void CMJDesk::Init(const UserInfoForGame_t* pUserInfo)
{
	m_byLastFetchCardUser=255;

	//ȫ��ʼ�������Ϸ����
	HalfInit();

	for(int i = 0; i < PLAY_COUNT; i++)
	{
		//�����û�����
		m_UserData[i].Init();
	}

	//����ĳɼ���
	m_tagResult.Clear();

	//ׯ��
	m_byNTUser = 255;

}

///���������ʼ�������ڻغ�֮��������ȫ�Ĳ���������Ϊ�е�������Ҫ������
void CMJDesk::HalfInit()
{
	m_nLastPengCard=255;

	m_nLastPengUser=255;

	m_bHaveFetchBird=false;

	for(int i = 0; i < PLAY_COUNT; i++)
	{
		//�����û�����
		m_UserData[i].HalfInit();
	}
	//ׯ�Ҵ����Ӷ�ȡ�Ʒ���ʱ�����ӵĵ���ֵ
	m_byFetchPoint[0] = 0;
	m_byFetchPoint[1] = 0;

	m_byLaiZi=255;

	//��ǰ����û�
	m_byTokenUser = 255;

	//ָ���¸������Ƶ����
	m_byTokenNextUser = 255;

	//�Ƿ��ׯ
	m_bHuangZhuang = false;		

	m_iWallLeft = 0;
	m_iMjCardNum = 0;
	m_bHaveGang = false;
	m_bPiao = false;
	::memset(m_niaoCardValues,0,sizeof(m_niaoCardValues));
	::memset(m_iAllCard,0,sizeof(m_iAllCard));
	memset(m_bIsZhongNiao,0,sizeof(m_bIsZhongNiao));
}


//��ʼ������
int CMJDesk::InitialDesk(int nDeskNo, int nMaxPeople, IGameMainManager* pIMainManager)
{
	int bRet =  __super::InitialDesk(nDeskNo,nMaxPeople,pIMainManager);
	//���������ļ�
	LoadDeskCfg();
	return bRet;

}

//������ע
BOOL CMJDesk::LoadDeskCfg()
{
	TCHAR szIniName[MAX_PATH];
	sprintf(szIniName,"%s",CINIFile::GetAppPath());
	sprintf_s(szIniName,"%s%d_s.ini",szIniName,NAME_ID);
	CINIFile f(szIniName);
	Tstring strSeg = "game";
	Tstring strKey;
	TDeskCfg* pDeskCfg = &m_tagDeskCfg;

	//����˼��ʱ��(��)
	strKey = _TEXT("OutCardTime");
	pDeskCfg->byOutCard = f.GetKeyVal(strSeg,strKey,20);

	//����˼��ʱ�䣬�����ܺ�(��)
	strKey = _TEXT("ActTime");
	pDeskCfg->byAct = f.GetKeyVal(strSeg,strKey,15);

	//�Ƿ��������� 

	strKey = _TEXT("CheaterOpen");
	pDeskCfg->byCheaterOpen = f.GetKeyVal(strSeg,strKey,0);


	return TRUE;
}

void CMJDesk::killAllTimer()
{
	for (int i=10;i<40;i++)
	{
		KillTimer(i);
	}
}

void CMJDesk::OnUserLeftDesk(int nDeskStation)
{
	SetUnknowStateTo(nDeskStation);
}


//������Ϸ�ڲ���Ϣ
int CMJDesk::HandleNotifyMessage(int nDeskStation, UINT code, BYTE* pBuffer, int nLen)
{
	DebugPrintf(m_pGameManager->GetRoomID(),m_nDeskIndex,"HandleNotifyMessage====>chair=%d,code=%d",nDeskStation,code);
	bool bWatchUser = false;
	switch (code)
	{

	case ASS_ASSIGN_MAKENT_FINISH:
		{
			//���������ɶ����ƶ�������
			//return OnUserMakeNTOver(nDeskStation,pBuffer,nLen,bWatchUser);
			return 0;
		}
		break;
	case ASS_PIAO_REQ:
		{
			//�ͻ��˷�����Ʈ��Ϣ
			return OnUserPiao(nDeskStation,pBuffer,nLen,bWatchUser);
		}
		break;
	case ASS_OUT_CARD_REQ:
		{
			//������ҳ�������
			return OnUserOutCardRequest(nDeskStation,pBuffer,nLen,bWatchUser);
		}
		break;
	case ASS_ACT_REQ:
		{
			//������ҵĶ�������
			return OnUserActRequest(nDeskStation,pBuffer,nLen,bWatchUser);
		}
		break;

	case ASS_AUTO_REQ:
		{
			//��������й���Ϣ
			return OnUserAutoRequest(nDeskStation,pBuffer,nLen,bWatchUser);
		}
		break;
		//��Ϸ���Ե���Ϣ����
#ifdef PEI_PAI_ON
	case ASS_TEST_FETCH_INFO:
		{
			//�ͻ���ָ���·�����
			return OnTestChangeFetchHandcards(nDeskStation,pBuffer,nLen,bWatchUser);
		}
		break;
	case ASS_TEST_CH_HAND:
		{
			//�۸�����
			return OnTestChangeHands(nDeskStation,pBuffer,nLen,bWatchUser);
		}
		break;

	case ASS_TEST_CH_FETCH:
		{
			//�۸�����
			return OnTestChangeFetch(nDeskStation,pBuffer,nLen,bWatchUser);
		}
		break;	
#endif

	}
	return __super::HandleNotifyMessage(nDeskStation,code,pBuffer,nLen);
}

//��ȡ��Ϸ״̬�������͵��ͻ���
int CMJDesk::OnGetGameStation(int nDeskStation, UserInfoForGame_t& userInfo)
{
	DebugPrintf(m_pGameManager->GetRoomID(),m_nDeskIndex,"OnGetGameStation====>chair=%d,station%d",nDeskStation,m_nGameStation);
	this->notiyDissmissState(nDeskStation);
	bool bWatchUser = false;
	UINT uSocketID = userInfo._SocketIndex;
	switch (m_nGameStation)
	{

	case GS_WAIT_SETGAME:	
	case GS_WAIT_ARGEE:
		{
			//��Ϸû�п�ʼ״̬
			return OnGetGSWaitSet(nDeskStation,uSocketID,bWatchUser);
		}
		break;

	case GS_PLAYING:
		{
			//����Ҵ���״̬
			return OnGetGSPlaying(nDeskStation,uSocketID,bWatchUser);
		}
		break;
	case GS_WAIT_PIAO:
		{
			//Ư������
			return  OnGetGSWaitPiao(nDeskStation,uSocketID,bWatchUser);
		}
	case GS_WAIT_NEXT_ROUND:
		{
			//�ȴ���һ����Ϸ�Ŀ�ʼ
			return OnGetGSNextRound(nDeskStation,uSocketID,bWatchUser);
		}
		break;
	default:
		{
			return 0;
		}
		break;
	}

	return 0;
}

//��ʱ��ʱ�䵽
int CMJDesk::OnGameTimer(int uTimerID)
{
	switch(uTimerID)
	{
	case TIME_FETCH_GUI:
		{
			KillTimer(TIME_FETCH_GUI);
			OnEventFetchGui();
		}
		break;

	case TIME_NT_MORE:
		{
			KillTimer(TIME_NT_MORE);
			if (m_bPiao)
			{
				SetTimer(TIME_PIAO,500);
			}
			else
			{
				//������ --> 20181203����
				SetTimer(TIME_FETCH_METHOD,500);
			}
		}
		break;
		///��ׯ	
	case TIME_MAKE_NT :
		{
			KillTimer(TIME_MAKE_NT);
			OnEventMakeNT();
		}
		break;
		// ���Ʒ���
	case TIME_FETCH_METHOD:
		{
			KillTimer(TIME_FETCH_METHOD);
			OnEventFetchMethod();
		}
		break;
		///�·�����
	case TIME_FETCH_HANDCARDS:
		{
			KillTimer(TIME_FETCH_HANDCARDS);
			OnEventFetchHandCards();
		}
		break;
		///Ư
	case  TIME_PIAO:
		{
			KillTimer(TIME_PIAO);
			OnEventPiao();
		}
		break;
		///�·�����
	case TIME_GIVE_NEXT_TOKEN:
		{
			KillTimer(TIME_GIVE_NEXT_TOKEN);
			OnEventGiveNextToken();
		}
		break;
		///ץ��
	case TIME_FETCH_BIRD:
		{
			KillTimer(TIME_FETCH_BIRD);
			OnEventFetchNiao();
		}
		break;
		//һ�غϽ������¼�����
	case TIME_ROUND_FINISH:
		{
			KillTimer(TIME_ROUND_FINISH);
			OnEventRoundFinish();

		}
		break;
		///����
	case TIME_RESULT:
		{
			KillTimer(TIME_RESULT);
			OnEventResult();
		}
	case TIME_AUTO_ACT:
		{
			KillTimer(TIME_AUTO_ACT);
			OnEventAutoAct(true);
		}
		break;
	case TIME_AUTO_OUTCARD:
		{
			KillTimer(TIME_AUTO_OUTCARD);
			OnEventAutoOutCard(true);
		}
		break;
	case TIME_AUTO_ACT_NO:
		{
			KillTimer(TIME_AUTO_ACT_NO);
			OnEventAutoAct(false);
		}
		break;
	case TIME_AUTO_OUTCARD_NO:
		{
			KillTimer(TIME_AUTO_OUTCARD_NO);
			OnEventAutoOutCard(false);
		}
		break;
	}
	return __super::OnGameTimer(uTimerID);
}

//���������ĳλ��
void CMJDesk::OnUserSitDesk(int nDeskStation)
{
	memcpy(&m_ResultUserData[nDeskStation],&m_UserInfo[nDeskStation],sizeof(m_UserInfo[nDeskStation]));	
}


//��Ҷ���
void CMJDesk::OnUserNetCut(int nDeskStation)
{
	UserData* pNCUser = &m_UserData[nDeskStation];
	//����״̬
	pNCUser->m_bNetCut = true;
	//�й�״̬
	pNCUser->m_bAuto = true;

	//֪ͨ�������pNCUser��ȡ���й�
	//��Ҷ��� ��ǰ����й�
	Json::Value response;
	response["chair"] = nDeskStation;
	response["isAuto"] = true;

	for (int i = 0; i < m_iPlayerCount; i ++)
	{
		//Ϊ�����˷��͵�ǰ����й�
		SendGameData(i,response,ASS_AUTO_INFO);
	}
}

///���ƣ�OnUserAgreeGame
///�������������ͬ����Ϸ
///@param bDeskStation λ��, pNetHead ��Ϣͷ,pData ��Ϣ����,uSize ��Ϣ������,uSocketID id,bWatchUser �Ƿ��Թ�
///@return 
void CMJDesk::OnUserAgreeGame(int bDeskStation)
{
	bool isAgree = (m_UserData[bDeskStation].m_nState == UserData::AGREEGAME);
	if (isAgree)
	{
		return  ;
	}
	SetAgreeStateTo(bDeskStation);
	m_PlayCount = GetAgreePlayers();
	if (m_PlayCount == m_iPlayerCount)
	{
		GameBegin(0);
	}
}

void CMJDesk::OnUserRecome(int bDeskStation)
{
	//�����������������ȡ���й�
	if (m_UserData[bDeskStation].m_bAuto)
	{

		m_UserData[bDeskStation].m_bAuto = false;
	}

}


///���ƣ�GetAgreeNums
///�������õ��ж��ٸ����ͬ����Ϸ
int CMJDesk::GetAgreePlayers()
{
	int iAgreeConuter = 0;
	for (int i = 0; i < m_iPlayerCount; ++i)
	{
		if (m_UserData[i].m_nState == UserData::AGREEGAME)
		{
			iAgreeConuter++;
		}
	}

	return iAgreeConuter;
}

//���䱻�����ɢ
void CMJDesk::onDismissDesk()
{
	//�ܾ�������Ϊ��ǰ�ľ���
	m_tagRoundFinish.iAllRoundTimes = m_tagRoundFinish.iRoundTimes;

	//�����˶�ͳһ��ɢ����
	m_bAllAgreeDismissRoom = true;

	for (int i = 0; i < PLAY_COUNT; ++i)
	{
		m_UserData[i].m_nResult = MjResult::Res_Unknown;
	}

	if (m_nGameStation == GS_WAIT_NEXT_ROUND)
	{
		for (int i= 0; i< PLAY_COUNT ; i++)
		{
			m_UserData[i].m_iHandNums = 0;
			m_UserData[i].m_iCGPNums = 0;
			Init(NULL);
		}
	}
	OnEventResult();
	m_nGameStation = GS_WAIT_SETGAME;
}
///��ȡ��Ϸ��ID¼����õ�
unsigned int CMJDesk::GetGameNameID()
{
	return NAME_ID;
}


//��Ϸ��ʼ
bool CMJDesk::GameBegin(BYTE bBeginFlag)
{
	DebugPrintf(m_pGameManager->GetRoomID(),m_nDeskIndex,"GameFinish====>begin");
	//�ȵ��ø���  ��ֹ��ʼ֮ǰ������뿪����
	if (!__super::GameBegin(bBeginFlag))
	{
		DebugPrintf(m_pGameManager->GetRoomID(),m_nDeskIndex,"GameFinish====>1");
		return false;
	}
	m_nGameStation = GS_PLAYING;
	m_gameBeginTime = time(NULL);
	//�غϼ�������
	m_tagRoundFinish.iRoundTimes ++;

	if (!m_pGameManager->IsPrivateRoom())
	{
		//��ҳ�ÿ�ζ�����ѡׯ
		m_tagRoundFinish.iRoundTimes = 1;
	}

	//if(m_tagRoundFinish.iRoundTimes == 1)
	//{
	//	//������Ϸ�����̷���
	//	CXGameDesk::StartUp();
	//}
	//���浱ǰ����
	Json::Value  RoundStatus;
	RoundStatus["currRounds"] = m_tagRoundFinish.iRoundTimes;
	Json::FastWriter write;
	std::string str = write.write(RoundStatus);
	m_pGameManager->SaveStatus(m_nDeskIndex,str.c_str());

	if (m_tagRoundFinish.iRoundTimes == 1)
	{
		//��һ�غ�,ȫ��ʼ�������Ϸ����
		Init(m_UserInfo);

		if (!m_pGameManager->IsPrivateRoom())
		{
			//��ҳ���һ��Ĭ�ϵķ�������


		}

	}
	else if(m_tagRoundFinish.iRoundTimes > 1)
	{
		//�ڶ��غϺ�,���ʼ�������Ϸ����
		HalfInit();
	}

	Json::Value response;
	//��ʼ���;���
	response["RoundTimes"] = m_tagRoundFinish.iRoundTimes;
	response["AllRoundTimes"] = m_tagRoundFinish.iAllRoundTimes;
	//�·���Ϸ��ʼ�źţ�֪ͨ�ͻ��˿�����Ϸ״̬��
	for (int i = 0; i < m_iPlayerCount; i++)
	{
		SendGameData(i,response,ASS_GAME_BEGIN);
	}

	this->InitPlatVideo(m_tagRoundFinish.iRoundTimes ,m_iPlayerCount,szConfig );
	this->InsertGameVideo( Video_Event::Video_Event_begin , response );
	//������ׯ����
	SetTimer(TIME_MAKE_NT,1500);


	///����Ҫע��Ҫ������Ҫ׼ȷ�����������112 ��  ����108��Ҫȷ��
	if (m_bZhongCaishen)
	{
		m_iWallLeft = MJ_MAX_CARDS_ZHONG;
	}
	else
	{
		m_iWallLeft = MJ_MAX_CARDS;
	}

	///��Ϸ��ʼ��ϴ��
	///���������������������Ҳ���Ըոպ�
	m_iMjCardNum = MJ_MAX_CARDS_ZHONG;//���������������ȥ��ֵ��

	if (m_bZhongCaishen)
	{
		////126===1111110   
		CMJLogic::ShuffleCards(m_iAllCard,MJ_MAX_CARDS_ZHONG,126);
	}
	else
	{
		////46===1110   
		CMJLogic::ShuffleCards(m_iAllCard,MJ_MAX_CARDS_ZHONG,14);
	}

	DebugPrintf(m_pGameManager->GetRoomID(),m_nDeskIndex,"GameFinish====>end");
	return true;
}

//��Ϸ����
bool CMJDesk::GameFinish(BYTE bDeskStation, BYTE bCloseFlag)
{
	DebugPrintf(m_pGameManager->GetRoomID(),m_nDeskIndex,"GameFinish====>begin");
	this->SaveVideo( m_gameBeginTime );
	//��������
	ReSetGameState(bCloseFlag);

	//��������
	m_dwBeginTime = (long int)time(NULL);

	//�����������
	if (m_pGameManager->IsPrivateRoom())
	{
		if (DeskCanDismiss())//����ܹ��ﵽ���ӽ�ɢ���������Ǿͽ���Ϸ״̬��Ϊ�ƾֽ���״̬
		{
			m_nGameStation = GS_GAME_OVER;
		}
		else
		{
			m_nGameStation = GS_WAIT_NEXT;
		}
	}

	//�����״̬ͬ��״̬���
	for (int i =0; i < m_nMaxPeople; i++)
	{
		m_UserInfo[i]._bIsAgree = false;
	}

	if (!m_bAllAgreeDismissRoom)//������һ�֣��ſ۷���
	{
		m_pGameManager->OnGameFinish(m_nDeskIndex);
	}
	killAllTimer();
	DebugPrintf(m_pGameManager->GetRoomID(),m_nDeskIndex,"GameFinish====>end");
	return 0;
}

//������Ϸ״̬
bool CMJDesk::ReSetGameState(BYTE bLastStation)
{
	//if (this->DeskCanDismiss())
	//{
	//	//�ر���Ϸ�����̷���
	//	CXGameDesk::ShutDown();
	//}

	//dong add
	m_iPiaoCount = 0;

	for (int i= 0; i<PLAY_COUNT;i++)
	{
		UserData* pUserF = &m_UserData[i];
		pUserF->iGangPoint = 0;
		pUserF->m_nState = UserData::UNKNOWN;
	}

	return TRUE;
}

///��ʾ��Ϸ�Ƿ�ʼ�����þ��������֣�
bool CMJDesk::IsHavePlaying()
{
	return m_tagRoundFinish.iRoundTimes > 0;
}


///�Ƿ�ﵽ��ɢ������
bool CMJDesk::DeskCanDismiss()
{
	return (m_tagRoundFinish.iRoundTimes >= m_tagRoundFinish.iAllRoundTimes || m_dismiss_condition_valid);
}


///�����ļ��Ĵ���
void CMJDesk::OnDeskConfigured(const char* szClientConfig, const char *szSysConfig)
{
	m_bAllAgreeDismissRoom = false;
	Init(NULL);
	m_nGameStation = GS_WAIT_ARGEE;

	m_bNTResult = false;		//�Ƿ�ׯ�����
	m_bAllowDui7 = false;	//�Ƿ�ɺ��߶�
	m_bPiao = false;			//�Ƿ��Ʈ
	m_bZhongCaishen = true;	//����Ϊ����
	m_iPlayerCount = 4;
	m_iNiaoNums=0;
	m_bIsYiMaQuanZhong=false;

	m_playType = emPlayType::pt_Zimo;
	szConfig = szClientConfig;

	Json::Reader reader(Json::Features::strictMode());
	Json::Value root;
	if (!reader.parse(szClientConfig,root))
	{
		return;
	}
	Json::Value rootDesc;
	if (!reader.parse(szSysConfig,rootDesc))
	{
		return;
	}
	char RoomConfig[128];
	memset(RoomConfig,0,sizeof(RoomConfig));


	//��������

	int iPlayerCountID = 1;
	if (!root["PlayerCount"].isNull())
	{
		if (root["PlayerCount"]["id"].isInt())
		{
			iPlayerCountID = root["PlayerCount"]["id"].asInt();
			if (!rootDesc["PlayerCount"]["cell"].isNull() && rootDesc["PlayerCount"]["cell"].size() >= 1)
			{
				Json::Value PlayerCountCell = rootDesc["PlayerCount"]["cell"];
				Json::Value maxScoreJson = PlayerCountCell[iPlayerCountID - 1];

				if (maxScoreJson["Player"].isInt())
				{
					m_iPlayerCount = maxScoreJson["Player"].asInt();
				}
			}
		}
	}
	m_nMaxPeople=m_iPlayerCount;


	if (!root["ZhuaMa"].isNull())
	{
		if (root["ZhuaMa"]["id"].isInt())
		{
			int nID = root["ZhuaMa"]["id"].asInt();
			if (!rootDesc["ZhuaMa"]["cell"].isNull() && rootDesc["ZhuaMa"]["cell"].size() >= 1)
			{
				Json::Value jsCell = rootDesc["ZhuaMa"]["cell"];
				Json::Value js = jsCell[nID - 1];

				if (js["Ma"].isInt())
				{
					m_iNiaoNums = js["Ma"].asInt();
				}
			}

			if (nID==1)
			{
				m_bIsYiMaQuanZhong=true;
			}
		}
	}

	/*int roundID = root["Round"]["id"].asInt();
	if (roundID == 1)
	{
	m_iAllRounds = 4;
	}
	else if(roundID == 2) 
	{
	m_iAllRounds = 8;
	}
	else if(roundID == 3) 
	{
	m_iAllRounds = 16;

	}*/
	int iRoundID = 1;
	if (!root["Round"].isNull())
	{
		if (root["Round"]["id"].isInt())
		{
			iRoundID = root["Round"]["id"].asInt();
			if (!rootDesc["Round"]["cell"].isNull() && rootDesc["Round"]["cell"].size() >= iRoundID)
			{
				Json::Value RoundCell = rootDesc["Round"]["cell"];
				Json::Value maxScoreJson = RoundCell[iRoundID - 1];

				if (maxScoreJson["Round"].isInt())
				{
					m_iAllRounds = maxScoreJson["Round"].asInt();
				}
				if (maxScoreJson["RoomCard"].isInt())
				{
					m_iNeedRoomCards = maxScoreJson["RoomCard"].asInt();
				}
			}
		}
	}

	m_tagRoundFinish.iRoundTimes = 0;
	m_tagRoundFinish.iAllRoundTimes = m_iAllRounds;


	__super::OnDeskConfigured(szClientConfig,szSysConfig);
	//m_bSingleRoom=true;

	return; 
}

// ���ö�ʱ��
void CMJDesk::SetTimer( int iTimeID, int iTimerInterval )
{
	m_pGameManager->SetGameTimer(m_nDeskIndex, iTimeID, iTimerInterval);
}

void CMJDesk::KillTimer( int iTimeID )
{
	m_pGameManager->KillGameTimer(m_nDeskIndex, iTimeID);
}
//�趨ׯ���¼�
bool CMJDesk::OnEventMakeNT()
{
	DebugPrintf(m_pGameManager->GetRoomID(),m_nDeskIndex,"OnEventMakeNT====>begin");
	Json::Value response;
	if (m_tagRoundFinish.iRoundTimes == 1)
	{
		//�׾����ׯ��
		::srand(GetTickCount());

		//m_byNTUser = rand() % m_iPlayerCount;

		int nDice[2]={0,0};
		nDice[0]=rand()%6+1;
		nDice[1]=rand()%6+1;
		response["dice1"]=nDice[0];
		response["dice2"]=nDice[1];

		m_byNTUser=((nDice[0]+nDice[1])-1)%m_iPlayerCount;


#if PEI_PAI_ON
		//�������������� ������������ׯ
		for (int i= 0 ; i< m_iPlayerCount ; i++)
		{
			if (m_UserData[i].m_bFetchHandCardCheat)
			{
				m_byNTUser = i;
				break;
			}
		}
#endif
	}
	else if(m_tagRoundFinish.iRoundTimes > 1)
	{
		//�ڶ��ֿ�ʼ�Զ�ׯ�Ĺ������£�
		//�����ׯ,����ׯ;ׯ��Ӯ,����ׯ;ׯ��ûӮ,���������ׯ
		if (!GetIsHe())
		{
			//��ȡׯ��ҵ�����
			UserData* pNTUser = &m_UserData[m_byNTUser];
			int nFangPaoUser=255;
			int nCurHuUser=255;
			int nHuUserNum=0;
			bool bIsQiangGang=false;
			bool bHaveDianGangKai = false;

			for (int i= 0 ; i< m_iPlayerCount ; i++ )
			{
				UserData* pUser = &m_UserData[i];
				//20181205
				if (pUser->m_bLastDianGangKai)
				{
					bHaveDianGangKai = true;
				}
				if (MjResult::Res_JiePao == pUser->m_nResult  ||MjResult::Res_ZiMo == pUser->m_nResult )
				{
					nCurHuUser=i;
					nHuUserNum++;

					if (pUser->m_bIsLastRoundQiangGang)
					{
						bIsQiangGang=true;
					}
				}
				else if (MjResult::Res_FangPao==pUser->m_nResult)
				{
					nFangPaoUser=i;
				}
			}


			if (bIsQiangGang==true)
			{
				if (nFangPaoUser!=255)
				{
					m_byNTUser=nFangPaoUser;
				}else
				{
					DebugPrintf(m_pGameManager->GetRoomID(),m_nDeskIndex,"OnEventMakeNT===>error 1");
				}
			}
			else if (bHaveDianGangKai)
			{
				if (nFangPaoUser!=255)
				{
					m_byNTUser=nFangPaoUser;
				}else
				{
					DebugPrintf(m_pGameManager->GetRoomID(),m_nDeskIndex,"OnEventMakeNT===>error 1");
				}
			}
			else
			{
				if (nHuUserNum==1)
				{
					if (nCurHuUser!=255)
					{
						m_byNTUser=nCurHuUser;
					}else
					{
						DebugPrintf(m_pGameManager->GetRoomID(),m_nDeskIndex,"OnEventMakeNT===>error 2");
					}
				}else if (nHuUserNum>1)
				{
					if (nFangPaoUser!=255)
					{
						m_byNTUser=nFangPaoUser;
					}else
					{
						DebugPrintf(m_pGameManager->GetRoomID(),m_nDeskIndex,"OnEventMakeNT===>error 3");
					}
				}else
				{
					DebugPrintf(m_pGameManager->GetRoomID(),m_nDeskIndex,"OnEventMakeNT===>error 4");
				}
			}
		}else
		{
			if (m_byLastFetchCardUser!=255)
			{
				m_byNTUser=m_byLastFetchCardUser;
			}
		}
	}	

	m_byLastFetchCardUser=255;

	//��һ�ֵĽ�ּ�¼��û�����ü�ֵ�����
	for (int i = 0; i < m_iPlayerCount; ++i)
	{
		m_UserData[i].m_nResult = MjResult::Res_Unknown;
		m_UserData[i].m_bIsLastRoundQiangGang=false;
		m_UserData[i].m_bLastDianGangKai=false;
	}

	//�㲥��ׯ���/////////////////////////////////////////////////////////



	//���w���� �˴�ֵ���ܻ᲻�Ϸ�
	if (m_byNTUser == 255)
	{
		::srand(GetTickCount());
		m_byNTUser = rand() % m_iPlayerCount;
	}

	response["NtUser"] = m_byNTUser;
	//TODO ������˵ڶ���,ׯ��Ϊ�Ͼ��Ѿ����Ƶ��Ǹ����
	//��ǰ�ڼ���
	response["RoundTimes"] = m_tagRoundFinish.iRoundTimes;
	response["AllRoundTimes"] = m_tagRoundFinish.iAllRoundTimes;
	for (int i = 0; i < m_iPlayerCount; ++i)
	{
		SendGameData(i,response,ASS_MAKE_NT_INFO);
	}
	this->InsertGameVideo( Video_Event::Video_Event_select_banker , response );

	//��һ��Ҫ�ȿͻ��˲���ѡׯ����
	if(m_tagRoundFinish.iRoundTimes > 1)
	{

		//���������Ʒ������ƶ�λ����
		SetTimer(TIME_FETCH_METHOD, 500);

	}
	else
	{
		//��һ���ж�ׯ����Ҫ��ȴ�ʱ��
		SetTimer(TIME_NT_MORE, 2*1000);
	}
	DebugPrintf(m_pGameManager->GetRoomID(),m_nDeskIndex,"OnEventMakeNT====>end");
	return true;
}
//���ݸ���ҵ�m_nResult�ж��Ƿ�Ϊ��ׯ
///@return �Ƿ��ׯ��
int CMJDesk::GetIsHe()
{
	for (int i = 0; i < m_iPlayerCount; ++i)
	{
		if (MjResult::Res_Unknown != m_UserData[i].m_nResult)
		{
			return false;
		}
	}
	return true;
}


bool CMJDesk::OnEventFetchMethod()
{
	DebugPrintf(m_pGameManager->GetRoomID(),m_nDeskIndex,"OnEventFetchMethod====>begin");
	Json::Value response;
	int nDice[2]={0,0};
	nDice[0]=rand()%6+1;
	nDice[1]=rand()%6+1;
	response["dice1"]=nDice[0];
	response["dice2"]=nDice[1];
	for (int i = 0; i < m_iPlayerCount; ++i)
	{
		SendGameData(i,response,ASS_FETCH_METHOD_INFO);
	}
	SetTimer(TIME_FETCH_HANDCARDS, 2*1000);
	return true;
}

//ϵͳ�������¼�
bool CMJDesk::OnEventFetchHandCards()
{
	DebugPrintf(m_pGameManager->GetRoomID(),m_nDeskIndex,"OnEventFetchHandCards====>begin");
//	// ��������
//	//��ÿ�����ȡ����,���Ƴ���ȡ��
//	int nJinPai = 5;
//	int nMa = 23;
//	int arrNT[] = {21, 21, 21, 22, 22, 22, 23, 23, 23, 24, 24, 24, 1, 9};
//	int arr2Test[][14] =
//	{ 
//		{1, 1, 1, 2, 2, 2, 3, 3, 3, 4, 4, 4, 21, 0},
//		{11, 11, 11, 12, 12, 12, 13, 13, 13, 14, 14, 14, 5, 0},
//		{9, 9, 9, 8, 8, 8, 7, 7, 7, 6, 6, 6, 5, 0},
//		{29, 29, 29, 28, 28, 28, 27, 27, 27, 26, 26, 26,25, 0}
//	};
//	//int **p;
//	//*p = arrTest[m_byNTUser];
//	memcpy( arr2Test[m_byNTUser], arrNT, sizeof(arrNT) );
//
//	int nTotalFetchHandCards = m_iPlayerCount * 13 + 1 + 1 + 1;
//	int *pHand = new int [nTotalFetchHandCards * sizeof(int)];
//	int nOffsetHand = 0;
//	for ( int i = 0; i < m_iPlayerCount; ++i )
//	{
//		int nLengthHand = arr2Test[i][13] == 0 ? 13 : 14;
//		memcpy( pHand + nOffsetHand, arr2Test[i], nLengthHand * sizeof(int) );
//		nOffsetHand += nLengthHand;
//	}
//	memcpy( pHand + nOffsetHand++, &nJinPai, sizeof(int) );
//	memcpy( pHand + nOffsetHand++, &nMa, sizeof(int) );
//	m_byLaiZi = nJinPai;
//#ifndef TEST
//#define TEST
//#endif
//	testCardModel(m_iAllCard, MJ_MAX_CARDS_ZHONG, pHand, nTotalFetchHandCards);
//	delete []pHand;

	for (int i = 0; i < m_iPlayerCount; ++i)
	{
		if (i==m_byNTUser)
		{
			DealHandCardsTo(i,HAND_MAX_NUM_MJ);
		}else
		{
			DealHandCardsTo(i,HAND_MAX_NUM_MJ - 1);
		}
	}

#if PEI_PAI_ON
	for (int i= 0; i< m_iPlayerCount ; i++)
	{

		//�����������÷�������
		if (m_UserData[i].m_bFetchHandCardCheat )
		{
			::memcpy(m_UserData[i].m_byHandCards, m_UserData[i].m_byHandCardsCheat, m_UserData[i].m_iHandNums*sizeof(int));
		}
		m_UserData[i].m_bFetchHandCardCheat = false;
	}
#endif
	//�㲥���ȡ���Ƶ�����/////////////////
	Json::Value video_send_card;
	for (int i = 0; i < m_iPlayerCount; ++i)
	{
		UserData* pUser = &m_UserData[i];

		Json::Value root;
		root["CardNum"] =  pUser->m_iHandNums;
		for ( int k = 0 ; k<m_iPlayerCount ; ++ k )
		{
			UserData *pCurUser=&m_UserData[k];
			for(int j = 0; j < pCurUser->m_iHandNums; ++j)
			{
				if ( i == k )
				{
					root["Cards"][k].append(pCurUser->m_byHandCards[j]);
					video_send_card["Cards"][k].append(  pCurUser->m_byHandCards[j] );
				}
				else
				{
					root["Cards"][k].append(0);
				}
				CString cstr;
				cstr.Format("hebiao_987  %d ",m_UserData[i].m_byHandCards[j]);
				OutputDebugString(cstr);
			}
		}

		root["NTUser"]=m_byNTUser;
		// 20181220 add leisx
		video_send_card["NTUser"] = m_byNTUser;

		//��ǽʣ��������
		video_send_card["WallLeft"] = root["WallLeft"] = GetWallLeft();
		//�·�����

		SendGameData(i,root,ASS_FETCH_HANDCARDS);
	}
	Json::Value piao_value;
	for( int i = 0 ; i < m_iPlayerCount ; ++i )
	{
		if(m_UserData[i].m_bPiao == true)
		{
			piao_value[i] = m_UserData[i].m_iPiaoNum;
		}
		else
		{
			piao_value[i] = 0;
		}
	}

	this->InsertGameVideo( Video_Event::Video_Event_select_Piao , piao_value );

	this->InsertGameVideo( Video_Event::Video_Event_send_card , video_send_card );

	//�㲥ȡ������Ϣ�󣬿ͻ��˲���ȡ���ƶ�������־����Ϸ����GS_FETCH_HANDCARDS״̬������ҷ�λȡ������״̬��
	m_nGameStation = GS_PLAYING;

	//�����ƽ�������(��ֹ��һ���ƶ��������ͻ�������)
	for (int i = 0 ; i< m_iPlayerCount ; i++)
	{
		CMJLogic::sort(m_UserData[i].m_byHandCards,m_UserData[i].m_iHandNums);
	}

	SetTimer(TIME_FETCH_GUI,2500);

	DebugPrintf(m_pGameManager->GetRoomID(),m_nDeskIndex,"OnEventFetchHandCards====>end");
	return true;
}



///������Ϊĳ����ҷ���
///@param bDeskStation���λ�ã�����籣֤��Խ�磻iSendNum����������
///@return 
bool CMJDesk::DealHandCardsTo(BYTE bDeskStation,int iSendNum)
{
	UserData* pUserData = &m_UserData[bDeskStation];

	TMjFetch tagFetch;

	for (int i = 0; i < iSendNum; ++i)
	{
		//��˳ʱ�����ǽȡ��
		do 
		{
			tagFetch.val = m_iAllCard[--m_iMjCardNum];
			m_iAllCard[m_iMjCardNum] = 0;///ץ������Ҫ��Ϊ255
			tagFetch.left = m_iMjCardNum;
		} while (tagFetch.val == 0 && tagFetch.left > 0);
		m_iWallLeft --;
		pUserData->m_byHandCards[i] = tagFetch.val;
	}
	pUserData->m_iHandNums = iSendNum;

	return true;
}

//��Ҽ�Ʈ
bool CMJDesk::OnEventPiao()
{
	for (int i= 0 ; i< m_iPlayerCount ; i++)
	{
		//֪ͨ��ҿ�ʼѡ���Ƿ��Ʈ
		Json::Value response;
		SendGameData(i,response,ASS_PIAO_NOTIFY);
	}
	m_nGameStation = GS_WAIT_PIAO;
	return true;
}


TMjFetch CMJDesk::FetchLaiZiCard()
{
	TMjFetch tagFetch;

	//���ȡ������Ϊ�գ�����ǽ�����ƣ������ȡ��һ����
	int nIndex=-1;
	do 
	{
		nIndex=rand()%m_iMjCardNum;
		tagFetch.val = m_iAllCard[nIndex];
		tagFetch.left=m_iMjCardNum-1;
	} while((tagFetch.val == 0 || tagFetch.val==35) && tagFetch.left>0);

	if (nIndex>=0 && nIndex<sizeof(m_iAllCard)/sizeof(m_iAllCard[0]))
	{
		m_iAllCard[nIndex]=0;
		m_iWallLeft--;
	}


	return tagFetch;
}


bool CMJDesk::OnEventFetchGui()
{
	DebugPrintf(m_pGameManager->GetRoomID(),m_nDeskIndex,"OnEventFetchGui====>begin");


	TMjFetch fetch=FetchLaiZiCard();

#ifdef TEST
	//fetch.val=35;
#else
	m_byLaiZi=fetch.val;
#endif

	Json::Value response;
	response["laizis"]=m_byLaiZi;

	for (int i=0;i<m_iPlayerCount;++i)
	{
		SendGameData(i,response,ASS_FETCH_GUI_INFO);
	}
	this->InsertGameVideo(Video_Event::Video_Event_gui,response);


	//dong add û�в���ֱ�Ӹ�ׯ���·�����
	//Ϊ��Ҹ�����,�����Ƶ���ҽ���ץ�Ƴ��ƽ���,ׯ���ȵ���,������ץ��
	m_tagParam.bCanFetch = false;
	m_tagParam.byNextUser = m_byNTUser;

	SetTimer(TIME_GIVE_NEXT_TOKEN,1500);
	DebugPrintf(m_pGameManager->GetRoomID(),m_nDeskIndex,"OnEventFetchGui====>end");
	return true;
}


//ϵͳΪ��һ������·�����,��������ץ�Ƴ���
bool CMJDesk::OnEventGiveNextToken()
{
	DebugPrintf(m_pGameManager->GetRoomID(),m_nDeskIndex,"OnEventGiveNextToken====>begin");
	TEvTokenParm* pEvParm = &m_tagParam;

	//�����ƽ�/////////////////////////////////////////////////////////////////////////////////////////////


	if (pEvParm->byNextUser != 255) //�ƽ���ָ�����
	{
		//������ֵ�һ�η�����,����Ӧ����ׯ��
		m_byTokenUser = pEvParm->byNextUser;
	}
	else
	{
		//����ʱ�뷽��ȡ��һ�����,�����ƽ���ta
		m_byTokenUser = CMJLogic::GetNextUserStation(m_byTokenUser,false,m_iPlayerCount);
	}

	UserData* pTokenUser = &m_UserData[m_byTokenUser];

	//dong change
	//��һ������  ©��״̬����
	pTokenUser->m_bLouHu = false;
	//���©����Щ��
	::memset(pTokenUser->m_LouHuCard,0,sizeof(pTokenUser->m_LouHuCard));

	pTokenUser->clearLouPeng();

	//ϵͳΪ���ץһ����//////////////////////////////////////////////////////////////////////////////////
	//��λ�����Ƶ���ң���ׯ�ң�����ץ�� 
	//�����Ҳ�Ҫȡ�ƣ����������¼�ǰ��pTokenUser->m_bCanFetch��Ϊfalse��

	TMjFetch tagFetch;
	bool bFetchHua = false;

	if (pEvParm->bCanFetch)
	{
		if (GetWallLeft()-m_iNiaoNums <= 0) //����ȡ��
		{
			//ֱ�ӽ���������Ϸ
			m_RoundFiniParm.bHuang = true;
			SetTimer(TIME_ROUND_FINISH,500);

			DebugPrintf(m_pGameManager->GetRoomID(),m_nDeskIndex,"OnEventGiveNextToken====>1");
			return true;
		}

		//Ϊ���ץһ����
		tagFetch = FetchCardTo(m_byTokenUser);

	}

	DebugPrintf(m_pGameManager->GetRoomID(),m_nDeskIndex,"OnEventGiveNextToken====>chair=%d,card=%d",m_byTokenUser,tagFetch.val);
	//�㲥�����Ƶ���Ϣ////////////////////////////////////////////////////////////////////////////////////
	Json::Value tokenResp;
	Json::Value video_mo;
	tokenResp["TokenbyUser"] = m_byTokenUser;;//������
	tokenResp["TokenbFetch"] = pEvParm->bCanFetch;//�������Ƿ�ȡ��

	for (int i = 0; i < m_iPlayerCount; ++i)
	{
		//ֻ�����Ƶ��߲���֪��ȡ��ֵ�����߲���ʱ�������Ҫ֪��ȡ��ֵ
		int CardVal = (i == m_byTokenUser || bFetchHua) ? tagFetch.val : 0;

		tokenResp["TokenFetcherVal"] = CardVal;//ȡ������
		//dong change

		//��ǽʣ��������
		tokenResp["WallLeft"] = GetWallLeft();

		SendGameData(i,tokenResp,ASS_TOKEN);
		if( i == m_byTokenUser)
		{
			video_mo["moInfo"] = tokenResp;
		}
	}


	//ץ�����ƿ���ʹ���TokenUser�ж���///////////////////////////////////////////////////////////////////
	//��������pTokenUser���ƶ������Ķ�����Ϣ\
	//�����Ķ�����Ϣ�����������pTokenUser������m_tagActInfo��

	USHORT usIgnoreFlags = 0;

	//�Լ����ƿ϶����ܳ���
	usIgnoreFlags |= E_MAHJONG_ACTION::ACT_PENG;

	//û��ץ�Ʋ��ܺ����
	if (!pEvParm->bCanFetch)
	{
		//������ׯ�ҳ��ƣ������ƺ����
		if (!(pTokenUser->m_iOutedTimes == 0 && m_byTokenUser == m_byNTUser))
		{
			usIgnoreFlags |=  E_MAHJONG_ACTION::ACT_HU;
		}
	}

	//��������������ֻ�ܺ�
	if (pTokenUser->m_bIsTing)
	{
		usIgnoreFlags = ~ E_MAHJONG_ACTION::ACT_HU;
	}

	bool bHaveAct = SelectActInfoFrom(255,m_byTokenUser,0,&pTokenUser->m_tagActInfo,tagFetch.val,usIgnoreFlags);
	if (bHaveAct)
	{
		if (pTokenUser->m_tagActInfo.usFlags & E_MAHJONG_ACTION::ACT_HU)
		{
			if (pTokenUser->m_iOutedTimes == 0) //�״�ץ��
			{
				if (m_byTokenUser == m_byNTUser)//ׯ�������
				{
					pTokenUser->m_tagActInfo.nHuSpecWay[0] = emHuSpecialWay::hsw_TianHu;
				}
			}

			//���Ͽ�����pTokenUser->m_nHuSpecWay��pTokenUserִ�и���ʱ�õģ��ڱ��¼�����ǰע�⽫�����
			if (emHuSpecialWay::hsw_DianGangKai == pTokenUser->m_nHuSpecWay[1] || emHuSpecialWay::hsw_AnGangKai == pTokenUser->m_nHuSpecWay[1] || emHuSpecialWay::hsw_BuGangKai == pTokenUser->m_nHuSpecWay[1]) 
			{
				pTokenUser->m_tagActInfo.nHuSpecWay[1] = pTokenUser->m_nHuSpecWay[1];
			}
		}

		for (int i = 0; i < m_iPlayerCount; ++i)
		{
			TActNotify tagActNotify;

			//������i������
			UserData* pUser = &m_UserData[i];

			Json::Value actResp;

			//�ж����Ÿ�������
			if (i == m_byTokenUser)
			{
				::memcpy(&tagActNotify.info,&pUser->m_tagActInfo,sizeof(TMjActInfo));

				actResp["usFlags"] = tagActNotify.info.usFlags;
				//dong add

				if (tagActNotify.info.usFlags & E_MAHJONG_ACTION::ACT_CHI)
				{
					//���Ϳɳ�����Ϣ
					actResp["isCanC"] = true;//�Ƿ���Գ���
					actResp["iChiNums"] = tagActNotify.info.iChiNums;//����ѡ������
					actResp["usChiFlags"] = tagActNotify.info.usChiFlags;//�Եı�־ ��(ͷ��β)
				}
				else
				{
					actResp["isCanC"] = false;
				}

				if (tagActNotify.info.usFlags & E_MAHJONG_ACTION::ACT_PENG)
				{
					//���Ϳ�������Ϣ
					actResp["isCanP"] = true;

				}
				else
				{
					actResp["isCanP"] = false;
				}

				if (tagActNotify.info.usFlags & E_MAHJONG_ACTION::ACT_GANG)
				{
					//���Ϳɸ�����Ϣ

					actResp["isCanG"] = true;
					actResp["iGangNums"] = tagActNotify.info.iGangNums;//���Ը�ѡ������
					for (int j = 0 ; j < MJ_MAX_CGP ; j++)
					{
						actResp["byGangSels"].append(tagActNotify.info.byGangSels[j]);//���Ըܵ�ѡ��
						actResp["nBuGangInPeng"].append(tagActNotify.info.nBuGangInPeng[j]);//�����ڽ�������һ��������
						actResp["nGType"].append(tagActNotify.info.nGType[j]);//������
					}
				}
				else
				{
					actResp["isCanG"] = false;
				}

				if (tagActNotify.info.usFlags & E_MAHJONG_ACTION::ACT_HU)
				{
					//���Ϳɺ�����Ϣ
					actResp["isCanH"] = true;
					actResp["iHuTypeNums"] = tagActNotify.info.iHuTypeNums;

				}
				else
				{
					actResp["isCanH"] = false;
				}
				video_mo["action"] = actResp;
			}

			SendGameData(i,actResp,ASS_ACT_NOTIFY);
		}
	}

	this->InsertGameVideo( Video_Event::Video_Event_mo_card , video_mo );

	//����CUser::m_nHuSpecWay��ת�Ƶ�CUser::m_tagActInfo��
	//��ʱ��Ҫ���CUser::m_nHuSpecWay�ĸ��Ͽ�����־
	pTokenUser->m_nHuSpecWay[1] = emHuSpecialWay::hsw_Unknown;

	if (m_bHaveGang)
	{
		//TokenUser��ʱ��ȡ�ƺ���ƣ��������ҿ��Ժ��Ļ����иܳ�
		//�������������ܷ�����Ȱ������������Ϊ�ܳ壬��TokenUser�ĳ��ƴ���ʱ��ȷ��
		//�Ƿ��CUser::m_nHuSpecWayת��CUser::m_tagActInfo��

		for (int i = 0; i < m_iPlayerCount; ++i)
		{
			if (i != m_byTokenUser)
			{
				//������i������
				UserData* pUser = &m_UserData[i];
				pUser->m_nHuSpecWay[1] = emHuSpecialWay::hsw_GangChong;
			}
		}
	}
	m_bHaveGang = false;



	if (!m_pGameManager->IsPrivateRoom() )
	{
		if (pTokenUser->m_bAuto)//�й�
		{
			//����ǽ�ҷ��� ���Ҹ�����Ѿ��й�
			if (bHaveAct)
			{
				//m_pGameManager->SetGameTimer(m_nDeskIndex,TIME_AUTO_ACT,1000);
				SetTimer(TIME_AUTO_ACT,1000);
				DebugPrintf(m_pGameManager->GetRoomID(),m_nDeskIndex,"OnEventGiveNextToken==>2");
			}
			else
			{
				//m_pGameManager->SetGameTimer(m_nDeskIndex,TIME_AUTO_OUTCARD,1000);
				SetTimer(TIME_AUTO_OUTCARD,1000);
				DebugPrintf(m_pGameManager->GetRoomID(),m_nDeskIndex,"OnEventGiveNextToken===>3");
			}
			DebugPrintf(m_pGameManager->GetRoomID(),m_nDeskIndex,"OnEventGiveNextToken===>4");
			return true;
		}
		else//δ�й�
		{
			///�����ҳ�ʱ�䵽��Ҫ�Զ�����
			if (bHaveAct)
			{
				//m_pGameManager->SetGameTimer(m_nDeskIndex,TIME_AUTO_ACT_NO,m_tagDeskCfg.byAct * 1000);
				SetTimer(TIME_AUTO_ACT_NO,m_tagDeskCfg.byAct * 1000);
				DebugPrintf(m_pGameManager->GetRoomID(),m_nDeskIndex,"OnEventGiveNextToken==>5");
			}
			else
			{
				//m_pGameManager->SetGameTimer(m_nDeskIndex,TIME_AUTO_OUTCARD_NO,m_tagDeskCfg.byOutCard * 1000);
				SetTimer(TIME_AUTO_OUTCARD_NO,m_tagDeskCfg.byOutCard * 1000);
				DebugPrintf(m_pGameManager->GetRoomID(),m_nDeskIndex,"OnEventGiveNextToken==>6");
			}
		}

	}

	DebugPrintf(m_pGameManager->GetRoomID(),m_nDeskIndex,"OnEventGiveNextToken====>end");
	return true;
}

//��ȡʣ���Ƴǵ�������
//@return ʣ������
int CMJDesk::GetWallLeft()
{
	return m_iWallLeft;
}


//������Ϊĳ�����ץһ����
///@param bDeskStation���λ��,bCW����ǽ�ķ���(trueΪ˳ʱ��)��
///@return ����������
TMjFetch CMJDesk::FetchCardTo(BYTE bDeskStation)
{
	TMjFetch tagFetch;

	//���ȡ������Ϊ�գ�����ǽ�����ƣ������ȡ��һ����
	do 
	{
		tagFetch.val = m_iAllCard[--m_iMjCardNum];
		m_iAllCard[m_iMjCardNum] =0;
		tagFetch.left = m_iMjCardNum;

	} while(tagFetch.val == 0 && tagFetch.left > 0);

	if (CMJLogic::Verify(tagFetch.val)) //���ȡ��������Ч��
	{
		if(bDeskStation!=255)
		{
			//ȡ��ץ����ҵ�����
			UserData* pUser = &m_UserData[bDeskStation];

			if (CMJLogic::Verify(pUser->m_byChFetch))
			{
				tagFetch.val = pUser->m_byChFetch;
				pUser->m_byChFetch = 0;
			}

			//��ȡ�����Ƽӵ����bDeskStation������������

			pUser->m_byHandCards[pUser->m_iHandNums++] = tagFetch.val;
			//��¼ȡ�ƴ���
			pUser->m_iFetchTimes++;

			m_byLastFetchCardUser=bDeskStation;
		}



		m_iWallLeft --;
	}

	return tagFetch;
}




///��������ĳ�����ȡ�ö�����Ϣ
///@param byFromUser�����������,ֵΪ255��ʾ�Լ���������ץ�ƺ��ж���;\
byToUser����ִ�����,byCard�����ץ���ƣ�[OUT]pActInfo��ϸ�Ķ�����Ϣ;\
usIgnoreFlags,����Щ�������Բ����Ļ�,��Щ���Ͽ���,ʹ������MjActFlag::usTing���а�λ������
///@return �Ƿ��ж���
bool CMJDesk::SelectActInfoFrom(BYTE byFromUser,BYTE byToUser,int byCard,TMjActInfo* pActInfo,int byFetctCard,USHORT usIgnoreFlags/* = 0*/,bool bIsQiangGangJudge)
{
	//��ʼActInfo
	pActInfo->Clear();

	//������byFromUser������
	UserData* pUserF = NULL;
	if(byFromUser != 255)
	{
		pUserF = &m_UserData[byFromUser];
	}
	//������byToUser������
	UserData* pUserT = &m_UserData[byToUser];


	///����Ǳ��˳�����Ҫ���Ƽ��뵽�Լ��������������У�����Ƿ���Ժ�
	int byTemp[HAND_MAX_NUM_MJ];
	int iTempNums = pUserT->m_iHandNums;
	::memcpy(byTemp,pUserT->m_byHandCards,iTempNums*sizeof(int));

	if (byCard != 0)
	{
		byTemp[iTempNums++] = byCard;
	}
	if (!(usIgnoreFlags & E_MAHJONG_ACTION::ACT_CHI))
	{
#ifdef IS_HAVECHI 
		//�жϿɷ��
		BYTE byNextUser = CMJLogic::GetNextUserStation(byFromUser, false,m_iPlayerCount);
		if (byNextUser == byToUser && byCard != 0)
		{
			if (CMJLogic::IsCanChi(byCard, pUserT->m_byHandCards, pUserT->m_iHandNums, pActInfo,false))
			{
				pActInfo->usFlags |= (E_MAHJONG_ACTION::ACT_CHI | E_MAHJONG_ACTION::ACT_GUO);
			}
		}
#endif
	}
	if (!(usIgnoreFlags & E_MAHJONG_ACTION::ACT_PENG))
	{
		//�ж�����ܷ�����
		if (CMJLogic::IsCanPeng(byCard,pUserT->m_byHandCards,pUserT->m_iHandNums))
		{
			bool bIsCanPeng=true;
			if ( byFromUser != 255 )
			{
				if (byCard != 0 && pUserT->checkIsLouPeng(byCard))
				{
					bIsCanPeng=false;

				}

				if (byCard!=0 && m_byLaiZi==byCard)
				{
					bIsCanPeng=false;
				}
			}

			if (bIsCanPeng)
			{
				pActInfo->usFlags |= (E_MAHJONG_ACTION::ACT_PENG | E_MAHJONG_ACTION::ACT_GUO);
			}
		}
	}

	if (!(usIgnoreFlags & E_MAHJONG_ACTION::ACT_GANG))
	{
		////�ж�����ܷ��
		if (CMJLogic::IsCanGang(byCard,pUserT->m_byHandCards,pUserT->m_iHandNums,
			pUserT->m_lstCGP,pUserT->m_iCGPNums,pActInfo,byFetctCard))
		{
			//�ܺ�ȡ�Ʋ��ܽ��в��ܶ���
			pActInfo->usFlags |= (E_MAHJONG_ACTION::ACT_GANG | E_MAHJONG_ACTION::ACT_GUO);
		}
	}

	if (!(usIgnoreFlags & E_MAHJONG_ACTION::ACT_TING))
	{
		////�ж��ܷ���,ֻ��ץ�Ʋ�������,�����ǵڶ���ץ��ǰ������(��������)
		////DONG change תת�齫��ʱ������ 
		//if (CMJLogic::IsCanTing(pUserT->m_byHandCards,pUserT->m_iHandNums,0,false))
		//{
		//	pActInfo->usFlags |= (E_MAHJONG_ACTION::ACT_TING | E_MAHJONG_ACTION::ACT_GUO);
		//}
	}

	if (!(usIgnoreFlags & E_MAHJONG_ACTION::ACT_HU))
	{
		//dong change
		// Ҫ��������
		if (!(byCard==m_byLaiZi && byFromUser!=255) && CMJLogic::IsCanHu(byCard,byFetctCard,byTemp,iTempNums,pUserT->m_lstCGP,pUserT->m_iCGPNums,pActInfo,m_byLaiZi,NULL,bIsQiangGangJudge))
		{
			//�������Ժ� ���©�����˴���Ĳ��ܺ�
			bool isCanHu = true;
			//if ( byFromUser != 255 )
			//{
			//	if (pUserT->m_bLouHu && byCard != 0)
			//	{

			//		isCanHu = false;
			//	}
			//}

			//������ ֻ������
			if (m_playType == emPlayType::pt_Zimo)
			{
				if (byFromUser != 255 && bIsQiangGangJudge==false)
				{
					isCanHu = false;
				}
			}

			if (isCanHu)
			{
				pActInfo->usFlags |= (E_MAHJONG_ACTION::ACT_HU | E_MAHJONG_ACTION::ACT_GUO);
			}
		}
	}

	return (pActInfo->usFlags > 0);
}

//�õ�ѡׯ������ɵ��������
int CMJDesk::GetMakeNtPlayers()
{
	int iMakeNT = 0;
	for (int i = 0; i < m_iPlayerCount; ++i)
	{
		if (m_UserData[i].m_bMakeNTOK)
		{
			iMakeNT++;
		}
	}
	return iMakeNT;
}



//��ȡָ������������������û�ж���
//@return true��������ж���;false�������û����
bool CMJDesk::IsOtherPlayersHaveAct(BYTE byStation)
{
	for (int i = 0; i < m_iPlayerCount; ++i)
	{
		if (i != byStation)
		{
			if (m_UserData[i].m_tagActInfo.usFlags > 0)
			{
				return true;
			}
		}
	}

	return false;
}

///������Ϊĳһ��ҳ�һ����
///@param bDeskStation���λ��,Ҫ�����ơ�
///@return ����������
void CMJDesk::OutCardTo(BYTE bDeskStation,int byCard)
{
	UserData* pUser = &m_UserData[bDeskStation];
	pUser->m_dianGangGangKaiIndex = -1;
	m_byLastOutCard = byCard;
	//ɾ��һ������
	if (CMJLogic::RemoveOneCard(byCard,pUser->m_byHandCards,pUser->m_iHandNums))
	{
		pUser->m_iHandNums--;

		//������������
		CMJLogic::sort(pUser->m_byHandCards,pUser->m_iHandNums);

		//����Ʊ�����Ӹ���
		pUser->m_byOutedCards[pUser->m_iOutedNums++] = byCard;

		//���³��Ƽ���
		pUser->m_iOutedTimes++;
	}

	return;
}
///������ͨ������ҵĶ���״̬λ���õ����Ͽ���ִ�ж�������Ҽ�
///@param [OUT]byUser�������̽��ж�������ұ�[IN]usActFlag��Щ��ҿɽ��еĶ���
///@return �������̽��ж������������
int CMJDesk::SelectActAtOnceUsers(BYTE byUsers[],USHORT& usActFlag)
{
	usActFlag = 0L;
	//����δ���붯����������������ң�������
	unsigned short usMax1 = 0;
	int byMax1 = 255;

	//���������붯����������������ң�������
	unsigned short usMax2 = 0;
	int byMax2 = 255;
	for (int i = 0; i < PLAY_COUNT; ++i)
	{
		USHORT usFlags = m_UserData[i].m_tagActInfo.usFlags;

		if (usFlags & 0x0003) //�ж��������
		{
			unsigned short usTemp = usFlags & 0xFFFE;

			if (usFlags & 0x0001)
			{
				if (usTemp > usMax2)
				{
					usMax2 = usTemp;
					byMax2 = i;
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

#ifdef IS_TONGPAO 
	//ͨ�������ܺ���ѡ�����Ժ� �����������  (������˲��ú�)
	if (usMax2 > usMax1)
	{
		for (int i = 0; i < PLAY_COUNT; ++i)
		{
			USHORT usFlags = m_UserData[i].m_tagActInfo.usFlags;

			if ((usFlags & 0x0001) && (usFlags & usMax2))
			{
				byUsers[iCounter++] = i;
			}
		}
		usActFlag = usMax2;
	}
#else

	//һ�ڵ���ʱ�� ������������Ŀ�������ִ��

	if (usMax2 > usMax1 || (usMax2&E_MAHJONG_ACTION::ACT_HU && E_MAHJONG_ACTION::ACT_HU & usMax1))
	{
		for (int i = 0; i < PLAY_COUNT; ++i)
		{
			USHORT usFlags = m_UserData[i].m_tagActInfo.usFlags;

			if ((usFlags & 0x0001) && (usFlags & usMax2))
			{
				byUsers[iCounter++] = i;
			}
			else
			{
				if (usFlags &E_MAHJONG_ACTION::ACT_HU)
				{
					byUsers[iCounter++] = i;
				}
			}

		}

		usActFlag = usMax2;
	}

	if (iCounter > 1 && (usMax2&E_MAHJONG_ACTION::ACT_HU || E_MAHJONG_ACTION::ACT_HU & usMax1))
	{
		//dong add ��Ϊֻ���Ե���

		int nearlestPlayer = -1;
		for (int i = 1; i< PLAY_COUNT ; i++ )
		{
			// �������Ƿ���byToUsers ����
			for (int j = 0 ;j< iCounter ; j++)
			{
				if (byUsers [j] == ((m_byTokenUser + i)%4))
				{
					//�ҵ�������
					nearlestPlayer = byUsers [j];
					break;
				}
			}

			if (nearlestPlayer >= 0)
			{
				break;
			}
		}
		//��������������Ǹ�����ʱ���Ժ�
		if (m_UserData[nearlestPlayer].m_tagActInfo.usFlags & 0x0001)
		{
			byUsers[0] = nearlestPlayer;
			iCounter = 1;
		}
		else
		{
			iCounter = 0;
		}
	}



#endif

	return iCounter;
}

//�յ��ͻ��˵ļ�Ʈ����
int CMJDesk::OnUserPiao(BYTE bDeskStation,void* pData,UINT uSize,bool bWatchUser)
{
	Json::Reader reader(Json::Features::strictMode());
	//�յ���ҷ���������
	Json::Value  _root;
	std::string sBuffer = (char *)pData;
	if (!reader.parse(sBuffer, _root))
	{
		return -1;
	}
	if (!(_root["piaoType"].isInt()))
	{
		return -1;
	}
	m_UserData[bDeskStation].m_iPiaoNum = _root["piaoType"].asInt();

	//֪ͨ���������Ʈ
	for (int i=0 ; i<m_iPlayerCount ; i++)
	{
		Json::Value response;
		response["chair"] = bDeskStation;
		response["piaoType"] = _root["piaoType"].asInt();
		SendGameData(i,response,ASS_PIAO_RESP);
	}


	//��������Ҷ���Ʈ���ִ����һ��Event
	m_UserData[bDeskStation].m_bPiao = true;
	m_iPiaoCount++;
	if (m_iPiaoCount == m_iPlayerCount)
	{
		SetTimer(TIME_FETCH_HANDCARDS,500);
	}
	return 0;
}

///���ƣ�OnUserMakeNTOver
///���������������ɲ��Ŷ�ׯ��������
///@param bDeskStation λ��, pNetHead ��Ϣͷ,pData ��Ϣ����,uSize ��Ϣ������,uSocketID id,bWatchUser �Ƿ��Թ�
///@return
int CMJDesk::OnUserMakeNTOver(BYTE bDeskStation,void* pData,UINT uSize,bool bWatchUser)
{
	UserData* pUser = &m_UserData[bDeskStation];

	//����//////////////////////////////////////////////////////////////////

	if (bWatchUser)
	{
		return 0;
	}

	if (pUser->m_bMakeNTOK)
	{
		return 0;
	}

	//����//////////////////////////////////////////////////////////////////

	//�������Ϊ�Ѷ���λ״̬
	pUser->m_bMakeNTOK = true;

	if (GetMakeNtPlayers() < m_iPlayerCount)
	{
		return 0;
	}
	//CloseEventProtection();

	KillTimer(TIME_NT_MORE);
	SetTimer(TIME_NT_MORE,500);

	return 0;
}

///���ƣ�OnUserOutCardRequest
///������������ҳ�������
///@param bDeskStation λ��, pNetHead ��Ϣͷ,pData ��Ϣ����,uSize ��Ϣ������,uSocketID id,bWatchUser �Ƿ��Թ�
///@return
int CMJDesk::OnUserOutCardRequest(BYTE bDeskStation,void* pData,UINT uSize,bool bWatchUser)
{
	DebugPrintf(m_pGameManager->GetRoomID(),m_nDeskIndex,"OnUserOutCardRequest====>chair=%d",bDeskStation);
	if (NULL == pData || 0 == uSize)
	{
		DebugPrintf(m_pGameManager->GetRoomID(),m_nDeskIndex,"���=%d,������󣬶��ߴ���",bDeskStation);
		return -1;
	}

	//dong add
	Json::Reader reader(Json::Features::strictMode());

	//�յ���ҷ���������
	Json::Value  _root;
	std::string sBuffer = (char *)pData;
	if (!reader.parse(sBuffer, _root))
	{
		DebugPrintf(m_pGameManager->GetRoomID(),m_nDeskIndex,"OnUserOutCardRequest====>1");
		return -1;
	}
	if (!(_root["cardvalue"].isInt()))
	{
		DebugPrintf(m_pGameManager->GetRoomID(),m_nDeskIndex,"OnUserOutCardRequest====>2");
		return -1;
	}
	if (!(_root["outcardtype"].isInt()))
	{
		DebugPrintf(m_pGameManager->GetRoomID(),m_nDeskIndex,"OnUserOutCardRequest====>3");
		return -1;
	}
	int cardvalue = _root["cardvalue"].asInt();
	int outcardtype = _root["outcardtype"].asInt();
	//��ó����������
	UserData* pUserF = &m_UserData[bDeskStation];

	//����//////////////////////////////////////////////////////////////////
	DebugPrintf(m_pGameManager->GetRoomID(),m_nDeskIndex,"OnUserOutCardRequest====>value=%d",cardvalue);
	if (bWatchUser)
	{
		DebugPrintf(m_pGameManager->GetRoomID(),m_nDeskIndex,"OnUserOutCardRequest====>4");
		return 0;
	}
	if (outcardtype != TOutCard::REQUEST)
	{
		DebugPrintf(m_pGameManager->GetRoomID(),m_nDeskIndex,"OnUserOutCardRequest====>5");
		return 0;
	}

	//��������ж���δ��Ӧ�����ܳ���
	if (IsOtherPlayersHaveAct(bDeskStation))
	{
		DebugPrintf(m_pGameManager->GetRoomID(),m_nDeskIndex,"OnUserOutCardRequest====>6");
		return 0;
	}

	if (!m_pGameManager->IsPrivateRoom() )
	{
		KillTimer(TIME_AUTO_ACT);
		KillTimer(TIME_AUTO_OUTCARD);
		KillTimer(TIME_AUTO_ACT_NO);
		KillTimer(TIME_AUTO_OUTCARD_NO);
	}

	//������Ҫע��
	if (!pUserF->m_bIsTing)
	{
		//���ɳ��Ƶ�״̬����������������ж�
		if (!CMJLogic::InCards(cardvalue,pUserF->m_byHandCards,pUserF->m_iHandNums))
		{
			DebugPrintf(m_pGameManager->GetRoomID(),m_nDeskIndex,"OnUserOutCardRequest====>7");
			return -1;
		}
	}
	else
	{
		//������������,ֻ�ܳ����ץ��һ��
		if (cardvalue != pUserF->m_byHandCards[pUserF->m_iHandNums - 1])
		{
			DebugPrintf(m_pGameManager->GetRoomID(),m_nDeskIndex,"OnUserOutCardRequest====>8");
			return 0;
		}
	}

	//����///////////////////////////////////////////////////////////////////////////////////////////////
	//�����ҳ���ǰ���ж���������Ҫ��ն�����Ϣ
	if (pUserF->m_tagActInfo.usFlags > 0)
	{
		pUserF->m_tagActInfo.Clear();
	}
	m_nLastPengCard=255;
	m_nLastPengUser=255;
	//Ϊ���bDeskStation���һ����,��ҵ�������ɾ������
	OutCardTo(bDeskStation,cardvalue);

	//�㲥���bDeskStation�ĳ��ƽ��//////////////////////////////////////////////////////////////////////

	TOutCard tagOC;
	tagOC.nType = TOutCard::INFO;
	tagOC.byCard = cardvalue;
	tagOC.byUser = bDeskStation;

	//�����ã������Ʒ����ͻ��ˣ������ƿؼ����ݶԱȡ�
	::memcpy(&tagOC.byCards,pUserF->m_byHandCards,pUserF->m_iHandNums*sizeof(int));
	tagOC.iHandNums = pUserF->m_iHandNums;

	// 20181203��ҳ��ƺ����е��ܸ�ȴ����ʹ�ٸܣ��ܷ�ʧЧ
	//if (!pUserF->m_mapValidGangPoint.empty())
	//{
	//	std::for_each(pUserF->m_mapValidGangPoint.begin(), pUserF->m_mapValidGangPoint.end(), [](std::map<int, bool>::reference a){
	//		a.second = false;
	//	});
	//}
	// memset(pUserF->m_arrValidGangPoint, 0, sizeof(pUserF->m_arrValidGangPoint));

	for (int i = 0; i < 40; ++i)
	{
		if (pUserF->m_arrValidGangPoint[i] == 1)
		{
			pUserF->m_arrValidGangPoint[i] = 2;
		}
	}

	Json::Value tokenResp ;

	for (int i = 0; i < m_iPlayerCount; ++i)
	{		
		//������
		tokenResp["outCardValue"] = tagOC.byCard;
		//���Ƶ����
		tokenResp["outCardUser"] = tagOC.byUser;
		//��������
		tokenResp["outCardNum"] = pUserF->m_iOutedNums;


		//����ʣ������
		tokenResp["handCardNums"] = tagOC.iHandNums;
		tokenResp["OutCardType"] = tagOC.nType;
		for (int j = 0 ; j< HAND_MAX_NUM_MJ ; j++)
		{
			if (tagOC.byCards[j] > 0 && i == bDeskStation )
			{
				tokenResp["HandCards"].append(tagOC.byCards[j]);
			}
		}

		SendGameData(i,tokenResp,ASS_OUT_CARD_INFO);
		tokenResp.clear();
	}

	Json::Value video_out_card;
	video_out_card["OutInfo"]["outCardValue"] = tagOC.byCard;
	video_out_card["OutInfo"]["outCardUser"] =  tagOC.byUser;
	video_out_card["OutInfo"]["outCardNum"] = pUserF->m_iOutedNums;

	//��Ҵ���ƿ��ܻ�������////////////////////////////////////////////////////////////////////////////

	//����������bDeskStation������ƶ������Ķ�����Ϣ\
	//�����Ķ�����Ϣ�����������i������m_tagActInfo��
	bool bHaveAct = false;

	for (int i = 0; i < m_iPlayerCount; ++i)
	{
		if (i != bDeskStation) //�ǳ��ƶ��ſ����ж���
		{
			//������i������
			UserData* pUserT = &m_UserData[i];

			USHORT usIgnoreFlags = 0;

			//�����������ƣ�ֻ�ܺ�
			if (pUserT->m_bIsTing)
			{
				usIgnoreFlags = ~ E_MAHJONG_ACTION::ACT_HU;
			}

			if (SelectActInfoFrom(bDeskStation,i,cardvalue,&pUserT->m_tagActInfo,0,usIgnoreFlags))	
			{
				bHaveAct = true;
			}
		}
	}

	if (bHaveAct)
	{
		for (int i = 0; i < m_iPlayerCount; ++i)
		{
			if (i != bDeskStation)
			{
				//������i������
				UserData* pUser = &m_UserData[i];

				if (pUser->m_tagActInfo.usFlags & E_MAHJONG_ACTION::ACT_HU)
				{
					//�غ����ж�
					if (i != m_byNTUser) //�м�
					{
						if (pUserF->m_iOutedTimes == 1) //�����߳���һ����
						{
							if (bDeskStation == CMJLogic::GetNextUserStation(i,true,m_iPlayerCount)) //�������Ǻ����ߵ��ϼ�
							{
								pUser->m_tagActInfo.nHuSpecWay[0] = emHuSpecialWay::hsw_DiHu;
							}
						}
					}

					//�ܳ壬pTokenUser->m_nHuSpecWay��pTokenUserץ��ʱ�õģ��ڱ��¼�����ǰע�⽫�����
					if (emHuSpecialWay::hsw_GangChong == pUser->m_nHuSpecWay[1]) 
					{
						pUser->m_tagActInfo.nHuSpecWay[1] = emHuSpecialWay::hsw_GangChong;
					}
				}
			}
		}
	}

	//���������ж����������ҷ��Ͷ���֪ͨ
	if (bHaveAct)
	{
		for (int i = 0; i < m_iPlayerCount; ++i)
		{
			TActNotify tagActNotify;

			//������i������
			const UserData* pUser = &m_UserData[i];


			//dong add ���ͻ��˷�������п�ִ�еĶ�������
			Json::Value actResp;

			//�ж����Ÿ�������
			if (pUser->m_tagActInfo.usFlags > 0)
			{
				::memcpy(&tagActNotify.info,&pUser->m_tagActInfo,sizeof(TMjActInfo));
				actResp["usFlags"] = tagActNotify.info.usFlags;

				for (int i = 0 ; i<3 ; i++)
				{
					actResp["byChiSels"].append(tagActNotify.info.byChiSels[i]);
				}


				if (tagActNotify.info.usFlags & E_MAHJONG_ACTION::ACT_CHI)
				{
					//���Ϳɳ�����Ϣ
					actResp["isCanC"] = true;//�Ƿ���Գ���
					actResp["iChiNums"] = tagActNotify.info.iChiNums;//����ѡ������
					actResp["usChiFlags"] = tagActNotify.info.usChiFlags;//�Եı�־ ��(ͷ��β)
				}
				else
				{
					actResp["isCanC"] = false;
				}

				if (tagActNotify.info.usFlags & E_MAHJONG_ACTION::ACT_PENG)
				{
					//���Ϳ�������Ϣ
					actResp["isCanP"] = true;

				}
				else
				{
					actResp["isCanP"] = false;
				}

				if (tagActNotify.info.usFlags & E_MAHJONG_ACTION::ACT_GANG)
				{
					//���Ϳɸ�����Ϣ
					actResp["isCanG"] = true;
					actResp["iGangNums"] = tagActNotify.info.iGangNums;//���Ը�ѡ������
					for (int i= 0 ; i< MJ_MAX_CGP ; i++)
					{
						actResp["byGangSels"].append(tagActNotify.info.byGangSels[i]);//���Ըܵ�ѡ��
					}for (int i= 0 ; i< MJ_MAX_CGP ; i++)
					{
						actResp["nBuGangInPeng"].append(tagActNotify.info.nBuGangInPeng[i]);//�����ڽ�������һ��������
					}

				}
				else
				{
					actResp["isCanG"] = false;
				}

				if (tagActNotify.info.usFlags & E_MAHJONG_ACTION::ACT_HU)
				{
					//���Ϳɺ�����Ϣ
					actResp["isCanH"] = true;
					actResp["iHuTypeNums"] = tagActNotify.info.iHuTypeNums;

				}
				else
				{
					actResp["isCanH"] = false;
				}
				Json::Value video_action;
				video_action["data"] = actResp;
				video_action["chair"] = i;
				video_out_card["action"].append( video_action );
			}

			//dong add
			SendGameData(i,actResp,ASS_ACT_NOTIFY);	
		}
	}
	this->InsertGameVideo( Video_Event::Video_Event_out_card , video_out_card );
	//����CUser::m_nHuSpecWay��ת�Ƶ�CUser::m_tagActInfo��\
	��ʱ��Ҫ���CUser::m_nHuSpecWay�ĸܳ��־
	for (int i = 0; i < m_iPlayerCount; ++i)
	{
		//������i������
		UserData* pUser = &m_UserData[i];
		pUser->m_nHuSpecWay[1] = emHuSpecialWay::hsw_Unknown;
	}

	//������ת��////////////////////////////////////////////////////////////////////////////////////////////

	//��ҳ������,������£�\
	1.���û�����ж���,ϵͳΪ�¸���Ҹ�����\
	2.���ж������������ȴ����ѡ��������Ҫ�����̱���

	if (!bHaveAct)
	{
		m_tagParam.bCanFetch = true;
		m_tagParam.byNextUser = 255;
		SetTimer(TIME_GIVE_NEXT_TOKEN,500);
	}
	else
	{
		if (!m_pGameManager->IsPrivateRoom() )
		{
			//����ǽ�ҷ��� 
			//m_pGameManager->SetGameTimer(m_nDeskIndex,TIME_AUTO_ACT,1000);
			SetTimer(TIME_AUTO_ACT,1000);
		}
		//m_pGameManager->SetGameTimer(m_nDeskIndex,TIME_AUTO_ACT_NO,m_tagDeskCfg.byAct * 1000);
		SetTimer(TIME_AUTO_ACT_NO,m_tagDeskCfg.byAct * 1000);
	}
	DebugPrintf(m_pGameManager->GetRoomID(),m_nDeskIndex,"OnUserOutCardRequest====>end");
	return 0;
}
///���ƣ�OnUserActRequest
///������������ҵĶ�������
///@param bDeskStation λ��, pNetHead ��Ϣͷ,pData ��Ϣ����,uSize ��Ϣ������,uSocketID id,bWatchUser �Ƿ��Թ�
///@return
int CMJDesk::OnUserActRequest(BYTE bDeskStation,void* pData,UINT uSize,bool bWatchUser)
{
	DebugPrintf(m_pGameManager->GetRoomID(),m_nDeskIndex,"OnUserActRequest====>chair=%d",bDeskStation);
	if (NULL == pData || 0 == uSize)
	{
		DebugPrintf(m_pGameManager->GetRoomID(),m_nDeskIndex,"���=%d,������󣬶��ߴ���",bDeskStation);
		return -1;
	}

	Json::Reader reader(Json::Features::strictMode());
	Json::Value  _root;
	std::string sBuffer = (char *)pData;
	if (!reader.parse(sBuffer, _root))
	{
		DebugPrintf(m_pGameManager->GetRoomID(),m_nDeskIndex,"OnUserActRequest====>1");
		return -1;
	}
	if (!(_root["ActFlag"].isInt()))
	{
		DebugPrintf(m_pGameManager->GetRoomID(),m_nDeskIndex,"OnUserActRequest====>2");
		return -1;
	}

	int m_usAct = _root["ActFlag"].asInt();

	//��ó���������ҵ�����
	UserData* pUser = &m_UserData[bDeskStation];

	//�õ���ҵĶ������
	USHORT usActFlags = pUser->m_tagActInfo.usFlags;

	//����//////////////////////////////////////////////////////////////////////////////////////
	DebugPrintf(m_pGameManager->GetRoomID(),m_nDeskIndex,"OnUserActRequest====>sel_act=%d,act=%d",m_usAct,usActFlags);
	if (bWatchUser)
	{
		DebugPrintf(m_pGameManager->GetRoomID(),m_nDeskIndex,"OnUserActRequest====>3");
		return 0;
	}
	//���û�ж���,��������
	if (usActFlags == 0 || (usActFlags & E_MAHJONG_ACTION::ACT_WAIT))
	{
		DebugPrintf(m_pGameManager->GetRoomID(),m_nDeskIndex,"OnUserActRequest====>4");
		return 0;
	}
	//���û��Щ����
	if(!( usActFlags & m_usAct))
	{
		DebugPrintf(m_pGameManager->GetRoomID(),m_nDeskIndex,"OnUserActRequest====>5");
		return 0;
	}

	if (E_MAHJONG_ACTION::ACT_CHI == m_usAct)
	{
		if (!(_root["EatFlag"].isInt()))
		{
			DebugPrintf(m_pGameManager->GetRoomID(),m_nDeskIndex,"OnUserActRequest====>6");
			return -1;
		}
		//��⵱ǰ����Ƿ������ֳԷ� (��ͷ��β)
		if (!((_root["EatFlag"].asInt()) & (pUser->m_tagActInfo.usChiFlags)))
		{
			DebugPrintf(m_pGameManager->GetRoomID(),m_nDeskIndex,"OnUserActRequest====>7");
			return -1;
		}

		//��¼�Ե�״̬ Ҫ��ExecuteChiAct֮ǰ��¼
		m_usChiFlags = _root["EatFlag"].asInt();

	}
	if (E_MAHJONG_ACTION::ACT_HU > m_usAct)
	{
		//���ִ���˹��Ĳ��� �����Ƿ�©��
		if (E_MAHJONG_ACTION::ACT_HU & usActFlags)
		{
			m_UserData[bDeskStation].m_bLouHu = true;
			//��¼©��������
			m_UserData[bDeskStation].addALouHuCard(m_byLastOutCard);
		}
	}

	if (E_MAHJONG_ACTION::ACT_PENG&usActFlags)
	{
		if (m_usAct!=E_MAHJONG_ACTION::ACT_PENG)
		{
			m_UserData[bDeskStation].addALouPengCard(m_byLastOutCard);
		}
	}

	//����ѡ�������쳣,��Ҫ��ԡ��ܡ��롰����������
	if (E_MAHJONG_ACTION::ACT_GANG == m_usAct)
	{
		//dong add ��Ҫ�ͻ��˴������ֵ
		if (!(_root["nSelIndex"].isInt()))
		{
			DebugPrintf(m_pGameManager->GetRoomID(),m_nDeskIndex,"OnUserActRequest====>8");
			return -1;
		}
		int nSelIndex = _root["nSelIndex"].asInt();
		if (pUser->m_tagActInfo.iGangNums < nSelIndex || nSelIndex < 0)
		{
			DebugPrintf(m_pGameManager->GetRoomID(),m_nDeskIndex,"OnUserActRequest====>9");
			return 0;
		}
		pUser->m_tagActInfo.nIndexSel = nSelIndex;
	}
	else if (E_MAHJONG_ACTION::ACT_TING == m_usAct)
	{
		//dong add ��Ҫ�ͻ��˴������ֵ 
		if (!(_root["nSelIndex"].isInt()))
		{
			DebugPrintf(m_pGameManager->GetRoomID(),m_nDeskIndex,"OnUserActRequest====>10");
			return -1;
		}

		int nSelIndex = _root["nSelIndex"].asInt();
		if (pUser->m_tagActInfo.iTingNums < nSelIndex || nSelIndex < 0)
		{
			DebugPrintf(m_pGameManager->GetRoomID(),m_nDeskIndex,"OnUserActRequest====>11");
			return 0;
		}
		pUser->m_tagActInfo.nIndexSel = nSelIndex;
	}

	//��֤�ɹ�������///////////////////////////////////////////////////////////////////////////////
	if (!m_pGameManager->IsPrivateRoom() )
	{
		KillTimer(TIME_AUTO_ACT);
		KillTimer(TIME_AUTO_OUTCARD);
		KillTimer(TIME_AUTO_ACT_NO);
		KillTimer(TIME_AUTO_OUTCARD_NO);
	}
	//������ҵĶ�����־��ֻ����������ض����ı�־
	pUser->m_tagActInfo.usFlags &= m_usAct;

	//����ҵĶ�����־�ó����ڵȴ�״̬
	pUser->m_tagActInfo.usFlags |= E_MAHJONG_ACTION::ACT_WAIT;


	//dong change
	//��¼��ҵĶ�������,��Ҫ��ԡ��ܡ��롰����������
	BYTE byCanActUsers[PLAY_COUNT];
	USHORT usAct = 0;

	//��ȡ����Щ��ҿ�����ִ�ж���
	int iAtOnceCounter = SelectActAtOnceUsers(byCanActUsers,usAct);

	//����п�����ִ�еĶ�����\
	�������̵�ת�򷽷���ÿ�ֶ������������죬�����ɸ��ֶ�����ִ�к��������д���\
	ע�⣬ִ�ж�����ע������������ҵĶ�����¼

	if (iAtOnceCounter > 0)
	{
		//IDEV_GIVE_NEXT_TOKEN�¼������,����IDEV_GIVE_NEXT_TOKEN���ܱ���,Ҫ�ͷű���
		//CloseEventProtection();

		//���iAtOnceCounter == 1,��ôֻ��byCanActUsers[0]ִ�ж���
		//���iAtOnceCounter > 1,�����ж����ҿ�ִ��ͬһ������,��ô���ݶ����������ȴ���


		if(E_MAHJONG_ACTION::ACT_GUO == usAct)
		{
			//Ϊ���ִ�й�����
			ExecuteGuoAct(bDeskStation);

		}
		else if (E_MAHJONG_ACTION::ACT_PENG == usAct)
		{
			//Ϊ���ִ��������
			ExecutePengAct(m_byTokenUser,byCanActUsers[0]);
		}
		else if (E_MAHJONG_ACTION::ACT_GANG == usAct)
		{
			//Ϊ���ִ�иܲ���
			ExecuteGangAct(m_byTokenUser,byCanActUsers[0]);
		}	
		else if (E_MAHJONG_ACTION::ACT_TING == usAct)
		{
			//Ϊ���ִ��������
			ExecuteTingAct(byCanActUsers[0]);
		}
		else if (E_MAHJONG_ACTION::ACT_HU == usAct)
		{
			//Ϊ���ִ�к�����
			ExecuteHuAct(m_byTokenUser,byCanActUsers,iAtOnceCounter);
		}
		else if (E_MAHJONG_ACTION::ACT_CHI == usAct)
		{
			//���ִ�г��ƶ���
			ExecuteChiAct(m_byTokenUser, byCanActUsers[0]);
		}

		if (E_MAHJONG_ACTION::ACT_PENG==usAct)
		{
			m_nLastPengCard=m_byLastOutCard;
			m_nLastPengUser=byCanActUsers[0];
		}else
		{
			m_nLastPengCard=255;
			m_nLastPengUser=255;
		}

		m_usChiFlags = 0;
	}
	DebugPrintf(m_pGameManager->GetRoomID(),m_nDeskIndex,"OnUserActRequest====>end");
	return 0;
}

//ִ�С���������
bool CMJDesk::ExecuteGuoAct(BYTE bDeskStation)
{
	UserData* pUser = NULL;

	// 20181204 ��ʣ������Ƿ��з��ν��������, ���û�з��ν������, ���ν���Ҳ�����������,
	// ����з��ν��������ѡ���, û���ν����������Ϸ����
	pUser = &m_UserData[bDeskStation];
	pUser->m_bNormalQiangGang = false;
	pUser->m_bYouJinQiangGang = false;
	pUser->m_tagActInfo.nHuSpecWay[1] = emHuSpecialWay::hsw_Unknown;
	pUser->m_nHuSpecWay[1]==emHuSpecialWay::hsw_Unknown;
	pUser->m_tagActInfo.Clear();
	bool bYouJinCanQiangGang= false;
	for (int i = 0; i < m_iPlayerCount; ++i)
	{
		if (m_UserData[i].m_bNormalQiangGang)
		{
			bYouJinCanQiangGang = true;
			break;
		}
	}
	if (bYouJinCanQiangGang)
	{
		for (int i = 0; i < m_iPlayerCount; ++i)
		{
			if (m_UserData[i].m_bYouJinQiangGang)
			{
				TActNotify tagActNotify;
				//������i������
				pUser = &m_UserData[i];
				Json::Value actResp;
				::memcpy(&tagActNotify.info, &pUser->m_tagActInfo, sizeof(TMjActInfo));
				actResp["usFlags"] = tagActNotify.info.usFlags;
				actResp["isCanH"] = true;
				actResp["iHuTypeNums"] = tagActNotify.info.iHuTypeNums;
				SendGameData(i,actResp,ASS_ACT_NOTIFY);
				return true;
			}
		}
		// ��Ϸ����
		OnEventRoundFinish();
	}

	//���������û�й�������
	bool bGuoQGang = false;

	for (int i = 0; i < m_iPlayerCount; ++i)
	{
		pUser = &m_UserData[i];
		if (emHuSpecialWay::hsw_QiangGang == pUser->m_tagActInfo.nHuSpecWay[1])
		{
			bGuoQGang = true;
			break;
		}
	}

	//����ҵĶ�����Ϣ�����///////////
	for (int i = 0; i < m_iPlayerCount; ++i)
	{
		m_UserData[i].m_tagActInfo.Clear();
	}

	//����ϵͳΪһ��һ����Ҹ����Ƶ�����,\
	����й������ܻ����,Ҫ��ִ�и�����,��ʱ�벹��

	if (!bGuoQGang)
	{

		m_tagParam.bCanFetch = true;
		m_tagParam.byNextUser = 255;
		SetTimer(TIME_GIVE_NEXT_TOKEN,500);
		return true;
	}
	else
	{
		m_tagParam.bCanFetch = true;
		m_tagParam.byNextUser = m_byTokenUser;
		SetTimer(TIME_GIVE_NEXT_TOKEN,500);
		return true;
	}

	return true;
}

//ִ�С���������
///@param byFromUser ��˭��bToUser ˭����
bool CMJDesk::ExecutePengAct(BYTE byFromUser,BYTE byToUser)
{
	TMjCGPNode tagNode;

	//��ȡ���byToUser������
	UserData* pUserT = &m_UserData[byToUser];

	//���byToUser��byFromUser����
	ActPengTo(byFromUser,byToUser,&tagNode);

	//�����ҹ㲥������Ϣ////////////////////////////////////////////////

	TActInfo tagActInfo;

	//����������
	tagActInfo.byUsers[0] = byToUser;
	tagActInfo.iUserNums = 1;

	//��������
	tagActInfo.usActFlag = E_MAHJONG_ACTION::ACT_PENG;

	//�����������
	tagActInfo.CGPNode = tagNode;

	//������ҵ���������
	tagActInfo.iHandNums = pUserT->m_iHandNums;

	for (int i = 0; i < m_iPlayerCount; ++i)
	{
		if (byToUser == i)
		{
			//ֻ�ж���ִ���ߵĿͻ��˲�֪����������
			::memcpy(tagActInfo.byHands,pUserT->m_byHandCards,sizeof(int)*pUserT->m_iHandNums);
		}
		else
		{
			::memset(tagActInfo.byHands,0,sizeof(tagActInfo.byHands));
		}

		//Ϊ��ҷ�������
		Json::Value response;
		response["iHandNums"] = tagActInfo.iHandNums;
		response["usActFlag"] = tagActInfo.usActFlag;
		response["byFromUser"] = byFromUser;//��˭����
		response["OutCardNum"] = m_UserData[byFromUser].m_iOutedNums;//���³�����ҵĳ�������
		response["byToUser"] = byToUser;//˭������
		//������
		for(int j = 0 ; j< 3 ;j++)
		{
			response["cpgCards"].append(tagActInfo.CGPNode.val[j]);
		}

		//������λ��
		response["nIdx"] = tagActInfo.CGPNode.nIdx;

		if (byToUser == i)
		{
			this->InsertGameVideo( Video_Event::Video_Event_action , response );
		}

		//����ʣ�µ���
		for (int j= 0 ; j< tagActInfo.iHandNums ;j++)
		{
			if (tagActInfo.byHands[j] > 0 )
			{
				response["HandCards"].append(tagActInfo.byHands[j]);
			}

		}

		SendGameData(i,response,ASS_ACT_INFO);
	}


	//����ҵĶ�����Ϣ�����///////////////////////////////
	for (int j = 0; j < m_iPlayerCount; ++j)
	{
		m_UserData[j].m_tagActInfo.Clear();
	}

	//��������������Ϊ��һ����ҷ����Ƶ����̡�����������ó����Ƶ����,����Ҫץ�ơ�
	m_tagParam.bCanFetch = false;
	m_tagParam.byNextUser = byToUser;
	SetTimer(TIME_GIVE_NEXT_TOKEN,500);
	return true;
}

///������ĳ���ִ�����������޸�
///@param byFrom��˭��byTo˭����[OUT]pCGPNode���ص����ƽ��
///@return
bool CMJDesk::ActPengTo(BYTE byFrom,BYTE byTo,TMjCGPNode* pCGPNode)
{
	//�õ����byFrom����
	UserData* pUserF = &m_UserData[byFrom];

	//�õ����byTo����
	UserData* pUserT = &m_UserData[byTo];

	//�õ����byFrom���������,���ӳ��Ʊ���ȡ��
	int byOutCard = pUserF->m_byOutedCards[pUserF->m_iOutedNums - 1];
	pUserF->m_iOutedNums--;

	//�������ƽ��pCGPNode����Ϣ,����ӵ����byTo�����Ʊ���
	pCGPNode->nType = emType::Peng;
	for (int i = 0; i < 3 ; i ++)
	{
		pCGPNode->val[i] = byOutCard;
	}
	pCGPNode->from = byFrom;

	//�õ����Ƶ�λ��
	pCGPNode->nIdx = pUserT->m_iCGPNums;

	pUserT->m_lstCGP[pUserT->m_iCGPNums++] = *pCGPNode;

	//ɾ���������������
	CMJLogic::RemoveCards(pCGPNode->val,2,pUserT->m_byHandCards,pUserT->m_iHandNums);
	pUserT->m_iHandNums -= 2;

	if (CMJLogic::InCards(pCGPNode->val[0], pUserT->m_byHandCards, pUserT->m_iHandNums))
	{
		// pUserT->m_vCanGangSelPengCard.push_back(pCGPNode->val[0]); 
		//pUserT->m_mapValidGangPoint.insert(make_pair(pCGPNode->val[0], true));
		//pUserT->m_mapValidGangPoint[pCGPNode->val[0]] = true;
		pUserT->m_arrValidGangPoint[pCGPNode->val[0]] = 1;
	}


	return true;
}

//ִ�С��ܡ�����
///@param byFromUser ��˭��bToUser ˭��
bool CMJDesk::ExecuteGangAct(BYTE byFromUser,BYTE byToUser)
{
	TMjCGPNode tagNode;

	//��ȡ���byToUser������
	UserData* pUserT = &m_UserData[byToUser];

	//���byToUser��byFromUser����
	int iChPoint[PLAY_COUNT];
	ActGangTo(byFromUser,byToUser,&tagNode,iChPoint);

	//�����ҹ㲥������Ϣ/////////////////////////////////////////////////////////////////////////////////////////////////////////

	TActInfo tagActInfo;

	//����������
	tagActInfo.byUsers[0] = byToUser;
	tagActInfo.iUserNums = 1;

	//��������
	tagActInfo.usActFlag = E_MAHJONG_ACTION::ACT_GANG;

	//�����������
	tagActInfo.CGPNode = tagNode;

	//������ҵ���������
	tagActInfo.iHandNums = pUserT->m_iHandNums;

	for (int i = 0; i < m_iPlayerCount; ++i)
	{
		Json::Value response;
		response["usActFlag"] = tagActInfo.usActFlag;

		response["byFromUser"] = byFromUser;//��˭����
		response["OutCardNum"] = m_UserData[byFromUser].m_iOutedNums;//���³�����ҵĳ�������
		response["byToUser"] = byToUser;//˭�ܵ���

		//������λ��
		response["nIdx"] = tagActInfo.CGPNode.nIdx;

		//��ǽʣ��������
		response["WallLeft"] = GetWallLeft();
		//�ܵ�����
		// ���͵Ķ����ǲ���ʵ��������-���⴦��
		if (pUserT->m_arrValidGangPoint[tagNode.val[0]] == 1 && tagNode.nType == emType::MingGang)
		{
			tagActInfo.CGPNode.nType = emType::BuGang;
		}
		response["gangType"] = tagActInfo.CGPNode.nType;
		//�ܵ���
		for(int j = 0 ; j< 4 ;j++)
		{
			response["cpgCards"].append(tagActInfo.CGPNode.val[j]);
		}


		if (byToUser == i)
		{
			//ֻ�ж���ִ���ߵĿͻ��˲�֪����������
			::memcpy(tagActInfo.byHands,pUserT->m_byHandCards,pUserT->m_iHandNums*sizeof(int));
			//����ʣ�µ���
			for (int j= 0 ; j< tagActInfo.iHandNums ;j++)
			{
				if (tagActInfo.byHands[j] > 0 )
				{
					response["HandCards"].append(tagActInfo.byHands[j]);
				}
			}
		}
		else
		{
			::memset(tagActInfo.byHands,0,sizeof(tagActInfo.byHands));
		}
		response["iHandNums"] = tagActInfo.iHandNums;

		//Ϊ��ҷ�������
		SendGameData(i,response,ASS_ACT_INFO);

		if( byToUser == i )
		{
			this->InsertGameVideo( Video_Event::Video_Event_action , response);
		}
	}



	//����ҵĶ�����Ϣ�����
	for (int i = 0; i < m_iPlayerCount; ++i)
	{
		m_UserData[i].m_tagActInfo.Clear();
	}

	//��Ҳ��ܵ��ƿ�������������Һ���/////////////////////////////////////////////////////////////////////////////////////////////////////////

	//����������bDeskStation������ƶ������Ķ�����Ϣ\
	//�����Ķ�����Ϣ�����������i������m_tagActInfo��
	bool bHaveQGHu = false;
	bool bYouJinCanQiangGang= false;	// 20181129 �ͻ�Ҫ���ν�״̬�������ܣ������������������ܺ�ʱ�����Խ������ܣ���ʱ������Ρ�

	if (tagNode.nType == emType::BuGang)
	{
		for (int i = 0; i < m_iPlayerCount; ++i)
		{
			//������i������
			UserData* pUser = &m_UserData[i];

			if (i != byToUser) //�Ǹܶ���ִ���߿����ж���,ֻ�ܺ�
			{
				USHORT usIgnoreFlags = ~ E_MAHJONG_ACTION::ACT_HU;

				if (SelectActInfoFrom(byToUser,i,tagNode.val[0],&pUser->m_tagActInfo,0,usIgnoreFlags,true))
				{
					//�������ҿ������ܺ�����������������Ϣ

					//���ϸܳ���
					pUser->m_tagActInfo.nHuSpecWay[1] = emHuSpecialWay::hsw_QiangGang;

					//��¼�ܿؼ�λ��
					pUser->m_tagActInfo.nHuGangIdx = tagNode.nIdx;

					bHaveQGHu = true;

					// 20181129 �ͻ�Ҫ���ν�״̬�������ܣ������������������ܺ�ʱ�����Խ������ܣ���ʱ������Ρ�
					bool bYouJin = false;
					for (int i = 0; i < MJ_MAX_HUTYPE; ++i)
					{
						if(pUser->m_tagActInfo.nHuType[i] == ht_YouJin)
						{
							bYouJin = true;
							break;
						}
					}
					if (bYouJin)
					{
						pUser->m_bYouJinQiangGang = true;
					}
					else
					{
						pUser->m_bNormalQiangGang = true;
					}
					// ֻҪ��һ�ҷ��ν�����
					if (pUser->m_bNormalQiangGang)
					{
						bYouJinCanQiangGang = true;
					}

				}
			}
		}

		//�������ҿ������ܺ��������ҷ��Ͷ���֪ͨ
		if (bHaveQGHu)
		{
			for (int i = 0; i < m_iPlayerCount; ++i)
			{
				TActNotify tagActNotify;

				//������i������
				UserData* pUser = &m_UserData[i];
				Json::Value actResp;
				//�ж����Ÿ�������
				if (pUser->m_tagActInfo.usFlags > 0)
				{
					::memcpy(&tagActNotify.info,&pUser->m_tagActInfo,sizeof(TMjActInfo));
					actResp["usFlags"] = tagActNotify.info.usFlags;
					//dong add

					//if (tagActNotify.info.usFlags & E_MAHJONG_ACTION::ACT_CHI)
					//{
					//	//���Ϳɳ�����Ϣ
					//	actResp["isCanC"] = true;//�Ƿ���Գ���
					//	actResp["iChiNums"] = tagActNotify.info.iChiNums;//����ѡ������
					//	actResp["usChiFlags"] = tagActNotify.info.usChiFlags;//�Եı�־ ��(ͷ��β)
					//}
					//else
					//{
					//	actResp["isCanC"] = false;
					//}

					//if (tagActNotify.info.usFlags & E_MAHJONG_ACTION::ACT_PENG)
					//{
					//	//���Ϳ�������Ϣ
					//	actResp["isCanP"] = true;

					//}
					//else
					//{
					//	actResp["isCanP"] = false;
					//}

					//if (tagActNotify.info.usFlags & E_MAHJONG_ACTION::ACT_GANG)
					//{
					//	//���Ϳɸ�����Ϣ
					//	actResp["isCanG"] = true;
					//	actResp["iGangNums"] = tagActNotify.info.iGangNums;//���Ը�ѡ������
					//	for (int i= 0 ; i< MJ_MAX_CGP ; i++)
					//	{
					//		actResp["byGangSels"].append(tagActNotify.info.byGangSels[i]);//���Ըܵ�ѡ��
					//	}for (int i= 0 ; i< MJ_MAX_CGP ; i++)
					//	{
					//		actResp["nBuGangInPeng"].append(tagActNotify.info.nBuGangInPeng[i]);//�����ڽ�������һ��������
					//	}

					//}
					//else
					//{
					//	actResp["isCanG"] = false;
					//}

					if (tagActNotify.info.usFlags & E_MAHJONG_ACTION::ACT_HU)
					{
						////���Ϳɺ�����Ϣ
						//actResp["isCanH"] = true;
						//actResp["iHuTypeNums"] = tagActNotify.info.iHuTypeNums;

						// 20181129 �ͻ�Ҫ���ν�״̬�������ܣ������������������ܺ�ʱ�����Խ������ܣ���ʱ������Ρ�
						if (bYouJinCanQiangGang)
						{
							// �ȷ��͸����ν����
							if (pUser->m_bNormalQiangGang)
							{
								actResp["isCanH"] = true;
								actResp["iHuTypeNums"] = tagActNotify.info.iHuTypeNums;
								SendGameData(i,actResp,ASS_ACT_NOTIFY);
								//continue;
							}
							else
							{
								pUser->m_tagActInfo.usFlags = 0;
								//actResp["isCanH"] = false;
								//continue;
							}
						}
						else
						{
							// ����ν����ܱ��
							pUser->m_tagActInfo.usFlags = 0;
							pUser->m_bYouJinQiangGang = false;
							pUser->m_tagActInfo.nHuSpecWay[1] = emHuSpecialWay::hsw_Unknown;
							 // actResp["isCanH"] = false;
						}
					}
				}

				//Ϊ��ҷ�����Ϣ
				//SendGameData(i,actResp,ASS_ACT_NOTIFY);
			}
		}
	}

	//���û�����ܣ���Ӧ�õ��������byToUserץ�ƣ����ץ���ܺ���Ϊ���Ͽ���\
	����byToUser�ܷ�����Ȱ������������Ϊ���Ͽ�������IDEV_GIVE_NEXT_TOKEN�¼�����ȷ��\
	�Ƿ��CUser::m_nHuSpecWayת��CUser::m_tagActInfo��
	//if (!bHaveQGHu)
	{
		//pUserT->m_nHuSpecWay[1] = emHuSpecialWay::hsw_GangKai;
		if (tagNode.nType==emType::AnGang)
		{
			pUserT->m_nHuSpecWay[1]=emHuSpecialWay::hsw_AnGangKai;
		}else if (tagNode.nType==emType::MingGang)
		{
			pUserT->m_nHuSpecWay[1]=emHuSpecialWay::hsw_DianGangKai;
		}else if (tagNode.nType==emType::BuGang)
		{
			pUserT->m_nHuSpecWay[1]=emHuSpecialWay::hsw_BuGangKai;
			// 201812
			//if (pUserT->m_arrValidGangPoint[tagNode.val[0]] == 1)
			//{
			//	pUserT->m_nHuSpecWay[1]=emHuSpecialWay::hsw_DianGangKai;
			//}
		}
	}

	//���̴���/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//��������ܣ��ȴ������Ӧ����������Ҫ�����̱���
	//û�����ܣ�������Ϊ��һ����ҷ����Ƶ�����

	if (!bHaveQGHu)
	{
		//�´ε���������ó�ִ�и��Ƶ���ң���ʱ��ץ��
		m_tagParam.bCanFetch = true;
		m_tagParam.byNextUser = byToUser;
		m_bHaveGang = true;
		SetTimer(TIME_GIVE_NEXT_TOKEN,500);
	}
	else
	{
		if (bYouJinCanQiangGang)
		{
			//���̱���
			m_byTokenUser = byToUser;
		}
		else
		{
			//�´ε���������ó�ִ�и��Ƶ���ң���ʱ��ץ��
			m_tagParam.bCanFetch = true;
			m_tagParam.byNextUser = byToUser;
			m_bHaveGang = true;
			SetTimer(TIME_GIVE_NEXT_TOKEN,500);
		}
	}

	return true;
}

///������ĳ���ִ�С��ܡ������������޸�
///@param byFrom��˭��byTo˭�ܣ�[OUT]pCGPNode���ص����ƽ�㣬[OUT]iChPoint[] �����������θܶ������������仯
///@return
bool CMJDesk::ActGangTo(BYTE byFrom,BYTE byTo,TMjCGPNode* pCGPNode,int iChPoint[]/* = NULL*/)
{
	//�õ����byFrom����
	UserData* pUserF = &m_UserData[byFrom];

	//�õ����byTo����
	UserData* pUserT = &m_UserData[byTo];

	//��ҵ�ѡ��
	int nIndexSel = pUserT->m_tagActInfo.nIndexSel;

	emGangType nGangType = pUserT->m_tagActInfo.nGType[nIndexSel];

	//�������ݴ���///////////////////////////////////////////////////////////////////////////////////////
	if (emGangType::gt_MingGang == nGangType) 
	{
		//�õ����byFrom���������,���ӳ��Ʊ���ȡ��
		int byOutCard = pUserF->m_byOutedCards[pUserF->m_iOutedNums - 1];
		pUserF->m_iOutedNums--;

		//�������ƽ��pCGPNode����Ϣ,����ӵ����byTo�����Ʊ���
		pCGPNode->nType = emType::MingGang;
		for (int i = 0;i < 4 ; i++)
		{
			pCGPNode->val[i] = byOutCard;
		}
		pCGPNode->from = byFrom;

		//�õ����Ƶ�λ��
		pCGPNode->nIdx = pUserT->m_iCGPNums;
		pUserT->m_dianGangGangKaiIndex = pUserT->m_iCGPNums;
		pUserT->m_lstCGP[pUserT->m_iCGPNums++] = *pCGPNode;

		//ɾ���������������
		CMJLogic::RemoveCards(pCGPNode->val,3,pUserT->m_byHandCards,pUserT->m_iHandNums);
		pUserT->m_iHandNums -= 3;

		//dong add �ִܷ���
		pUserF->iGangPoint -= 1;
		pUserT->iGangPoint += 1;

		//�������ܴ���
		m_tagRoundFinish.iMingGangTimes[byTo] ++;

	}

	//�������ݴ���///////////////////////////////////////////////////////////////////////////////////////
	else if (emGangType::gt_AnGang == nGangType) 
	{
		//ȡ��Ҫ�ܵ���
		int byGangCard = pUserT->m_tagActInfo.byGangSels[nIndexSel];

		//�������ƽ��pCGPNode����Ϣ,����ӵ����byTo�����Ʊ���
		pCGPNode->nType = emType::AnGang;
		for (int i = 0;i < 4 ; i++)
		{
			pCGPNode->val[i] = byGangCard;
		}
		pCGPNode->from = byFrom;

		//�õ����Ƶ�λ��
		pCGPNode->nIdx = pUserT->m_iCGPNums;

		pUserT->m_lstCGP[pUserT->m_iCGPNums++] = *pCGPNode;

		//ɾ���������������
		CMJLogic::RemoveCards(pCGPNode->val,4,pUserT->m_byHandCards,pUserT->m_iHandNums);
		pUserT->m_iHandNums -= 4;

		//dong add �ִܷ���
		for (int i= 0 ; i< m_iPlayerCount ; i++)
		{	
			UserData* pUser = &m_UserData[i];
			if ( i == byTo)
			{
				pUser->iGangPoint += (1*(m_iPlayerCount-1));
			}
			else
			{
				pUser->iGangPoint -= 1;
			}
		}
		//���°��ܴ���
		m_tagRoundFinish.iAnGangTimes[byTo] ++;
	}

	//�������ݴ���///////////////////////////////////////////////////////////////////////////////////////
	else if (emGangType::gt_BuGang == nGangType) 
	{
		//ȡ��Ҫ�ܵ���
		int byGangCard = pUserT->m_tagActInfo.byGangSels[nIndexSel];

		//�������ƽ��pCGPNode����Ϣ,������Ҫ���Ƽӵ����е�������,\
		����pCGPNodeҪ�޸ĵ����Ʊ��У�������ӡ�

		//�õ����Ƶ�λ��
		pCGPNode->nIdx = pUserT->m_tagActInfo.nBuGangInPeng[nIndexSel];

		pCGPNode->nType = emType::BuGang;
		for (int i = 0;i < 4 ; i++)
		{
			pCGPNode->val[i] = byGangCard;
		}
		// 20181204�������ƹ����û�
		int nPengFrom = 0;
		for (int i = 0; i < pUserT->m_iCGPNums; ++i)
		{
			if (pUserT->m_lstCGP[i].val[0] == byGangCard)
			{
				nPengFrom = pUserT->m_lstCGP[i].from;
				break;
			}
		}
		
		pCGPNode->from = byFrom;

		pUserT->m_lstCGP[pCGPNode->nIdx] = *pCGPNode;

		//ɾ���������һ�Ų�����
		CMJLogic::RemoveOneCard(byGangCard,pUserT->m_byHandCards,pUserT->m_iHandNums);
		pUserT->m_iHandNums--;

		//bool bNeedCal = true;
		//for (auto item = pUserT->m_vCanGangSelPengCard.begin(); item != pUserT->m_vCanGangSelPengCard.end(); item++)
		//{
		//	//ѡ�������������ٵĸֲܷ�����
		//	if (*item == byGangCard)
		//	{
		//		bNeedCal = false;
		//		break;
		//	}
		//}

		//if(bNeedCal)
		//{
		//	for (int i= 0 ; i< m_iPlayerCount ; i++)
		//	{	
		//		UserData* pUser = &m_UserData[i];
		//		if ( i == byTo)
		//		{
		//			pUser->iGangPoint += (1*(m_iPlayerCount-1));
		//		}
		//		else
		//		{
		//			pUser->iGangPoint -= 1;
		//		}
		//	}
		//}

		// 20181203 
		//if (pUserT->m_mapValidGangPoint.find(byGangCard) != pUserT->m_mapValidGangPoint.end())
		//{
		//	// ��������,�������
		//	if (pUserT->m_mapValidGangPoint.at(byGangCard))
		//	{
		//		pUserF->iGangPoint -= 1;
		//		pUserT->iGangPoint += 1;
		//	}
		//}
		//else
		//{
		//	// �������Ĳ��ܼ���
		//	for (int i= 0 ; i< m_iPlayerCount ; i++)
		//	{	
		//		UserData* pUser = &m_UserData[i];
		//		if ( i == byTo)
		//		{
		//			pUser->iGangPoint += (1*(m_iPlayerCount-1));
		//		}
		//		else
		//		{
		//			pUser->iGangPoint -= 1;
		//		}
		//	}
		//}
		if (pUserT->m_arrValidGangPoint[byGangCard] == 1)
		{
				// ��������
				// �ڵ���Ϣ����Ϊ����
				pCGPNode->nType = emType::MingGang;
				UserData* pUserPengFrom = &m_UserData[nPengFrom];
				pUserPengFrom->iGangPoint -= 1;
				pUserT->iGangPoint += 1;
				// Ҫ����action�ڵ���Ϣ-����
				//pCGPNode->nType = emType::MingGang;
				//pCGPNode->from  = nPengFrom;
				
				// �û�ʵ���������,�����͵ĸܶ����ǲ��ܣ������������
				TMjCGPNode tMjCGPNode;
				memcpy(&tMjCGPNode, pCGPNode, sizeof(TMjCGPNode));
				tMjCGPNode.nType = emType::MingGang;
				tMjCGPNode.from  = nPengFrom;
				pUserT->m_dianGangGangKaiIndex = tMjCGPNode.nIdx;
				pUserT->m_lstCGP[tMjCGPNode.nIdx] = tMjCGPNode;

		}
		else if (pUserT->m_arrValidGangPoint[byGangCard] == 0)
		{
			// �������Ĳ��ܼ���
			//pCGPNode->nType = emType::BuGang;
			for (int i= 0 ; i< m_iPlayerCount ; i++)
			{	
				UserData* pUser = &m_UserData[i];
				if ( i == byTo)
				{
					pUser->iGangPoint += (1*(m_iPlayerCount-1));
				}
				else
				{
					pUser->iGangPoint -= 1;
				}
			}
		}
		else
		{
			//pCGPNode->nType = emType::BuGang;
			// �ܷ�ʧЧ
		}


		//�������ܴ���
		m_tagRoundFinish.iBuGangTimes[byTo] ++;
	}

	return true;
}

//ִ�С���������
///@param bToUser ˭����
bool CMJDesk::ExecuteTingAct(BYTE byToUser)
{
	//��ȡ���byToUser������
	UserData* pUserT = &m_UserData[byToUser];

	//���byToUser���������ݴ���
	ActTingTo(byToUser);

	//�����ҹ㲥������Ϣ///////////////////////////////////////////////////////////////////////////

	TActInfo tagActInfo;

	//����������
	tagActInfo.byUsers[0] = byToUser;
	tagActInfo.iUserNums = 1;

	//��������
	tagActInfo.usActFlag = E_MAHJONG_ACTION::ACT_TING;

	//������ҵ���������
	tagActInfo.iHandNums = pUserT->m_iHandNums;
	Json::Value response ;
	response["usActFlag"] = tagActInfo.usActFlag;
	response["byToUser"] = byToUser;//˭����
	for (int i = 0; i < m_iPlayerCount; ++i)
	{
		if (byToUser == i)
		{
			//ֻ�ж���ִ���ߵĿͻ��˲�֪����������
			::memcpy(tagActInfo.byHands,pUserT->m_byHandCards,sizeof(int)*pUserT->m_iHandNums);
		}
		else
		{
			::memset(tagActInfo.byHands,0,sizeof(tagActInfo.byHands));
		}

		//Ϊ��ҷ�������
		SendGameData(i,response,ASS_ACT_INFO);
	}

	//����ҵĶ�����Ϣ�����///////////////////////////////
	for (int i = 0; i < m_iPlayerCount; ++i)
	{
		m_UserData[i].m_tagActInfo.Clear();
	}

	return true;
}
///������ĳ���ִ�С����������������޸�
///@param byTo��˭
///@return 
bool CMJDesk::ActTingTo(BYTE byTo)
{
	//�õ����byTo����
	UserData* pUserT = &m_UserData[byTo];

	//�õ���������
	const TMjActInfo* pActInfo = &pUserT->m_tagActInfo;

	//��ҵ�ѡ��
	pUserT->m_bIsTing = true;


	return true;
}
//ִ�гԶ���
bool CMJDesk::ExecuteChiAct(BYTE byFromUser, BYTE byToUser)
{
	TMjCGPNode tagNode;

	//��ȡ���byToUser������
	UserData *pUserT = &m_UserData[byToUser];

	//���byToUser��byFromUser����
	ActChiTo(byFromUser, byToUser, &tagNode);

	//�������ҹ㲥������Ϣ
	TActInfo tagActInfo;

	//�����ķ�����  ���͸�ÿ�����
	tagActInfo.byUsers[0] = byToUser;
	tagActInfo.iUserNums = 1;
	tagActInfo.usActFlag = E_MAHJONG_ACTION::ACT_CHI;
	tagActInfo.CGPNode = tagNode;
	tagActInfo.iHandNums = pUserT->m_iHandNums;
	for (int i = 0; i < m_iPlayerCount; i ++)
	{
		if (byToUser == i)
		{
			::memcpy(tagActInfo.byHands, pUserT->m_byHandCards, sizeof(int)*pUserT->m_iHandNums);
		}
		else 
		{
			::memset(tagActInfo.byHands, 0, sizeof(tagActInfo.byHands));
		}

		Json::Value response;
		response["iHandNums"] = tagActInfo.iHandNums;
		response["usActFlag"] = tagActInfo.usActFlag;
		response["byFromUser"] = byFromUser;//��˭����
		response["OutCardNum"] = m_UserData[byFromUser].m_iOutedNums;//���³�����ҵĳ�������
		response["byToUser"] = byToUser;//˭�Ե���
		response["chiFlag"] = m_usChiFlags;//�� ͷ  ��  β
		//�Ե���
		for(int j = 0 ; j< 3 ;j++)
		{
			response["cpgCards"].append(tagActInfo.CGPNode.val[j]);
		}

		//������λ��
		response["nIdx"] = tagActInfo.CGPNode.nIdx;
		OutputTraceStr("wgd:ExecuteChiAct response[nIdx] = %d",tagActInfo.CGPNode.nIdx);
		//����ʣ�µ���
		for (int j= 0 ; j< tagActInfo.iHandNums ;j++)
		{
			if (tagActInfo.byHands[j] > 0 )
			{
				response["HandCards"].append(tagActInfo.byHands[j]);
			}
		}

		//���͸��������
		SendGameData(i,response,ASS_ACT_INFO);
	}

	//���������ҵ���Ϣ
	for (int i = 0; i < m_iPlayerCount; i ++)
	{
		m_UserData[i].m_tagActInfo.Clear();
	}

	//�´ε����������ó����Ƶ����
	m_byTokenNextUser = byToUser;

	//��������������Ϊ��һ����ҷ����Ƶ����̡�����������ó����Ƶ����,����Ҫץ�ơ�
	m_tagParam.bCanFetch = false;
	m_tagParam.byNextUser = byToUser;
	SetTimer(TIME_GIVE_NEXT_TOKEN,500);

	return true;
}


//ִ��ĳ��ҡ��ԡ��������޸�����
bool CMJDesk::ActChiTo(BYTE byFrom, BYTE byTo, TMjCGPNode* pCGPNode)
{
	//�õ����byFrom������
	UserData *pUserF = &m_UserData[byFrom];

	//�õ����byTo������
	UserData *pUserT = &m_UserData[byTo];

	//�õ����byFrom�������������,���Ҵ��Ʊ���ȡ��
	int byOutCard = pUserF->m_byOutedCards[pUserF->m_iOutedNums - 1];
	pUserF->m_iOutedNums --;

	int delVal[2];
	pCGPNode->nType = emType::Chi;
	if (m_usChiFlags == MjChiFlag::usT)
	{
		pCGPNode->val[0] = byOutCard;
		pCGPNode->val[1] = byOutCard + 1;
		pCGPNode->val[2] = byOutCard + 2;
		delVal[0] = byOutCard + 1;
		delVal[1] = byOutCard + 2;
	}
	else if (m_usChiFlags == MjChiFlag::usZ)
	{
		pCGPNode->val[0] = byOutCard - 1;
		pCGPNode->val[1] = byOutCard;
		pCGPNode->val[2] = byOutCard + 1;
		delVal[0] = byOutCard - 1;
		delVal[1] = byOutCard + 1;
	}
	else if (m_usChiFlags == MjChiFlag::usW)
	{
		pCGPNode->val[0] = byOutCard - 2;
		pCGPNode->val[1] = byOutCard - 1;
		pCGPNode->val[2] = byOutCard;
		delVal[0] = byOutCard - 2;
		delVal[1] = byOutCard - 1;
	}
	pCGPNode->from = byFrom;

	//�õ����Ƶ�λ��
	pCGPNode->nIdx = pUserT->m_iCGPNums;

	pUserT->m_lstCGP[pUserT->m_iCGPNums++] = *pCGPNode;
	CMJLogic::RemoveCards(delVal, 2, pUserT->m_byHandCards, pUserT->m_iHandNums);
	pUserT->m_iHandNums -= 2;
	return 0;
}

///ִ�С���������
///@param byFromUser���ڵ���ң�byToUsers[] �ܺ������Deskstation��iUserNums �ܺ����������
bool CMJDesk::ExecuteHuAct(BYTE byFromUser,BYTE byToUsers[],int iUserNums)
{
	//��ȡ���byFromUser������
	UserData* pUserF = &m_UserData[byFromUser];

	//�޸���Ϸ����
	ActHuTo(byFromUser,byToUsers,iUserNums);

	//�����ҹ㲥������Ϣ///////////////////////////////////////////////////////////////////////////

	TActInfo tagActInfo;

	//��������
	tagActInfo.usActFlag = E_MAHJONG_ACTION::ACT_HU;

	//����������
	tagActInfo.byFromUser = byFromUser;

	//����������
	::memcpy(tagActInfo.byUsers,byToUsers,sizeof(byToUsers));
	tagActInfo.iUserNums = iUserNums;

	//�����������
	for (int i = 0; i < iUserNums; ++i)
	{
		UserData* pUser = &m_UserData[byToUsers[i]];

		if (emHuSpecialWay::hsw_QiangGang == pUser->m_tagActInfo.nHuSpecWay[1])
		{
			pUser->m_nHuSpecWay[1] = emHuSpecialWay::hsw_QiangGang;
			pUser->m_bIsLastRoundQiangGang=true;

			tagActInfo.bQGang = true;

			tagActInfo.nQGangIdx = pUser->m_tagActInfo.nHuGangIdx;

		}
	}

	// ��ÿ����ҷ��ͺ�����ҵ�����
	Json::Value response ;
	response["usActFlag"] = tagActInfo.usActFlag;
	response["byFromUser"] = byFromUser;//˭����
	response["OutCardNum"] = m_UserData[byFromUser].m_iOutedNums;//���³�����ҵĳ�������

	for (int i= 0; i< iUserNums ; i++)
	{
		UserData* pUserT = &m_UserData[byToUsers[i]];
		response["byToUsers"].append(byToUsers[i]);//�������

		for (int j= 0 ; j< pUserT->m_iHandNums ; j++)
		{
			response["HandCards"][i].append(pUserT->m_byHandCards[j]);
		}
	}
	//response["byToUsers"] = byToUsers[0];//�������

	//�����������
	UserData* pUserT = &m_UserData[byToUsers[0]];
	//response["iHandNums"] = pUserT->m_iHandNums;//������ҵ���������
	bool isZimo = (pUserT->m_nResult == MjResult::Res_ZiMo);
	response["isZiMo"] = isZimo;//��������Ƿ�Ϊ����

	for (int i = 0; i < m_iPlayerCount; ++i)
	{
		UserData* pUser = &m_UserData[i];

		//������������
		::memcpy(tagActInfo.byHands,pUser->m_byHandCards,sizeof(int)*pUser->m_iHandNums);

		//�����������
		tagActInfo.iHandNums = pUser->m_iHandNums;

		//Ϊ��ҷ�������
		SendGameData(i,response,ASS_ACT_INFO);
	}

	response["iHandNums"].clear();
	response["HandCards"].clear();
	this->InsertGameVideo( Video_Event::Video_Event_action , response );

	/*****20181204���ܺ���������ν���ң���Ҫ��Ӧ�ν����*****/
	//���͸����ν����
	////pUserT->m_bNormalQiangGang = false;  //-->��ʱ���������
	//pUserT->m_bYouJinQiangGang = false;
	//pUserT->m_tagActInfo.usFlags = 0;
	// 2�ҷ��ν�����1�ν����ܳ��ֵ����
	for (int i = 0; i < iUserNums; ++i)
	{
		UserData* pUser = &m_UserData[byToUsers[i]];
		pUser->m_bYouJinQiangGang = false;
		pUser->m_tagActInfo.usFlags = 0;
	}

	for (int i = 0; i < m_iPlayerCount; ++i)
	{
		TActNotify tagActNotify;
		//������i������
		UserData* pUser = &m_UserData[i];

		if (pUser->m_bYouJinQiangGang)
		{
			pUser->m_tagActInfo.usFlags  |= (E_MAHJONG_ACTION::ACT_HU | E_MAHJONG_ACTION::ACT_GUO);
			pUser->m_tagActInfo.nHuSpecWay[1] = emHuSpecialWay::hsw_QiangGang;
			pUser->m_nHuSpecWay[1]==emHuSpecialWay::hsw_QiangGang;

			Json::Value actResp;
			::memcpy(&tagActNotify.info, &pUser->m_tagActInfo, sizeof(TMjActInfo));
			actResp["usFlags"] = tagActNotify.info.usFlags;
			actResp["isCanH"] = true;
			actResp["iHuTypeNums"] = tagActNotify.info.iHuTypeNums;
			SendGameData(i,actResp,ASS_ACT_NOTIFY);

			//���̱���
			m_byTokenUser = byFromUser;

			// ����, ���ܲ���Ҫץ��
			return true;
		}
	}

	//����ҵĶ�����Ϣ�����///////////////////////////////
	for (int i = 0; i < m_iPlayerCount; ++i)
	{
		m_UserData[i].m_tagActInfo.Clear();
	}

	//��������������Ϊ��һ����ҷ����Ƶ�����
	m_RoundFiniParm.bHuang = false;
	//dong change
	//�������ץ��

	bool bFectchBird=true;
	//if (m_bIsYiMaQuanZhong==true)
	//{
	//	if (pUserT->checkIsGangKai() || pUserT->checkIsQiangGang())
	//	{
	//		bFectchBird=false;
	//	}
	//}else
	//{
	//	if (pUserT->checkIsQiangGang()||pUserT->checkIsGangKai())
	//	{
	//		bFectchBird=false;
	//	}
	//}
	if (pUserT->checkIsGangKai() || pUserT->checkIsQiangGang() || pUserT->IsHuDianGang())
	{
		bFectchBird=false;
	}

	if (bFectchBird==true)
	{
		SetTimer(TIME_FETCH_BIRD,1500);
	}else
	{
		m_RoundFiniParm.bHuang = false;
		// SetTimer(TIME_ROUND_FINISH,1500);
		// 20181212 Ҫ��ʱ��Ķ�һ��
		SetTimer(TIME_ROUND_FINISH,500);
	}

	return true;
}

///������ĳ���ִ�С����������������޸�
///@param byFrom���ڵ���ң�byToUsers[] �ܺ������Deskstation��iUserNums �ܺ����������
///  ��iUserNums > 1  ��������
///  ��iUserNums == 1 �� byFromUser = byToUsers[0] ʱ����������
bool CMJDesk::ActHuTo(BYTE byFrom,BYTE byToUsers[],int iUserNums)
{
	//�õ����byFrom����
	UserData* pUserF = &m_UserData[byFrom];

	//�õ����byTo����
	UserData* pUserT = NULL;

	if (iUserNums > 1) //����///////////////////////////////////////////////////////////////////////////////////////////////////////
	{
		//һ��������
		pUserF->m_nResult = MjResult::Res_FangPao;

		int byOutCard = 0;

		pUserT = &m_UserData[byToUsers[0]];

		if (emHuSpecialWay::hsw_QiangGang == pUserT->m_tagActInfo.nHuSpecWay[1]) //����
		{
			//�õ������ܵĸ�����
			TMjCGPNode* pCGPNode = &pUserF->m_lstCGP[pUserT->m_tagActInfo.nHuGangIdx];

			//�õ����byFrom��������ĸ��ƣ��Ӷ�Ӧ�ĸ��ƿؼ���ɾ�����ƣ�ʹ�ܱ����
			byOutCard = pCGPNode->val[0];
			pCGPNode->nType = emType::Peng;
			pCGPNode->val[3] = 0;

			// 20181207 ������ܸ�ȴ����������ƣ���ֱܷ���û�ӽ����������ܲ�����, ��֮����
			if (pUserF->m_arrValidGangPoint[byOutCard] == 0)
			{
				//���������Ҫ�˻�֮ǰ�ܵ��Ǹ��˵�Ӯ��Ǯ
				for (int i = 0 ; i< m_iPlayerCount ; i++)
				{
					if (i != byFrom)
					{
						m_UserData[byFrom].iGangPoint -= 1;
						m_UserData[i].iGangPoint += 1;
					}
				}
				// �ܷ��Ѿ�����, �����ظ��˸ܷ�
				pUserF->m_arrValidGangPoint[byOutCard] = 2;
			}

			m_tagRoundFinish.iBuGangTimes[byFrom]--;

		}
		else //��ͨ����
		{
			//�õ����byFrom��������ƣ����ӳ����б��п۳�
			byOutCard = pUserF->m_byOutedCards[--pUserF->m_iOutedNums];
		}

		//ʣ�µ������byToUsers������Ҫ����byOutCard��������
		for (int i = 0; i < iUserNums; ++i)
		{
			pUserT = &m_UserData[byToUsers[i]];

			pUserT->m_nResult = MjResult::Res_JiePao;
			pUserT->m_byHandCards[pUserT->m_iHandNums++] = byOutCard;

			//��¼������
			pUserT->m_byHuCard = byOutCard;
		}

	}
	else 
	{
		pUserT = &m_UserData[byToUsers[0]];

		int byOutCard = 0;

		if (byFrom != byToUsers[0]) //����////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		{
			pUserF->m_nResult = MjResult::Res_FangPao;
			pUserT->m_nResult = MjResult::Res_JiePao;

			if (emHuSpecialWay::hsw_QiangGang == pUserT->m_tagActInfo.nHuSpecWay[1]) //����
			{
				// 20181207���ܺ����ܴ�����������
				memset(pUserT->m_tagActInfo.nHuType, 0, sizeof(pUserT->m_tagActInfo.nHuType));
/*				pUserT->m_tagActInfo.nHuType[0] = ht_QiangGangHu;
				pUserT->m_tagActInfo.iHuTypeNums = 1;*/	

				//�õ������ܵĸ�����
				TMjCGPNode* pCGPNode = &pUserF->m_lstCGP[pUserT->m_tagActInfo.nHuGangIdx];

				//�õ����byFrom��������ĸ��ƣ��Ӷ�Ӧ�ĸ��ƿؼ���ɾ�����ƣ�ʹ�ܱ����
				byOutCard = pCGPNode->val[0];
				pCGPNode->nType = emType::Peng;
				pCGPNode->val[3] = 0;

				// 20181207 ������ܸ�ȴ����������ƣ���ֱܷ���û�ӽ����������ܲ�����, ��֮����
				// ���ν���Һ��ν������ʽ�Ϲ��ɵ���һ�ڶ��죬��Ӧ��ȴ����˳��ĵ��죬ע���ظ��˸ܷ�
				if (pUserF->m_arrValidGangPoint[byOutCard] == 0)
				{
				//���������Ҫ�˻�֮ǰ�ܵ��Ǹ��˵�Ӯ��Ǯ
					for (int i = 0 ; i< m_iPlayerCount ; i++)
					{
						if (i != byFrom)
						{
							m_UserData[byFrom].iGangPoint -= 1;
							m_UserData[i].iGangPoint += 1;
						}
					}

					// �ܷ��Ѿ�����, �����ظ��˸ܷ�
					pUserF->m_arrValidGangPoint[byOutCard] = 2;
				}

				m_tagRoundFinish.iBuGangTimes[byFrom]--;
			}
			else //��ͨ����
			{

				//�õ����byFrom��������ƣ����ӳ����б��п۳�
				byOutCard = pUserF->m_byOutedCards[--pUserF->m_iOutedNums];
			}

			//��byOutCard�ӵ����pUserT�����ϱ�ʾ����
			pUserT->m_byHandCards[pUserT->m_iHandNums++] = byOutCard;

			//��¼������
			pUserT->m_byHuCard = byOutCard;

		}
		else //����////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		{
			pUserT->m_nResult = MjResult::Res_ZiMo;

			//��¼������,Ҳ�������ץ��һ����
			pUserT->m_byHuCard = pUserT->m_byHandCards[pUserT->m_iHandNums - 1];
		}
	}

	//��Щ��CUserData::m_tagActInfo�е�����ת�Ƶ�CUserData��,�Ա����ʹ�á�
	//ע��CUserData::m_tagActInfo���ݽ���ActHuTo���пɱ��������,����ת�Ƶ�CUserData���Ǽ��б�Ҫ��

	for (int i = 0; i < m_iPlayerCount; ++i)
	{
		UserData* pUser = &m_UserData[i];

		//��¼���Ƶ����ͱ��䳤��
		::memcpy(pUser->m_nHuTypes,pUser->m_tagActInfo.nHuType,sizeof(pUser->m_nHuTypes));
		pUser->m_iHuTypeNums = pUser->m_tagActInfo.iHuTypeNums;

		//��¼���������Ʒ�ʽ
		pUser->m_nHuSpecWay[0] = pUser->m_tagActInfo.nHuSpecWay[0];
		pUser->m_nHuSpecWay[1] = pUser->m_tagActInfo.nHuSpecWay[1];
	}

	return true;
}
//���ץ��
bool CMJDesk::OnEventFetchNiao()
{
	DebugPrintf(m_pGameManager->GetRoomID(),m_nDeskIndex,"OnEventFetchNiao====>begin");
	m_bHaveFetchBird=true;

	int zhuaniaoUser = m_byTokenUser;
	int jiepaoUserNum = 0;	
	int fangPaoUser = 0;
	int huUser = 255;
	for (int j = 0; j<PLAY_COUNT ; j++)
	{
		if (MjResult::Res_JiePao == m_UserData[j].m_nResult)
		{
			jiepaoUserNum ++;
			huUser = j;
		}
		else if (MjResult::Res_FangPao == m_UserData[j].m_nResult) //else if (MjResult::Res_JiePao == m_UserData[j].m_nResult) 20181206 -->
		{
			fangPaoUser = j;
		}
		else if(MjResult::Res_ZiMo == m_UserData[j].m_nResult)
		{
			huUser = j;
		}
	}
	if (huUser != 255)
	{
		zhuaniaoUser = huUser;
	}

#ifdef IS_TONGPAO 
	if (jiepaoUserNum > 1)
	{
		zhuaniaoUser = fangPaoUser;
	}
#endif


	int iHandCardsTemp[HAND_MAX_NUM_MJ];
	int iHandCardNumsTemp = m_UserData[zhuaniaoUser].m_iHandNums;
	::memcpy(iHandCardsTemp,m_UserData[zhuaniaoUser].m_byHandCards,sizeof(int) * iHandCardNumsTemp);

	//dong add ץ��
	Json::Value response ;
	for (int i= 0 ; i< m_iNiaoNums ; i++)
	{
		//Ϊ���Ƶ����ץ����

		if (GetWallLeft() > 0)
		{
			TMjFetch tagFetch = FetchCardTo(255);
			response["niaoCards"].append(tagFetch.val);
			//��������
			m_niaoCardValues[i] = tagFetch.val;
			//ץ�������ƴ��������Ƴ�
			//OutCardTo(zhuaniaoUser,tagFetch.val);
		}
		else
		{
			response["niaoCards"].append(-1);
		}
	}

	m_UserData[zhuaniaoUser].m_iHandNums = iHandCardNumsTemp;
	::memcpy(m_UserData[zhuaniaoUser].m_byHandCards,iHandCardsTemp,sizeof(int) * iHandCardNumsTemp);

	//���Ƶ���� TODO

	/*for (int i= 0 ; i< m_iPlayerCount ; i++)
	{
	if (m_UserData[i].m_nResult == MjResult::Res_JiePao || 
	m_UserData[i].m_nResult == MjResult::Res_ZiMo)
	{
	response["huUser"] = i;
	break;
	}
	}*/

	if (huUser!=255)
	{

		for (int i=0;i<m_iPlayerCount;++i)
		{

			if (m_UserData[i].m_nResult==MjResult::Res_JiePao || m_UserData[i].m_nResult==MjResult::Res_ZiMo)
			{
				int nCurUser=(i-m_byNTUser+m_iPlayerCount)%m_iPlayerCount;
				for (int j=0;j<m_iNiaoNums;++j)
				{
					int niao=m_niaoCardValues[j];
					if (m_bIsYiMaQuanZhong==false)
					{
						if (niao<35)
						{
							if (nCurUser==((niao%10)-1)%PLAY_COUNT)
							{
								m_bIsZhongNiao[j]=true;
							}
						}else if (niao<=37)
						{
							if (nCurUser==((niao-35)%10)%PLAY_COUNT)
							{
								m_bIsZhongNiao[j]=true;
							}
						}
					}else
					{
						// һ��ȫ��
						m_bIsZhongNiao[j]=true;
					}

				}
			}

		}
	}

	for (int i=0;i<m_iNiaoNums;++i)
	{
		response["bIsZhongNiao"].append(m_bIsZhongNiao[i]);
	}

	if (zhuaniaoUser != 255)
	{
		response["ZhuaNiaoUser"] = zhuaniaoUser;
	}

	//��ǽʣ��������
	response["WallLeft"] = GetWallLeft();

	//Ϊÿ����ҹ㲥����
	for (int i= 0 ; i< m_iPlayerCount ; i++)
	{
		SendGameData(i,response,ASS_NIAOCARDS_INFO);
	}
	this->InsertGameVideo( Video_Event::Video_Event_niao , response);
	//ץ�����غϽ���
	m_RoundFiniParm.bHuang = false;
	SetTimer(TIME_ROUND_FINISH,2000);

	DebugPrintf(m_pGameManager->GetRoomID(),m_nDeskIndex,"OnEventFetchNiao====>end");
	return true;
}


//һ�غϽ������¼�����
bool CMJDesk::OnEventRoundFinish()
{
	DebugPrintf(m_pGameManager->GetRoomID(),m_nDeskIndex,"OnEventRoundFinish====>begin");
	TEvRoundFiniParm* pEvParm = &m_RoundFiniParm;

	//��������
	Json::Value response;

	//�㲥һ�غ���Ϸ����֪ͨ����־����Ϸ����GS_WAIT_NEXT_ROUND״̬��һ�غϽ���״̬��
	m_nGameStation = GS_WAIT_NEXT_ROUND;

	if (!m_pGameManager->IsPrivateRoom())
	{
		//��ҳ�
		m_nGameStation = GS_WAIT_ARGEE;
	}

	//�����غϽ�������,�ǻ�ׯʱҪ�ȴ����ƶ������ţ���Ҫ��ʱ��һ��
	if (!pEvParm->bHuang)
	{
		// 20181213�ͻ�����Ҫʱ���ӳ�
		//SetTimer(TIME_RESULT,2500);
		SetTimer(TIME_RESULT,500);
	}
	else
	{
		m_bHuangZhuang = true;
		SetTimer(TIME_RESULT,500);
	}
	DebugPrintf(m_pGameManager->GetRoomID(),m_nDeskIndex,"OnEventRoundFinish====>end");
	return true;
}

//����¼�����
bool CMJDesk::OnEventResult()
{
	DebugPrintf(m_pGameManager->GetRoomID(),m_nDeskIndex,"OnEventResult====>begin");
	//killAllTimer();
	TResult tagResult;

	UserData* pUser = NULL;

	//��ʼ�������
	for (int i = 0; i < m_iPlayerCount; ++i)
	{
		pUser = &m_UserData[i];

		//��ȡ��ҵ���Ӯ��־,����¼����
		if (pUser->m_nResult == MjResult::Res_ZiMo || pUser->m_nResult == MjResult::Res_JiePao)
		{
			//��ȡ���i���������ʽ
			::memcpy(tagResult.nHuSpecWay[i],pUser->m_nHuSpecWay,sizeof(pUser->m_nHuSpecWay));

			//��ȡ���i�ĺ������ͼ�����
			::memcpy(tagResult.nHuTypes[i],pUser->m_nHuTypes,sizeof(pUser->m_nHuTypes));
			tagResult.iHuTypeNums[i] = pUser->m_iHuTypeNums;

			tagResult.bHu[i] = true;
			tagResult.iHuNums++;
			tagResult.byHuUser = i;
		}

		//��ȡ���ڵ����
		if (pUser->m_nResult == MjResult::Res_FangPao)
		{
			tagResult.byFangPaoUser = i;
		}
	}

	//������Ϸ�����㷨
	CalcRoundFen(tagResult);

	if (GetIsHe())
	{
		for (int i=0;i<m_iPlayerCount;++i)
		{
			m_tagRoundFinish.iLiuJuTimes[i]++;
		}
	}

	//������ҽ��
	for (int i = 0; i< m_iPlayerCount ; i++)
	{
		///������ҽ����ұ䶯����
		m_ResultUserData[i]._nPoint = tagResult.iTotalPoint[i];
		//������ҵ�ǰ���
		m_ResultUserData[i]._iUserMoney += tagResult.iTotalPoint[i];
	}

	//д�����ݿ�				
	bool bNetCutUser[PLAY_COUNT];
	memset(bNetCutUser,0,sizeof(bNetCutUser));

	//TODO������ҽ��
	ResultInfo_t result;
	result._nCount = m_iPlayerCount;//�����ϵ�����
	result._nDeskIndex = m_nDeskIndex;//���Ӻ�


	//�������ݿ���ҽ������
	int changeResult = m_pGameManager->ChangeUserPoint(result,m_ResultUserData);
	if (changeResult != 0)
	{
		OutputTraceStr("wgd: update user score failed   changeResult = %d",changeResult);
	}
	//�ϴ�ս��
	if (!(m_bAllAgreeDismissRoom && m_nGameStation == GS_WAIT_NEXT_ROUND))
	{
		RecordMJUsersScore(tagResult);
	}

	Json::Value response;

	if (!GetIsHe()&& m_bHaveFetchBird)
	{
		//�·�����

		for (int i= 0 ; i< m_iNiaoNums ; i++)
		{
			response["niaoCards"].append(m_niaoCardValues[i]);
			response["bIsZhongNiao"].append(m_bIsZhongNiao[i]);
		}
	}
	response["Rounds"] = m_tagRoundFinish.iRoundTimes;
	response["AllRoundTimes"] = m_tagRoundFinish.iAllRoundTimes;
	response["Jin"] = m_byLaiZi;
	Json::Value video_resoult;

	//����ɼ���
	::memcpy(&m_tagResult,&tagResult,sizeof(TResult));
	//�·�ÿ������Ƿ���ƺͱ��ε÷����
	for (int i = 0; i < m_iPlayerCount; ++i)
	{
		response["isHu"].append(tagResult.bHu[i]) ;

		Json::Value huType;
		if (m_UserData[i].m_iHuTypeNums>0)
		{
			for (int j=0;j<m_UserData[i].m_iHuTypeNums;++j)
			{
				huType.append(m_UserData[i].m_nHuTypes[j]);
			}
		}else
		{
			huType.append(-1);
		}

		response["HuTypes"].append(huType);
		

		//��������
		response["HuCards"].append(m_UserData[i].m_byHuCard);

		response["HuPoint"].append( tagResult.iHuPoint[i]);
		response["GangPoint"].append(tagResult.iGangPoint[i]);
		response["NiaoPoint"].append(tagResult.iNiaoPoint[i]);

		//�������Ʈ �·�Ʈ�Ľ������
		if (m_bPiao)
		{
			response["PiaoPoint"].append(tagResult.iPiaoPoint[i]);
			response["PiaoType"].append(m_UserData[i].m_iPiaoNum);
		}
		if (m_bNTResult)
		{
			response["NTPoint"].append(tagResult.iNTPoint[i]);
		}
		response["TotalPoint"].append(tagResult.iTotalPoint[i]);
		response["TotalScore"].append(m_tagRoundFinish.iTotalScore[i]);



		//����
		response["HandNums"].append(m_UserData[i].m_iHandNums);
		Json::Value handcards;
		for (int j = 0;j< m_UserData[i].m_iHandNums ; j++)
		{
			handcards.append(m_UserData[i].m_byHandCards[j]);
		}
		response["HandCards"].append(handcards);

		//���Ƽ�¼
		response["CGPNums"].append(m_UserData[i].m_iCGPNums);
		Json::Value CPGRecords;
		if (m_UserData[i].m_iCGPNums > 0)
		{
			for(int j = 0 ; j< m_UserData[i].m_iCGPNums ; j++)
			{
				Json::Value cpgRecord;
				cpgRecord["usFlags"] = m_UserData[i].m_lstCGP[j].nType;//����������
				cpgRecord["from"] = m_UserData[i].m_lstCGP[j].from;
				for (int k = 0 ; k < 4 ;k++ )
				{
					cpgRecord["val"].append(m_UserData[i].m_lstCGP[j].val[k]);
				}
				CPGRecords.append(cpgRecord);
			}
		}
		else
		{
			Json::Value cpgRecord;
			cpgRecord["usFlags"] = 0;
			CPGRecords.append(cpgRecord);
		}
		response["CPGRecords"].append(CPGRecords);
		//�·�
		if(m_bPiao)
		{
			//�����ƮҪ�·���Ʈ������
			response["PiaoNum"].append(m_UserData[i].m_iPiaoNum);
		}
	}
	Json::Value paijuInfo;
	//�ƾ���Ϣ����
	for (int i = 0 ; i< m_iPlayerCount; i++)
	{
		paijuInfo["ZimoTimes"].append(m_tagRoundFinish.iZimoTimes[i]);
		paijuInfo["YouJinTimes"].append(m_tagRoundFinish.iYouJinTimes[i]);
		paijuInfo["LiuJuTimes"].append(m_tagRoundFinish.iLiuJuTimes[i]);
		paijuInfo["GangShangHuaTimes"].append(m_tagRoundFinish.iGangKaiTimes[i]);
		paijuInfo["JiePaoTimes"].append(m_tagRoundFinish.iJiePaoTimes[i]);
		paijuInfo["FangPaoTimes"].append(m_tagRoundFinish.iFangPaoTimes[i]);
		paijuInfo["MingGangTimes"].append(m_tagRoundFinish.iMingGangTimes[i]);
		paijuInfo["AnGangTimes"].append(m_tagRoundFinish.iAnGangTimes[i]);
		paijuInfo["BuGangTimes"].append(m_tagRoundFinish.iBuGangTimes[i]);
		paijuInfo["TotalScore"].append(m_tagRoundFinish.iTotalScore[i]);
	}
	paijuInfo["RoundTimes"] = m_tagRoundFinish.iRoundTimes;
	paijuInfo["AllRoundTimes"] = m_tagRoundFinish.iAllRoundTimes;

	//�Ƿ��ׯ
	bool isHuangZhuang = GetIsHe();
	response["IsHuangZhuang"] = isHuangZhuang;
	//����ׯ�·�������ҵ�����
	if (!isHuangZhuang )
	{
		response["FangPaoUser"] = tagResult.byFangPaoUser;
		response["HuUser"] = tagResult.byHuUser;
	}
	//�ƾ���Ϣ��ҪЯ���������� ������ʾ���һ�ֵĽ������
	paijuInfo["ResultInfo"] = response;
	//Ϊÿ����ҷ��ͽ������ݺ��ƾ���Ϣ
	for (int i = 0; i < m_iPlayerCount; ++i)
	{

		if (!m_pGameManager->IsPrivateRoom() || m_tagRoundFinish.iRoundTimes < m_tagRoundFinish.iAllRoundTimes )
		{
			//���ͽ�������
			SendGameData(i,response,ASS_RESULT);
		}
		else
		{
			//�����ƾ���Ϣ
			SendGameData(i,paijuInfo,ASS_PAIJU_INFO);
			m_nGameStation = GS_PAIJU_FINISH;
		}
	}

	this->InsertGameVideo( Video_Event::Video_Event_resoult , paijuInfo);
	if (m_tagRoundFinish.iRoundTimes >= m_tagRoundFinish.iAllRoundTimes)
	{

		//�����ƾֽ�����Ϣ
		Json::Value resp;
		for (int i = 0; i < m_iPlayerCount; ++i)
		{
			//���ͽ�������
			SendGameData(i,resp,ASS_GAME_OVER);
		}
		//���¼����ƾ���Ϣ
		m_tagRoundFinish.Clear();
	}
	//�������ҵ�ͬ��״̬
	for (int i = 0; i < m_iPlayerCount; ++i)
	{
		SetUnknowStateTo(i);
	}
	//������Ϸ
	GameFinish(0,GF_SALE);

	DebugPrintf(m_pGameManager->GetRoomID(),m_nDeskIndex,"OnEventResult====>end");
	return true;
}

bool CMJDesk::OnEventAutoAct(bool bCan)
{
	const UserData* pUser = NULL;
	for (int i = 0; i < PLAY_COUNT; ++i)
	{
		pUser = &m_UserData[i];
		if (!m_pGameManager->IsPrivateRoom())//��ҳ�Ҫ�Զ�����
		{
			CString strMJOutput;

			USHORT usAct = pUser->m_tagActInfo.usFlags;
			bool isNeedOutCard = false;
			int byOutTingCard = 0;
			bool bAuto = false;
			if (bCan)
			{
				bAuto = pUser->m_bAuto;
			}	
			else
			{
				bAuto = true;
			}

			if (usAct > 0 && bAuto)
			{

				Json::Value actReq;
				//�ȼ��������ȼ��Ķ���
				if (pUser->m_tagActInfo.usFlags & E_MAHJONG_ACTION::ACT_HU)
				{
					actReq["ActFlag"] =  E_MAHJONG_ACTION::ACT_HU;
				}
				else if (pUser->m_tagActInfo.usFlags & E_MAHJONG_ACTION::ACT_TING)
				{
					actReq["ActFlag"] =  E_MAHJONG_ACTION::ACT_TING;
					isNeedOutCard = true;
					byOutTingCard = pUser->m_tagActInfo.byTingSels[0];
				}
				else if (pUser->m_tagActInfo.usFlags & E_MAHJONG_ACTION::ACT_GANG)
				{
					actReq["ActFlag"] =  E_MAHJONG_ACTION::ACT_GANG;
					//�ܵ�ѡ��
					actReq["nSelIndex"] = 0;
				}
				else if (pUser->m_tagActInfo.usFlags & E_MAHJONG_ACTION::ACT_PENG)
				{
					actReq["ActFlag"] =  E_MAHJONG_ACTION::ACT_PENG;
				}
				else if (pUser->m_tagActInfo.usFlags & E_MAHJONG_ACTION::ACT_CHI)
				{
					actReq["ActFlag"] =  E_MAHJONG_ACTION::ACT_CHI;
					//�ȼ���ͷ Ȼ����� ����β
					if(pUser->m_tagActInfo.usChiFlags & MjChiFlag::usT)
					{
						actReq["EatFlag"] =  MjChiFlag::usT;
					}
					else if(pUser->m_tagActInfo.usChiFlags & MjChiFlag::usZ)
					{
						actReq["EatFlag"] =  MjChiFlag::usZ;
					}
					else
					{
						actReq["EatFlag"] =  MjChiFlag::usW;
					}
				}
				else
				{
					actReq["ActFlag"] =  E_MAHJONG_ACTION::ACT_GUO;
				}
				Json::FastWriter _writer;
				std::string strSend = _writer.write(actReq);
				OnUserActRequest(i,(void*)strSend.c_str(),strSend.length(),false);

				if (isNeedOutCard)
				{
					//��������ִ��������  ����Ҫ���ͳ�����Ϣ
					Json::Value outCardReq;
					outCardReq["cardvalue"] = byOutTingCard;
					outCardReq["outcardtype"] = TOutCard::REQUEST;
					outCardReq["isBaoCard"] = 1;
					Json::FastWriter _writer;
					std::string strSend = _writer.write(outCardReq);
					OnUserOutCardRequest(i,(void *)strSend.c_str(),strSend.length(),false);
				}
			}
		}
	}
	return true;
}


bool CMJDesk::OnEventAutoOutCard(bool bCan)
{
	const UserData* pUser = &m_UserData[m_byTokenUser];
	Json::Value outCardReq;
	outCardReq["cardvalue"] = CMJLogic::getAutoOutCard(pUser->m_byHandCards,pUser->m_iHandNums);
	outCardReq["outcardtype"] = TOutCard::REQUEST;
	outCardReq["isBaoCard"] = 0;

	bool bAuto = false;
	if (bCan)
	{
		bAuto = pUser->m_bAuto;
	}
	else
	{
		bAuto = true;
	}

	if (!pUser->m_bIsTing && bAuto)
	{
		Json::FastWriter _writer;
		std::string strSend = _writer.write(outCardReq);
		OnUserOutCardRequest(m_byTokenUser,(void *)strSend.c_str(),strSend.length(),false);
	}
	return true;
}

///���ƣ�CalcRoundFen
///�������������
///@param result��������������룬���������result���ⲿ��ʼ�����ID��
//�ڲ������������Ӯ��ҵ����ݡ�����������ݵȣ�������result�С�
///@return 
void CMJDesk::CalcRoundFen(TResult& result)
{
	UserData* pUser = NULL;
	bool bZiMo = false;
	bool bQiangGang = false;
	BYTE byZiMoUser = 255;
	BYTE byHuUser = 255;
	BYTE byFangPaoUser = 255;
	BYTE byDianGangKaiUserFrom=255;

	emSuanFenType eSuanFenType=sf_Unknown;
	int nBaseHuPoint[PLAY_COUNT]={0};
	int nHuUserNum=0;
	int nHuUserArray[PLAY_COUNT]={255,255,255,255};

	for (int i=0;i<m_iPlayerCount;++i)
	{
		pUser = &m_UserData[i];

		if (emHuSpecialWay::hsw_Unknown != pUser->m_nHuSpecWay[1]) 
		{
			if (emHuSpecialWay::hsw_QiangGang == pUser->m_nHuSpecWay[1])
			{
				pUser->m_nHuTypes[pUser->m_iHuTypeNums++]=ht_QiangGangHu;
				eSuanFenType=sf_QiangGangHu;
			}
			else if (emHuSpecialWay::hsw_AnGangKai==pUser->m_nHuSpecWay[1])
			{
				if (pUser->IsHuDianGang())
				{
					byDianGangKaiUserFrom = pUser->m_lstCGP[pUser->m_dianGangGangKaiIndex].from;
					m_UserData[byDianGangKaiUserFrom].m_nResult = Res_FangPao;
					eSuanFenType = sf_DianGangKai;
					pUser->m_bLastDianGangKai = true;
				}
				else
				{
					eSuanFenType=sf_AnGangKai;
				}
				pUser->m_nHuTypes[pUser->m_iHuTypeNums++]=ht_GangKai;

				m_tagRoundFinish.iGangKaiTimes[i]++;
			}
			else if (emHuSpecialWay::hsw_BuGangKai==pUser->m_nHuSpecWay[1])
			{
				if (pUser->IsHuDianGang())
				{
					byDianGangKaiUserFrom = pUser->m_lstCGP[pUser->m_dianGangGangKaiIndex].from;
					m_UserData[byDianGangKaiUserFrom].m_nResult = Res_FangPao;
					eSuanFenType = sf_DianGangKai;
					pUser->m_bLastDianGangKai = true;
				}
				else
				{
					eSuanFenType=sf_BuGangKai;
				}
				pUser->m_nHuTypes[pUser->m_iHuTypeNums++]=ht_GangKai;

				m_tagRoundFinish.iGangKaiTimes[i]++;
			}else if (emHuSpecialWay::hsw_DianGangKai==pUser->m_nHuSpecWay[1])
			{
				eSuanFenType=sf_DianGangKai;
				pUser->m_nHuTypes[pUser->m_iHuTypeNums++]=ht_GangKai;

				m_tagRoundFinish.iGangKaiTimes[i]++;
				if (pUser->m_iCGPNums>0)
				{
					if (pUser->m_lstCGP[pUser->m_iCGPNums-1].nType==emType::MingGang)
					{
						byDianGangKaiUserFrom=pUser->m_lstCGP[pUser->m_iCGPNums-1].from;
						m_UserData[byDianGangKaiUserFrom].m_nResult = Res_FangPao;
					}
				}

				// 20181205
				pUser->m_bLastDianGangKai = true;
			}

		}
	}

	for (int i=0;i<m_iPlayerCount;++i)
	{
		pUser = &m_UserData[i];

		if (MjResult::Res_ZiMo == pUser->m_nResult || MjResult::Res_JiePao == pUser->m_nResult)
		{
			for (int j=0;j<pUser->m_iHuTypeNums;++j)
			{
				if (pUser->m_nHuTypes[j]==ht_YouJin)
				{
					if (eSuanFenType!=sf_BuGangKai && eSuanFenType!=sf_AnGangKai && eSuanFenType!=sf_DianGangKai)
					{
						eSuanFenType=sf_YouJinZiMo;
						// 
						//m_tagRoundFinish.iYouJinTimes[i]++;
						break;
					}

				}
			}
		}
	}


	if (!GetIsHe())
	{
		if (eSuanFenType==sf_Unknown)
		{
			eSuanFenType=sf_PuTongZiMo;
		}
	}

	for (int i = 0; i < m_iPlayerCount; ++i)
	{
		pUser = &m_UserData[i];

		if (MjResult::Res_ZiMo == pUser->m_nResult || MjResult::Res_JiePao == pUser->m_nResult)
		{
			byHuUser = i;
			//�������ּ���/////////////////////////////////////////////////////////////////////////////////////////////////////
			switch(eSuanFenType)
			{
			case sf_PuTongZiMo:
				{
					nBaseHuPoint[i]=1;
				}
				break;

			case sf_YouJinZiMo:
				{
					nBaseHuPoint[i]=2;
				}
				break;

			case sf_QiangGangHu:
				{
					if (m_bIsYiMaQuanZhong)
					{
						nBaseHuPoint[i]=10;
					}else
					{
						nBaseHuPoint[i]=5;
					}

				}
				break;

			case sf_DianGangKai:
				{
					if (m_bIsYiMaQuanZhong)
					{
						nBaseHuPoint[i]=10;
					}else
					{
						nBaseHuPoint[i]=5;
					}

				}
				break;

			case sf_AnGangKai:
				{
					nBaseHuPoint[i]=1;
				}
				break;

			case sf_BuGangKai:
				{
					nBaseHuPoint[i]=1;
				}
				break;
			}

			if (emHuSpecialWay::hsw_Unknown != pUser->m_nHuSpecWay[1]) 
			{
				if (emHuSpecialWay::hsw_QiangGang == pUser->m_nHuSpecWay[1])
				{
					bQiangGang = true;
					//::sprintf_s(result.szHuRepo[i],"���� 3��  ",result.szHuRepo[i]);
					//result.iHuPoint[i] = 3;
				}

			}
		}

		if (MjResult::Res_ZiMo == pUser->m_nResult)
		{
			////bZiMo = true;
			////byZiMoUser = i;
			////������������
			//m_tagRoundFinish.iZimoTimes[i] ++;

			//nHuUserArray[nHuUserNum++]=i;

			// 200181130�������ν������ֿ���
			bool bYouJin = false;
			for (int j=0; j<pUser->m_iHuTypeNums;++j)
			{
				if (pUser->m_nHuTypes[j] == ht_YouJin)
				{
					bYouJin = true;
					break;
				}
			}
			if ( emHuSpecialWay::hsw_Unknown == pUser->m_nHuSpecWay[1])
			{
				// �����Ѽ�����
			}
			else if (bYouJin)
			{
				m_tagRoundFinish.iYouJinTimes[i] ++;
			}
			else
			{
				m_tagRoundFinish.iZimoTimes[i] ++;
			}
			nHuUserArray[nHuUserNum++]=i;
		}

		if (MjResult::Res_FangPao == pUser->m_nResult)
		{
			byFangPaoUser = i;
			m_tagRoundFinish.iFangPaoTimes[i] ++;
		}
		if (MjResult::Res_JiePao == pUser->m_nResult)
		{
			//���ڴ�������
			m_tagRoundFinish.iJiePaoTimes[i] ++;
			nHuUserArray[nHuUserNum++]=i;
		}
	}

	//ȡ�ñ仯�ķ�ֵ
	for(int i = 0; i < m_iPlayerCount; ++i)
	{
		if (result.iHuPoint[i] > 0)
		{
			result.iChPoint += result.iHuPoint[i];
		}
	}



	for (int i = 0; i < m_iPlayerCount; ++i)
	{
		pUser = &m_UserData[i];

		if (MjResult::Res_ZiMo == pUser->m_nResult || MjResult::Res_JiePao == pUser->m_nResult)
		{
			//�������ּ���/////////////////////////////////////////////////////////////////////////////////////////////////////
			switch(eSuanFenType)
			{
			case sf_PuTongZiMo:
			case sf_YouJinZiMo:
			case sf_AnGangKai:
			case sf_BuGangKai:
				{
					for (int j=0;j<m_iPlayerCount;++j)
					{
						if (j!=i)
						{
							result.iHuPoint[j]-=nBaseHuPoint[i];
						}
					}

					result.iHuPoint[i]+=nBaseHuPoint[i]*(m_iPlayerCount-1);
				}
				break;

			case sf_QiangGangHu:
				{
					result.iHuPoint[i]+=nBaseHuPoint[i];
					result.iHuPoint[byFangPaoUser]-=nBaseHuPoint[i];
				}
				break;

			case sf_DianGangKai:
				{
					if (byDianGangKaiUserFrom!=255)
					{
						result.iHuPoint[i]+=nBaseHuPoint[i];
						result.iHuPoint[byDianGangKaiUserFrom]-=nBaseHuPoint[i];
					}
				}
				break;
			}

		}
	}

	//��ׯ����
	if (GetIsHe())
	{
		for (int i = 0; i < m_iPlayerCount; ++i)
		{
			result.iHuPoint[i] = 0;
			result.iNiaoPoint[i] = 0;
			result.iGangPoint[i]=0;
			//�����Ʈ��Ҫ��Ʈ�ķ�
			if (m_bPiao)
			{
				result.iPiaoPoint[i] = 0;
			}
			//���ׯ�����Ҫ��ׯ�з�
			if (m_bNTResult)
			{
				result.iNTPoint[i] = 0;
			}
		}


	}
	else
	{
		if (eSuanFenType == sf_DianGangKai)
		{//����Ǹ���
			ModifyGangPointWhileDianGangKai(byHuUser);
		}
		//����ܷ�
		for (int i = 0; i < m_iPlayerCount; ++i)
		{
			result.iGangPoint[i] = m_UserData[i].iGangPoint;
		}

		int nZhongMaNum=0;
		for (int i=0;i<m_iNiaoNums;++i)
		{
			if (m_bIsZhongNiao[i])
			{
				nZhongMaNum++;
			}
		}

		//�������ü���ץ���
		if (m_iNiaoNums > 0)
		{
			for (int i=0;i<nHuUserNum;++i)
			{
				int nCurHuUser=nHuUserArray[i];
				switch(eSuanFenType)
				{
				case sf_PuTongZiMo:
				case sf_AnGangKai:
					{
						if (m_bIsYiMaQuanZhong)
						{
							for (int j=0;j<m_iNiaoNums;++j)
							{
								int nNiaoValue=m_niaoCardValues[j];
								int nBeiShu=1;
								if (nNiaoValue>30)
								{
									nBeiShu=5;
								}else
								{
									nBeiShu=nNiaoValue%10;
								}

								if (m_bIsZhongNiao[j])
								{
									for (int k=0;k<m_iPlayerCount;k++)
									{
										if (k!=nCurHuUser)
										{
											result.iNiaoPoint[k]-=1*nBeiShu;
										}
									}
									result.iNiaoPoint[nCurHuUser]+=(1*nBeiShu)*(m_iPlayerCount-1);
								}
							}
						}else
						{
							for (int j=0;j<m_iPlayerCount;++j)
							{
								if (j!=nCurHuUser)
								{
									result.iNiaoPoint[j]-=1*nZhongMaNum;
								}
							}
							result.iNiaoPoint[nCurHuUser]+=(1*nZhongMaNum)*(m_iPlayerCount-1);

						}
						break;
					}
				case sf_YouJinZiMo:
					{
						if (m_bIsYiMaQuanZhong)
						{
							for (int j=0;j<m_iNiaoNums;++j)
							{
								int nNiaoValue=m_niaoCardValues[j];
								int nBeiShu=1;
								if (nNiaoValue>30)
								{
									nBeiShu=5;
								}else
								{
									nBeiShu=nNiaoValue%10;
								}

								if (m_bIsZhongNiao[j])
								{
									for (int k=0;k<m_iPlayerCount;k++)
									{
										if (k!=nCurHuUser)
										{
											result.iNiaoPoint[k]-=2*nBeiShu;
										}
									}
									result.iNiaoPoint[nCurHuUser]+=(2*nBeiShu)*(m_iPlayerCount-1);
								}
							}
						}else
						{
							for (int j=0;j<m_iPlayerCount;++j)
							{
								if (j!=nCurHuUser)
								{
									result.iNiaoPoint[j]-=2*nZhongMaNum;
								}
							}
							result.iNiaoPoint[nCurHuUser]+=(2*nZhongMaNum)*(m_iPlayerCount-1);

						}
					}
					break;
				}
			}
		}
	}

	//�����ܷ�
	for (int i = 0; i < m_iPlayerCount; ++i)
	{

		result.iTotalPoint[i] = result.iHuPoint[i] + result.iGangPoint[i] + result.iNiaoPoint[i];

		DebugPrintf(m_pGameManager->GetRoomID(),m_nDeskIndex,"���%d,����=%d,�ܷ�=%d,���=%d,�ܷ�=%d",i,result.iHuPoint[i],result.iGangPoint[i],result.iNiaoPoint[i],result.iTotalPoint[i]);

		//�����ƾ��ܷ�
		m_tagRoundFinish.iTotalScore[i] += result.iTotalPoint[i];
	}

	return;
}

void CMJDesk::RecordMJUsersScore(TResult& result)
{
	Json::Value root;
	root["roundID"] = m_tagRoundFinish.iRoundTimes;			//��ս�ֱ��
	//ս����¼�� 

	for (int i = 0 ; i< m_iPlayerCount ; i++)
	{
		Json::Value detail;
		detail["bIsBanker"] = (i == m_byNTUser);													//�Ƿ���ׯ��
		Json::Value user;			
		user["userID"] = m_ResultUserData[i]._dwUserID;									//�û�ID
		user["beforePoint"] = m_ResultUserData[i]._iUserMoney;							//��սǰ�ķ���
		user["changedPoint"] = result.iTotalPoint[i];														//���ζ�ս�ķ���
		user["detail"] = detail;																			//�������Ϸ����Ϣ����������Ϸ�������������
		root["records"][i] = user;
	}


	Json::FastWriter write;
	std::string str = write.write(root);
	int recoredResult = m_pGameManager->RecordUsersScore(m_nDeskIndex,m_gameBeginTime,str.c_str());
	if(recoredResult !=  0)
	{
		//�����¼ʧ�ܣ����ҳ�ԭ�򣬾�����֤��¼�ɹ�;
		OutputTraceStr("wgd: record user score failed %d",recoredResult);
	}

}

///�������������Ϊδ֪�Ƿ�ͬ���ע״̬
///@param bDeskStation���λ��
void CMJDesk::SetUnknowStateTo(BYTE bDeskStation)
{
	m_UserData[bDeskStation].m_nState = UserData::UNKNOWN;
	return;
}
///�������������Ϊͬ���ע״̬
///@param bDeskStation���λ��
void CMJDesk::SetAgreeStateTo(BYTE bDeskStation)
{
	m_UserData[bDeskStation].m_nState = UserData::AGREEGAME;
	return;
}

//�ͻ��������·�����
int CMJDesk::OnTestChangeFetchHandcards(BYTE bDeskStation,void* pData,UINT uSize,bool bWatchUser)
{
	m_UserData[bDeskStation].m_bFetchHandCardCheat = true;
	Json::Reader reader(Json::Features::strictMode());

	//�յ���ҷ���������
	Json::Value  _root;
	std::string sBuffer = (char *)pData;
	if (!reader.parse(sBuffer, _root))
	{
		return -1;
	}

	if (m_tagDeskCfg.byCheaterOpen!=1)
	{
		return -1;
	}

	CString str;


	//������ҵ���������
	UserData* pUser = &m_UserData[bDeskStation];
	for (int i = 0 ; i< _root["cards"].size() ; i++)
	{
		if (i>13)
		{
			return 0;
		}
		if (!_root["cards"][i].isInt())
		{
			return 0;
		}
		pUser->m_byHandCardsCheat[i] = _root["cards"][i].asInt();

		CString strTemp;
		strTemp.Format("%d,",pUser->m_byHandCardsCheat[i]);
		str+=strTemp;
	}
	DebugPrintf(m_pGameManager->GetRoomID(),m_nDeskIndex,"CMjGameDesk::OnTestChangeFetchHandcards========>chair=%d,%s",bDeskStation,str);
	return 0;
}

///���ƣ�OnTestChangeFetch
///������ָ��ץ��ֵ
///@param bDeskStation λ��, pNetHead ��Ϣͷ,pData ��Ϣ����,uSize ��Ϣ������,uSocketID id,bWatchUser �Ƿ��Թ�
///@return
int CMJDesk::OnTestChangeFetch(BYTE bDeskStation,void* pData,UINT uSize,bool bWatchUser)
{
	Json::Reader reader(Json::Features::strictMode());

	//�յ���ҷ���������
	Json::Value  _root;
	std::string sBuffer = (char *)pData;
	if (!reader.parse(sBuffer, _root))
	{
		return -1;
	}

	if (!_root["cardvalue"].isInt())
	{
		return -1;
	}

	if (m_tagDeskCfg.byCheaterOpen!=1)
	{
		return -1;
	}

	CString str;

	UserData* pUser = &m_UserData[bDeskStation];
	pUser->m_byChFetch = _root["cardvalue"].asInt();

	CString strTemp;
	strTemp.Format("%d,",pUser->m_byChFetch);
	str+=strTemp;
	DebugPrintf(m_pGameManager->GetRoomID(),m_nDeskIndex,"CMjGameDesk::OnTestChangeFetch=========>chair=%d,%s",bDeskStation,str);
	return 0;
}


///���ƣ�OnUserAutoRequest
///��������������й���Ϣ
///@param bDeskStation λ��, pNetHead ��Ϣͷ,pData ��Ϣ����,uSize ��Ϣ������,uSocketID id,bWatchUser �Ƿ��Թ�
///@return
int CMJDesk::OnUserAutoRequest(BYTE bDeskStation,void* pData,UINT uSize,bool bWatchUser)
{
	UserData* pUser = &m_UserData[bDeskStation];
	Json::Reader reader(Json::Features::strictMode());

	//�յ���ҷ���������
	Json::Value  _root;
	std::string sBuffer = (char *)pData;
	if (!reader.parse(sBuffer, _root))
	{
		return -1;
	}
	if (!(_root["IsAuto"].isBool()))
	{
		return -1;
	}

	//����//////////////////////////////////////////////////////////////////

	if (bWatchUser)
	{
		return 0;
	}
	//����//////////////////////////////////////////////////////////////////

	pUser->m_bAuto = _root["IsAuto"].asBool();

	//�㲥�й���Ϣ/////////////////////////////////////////////////////////
	Json::Value AutoInfo;
	AutoInfo["chair"] = bDeskStation;
	AutoInfo["IsAuto"] = pUser->m_bAuto;
	if (pUser->m_bAuto)
	{
		//�����ҷ����й���Ϣ
		if (pUser->m_tagActInfo.usFlags > 0)
		{
			//m_pGameManager->SetGameTimer(m_nDeskIndex,TIME_AUTO_ACT,1000);
			SetTimer(TIME_AUTO_ACT,1000);
		}
		else
		{
			//m_pGameManager->SetGameTimer(m_nDeskIndex,TIME_AUTO_OUTCARD,1000);
			SetTimer(TIME_AUTO_OUTCARD,1000);
		}
	}

	for (int i = 0; i < m_iPlayerCount; ++i)
	{
		//����ĳ������й�״̬�����Ϣ
		SendGameData(i,AutoInfo,ASS_AUTO_INFO);
	}
	return 0;
}

///���ƣ�OnTestChangeHands
///�������۸�����
///@param bDeskStation λ��, pNetHead ��Ϣͷ,pData ��Ϣ����,uSize ��Ϣ������,uSocketID id,bWatchUser �Ƿ��Թ�
///@return
int CMJDesk::OnTestChangeHands(BYTE bDeskStation,void* pData,UINT uSize,bool bWatchUser)
{
	Json::Reader reader(Json::Features::strictMode());

	//�յ���ҷ���������
	Json::Value  _root;
	std::string sBuffer = (char *)pData;
	if (!reader.parse(sBuffer, _root))
	{
		return -1;
	}

	if (m_tagDeskCfg.byCheaterOpen!=1)
	{
		return -1;
	}


	CString str;

	//������ҵ���������
	UserData* pUser = &m_UserData[bDeskStation];
	for (int i = 0 ; i< _root["cards"].size() ; i++)
	{
		if (i>13)
		{
			return 0;
		}
		if (!_root["cards"][i].isInt())
		{
			return 0;
		}
		pUser->m_byHandCards[i] = _root["cards"][i].asInt();

		CString strTemp;
		strTemp.Format("%d,",pUser->m_byHandCards[i]);
		str+=strTemp;
	}
	DebugPrintf(m_pGameManager->GetRoomID(),m_nDeskIndex,"CMjGameDesk::OnTestChangeHands=======>chair=%d,%s",bDeskStation,str);
	return 0;
}


//�����Ϸ�Ļ�����״̬
bool CMJDesk::OnGetGSWaitSet(BYTE bDeskStation,UINT uSocketID,bool bWatchUser)
{
	//��������ݻ�ȡ����д�´�
	Json::Value response;
	response["GSID"] = GS_WAIT_SETGAME;
	response["CheaterOpen"] = m_tagDeskCfg.byCheaterOpen;
	for (int i = 0; i<m_iPlayerCount ;i++)
	{
		response["TotalScore"].append(m_tagRoundFinish.iTotalScore[i]);
	}
	for (int i = 0; i < m_iPlayerCount; i++)
	{
		bool isAgree = (m_UserData[i].m_nState == UserData::AGREEGAME);
		response["IsAgree"].append(isAgree);
	}
	response["chair"] = bDeskStation;
	response["NTUser"] = m_byNTUser;
	response["RoundTimes"] = m_tagRoundFinish.iRoundTimes;
	response["AllRoundTimes"] = m_tagRoundFinish.iAllRoundTimes;
	return SendGameData(bDeskStation,response,ASS_GM_GAME_STATION);
}


bool CMJDesk::OnGetGSNextRound(BYTE bDeskStation,UINT uSocketID,bool bWatchUser)
{
	Json::Value response;
	//��ǰ��Ϸ״̬
	response["GSID"] = GS_WAIT_NEXT_ROUND;
	response["CheaterOpen"] = m_tagDeskCfg.byCheaterOpen;
	for (int i = 0; i<m_iPlayerCount ;i++)
	{
		response["TotalScore"].append(m_tagRoundFinish.iTotalScore[i]);
	}
	for (int i = 0; i < m_iPlayerCount; i++)
	{
		bool isAgree = (m_UserData[i].m_nState == UserData::AGREEGAME);
		response["IsAgree"].append(isAgree);
	}
	response["chair"] = bDeskStation;
	response["NTUser"] = m_byNTUser;
	response["RoundTimes"] = m_tagRoundFinish.iRoundTimes;
	response["AllRoundTimes"] = m_tagRoundFinish.iAllRoundTimes;
	return SendGameData(bDeskStation,response,ASS_GM_GAME_STATION);
}

//����״̬����
bool CMJDesk::OnGetGSPlaying(BYTE bDeskStation,UINT uSocketID,bool bWatchUser)
{
	//��������ݻ�ȡ����д�´�
	const UserData* pUser = NULL;

	//����Լ��Ķ���
	pUser = &m_UserData[bDeskStation];
	Json::Value response;
	//������ҵ�ǰ�����Ӻ�
	response["chair"] = bDeskStation;

	response["laizis"]=m_byLaiZi;

	//��ǰ��Ϸ״̬
	response["GSID"] = m_nGameStation;
	response["CheaterOpen"] = m_tagDeskCfg.byCheaterOpen;
	for (int i = 0; i<m_iPlayerCount ;i++)
	{
		response["TotalScore"].append(m_tagRoundFinish.iTotalScore[i]);
	}
	//ʣ���Ƶ�����
	response["WallLeft"] = GetWallLeft();

	//�Ƿ��Թ����
	response["isWatchUser"] = false;
	//��ǰׯ��
	response["NTUser"] = m_byNTUser;
	//��ǰʣ��������
	response["WallLeftNums"] =GetWallLeft();	
	//ÿ�����ʣ�����������
	for (int i= 0 ; i< m_iPlayerCount ; i++)
	{
		response["HandNums"].append(m_UserData[i].m_iHandNums) ;
		//ÿ����ҵ�����
		if (i == bDeskStation)
		{
			for (int j= 0; j<m_UserData[i].m_iHandNums ; j++)
			{
				response["HandCards"][i].append(pUser->m_byHandCards[j]);
			}
		}
		else
		{
			for (int j= 0; j<m_UserData[i].m_iHandNums ; j++)
			{
				response["HandCards"][i].append(0);
			}
		}
	}

	//��ǰ�������
	response["TokenUser"] = m_byTokenUser;

	//ÿ����ҳ�����

	for (int i = 0 ; i< m_iPlayerCount ; i++)
	{
		response["OutedNums"].append(m_UserData[i].m_iOutedNums);
		Json::Value outcards;
		if (m_UserData[i].m_iOutedNums > 0)
		{
			for (int j = 0; j< m_UserData[i].m_iOutedNums;j++)
			{
				outcards.append(m_UserData[i].m_byOutedCards[j]);
			}
		}
		else
		{
			outcards["id"] = "nooutcard";
		}
		response["OutedCards"].append(outcards);
	}

	//ÿ����ҳ��������

	for (int i = 0 ; i< m_iPlayerCount ; i++)
	{
		response["CGPNums"].append(m_UserData[i].m_iCGPNums);
		Json::Value CPGRecords;
		if (m_UserData[i].m_iCGPNums > 0)
		{
			for(int j = 0 ; j< m_UserData[i].m_iCGPNums ; j++)
			{
				Json::Value cpgRecord;
				cpgRecord["usFlags"] = m_UserData[i].m_lstCGP[j].nType;//����������
				for (int k = 0 ; k < 4 ;k++ )
				{
					cpgRecord["val"].append(m_UserData[i].m_lstCGP[j].val[k]);
				}
				cpgRecord["from"] = m_UserData[i].m_lstCGP[j].from;
				CPGRecords.append(cpgRecord);
			}
		}
		else
		{
			Json::Value cpgRecord;
			cpgRecord["usFlags"] = 0;
			CPGRecords.append(cpgRecord);
		}

		response["CPGRecords"].append(CPGRecords);
	}

	//�Լ���ǰ�Ƿ��ж���
	Json::Value actResp ;
	actResp["usFlags"] = pUser->m_tagActInfo.usFlags;

	if (pUser->m_tagActInfo.usFlags & E_MAHJONG_ACTION::ACT_CHI)
	{
		//���Ϳɳ�����Ϣ
		actResp["isCanC"] = true;//�Ƿ���Գ���
		actResp["iChiNums"] = pUser->m_tagActInfo.iChiNums;//����ѡ������
		actResp["usChiFlags"] = pUser->m_tagActInfo.usChiFlags;//�Եı�־ ��(ͷ��β)
	}
	else
	{
		actResp["isCanC"] = false;
	}

	if (pUser->m_tagActInfo.usFlags & E_MAHJONG_ACTION::ACT_PENG)
	{
		//���Ϳ�������Ϣ
		actResp["isCanP"] = true;

	}
	else
	{
		actResp["isCanP"] = false;
	}

	if (pUser->m_tagActInfo.usFlags & E_MAHJONG_ACTION::ACT_GANG)
	{
		//���Ϳɸ�����Ϣ
		actResp["isCanG"] = true;
		actResp["iGangNums"] = pUser->m_tagActInfo.iGangNums;//���Ը�ѡ������
		for (int i= 0 ; i< MJ_MAX_CGP ; i++)
		{
			actResp["byGangSels"].append(pUser->m_tagActInfo.byGangSels[i]);//���Ըܵ�ѡ��
		}for (int i= 0 ; i< MJ_MAX_CGP ; i++)
		{
			actResp["nBuGangInPeng"].append(pUser->m_tagActInfo.nBuGangInPeng[i]);//�����ڽ�������һ��������
		}

	}
	else
	{
		actResp["isCanG"] = false;
	}

	if (pUser->m_tagActInfo.usFlags & E_MAHJONG_ACTION::ACT_HU)
	{
		//���Ϳɺ�����Ϣ
		actResp["isCanH"] = true;
		actResp["iHuTypeNums"] = pUser->m_tagActInfo.iHuTypeNums;
	}
	else
	{
		actResp["isCanH"] = false;
	}
	response["actResp"] = actResp;

	response["RoundTimes"] = m_tagRoundFinish.iRoundTimes;
	response["AllRoundTimes"] = m_tagRoundFinish.iAllRoundTimes;
	///Ư������
	Json::Value PiaoResp;
	for (int i = 0; i < m_iPlayerCount;i++)
	{
		Json::Value Resp;
		Resp["chair"] = i;
		Resp["piaoType"] = m_UserData[i].m_iPiaoNum;
		PiaoResp.append(Resp);
	}
	response["PiaoResp"] = PiaoResp;

	//�Ƿ��й�
	response["IsAuto"] = pUser->m_bAuto;

	return SendGameData(bDeskStation,response,ASS_GM_GAME_STATION);

}

//��Ư��״̬
bool CMJDesk::OnGetGSWaitPiao(BYTE bDeskStation,UINT uSocketID,bool bWatchUser)
{
	Json::Value response;
	//��ǰ״̬ID
	response["GSID"] = GS_WAIT_PIAO;
	response["CheaterOpen"] = m_tagDeskCfg.byCheaterOpen;
	for (int i = 0; i<m_iPlayerCount ;i++)
	{
		response["TotalScore"].append(m_tagRoundFinish.iTotalScore[i]);
	}
	Json::Value PiaoResp;
	for (int i  = 0; i < m_iPlayerCount; ++i)
	{
		Json::Value item;
		item["chair"] = i;
		item["bPiao"] = m_UserData[i].m_bPiao;
		item["piaoType"] = m_UserData[i].m_iPiaoNum;
		PiaoResp.append(item);
	}
	response["chair"] = bDeskStation;
	response["PiaoResp"] = PiaoResp;
	response["NTUser"] = m_byNTUser;
	response["RoundTimes"] = m_tagRoundFinish.iRoundTimes;
	response["AllRoundTimes"] = m_tagRoundFinish.iAllRoundTimes;

	return SendGameData(bDeskStation,response,ASS_GM_GAME_STATION);
}

void CMJDesk::ModifyGangPointWhileDianGangKai(int huUser)
{
	UserData *pUser = &m_UserData[huUser]; 
	if (pUser->m_dianGangGangKaiIndex + 1 == pUser->m_iCGPNums)
	{
		// 20181211 �޸�һ��,���ﲻ�����зֹ�0����������ܱ����ϻ��ĸܹ�0
		//for (int i = 0; i<m_iPlayerCount; i++)
		//{
		//	m_UserData[i].iGangPoint = 0;
		//}
		int fangGangUser = pUser->m_lstCGP[pUser->m_dianGangGangKaiIndex].from;
		m_UserData[fangGangUser].iGangPoint += 1;
		m_UserData[huUser].iGangPoint -= 1;
	}
	else if(pUser->m_dianGangGangKaiIndex < pUser->m_iCGPNums)
	{
		//��ܺ����ָܵĸ��ϻ������ɵ����ҳа��ܷ�
		int fangGangUser = pUser->m_lstCGP[pUser->m_dianGangGangKaiIndex].from;
		// 20181203��ǰ��ĵ�ܲ���
		m_UserData[fangGangUser].iGangPoint += 1;
		m_UserData[huUser].iGangPoint -= 1;
		for (int i = pUser->m_dianGangGangKaiIndex+1; i < pUser->m_iCGPNums; i++)
		{
			if (pUser->m_lstCGP[i].nType == emType::BuGang || pUser->m_lstCGP[i].nType == emType::AnGang)
			{
				for (int j = 0; j<m_PlayCount; j++)
				{
					if (j != huUser && j != fangGangUser)
					{
						m_UserData[j].iGangPoint += 1;
						m_UserData[fangGangUser].iGangPoint -= 1;
					}
				}
			}
		}
	}
}

bool CMJDesk::testCardModel(int arrWall[], int nWallCardNum, int arrTest[], int nTestNum)
{
	for(int i = 0; i < nTestNum; ++i)
	{
		//// ��ǰ��ץ��
		//for (int j = i + 1; j < nWallCardNum; ++j)
		//{
		//	if (arrTest[i] == arrWall[j])
		//	{
		//		int nTemp = arrWall[j];
		//		arrWall[j] = arrWall[i];
		//		arrWall[i] = nTemp;
		//		//std::swap(arrWall[i], arrWall[j]);
		//		break;
		//	}
		//}

		// �Ӻ�����ץ��
		for (int j = nWallCardNum - i - 1;  j > -1; --j)
		{
			if (arrTest[i] == arrWall[j])
			{
				int nTemp = arrWall[j];
				arrWall[j] = arrWall[nWallCardNum - i - 1];
				arrWall[nWallCardNum - i - 1] = nTemp;
				break;
			}
		}
	}
	return true;
}
