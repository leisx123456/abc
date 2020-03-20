#pragma once
#include <map>
#include "comstruct.h"
#include "IGameDesk.h"
#include "MjCommonDef.h"
#include "UpgradeMessage.h"
#include "CGameDesk.h"

//***************************************************************************************
//**********************************�û����ݹ���*****************************************
//***************************************************************************************
////��¼����������ϵ���Ϸ��Ϣ
struct UserData
{
	/*-�����Ϸ���ݶ���-*/
	//���״̬����Ҫ�����ж���Щ���ͬ��/��ͬ����Ϸ 
	enum emState
	{
		UNKNOWN = 0,		 //δ֪״̬
		AGREEGAME = 3,       //���ͬ����Ϸ
	};
	emState m_nState;

	////��ҷ��򣬶��������ֱ�Ϊ0,1,2,3��255Ϊδ������
	//BYTE m_byDirect;

	//����Ƿ����ѡׯ����
	bool m_bMakeNTOK;

	//����Ƿ���ɴ����Ӷ���λ����
	bool m_bAssignedDir;


	//����Ƿ���ɴ����Ӷ����Ʒ�������
	bool m_bFetchMethodOK;
	//�û�������
	int m_byHandCards[HAND_MAX_NUM_MJ];

	//�û���������
	int m_iHandNums;

	//ץ�ƴ���
	int m_iFetchTimes;

	//����ѳ��ƵĴ���
	int m_iOutedTimes;

	//�ѳ��Ʊ�
	int m_byOutedCards[MJ_MAX_OUTED];

	//�ѳ�������
	int m_iOutedNums;
	//��ҵĶ�����Ϣ�ṹ
	TMjActInfo m_tagActInfo;
	//���Ƽ�¼
	TMjCGPNode m_lstCGP[MJ_MAX_CGP];
	
	//��������
	int m_iCGPNums;

	//����Ƿ�������
	bool m_bIsTing;

	//��Һ�����
	int m_byHuCard;

	//��Ϸ�����ָһ���齫�Ľ��������ڡ�������(ʹ��MjBase::MjResult�ռ䶨��ĳ���)
	int m_nResult;

	//��Һ������ͱ�
    emHuType m_nHuTypes[MJ_MAX_HUTYPE];

	//��Һ������ͱ���
	int m_iHuTypeNums;

	//��ҵ���������(����Ͽ���,���ܺ�,�ܳ��)
	emHuSpecialWay m_nHuSpecWay[2];	

	//�Ͼ��Ƿ������ܺ�
	bool m_bIsLastRoundQiangGang;
	bool m_bLastDianGangKai;

	///Ư
	bool m_bPiao;
	int  m_nMingGangNums;             //��������(�������ܡ����ܡ����С�����)
	int	 m_nZhongGangNums;			//���и�����
	int	 m_nFagangNums;				//���Ƹ�����
	int  m_nAnGangNums;               //��������
	int  m_nSpecialGangNums;          //���������(��Ӹ�)
	//��Ҵ������
	int  m_nDaHuCounts;

	//ÿ�����ÿ�ֵĸܷ�
	int iGangPoint;

	//����Ƿ�©��
	bool m_bLouHu;

	//���©����Щ��
	int m_LouHuCard[MJ_MAX_LOUHU];
	//���©������
	int m_LouFan;

	//�ܸܵ���ѡ����������
	std::vector<int> m_vCanGangSelPengCard;

	//20181203�ܸܵ���ѡ�����������Ƿ񲹸ܼӷ�-�����ƺ�Ͳ��ӷ���
	//std::map<int, bool> m_mapValidGangPoint;
	int m_arrValidGangPoint[40]; //�±�-��ֵ�� �±�ֵ:0-���Ʋ����ܸ�ѡ����, 1-�ܷ���Ч, 2-�ܷ���Ч

	//���Ʈ�ĵ���
	int m_iPiaoNum;

	//��ܸܿ�����ʼ��������Ϊʵ�ֵ�ܺ��ƺ��ָܸ��Ͽ�����Ҫ�а��ܷ֣�
	int m_dianGangGangKaiIndex;
/*********************************************************************************/
/*-��ҷ���Ϸ����-*/
	//�Ƿ��й�״̬
	bool m_bAuto;
	//����Ƿ��Ѷ���
	bool m_bNetCut;

/*-��������-*/
	//�����ֵ��Ч(������0),��ץ��ȡǿ��ʹ�ô�ֵ,\
	��ֵһ����ʹ��
	int m_byChFetch;

	//�����ֵΪtrue ˵���ͻ��˷����䷢������
	bool m_bFetchHandCardCheat;
	int m_byHandCardsCheat[HAND_MAX_NUM_MJ];

	//©����
	bool m_bLouPeng[40];

	// ���������Ϣ
		bool m_bYouJinQiangGang;
		bool m_bNormalQiangGang;

	TAG_INIT(UserData)
	{
		::memset(this,0,sizeof(UserData));
	}


	//���ʼ��
	void HalfInit()
	{
		::memset(m_bLouPeng,0,sizeof(m_bLouPeng));

		//m_vCanGangSelPengCard.clear();
		//if (!m_mapValidGangPoint.empty())
		//{
		//	m_mapValidGangPoint.clear();
		//}
		::memset(m_arrValidGangPoint,0,sizeof(m_arrValidGangPoint));

		//�Լ���״̬
		m_nState = UNKNOWN;

		//�Ƿ��й�״̬
		m_bAuto = false;

		//��ʼ��Ʈ�ķ�
		m_iPiaoNum = -1;

		m_bPiao = false;

		//����Ƿ���ɴ����Ӷ����Ʒ�������
		m_bFetchMethodOK = false;




		//�û�������
		::memset(m_byHandCards,0,sizeof(m_byHandCards));

		//�û���������
		m_iHandNums = 0;

		//ץ�ƴ���
		m_iFetchTimes = 0;

		//�ѳ��Ʊ�
		::memset(m_byOutedCards,0,sizeof(m_byOutedCards));

		//�ѳ�������
		m_iOutedNums = 0;

		//����ѳ��ƵĴ���
		m_iOutedTimes = 0;

		//��ҵĶ�����Ϣ�ṹ
		m_tagActInfo.Clear();

		//���Ƽ�¼
		for (int i = 0; i < MJ_MAX_CGP; ++i)
		{
			m_lstCGP[i].Clear();
		}

		//����Ƿ�������
		m_bIsTing = false;

		//��������
		m_iCGPNums = 0;

		//��Һ�����
		m_byHuCard = 0;

		//��Һ������ͱ�
		::memset(m_nHuTypes,emHuType::ht_Unknown,sizeof(m_nHuTypes));

		//��Һ������ͱ���
		m_iHuTypeNums = 0;

		//��ҵ�������Ʒ�ʽ(����غ�,���Ͽ���,���ܺ�,�ܳ��)
		m_nHuSpecWay[0] = emHuSpecialWay::hsw_Unknown;
		m_nHuSpecWay[1] = emHuSpecialWay::hsw_Unknown;
		m_bLouHu = false;
		::memset(m_LouHuCard,0,sizeof(m_LouHuCard));

		//��ʼ����		
		m_nMingGangNums = 0;             //��������(�������ܡ����ܡ����С�����)
		m_nAnGangNums = 0;               //��������
		m_nSpecialGangNums = 0;          //���������(��Ӹ�)
		m_nZhongGangNums = 0;			//���и�����
		m_nFagangNums = 0;				//���Ƹ�����

		m_nDaHuCounts = 0;               //��Ҵ������
		m_LouFan = 0;

		m_dianGangGangKaiIndex = -1;

		m_bYouJinQiangGang = false;
		m_bNormalQiangGang = false;
	}


	//��������ʼ��
	void Init()
	{
		HalfInit();

		m_bIsLastRoundQiangGang=false;
		m_bLastDianGangKai = false;

		//����Ƿ���ɴ����Ӷ���λ����
		m_bAssignedDir = false;

		//����Ƿ����ѡׯ����
		m_bMakeNTOK = false;

		//��Ϸ�����ָһ���齫�Ľ��
		m_nResult = MjResult::Res_Unknown;

		//����Ƿ��Ѷ���
		m_bNetCut = false;

		//�ܷ�
		iGangPoint = 0;

		m_LouFan = 0;
	}

	//���©������ֵ��������©��Ҫ������Ų��ܺ� �� ����5w  ����5w���ܺ������������ڻ��ǿ��Ժ���
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





//��Ϸ����


class CMJDesk:public CGameDesk
{
public:
	//���캯��
	CMJDesk(); 

	//��������
	virtual ~CMJDesk();

	///��Ϸ���ݰ��ʼ��
	void HalfInit();
	///ȫ��ʼ��
	void Init(const UserInfoForGame_t* pUserInfo);
/*-----------------------------------------------------------------------------------------------------------
    ʵ��IGameDesk�ӿ�,һϵ��ƽ̨�ṩ����Ҫ������������Ϸ����ĺ����������ô������Ϸ�Ŀ�������ÿ��������ʵ������Ϸ���졣
  ----------------------------------------------------------------------------------------------------------*/
public:
	void killAllTimer();

	//�������
	virtual void OnUserLeftDesk(int nDeskStation);

	////��ʼ������
	virtual int InitialDesk(int nDeskNo, int nMaxPeople, IGameMainManager* pIMainManager);

	//������Ϸ�ڲ���Ϣ
	virtual int HandleNotifyMessage(int nDeskStation, UINT code, BYTE* pBuffer, int nLen);

	//��ȡ��Ϸ״̬�������͵��ͻ���
	virtual int OnGetGameStation(int nDeskStation, UserInfoForGame_t& userInfo);

	//��ʱ��ʱ�䵽
	virtual int OnGameTimer(int nTimerID);

	//���������ĳλ��
	virtual void OnUserSitDesk(int nDeskStation);

	//��Ҷ���
	virtual void OnUserNetCut(int nDeskStation);

	//���䱻�����ɢ
	virtual void onDismissDesk();

	//��ȡ��ϷID
	virtual unsigned int GetGameNameID();

	//�������ͬ����Ϸ
	virtual void  OnUserAgreeGame(int bDeskStation);

	//�������������
	virtual void OnUserRecome(int bDeskStation);
	////��ʼ��Ϸ
	//virtual bool InitDeskGameStation();

	//��Ϸ��ʼ
	virtual bool GameBegin(BYTE bBeginFlag);

	//��Ϸ����
	virtual bool GameFinish(BYTE bDeskStation,BYTE bCloseFlag);


	/*-���ش��麯��-*/

	//������Ϸ״̬
	virtual bool ReSetGameState(BYTE bLastStation);


	//�ж���Ϸ�Ƿ��Ѿ���ʼ����
	virtual bool IsHavePlaying();

	//�ж������Ƿ�ﵽ��ɢ����
	virtual bool DeskCanDismiss();

	// ���ӱ����ú󱻴����ĺ���
	virtual void OnDeskConfigured(const char* szClientConfig, const char *szSysConfig = NULL);
////��Ϸ�߼�

public:
	//���ö�ʱ��
	void SetTimer(int iTimeID, int iTimerInterval);
	//ɱ����ʱ��
	void KillTimer(int iTimeID);
	//�趨ׯ���¼�
	bool OnEventMakeNT();

	// 20181203 �ͻ���Ҫ���Ʒ�����ɸ�ӵĶ���
	bool OnEventFetchMethod();

	//ϵͳ�������¼�
	bool OnEventFetchHandCards();

	bool OnEventFetchGui();

	//��Ҽ�Ʈ�¼�
	bool OnEventPiao();
	//���ץ���¼�
	bool OnEventFetchNiao();
	//ϵͳΪ��һ������·�����,��������ץ�Ƴ���
	bool OnEventGiveNextToken();
	//���ݸ���ҵ�m_nResult�ж��Ƿ�Ϊ��ׯ
	int GetIsHe();
	//��ȡʣ���Ƴǵ�������
	int GetWallLeft();
	//��ĳ�����ȡ�ö���(���ܷ�����ܺ���)
	bool SelectActInfoFrom(BYTE byFromUser,BYTE byToUser,int byCard,TMjActInfo* pActInfo,int byFetctCard,USHORT usIgnoreFlags = 0,bool bIsQiangGangJudge=false);

	//�õ�ѡׯ������ɵ��������
	int GetMakeNtPlayers();
	//��ȡָ������������������û�ж���
	bool IsOtherPlayersHaveAct(BYTE byStation);
	///��ҳ���ɾ�����е���
	void OutCardTo(BYTE bDeskStation,int byCard);
	//ͨ������ҵĶ���״̬λ���õ����Ͽ���ִ�ж�������Ҽ�
	int SelectActAtOnceUsers(BYTE byUsers[],USHORT& usActFlag);
	//�õ����ͬ����Ϸ������ͬ���ע��
	int GetAgreePlayers();
/////////////���տͻ�����Ϣ����//////////////////////////////////////////
   //������Ҷ�ׯ��ɶ���
    int OnUserMakeNTOver(BYTE bDeskStation,void* pData,UINT uSize,bool bWatchUser);
    //����ͻ��˼�Ʈ
    int OnUserPiao(BYTE bDeskStation,void* pData,UINT uSize,bool bWatchUser);
    //������ҳ�������
    int OnUserOutCardRequest(BYTE bDeskStation,void* pData,UINT uSize,bool bWatchUser);
    //������ҵĶ�������
    int OnUserActRequest(BYTE bDeskStation,void* pData,UINT uSize,bool bWatchUser);
	//һ�غϽ������¼�����
	bool OnEventRoundFinish();
	//����¼�����
	bool OnEventResult();
	///�Զ�����
	bool OnEventAutoAct(bool bCan);
	///�Զ�����
	bool OnEventAutoOutCard(bool bCan);
////////////////////////////////������ִ��//////////////////////////////////////////////////
   //ִ�С���������
   bool ExecuteGuoAct(BYTE bDeskStation);
   //ִ�С���������
   bool ExecutePengAct(BYTE byFromUser,BYTE byToUser);
   //ִ�С��ܡ�����
   bool ExecuteGangAct(BYTE byFromUser,BYTE byToUser);
   //ִ�С���������
   bool ExecuteTingAct(BYTE byToUser);
   //ִ�г��ƶ���
   bool ExecuteChiAct(BYTE byFromUser, BYTE byToUser);
   //ִ�С���������
   bool ExecuteHuAct(BYTE byFromUser,BYTE byToUsers[],int iUserNums);
   //ĳ���ִ�С����������������޸�
   bool ActPengTo(BYTE byFrom,BYTE byTo,TMjCGPNode* pCGPNode);
   //ĳ���ִ�С��ܡ������������޸�
   bool ActGangTo(BYTE byFrom,BYTE byTo,TMjCGPNode* pCGPNode,int iChPoint[] = NULL);
   //ĳ���ִ�С����������������޸�
   bool ActTingTo(BYTE byTo);
   //ִ��ĳ��ҡ��ԡ��������޸�����
   bool ActChiTo(BYTE byFrom, BYTE byTo, TMjCGPNode* pCGPNode);
   //ĳ���ִ�С����������������޸�
   bool ActHuTo(BYTE byFrom,BYTE byToUsers[],int iUserNums);
   //�������
   void CalcRoundFen(TResult& result);
   //�ϴ�ս��
   void RecordMJUsersScore(TResult& result);
   //�������Ϊδ֪�Ƿ�ͬ���ע״̬
   void SetUnknowStateTo(BYTE bDeskStation);
   //�������Ϊͬ���ע״̬
   void SetAgreeStateTo(BYTE bDeskStation);
///////////////////�㷨����////////////////////////
   //Ϊĳ����ҷ�����
   bool DealHandCardsTo(BYTE bDeskStation,int iSendNum);

   // ��������
   bool testCardModel(int arrWall[], int nWallCardNum, int arrTest[], int nTestNum);

   //�������
   TMjFetch FetchLaiZiCard();


   //Ϊĳ�����ץһ����
   TMjFetch FetchCardTo(BYTE bDeskStation);

   /*-�����¼���Ӧ�����ڲ��ԣ�ͨ���ͻ�������---*/

   //�ͻ��������·�����
   int OnTestChangeFetchHandcards(BYTE bDeskStation,void* pData,UINT uSize,bool bWatchUser);

   //�۸�����
   int OnTestChangeHands(BYTE bDeskStation,void* pData,UINT uSize,bool bWatchUser);

   //ָ��ץ��ֵ
   int OnTestChangeFetch(BYTE bDeskStation,void* pData,UINT uSize,bool bWatchUser);

   //���ع�����������
   BOOL LoadDeskCfg();
   //��������й���Ϣ
   int OnUserAutoRequest(BYTE bDeskStation,void* pData,UINT uSize,bool bWatchUser);
//-----------------------------��������------------------------------------------

   //����Ҫ�õ���Ϸδ��ʼ��״̬������
   bool OnGetGSWaitSet(BYTE bDeskStation,
	   UINT uSocketID,bool bWatchUser);
   //����״̬����
   bool OnGetGSPlaying(BYTE bDeskStation,
	   UINT uSocketID,bool bWatchUser);
   //����Ҫ�õ���Ϸ�ȴ���ʼ������
   bool OnGetGSWaitPiao(BYTE bDeskStation,
	   UINT uSocketID,bool bWatchUser);
   //�ȴ���һ�ֵ�״̬����
   bool OnGetGSNextRound(BYTE bDeskStation,
	   UINT uSocketID,bool bWatchUser);

   void ModifyGangPointWhileDianGangKai(int huUser);
/*-������齫��Ϸ�����е�����-*/
public:
	//ׯ��
	int m_byNTUser;

	int m_byLaiZi;

	//�Ƿ���ץ��
	bool m_bHaveFetchBird;

	//ׯ�Ҵ����Ӷ�ȡ�Ʒ���ʱ�����ӵĵ���ֵ
	int m_byFetchPoint[2];

	//�õ����Ƶ��û�������������Ҫ���ץ�Ƴ��ƽ���
	int m_byTokenUser;

	//ָ���¸������Ƶ���ң����û��ָ��(��ֵΪ255)��Ĭ��Ϊ�¼ҵ�����
	int m_byTokenNextUser;

	//�ϴ���ҳ�����
	int m_byLastOutCard;

	//��һ��������Ƶ����
	int m_nLastPengUser;

	int m_nLastPengCard;

	//��Ϸ��ʼʱ��
	time_t  m_gameBeginTime;
	//dong add
	UserInfoForGame_t m_ResultUserData[PLAY_COUNT];//�û���Ϣ

	//�齫�淨
	enum emPlayType
	{
		pt_Unknown	= 0,	//δ֪����
		pt_HongZhong = 1,	//�޺��е��ں�
		pt_Zimo	= 2,		//������
	}m_playType;

	bool    m_bHuangZhuang;		// �Ƿ��ׯ
	bool    m_bNTResult;			// �Ƿ�ׯ�����
	bool    m_bAllowDui7;			// �Ƿ�ɺ��߶�
	bool    m_bPiao;			//�Ƿ��Ʈ
	bool    m_bZhongCaishen;	//����Ϊ����
	int     m_iPlayerCount;      ////2 ��ʾ2����  3 ��ʾ3����  4��ʾ4��
	
	bool    m_bIsYiMaQuanZhong;	//�Ƿ�һ��ȫ��

	int     m_iNiaoNums;			//׽������
	int     m_iAllRounds;				// �ܹ����ֽ���
	int		m_iNeedRoomCards;				//�ܹ���Ҫ���ļ��ŷ���
	int     m_niaoCardValues[MAX_NIAO_COUNT];			//����������Ϣ
	bool m_bIsZhongNiao[MAX_NIAO_COUNT];

	int    m_iAllCard[MJ_MAX_CARDS_ZHONG];             //���е�������
	//��ǽʣ�������
	int    m_iWallLeft;
	//�����ֵҪ�������ݵ�����Ĵ�Сһ��
	int    m_iMjCardNum;  

	std::string szConfig;
public:
	//���һ��ץ�Ƶ����
	BYTE m_byLastFetchCardUser;

	//�û�����
	UserData m_UserData[PLAY_COUNT];

	//��������
	TDeskCfg m_tagDeskCfg;

	//����ĳɼ���
	TResult m_tagResult;

	//�ƾ���Ϣÿ�ֽ������ۼ� �������������ʱ  ���¼���
	TRoundFinish m_tagRoundFinish;

	USHORT		m_usChiFlags;

	TEvTokenParm m_tagParam;

	TEvRoundFiniParm  m_RoundFiniParm;


	bool  m_bHaveGang;//�Ƿ��иܵĲ���(�ǵ�Ҫ��ExecuteGangAct���д���)

	//������Ϸ������
	int m_PlayCount;

private:
	//����ҵķ����
	BYTE m_byGroupTable[PLAY_COUNT];

	//������Ϸ������
	int m_iPlayCount;

	//ѡ���Ƿ��Ʈ����
	int m_iPiaoCount;

	//�Ƿ������˶�ͬ���ɢ����
	bool m_bAllAgreeDismissRoom;
};
