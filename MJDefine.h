#ifndef __MJDEFINE_H__
#define __MJDEFINE_H__


#define CARD_TOTAL_NUM      108
#define PLAYER_NUM		    4
#define HOLD_CARD_NUM		13
#define ALL_TYPE_NUM		27		// ���������Ƶĸ���
#define TimeTBA         20          
#define TimeSelect		20				//���ѡ���ʱ�䣨�룩
#define TimeSezi		15				//ɫ�Ӷ�����ʱ�䣨����
#define TimeRun			500			//������Ҵ����Ƶ�ʱ�䣨���룩

//enum E_ExtraHuType
//{
//
//	EE_SelfDrawn,	 //�����ӷ�(�ط��ӵ�)	1��
//
//	EE_Kong_hua,	 //�ܿ�		1��
//
//	EE_Kong_pao,	 //����		1��
//
//	EE_Kong_qiang,	 //����		1��
//
//	EE_Kong,	    //�����ܣ�	1��
//};

//���˺������Ƽӷ�����ļӷ�����
#define HU_SELF_DRAWN       0x01
#define HU_KONG_HUA         0x02
#define HU_KONG_PAO         0x04
#define HU_KONG_QIANG       0x08
#define HU_GOLD_HOOK        0x10        //�𹳵�
#define HU_HAI_DI_LAO		0x20		//������
#define HU_HAI_DI_PAO		0x40		//������

const int SCORE_BASE = 5;
const int MAX_FAN = 3;
const long COIN_MUL = 200;

//�齫��ȡ�Ʒ���-˳ʱ��-������-clockwise
//���ӷ���������ƴ���˳��-��ʱ��-������-antiClockWise 
enum E_WallDirection
{
	EW_North,
	EW_East,
	EW_South,
	EW_West,
	EP_InValid
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


enum E_Sex
{
	S_Boy,
	S_Girl
};

enum E_CardType
{
	//����
	CT_HAND_North,
	CT_HAND_East,
	CT_HAND_South,
	CT_HAND_West,

	//��ǽ
	CT_WALL_North,
	CT_WALL_East,
	CT_WALL_South,
	CT_WALL_West,

	//����
	CT_HU_North,
	CT_HU_East,
	CT_HU_South,
	CT_HU_West,

	//�Ƴ�(����˵���)
	CT_POLL_North,
	CT_POLL_East,
	CT_POLL_South,
	CT_POLL_West,
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


//���״̬
enum E_PlayerStatus
{
	EPS_DrawACard, //����
	EPS_ThinkOutCard, //˼������
	EPS_OutCard, //�ճ�����
	EPS_Waiting, //�ȴ������˳���
};

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






#endif // __MJDEFINE_H__