#ifndef UPGRADEMESSAGE_MOBAN_H
#define UPGRADEMESSAGE_MOBAN_H
#include "../Server/MjCommonDef.h"
#define TAG_INIT(tagName) \
	tagName(){Clear();} \
	void Clear()


#define PLAY_COUNT						4				//��Ϸ����
#define MAX_NIAO_COUNT					12				//���׽������
#define NAME_ID                         40026400        ///��Ϸid
#define GET_SERVER_INI_NAME(str)		sprintf(str,"%s%d_s.ini",CINIFile::GetAppPath(),NAME_ID);
//******************************************************************************************
//���ݰ���������ʶ(��Ϣ�Ĵ���)
//******************************************************************************************
#define ASS_GM_AGREE_GAME				1				//ͬ����Ϸ
#define ASS_GM_GAME_STATION				2				//��Ϸ״̬

#define ASS_GM_ROOM_DISSMISS			10				//�����ɢ

#define MDM_GM_GAME_NOTIFY              180
#define ASS_UG_USER_SET					50				//�û�������Ϸ
#define ASS_UG_USER_SET_CHANGE			51				//������ø���

#define ASS_GAME_BEGIN					120				//�齫��Ϸ��ʼ
#define ASS_AUTO_REQ					126				//���Կͻ��˵��й�����
#define ASS_AUTO_INFO					127				//�������ĳ����й���Ϣ

#define ASS_FETCH_GUI_INFO        128         //�������·�������Ϣ

#define ASS_MAKE_NT_INFO				132				//�������·���ׯ����Ϣ
#define ASS_FETCH_METHOD_INFO			133				//�������·����Ʒ���
//#define ASS_FETCH_METHOD_FINISH			135				//�ͻ��˱�������������Ƶ㻭�������
#define ASS_FETCH_HANDCARDS				136				//�������·���������
#define ASS_TIAO_CARDS					137				//�������·���������
#define ASS_BU_HUAS						138				//������Ϊ����Ҳ���������
#define ASS_BU_HUAS_FINISH				139				//�ͻ��˷�����д�ɲ���������Ϣ
#define ASS_TOKEN						140				//������Ϊĳ�淢����
#define ASS_OUT_CARD_REQ				141				//�ͻ��˵ĳ�������
#define ASS_OUT_CARD_INFO				142				//����������ĳ��ҵĳ��ƽ��
#define ASS_ACT_NOTIFY					143				//������֪ͨĳ����ж���(����������ȶ���)
#define ASS_ACT_REQ						144				//�ͻ�����������������ĳ����������
#define ASS_ACT_INFO					145				//����������ĳ��Ҷ���ִ�н��
#define ASS_ANGANG_RESULT				146				//���ܵļ�ʱ������Ϣ

#define ASS_ROUND_FINISH				149				//���Է���˻غϽ�������Ϣ
#define ASS_RESULT						150				//�غ����

//-----------------------huansanzhang 160---------------
#define ASS_CHANGE_CARD					160				//������
#define ASS_CHANGE_CARD_RESP			161				//ĳһ�������ŵ�����
#define ASS_CHANGE_CARD_FINISH			162				//��������ɣ��������Ƶ����ݷ��͸��ͻ�
#define ASS_DING_QUE					163				//��ȱ
#define ASS_DING_QUE_RESP				164				//��ȱ�������
#define ASS_JI_INFO						165				//���ͼ�����Ϣ���ͻ���

//dong add
#define ASS_PAIJU_INFO					176				//�������ͻ��˷����ƾ���Ϣ
#define ASS_GAME_OVER					177				//�������ͻ��˷����ƾֽ��������ɢ�����Լ�����Ϸ
#define ASS_ROOM_CONFIG					178				//�������ͻ��˷��ͷ���������Ϣ
#define ASS_ASSIGN_MAKENT_FINISH		179				//�ͻ�������������Ͳ����궨ׯ����
#define	ASS_GAME_REBIND					180				//��������
//----------------------------------------------------------------------------
#define ASS_PIAO_NOTIFY					170				//������֪ͨ�ͻ��˼�Ʈ
#define ASS_PIAO_REQ					171				//�ͻ��˱���������Ƿ��Ʈ
#define ASS_PIAO_RESP					172				//֪ͨ�ͻ���Ʈ�Ľ��	
#define ASS_NIAOCARDS_INFO				173				//֪ͨ�ͻ���������Ϣ

//�쳣����Ϣ
#define ASS_NO_PLAYER					155				//û����ҽ�����Ϸ
#define ASS_AGREE						156				//����Ƿ�ͬ�����Ϣ
#define ASS_NOT_ENOUGH_MONEY			158				//��ҽ�Ҳ������ʾ
#define ASS_FORCE_QUIT_REQ				160				//���ǿ������
#define ASS_QUIT_INFO					161				//�������ͻ��˱����Ǹ�����˳�����Ϣ


//******************************************************************************************

//************************************************************************************************************

//��Ϸ�¼�ID����,(ȡֵ��Χ��10-49)����ʱ���Ĵ���

#define TIME_ASSIGN_DIRECT			10			//����ҷ����¼�
#define TIME_MAKE_NT				11			//ϵͳѡׯ�¼�
#define TIME_FETCH_METHOD			12			//ϵͳȷ�����Ʒ������ƶ������¼�
#define TIME_RAND_CARD				13			//ϵͳϴ�ƾ��¼�
#define TIME_FETCH_HANDCARDS		14			//ϵͳ�������¼�
#define TIME_TIAO_CARDS				15			//Ϊ����������¼�
#define TIME_BU_HUAS				16			//Ϊ����Ҳ����¼�
#define TIME_GIVE_NEXT_TOKEN		17			//ϵͳΪĳ������·������¼�,��������ץ�Ƴ���
#define TIME_GANG_HUA				18			//ϵͳΪĳ�����ִ�иܻ��¼�
#define TIME_RESULT					19			//��Ϸ�����¼�
#define TIME_ROUND_FINISH			20			//һ�غ����/�¼�


//dong add
#define TIME_FETCH_BIRD				21			//ץ��
#define TIME_PIAO					22			//��Ʈ

#define TIME_NT_MORE                23          //��������

#define TIME_AUTO_ACT				24			//�Զ�ִ�ж���
#define TIME_AUTO_OUTCARD			25			//�Զ�����
#define TIME_AUTO_ACT_NO			26			//û���й��Զ�ִ�ж���
#define TIME_AUTO_OUTCARD_NO		27			//û���й��Զ�����

#define TIME_FETCH_GUI              28          //ץ����
//��Ϸ������־����
#define GF_NORMAL					10			//��Ϸ��������
#define GF_SALE						11			//��Ϸ��ȫ����
//****************************************************************************************


//******************************************************************************************
// ��Ϸ״̬����
//******************************************************************************************

#define GS_WAIT_SETGAME					0	//�ȴ���������״̬
#define GS_WAIT_ARGEE					1	//�ȴ�ͬ������
#define GS_CONFIG_DESK					20	//���õ�ע״̬
#define GS_AGREE_NOTE					21	//ͬ���ע״̬
#define GS_ASSIGN_DIRECT				22	//�������ҷ�λ��״̬
#define GS_FETCH_HANDCARDS				23	//����ҷ�λȡ������״̬
#define GS_TIAO_CARDS					24	//���������״̬
#define GS_BU_HUA						25	//����ҽ��в���״̬
#define GS_PLAYING						26	//����ҽ��д���״̬

#define GS_WAIT_PIAO                    27  //Ư��״̬
#define GS_WAIT_NEXT_ROUND				28	//�ȴ���һ�غ�״̬
#define GS_PAIJU_FINISH					29	//�ƾֽ���

//******************************************************************************************

// ¼����¼�ID
enum Video_Event
{
	Video_Event_begin				= ASS_GAME_BEGIN ,			//��Ϸ��ʼ
	Video_Event_select_banker		= ASS_MAKE_NT_INFO ,		//ѡ��ׯ��
	Video_Event_select_Piao			= ASS_PIAO_RESP ,			//ѡƱ
	Video_Event_send_card			= ASS_FETCH_HANDCARDS ,		//����
	Video_Event_tiao_card			= ASS_TIAO_CARDS ,			//ׯ�����ƣ����ĵ�һ���ƣ�
	Video_Event_mo_card				= ASS_TOKEN ,				//����
	Video_Event_out_card			= ASS_OUT_CARD_INFO ,		//����
	Video_Event_action				= ASS_ACT_INFO ,			//������
	Video_Event_niao				= ASS_NIAOCARDS_INFO,		//������Ϣ		
	Video_Event_resoult				= ASS_RESULT,				//����
	Video_Event_gui					= ASS_FETCH_GUI_INFO,	//������Ϣ
};



//�й���������
struct TDeskCfg
{

	BYTE    byWaitAgree;	 //�ȴ�ͬ���ʱ��
	BYTE    byOutCard;	   //����˼��ʱ��
	BYTE    byAct;		    //����˼��ʱ�䣬�����ܺ�
	BYTE	byCheaterOpen;		//�Ƿ���������
	BYTE	byNextRound;	 //��һ�غϵȴ�ʱ�䣬�ǽ��������ʾ
	int		nFirstNtMode;	//����ׯ�ҵ�ѡ��ģʽ  1---���ׯ�ң�2---����λ��ׯ��3---������ׯ
	int		nNtMode;		//ȷ��ׯ��ģʽ    1--������Ҽ�����ׯ��2--�ϴ�ׯ�ҵ�˳λ��ׯ


	TDeskCfg()
	{
		::memset(this,0,sizeof(TDeskCfg));
	}
	
};


//����������ҷ��͵Ķ��������
struct TActInfo 
{
	USHORT	usActFlag;				//��������
	BYTE	byUsers[PLAY_COUNT];	//���������߱�,�ڳԡ������ܡ��������У�����һ��ֻ��һ����ң�ֻ��byUsers[0]����Ҳ�ж����ɶ����ִ�У���һ�ڶ��졣
	int		iUserNums;				//���������߱���
	BYTE	byFromUser;				//�����Ĵ����ߣ��ó�Աֻ���ں������ͣ��ԡ��������и�ֵ��Ч��FromUser�ɴ�CGPNode��ȡ��

	int	byHands[HAND_MAX_NUM_MJ]; //ִ�ж������˱仯�������
	int		iHandNums;		 //�仯�����������

	TMjCGPNode CGPNode;		//��������ƣ��ڴ˴�ȡ����

	bool    bQGang;			//��������ܺ����ڴ˴�ȡ����
	int		nQGangIdx;		//����һ���ܿؼ�

	TActInfo()
	{
		::memset(this,0,sizeof(TActInfo));
	}
};

//һ�غ����
struct TRoundFinish 
{
	int		iRoundTimes;						//��ǰ����
	int		iAllRoundTimes;						//�ܵľ���
	int		iZimoTimes[PLAY_COUNT];				//��������
	int		iJiePaoTimes[PLAY_COUNT];			//���ڴ���
	int		iFangPaoTimes[PLAY_COUNT];			//���ڴ���
	int		iMingGangTimes[PLAY_COUNT];			//���ܴ���
	int		iAnGangTimes[PLAY_COUNT];			//���ܴ���
	int     iBuGangTimes[PLAY_COUNT];			//���ܴ���
	int     iYouJinTimes[PLAY_COUNT];			//�ν����
	int		iLiuJuTimes[PLAY_COUNT];			//���ִ���
	int		iGangKaiTimes[PLAY_COUNT];			//�ܿ�����
	int		iTotalScore[PLAY_COUNT];			//����ܷ�
	TAG_INIT(TRoundFinish)
	{
		::memset(this,0,sizeof(TRoundFinish));

	}
};

//����ṹ��
struct TResult
{
	bool m_bHuangZhuang;				//�Ƿ��ׯ
	bool bHu[PLAY_COUNT];				//���Ƶ������true
	int  iHuNums;						//������ҵ�����
	BYTE byFangPaoUser;					//�������
	BYTE byHuUser;					   //�������
	char szHuRepo[PLAY_COUNT][128];		//����ҵĺ��Ʊ���

	emHuSpecialWay nHuSpecWay[PLAY_COUNT][2];	//�������ʽ
	emHuType nHuTypes[PLAY_COUNT][MJ_MAX_HUTYPE]; //��������ҵĺ������ͱ�
	int iHuTypeNums[PLAY_COUNT];				//��������ҵĺ������ͱ�ĳ���

	int		iHuPoint[PLAY_COUNT];		//����Һ��Ƶĵ�ʧ��
	int		iPiaoPoint[PLAY_COUNT];		//�����Ʈ�ĵ�ʧ��
	int		iNiaoPoint[PLAY_COUNT];		//����ҵ����
	int		iGangPoint[PLAY_COUNT];		//����ҵĸܷ�
	int		iNTPoint[PLAY_COUNT];		//�����ׯ�е÷�
	__int64	iTotalPoint[PLAY_COUNT];	//����ҵ��ܷ�
	__int64	iRealMoney[PLAY_COUNT];		//�����ʵ�ʵ�ʧ��
	int		iChPoint;					//�������䶯�ķ���

	int		iLeftMoney[PLAY_COUNT];		//���������ʣ���Ǯ
	int		iPoolPoint[PLAY_COUNT];		//����ҵķֳ�

	TAG_INIT(TResult)
	{
		::memset(this,0,sizeof(TResult));
		byFangPaoUser = 255;
		byHuUser = 255;
	}
};

//���������¼�ͨѶ��,���ڿ������Ʒ��ŵķ�ʽ�����������ܲ���ץ�ơ�ץ�Ʒ����

struct TEvTokenParm
{
	//��������ܷ�ץ��
	bool bCanFetch;
	//����������,255Ϊ��ǰ���Ƶ��ߵ��¼�
	int byNextUser;
};


//����غϽ����¼�ͨѶ��\
���ڱ����þ��Ƿ����ׯ��
struct TEvRoundFiniParm
{
	//�Ƿ��ׯ
	bool bHuang;
};


//������֪ͨ����ж���(�������ܵ�)�����ݰ�
struct TActNotify
{
	TMjActInfo info; //��ҵĶ�����Ϣ

	TAG_INIT(TActNotify)
	{
		::memset(this,0,sizeof(TActNotify));
	}
};

struct TOutCard
{
	enum emType
	{
		UNKNOWN		= 0,	//δ֪����
		REQUEST		= 1,	//�������
		INFO		= 2,	//������Ϣ
	};

	emType nType; //���������

	int byCard; //������
	BYTE byUser; //���Ƶ���� ����INFO����

	int byCards[HAND_MAX_NUM_MJ];
	int  iHandNums;

	TAG_INIT(TOutCard)
	{
		::memset(this,0,sizeof(TOutCard));
	}
};

#endif