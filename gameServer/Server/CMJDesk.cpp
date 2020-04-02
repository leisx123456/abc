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
	//初始化游戏状态
	m_nGameStation = GS_WAIT_SETGAME;
	m_bAllAgreeDismissRoom = false; 
	m_iPlayerCount = 4;

	m_bReserveSeat=true;

}
//析构函数
CMJDesk::~CMJDesk(void)
{

}

void CMJDesk::Init(const UserInfoForGame_t* pUserInfo)
{
	m_byLastFetchCardUser=255;

	//全初始服务端游戏数据
	HalfInit();

	for(int i = 0; i < PLAY_COUNT; i++)
	{
		//清理用户数据
		m_UserData[i].Init();
	}

	//最近的成绩单
	m_tagResult.Clear();

	//庄家
	m_byNTUser = 255;

}

///描述：半初始化，用于回合之间数据完全的部分清理，因为有的数据需要保留。
void CMJDesk::HalfInit()
{
	m_nLastPengCard=255;

	m_nLastPengUser=255;

	m_bHaveFetchBird=false;

	for(int i = 0; i < PLAY_COUNT; i++)
	{
		//清理用户数据
		m_UserData[i].HalfInit();
	}
	//庄家打骰子定取牌方法时两骰子的点数值
	m_byFetchPoint[0] = 0;
	m_byFetchPoint[1] = 0;

	m_byLaiZi=255;

	//当前活动的用户
	m_byTokenUser = 255;

	//指定下个得令牌的玩家
	m_byTokenNextUser = 255;

	//是否荒庄
	m_bHuangZhuang = false;		

	m_iWallLeft = 0;
	m_iMjCardNum = 0;
	m_bHaveGang = false;
	m_bPiao = false;
	::memset(m_niaoCardValues,0,sizeof(m_niaoCardValues));
	::memset(m_iAllCard,0,sizeof(m_iAllCard));
	memset(m_bIsZhongNiao,0,sizeof(m_bIsZhongNiao));
}


//初始化桌子
int CMJDesk::InitialDesk(int nDeskNo, int nMaxPeople, IGameMainManager* pIMainManager)
{
	int bRet =  __super::InitialDesk(nDeskNo,nMaxPeople,pIMainManager);
	//加载配置文件
	LoadDeskCfg();
	return bRet;

}

//加载限注
BOOL CMJDesk::LoadDeskCfg()
{
	TCHAR szIniName[MAX_PATH];
	sprintf(szIniName,"%s",CINIFile::GetAppPath());
	sprintf_s(szIniName,"%s%d_s.ini",szIniName,NAME_ID);
	CINIFile f(szIniName);
	Tstring strSeg = "game";
	Tstring strKey;
	TDeskCfg* pDeskCfg = &m_tagDeskCfg;

	//出牌思考时间(秒)
	strKey = _TEXT("OutCardTime");
	pDeskCfg->byOutCard = f.GetKeyVal(strSeg,strKey,20);

	//动作思考时间，如碰杠胡(秒)
	strKey = _TEXT("ActTime");
	pDeskCfg->byAct = f.GetKeyVal(strSeg,strKey,15);

	//是否开启配牌器 

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


//处理游戏内部消息
int CMJDesk::HandleNotifyMessage(int nDeskStation, UINT code, BYTE* pBuffer, int nLen)
{
	DebugPrintf(m_pGameManager->GetRoomID(),m_nDeskIndex,"HandleNotifyMessage====>chair=%d,code=%d",nDeskStation,code);
	bool bWatchUser = false;
	switch (code)
	{

	case ASS_ASSIGN_MAKENT_FINISH:
		{
			//处理玩家完成定起牌动画报告
			//return OnUserMakeNTOver(nDeskStation,pBuffer,nLen,bWatchUser);
			return 0;
		}
		break;
	case ASS_PIAO_REQ:
		{
			//客户端发来加飘消息
			return OnUserPiao(nDeskStation,pBuffer,nLen,bWatchUser);
		}
		break;
	case ASS_OUT_CARD_REQ:
		{
			//处理玩家出牌请求
			return OnUserOutCardRequest(nDeskStation,pBuffer,nLen,bWatchUser);
		}
		break;
	case ASS_ACT_REQ:
		{
			//处理玩家的动作请求
			return OnUserActRequest(nDeskStation,pBuffer,nLen,bWatchUser);
		}
		break;

	case ASS_AUTO_REQ:
		{
			//处理玩家托管消息
			return OnUserAutoRequest(nDeskStation,pBuffer,nLen,bWatchUser);
		}
		break;
		//游戏调试的消息处理
#ifdef PEI_PAI_ON
	case ASS_TEST_FETCH_INFO:
		{
			//客户端指定下发手牌
			return OnTestChangeFetchHandcards(nDeskStation,pBuffer,nLen,bWatchUser);
		}
		break;
	case ASS_TEST_CH_HAND:
		{
			//篡改手牌
			return OnTestChangeHands(nDeskStation,pBuffer,nLen,bWatchUser);
		}
		break;

	case ASS_TEST_CH_FETCH:
		{
			//篡改手牌
			return OnTestChangeFetch(nDeskStation,pBuffer,nLen,bWatchUser);
		}
		break;	
#endif

	}
	return __super::HandleNotifyMessage(nDeskStation,code,pBuffer,nLen);
}

//获取游戏状态，并发送到客户端
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
			//游戏没有开始状态
			return OnGetGSWaitSet(nDeskStation,uSocketID,bWatchUser);
		}
		break;

	case GS_PLAYING:
		{
			//各玩家打牌状态
			return OnGetGSPlaying(nDeskStation,uSocketID,bWatchUser);
		}
		break;
	case GS_WAIT_PIAO:
		{
			//漂的问题
			return  OnGetGSWaitPiao(nDeskStation,uSocketID,bWatchUser);
		}
	case GS_WAIT_NEXT_ROUND:
		{
			//等待下一轮游戏的开始
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

//定时器时间到
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
				//发手牌 --> 20181203起牌
				SetTimer(TIME_FETCH_METHOD,500);
			}
		}
		break;
		///定庄	
	case TIME_MAKE_NT :
		{
			KillTimer(TIME_MAKE_NT);
			OnEventMakeNT();
		}
		break;
		// 起牌方法
	case TIME_FETCH_METHOD:
		{
			KillTimer(TIME_FETCH_METHOD);
			OnEventFetchMethod();
		}
		break;
		///下发手牌
	case TIME_FETCH_HANDCARDS:
		{
			KillTimer(TIME_FETCH_HANDCARDS);
			OnEventFetchHandCards();
		}
		break;
		///漂
	case  TIME_PIAO:
		{
			KillTimer(TIME_PIAO);
			OnEventPiao();
		}
		break;
		///下发令牌
	case TIME_GIVE_NEXT_TOKEN:
		{
			KillTimer(TIME_GIVE_NEXT_TOKEN);
			OnEventGiveNextToken();
		}
		break;
		///抓鸟
	case TIME_FETCH_BIRD:
		{
			KillTimer(TIME_FETCH_BIRD);
			OnEventFetchNiao();
		}
		break;
		//一回合结束的事件处理
	case TIME_ROUND_FINISH:
		{
			KillTimer(TIME_ROUND_FINISH);
			OnEventRoundFinish();

		}
		break;
		///结算
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

//玩家入坐到某位置
void CMJDesk::OnUserSitDesk(int nDeskStation)
{
	memcpy(&m_ResultUserData[nDeskStation],&m_UserInfo[nDeskStation],sizeof(m_UserInfo[nDeskStation]));	
}


//玩家断线
void CMJDesk::OnUserNetCut(int nDeskStation)
{
	UserData* pNCUser = &m_UserData[nDeskStation];
	//断线状态
	pNCUser->m_bNetCut = true;
	//托管状态
	pNCUser->m_bAuto = true;

	//通知所有玩家pNCUser已取消托管
	//玩家断线 当前玩家托管
	Json::Value response;
	response["chair"] = nDeskStation;
	response["isAuto"] = true;

	for (int i = 0; i < m_iPlayerCount; i ++)
	{
		//为所有人发送当前玩家托管
		SendGameData(i,response,ASS_AUTO_INFO);
	}
}

///名称：OnUserAgreeGame
///描述：处理玩家同意游戏
///@param bDeskStation 位置, pNetHead 消息头,pData 消息数据,uSize 消息包长度,uSocketID id,bWatchUser 是否旁观
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
	//断线重连回来的玩家取消托管
	if (m_UserData[bDeskStation].m_bAuto)
	{

		m_UserData[bDeskStation].m_bAuto = false;
	}

}


///名称：GetAgreeNums
///描述：得到有多少个玩家同意游戏
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

//房间被申请解散
void CMJDesk::onDismissDesk()
{
	//总局数更新为当前的局数
	m_tagRoundFinish.iAllRoundTimes = m_tagRoundFinish.iRoundTimes;

	//所有人都统一解散房间
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
///获取游戏的ID录像会用到
unsigned int CMJDesk::GetGameNameID()
{
	return NAME_ID;
}


//游戏开始
bool CMJDesk::GameBegin(BYTE bBeginFlag)
{
	DebugPrintf(m_pGameManager->GetRoomID(),m_nDeskIndex,"GameFinish====>begin");
	//先调用父类  防止开始之前有玩家离开桌子
	if (!__super::GameBegin(bBeginFlag))
	{
		DebugPrintf(m_pGameManager->GetRoomID(),m_nDeskIndex,"GameFinish====>1");
		return false;
	}
	m_nGameStation = GS_PLAYING;
	m_gameBeginTime = time(NULL);
	//回合计数递增
	m_tagRoundFinish.iRoundTimes ++;

	if (!m_pGameManager->IsPrivateRoom())
	{
		//金币场每次都重新选庄
		m_tagRoundFinish.iRoundTimes = 1;
	}

	//if(m_tagRoundFinish.iRoundTimes == 1)
	//{
	//	//启动游戏的流程服务
	//	CXGameDesk::StartUp();
	//}
	//保存当前局数
	Json::Value  RoundStatus;
	RoundStatus["currRounds"] = m_tagRoundFinish.iRoundTimes;
	Json::FastWriter write;
	std::string str = write.write(RoundStatus);
	m_pGameManager->SaveStatus(m_nDeskIndex,str.c_str());

	if (m_tagRoundFinish.iRoundTimes == 1)
	{
		//第一回合,全初始服务端游戏数据
		Init(m_UserInfo);

		if (!m_pGameManager->IsPrivateRoom())
		{
			//金币场给一个默认的房间配置


		}

	}
	else if(m_tagRoundFinish.iRoundTimes > 1)
	{
		//第二回合后,半初始服务端游戏数据
		HalfInit();
	}

	Json::Value response;
	//开始发送局数
	response["RoundTimes"] = m_tagRoundFinish.iRoundTimes;
	response["AllRoundTimes"] = m_tagRoundFinish.iAllRoundTimes;
	//下发游戏开始信号，通知客户端开启游戏状态机
	for (int i = 0; i < m_iPlayerCount; i++)
	{
		SendGameData(i,response,ASS_GAME_BEGIN);
	}

	this->InitPlatVideo(m_tagRoundFinish.iRoundTimes ,m_iPlayerCount,szConfig );
	this->InsertGameVideo( Video_Event::Video_Event_begin , response );
	//启动定庄流程
	SetTimer(TIME_MAKE_NT,1500);


	///这里要注意要牌数量要准确，比如带红中112 张  不带108张要确定
	if (m_bZhongCaishen)
	{
		m_iWallLeft = MJ_MAX_CARDS_ZHONG;
	}
	else
	{
		m_iWallLeft = MJ_MAX_CARDS;
	}

	///游戏开始后洗牌
	///这里的数量可以是最大的牌也可以刚刚好
	m_iMjCardNum = MJ_MAX_CARDS_ZHONG;//（方便从数组里面去牌值）

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

//游戏结束
bool CMJDesk::GameFinish(BYTE bDeskStation, BYTE bCloseFlag)
{
	DebugPrintf(m_pGameManager->GetRoomID(),m_nDeskIndex,"GameFinish====>begin");
	this->SaveVideo( m_gameBeginTime );
	//重置数据
	ReSetGameState(bCloseFlag);

	//设置数据
	m_dwBeginTime = (long int)time(NULL);

	//清理断线资料
	if (m_pGameManager->IsPrivateRoom())
	{
		if (DeskCanDismiss())//如果能够达到桌子解散的条件，那就将游戏状态置为牌局结束状态
		{
			m_nGameStation = GS_GAME_OVER;
		}
		else
		{
			m_nGameStation = GS_WAIT_NEXT;
		}
	}

	//将玩家状态同意状态清掉
	for (int i =0; i < m_nMaxPeople; i++)
	{
		m_UserInfo[i]._bIsAgree = false;
	}

	if (!m_bAllAgreeDismissRoom)//超过第一局，才扣房卡
	{
		m_pGameManager->OnGameFinish(m_nDeskIndex);
	}
	killAllTimer();
	DebugPrintf(m_pGameManager->GetRoomID(),m_nDeskIndex,"GameFinish====>end");
	return 0;
}

//重置游戏状态
bool CMJDesk::ReSetGameState(BYTE bLastStation)
{
	//if (this->DeskCanDismiss())
	//{
	//	//关闭游戏的流程服务
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

///表示游戏是否开始过（用局数来区分）
bool CMJDesk::IsHavePlaying()
{
	return m_tagRoundFinish.iRoundTimes > 0;
}


///是否达到解散的条件
bool CMJDesk::DeskCanDismiss()
{
	return (m_tagRoundFinish.iRoundTimes >= m_tagRoundFinish.iAllRoundTimes || m_dismiss_condition_valid);
}


///配置文件的处理
void CMJDesk::OnDeskConfigured(const char* szClientConfig, const char *szSysConfig)
{
	m_bAllAgreeDismissRoom = false;
	Init(NULL);
	m_nGameStation = GS_WAIT_ARGEE;

	m_bNTResult = false;		//是否庄闲算分
	m_bAllowDui7 = false;	//是否可胡七对
	m_bPiao = false;			//是否加飘
	m_bZhongCaishen = true;	//红中为财神
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


	//房间人数

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

// 设置定时器
void CMJDesk::SetTimer( int iTimeID, int iTimerInterval )
{
	m_pGameManager->SetGameTimer(m_nDeskIndex, iTimeID, iTimerInterval);
}

void CMJDesk::KillTimer( int iTimeID )
{
	m_pGameManager->KillGameTimer(m_nDeskIndex, iTimeID);
}
//设定庄家事件
bool CMJDesk::OnEventMakeNT()
{
	DebugPrintf(m_pGameManager->GetRoomID(),m_nDeskIndex,"OnEventMakeNT====>begin");
	Json::Value response;
	if (m_tagRoundFinish.iRoundTimes == 1)
	{
		//首局随机庄家
		::srand(GetTickCount());

		//m_byNTUser = rand() % m_iPlayerCount;

		int nDice[2]={0,0};
		nDice[0]=rand()%6+1;
		nDice[1]=rand()%6+1;
		response["dice1"]=nDice[0];
		response["dice2"]=nDice[1];

		m_byNTUser=((nDice[0]+nDice[1])-1)%m_iPlayerCount;


#if PEI_PAI_ON
		//如果有玩家配起牌 就让这个玩家做庄
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
		//第二局开始以定庄的规则如下：
		//如果荒庄,不换庄;庄家赢,不换庄;庄家没赢,胡牌玩家坐庄
		if (!GetIsHe())
		{
			//获取庄玩家的数据
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

	//上一局的结局记录已没有利用价值，清掉
	for (int i = 0; i < m_iPlayerCount; ++i)
	{
		m_UserData[i].m_nResult = MjResult::Res_Unknown;
		m_UserData[i].m_bIsLastRoundQiangGang=false;
		m_UserData[i].m_bLastDianGangKai=false;
	}

	//广播定庄结果/////////////////////////////////////////////////////////



	//如果w崩溃 此处值可能会不合法
	if (m_byNTUser == 255)
	{
		::srand(GetTickCount());
		m_byNTUser = rand() % m_iPlayerCount;
	}

	response["NtUser"] = m_byNTUser;
	//TODO 如果到了第二轮,庄家为上局已经胡牌的那个玩家
	//当前第几局
	response["RoundTimes"] = m_tagRoundFinish.iRoundTimes;
	response["AllRoundTimes"] = m_tagRoundFinish.iAllRoundTimes;
	for (int i = 0; i < m_iPlayerCount; ++i)
	{
		SendGameData(i,response,ASS_MAKE_NT_INFO);
	}
	this->InsertGameVideo( Video_Event::Video_Event_select_banker , response );

	//第一局要等客户端播放选庄动画
	if(m_tagRoundFinish.iRoundTimes > 1)
	{

		//启动定起牌方向及起牌墩位流程
		SetTimer(TIME_FETCH_METHOD, 500);

	}
	else
	{
		//第一局有定庄动画要多等待时间
		SetTimer(TIME_NT_MORE, 2*1000);
	}
	DebugPrintf(m_pGameManager->GetRoomID(),m_nDeskIndex,"OnEventMakeNT====>end");
	return true;
}
//根据各玩家的m_nResult判断是否为荒庄
///@return 是否荒庄局
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

//系统发手牌事件
bool CMJDesk::OnEventFetchHandCards()
{
	DebugPrintf(m_pGameManager->GetRoomID(),m_nDeskIndex,"OnEventFetchHandCards====>begin");
//	// 测试手牌
//	//这每个玩家取手牌,这牌城中取牌
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

		//如果有玩家配置发牌数据
		if (m_UserData[i].m_bFetchHandCardCheat )
		{
			::memcpy(m_UserData[i].m_byHandCards, m_UserData[i].m_byHandCardsCheat, m_UserData[i].m_iHandNums*sizeof(int));
		}
		m_UserData[i].m_bFetchHandCardCheat = false;
	}
#endif
	//广播玩家取手牌的数据/////////////////
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

		//牌墙剩余牌数量
		video_send_card["WallLeft"] = root["WallLeft"] = GetWallLeft();
		//下发数据

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

	//广播取手牌消息后，客户端播放取手牌动画，标志着游戏进入GS_FETCH_HANDCARDS状态（各玩家方位取手张牌状态）
	m_nGameStation = GS_PLAYING;

	//对手牌进行排序(防止第一手牌断线重连客户端乱序)
	for (int i = 0 ; i< m_iPlayerCount ; i++)
	{
		CMJLogic::sort(m_UserData[i].m_byHandCards,m_UserData[i].m_iHandNums);
	}

	SetTimer(TIME_FETCH_GUI,2500);

	DebugPrintf(m_pGameManager->GetRoomID(),m_nDeskIndex,"OnEventFetchHandCards====>end");
	return true;
}



///描述：为某个玩家发牌
///@param bDeskStation玩家位置，由外界保证不越界；iSendNum发牌数量。
///@return 
bool CMJDesk::DealHandCardsTo(BYTE bDeskStation,int iSendNum)
{
	UserData* pUserData = &m_UserData[bDeskStation];

	TMjFetch tagFetch;

	for (int i = 0; i < iSendNum; ++i)
	{
		//按顺时针从牌墙取牌
		do 
		{
			tagFetch.val = m_iAllCard[--m_iMjCardNum];
			m_iAllCard[m_iMjCardNum] = 0;///抓过的牌要置为255
			tagFetch.left = m_iMjCardNum;
		} while (tagFetch.val == 0 && tagFetch.left > 0);
		m_iWallLeft --;
		pUserData->m_byHandCards[i] = tagFetch.val;
	}
	pUserData->m_iHandNums = iSendNum;

	return true;
}

//玩家加飘
bool CMJDesk::OnEventPiao()
{
	for (int i= 0 ; i< m_iPlayerCount ; i++)
	{
		//通知玩家开始选择是否加飘
		Json::Value response;
		SendGameData(i,response,ASS_PIAO_NOTIFY);
	}
	m_nGameStation = GS_WAIT_PIAO;
	return true;
}


TMjFetch CMJDesk::FetchLaiZiCard()
{
	TMjFetch tagFetch;

	//如果取出的牌为空，但牌墙还有牌，则继续取下一张牌
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


	//dong add 没有补花直接给庄家下发令牌
	//为玩家赋令牌,有令牌的玩家进行抓牌出牌交互,庄家先得令,但不能抓牌
	m_tagParam.bCanFetch = false;
	m_tagParam.byNextUser = m_byNTUser;

	SetTimer(TIME_GIVE_NEXT_TOKEN,1500);
	DebugPrintf(m_pGameManager->GetRoomID(),m_nDeskIndex,"OnEventFetchGui====>end");
	return true;
}


//系统为下一个玩家下发令牌,得令牌者抓牌出牌
bool CMJDesk::OnEventGiveNextToken()
{
	DebugPrintf(m_pGameManager->GetRoomID(),m_nDeskIndex,"OnEventGiveNextToken====>begin");
	TEvTokenParm* pEvParm = &m_tagParam;

	//令牌移交/////////////////////////////////////////////////////////////////////////////////////////////


	if (pEvParm->byNextUser != 255) //移交到指定玩家
	{
		//如果本局第一次发令牌,令牌应交给庄家
		m_byTokenUser = pEvParm->byNextUser;
	}
	else
	{
		//按逆时针方向取下一个玩家,令牌移交给ta
		m_byTokenUser = CMJLogic::GetNextUserStation(m_byTokenUser,false,m_iPlayerCount);
	}

	UserData* pTokenUser = &m_UserData[m_byTokenUser];

	//dong change
	//玩家获得令牌  漏胡状态更新
	pTokenUser->m_bLouHu = false;
	//玩家漏胡哪些牌
	::memset(pTokenUser->m_LouHuCard,0,sizeof(pTokenUser->m_LouHuCard));

	pTokenUser->clearLouPeng();

	//系统为玩家抓一张牌//////////////////////////////////////////////////////////////////////////////////
	//首位得令牌的玩家，即庄家，无需抓牌 
	//如果玩家不要取牌，在启动本事件前把pTokenUser->m_bCanFetch设为false。

	TMjFetch tagFetch;
	bool bFetchHua = false;

	if (pEvParm->bCanFetch)
	{
		if (GetWallLeft()-m_iNiaoNums <= 0) //牌已取完
		{
			//直接结束本轮游戏
			m_RoundFiniParm.bHuang = true;
			SetTimer(TIME_ROUND_FINISH,500);

			DebugPrintf(m_pGameManager->GetRoomID(),m_nDeskIndex,"OnEventGiveNextToken====>1");
			return true;
		}

		//为玩家抓一张牌
		tagFetch = FetchCardTo(m_byTokenUser);

	}

	DebugPrintf(m_pGameManager->GetRoomID(),m_nDeskIndex,"OnEventGiveNextToken====>chair=%d,card=%d",m_byTokenUser,tagFetch.val);
	//广播发令牌的消息////////////////////////////////////////////////////////////////////////////////////
	Json::Value tokenResp;
	Json::Value video_mo;
	tokenResp["TokenbyUser"] = m_byTokenUser;;//得令者
	tokenResp["TokenbFetch"] = pEvParm->bCanFetch;//得令者是否取牌

	for (int i = 0; i < m_iPlayerCount; ++i)
	{
		//只有令牌得者才能知道取牌值。或者补花时各玩家需要知道取牌值
		int CardVal = (i == m_byTokenUser || bFetchHua) ? tagFetch.val : 0;

		tokenResp["TokenFetcherVal"] = CardVal;//取到的牌
		//dong change

		//牌墙剩余牌数量
		tokenResp["WallLeft"] = GetWallLeft();

		SendGameData(i,tokenResp,ASS_TOKEN);
		if( i == m_byTokenUser)
		{
			video_mo["moInfo"] = tokenResp;
		}
	}


	//抓到的牌可能使玩家TokenUser有动作///////////////////////////////////////////////////////////////////
	//计算各玩家pTokenUser因牌而产生的动作信息\
	//产生的动作信息将保存在玩家pTokenUser的数据m_tagActInfo中

	USHORT usIgnoreFlags = 0;

	//自己摸牌肯定不能吃碰
	usIgnoreFlags |= E_MAHJONG_ACTION::ACT_PENG;

	//没有抓牌不能胡与杠
	if (!pEvParm->bCanFetch)
	{
		//非首轮庄家出牌，才限制胡与杠
		if (!(pTokenUser->m_iOutedTimes == 0 && m_byTokenUser == m_byNTUser))
		{
			usIgnoreFlags |=  E_MAHJONG_ACTION::ACT_HU;
		}
	}

	//如果玩家已上听，只能胡
	if (pTokenUser->m_bIsTing)
	{
		usIgnoreFlags = ~ E_MAHJONG_ACTION::ACT_HU;
	}

	bool bHaveAct = SelectActInfoFrom(255,m_byTokenUser,0,&pTokenUser->m_tagActInfo,tagFetch.val,usIgnoreFlags);
	if (bHaveAct)
	{
		if (pTokenUser->m_tagActInfo.usFlags & E_MAHJONG_ACTION::ACT_HU)
		{
			if (pTokenUser->m_iOutedTimes == 0) //首次抓牌
			{
				if (m_byTokenUser == m_byNTUser)//庄家有天胡
				{
					pTokenUser->m_tagActInfo.nHuSpecWay[0] = emHuSpecialWay::hsw_TianHu;
				}
			}

			//杠上开花，pTokenUser->m_nHuSpecWay在pTokenUser执行杠牌时置的，在本事件结束前注意将其清空
			if (emHuSpecialWay::hsw_DianGangKai == pTokenUser->m_nHuSpecWay[1] || emHuSpecialWay::hsw_AnGangKai == pTokenUser->m_nHuSpecWay[1] || emHuSpecialWay::hsw_BuGangKai == pTokenUser->m_nHuSpecWay[1]) 
			{
				pTokenUser->m_tagActInfo.nHuSpecWay[1] = pTokenUser->m_nHuSpecWay[1];
			}
		}

		for (int i = 0; i < m_iPlayerCount; ++i)
		{
			TActNotify tagActNotify;

			//获得玩家i的数据
			UserData* pUser = &m_UserData[i];

			Json::Value actResp;

			//有动作才复制数据
			if (i == m_byTokenUser)
			{
				::memcpy(&tagActNotify.info,&pUser->m_tagActInfo,sizeof(TMjActInfo));

				actResp["usFlags"] = tagActNotify.info.usFlags;
				//dong add

				if (tagActNotify.info.usFlags & E_MAHJONG_ACTION::ACT_CHI)
				{
					//发送可吃牌信息
					actResp["isCanC"] = true;//是否可以吃牌
					actResp["iChiNums"] = tagActNotify.info.iChiNums;//吃牌选择数量
					actResp["usChiFlags"] = tagActNotify.info.usChiFlags;//吃的标志 吃(头中尾)
				}
				else
				{
					actResp["isCanC"] = false;
				}

				if (tagActNotify.info.usFlags & E_MAHJONG_ACTION::ACT_PENG)
				{
					//发送可碰牌信息
					actResp["isCanP"] = true;

				}
				else
				{
					actResp["isCanP"] = false;
				}

				if (tagActNotify.info.usFlags & E_MAHJONG_ACTION::ACT_GANG)
				{
					//发送可杠牌信息

					actResp["isCanG"] = true;
					actResp["iGangNums"] = tagActNotify.info.iGangNums;//可以杠选择数量
					for (int j = 0 ; j < MJ_MAX_CGP ; j++)
					{
						actResp["byGangSels"].append(tagActNotify.info.byGangSels[j]);//可以杠的选择
						actResp["nBuGangInPeng"].append(tagActNotify.info.nBuGangInPeng[j]);//补杠在建立在那一个碰牌上
						actResp["nGType"].append(tagActNotify.info.nGType[j]);//杠类型
					}
				}
				else
				{
					actResp["isCanG"] = false;
				}

				if (tagActNotify.info.usFlags & E_MAHJONG_ACTION::ACT_HU)
				{
					//发送可胡牌信息
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

	//由于CUser::m_nHuSpecWay已转移到CUser::m_tagActInfo，
	//这时需要清空CUser::m_nHuSpecWay的杠上开花标志
	pTokenUser->m_nHuSpecWay[1] = emHuSpecialWay::hsw_Unknown;

	if (m_bHaveGang)
	{
		//TokenUser逆时针取牌后出牌，其他三家可以胡的话，叫杠冲
		//不管其他三家能否胡，先把其特殊胡牌置为杠冲，在TokenUser的出牌处理时再确定
		//是否把CUser::m_nHuSpecWay转到CUser::m_tagActInfo中

		for (int i = 0; i < m_iPlayerCount; ++i)
		{
			if (i != m_byTokenUser)
			{
				//获得玩家i的数据
				UserData* pUser = &m_UserData[i];
				pUser->m_nHuSpecWay[1] = emHuSpecialWay::hsw_GangChong;
			}
		}
	}
	m_bHaveGang = false;



	if (!m_pGameManager->IsPrivateRoom() )
	{
		if (pTokenUser->m_bAuto)//托管
		{
			//如果是金币房间 并且该玩家已经托管
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
		else//未托管
		{
			///如果金币场时间到了要自动出牌
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

//获取剩在牌城的牌数量
//@return 剩余数量
int CMJDesk::GetWallLeft()
{
	return m_iWallLeft;
}


//描述：为某个玩家抓一张牌
///@param bDeskStation玩家位置,bCW从牌墙的方向(true为顺时针)。
///@return 补花的数量
TMjFetch CMJDesk::FetchCardTo(BYTE bDeskStation)
{
	TMjFetch tagFetch;

	//如果取出的牌为空，但牌墙还有牌，则继续取下一张牌
	do 
	{
		tagFetch.val = m_iAllCard[--m_iMjCardNum];
		m_iAllCard[m_iMjCardNum] =0;
		tagFetch.left = m_iMjCardNum;

	} while(tagFetch.val == 0 && tagFetch.left > 0);

	if (CMJLogic::Verify(tagFetch.val)) //如果取的牌是有效的
	{
		if(bDeskStation!=255)
		{
			//取得抓牌玩家的数据
			UserData* pUser = &m_UserData[bDeskStation];

			if (CMJLogic::Verify(pUser->m_byChFetch))
			{
				tagFetch.val = pUser->m_byChFetch;
				pUser->m_byChFetch = 0;
			}

			//把取出的牌加到玩家bDeskStation的手牌数据中

			pUser->m_byHandCards[pUser->m_iHandNums++] = tagFetch.val;
			//记录取牌次数
			pUser->m_iFetchTimes++;

			m_byLastFetchCardUser=bDeskStation;
		}



		m_iWallLeft --;
	}

	return tagFetch;
}




///描述：从某玩家中取得动作消息
///@param byFromUser动作触发玩家,值为255表示自己触发，如抓牌后有动作;\
byToUser动作执行玩家,byCard打出或抓的牌，[OUT]pActInfo详细的动作消息;\
usIgnoreFlags,有那些动作忽略不检测的话,在些打上开关,使用类似MjActFlag::usTing进行按位与运算
///@return 是否有动作
bool CMJDesk::SelectActInfoFrom(BYTE byFromUser,BYTE byToUser,int byCard,TMjActInfo* pActInfo,int byFetctCard,USHORT usIgnoreFlags/* = 0*/,bool bIsQiangGangJudge)
{
	//初始ActInfo
	pActInfo->Clear();

	//获得玩家byFromUser的数据
	UserData* pUserF = NULL;
	if(byFromUser != 255)
	{
		pUserF = &m_UserData[byFromUser];
	}
	//获得玩家byToUser的数据
	UserData* pUserT = &m_UserData[byToUser];


	///如果是别人出的牌要将牌加入到自己手牌里面来进行，检测是否可以胡
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
		//判断可否吃
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
		//判断玩家能否碰牌
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
		////判断玩家能否杠
		if (CMJLogic::IsCanGang(byCard,pUserT->m_byHandCards,pUserT->m_iHandNums,
			pUserT->m_lstCGP,pUserT->m_iCGPNums,pActInfo,byFetctCard))
		{
			//杠后取牌不能进行补杠动作
			pActInfo->usFlags |= (E_MAHJONG_ACTION::ACT_GANG | E_MAHJONG_ACTION::ACT_GUO);
		}
	}

	if (!(usIgnoreFlags & E_MAHJONG_ACTION::ACT_TING))
	{
		////判断能否听,只能抓牌才能上听,而且是第二次抓牌前才能听(称起手听)
		////DONG change 转转麻将随时可以听 
		//if (CMJLogic::IsCanTing(pUserT->m_byHandCards,pUserT->m_iHandNums,0,false))
		//{
		//	pActInfo->usFlags |= (E_MAHJONG_ACTION::ACT_TING | E_MAHJONG_ACTION::ACT_GUO);
		//}
	}

	if (!(usIgnoreFlags & E_MAHJONG_ACTION::ACT_HU))
	{
		//dong change
		// 要考虑抢杠
		if (!(byCard==m_byLaiZi && byFromUser!=255) && CMJLogic::IsCanHu(byCard,byFetctCard,byTemp,iTempNums,pUserT->m_lstCGP,pUserT->m_iCGPNums,pActInfo,m_byLaiZi,NULL,bIsQiangGangJudge))
		{
			//自摸可以胡 如果漏胡被人打出的不能胡
			bool isCanHu = true;
			//if ( byFromUser != 255 )
			//{
			//	if (pUserT->m_bLouHu && byCard != 0)
			//	{

			//		isCanHu = false;
			//	}
			//}

			//自摸胡 只能自摸
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

//得到选庄动画完成的玩家数量
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



//获取指定玩家以外其他玩家有没有动作
//@return true其他玩家有动作;false其他玩家没动作
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

///描述：为某一玩家出一张牌
///@param bDeskStation玩家位置,要出的牌。
///@return 补花的数量
void CMJDesk::OutCardTo(BYTE bDeskStation,int byCard)
{
	UserData* pUser = &m_UserData[bDeskStation];
	pUser->m_dianGangGangKaiIndex = -1;
	m_byLastOutCard = byCard;
	//删除一张手牌
	if (CMJLogic::RemoveOneCard(byCard,pUser->m_byHandCards,pUser->m_iHandNums))
	{
		pUser->m_iHandNums--;

		//重新排列手牌
		CMJLogic::sort(pUser->m_byHandCards,pUser->m_iHandNums);

		//向出牌表中添加该牌
		pUser->m_byOutedCards[pUser->m_iOutedNums++] = byCard;

		//更新出牌计数
		pUser->m_iOutedTimes++;
	}

	return;
}
///描述：通过各玩家的动作状态位，得到马上可以执行动作的玩家集
///@param [OUT]byUser可以立刻进行动作的玩家表，[IN]usActFlag这些玩家可进行的动作
///@return 可以立刻进行动作的玩家数量
int CMJDesk::SelectActAtOnceUsers(BYTE byUsers[],USHORT& usActFlag)
{
	usActFlag = 0L;
	//定义未申请动作的玩家中最大动作玩家，及动作
	unsigned short usMax1 = 0;
	int byMax1 = 255;

	//定义已申请动作的玩家中最大动作玩家，及动作
	unsigned short usMax2 = 0;
	int byMax2 = 255;
	for (int i = 0; i < PLAY_COUNT; ++i)
	{
		USHORT usFlags = m_UserData[i].m_tagActInfo.usFlags;

		if (usFlags & 0x0003) //有动作的玩家
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
	//通炮所有能胡的选择完以后 弹出结算界面  (点过的人不让胡)
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

	//一炮单响时候 离出牌玩家最近的可以优先执行

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
		//dong add 改为只可以单响

		int nearlestPlayer = -1;
		for (int i = 1; i< PLAY_COUNT ; i++ )
		{
			// 依次找是否在byToUsers 里面
			for (int j = 0 ;j< iCounter ; j++)
			{
				if (byUsers [j] == ((m_byTokenUser + i)%4))
				{
					//找到最的玩家
					nearlestPlayer = byUsers [j];
					break;
				}
			}

			if (nearlestPlayer >= 0)
			{
				break;
			}
		}
		//离出牌玩家最近的那个人随时可以胡
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

//收到客户端的加飘请求
int CMJDesk::OnUserPiao(BYTE bDeskStation,void* pData,UINT uSize,bool bWatchUser)
{
	Json::Reader reader(Json::Features::strictMode());
	//收到玩家发来的数据
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

	//通知桌面玩家有飘
	for (int i=0 ; i<m_iPlayerCount ; i++)
	{
		Json::Value response;
		response["chair"] = bDeskStation;
		response["piaoType"] = _root["piaoType"].asInt();
		SendGameData(i,response,ASS_PIAO_RESP);
	}


	//当所有玩家都加飘完成执行下一个Event
	m_UserData[bDeskStation].m_bPiao = true;
	m_iPiaoCount++;
	if (m_iPiaoCount == m_iPlayerCount)
	{
		SetTimer(TIME_FETCH_HANDCARDS,500);
	}
	return 0;
}

///名称：OnUserMakeNTOver
///描述：处理玩家完成播放定庄动画报告
///@param bDeskStation 位置, pNetHead 消息头,pData 消息数据,uSize 消息包长度,uSocketID id,bWatchUser 是否旁观
///@return
int CMJDesk::OnUserMakeNTOver(BYTE bDeskStation,void* pData,UINT uSize,bool bWatchUser)
{
	UserData* pUser = &m_UserData[bDeskStation];

	//防错//////////////////////////////////////////////////////////////////

	if (bWatchUser)
	{
		return 0;
	}

	if (pUser->m_bMakeNTOK)
	{
		return 0;
	}

	//处理//////////////////////////////////////////////////////////////////

	//把玩家置为已定方位状态
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

///名称：OnUserOutCardRequest
///描述：处理玩家出牌请求
///@param bDeskStation 位置, pNetHead 消息头,pData 消息数据,uSize 消息包长度,uSocketID id,bWatchUser 是否旁观
///@return
int CMJDesk::OnUserOutCardRequest(BYTE bDeskStation,void* pData,UINT uSize,bool bWatchUser)
{
	DebugPrintf(m_pGameManager->GetRoomID(),m_nDeskIndex,"OnUserOutCardRequest====>chair=%d",bDeskStation);
	if (NULL == pData || 0 == uSize)
	{
		DebugPrintf(m_pGameManager->GetRoomID(),m_nDeskIndex,"玩家=%d,数组错误，断线处理",bDeskStation);
		return -1;
	}

	//dong add
	Json::Reader reader(Json::Features::strictMode());

	//收到玩家发来的数据
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
	//获得出牌玩家数据
	UserData* pUserF = &m_UserData[bDeskStation];

	//防错//////////////////////////////////////////////////////////////////
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

	//其他玩家有动作未响应，不能出牌
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

	//这里面要注意
	if (!pUserF->m_bIsTing)
	{
		//自由出牌的状态，则从所有手牌中判断
		if (!CMJLogic::InCards(cardvalue,pUserF->m_byHandCards,pUserF->m_iHandNums))
		{
			DebugPrintf(m_pGameManager->GetRoomID(),m_nDeskIndex,"OnUserOutCardRequest====>7");
			return -1;
		}
	}
	else
	{
		//如果玩家已听牌,只能出最后抓的一张
		if (cardvalue != pUserF->m_byHandCards[pUserF->m_iHandNums - 1])
		{
			DebugPrintf(m_pGameManager->GetRoomID(),m_nDeskIndex,"OnUserOutCardRequest====>8");
			return 0;
		}
	}

	//处理///////////////////////////////////////////////////////////////////////////////////////////////
	//如果玩家出牌前带有动作，则不需要清空动作信息
	if (pUserF->m_tagActInfo.usFlags > 0)
	{
		pUserF->m_tagActInfo.Clear();
	}
	m_nLastPengCard=255;
	m_nLastPengUser=255;
	//为玩家bDeskStation打出一张牌,玩家的手牌中删除出牌
	OutCardTo(bDeskStation,cardvalue);

	//广播玩家bDeskStation的出牌结果//////////////////////////////////////////////////////////////////////

	TOutCard tagOC;
	tagOC.nType = TOutCard::INFO;
	tagOC.byCard = cardvalue;
	tagOC.byUser = bDeskStation;

	//测试用，把手牌发到客户端，与手牌控件数据对比。
	::memcpy(&tagOC.byCards,pUserF->m_byHandCards,pUserF->m_iHandNums*sizeof(int));
	tagOC.iHandNums = pUserF->m_iHandNums;

	// 20181203玩家出牌后所有的能刚却碰即使再杠，杠分失效
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
		//出的牌
		tokenResp["outCardValue"] = tagOC.byCard;
		//出牌的玩家
		tokenResp["outCardUser"] = tagOC.byUser;
		//出牌数量
		tokenResp["outCardNum"] = pUserF->m_iOutedNums;


		//手牌剩余数量
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

	//玩家打出牌可能会引起动作////////////////////////////////////////////////////////////////////////////

	//计算各玩家因bDeskStation打出的牌而产生的动作信息\
	//产生的动作信息将保存在玩家i的数据m_tagActInfo中
	bool bHaveAct = false;

	for (int i = 0; i < m_iPlayerCount; ++i)
	{
		if (i != bDeskStation) //非出牌都才可能有动作
		{
			//获得玩家i的数据
			UserData* pUserT = &m_UserData[i];

			USHORT usIgnoreFlags = 0;

			//如果玩家已听牌，只能胡
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
				//获得玩家i的数据
				UserData* pUser = &m_UserData[i];

				if (pUser->m_tagActInfo.usFlags & E_MAHJONG_ACTION::ACT_HU)
				{
					//地胡的判定
					if (i != m_byNTUser) //闲家
					{
						if (pUserF->m_iOutedTimes == 1) //放炮者出第一张牌
						{
							if (bDeskStation == CMJLogic::GetNextUserStation(i,true,m_iPlayerCount)) //放炮者是胡牌者的上家
							{
								pUser->m_tagActInfo.nHuSpecWay[0] = emHuSpecialWay::hsw_DiHu;
							}
						}
					}

					//杠冲，pTokenUser->m_nHuSpecWay在pTokenUser抓牌时置的，在本事件结束前注意将其清空
					if (emHuSpecialWay::hsw_GangChong == pUser->m_nHuSpecWay[1]) 
					{
						pUser->m_tagActInfo.nHuSpecWay[1] = emHuSpecialWay::hsw_GangChong;
					}
				}
			}
		}
	}

	//如果有玩家有动作，向各玩家发送动作通知
	if (bHaveAct)
	{
		for (int i = 0; i < m_iPlayerCount; ++i)
		{
			TActNotify tagActNotify;

			//获得玩家i的数据
			const UserData* pUser = &m_UserData[i];


			//dong add 给客户端返回玩家有可执行的动作数据
			Json::Value actResp;

			//有动作才复制数据
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
					//发送可吃牌信息
					actResp["isCanC"] = true;//是否可以吃牌
					actResp["iChiNums"] = tagActNotify.info.iChiNums;//吃牌选择数量
					actResp["usChiFlags"] = tagActNotify.info.usChiFlags;//吃的标志 吃(头中尾)
				}
				else
				{
					actResp["isCanC"] = false;
				}

				if (tagActNotify.info.usFlags & E_MAHJONG_ACTION::ACT_PENG)
				{
					//发送可碰牌信息
					actResp["isCanP"] = true;

				}
				else
				{
					actResp["isCanP"] = false;
				}

				if (tagActNotify.info.usFlags & E_MAHJONG_ACTION::ACT_GANG)
				{
					//发送可杠牌信息
					actResp["isCanG"] = true;
					actResp["iGangNums"] = tagActNotify.info.iGangNums;//可以杠选择数量
					for (int i= 0 ; i< MJ_MAX_CGP ; i++)
					{
						actResp["byGangSels"].append(tagActNotify.info.byGangSels[i]);//可以杠的选择
					}for (int i= 0 ; i< MJ_MAX_CGP ; i++)
					{
						actResp["nBuGangInPeng"].append(tagActNotify.info.nBuGangInPeng[i]);//补杠在建立在那一个碰牌上
					}

				}
				else
				{
					actResp["isCanG"] = false;
				}

				if (tagActNotify.info.usFlags & E_MAHJONG_ACTION::ACT_HU)
				{
					//发送可胡牌信息
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
	//由于CUser::m_nHuSpecWay已转移到CUser::m_tagActInfo，\
	这时需要清空CUser::m_nHuSpecWay的杠冲标志
	for (int i = 0; i < m_iPlayerCount; ++i)
	{
		//获得玩家i的数据
		UserData* pUser = &m_UserData[i];
		pUser->m_nHuSpecWay[1] = emHuSpecialWay::hsw_Unknown;
	}

	//动作流转向////////////////////////////////////////////////////////////////////////////////////////////

	//玩家出牌完成,情况如下：\
	1.如果没有人有动作,系统为下个玩家赋令牌\
	2.如有动作，即断流等待玩家选择动作。需要有流程保护

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
			//如果是金币房间 
			//m_pGameManager->SetGameTimer(m_nDeskIndex,TIME_AUTO_ACT,1000);
			SetTimer(TIME_AUTO_ACT,1000);
		}
		//m_pGameManager->SetGameTimer(m_nDeskIndex,TIME_AUTO_ACT_NO,m_tagDeskCfg.byAct * 1000);
		SetTimer(TIME_AUTO_ACT_NO,m_tagDeskCfg.byAct * 1000);
	}
	DebugPrintf(m_pGameManager->GetRoomID(),m_nDeskIndex,"OnUserOutCardRequest====>end");
	return 0;
}
///名称：OnUserActRequest
///描述：处理玩家的动作请求
///@param bDeskStation 位置, pNetHead 消息头,pData 消息数据,uSize 消息包长度,uSocketID id,bWatchUser 是否旁观
///@return
int CMJDesk::OnUserActRequest(BYTE bDeskStation,void* pData,UINT uSize,bool bWatchUser)
{
	DebugPrintf(m_pGameManager->GetRoomID(),m_nDeskIndex,"OnUserActRequest====>chair=%d",bDeskStation);
	if (NULL == pData || 0 == uSize)
	{
		DebugPrintf(m_pGameManager->GetRoomID(),m_nDeskIndex,"玩家=%d,数组错误，断线处理",bDeskStation);
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

	//获得出牌请求玩家的数据
	UserData* pUser = &m_UserData[bDeskStation];

	//得到玩家的动作标记
	USHORT usActFlags = pUser->m_tagActInfo.usFlags;

	//防错//////////////////////////////////////////////////////////////////////////////////////
	DebugPrintf(m_pGameManager->GetRoomID(),m_nDeskIndex,"OnUserActRequest====>sel_act=%d,act=%d",m_usAct,usActFlags);
	if (bWatchUser)
	{
		DebugPrintf(m_pGameManager->GetRoomID(),m_nDeskIndex,"OnUserActRequest====>3");
		return 0;
	}
	//玩家没有动作,或已请求
	if (usActFlags == 0 || (usActFlags & E_MAHJONG_ACTION::ACT_WAIT))
	{
		DebugPrintf(m_pGameManager->GetRoomID(),m_nDeskIndex,"OnUserActRequest====>4");
		return 0;
	}
	//玩家没有些动作
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
		//检测当前玩家是否有这种吃法 (吃头中尾)
		if (!((_root["EatFlag"].asInt()) & (pUser->m_tagActInfo.usChiFlags)))
		{
			DebugPrintf(m_pGameManager->GetRoomID(),m_nDeskIndex,"OnUserActRequest====>7");
			return -1;
		}

		//记录吃的状态 要在ExecuteChiAct之前记录
		m_usChiFlags = _root["EatFlag"].asInt();

	}
	if (E_MAHJONG_ACTION::ACT_HU > m_usAct)
	{
		//玩家执行了过的操作 看看是否漏胡
		if (E_MAHJONG_ACTION::ACT_HU & usActFlags)
		{
			m_UserData[bDeskStation].m_bLouHu = true;
			//记录漏胡哪张牌
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

	//动作选择索引异常,主要针对“杠”与“听”的请求
	if (E_MAHJONG_ACTION::ACT_GANG == m_usAct)
	{
		//dong add 需要客户端传递这个值
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
		//dong add 需要客户端传递这个值 
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

	//验证成功，处理///////////////////////////////////////////////////////////////////////////////
	if (!m_pGameManager->IsPrivateRoom() )
	{
		KillTimer(TIME_AUTO_ACT);
		KillTimer(TIME_AUTO_OUTCARD);
		KillTimer(TIME_AUTO_ACT_NO);
		KillTimer(TIME_AUTO_OUTCARD_NO);
	}
	//清理玩家的动作标志，只留下请求相关动作的标志
	pUser->m_tagActInfo.usFlags &= m_usAct;

	//把玩家的动作标志置成正在等待状态
	pUser->m_tagActInfo.usFlags |= E_MAHJONG_ACTION::ACT_WAIT;


	//dong change
	//记录玩家的动作索引,主要针对“杠”与“听”的请求
	BYTE byCanActUsers[PLAY_COUNT];
	USHORT usAct = 0;

	//获取有哪些玩家可马上执行动作
	int iAtOnceCounter = SelectActAtOnceUsers(byCanActUsers,usAct);

	//如果有可立刻执行的动作。\
	关于流程的转向方法在每种动作中有所差异，所以由各种动作的执行函数中自行处理\
	注意，执行动作后注意清理所有玩家的动作记录

	if (iAtOnceCounter > 0)
	{
		//IDEV_GIVE_NEXT_TOKEN事件已完成,则于IDEV_GIVE_NEXT_TOKEN是受保护,要释放保护
		//CloseEventProtection();

		//如果iAtOnceCounter == 1,那么只有byCanActUsers[0]执行动作
		//如果iAtOnceCounter > 1,表明有多个玩家可执行同一个动作,那么根据动作进行优先处理


		if(E_MAHJONG_ACTION::ACT_GUO == usAct)
		{
			//为玩家执行过操作
			ExecuteGuoAct(bDeskStation);

		}
		else if (E_MAHJONG_ACTION::ACT_PENG == usAct)
		{
			//为玩家执行碰操作
			ExecutePengAct(m_byTokenUser,byCanActUsers[0]);
		}
		else if (E_MAHJONG_ACTION::ACT_GANG == usAct)
		{
			//为玩家执行杠操作
			ExecuteGangAct(m_byTokenUser,byCanActUsers[0]);
		}	
		else if (E_MAHJONG_ACTION::ACT_TING == usAct)
		{
			//为玩家执行听操作
			ExecuteTingAct(byCanActUsers[0]);
		}
		else if (E_MAHJONG_ACTION::ACT_HU == usAct)
		{
			//为玩家执行胡操作
			ExecuteHuAct(m_byTokenUser,byCanActUsers,iAtOnceCounter);
		}
		else if (E_MAHJONG_ACTION::ACT_CHI == usAct)
		{
			//玩家执行吃牌动作
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

//执行“过”动作
bool CMJDesk::ExecuteGuoAct(BYTE bDeskStation)
{
	UserData* pUser = NULL;

	// 20181204 看剩下玩家是否还有非游金抢杠玩家, 如果没有非游金玩家了, 则游金玩家不可以抢杠了,
	// 如果有非游金抢杠玩家选择过, 没有游金玩家了则游戏结算
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
				//获得玩家i的数据
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
		// 游戏结束
		OnEventRoundFinish();
	}

	//看看这次有没有过掉抢杠
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

	//各玩家的动作信息可清除///////////
	for (int i = 0; i < m_iPlayerCount; ++i)
	{
		m_UserData[i].m_tagActInfo.Clear();
	}

	//启动系统为一下一个玩家赋令牌的流程,\
	如果有过掉抢杠机会的,要让执行杠牌者,逆时针补牌

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

//执行“碰”动作
///@param byFromUser 碰谁；bToUser 谁碰；
bool CMJDesk::ExecutePengAct(BYTE byFromUser,BYTE byToUser)
{
	TMjCGPNode tagNode;

	//获取玩家byToUser的数据
	UserData* pUserT = &m_UserData[byToUser];

	//玩家byToUser碰byFromUser的牌
	ActPengTo(byFromUser,byToUser,&tagNode);

	//向各玩家广播动作消息////////////////////////////////////////////////

	TActInfo tagActInfo;

	//动作发出者
	tagActInfo.byUsers[0] = byToUser;
	tagActInfo.iUserNums = 1;

	//动作类型
	tagActInfo.usActFlag = E_MAHJONG_ACTION::ACT_PENG;

	//添加拦牌数据
	tagActInfo.CGPNode = tagNode;

	//更新玩家的手牌数据
	tagActInfo.iHandNums = pUserT->m_iHandNums;

	for (int i = 0; i < m_iPlayerCount; ++i)
	{
		if (byToUser == i)
		{
			//只有动作执行者的客户端才知道手牌数据
			::memcpy(tagActInfo.byHands,pUserT->m_byHandCards,sizeof(int)*pUserT->m_iHandNums);
		}
		else
		{
			::memset(tagActInfo.byHands,0,sizeof(tagActInfo.byHands));
		}

		//为玩家发送数据
		Json::Value response;
		response["iHandNums"] = tagActInfo.iHandNums;
		response["usActFlag"] = tagActInfo.usActFlag;
		response["byFromUser"] = byFromUser;//碰谁的牌
		response["OutCardNum"] = m_UserData[byFromUser].m_iOutedNums;//更新出牌玩家的出牌数量
		response["byToUser"] = byToUser;//谁碰的牌
		//碰的牌
		for(int j = 0 ; j< 3 ;j++)
		{
			response["cpgCards"].append(tagActInfo.CGPNode.val[j]);
		}

		//牌栏的位置
		response["nIdx"] = tagActInfo.CGPNode.nIdx;

		if (byToUser == i)
		{
			this->InsertGameVideo( Video_Event::Video_Event_action , response );
		}

		//手里剩下的牌
		for (int j= 0 ; j< tagActInfo.iHandNums ;j++)
		{
			if (tagActInfo.byHands[j] > 0 )
			{
				response["HandCards"].append(tagActInfo.byHands[j]);
			}

		}

		SendGameData(i,response,ASS_ACT_INFO);
	}


	//各玩家的动作信息可清除///////////////////////////////
	for (int j = 0; j < m_iPlayerCount; ++j)
	{
		m_UserData[j].m_tagActInfo.Clear();
	}

	//动作结束后，启动为下一个玩家发令牌的流程。得令牌玩家置成碰牌的玩家,不需要抓牌。
	m_tagParam.bCanFetch = false;
	m_tagParam.byNextUser = byToUser;
	SetTimer(TIME_GIVE_NEXT_TOKEN,500);
	return true;
}

///描述：某玩家执行碰的数据修改
///@param byFrom碰谁，byTo谁碰，[OUT]pCGPNode返回的拦牌结点
///@return
bool CMJDesk::ActPengTo(BYTE byFrom,BYTE byTo,TMjCGPNode* pCGPNode)
{
	//得到玩家byFrom数据
	UserData* pUserF = &m_UserData[byFrom];

	//得到玩家byTo数据
	UserData* pUserT = &m_UserData[byTo];

	//得到玩家byFrom最后打出的牌,并从出牌表中取走
	int byOutCard = pUserF->m_byOutedCards[pUserF->m_iOutedNums - 1];
	pUserF->m_iOutedNums--;

	//完善拦牌结点pCGPNode的信息,并添加到玩家byTo的拦牌表中
	pCGPNode->nType = emType::Peng;
	for (int i = 0; i < 3 ; i ++)
	{
		pCGPNode->val[i] = byOutCard;
	}
	pCGPNode->from = byFrom;

	//得到杠牌的位置
	pCGPNode->nIdx = pUserT->m_iCGPNums;

	pUserT->m_lstCGP[pUserT->m_iCGPNums++] = *pCGPNode;

	//删除玩家手上两张牌
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

//执行“杠”动作
///@param byFromUser 杠谁；bToUser 谁杠
bool CMJDesk::ExecuteGangAct(BYTE byFromUser,BYTE byToUser)
{
	TMjCGPNode tagNode;

	//获取玩家byToUser的数据
	UserData* pUserT = &m_UserData[byToUser];

	//玩家byToUser碰byFromUser的牌
	int iChPoint[PLAY_COUNT];
	ActGangTo(byFromUser,byToUser,&tagNode,iChPoint);

	//向各玩家广播动作消息/////////////////////////////////////////////////////////////////////////////////////////////////////////

	TActInfo tagActInfo;

	//动作发出者
	tagActInfo.byUsers[0] = byToUser;
	tagActInfo.iUserNums = 1;

	//动作类型
	tagActInfo.usActFlag = E_MAHJONG_ACTION::ACT_GANG;

	//添加拦牌数据
	tagActInfo.CGPNode = tagNode;

	//更新玩家的手牌数据
	tagActInfo.iHandNums = pUserT->m_iHandNums;

	for (int i = 0; i < m_iPlayerCount; ++i)
	{
		Json::Value response;
		response["usActFlag"] = tagActInfo.usActFlag;

		response["byFromUser"] = byFromUser;//杠谁的牌
		response["OutCardNum"] = m_UserData[byFromUser].m_iOutedNums;//更新出牌玩家的出牌数量
		response["byToUser"] = byToUser;//谁杠的牌

		//牌栏的位置
		response["nIdx"] = tagActInfo.CGPNode.nIdx;

		//牌墙剩余牌数量
		response["WallLeft"] = GetWallLeft();
		//杠的类型
		// 发送的动作是补杠实质是明杠-特殊处理
		if (pUserT->m_arrValidGangPoint[tagNode.val[0]] == 1 && tagNode.nType == emType::MingGang)
		{
			tagActInfo.CGPNode.nType = emType::BuGang;
		}
		response["gangType"] = tagActInfo.CGPNode.nType;
		//杠的牌
		for(int j = 0 ; j< 4 ;j++)
		{
			response["cpgCards"].append(tagActInfo.CGPNode.val[j]);
		}


		if (byToUser == i)
		{
			//只有动作执行者的客户端才知道手牌数据
			::memcpy(tagActInfo.byHands,pUserT->m_byHandCards,pUserT->m_iHandNums*sizeof(int));
			//手里剩下的牌
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

		//为玩家发送数据
		SendGameData(i,response,ASS_ACT_INFO);

		if( byToUser == i )
		{
			this->InsertGameVideo( Video_Event::Video_Event_action , response);
		}
	}



	//各玩家的动作信息可清除
	for (int i = 0; i < m_iPlayerCount; ++i)
	{
		m_UserData[i].m_tagActInfo.Clear();
	}

	//玩家补杠的牌可能引起其它玩家胡牌/////////////////////////////////////////////////////////////////////////////////////////////////////////

	//计算各玩家因bDeskStation打出的牌而产生的动作信息\
	//产生的动作信息将保存在玩家i的数据m_tagActInfo中
	bool bHaveQGHu = false;
	bool bYouJinCanQiangGang= false;	// 20181129 客户要求游金状态不能抢杠，但如果有其他玩家抢杠胡时，可以进行抢杠，这时不算金游。

	if (tagNode.nType == emType::BuGang)
	{
		for (int i = 0; i < m_iPlayerCount; ++i)
		{
			//获得玩家i的数据
			UserData* pUser = &m_UserData[i];

			if (i != byToUser) //非杠动作执行者可能有动作,只能胡
			{
				USHORT usIgnoreFlags = ~ E_MAHJONG_ACTION::ACT_HU;

				if (SelectActInfoFrom(byToUser,i,tagNode.val[0],&pUser->m_tagActInfo,0,usIgnoreFlags,true))
				{
					//如果有玩家可以抢杠胡，补充生成抢杠信息

					//打上杠冲标记
					pUser->m_tagActInfo.nHuSpecWay[1] = emHuSpecialWay::hsw_QiangGang;

					//记录杠控件位置
					pUser->m_tagActInfo.nHuGangIdx = tagNode.nIdx;

					bHaveQGHu = true;

					// 20181129 客户要求游金状态不能抢杠，但如果有其他玩家抢杠胡时，可以进行抢杠，这时不算金游。
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
					// 只要有一家非游金抢杠
					if (pUser->m_bNormalQiangGang)
					{
						bYouJinCanQiangGang = true;
					}

				}
			}
		}

		//如果有玩家可以抢杠胡，向各玩家发送动作通知
		if (bHaveQGHu)
		{
			for (int i = 0; i < m_iPlayerCount; ++i)
			{
				TActNotify tagActNotify;

				//获得玩家i的数据
				UserData* pUser = &m_UserData[i];
				Json::Value actResp;
				//有动作才复制数据
				if (pUser->m_tagActInfo.usFlags > 0)
				{
					::memcpy(&tagActNotify.info,&pUser->m_tagActInfo,sizeof(TMjActInfo));
					actResp["usFlags"] = tagActNotify.info.usFlags;
					//dong add

					//if (tagActNotify.info.usFlags & E_MAHJONG_ACTION::ACT_CHI)
					//{
					//	//发送可吃牌信息
					//	actResp["isCanC"] = true;//是否可以吃牌
					//	actResp["iChiNums"] = tagActNotify.info.iChiNums;//吃牌选择数量
					//	actResp["usChiFlags"] = tagActNotify.info.usChiFlags;//吃的标志 吃(头中尾)
					//}
					//else
					//{
					//	actResp["isCanC"] = false;
					//}

					//if (tagActNotify.info.usFlags & E_MAHJONG_ACTION::ACT_PENG)
					//{
					//	//发送可碰牌信息
					//	actResp["isCanP"] = true;

					//}
					//else
					//{
					//	actResp["isCanP"] = false;
					//}

					//if (tagActNotify.info.usFlags & E_MAHJONG_ACTION::ACT_GANG)
					//{
					//	//发送可杠牌信息
					//	actResp["isCanG"] = true;
					//	actResp["iGangNums"] = tagActNotify.info.iGangNums;//可以杠选择数量
					//	for (int i= 0 ; i< MJ_MAX_CGP ; i++)
					//	{
					//		actResp["byGangSels"].append(tagActNotify.info.byGangSels[i]);//可以杠的选择
					//	}for (int i= 0 ; i< MJ_MAX_CGP ; i++)
					//	{
					//		actResp["nBuGangInPeng"].append(tagActNotify.info.nBuGangInPeng[i]);//补杠在建立在哪一个碰牌上
					//	}

					//}
					//else
					//{
					//	actResp["isCanG"] = false;
					//}

					if (tagActNotify.info.usFlags & E_MAHJONG_ACTION::ACT_HU)
					{
						////发送可胡牌信息
						//actResp["isCanH"] = true;
						//actResp["iHuTypeNums"] = tagActNotify.info.iHuTypeNums;

						// 20181129 客户要求游金状态不能抢杠，但如果有其他玩家抢杠胡时，可以进行抢杠，这时不算金游。
						if (bYouJinCanQiangGang)
						{
							// 先发送给非游金玩家
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
							// 清除游金抢杠标记
							pUser->m_tagActInfo.usFlags = 0;
							pUser->m_bYouJinQiangGang = false;
							pUser->m_tagActInfo.nHuSpecWay[1] = emHuSpecialWay::hsw_Unknown;
							 // actResp["isCanH"] = false;
						}
					}
				}

				//为玩家发送消息
				//SendGameData(i,actResp,ASS_ACT_NOTIFY);
			}
		}
	}

	//如果没有抢杠，则应该到杠牌玩家byToUser抓牌，如果抓牌能胡则为杠上开花\
	不管byToUser能否胡，先把其特殊胡牌置为杠上开花，在IDEV_GIVE_NEXT_TOKEN事件中再确定\
	是否把CUser::m_nHuSpecWay转到CUser::m_tagActInfo中
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

	//流程处理/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//如果有抢杠，等待玩家响应胡动作，需要有流程保护
	//没有抢杠，则启动为下一个玩家发令牌的流程

	if (!bHaveQGHu)
	{
		//下次得令牌玩家置成执行杠牌的玩家，逆时针抓牌
		m_tagParam.bCanFetch = true;
		m_tagParam.byNextUser = byToUser;
		m_bHaveGang = true;
		SetTimer(TIME_GIVE_NEXT_TOKEN,500);
	}
	else
	{
		if (bYouJinCanQiangGang)
		{
			//流程保护
			m_byTokenUser = byToUser;
		}
		else
		{
			//下次得令牌玩家置成执行杠牌的玩家，逆时针抓牌
			m_tagParam.bCanFetch = true;
			m_tagParam.byNextUser = byToUser;
			m_bHaveGang = true;
			SetTimer(TIME_GIVE_NEXT_TOKEN,500);
		}
	}

	return true;
}

///描述：某玩家执行“杠”动作的数据修改
///@param byFrom杠谁，byTo谁杠，[OUT]pCGPNode返回的拦牌结点，[OUT]iChPoint[] 所有玩家因这次杠动作产生分数变化
///@return
bool CMJDesk::ActGangTo(BYTE byFrom,BYTE byTo,TMjCGPNode* pCGPNode,int iChPoint[]/* = NULL*/)
{
	//得到玩家byFrom数据
	UserData* pUserF = &m_UserData[byFrom];

	//得到玩家byTo数据
	UserData* pUserT = &m_UserData[byTo];

	//玩家的选择
	int nIndexSel = pUserT->m_tagActInfo.nIndexSel;

	emGangType nGangType = pUserT->m_tagActInfo.nGType[nIndexSel];

	//明杠数据处理///////////////////////////////////////////////////////////////////////////////////////
	if (emGangType::gt_MingGang == nGangType) 
	{
		//得到玩家byFrom最后打出的牌,并从出牌表中取走
		int byOutCard = pUserF->m_byOutedCards[pUserF->m_iOutedNums - 1];
		pUserF->m_iOutedNums--;

		//完善拦牌结点pCGPNode的信息,并添加到玩家byTo的拦牌表中
		pCGPNode->nType = emType::MingGang;
		for (int i = 0;i < 4 ; i++)
		{
			pCGPNode->val[i] = byOutCard;
		}
		pCGPNode->from = byFrom;

		//得到杠牌的位置
		pCGPNode->nIdx = pUserT->m_iCGPNums;
		pUserT->m_dianGangGangKaiIndex = pUserT->m_iCGPNums;
		pUserT->m_lstCGP[pUserT->m_iCGPNums++] = *pCGPNode;

		//删除玩家手上三张牌
		CMJLogic::RemoveCards(pCGPNode->val,3,pUserT->m_byHandCards,pUserT->m_iHandNums);
		pUserT->m_iHandNums -= 3;

		//dong add 杠分处理
		pUserF->iGangPoint -= 1;
		pUserT->iGangPoint += 1;

		//更新明杠次数
		m_tagRoundFinish.iMingGangTimes[byTo] ++;

	}

	//暗杠数据处理///////////////////////////////////////////////////////////////////////////////////////
	else if (emGangType::gt_AnGang == nGangType) 
	{
		//取得要杠的牌
		int byGangCard = pUserT->m_tagActInfo.byGangSels[nIndexSel];

		//完善拦牌结点pCGPNode的信息,并添加到玩家byTo的拦牌表中
		pCGPNode->nType = emType::AnGang;
		for (int i = 0;i < 4 ; i++)
		{
			pCGPNode->val[i] = byGangCard;
		}
		pCGPNode->from = byFrom;

		//得到杠牌的位置
		pCGPNode->nIdx = pUserT->m_iCGPNums;

		pUserT->m_lstCGP[pUserT->m_iCGPNums++] = *pCGPNode;

		//删除玩家手上四张牌
		CMJLogic::RemoveCards(pCGPNode->val,4,pUserT->m_byHandCards,pUserT->m_iHandNums);
		pUserT->m_iHandNums -= 4;

		//dong add 杠分处理
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
		//更新暗杠次数
		m_tagRoundFinish.iAnGangTimes[byTo] ++;
	}

	//补杠数据处理///////////////////////////////////////////////////////////////////////////////////////
	else if (emGangType::gt_BuGang == nGangType) 
	{
		//取得要杠的牌
		int byGangCard = pUserT->m_tagActInfo.byGangSels[nIndexSel];

		//完善拦牌结点pCGPNode的信息,补杠是要把牌加到现有的碰牌中,\
		所以pCGPNode要修改到拦牌表中，并加添加。

		//得到碰牌的位置
		pCGPNode->nIdx = pUserT->m_tagActInfo.nBuGangInPeng[nIndexSel];

		pCGPNode->nType = emType::BuGang;
		for (int i = 0;i < 4 ; i++)
		{
			pCGPNode->val[i] = byGangCard;
		}
		// 20181204保存碰牌供牌用户
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

		//删除玩家手上一张补杠牌
		CMJLogic::RemoveOneCard(byGangCard,pUserT->m_byHandCards,pUserT->m_iHandNums);
		pUserT->m_iHandNums--;

		//bool bNeedCal = true;
		//for (auto item = pUserT->m_vCanGangSelPengCard.begin(); item != pUserT->m_vCanGangSelPengCard.end(); item++)
		//{
		//	//选择了碰，后面再的杠分不计算
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
		//	// 按明杠算,否则不算分
		//	if (pUserT->m_mapValidGangPoint.at(byGangCard))
		//	{
		//		pUserF->iGangPoint -= 1;
		//		pUserT->iGangPoint += 1;
		//	}
		//}
		//else
		//{
		//	// 按正常的补杠计算
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
				// 按明杠算
				// 节点信息保存为明杠
				pCGPNode->nType = emType::MingGang;
				UserData* pUserPengFrom = &m_UserData[nPengFrom];
				pUserPengFrom->iGangPoint -= 1;
				pUserT->iGangPoint += 1;
				// 要发送action节点信息-不改
				//pCGPNode->nType = emType::MingGang;
				//pCGPNode->from  = nPengFrom;
				
				// 用户实际算分依据,即发送的杠动作是补杠，但算分是明杠
				TMjCGPNode tMjCGPNode;
				memcpy(&tMjCGPNode, pCGPNode, sizeof(TMjCGPNode));
				tMjCGPNode.nType = emType::MingGang;
				tMjCGPNode.from  = nPengFrom;
				pUserT->m_dianGangGangKaiIndex = tMjCGPNode.nIdx;
				pUserT->m_lstCGP[tMjCGPNode.nIdx] = tMjCGPNode;

		}
		else if (pUserT->m_arrValidGangPoint[byGangCard] == 0)
		{
			// 按正常的补杠计算
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
			// 杠分失效
		}


		//更新明杠次数
		m_tagRoundFinish.iBuGangTimes[byTo] ++;
	}

	return true;
}

//执行“听”动作
///@param bToUser 谁听牌
bool CMJDesk::ExecuteTingAct(BYTE byToUser)
{
	//获取玩家byToUser的数据
	UserData* pUserT = &m_UserData[byToUser];

	//玩家byToUser上听的数据处理
	ActTingTo(byToUser);

	//向各玩家广播动作消息///////////////////////////////////////////////////////////////////////////

	TActInfo tagActInfo;

	//动作发出者
	tagActInfo.byUsers[0] = byToUser;
	tagActInfo.iUserNums = 1;

	//动作类型
	tagActInfo.usActFlag = E_MAHJONG_ACTION::ACT_TING;

	//更新玩家的手牌数据
	tagActInfo.iHandNums = pUserT->m_iHandNums;
	Json::Value response ;
	response["usActFlag"] = tagActInfo.usActFlag;
	response["byToUser"] = byToUser;//谁听牌
	for (int i = 0; i < m_iPlayerCount; ++i)
	{
		if (byToUser == i)
		{
			//只有动作执行者的客户端才知道手牌数据
			::memcpy(tagActInfo.byHands,pUserT->m_byHandCards,sizeof(int)*pUserT->m_iHandNums);
		}
		else
		{
			::memset(tagActInfo.byHands,0,sizeof(tagActInfo.byHands));
		}

		//为玩家发送数据
		SendGameData(i,response,ASS_ACT_INFO);
	}

	//各玩家的动作信息可清除///////////////////////////////
	for (int i = 0; i < m_iPlayerCount; ++i)
	{
		m_UserData[i].m_tagActInfo.Clear();
	}

	return true;
}
///描述：某玩家执行“听”动作的数据修改
///@param byTo听谁
///@return 
bool CMJDesk::ActTingTo(BYTE byTo)
{
	//得到玩家byTo数据
	UserData* pUserT = &m_UserData[byTo];

	//得到动作数据
	const TMjActInfo* pActInfo = &pUserT->m_tagActInfo;

	//玩家的选择
	pUserT->m_bIsTing = true;


	return true;
}
//执行吃动作
bool CMJDesk::ExecuteChiAct(BYTE byFromUser, BYTE byToUser)
{
	TMjCGPNode tagNode;

	//获取玩家byToUser的数据
	UserData *pUserT = &m_UserData[byToUser];

	//玩家byToUser吃byFromUser的牌
	ActChiTo(byFromUser, byToUser, &tagNode);

	//向各个玩家广播动作消息
	TActInfo tagActInfo;

	//动作的发出者  发送给每个玩家
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
		response["byFromUser"] = byFromUser;//吃谁的牌
		response["OutCardNum"] = m_UserData[byFromUser].m_iOutedNums;//更新出牌玩家的出牌数量
		response["byToUser"] = byToUser;//谁吃的牌
		response["chiFlag"] = m_usChiFlags;//吃 头  中  尾
		//吃的牌
		for(int j = 0 ; j< 3 ;j++)
		{
			response["cpgCards"].append(tagActInfo.CGPNode.val[j]);
		}

		//牌栏的位置
		response["nIdx"] = tagActInfo.CGPNode.nIdx;
		OutputTraceStr("wgd:ExecuteChiAct response[nIdx] = %d",tagActInfo.CGPNode.nIdx);
		//手里剩下的牌
		for (int j= 0 ; j< tagActInfo.iHandNums ;j++)
		{
			if (tagActInfo.byHands[j] > 0 )
			{
				response["HandCards"].append(tagActInfo.byHands[j]);
			}
		}

		//发送给玩家数据
		SendGameData(i,response,ASS_ACT_INFO);
	}

	//清除各个玩家的信息
	for (int i = 0; i < m_iPlayerCount; i ++)
	{
		m_UserData[i].m_tagActInfo.Clear();
	}

	//下次得令的玩家设置成碰牌的玩家
	m_byTokenNextUser = byToUser;

	//动作结束后，启动为下一个玩家发令牌的流程。得令牌玩家置成碰牌的玩家,不需要抓牌。
	m_tagParam.bCanFetch = false;
	m_tagParam.byNextUser = byToUser;
	SetTimer(TIME_GIVE_NEXT_TOKEN,500);

	return true;
}


//执行某玩家“吃”动作且修改数据
bool CMJDesk::ActChiTo(BYTE byFrom, BYTE byTo, TMjCGPNode* pCGPNode)
{
	//得到玩家byFrom的数据
	UserData *pUserF = &m_UserData[byFrom];

	//得到玩家byTo的数据
	UserData *pUserT = &m_UserData[byTo];

	//得到玩家byFrom最后打出的那张牌,并且从牌表中取走
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

	//得到杠牌的位置
	pCGPNode->nIdx = pUserT->m_iCGPNums;

	pUserT->m_lstCGP[pUserT->m_iCGPNums++] = *pCGPNode;
	CMJLogic::RemoveCards(delVal, 2, pUserT->m_byHandCards, pUserT->m_iHandNums);
	pUserT->m_iHandNums -= 2;
	return 0;
}

///执行“胡”动作
///@param byFromUser放炮的玩家；byToUsers[] 能胡的玩家Deskstation；iUserNums 能胡的玩家数量
bool CMJDesk::ExecuteHuAct(BYTE byFromUser,BYTE byToUsers[],int iUserNums)
{
	//获取玩家byFromUser的数据
	UserData* pUserF = &m_UserData[byFromUser];

	//修改游戏数据
	ActHuTo(byFromUser,byToUsers,iUserNums);

	//向各玩家广播动作消息///////////////////////////////////////////////////////////////////////////

	TActInfo tagActInfo;

	//动作类型
	tagActInfo.usActFlag = E_MAHJONG_ACTION::ACT_HU;

	//动作触发者
	tagActInfo.byFromUser = byFromUser;

	//动作发出者
	::memcpy(tagActInfo.byUsers,byToUsers,sizeof(byToUsers));
	tagActInfo.iUserNums = iUserNums;

	//特殊胡牌类型
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

	// 给每个玩家发送胡牌玩家的手牌
	Json::Value response ;
	response["usActFlag"] = tagActInfo.usActFlag;
	response["byFromUser"] = byFromUser;//谁放炮
	response["OutCardNum"] = m_UserData[byFromUser].m_iOutedNums;//更新出牌玩家的出牌数量

	for (int i= 0; i< iUserNums ; i++)
	{
		UserData* pUserT = &m_UserData[byToUsers[i]];
		response["byToUsers"].append(byToUsers[i]);//胡的玩家

		for (int j= 0 ; j< pUserT->m_iHandNums ; j++)
		{
			response["HandCards"][i].append(pUserT->m_byHandCards[j]);
		}
	}
	//response["byToUsers"] = byToUsers[0];//胡的玩家

	//胡牌玩家数据
	UserData* pUserT = &m_UserData[byToUsers[0]];
	//response["iHandNums"] = pUserT->m_iHandNums;//胡牌玩家的手牌数量
	bool isZimo = (pUserT->m_nResult == MjResult::Res_ZiMo);
	response["isZiMo"] = isZimo;//胡牌玩家是否为自摸

	for (int i = 0; i < m_iPlayerCount; ++i)
	{
		UserData* pUser = &m_UserData[i];

		//拷贝手牌数据
		::memcpy(tagActInfo.byHands,pUser->m_byHandCards,sizeof(int)*pUser->m_iHandNums);

		//获得手牌数量
		tagActInfo.iHandNums = pUser->m_iHandNums;

		//为玩家发送数据
		SendGameData(i,response,ASS_ACT_INFO);
	}

	response["iHandNums"].clear();
	response["HandCards"].clear();
	this->InsertGameVideo( Video_Event::Video_Event_action , response );

	/*****20181204抢杠胡如果还有游金玩家，还要响应游金玩家*****/
	//发送给非游金玩家
	////pUserT->m_bNormalQiangGang = false;  //-->此时还不能清除
	//pUserT->m_bYouJinQiangGang = false;
	//pUserT->m_tagActInfo.usFlags = 0;
	// 2家非游金抢杠1游金抢杠出现的情况
	for (int i = 0; i < iUserNums; ++i)
	{
		UserData* pUser = &m_UserData[byToUsers[i]];
		pUser->m_bYouJinQiangGang = false;
		pUser->m_tagActInfo.usFlags = 0;
	}

	for (int i = 0; i < m_iPlayerCount; ++i)
	{
		TActNotify tagActNotify;
		//获得玩家i的数据
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

			//流程保护
			m_byTokenUser = byFromUser;

			// 返回, 抢杠不需要抓鸟
			return true;
		}
	}

	//各玩家的动作信息可清除///////////////////////////////
	for (int i = 0; i < m_iPlayerCount; ++i)
	{
		m_UserData[i].m_tagActInfo.Clear();
	}

	//动作结束后，启动为下一个玩家发令牌的流程
	m_RoundFiniParm.bHuang = false;
	//dong change
	//胡牌玩家抓鸟

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
		// 20181212 要求时间改短一点
		SetTimer(TIME_ROUND_FINISH,500);
	}

	return true;
}

///描述：某玩家执行“胡”动作的数据修改
///@param byFrom放炮的玩家；byToUsers[] 能胡的玩家Deskstation；iUserNums 能胡的玩家数量
///  当iUserNums > 1  表明多响
///  当iUserNums == 1 且 byFromUser = byToUsers[0] 时，表明自摸
bool CMJDesk::ActHuTo(BYTE byFrom,BYTE byToUsers[],int iUserNums)
{
	//得到玩家byFrom数据
	UserData* pUserF = &m_UserData[byFrom];

	//得到玩家byTo数据
	UserData* pUserT = NULL;

	if (iUserNums > 1) //多响///////////////////////////////////////////////////////////////////////////////////////////////////////
	{
		//一炮响流程
		pUserF->m_nResult = MjResult::Res_FangPao;

		int byOutCard = 0;

		pUserT = &m_UserData[byToUsers[0]];

		if (emHuSpecialWay::hsw_QiangGang == pUserT->m_tagActInfo.nHuSpecWay[1]) //抢杠
		{
			//得到被抢杠的杠数据
			TMjCGPNode* pCGPNode = &pUserF->m_lstCGP[pUserT->m_tagActInfo.nHuGangIdx];

			//得到玩家byFrom最后抢到的杠牌，从对应的杠牌控件中删除该牌，使杠变成碰
			byOutCard = pCGPNode->val[0];
			pCGPNode->nType = emType::Peng;
			pCGPNode->val[3] = 0;

			// 20181207 如果是能杠却碰并打出了牌，则杠分本身没加进来，则抢杠不必退, 反之则退
			if (pUserF->m_arrValidGangPoint[byOutCard] == 0)
			{
				//如果被抢杠要退回之前杠的那个人的赢得钱
				for (int i = 0 ; i< m_iPlayerCount ; i++)
				{
					if (i != byFrom)
					{
						m_UserData[byFrom].iGangPoint -= 1;
						m_UserData[i].iGangPoint += 1;
					}
				}
				// 杠分已经退了, 避免重复退杠分
				pUserF->m_arrValidGangPoint[byOutCard] = 2;
			}

			m_tagRoundFinish.iBuGangTimes[byFrom]--;

		}
		else //普通放炮
		{
			//得到玩家byFrom最后打出的牌，并从出牌列表中扣除
			byOutCard = pUserF->m_byOutedCards[--pUserF->m_iOutedNums];
		}

		//剩下的玩家们byToUsers，手牌要加上byOutCard，即接炮
		for (int i = 0; i < iUserNums; ++i)
		{
			pUserT = &m_UserData[byToUsers[i]];

			pUserT->m_nResult = MjResult::Res_JiePao;
			pUserT->m_byHandCards[pUserT->m_iHandNums++] = byOutCard;

			//记录胡的牌
			pUserT->m_byHuCard = byOutCard;
		}

	}
	else 
	{
		pUserT = &m_UserData[byToUsers[0]];

		int byOutCard = 0;

		if (byFrom != byToUsers[0]) //单响////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		{
			pUserF->m_nResult = MjResult::Res_FangPao;
			pUserT->m_nResult = MjResult::Res_JiePao;

			if (emHuSpecialWay::hsw_QiangGang == pUserT->m_tagActInfo.nHuSpecWay[1]) //抢杠
			{
				// 20181207抢杠胡不能存在其他牌型
				memset(pUserT->m_tagActInfo.nHuType, 0, sizeof(pUserT->m_tagActInfo.nHuType));
/*				pUserT->m_tagActInfo.nHuType[0] = ht_QiangGangHu;
				pUserT->m_tagActInfo.iHuTypeNums = 1;*/	

				//得到被抢杠的杠数据
				TMjCGPNode* pCGPNode = &pUserF->m_lstCGP[pUserT->m_tagActInfo.nHuGangIdx];

				//得到玩家byFrom最后抢到的杠牌，从对应的杠牌控件中删除该牌，使杠变成碰
				byOutCard = pCGPNode->val[0];
				pCGPNode->nType = emType::Peng;
				pCGPNode->val[3] = 0;

				// 20181207 如果是能杠却碰并打出了牌，则杠分本身没加进来，则抢杠不必退, 反之则退
				// 非游金玩家和游金玩家形式上构成的是一炮多响，响应的却是有顺序的单响，注意重复退杠分
				if (pUserF->m_arrValidGangPoint[byOutCard] == 0)
				{
				//如果被抢杠要退回之前杠的那个人的赢得钱
					for (int i = 0 ; i< m_iPlayerCount ; i++)
					{
						if (i != byFrom)
						{
							m_UserData[byFrom].iGangPoint -= 1;
							m_UserData[i].iGangPoint += 1;
						}
					}

					// 杠分已经退了, 避免重复退杠分
					pUserF->m_arrValidGangPoint[byOutCard] = 2;
				}

				m_tagRoundFinish.iBuGangTimes[byFrom]--;
			}
			else //普通放炮
			{

				//得到玩家byFrom最后打出的牌，并从出牌列表中扣除
				byOutCard = pUserF->m_byOutedCards[--pUserF->m_iOutedNums];
			}

			//把byOutCard加到玩家pUserT手牌上表示接炮
			pUserT->m_byHandCards[pUserT->m_iHandNums++] = byOutCard;

			//记录胡的牌
			pUserT->m_byHuCard = byOutCard;

		}
		else //自摸////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		{
			pUserT->m_nResult = MjResult::Res_ZiMo;

			//记录胡的牌,也就是最后抓的一张牌
			pUserT->m_byHuCard = pUserT->m_byHandCards[pUserT->m_iHandNums - 1];
		}
	}

	//有些在CUserData::m_tagActInfo中的数据转移到CUserData中,以便结算使用。
	//注意CUserData::m_tagActInfo数据将在ActHuTo后有可被马上清除,所以转移到CUserData中是极有必要的

	for (int i = 0; i < m_iPlayerCount; ++i)
	{
		UserData* pUser = &m_UserData[i];

		//记录胡牌的牌型表及其长度
		::memcpy(pUser->m_nHuTypes,pUser->m_tagActInfo.nHuType,sizeof(pUser->m_nHuTypes));
		pUser->m_iHuTypeNums = pUser->m_tagActInfo.iHuTypeNums;

		//记录玩家特殊胡牌方式
		pUser->m_nHuSpecWay[0] = pUser->m_tagActInfo.nHuSpecWay[0];
		pUser->m_nHuSpecWay[1] = pUser->m_tagActInfo.nHuSpecWay[1];
	}

	return true;
}
//玩家抓鸟
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

	//dong add 抓鸟
	Json::Value response ;
	for (int i= 0 ; i< m_iNiaoNums ; i++)
	{
		//为胡牌的玩家抓鸟牌

		if (GetWallLeft() > 0)
		{
			TMjFetch tagFetch = FetchCardTo(255);
			response["niaoCards"].append(tagFetch.val);
			//保存鸟牌
			m_niaoCardValues[i] = tagFetch.val;
			//抓到的鸟牌从手牌中移除
			//OutCardTo(zhuaniaoUser,tagFetch.val);
		}
		else
		{
			response["niaoCards"].append(-1);
		}
	}

	m_UserData[zhuaniaoUser].m_iHandNums = iHandCardNumsTemp;
	::memcpy(m_UserData[zhuaniaoUser].m_byHandCards,iHandCardsTemp,sizeof(int) * iHandCardNumsTemp);

	//胡牌的玩家 TODO

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
						// 一马全中
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

	//牌墙剩余牌数量
	response["WallLeft"] = GetWallLeft();

	//为每个玩家广播鸟牌
	for (int i= 0 ; i< m_iPlayerCount ; i++)
	{
		SendGameData(i,response,ASS_NIAOCARDS_INFO);
	}
	this->InsertGameVideo( Video_Event::Video_Event_niao , response);
	//抓完鸟后回合结束
	m_RoundFiniParm.bHuang = false;
	SetTimer(TIME_ROUND_FINISH,2000);

	DebugPrintf(m_pGameManager->GetRoomID(),m_nDeskIndex,"OnEventFetchNiao====>end");
	return true;
}


//一回合结束的事件处理
bool CMJDesk::OnEventRoundFinish()
{
	DebugPrintf(m_pGameManager->GetRoomID(),m_nDeskIndex,"OnEventRoundFinish====>begin");
	TEvRoundFiniParm* pEvParm = &m_RoundFiniParm;

	//返回数据
	Json::Value response;

	//广播一回合游戏结束通知，标志着游戏进入GS_WAIT_NEXT_ROUND状态（一回合结束状态）
	m_nGameStation = GS_WAIT_NEXT_ROUND;

	if (!m_pGameManager->IsPrivateRoom())
	{
		//金币场
		m_nGameStation = GS_WAIT_ARGEE;
	}

	//启动回合结算流程,非荒庄时要等待胡牌动画播放，需要延时长一点
	if (!pEvParm->bHuang)
	{
		// 20181213客户不需要时间延长
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

//算分事件处理
bool CMJDesk::OnEventResult()
{
	DebugPrintf(m_pGameManager->GetRoomID(),m_nDeskIndex,"OnEventResult====>begin");
	//killAllTimer();
	TResult tagResult;

	UserData* pUser = NULL;

	//初始化结算包
	for (int i = 0; i < m_iPlayerCount; ++i)
	{
		pUser = &m_UserData[i];

		//获取玩家的输赢标志,并记录数量
		if (pUser->m_nResult == MjResult::Res_ZiMo || pUser->m_nResult == MjResult::Res_JiePao)
		{
			//获取玩家i的特殊胡方式
			::memcpy(tagResult.nHuSpecWay[i],pUser->m_nHuSpecWay,sizeof(pUser->m_nHuSpecWay));

			//获取玩家i的胡牌牌型及长度
			::memcpy(tagResult.nHuTypes[i],pUser->m_nHuTypes,sizeof(pUser->m_nHuTypes));
			tagResult.iHuTypeNums[i] = pUser->m_iHuTypeNums;

			tagResult.bHu[i] = true;
			tagResult.iHuNums++;
			tagResult.byHuUser = i;
		}

		//获取放炮的玩家
		if (pUser->m_nResult == MjResult::Res_FangPao)
		{
			tagResult.byFangPaoUser = i;
		}
	}

	//调用游戏结算算法
	CalcRoundFen(tagResult);

	if (GetIsHe())
	{
		for (int i=0;i<m_iPlayerCount;++i)
		{
			m_tagRoundFinish.iLiuJuTimes[i]++;
		}
	}

	//更新玩家金币
	for (int i = 0; i< m_iPlayerCount ; i++)
	{
		///更新玩家结算金币变动数量
		m_ResultUserData[i]._nPoint = tagResult.iTotalPoint[i];
		//更新玩家当前金币
		m_ResultUserData[i]._iUserMoney += tagResult.iTotalPoint[i];
	}

	//写入数据库				
	bool bNetCutUser[PLAY_COUNT];
	memset(bNetCutUser,0,sizeof(bNetCutUser));

	//TODO更新玩家金币
	ResultInfo_t result;
	result._nCount = m_iPlayerCount;//桌子上的人数
	result._nDeskIndex = m_nDeskIndex;//桌子号


	//更新数据库玩家金币数量
	int changeResult = m_pGameManager->ChangeUserPoint(result,m_ResultUserData);
	if (changeResult != 0)
	{
		OutputTraceStr("wgd: update user score failed   changeResult = %d",changeResult);
	}
	//上传战绩
	if (!(m_bAllAgreeDismissRoom && m_nGameStation == GS_WAIT_NEXT_ROUND))
	{
		RecordMJUsersScore(tagResult);
	}

	Json::Value response;

	if (!GetIsHe()&& m_bHaveFetchBird)
	{
		//下发鸟牌

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

	//保存成绩单
	::memcpy(&m_tagResult,&tagResult,sizeof(TResult));
	//下发每个玩家是否胡牌和本次得分情况
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
		

		//胡哪张牌
		response["HuCards"].append(m_UserData[i].m_byHuCard);

		response["HuPoint"].append( tagResult.iHuPoint[i]);
		response["GangPoint"].append(tagResult.iGangPoint[i]);
		response["NiaoPoint"].append(tagResult.iNiaoPoint[i]);

		//如果可以飘 下发飘的结算点数
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



		//手牌
		response["HandNums"].append(m_UserData[i].m_iHandNums);
		Json::Value handcards;
		for (int j = 0;j< m_UserData[i].m_iHandNums ; j++)
		{
			handcards.append(m_UserData[i].m_byHandCards[j]);
		}
		response["HandCards"].append(handcards);

		//拦牌记录
		response["CGPNums"].append(m_UserData[i].m_iCGPNums);
		Json::Value CPGRecords;
		if (m_UserData[i].m_iCGPNums > 0)
		{
			for(int j = 0 ; j< m_UserData[i].m_iCGPNums ; j++)
			{
				Json::Value cpgRecord;
				cpgRecord["usFlags"] = m_UserData[i].m_lstCGP[j].nType;//吃碰杠类型
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
		//下发
		if(m_bPiao)
		{
			//如果加飘要下发加飘的数据
			response["PiaoNum"].append(m_UserData[i].m_iPiaoNum);
		}
	}
	Json::Value paijuInfo;
	//牌局信息数据
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

	//是否荒庄
	bool isHuangZhuang = GetIsHe();
	response["IsHuangZhuang"] = isHuangZhuang;
	//不荒庄下发胡牌玩家的牌型
	if (!isHuangZhuang )
	{
		response["FangPaoUser"] = tagResult.byFangPaoUser;
		response["HuUser"] = tagResult.byHuUser;
	}
	//牌局信息需要携带结算数据 用于显示最后一局的结算界面
	paijuInfo["ResultInfo"] = response;
	//为每个玩家发送结算数据和牌局信息
	for (int i = 0; i < m_iPlayerCount; ++i)
	{

		if (!m_pGameManager->IsPrivateRoom() || m_tagRoundFinish.iRoundTimes < m_tagRoundFinish.iAllRoundTimes )
		{
			//发送结算数据
			SendGameData(i,response,ASS_RESULT);
		}
		else
		{
			//发送牌局信息
			SendGameData(i,paijuInfo,ASS_PAIJU_INFO);
			m_nGameStation = GS_PAIJU_FINISH;
		}
	}

	this->InsertGameVideo( Video_Event::Video_Event_resoult , paijuInfo);
	if (m_tagRoundFinish.iRoundTimes >= m_tagRoundFinish.iAllRoundTimes)
	{

		//发送牌局结束消息
		Json::Value resp;
		for (int i = 0; i < m_iPlayerCount; ++i)
		{
			//发送结算数据
			SendGameData(i,resp,ASS_GAME_OVER);
		}
		//重新计算牌局信息
		m_tagRoundFinish.Clear();
	}
	//清理各玩家的同意状态
	for (int i = 0; i < m_iPlayerCount; ++i)
	{
		SetUnknowStateTo(i);
	}
	//结束游戏
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
		if (!m_pGameManager->IsPrivateRoom())//金币场要自动出牌
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
				//先检测最高优先级的动作
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
					//杠的选择
					actReq["nSelIndex"] = 0;
				}
				else if (pUser->m_tagActInfo.usFlags & E_MAHJONG_ACTION::ACT_PENG)
				{
					actReq["ActFlag"] =  E_MAHJONG_ACTION::ACT_PENG;
				}
				else if (pUser->m_tagActInfo.usFlags & E_MAHJONG_ACTION::ACT_CHI)
				{
					actReq["ActFlag"] =  E_MAHJONG_ACTION::ACT_CHI;
					//先检测吃头 然后吃中 最后吃尾
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
					//如果玩家有执行听动作  还需要发送出牌消息
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

///名称：CalcRoundFen
///描述：计算分数
///@param result结算包，既作输入，又作输出，result由外部初始化玩家ID，
//内部将计算玩家输赢金币等数据、输出胡牌数据等，保存在result中。
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
			//基本积分计算/////////////////////////////////////////////////////////////////////////////////////////////////////
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
					//::sprintf_s(result.szHuRepo[i],"抢杠 3分  ",result.szHuRepo[i]);
					//result.iHuPoint[i] = 3;
				}

			}
		}

		if (MjResult::Res_ZiMo == pUser->m_nResult)
		{
			////bZiMo = true;
			////byZiMoUser = i;
			////自摸次数更新
			//m_tagRoundFinish.iZimoTimes[i] ++;

			//nHuUserArray[nHuUserNum++]=i;

			// 200181130自摸和游金自摸分开算
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
				// 上面已加入了
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
			//接炮次数更新
			m_tagRoundFinish.iJiePaoTimes[i] ++;
			nHuUserArray[nHuUserNum++]=i;
		}
	}

	//取得变化的分值
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
			//基本积分计算/////////////////////////////////////////////////////////////////////////////////////////////////////
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

	//荒庄处理
	if (GetIsHe())
	{
		for (int i = 0; i < m_iPlayerCount; ++i)
		{
			result.iHuPoint[i] = 0;
			result.iNiaoPoint[i] = 0;
			result.iGangPoint[i]=0;
			//如果加飘还要算飘的分
			if (m_bPiao)
			{
				result.iPiaoPoint[i] = 0;
			}
			//如果庄闲算分要算庄闲分
			if (m_bNTResult)
			{
				result.iNTPoint[i] = 0;
			}
		}


	}
	else
	{
		if (eSuanFenType == sf_DianGangKai)
		{//如果是杠上
			ModifyGangPointWhileDianGangKai(byHuUser);
		}
		//计算杠分
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

		//根据配置计算抓鸟分
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

	//计算总分
	for (int i = 0; i < m_iPlayerCount; ++i)
	{

		result.iTotalPoint[i] = result.iHuPoint[i] + result.iGangPoint[i] + result.iNiaoPoint[i];

		DebugPrintf(m_pGameManager->GetRoomID(),m_nDeskIndex,"玩家%d,胡分=%d,杠分=%d,马分=%d,总分=%d",i,result.iHuPoint[i],result.iGangPoint[i],result.iNiaoPoint[i],result.iTotalPoint[i]);

		//更新牌局总分
		m_tagRoundFinish.iTotalScore[i] += result.iTotalPoint[i];
	}

	return;
}

void CMJDesk::RecordMJUsersScore(TResult& result)
{
	Json::Value root;
	root["roundID"] = m_tagRoundFinish.iRoundTimes;			//对战局编号
	//战绩记录集 

	for (int i = 0 ; i< m_iPlayerCount ; i++)
	{
		Json::Value detail;
		detail["bIsBanker"] = (i == m_byNTUser);													//是否是庄家
		Json::Value user;			
		user["userID"] = m_ResultUserData[i]._dwUserID;									//用户ID
		user["beforePoint"] = m_ResultUserData[i]._iUserMoney;							//对战前的分数
		user["changedPoint"] = result.iTotalPoint[i];														//本次对战的分数
		user["detail"] = detail;																			//玩家在游戏的信息，内容由游戏开发者自行设计
		root["records"][i] = user;
	}


	Json::FastWriter write;
	std::string str = write.write(root);
	int recoredResult = m_pGameManager->RecordUsersScore(m_nDeskIndex,m_gameBeginTime,str.c_str());
	if(recoredResult !=  0)
	{
		//如果记录失败，请找出原因，尽量保证记录成功;
		OutputTraceStr("wgd: record user score failed %d",recoredResult);
	}

}

///描述：设置玩家为未知是否同意底注状态
///@param bDeskStation玩家位置
void CMJDesk::SetUnknowStateTo(BYTE bDeskStation)
{
	m_UserData[bDeskStation].m_nState = UserData::UNKNOWN;
	return;
}
///描述：设置玩家为同意底注状态
///@param bDeskStation玩家位置
void CMJDesk::SetAgreeStateTo(BYTE bDeskStation)
{
	m_UserData[bDeskStation].m_nState = UserData::AGREEGAME;
	return;
}

//客户端配置下发手牌
int CMJDesk::OnTestChangeFetchHandcards(BYTE bDeskStation,void* pData,UINT uSize,bool bWatchUser)
{
	m_UserData[bDeskStation].m_bFetchHandCardCheat = true;
	Json::Reader reader(Json::Features::strictMode());

	//收到玩家发来的数据
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


	//更新玩家的手牌数据
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

///名称：OnTestChangeFetch
///描述：指定抓牌值
///@param bDeskStation 位置, pNetHead 消息头,pData 消息数据,uSize 消息包长度,uSocketID id,bWatchUser 是否旁观
///@return
int CMJDesk::OnTestChangeFetch(BYTE bDeskStation,void* pData,UINT uSize,bool bWatchUser)
{
	Json::Reader reader(Json::Features::strictMode());

	//收到玩家发来的数据
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


///名称：OnUserAutoRequest
///描述：处理玩家托管消息
///@param bDeskStation 位置, pNetHead 消息头,pData 消息数据,uSize 消息包长度,uSocketID id,bWatchUser 是否旁观
///@return
int CMJDesk::OnUserAutoRequest(BYTE bDeskStation,void* pData,UINT uSize,bool bWatchUser)
{
	UserData* pUser = &m_UserData[bDeskStation];
	Json::Reader reader(Json::Features::strictMode());

	//收到玩家发来的数据
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

	//防错//////////////////////////////////////////////////////////////////

	if (bWatchUser)
	{
		return 0;
	}
	//处理//////////////////////////////////////////////////////////////////

	pUser->m_bAuto = _root["IsAuto"].asBool();

	//广播托管消息/////////////////////////////////////////////////////////
	Json::Value AutoInfo;
	AutoInfo["chair"] = bDeskStation;
	AutoInfo["IsAuto"] = pUser->m_bAuto;
	if (pUser->m_bAuto)
	{
		//如果玩家发送托管消息
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
		//发送某个玩家托管状态变更信息
		SendGameData(i,AutoInfo,ASS_AUTO_INFO);
	}
	return 0;
}

///名称：OnTestChangeHands
///描述：篡改手牌
///@param bDeskStation 位置, pNetHead 消息头,pData 消息数据,uSize 消息包长度,uSocketID id,bWatchUser 是否旁观
///@return
int CMJDesk::OnTestChangeHands(BYTE bDeskStation,void* pData,UINT uSize,bool bWatchUser)
{
	Json::Reader reader(Json::Features::strictMode());

	//收到玩家发来的数据
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

	//更新玩家的手牌数据
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


//获得游戏的基本的状态
bool CMJDesk::OnGetGSWaitSet(BYTE bDeskStation,UINT uSocketID,bool bWatchUser)
{
	//具体的数据获取代码写下此
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
	//当前游戏状态
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

//打牌状态请求
bool CMJDesk::OnGetGSPlaying(BYTE bDeskStation,UINT uSocketID,bool bWatchUser)
{
	//具体的数据获取代码写下此
	const UserData* pUser = NULL;

	//获得自己的动作
	pUser = &m_UserData[bDeskStation];
	Json::Value response;
	//返回玩家当前的椅子号
	response["chair"] = bDeskStation;

	response["laizis"]=m_byLaiZi;

	//当前游戏状态
	response["GSID"] = m_nGameStation;
	response["CheaterOpen"] = m_tagDeskCfg.byCheaterOpen;
	for (int i = 0; i<m_iPlayerCount ;i++)
	{
		response["TotalScore"].append(m_tagRoundFinish.iTotalScore[i]);
	}
	//剩余牌的数量
	response["WallLeft"] = GetWallLeft();

	//是否旁观玩家
	response["isWatchUser"] = false;
	//当前庄家
	response["NTUser"] = m_byNTUser;
	//当前剩余牌数量
	response["WallLeftNums"] =GetWallLeft();	
	//每个玩家剩余的手牌数量
	for (int i= 0 ; i< m_iPlayerCount ; i++)
	{
		response["HandNums"].append(m_UserData[i].m_iHandNums) ;
		//每个玩家的手牌
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

	//当前得令玩家
	response["TokenUser"] = m_byTokenUser;

	//每个玩家出的牌

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

	//每个玩家吃碰杠情况

	for (int i = 0 ; i< m_iPlayerCount ; i++)
	{
		response["CGPNums"].append(m_UserData[i].m_iCGPNums);
		Json::Value CPGRecords;
		if (m_UserData[i].m_iCGPNums > 0)
		{
			for(int j = 0 ; j< m_UserData[i].m_iCGPNums ; j++)
			{
				Json::Value cpgRecord;
				cpgRecord["usFlags"] = m_UserData[i].m_lstCGP[j].nType;//吃碰杠类型
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

	//自己当前是否有动作
	Json::Value actResp ;
	actResp["usFlags"] = pUser->m_tagActInfo.usFlags;

	if (pUser->m_tagActInfo.usFlags & E_MAHJONG_ACTION::ACT_CHI)
	{
		//发送可吃牌信息
		actResp["isCanC"] = true;//是否可以吃牌
		actResp["iChiNums"] = pUser->m_tagActInfo.iChiNums;//吃牌选择数量
		actResp["usChiFlags"] = pUser->m_tagActInfo.usChiFlags;//吃的标志 吃(头中尾)
	}
	else
	{
		actResp["isCanC"] = false;
	}

	if (pUser->m_tagActInfo.usFlags & E_MAHJONG_ACTION::ACT_PENG)
	{
		//发送可碰牌信息
		actResp["isCanP"] = true;

	}
	else
	{
		actResp["isCanP"] = false;
	}

	if (pUser->m_tagActInfo.usFlags & E_MAHJONG_ACTION::ACT_GANG)
	{
		//发送可杠牌信息
		actResp["isCanG"] = true;
		actResp["iGangNums"] = pUser->m_tagActInfo.iGangNums;//可以杠选择数量
		for (int i= 0 ; i< MJ_MAX_CGP ; i++)
		{
			actResp["byGangSels"].append(pUser->m_tagActInfo.byGangSels[i]);//可以杠的选择
		}for (int i= 0 ; i< MJ_MAX_CGP ; i++)
		{
			actResp["nBuGangInPeng"].append(pUser->m_tagActInfo.nBuGangInPeng[i]);//补杠在建立在那一个碰牌上
		}

	}
	else
	{
		actResp["isCanG"] = false;
	}

	if (pUser->m_tagActInfo.usFlags & E_MAHJONG_ACTION::ACT_HU)
	{
		//发送可胡牌信息
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
	///漂的类型
	Json::Value PiaoResp;
	for (int i = 0; i < m_iPlayerCount;i++)
	{
		Json::Value Resp;
		Resp["chair"] = i;
		Resp["piaoType"] = m_UserData[i].m_iPiaoNum;
		PiaoResp.append(Resp);
	}
	response["PiaoResp"] = PiaoResp;

	//是否托管
	response["IsAuto"] = pUser->m_bAuto;

	return SendGameData(bDeskStation,response,ASS_GM_GAME_STATION);

}

//加漂的状态
bool CMJDesk::OnGetGSWaitPiao(BYTE bDeskStation,UINT uSocketID,bool bWatchUser)
{
	Json::Value response;
	//当前状态ID
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
		// 20181211 修改一下,这里不是所有分归0，而是最后点杠被杠上花的杠归0
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
		//点杠后不牌又杠的杠上花，需由点杠玩家承包杠分
		int fangGangUser = pUser->m_lstCGP[pUser->m_dianGangGangKaiIndex].from;
		// 20181203最前面的点杠不算
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
		//// 从前面抓牌
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

		// 从后面面抓牌
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
