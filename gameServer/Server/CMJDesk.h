#pragma once
#include <map>
#include "comstruct.h"
#include "IGameDesk.h"
#include "MjCommonDef.h"
#include "UpgradeMessage.h"
#include "CGameDesk.h"

//***************************************************************************************
//**********************************用户数据管理*****************************************
//***************************************************************************************
////记录玩家在桌子上的游戏信息
struct UserData
{
	/*-玩家游戏数据定义-*/
	//玩家状态，主要用于判断哪些玩家同意/不同意游戏 
	enum emState
	{
		UNKNOWN = 0,		 //未知状态
		AGREEGAME = 3,       //玩家同意游戏
	};
	emState m_nState;

	////玩家方向，东南西北分别为0,1,2,3。255为未定风向。
	//BYTE m_byDirect;

	//玩家是否完成选庄动画
	bool m_bMakeNTOK;

	//玩家是否完成打骰子定方位动画
	bool m_bAssignedDir;


	//玩家是否完成打骰子定起牌方法动画
	bool m_bFetchMethodOK;
	//用户的手牌
	int m_byHandCards[HAND_MAX_NUM_MJ];

	//用户手牌数量
	int m_iHandNums;

	//抓牌次数
	int m_iFetchTimes;

	//玩家已出牌的次数
	int m_iOutedTimes;

	//已出牌表
	int m_byOutedCards[MJ_MAX_OUTED];

	//已出的牌数
	int m_iOutedNums;
	//玩家的动作信息结构
	TMjActInfo m_tagActInfo;
	//拦牌记录
	TMjCGPNode m_lstCGP[MJ_MAX_CGP];
	
	//拦牌数量
	int m_iCGPNums;

	//玩家是否已听牌
	bool m_bIsTing;

	//玩家胡的牌
	int m_byHuCard;

	//游戏结果，指一局麻将的结果，如放炮、自摸等(使用MjBase::MjResult空间定义的常量)
	int m_nResult;

	//玩家胡的牌型表
    emHuType m_nHuTypes[MJ_MAX_HUTYPE];

	//玩家胡的牌型表长度
	int m_iHuTypeNums;

	//玩家的特殊牌型(如杠上开花,抢杠胡,杠冲等)
	emHuSpecialWay m_nHuSpecWay[2];	

	//上局是否是抢杠胡
	bool m_bIsLastRoundQiangGang;
	bool m_bLastDianGangKai;

	///漂
	bool m_bPiao;
	int  m_nMingGangNums;             //明杠数量(包含明杠、补杠、红中、发财)
	int	 m_nZhongGangNums;			//红中杠数量
	int	 m_nFagangNums;				//发财杠数量
	int  m_nAnGangNums;               //暗杠数量
	int  m_nSpecialGangNums;          //特殊杠数量(癞子杠)
	//玩家大胡个数
	int  m_nDaHuCounts;

	//每个玩家每局的杠分
	int iGangPoint;

	//玩家是否漏胡
	bool m_bLouHu;

	//玩家漏胡哪些牌
	int m_LouHuCard[MJ_MAX_LOUHU];
	//玩家漏胡番数
	int m_LouFan;

	//能杠的牌选择了碰的牌
	std::vector<int> m_vCanGangSelPengCard;

	//20181203能杠的牌选择了碰的牌是否补杠加分-出了牌后就不加分了
	//std::map<int, bool> m_mapValidGangPoint;
	int m_arrValidGangPoint[40]; //下标-牌值， 下标值:0-该牌不是能杠选碰牌, 1-杠分有效, 2-杠分无效

	//玩家飘的点数
	int m_iPiaoNum;

	//点杠杠开的起始杠索引（为实现点杠后补牌后又杠杠上开花需要承包杠分）
	int m_dianGangGangKaiIndex;
/*********************************************************************************/
/*-玩家非游戏数据-*/
	//是否托管状态
	bool m_bAuto;
	//玩家是否已断线
	bool m_bNetCut;

/*-测试数据-*/
	//如果该值有效(不等于0),则抓牌取强制使用此值,\
	该值一次性使用
	int m_byChFetch;

	//如果该值为true 说明客户端发起配发牌请求
	bool m_bFetchHandCardCheat;
	int m_byHandCardsCheat[HAND_MAX_NUM_MJ];

	//漏碰表
	bool m_bLouPeng[40];

	// 抢杠玩家信息
		bool m_bYouJinQiangGang;
		bool m_bNormalQiangGang;

	TAG_INIT(UserData)
	{
		::memset(this,0,sizeof(UserData));
	}


	//半初始化
	void HalfInit()
	{
		::memset(m_bLouPeng,0,sizeof(m_bLouPeng));

		//m_vCanGangSelPengCard.clear();
		//if (!m_mapValidGangPoint.empty())
		//{
		//	m_mapValidGangPoint.clear();
		//}
		::memset(m_arrValidGangPoint,0,sizeof(m_arrValidGangPoint));

		//自己的状态
		m_nState = UNKNOWN;

		//是否托管状态
		m_bAuto = false;

		//初始化飘的分
		m_iPiaoNum = -1;

		m_bPiao = false;

		//玩家是否完成打骰子定起牌方法动画
		m_bFetchMethodOK = false;




		//用户的手牌
		::memset(m_byHandCards,0,sizeof(m_byHandCards));

		//用户手牌数量
		m_iHandNums = 0;

		//抓牌次数
		m_iFetchTimes = 0;

		//已出牌表
		::memset(m_byOutedCards,0,sizeof(m_byOutedCards));

		//已出的牌数
		m_iOutedNums = 0;

		//玩家已出牌的次数
		m_iOutedTimes = 0;

		//玩家的动作信息结构
		m_tagActInfo.Clear();

		//拦牌记录
		for (int i = 0; i < MJ_MAX_CGP; ++i)
		{
			m_lstCGP[i].Clear();
		}

		//玩家是否已听牌
		m_bIsTing = false;

		//拦牌数量
		m_iCGPNums = 0;

		//玩家胡的牌
		m_byHuCard = 0;

		//玩家胡的牌型表
		::memset(m_nHuTypes,emHuType::ht_Unknown,sizeof(m_nHuTypes));

		//玩家胡的牌型表长度
		m_iHuTypeNums = 0;

		//玩家的特殊胡牌方式(如天地胡,杠上开花,抢杠胡,杠冲等)
		m_nHuSpecWay[0] = emHuSpecialWay::hsw_Unknown;
		m_nHuSpecWay[1] = emHuSpecialWay::hsw_Unknown;
		m_bLouHu = false;
		::memset(m_LouHuCard,0,sizeof(m_LouHuCard));

		//初始化杠		
		m_nMingGangNums = 0;             //明杠数量(包含明杠、补杠、红中、发财)
		m_nAnGangNums = 0;               //暗杠数量
		m_nSpecialGangNums = 0;          //特殊杠数量(癞子杠)
		m_nZhongGangNums = 0;			//红中杠数量
		m_nFagangNums = 0;				//发财杠数量

		m_nDaHuCounts = 0;               //玩家大胡个数
		m_LouFan = 0;

		m_dianGangGangKaiIndex = -1;

		m_bYouJinQiangGang = false;
		m_bNormalQiangGang = false;
	}


	//描述：初始化
	void Init()
	{
		HalfInit();

		m_bIsLastRoundQiangGang=false;
		m_bLastDianGangKai = false;

		//玩家是否完成打骰子定方位动画
		m_bAssignedDir = false;

		//玩家是否完成选庄动画
		m_bMakeNTOK = false;

		//游戏结果，指一局麻将的结果
		m_nResult = MjResult::Res_Unknown;

		//玩家是否已断线
		m_bNetCut = false;

		//杠分
		iGangPoint = 0;

		m_LouFan = 0;
	}

	//获得漏胡的牌值（适用于漏胡要求过那张不能胡 如 过了5w  这轮5w不能胡但是其他听口还是可以胡）
	void addALouHuCard(int byCard)
	{
		bool isHave = false;
		for (int i = 0 ;i< MJ_MAX_LOUHU ; i++)
		{
			if (m_LouHuCard[i] == byCard)
			{
				isHave = true;
				break;
			}
		}

		if (! isHave)
		{
			for (int i=0 ;i< MJ_MAX_LOUHU ; i++)
			{
				if (m_LouHuCard[i] == 0 )
				{
					m_LouHuCard[i] = byCard;
					break;
				}
			}
		}
	}

	bool checkIsGangKai()
	{
		return m_nHuSpecWay[1]==emHuSpecialWay::hsw_DianGangKai || m_nHuSpecWay[1]==emHuSpecialWay::hsw_BuGangKai;
	}
	bool checkIsBuGangkai()
	{
		return m_nHuSpecWay[1]==emHuSpecialWay::hsw_BuGangKai;
	}

	bool checkIsQiangGang()
	{
		return m_nHuSpecWay[1]==emHuSpecialWay::hsw_QiangGang;

	}


	void addALouPengCard(int nCard)
	{
		m_bLouPeng[nCard]=true;
	}

	void clearLouPeng()
	{
		memset(m_bLouPeng,0,sizeof(m_bLouPeng));
	}

	bool checkIsLouPeng(int nCard)
	{
		return m_bLouPeng[nCard];
	}

	bool IsHuDianGang()
	{
		bool bRet = m_dianGangGangKaiIndex != -1;
		return bRet;
	}
	/****************************************************************************************************/


};





//游戏桌类


class CMJDesk:public CGameDesk
{
public:
	//构造函数
	CMJDesk(); 

	//析构函数
	virtual ~CMJDesk();

	///游戏数据半初始化
	void HalfInit();
	///全初始化
	void Init(const UserInfoForGame_t* pUserInfo);
/*-----------------------------------------------------------------------------------------------------------
    实现IGameDesk接口,一系列平台提供的需要重载来驱动游戏服务的函数，已适用大多数游戏的开发，但每个函数的实现因游戏而异。
  ----------------------------------------------------------------------------------------------------------*/
public:
	void killAllTimer();

	//玩家离桌
	virtual void OnUserLeftDesk(int nDeskStation);

	////初始化桌子
	virtual int InitialDesk(int nDeskNo, int nMaxPeople, IGameMainManager* pIMainManager);

	//处理游戏内部消息
	virtual int HandleNotifyMessage(int nDeskStation, UINT code, BYTE* pBuffer, int nLen);

	//获取游戏状态，并发送到客户端
	virtual int OnGetGameStation(int nDeskStation, UserInfoForGame_t& userInfo);

	//定时器时间到
	virtual int OnGameTimer(int nTimerID);

	//玩家入坐到某位置
	virtual void OnUserSitDesk(int nDeskStation);

	//玩家断线
	virtual void OnUserNetCut(int nDeskStation);

	//房间被申请解散
	virtual void onDismissDesk();

	//获取游戏ID
	virtual unsigned int GetGameNameID();

	//处理玩家同意游戏
	virtual void  OnUserAgreeGame(int bDeskStation);

	//玩家重新连回来
	virtual void OnUserRecome(int bDeskStation);
	////初始游戏
	//virtual bool InitDeskGameStation();

	//游戏开始
	virtual bool GameBegin(BYTE bBeginFlag);

	//游戏结束
	virtual bool GameFinish(BYTE bDeskStation,BYTE bCloseFlag);


	/*-重载纯虚函数-*/

	//重置游戏状态
	virtual bool ReSetGameState(BYTE bLastStation);


	//判断游戏是否已经开始过了
	virtual bool IsHavePlaying();

	//判断桌子是否达到解散条件
	virtual bool DeskCanDismiss();

	// 桌子被设置后被触发的函数
	virtual void OnDeskConfigured(const char* szClientConfig, const char *szSysConfig = NULL);
////游戏逻辑

public:
	//设置定时器
	void SetTimer(int iTimeID, int iTimerInterval);
	//杀死定时器
	void KillTimer(int iTimeID);
	//设定庄家事件
	bool OnEventMakeNT();

	// 20181203 客户需要起牌方法正筛子的动画
	bool OnEventFetchMethod();

	//系统发手牌事件
	bool OnEventFetchHandCards();

	bool OnEventFetchGui();

	//玩家加飘事件
	bool OnEventPiao();
	//玩家抓鸟事件
	bool OnEventFetchNiao();
	//系统为下一个玩家下发令牌,得令牌者抓牌出牌
	bool OnEventGiveNextToken();
	//根据各玩家的m_nResult判断是否为荒庄
	int GetIsHe();
	//获取剩在牌城的牌数量
	int GetWallLeft();
	//从某玩家中取得动作(即能否吃碰杠胡等)
	bool SelectActInfoFrom(BYTE byFromUser,BYTE byToUser,int byCard,TMjActInfo* pActInfo,int byFetctCard,USHORT usIgnoreFlags = 0,bool bIsQiangGangJudge=false);

	//得到选庄动画完成的玩家数量
	int GetMakeNtPlayers();
	//获取指定玩家以外其他玩家有没有动作
	bool IsOtherPlayersHaveAct(BYTE byStation);
	///玩家出牌删除手中的牌
	void OutCardTo(BYTE bDeskStation,int byCard);
	//通过各玩家的动作状态位，得到马上可以执行动作的玩家集
	int SelectActAtOnceUsers(BYTE byUsers[],USHORT& usActFlag);
	//得到玩家同意游戏数量（同意底注）
	int GetAgreePlayers();
/////////////接收客户端消息处理//////////////////////////////////////////
   //处理玩家定庄完成动画
    int OnUserMakeNTOver(BYTE bDeskStation,void* pData,UINT uSize,bool bWatchUser);
    //处理客户端加飘
    int OnUserPiao(BYTE bDeskStation,void* pData,UINT uSize,bool bWatchUser);
    //处理玩家出牌请求
    int OnUserOutCardRequest(BYTE bDeskStation,void* pData,UINT uSize,bool bWatchUser);
    //处理玩家的动作请求
    int OnUserActRequest(BYTE bDeskStation,void* pData,UINT uSize,bool bWatchUser);
	//一回合结束的事件处理
	bool OnEventRoundFinish();
	//算分事件处理
	bool OnEventResult();
	///自动动作
	bool OnEventAutoAct(bool bCan);
	///自动出牌
	bool OnEventAutoOutCard(bool bCan);
////////////////////////////////动作的执行//////////////////////////////////////////////////
   //执行“过”动作
   bool ExecuteGuoAct(BYTE bDeskStation);
   //执行“碰”动作
   bool ExecutePengAct(BYTE byFromUser,BYTE byToUser);
   //执行“杠”动作
   bool ExecuteGangAct(BYTE byFromUser,BYTE byToUser);
   //执行“听”动作
   bool ExecuteTingAct(BYTE byToUser);
   //执行吃牌动作
   bool ExecuteChiAct(BYTE byFromUser, BYTE byToUser);
   //执行“胡”动作
   bool ExecuteHuAct(BYTE byFromUser,BYTE byToUsers[],int iUserNums);
   //某玩家执行“碰”动作的数据修改
   bool ActPengTo(BYTE byFrom,BYTE byTo,TMjCGPNode* pCGPNode);
   //某玩家执行“杠”动作的数据修改
   bool ActGangTo(BYTE byFrom,BYTE byTo,TMjCGPNode* pCGPNode,int iChPoint[] = NULL);
   //某玩家执行“听”动作的数据修改
   bool ActTingTo(BYTE byTo);
   //执行某玩家“吃”动作且修改数据
   bool ActChiTo(BYTE byFrom, BYTE byTo, TMjCGPNode* pCGPNode);
   //某玩家执行“胡”动作的数据修改
   bool ActHuTo(BYTE byFrom,BYTE byToUsers[],int iUserNums);
   //计算分数
   void CalcRoundFen(TResult& result);
   //上传战绩
   void RecordMJUsersScore(TResult& result);
   //设置玩家为未知是否同意底注状态
   void SetUnknowStateTo(BYTE bDeskStation);
   //设置玩家为同意底注状态
   void SetAgreeStateTo(BYTE bDeskStation);
///////////////////算法处理////////////////////////
   //为某个玩家发手牌
   bool DealHandCardsTo(BYTE bDeskStation,int iSendNum);

   // 测试牌型
   bool testCardModel(int arrWall[], int nWallCardNum, int arrTest[], int nTestNum);

   //翻癞子牌
   TMjFetch FetchLaiZiCard();


   //为某个玩家抓一张牌
   TMjFetch FetchCardTo(BYTE bDeskStation);

   /*-被动事件响应，用于测试，通过客户端驱动---*/

   //客户端配置下发手牌
   int OnTestChangeFetchHandcards(BYTE bDeskStation,void* pData,UINT uSize,bool bWatchUser);

   //篡改手牌
   int OnTestChangeHands(BYTE bDeskStation,void* pData,UINT uSize,bool bWatchUser);

   //指定抓牌值
   int OnTestChangeFetch(BYTE bDeskStation,void* pData,UINT uSize,bool bWatchUser);

   //加载公共桌子配置
   BOOL LoadDeskCfg();
   //处理玩家托管消息
   int OnUserAutoRequest(BYTE bDeskStation,void* pData,UINT uSize,bool bWatchUser);
//-----------------------------断线重连------------------------------------------

   //处理要得到游戏未开始的状态的请求
   bool OnGetGSWaitSet(BYTE bDeskStation,
	   UINT uSocketID,bool bWatchUser);
   //打牌状态请求
   bool OnGetGSPlaying(BYTE bDeskStation,
	   UINT uSocketID,bool bWatchUser);
   //处理要得到游戏等待开始的请求
   bool OnGetGSWaitPiao(BYTE bDeskStation,
	   UINT uSocketID,bool bWatchUser);
   //等待下一局的状态请求
   bool OnGetGSNextRound(BYTE bDeskStation,
	   UINT uSocketID,bool bWatchUser);

   void ModifyGangPointWhileDianGangKai(int huUser);
/*-大多数麻将游戏都会有的属性-*/
public:
	//庄家
	int m_byNTUser;

	int m_byLaiZi;

	//是否有抓鸟
	bool m_bHaveFetchBird;

	//庄家打骰子定取牌方法时两骰子的点数值
	int m_byFetchPoint[2];

	//得到令牌的用户，得令牌者需要完成抓牌出牌交互
	int m_byTokenUser;

	//指定下个得令牌的玩家，如果没有指定(即值为255)，默认为下家得令牌
	int m_byTokenNextUser;

	//上次玩家出的牌
	int m_byLastOutCard;

	//上一个获得令牌的玩家
	int m_nLastPengUser;

	int m_nLastPengCard;

	//游戏开始时间
	time_t  m_gameBeginTime;
	//dong add
	UserInfoForGame_t m_ResultUserData[PLAY_COUNT];//用户信息

	//麻将玩法
	enum emPlayType
	{
		pt_Unknown	= 0,	//未知类型
		pt_HongZhong = 1,	//无红中点炮胡
		pt_Zimo	= 2,		//自摸胡
	}m_playType;

	bool    m_bHuangZhuang;		// 是否荒庄
	bool    m_bNTResult;			// 是否庄闲算分
	bool    m_bAllowDui7;			// 是否可胡七对
	bool    m_bPiao;			//是否加飘
	bool    m_bZhongCaishen;	//红中为财神
	int     m_iPlayerCount;      ////2 表示2人玩  3 表示3人玩  4表示4人
	
	bool    m_bIsYiMaQuanZhong;	//是否一马全中

	int     m_iNiaoNums;			//捉几个鸟
	int     m_iAllRounds;				// 总共几局结束
	int		m_iNeedRoomCards;				//总共需要消耗几张房卡
	int     m_niaoCardValues[MAX_NIAO_COUNT];			//保存鸟牌信息
	bool m_bIsZhongNiao[MAX_NIAO_COUNT];

	int    m_iAllCard[MJ_MAX_CARDS_ZHONG];             //所有的牌数据
	//牌墙剩余和牌数
	int    m_iWallLeft;
	//这里的值要和牌数据的里面的大小一样
	int    m_iMjCardNum;  

	std::string szConfig;
public:
	//最后一个抓牌的玩家
	BYTE m_byLastFetchCardUser;

	//用户数据
	UserData m_UserData[PLAY_COUNT];

	//配置数据
	TDeskCfg m_tagDeskCfg;

	//最近的成绩单
	TResult m_tagResult;

	//牌局信息每局结束后累加 当房间局数到达时  重新计算
	TRoundFinish m_tagRoundFinish;

	USHORT		m_usChiFlags;

	TEvTokenParm m_tagParam;

	TEvRoundFiniParm  m_RoundFiniParm;


	bool  m_bHaveGang;//是否有杠的操作(记得要在ExecuteGangAct进行处理)

	//桌面游戏的人数
	int m_PlayCount;

private:
	//各玩家的分组表
	BYTE m_byGroupTable[PLAY_COUNT];

	//桌面游戏的人数
	int m_iPlayCount;

	//选择是否加飘人数
	int m_iPiaoCount;

	//是否所有人都同意解散房间
	bool m_bAllAgreeDismissRoom;
};
