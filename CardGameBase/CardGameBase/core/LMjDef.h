#ifndef __L_MJ_DEF_H__
#define __L_MJ_DEF_H__
#include <iostream>
#include <vector>
#include <math.h>

// �齫ͨ�ö���
//////////////////////////////////////////////////////////////////////////
//�����궨��
#define MJ_MAX_CARD_COUNT				144									// һ���齫�������	
#define MJ_MAX_PLAYER					4									//��Ϸ����
#define MJ_MAX_WEAVE					4									//������
#define MJ_MAX_INDEX					42									//�������
#define MJ_MAX_HAND_COUNT				14									//�����Ŀ
#define MJ_MAX_OUTED_COUNT				30									//�����Ŀ
#define MJ_TOTAL_CARD_NUM				108									//�����
#define MJ_MAX_SETTLEMENT_LIST_SIZE		16									// �����嵥�����

#define CARD_EMPTY					0


//��Ϸ�¼�ID����,(ȡֵ��Χ��10-49)����ʱ���Ĵ���

#define TIME_ID_Ready					8			// ׼��
#define TIME_ID_Begin					9			// ׼��
#define TIME_ID_CHOOSE_A_SEAT			10			// ѡ��
#define TIME_ID_CHOOSE_BANKER			11			// ѡׯ
#define TIME_ID_SHUFFLE_CARD			13			// ϵͳϴ�ƾ��¼�
#define TIME_ID_ROCK_DICE				12			// ϵͳȷ�����Ʒ������ƶ������¼�
#define TIME_ID_FETCH_HANDCARDS			14			//ϵͳ�������¼�
#define TIME_ID_TBA						16          
#define TIME_DRAW_CARD					17			//ϵͳΪĳ������·������¼�,��������ץ�Ƴ���
#define TIME_ID_RESULT					18			//��Ϸ�����¼�
#define TIME_ID_CHA_JIAO				19			//���
#define TIME_ID_ROUND_FINISH			20			//һ�غ����/�¼�

#define TIME_ID_COMPUTER_THINK_ACT		21			// ����˼�������ܺ�
#define TIME_ID_COMPUTER_THINK_OUT_CARD	22			// ����˼������

#define TIME_ID_AUTO_ACT				24			//�Զ�ִ�ж���
#define TIME_ID_AUTO_OUTCARD			25			//�Զ�����
#define TIME_ID_AUTO_ACT_NO				26			//û���й��Զ�ִ�ж���
#define TIME_ID_AUTO_OUTCARD_NO			27			//û���й��Զ�����



const int SCORE_BASE = 5;
const int MAX_FAN = 3;
const long COIN_MUL = 200;


// ������Եĺ�
#define TEST_LOCAL_LX

//////////////////////////////////////////////////////////////////////////
//�߼�����

#define	MASK_COLOR					0xF0								//��ɫ����
#define	MASK_VALUE					0x0F								//��ֵ����





//�齫��ȡ�Ʒ���-˳ʱ��-������
enum E_WallGetDirection
{
	EW_South,
	EW_West,
	EW_North,
	EW_East
};

//���ӷ���������ƴ���˳��-��ʱ��-������
enum E_PlayerPriorityDirection
{
	EP_South,
	EP_East,
	EP_North,
	EP_West,
	EP_InValid
};


enum E_GameState
{
	EGS_Ready,
	EGS_Begin,
	EGS_ReadyRolateDice,
	EGS_RolatingDice,         //ing
	EGS_TBA,                 //to be assured,��ȱ
	EGS_Playing,
	EGS_Over
};

struct T_DeskConfig
{
	enum E_ZiMoAddScoreType
	{
		EZ_ZiMoAddUnit,	// �����ӵ�
		EZ_ZimoAddFan,	// �����ӷ�
	};
	int bBaseScore;

	bool bAbleHaiDiLao;
	int nMaxFan;	// ��������������ֵ255

	T_DeskConfig()
		: bBaseScore(5)
		, bAbleHaiDiLao(true)
		, nMaxFan(3)
	{

	}
};

// �����
struct T_WeaveCardsValueItem
{
	unsigned char byWeaveKind;						//�������
	int aCards[4];					//�������
	int cardCenter;						//������
	int cardPublic;						//Ŀ����

	unsigned char byProvideUser;					//��Ӧ�û�

	int arrNotHuUser[MJ_MAX_PLAYER];
	int nNotHuUserNum;

	T_WeaveCardsValueItem()
	{
		memset(this, 0, sizeof(T_WeaveCardsValueItem));
	}

	int cardNum() const
	{
		if (byWeaveKind > 1 && byWeaveKind < 5)
		{
			return 4;
		}
		return 3;
	}

	int calculateScore(const T_DeskConfig & tDeskConfig)
	{
		if (byWeaveKind == 2)
		{
			return 2 * tDeskConfig.bBaseScore * nNotHuUserNum;
		}
		else if (byWeaveKind == 3)
		{
			return tDeskConfig.bBaseScore * nNotHuUserNum;
		}
		else if (byWeaveKind == 4)
		{
			return 2 * tDeskConfig.bBaseScore;
		}
		else
		{
			return 0;
		}
	}
};

enum ActiveUserType
{
	EA_FristGot,
	EA_Inherit,
	EA_Response
};
//���Ҷ���
// ��Ϊ���ҵ���Դ��1.��һ��ϵͳָ��ׯ�� 2. �̳У��ϸ����ҳ���û������Ӧ����ϵͳ����ʱ��ָ�� 3.��Ӧ�����ϸ����ҳ�������Ӧ�˶�����
// ������־:����
struct T_ActiveUser
{
	int nActiveUser;
	ActiveUserType eActiveUserType;

	// �̳���,��ǰ����������
	int nNewCard;

	// ��Ӧ��
	int nGotCard;
	int nFormUser;

	// �Լ��������
	int nOutCard;

	T_ActiveUser()
		: nActiveUser(0)
		, eActiveUserType(EA_FristGot)
	{

	}

};

// ��ҵĶ�����ϸ��Ϣ�ṹ////////////////////////////////////////////////////////////////////
// ö�����ж���
enum E_ActionTypeFlags
{
	EA_Wait = 0x0001L,  //�ȴ������Ƚ�λ�����������ǰΪ0����������Ϊ1
	EA_Pass = 0x0002L,  //��
	EA_Eat = 0x0004L,  //��
	EA_Pong = 0x0008L,	//����
	EA_Kong = 0x0010L,	//����
	EA_Listen = 0x0020L,
	EA_hu = 0x0040L,	//��

};

enum E_EatTypeFlags
{
	EA_EatLeft = 0x01,	//���
	EA_EatCenter = 0x02,	//�г�
	EA_EatRight = 0x04, //�ҳ�
};

struct T_MjActEatInfo
{
	int arrEatSelect[3];  //���Գ��Ƶ�ѡ��
	int nEatSelectNums;	  //���Գ���ѡ������
	unsigned short usEatFlags;	//�Եı�־
};

enum E_KongType
{
	EK_KongAn,	//����-����
	EK_KongBa,	//�η�-�͸�,����
	EK_KongDian	//�η�-���
};

// �е��齫�����������������
struct T_MjActKongInfo
{
	int arrKongSelect[MJ_MAX_WEAVE];	  //���Ըܵ�ѡ��
	int nKongSelectNums;				  //���Ը�ѡ������		
	int arrBuGangInPeng[MJ_MAX_WEAVE]; //�����ڽ�������һ��������
	E_KongType arrKongType[4]; //������

	int nCurSelectIndex;

	//���Ҹ���ѡ�������
	int FindGangIndex(int byCard) const
	{
		for (int i = 0; i < nKongSelectNums; ++i)
		{
			if (byCard == arrKongSelect[i])
			{
				return i;
			}
		}
		return -1;
	}
	T_MjActKongInfo()
	{
		clear();
	}

	void clear()
	{
		memset(this, 0, sizeof(T_MjActKongInfo));
	}
};

struct T_MjActListenInfo
{
	int arrListenSelect[14]; //�����ɴ��������ѡ��
	int nListenSelectNums; //�����ɴ��������ѡ������
	//���Ҹ���ѡ�������
	int FindTingIndex(int byCard) const
	{
		for (int i = 0; i < nListenSelectNums; ++i)
		{
			if (byCard == arrListenSelect[i])
			{
				return i;
			}
		}
		return -1;
	}
};

//���ƶ���
enum E_MjHuType
{
	EH_Unknown = 0,	//δ֪���
	EH_3x_2 = 1,	// 3x+2
	EH_SevenPair = 2,			//�߶�
	EH_ShiSanYao = 3,     //ʮ����
	EH_JiangJiangHu = 4,
	EH_YouJin = 5,        //�ν�
	EH_SanJinDao = 6,		//����,

};

enum E_MjHuNameFlags
{
	// ��������
	EHN_Ping = 0x0001,			//ƽ��
	EHN_Pong = 0x0002,	             //������,�����
	EHN_Qing = 0x0004,	    // ��һɫ
	//EHN_All_19 = 0x0008,	              //�۾�
	EHN_SevenPair = 0x0010,     	      //�߶�
	EHN_JinGouDiao = 0x0020,			// �𹳵�
	EHN_DragonSevenPair = 0x0040,	     //���߶�

	// ���ⷬ��
	//EHN_258_Pong,	          //����
	//EHN_Dai_19,	// ��19
	// ���Ϸ���
	EHN_QingPong = EHN_Qing | EHN_Pong,	          //����,�����
	EHN_QingSevenPair = EHN_Qing | EHN_SevenPair,	      //���߶�
	EHN_QingDragonSevenPair = EHN_Qing | EHN_DragonSevenPair,	  //�����߶�
	//EHN_Qing_19 = EHN_Qing | EHN_All_19,	          //���۾�
	EHN_QingJinGouDiao = EHN_Qing | EHN_JinGouDiao,	  //��𹳵�

};

// ������������������ϵ�
//enum E_MjHuAbleCombined
//{
//	//EHN_Qing,	    // ��һɫ
//	EHN_HaiDiLao,	// ������
//};

enum E_MjHuWay
{
	EHW_Unknown,  //δ֪���
	// ��ͨ��ʽ
	EHW_JiePao,  //����
	EHW_ZiMo,  //����
	// ���ⷽʽ
	EHW_DayHu,	              //���
	EHW_LandHu,	               //�غ� - �мҵ�һ������
	EHW_PensonHu,	              //�˺� - ��ׯ�Ҵ��һ���ƾͱ��мҺ���
	EHW_QiangGang,  //����
	EHW_DianGangKai,  //��ܸ��Ͽ���
	EHW_AnGangKai,		//���ܸ��Ͽ���
	EHW_BuGangKai,		//���ܸ��Ͽ���
	//EHW_GangChong,  //�ܳ�


};



// ��Һ��ƶ�����Ϣ
struct T_MjActHuInfo
{
	//int nHuNameNums;	//������������		
	E_MjHuNameFlags eMjHuName;
	E_MjHuWay eMjHuWay; // ���Ʒ�ʽ
	int gen; // ������
	bool bHaiDiLao;
	//int nHuIndex;
	T_MjActHuInfo()
	{
		memset(this, 0, sizeof(T_MjActHuInfo));
	}
};

// ������Ϣ
struct T_MjActOutInfo
{
	int nOutCardUser;
	int nOutCardValue;
	int m_nOutedNums; // ��������������������ȥ���ܺ����ƣ� ����ˢ�½������
	T_MjActOutInfo()
	{
		clear();
	}
	void clear()
	{
		nOutCardUser = -1;
		nOutCardValue = CARD_EMPTY;
		m_nOutedNums = 0;
	}
};

// ��Ҷ�����Ϣ-��
struct T_MjActInfo
{
	unsigned short usActFlags;	//������ʶ

	T_MjActEatInfo tMjActEatInfo;
	T_MjActKongInfo tMjActKongInfo;
	T_MjActListenInfo tMjActListenInfo;
	T_MjActHuInfo tMjActHuInfo;

	// ���⶯��
	T_MjActOutInfo tMjActOutInfo;

	T_MjActInfo()
	{
		memset(this, 0, sizeof(T_MjActInfo));
	}

	void clear()
	{
		memset(this, 0, sizeof(T_MjActInfo));
	}

};

//////////////////////////////////////////////////////////////////////////



// ���ָ��˺�����Ϣ
struct T_UserHuInfo
{
	bool bHu;
	int nHuIndex;						// �ڼ�����
	unsigned char byFangPaoUser;		//�������

	bool bChaJiao;	// �Ƿ�ɹ����˱��˵Ľ�
	int arrBeChaoJiaoUser[MJ_MAX_PLAYER];	// ����е����id

	E_MjHuWay eMjHuWay;
	E_MjHuNameFlags eMjHuName;
	int nGeng;	// ��

	// �ط�����
	bool bHaiDiLao;

	// 
	int nTotalFan;	// ��ǰ�����ܷ���
	int nFinalFan;	// ��ǰ�������շ���(����ж�����������ܳ��������)
	int nFinalUnit;	// ��ǰ�������ն��ٸ� �׷���
	int nFinalScore;	// ��ǰ�������յ÷�nTotalScore = nTotalUnit*UnitScore; //UnitScore����Ҷ�



	T_UserHuInfo()
	{
		memset(this, 0, sizeof(T_UserHuInfo));

	}

	bool isZiMoType() const
	{
		//bool bBelongZiMo = false;
		if (eMjHuWay == EHW_ZiMo
			|| eMjHuWay == EHW_DayHu
			|| eMjHuWay == EHW_LandHu
			|| eMjHuWay == EHW_PensonHu
			|| eMjHuWay == EHW_DianGangKai
			|| eMjHuWay == EHW_AnGangKai
			|| eMjHuWay == EHW_BuGangKai)
		{
			return true;
		}
		return false;
	}

	void calculateScore(const T_DeskConfig & tDeskConfig)
	{
		if (!bHu)
		{
			return;
		}
		// E_MjHuWay  + E_MjHuName + ��
		int nUnit = 0;
		int nFan = 0;
		//E_MjHuWay
		switch (eMjHuWay)
		{
		case EHW_Unknown:
			break;
		case EHW_JiePao:
			break;
		case EHW_ZiMo:
			nUnit++;//�����ӵ�
			break;
		case EHW_DayHu:
			nUnit++;
			nFan += 6;
			goto end;
		case EHW_LandHu:
			nUnit++;
			nFan += 6;
			goto end;
		case EHW_PensonHu:
			nFan += 6;
			goto end;
		case EHW_QiangGang:
			nFan++;
			break;
		case EHW_DianGangKai:
			nUnit++;
			nFan++;
			break;
		case EHW_AnGangKai:
			nUnit++;
			nFan++;
			break;
		case EHW_BuGangKai:
			nUnit++;
			nFan++;
			break;
		default:
			break;
		}

		// E_MjHuName
		switch (eMjHuName)
		{
		case EHN_Ping:
			break;
		case EHN_Pong:
			nFan++;
			break;
		case EHN_Qing:
			nFan += 2;
			break;
		case EHN_SevenPair:
			nFan += 2;
			break;
		case EHN_JinGouDiao:
			nFan += 2;
			break;
		case EHN_DragonSevenPair:
			nFan += 3;
			break;
		case EHN_QingPong:
			nFan += 3;
			break;
		case EHN_QingSevenPair:
			nFan += 4;
			break;
		case EHN_QingDragonSevenPair:
			nFan += 5;
			break;
		case EHN_QingJinGouDiao:
			nFan += 4;
			break;
		default:
			break;
		}

		// �ط��������
		if (tDeskConfig.bAbleHaiDiLao)
		{
			if (bHaiDiLao)
			{
				nFan++;
			}
		}

		//gen
		nFan += nGeng;	// ���ͷ���+��

	end:
		nTotalFan = nFan;
		nFinalFan = nTotalFan;
		if (nTotalFan > tDeskConfig.nMaxFan)
		{
			nFinalFan = tDeskConfig.nMaxFan;
		}

		int nFinalMultiple = pow(2, nFinalFan); //����
		nFinalUnit = nUnit + nFinalMultiple;
		nFinalScore = nFinalUnit * tDeskConfig.bBaseScore;
	}
};





//////////////////////////////////////////////////////////////////////////
// �齫����ͨ�����ݽṹ����

// ����������
struct T_MsgDealCards
{
	int arrCardHand[4][14];
	int nPlayerCount;
	int arrMjCardsPair[MJ_MAX_CARD_COUNT];
	int nMjNumAllocation;

};

// ��ȱ���
struct T_MsgDingQue
{
	int arrCardColor[4];

};

// ָ�ɻ�û�, 
struct T_MsgAppointActiveUser
{
	int nActiveUser;

};



// ��Ӧ��û�
struct T_MsgResponseToActiveUser
{
	int nActiveID;		// ����id
	int nResponseID[4];	// ��Ӧ�����id�������ǻ����Լ�

	// ��Ӧ��ҿ��Ծ߱���Щ������Ϊ
	T_MjActInfo tMjActInfo[4];

};

// ������Ϣ
struct T_MsgDrawCard
{
	int nDrawCardUser;
	int nNewCard;

	int arrMjCardsPair[MJ_MAX_CARD_COUNT]; // ������ǽ
	int nMjNumAllocation;	// һ���齫������

	int nIndexStart;
	int nIndexCurrent;
};

// ���ƽ��
struct T_MsgOutCard
{
	int nOutCardUser;
	int nOutCardValue;

	int arrOutCard[MJ_MAX_OUTED_COUNT];
	int nOutedNums; // ��������������������ȥ���ܺ����ƣ� ����ˢ�½������
};


// �������
struct T_MsgActResultInfo
{
	unsigned short	usActFlags;				//��������
	int nHuIndex;	// �ڼ�����
	int	arrActUsers[MJ_MAX_PLAYER];	//���������߱�,�ڳԡ������ܡ��������У�����һ��ֻ��һ����ң�ֻ��byUsers[0]����Ҳ�ж����ɶ����ִ�У���һ�ڶ��졣
	int nActUserNums;				//��������������

	int	byFromUser;				//�����Ĵ����ߣ��ó�Աֻ���ں������ͣ��ԡ��������и�ֵ��Ч��FromUser�ɴ�CGPNode��ȡ��
	int	byHands[MJ_MAX_HAND_COUNT]; //ִ�ж������˱仯�������
	int iHandNums;		 //�仯�����������

	int nFromOutCardNums;	// ���³�����ҵĳ�������

	T_WeaveCardsValueItem tWeaveCardsValueItem;		//��������ƣ��ڴ˴�ȡ����
	int nWeaveCardsItemNum;

	bool bQGang;			//��������ܺ����ڴ˴�ȡ����
	int nQGangIdx;		//����һ���ܿؼ�

	// ִ�иܻ��������ķ����仯
	int arrScoreChanged[MJ_MAX_PLAYER];
	void calculateKongScoreChanged(int nToUser, const T_DeskConfig & tDeskConfig)
	{
		arrScoreChanged[nToUser] += tWeaveCardsValueItem.calculateScore(tDeskConfig);
		if (tWeaveCardsValueItem.byWeaveKind == 2)//�����벹��
		{
			for (int i = 0; i < tWeaveCardsValueItem.nNotHuUserNum; ++i)
			{
				if (i == nToUser)
				{
					continue;
				}
				arrScoreChanged[nToUser] += 2 * tDeskConfig.bBaseScore;
				arrScoreChanged[tWeaveCardsValueItem.arrNotHuUser[i]] -= 2 * tDeskConfig.bBaseScore;


			}
		}
		else if (tWeaveCardsValueItem.byWeaveKind == 3)
		{
			for (int i = 0; i < tWeaveCardsValueItem.nNotHuUserNum; ++i)
			{
				if (i == nToUser)
				{
					continue;
				}
				arrScoreChanged[nToUser] += tDeskConfig.bBaseScore;
				arrScoreChanged[tWeaveCardsValueItem.arrNotHuUser[i]] -= tDeskConfig.bBaseScore;


			}
		}
		else
		{
			arrScoreChanged[nToUser] += 2 * tDeskConfig.bBaseScore;
			arrScoreChanged[tWeaveCardsValueItem.byProvideUser] -= 2 * tDeskConfig.bBaseScore;

		}
	}

	T_MsgActResultInfo()
	{
		::memset(this, 0, sizeof(T_MsgActResultInfo));
	}


};

struct T_MsgChaJiao
{
	int arrChaJiaoUser[MJ_MAX_PLAYER];
	int arrNotTingUser[MJ_MAX_PLAYER];

	// �˸ܷ�
	int arrScoreTuiGang[MJ_MAX_PLAYER];

	// ��еķ����仯
	int arrScoreChanged[MJ_MAX_PLAYER];

};

struct T_HuLostItem
{
	int nToUser;
	T_UserHuInfo tUserHuInfoOther;

	T_HuLostItem()
	{

	}
	T_HuLostItem(int nToUser, const T_UserHuInfo & tUserHuInfo)
		: nToUser(nToUser)
		, tUserHuInfoOther(tUserHuInfo)
	{

	}

	int calculateScore(const T_DeskConfig & tDeskConfig)
	{
		tUserHuInfoOther.calculateScore(tDeskConfig);
		return tUserHuInfoOther.nFinalScore;
	}

	int score()
	{
		return tUserHuInfoOther.nFinalScore;
	}

};

struct T_KongLostItem
{
	unsigned char byWeaveKind;						//�������
	unsigned char nToUser;					//��Ӧ�û�
	//int nLostScore;
	T_KongLostItem()
	{

	}
	T_KongLostItem(unsigned char nToUser, const T_WeaveCardsValueItem & tWeaveCardsValueItem)
		: nToUser(nToUser)
		, byWeaveKind(tWeaveCardsValueItem.byWeaveKind)
	{

	}

	int calculateScore(const T_DeskConfig & tDeskConfig)
	{
		if (byWeaveKind == 2)
		{
			return 2 * tDeskConfig.bBaseScore;
		}
		else if (byWeaveKind == 3)
		{
			return tDeskConfig.bBaseScore;
		}
		else if (byWeaveKind == 4)
		{
			return 2 * tDeskConfig.bBaseScore;
		}
		else
		{
			return 0;
		}
	}
};

// ĳ����ҵı��ֵĽ����嵥
struct T_SettlementList
{
	// �����е÷�
	T_UserHuInfo tUserHuInfo;

	// ������ʧ��
	T_HuLostItem arrHuLostItem[MJ_MAX_PLAYER];
	int nHuLostItemNum;
	int nHuLostScore;

	// �ֵܷ÷�
	T_WeaveCardsValueItem arrWeaveCardsKongGot[MJ_MAX_WEAVE];
	int nKongNum;
	int nKongScore;

	// �ܷ�ʧ��
	T_KongLostItem arrKongLostItem[MJ_MAX_WEAVE * 4];
	int nKongLostNum;
	int nKongLostScore;

	// ʵ�����շ���
	int nRealScore;

	//ʵ�ʵ�ʧ�� ������ֵ
	int realScore()
	{
		return nRealScore;
	}

	void addHuLostItem(const T_HuLostItem & tHuLostItem)
	{
		arrHuLostItem[nHuLostItemNum++] = tHuLostItem;
	}

	void addKongLostItem(const T_KongLostItem & tKongLostItem)
	{
		arrKongLostItem[nKongLostNum++] = tKongLostItem;
	}

	void calculateScore(const T_DeskConfig & tDeskConfig)
	{
		tUserHuInfo.calculateScore(tDeskConfig);

		nHuLostScore = 0;
		for (int i = 0; i < nHuLostItemNum; ++i)
		{
			nHuLostScore += arrHuLostItem[i].calculateScore(tDeskConfig);
		}

		nKongScore = 0;
		for (int i = 0; i < nKongNum; ++i)
		{
			nKongScore += arrWeaveCardsKongGot[i].calculateScore(tDeskConfig);
		}

		nKongLostScore = 0;
		for (int i = 0; i < nKongLostNum; ++i)
		{
			nKongLostScore += arrKongLostItem[i].calculateScore(tDeskConfig);
		}

		nRealScore = tUserHuInfo.nFinalScore - nHuLostScore + nKongScore - nKongLostScore;
	}

};

// ���ֽ��
struct T_MsgResult
{
	bool bHuangZhuang;				//�Ƿ��ׯ

	T_UserHuInfo arrUserHuInfo[MJ_MAX_PLAYER]; // Ҳ���Է���createSettlementList�����б���

	T_WeaveCardsValueItem arrWeaveCardsKong[MJ_MAX_PLAYER][MJ_MAX_WEAVE];// Ҳ���Է���createSettlementList�����б���
	int arrKongNum[MJ_MAX_PLAYER];// Ҳ���Է���createSettlementList�����б���

	T_SettlementList arrSettlementList[MJ_MAX_PLAYER];

	T_MsgResult()
	{
		::memset(this, 0, sizeof(T_MsgResult));

	}

	void addHuSettlement(int nHuUser, const T_UserHuInfo & tUserHuInfo, int nPlayerNum)
	{
		arrSettlementList[nHuUser].tUserHuInfo = tUserHuInfo;
		if (tUserHuInfo.isZiMoType())
		{
			for (int i = 0; i < nPlayerNum; ++i)
			{
				if (i == nHuUser)
				{
					continue;
				}
				if (arrUserHuInfo[i].nHuIndex > arrUserHuInfo[nHuUser].nHuIndex)
				{
					T_HuLostItem tLostScoreItem(nHuUser, arrUserHuInfo[nHuUser]);
					arrSettlementList[i].addHuLostItem(tLostScoreItem);
				}
			}
		}
		else
		{
			T_HuLostItem tLostScoreItem(arrUserHuInfo[nHuUser].byFangPaoUser, arrUserHuInfo[nHuUser]);
			arrSettlementList[arrUserHuInfo[nHuUser].byFangPaoUser].addHuLostItem(tLostScoreItem);
		}
	}

	void addKongSettlementItem(int nToUser, const T_WeaveCardsValueItem & tWeaveCardsValueItem)
	{
		arrSettlementList[nToUser].arrWeaveCardsKongGot[arrSettlementList[nToUser].nKongNum] = tWeaveCardsValueItem;
		arrSettlementList[nToUser].nKongNum++;
		if (tWeaveCardsValueItem.byWeaveKind == 2 || tWeaveCardsValueItem.byWeaveKind == 3)//�����벹��
		{
			for (int i = 0; i < tWeaveCardsValueItem.nNotHuUserNum; ++i)
			{
				if (tWeaveCardsValueItem.arrNotHuUser[i] == nToUser)
				{
					continue;
				}
				T_KongLostItem tKongLostItem(nToUser, tWeaveCardsValueItem);
				arrSettlementList[tWeaveCardsValueItem.arrNotHuUser[i]].addKongLostItem(tKongLostItem);
			}
		}
		else
		{
			T_KongLostItem tKongLostItem(nToUser, tWeaveCardsValueItem);
			arrSettlementList[tWeaveCardsValueItem.byProvideUser].addKongLostItem(tKongLostItem);
		}
	}

	void createSettlementList(std::vector<int> vecHu, int nPlayerNum, const T_DeskConfig & tDeskConfig)
	{

		// �������뱻�������֮���������������ȸ�һ������ҽ��㣬�ٸ���������ҽ���,�������
		for (int i = 0; i < vecHu.size(); i++)
		{
			int nHuUser = vecHu.at(i);
			addHuSettlement(nHuUser, arrUserHuInfo[nHuUser], nPlayerNum);
		}

		// ���ɸ��嵥


		for (int i = 0; i < nPlayerNum; ++i)
		{
			for (int j = 0; j < arrKongNum[i]; ++j)
			{
				addKongSettlementItem(i, arrWeaveCardsKong[i][j]);
			}
		}

		// ����
		for (int i = 0; i < nPlayerNum; ++i)
		{
			arrSettlementList[i].calculateScore(tDeskConfig);
		}

	}
};

//////////////////////////////////////////////////////////////////////////
/* client --> server*/

struct T_ActRequest
{
	unsigned short	usActFlags;

	int nKongCardValue;
	int nKongSelectIndex;

	// ���⶯��
	T_MjActOutInfo tMjActOutInfo;

	T_ActRequest()
	{
		memset(this, 0, sizeof(T_ActRequest));
	}
};

#endif // __L_MJ_DEF_H__