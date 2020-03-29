#ifndef __L_MJ_DEF_H__
#define __L_MJ_DEF_H__
namespace LxMahjone
{

}

//////////////////////////////////////////////////////////////////////////
//�����궨��

#define KIND_ID						324									//��Ϸ I D

#define GAME_MJ_CARD_COUNT_MAX					144		// һ���齫�������							//��Ϸ����
#define GAME_MJ_PLAYER_MAX					4									//��Ϸ����
#define GAME_NAME					TEXT("�ɶ��齫")					//��Ϸ����
#define GAME_GENRE					(GAME_GENRE_SCORE|GAME_GENRE_MATCH|GAME_GENRE_GOLD)	//��Ϸ����

//��Ϸ״̬
#define GS_MJ_FREE					GS_FREE								//����״̬
#define GS_MJ_PLAY					(GS_PLAYING+1)						//��Ϸ״̬

#define INVALID_CHAIR				0xFFFF								//��Ч����
#define INVALID_TABLE				0xFFFF								//��Ч����

//��������
#define MAX_WEAVE					4									//������
#define MAX_INDEX					42									//�������
//#define MAX_INDEX					34									//�������
//#define MAX_INDEX					27									//�������
#define MAX_HAND_COUNT				14									//�����Ŀ
#define MJ_MAX_OUTED				30									//�����Ŀ
#define CARD_TOTAL_NUM				108									//�����
#define CARD_EMPTY					0


//��Ϸ�¼�ID����,(ȡֵ��Χ��10-49)����ʱ���Ĵ���

#define TIME_ID_Ready			9			// ѡ��
#define TIME_ID_CHOOSE_A_SEAT			10			// ѡ��
#define TIME_ID_CHOOSE_BANKER				11			// ѡׯ
#define TIME_ID_SHUFFLE_CARD				13			// ϵͳϴ�ƾ��¼�
#define TIME_ID_ROCK_DICE			12			// ϵͳȷ�����Ʒ������ƶ������¼�
#define TIME_ID_FETCH_HANDCARDS		14			//ϵͳ�������¼�
#define TIME_ID_TIAO_CARDS				15			//Ϊ����������¼�
#define TIME_ID_TBA         16          
//#define TIME_ID_BU_HUAS				16			//Ϊ����Ҳ����¼�
#define TIME_ID_GIVE_NEXT_TOKEN		17			//ϵͳΪĳ������·������¼�,��������ץ�Ƴ���
#define TIME_ID_GANG_HUA				18			//ϵͳΪĳ�����ִ�иܻ��¼�
#define TIME_ID_RESULT					19			//��Ϸ�����¼�
#define TIME_ID_ROUND_FINISH			20			//һ�غ����/�¼�

//dong add
#define TIME_ID_FETCH_BIRD				21			//ץ��
#define TIME_ID_PIAO					22			//��Ʈ

#define TIME_ID_NT_MORE                23          //��������

#define TIME_ID_AUTO_ACT				24			//�Զ�ִ�ж���
#define TIME_ID_AUTO_OUTCARD			25			//�Զ�����
#define TIME_ID_AUTO_ACT_NO			26			//û���й��Զ�ִ�ж���
#define TIME_ID_AUTO_OUTCARD_NO		27			//û���й��Զ�����



const int SCORE_BASE = 5;
const int MAX_FAN = 3;
const long COIN_MUL = 200;


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

//һ�����ƽ��
struct TMjCGPNode
{
	int nType; //�������
	int val[4]; //������ֵ
	int from;	//���Ƶ���Դ
	int  nIdx;  //���Ƶ�λ��
	TMjCGPNode()
	{
		memset(this, 0, sizeof(TMjCGPNode));
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
	EA_Wait		= 0x0001L,  //�ȴ������Ƚ�λ�����������ǰΪ0����������Ϊ1
	EA_Pass		= 0x0002L,  //��
	EA_Eat		= 0x0004L,  //��
	EA_Pong		= 0x0008L,	//����
	EA_Kong		= 0x0010L,	//����
	EA_Listen	= 0x0020L,
	EA_hu		= 0x0040L,	//��

	// ���⶯��
	EA_OutCard	= 0x0080L,

};

enum E_EatTypeFlags
{
	EA_EatLeft		= 0x01,	//���
	EA_EatCenter	= 0x02,	//�г�
	EA_EatRight		= 0x04, //�ҳ�
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

struct T_MjActKongInfo
{
	int arrKongSelect[MAX_WEAVE];	  //���Ըܵ�ѡ��
	int nKongSelectNums;				  //���Ը�ѡ������		
	int arrBuGangInPeng[MAX_WEAVE]; //�����ڽ�������һ��������
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

enum E_MjHuName
{
	// ��������
	EHN_Ping,			//ƽ��
	EHN_Pong,	             //������,�����
	EHN_Qing,	            //��һɫ
	EHN_All_19,	              //�۾�
	EHN_SevenPair,     	      //�߶�
	EHN_JinGouDiao,			// �𹳵�
	// ���ⷬ��
	EHN_258_Pong,	          //����
	EHN_Dai_19,	// ��19
	// ���Ϸ���
	EHN_QingPong,	          //����,���
	EHN_DragonSevenPair,	     //���߶�
	EHN_QingSevenPair,	      //���߶�
	EHN_QingDragonSevenPair,	  //�����߶�
	EHN_Qing_19,	          //���۾�

	EHN_HuNameMax
};

enum E_MjHuWay
{
	EHW_Unknown,  //δ֪���
	// ��ͨ��ʽ
	EHW_JiePao,  //����
	EHW_ZiMo,  //����
	// ���ⷽʽ
	EHW_DayHu,	              //���
	EHW_LandHu,	               //�غ�
	EHW_PensonHu,	              //�˺�
	EHW_QiangGang,  //����
	EHW_DianGangKai,  //��ܸ��Ͽ���
	EHW_AnGangKai,		//���ܸ��Ͽ���
	EHW_BuGangKai,		//���ܸ��Ͽ���
	EHW_GangChong,  //�ܳ�

};


enum E_MjSuanFenType
{
	// E_MjHuWay  + E_MjHuName + ��
};

// ��Һ�������
struct T_MjActHuInfo
{
	int nHuNameNums;	//������������		
	E_MjHuName arrHuName[EHN_HuNameMax]; //���ͱ�
	E_MjHuWay eMjHuWay; //���������
	int  nHuGangIdx;	//���ܿؼ���λ��(��������)
};

// ������Ϣ
struct T_MjActOutInfo
{
	int nOutCardUser;
	int nOutCardValue;

	int m_nOutedNums; // ��������������������ȥ���ܺ����ƣ� ����ˢ�½������
};

// ��Ҷ�����Ϣ
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
		: usActFlags(0)
	{

	}

	void clear()
	{
		memset(this, 0, sizeof(T_MjActInfo));
	}

};





//////////////////////////////////////////////////////////////////////////
// �齫����ͨ�����ݽṹ����

// ����������
struct T_MsgDealCards
{
	int arrCardHand[4][14];
	int nPlayerCount;
	int arrMjCardsPair[GAME_MJ_CARD_COUNT_MAX];
	int nMjNumAllocation;
	
};

// ָ�ɻ�û�, 
struct T_MsgAppointActiveUser
{
	T_ActiveUser tActiveUser;
	T_MjActInfo tMjActInfo;
	int arrMjCardsPair[GAME_MJ_CARD_COUNT_MAX]; // һ���齫
	int nMjNumAllocation;	// һ���齫������
}; 



// ��Ӧ��û�
struct T_MsgResponseToActiveUser
{
	int nActiveID;		// ����id
	int nResponseID[4];	// ��Ӧ�����id�������ǻ����Լ�

	// ��Ӧ��ҿ��Ծ߱���Щ������Ϊ
	T_MjActInfo tMjActInfo[4];

};



// �������
struct T_MsgActResultInfo
{
	unsigned short	usActFlags;				//��������
	int	arrActUsers[4];	//���������߱�,�ڳԡ������ܡ��������У�����һ��ֻ��һ����ң�ֻ��byUsers[0]����Ҳ�ж����ɶ����ִ�У���һ�ڶ��졣
	int nActUserNums;				//��������������

	int	byFromUser;				//�����Ĵ����ߣ��ó�Աֻ���ں������ͣ��ԡ��������и�ֵ��Ч��FromUser�ɴ�CGPNode��ȡ��
	int	byHands[14]; //ִ�ж������˱仯�������
	int iHandNums;		 //�仯�����������

	TMjCGPNode CGPNode;		//��������ƣ��ڴ˴�ȡ����

	bool bQGang;			//��������ܺ����ڴ˴�ȡ����
	int nQGangIdx;		//����һ���ܿؼ�

	T_MsgActResultInfo()
	{
		::memset(this, 0, sizeof(T_MsgActResultInfo));
	}
};


/* client --> server*/
struct T_ActRequest
{
	unsigned short	usActFlags;

	int nKongCardValue;
	// ���⶯��
	T_MjActOutInfo tMjActOutInfo;
};

#endif // __L_MJ_DEF_H__