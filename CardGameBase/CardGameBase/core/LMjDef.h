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
//#define MAX_INDEX					34									//�������
#define MAX_INDEX					27									//�������
#define MAX_HAND_COUNT				14									//�����Ŀ
#define MJ_MAX_OUTED				30									//�����Ŀ
#define CARD_TOTAL_NUM				108									//�����
#define PLAYER_NUM		    4
#define HOLD_CARD_NUM		13
#define ALL_TYPE_NUM		27		// ���������Ƶĸ���

//��Ϸ�¼�ID����,(ȡֵ��Χ��10-49)����ʱ���Ĵ���

#define TIME_ID_CHOOSE_A_SEAT			10			// ѡ��
#define TIME_ID_CHOOSE_BANKER				11			// ѡׯ
#define TIME_ID_ROCK_DICE			12			// ϵͳȷ�����Ʒ������ƶ������¼�
#define TIME_ID_SHUFFLE_CARD				13			// ϵͳϴ�ƾ��¼�
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

#define TIME_ID_FETCH_GUI              28          //ץ����



const int SCORE_BASE = 5;
const int MAX_FAN = 3;
const long COIN_MUL = 200;

// ��ֵȨ�ض���
#define SCORE_Hu 1000            //
#define SCORE_Kong 500            //��Ӱ���Ƶ����
#define SCORE_Triplet 400        //����
#define SCORE_Sequence  300      //˳��
#define SCORE_Pair 120           //����
#define SCORE_Doors 100          //����
#define SCORE_DoorsEdge  80      //��Ե����
#define SCORE_Single_34567 3           //����34567
#define SCORE_Single_28 2          //����28
#define SCORE_Single_19 0           //����19

#define SCORE_Hu 1000            //
#define SCORE_Kong 500            //��Ӱ���Ƶ����
#define SCORE_Triplet 400        //���� ���-���� �̶���-����
#define SCORE_Sequence  300      //˳��
#define SCORE_Pair_Only 120      //����ȫ����ֻ��һ�����
#define SCORE_Door_TwoHead 100          //���ӣ���ͷ��
//#define SCORE_Pair 90           //����
#define SCORE_Door_Middle  80       //����
#define SCORE_Door_Edge  70      //��Ե����
//#define SCORE_Single 3           //����34567
#define SCORE_Single_Middle 10          //����2-8
#define SCORE_Single_Edge 6           //����1,9
#define SCORE_Single_R_TwoHead 3
#define SCORE_Single_R_Middle 2
#define SCORE_Single_R_Edge 1
#define SCORE_Badly_Zero 0             //��������ƺܲ�
#define SCORE_TBA 0                    //ȱ

//�齫�ƶ���
#define CARD_BACK_VALUE 0x31    //�Ʊ���ֵ
#define CARD_STAND_VALUE 0x99      //����ֵ
#define CARD_EMPTY	0x00				//��Ч��ֵ,����ռλ

#define SPACE_WEAVE_MY		10		//ÿ�������֮��ļ��-�Լ�
#define SPACE_WEAVE_OTHER	4		//ÿ�������֮��ļ��-����

//////////////////////////////////////////////////////////////////////////
//�߼�����

#define	MASK_COLOR					0xF0								//��ɫ����
#define	MASK_VALUE					0x0F								//��ֵ����


//////////////////////////////////////////////////////////////////////////
//��������
#define ACTION_NULL					0x00								//û��
#define ACTION_LEFT					0x01								//���
#define ACTION_CENTER					0x02								//�г�
#define ACTION_RIGHT					0x04								//�ҳ�
#define ACTION_PENG					0x08								//����
#define ACTION_GANG					0x10								//����
#define ACTION_LISTEN					0x20								//����
#define ACTION_CHI_HU					0x40								//�Ժ�
#define ACTION_ZI_MO					0x80								//����

//////////////////////////////////////////////////////////////////////////
//���ƶ���
#define HU_NULL					0x00										//�Ǻ�����
#define CHK_CHI_HU					0x01										//������
#define CHR_QIANG_GANG				0x00000001									//����
#define CHR_GANG_SHANG_PAO			0x00000002									//������
#define CHR_GANG_KAI				0x00000004									//���ϻ�
#define CHR_TIAN_HU					0x00000008									//���
#define CHR_DI_HU					0x00000010									//�غ�
#define CHR_DA_DUI_ZI				0x00000020									//�����
#define CHR_QING_YI_SE				0x00000040									//��һɫ
#define CHR_QI_XIAO_DUI				0x00000080									//���߶�
#define CHR_DAI_YAO					0x00000100									//����
#define CHR_JIANG_DUI				0x00000200									//����
#define CHR_SHU_FAN					0x00000400									//�ط�
#define CHR_QING_DUI				0x00000800									//���
#define CHR_LONG_QI_DUI				0x00001000									//���߶�
#define CHR_QING_QI_DUI				0x00002000									//���߶�
#define CHR_QING_YAO_JIU			0x00004000									//���۾�
#define CHR_QING_LONG_QI_DUI		0x00008000									//�����߶�

enum E_HuType
{
	EH_Fart,	              //ƨ��		1��

	EH_Pong,	             //������		2��

	EH_Qing,	            //��һɫ		3��

	EH_All_19,	              //�۾�		3��

	EH_SevenPair,     	      //�߶�		3��

	EH_PongQing,	          //����		4��

	EH_Pong_258,	          //����		4��

	EH_DragonSevenPair,	     //���߶�		4��

	EH_QingSevenPair,	      //���߶�		5��

	EH_QingAll_19,	          //���۾�		5��

	EH_QingDragonSevenPair,	  //�����߶�	6��

	EH_DayHu,	              //���		6��

	EH_LandHu,	               //�غ�		6��

	EH_PensonHu	              //�˺�		6��
};

enum E_ExtraHuType
{

	EE_SelfDrawn,	 //�����ӷ�(�ط��ӵ�)	1��

	EE_Kong_hua,	 //�ܿ�		1��

	EE_Kong_pao,	 //����		1��

	EE_Kong_qiang,	 //����		1��

	EE_Kong,	    //�����ܣ�	1��
};




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



enum E_Sex
{
	S_Boy,
	S_Girl
};

enum E_CardContentType
{
	E_Wind,            //����
	E_Wrigley,         //����
	E_Flower,           //����
	E_OrderNum          //������
};

enum E_OrderNumCardType
{
	EO_Circle,         //Ͳ�� 0-8
	EO_Character,      //���� 9-17
	EO_Bamboo,          //���� 18-26
	EO_Invalid = 255    //û����������
};

enum E_CardColorType
{
	EC_Circle,         //Ͳ�� 0-8
	EC_Character,      //���� 9-17
	EC_Bamboo,          //���� 18-26
	EC_Wind,			// ����
	EC_RedMiddle,			// ����
	EC_Fortune,				// ��
	EC_WhiteBoard,			// �װ�
	EC_Invalid = 255    //û����������
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






enum E_HandCardRelationType
{
	EHC_Group,      //һ��˳�ӡ����ӻ����
	EHC_OneToOne,     //��һ���γ���
	EHC_Single        //���ƣ�����Χ���κ�2�Ŷ������γ���
};

enum E_KongType
{
	EK_ConcealedKong,              //����-����
	EK_ExposedKong_Ba,              //�η�-�͸�
	EK_ExposedKong_Dian             //�η�-���
};

//�Ƿ����γ�˳��
enum E_SequenceType
{
	ES_MiddleCard,                   //����-����-Ƕ��
	ES_LeftEdgeCard,                  //����-Ŀ����������
	ES_RightEdgeCard                  //����-Ŀ���������ҽϴ�
};

//�Ƿ����γ�����(����)
enum E_DoorType
{
	ED_DoorTwoHead,                //��ͷ�� 23
	ED_DoorMiddle,                 //Ƕ�� 02,35,68
	ED_DoorEdge                    //���� 01,78
};

enum E_PlayerState
{
	P_Active,
	p_unActive
};








enum E_CardType
{
	//����
	CT_HAND_North,
	CT_HAND_East,
	CT_HAND_South,
	CT_HAND_West,

	//����
	CT_STAND_P,
	CT_STAND_S,
	CT_STAND_D,
	CT_STAND_X,

	//�������ϵĵ���
	CT_STAND_B_P,
	CT_STAND_B_S,
	CT_STAND_B_D,
	CT_STAND_B_X,

	//������
	CT_TABLE_P,
	CT_TABLE_S,
	CT_TABLE_D,
	CT_TABLE_X,

	//����
	CT_MO_P,
	CT_MO_S,
	CT_MO_D,
	CT_MO_X,

	//��ǽ
	CT_WALL_North,
	CT_WALL_East,
	CT_WALL_South,
	CT_WALL_West,

	//����
	CT_HU_North,
	CT_HU_East,
	CT_HU_South,
	CT_HU_Wes,

	//�����
	CT_Out
};


//���״̬
enum E_PlayerStatus
{
	EPS_Waiting,
	EPS_ThinkOutCard,
	EPS_Choosing,
	EPS_Hu,
};



#endif // __L_MJ_DEF_H__