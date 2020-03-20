// ***************************************************************
//  SoundsDef   version:  1.0   ��  date: 2/09/2017
//  -------------------------------------------------------------

//	�齫��Ϸ�������ݶ���,�ʺϴ󲿷��齫��Ϸ����ʹ��\

//  -------------------------------------------------------------
//  Copyright (C) 2011 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************
#include <Windows.h>
#ifndef MJCommon_DEF_HEAD_FILE

//�ú궨������Ƿ����齫�����߼�
//#define IS_HAVECHI						1					//�齫�Ƿ��г�
#define IS_TONGPAO						1					//�Ƿ���ͨ���淨
#define MJCommon_DEF_HEAD_FILE

#define MJ_MAX_CARDS					108					//�齫����
#define MJ_MAX_CARDS_ZHONG				136					//�齫����(������)
#define HAND_MAX_NUM_MJ					14					//�����������
#define MJ_MAX_OUTED					68					//���ĳ���������2��3��4�˵�ʱ��Ҫȡ���
#define MJ_MAX_CGP						12					//����������
#define MJ_MAX_HUTYPE					11					//��������
#define MJ_MAX_LOUHU					15					//���©������
#define MJ_MAX_BAO_CARD					1					//�����Ƶ�����
#define INVALID_MJ 0
#define TAG_INIT(tagName) \
	tagName(){Clear();} \
	void Clear()
typedef enum
{
	MJK_WAN = 0, 
	MJK_TIAO = 10, 
	MJK_TONG = 20, 
	MJK_FENG = 30, 
	MJK_ZI = 40, 
	MJK_END
} E_MAHJONG_KIND;

typedef enum
{
	LABEL_START = 0,
	LABEL_1 = 1,
	LABEL_2 = 2,
	LABEL_3 = 3,
	LABEL_4 = 4,
	LABEL_5 = 5,
	LABEL_6 = 6,
	LABEL_7 = 7,
	LABEL_8 = 8,
	LABEL_9 = 9,
	LABEL_PAI_END,

	LABEL_DONG = 1,
	LABEL_NAN = 2,
	LABEL_XI = 3,
	LABEL_BEI = 4,
	LABEL_ZHONG = 5,
	LABEL_FA = 6,
	LABEL_BAI = 7,
	LABEL_FENG_END,
	LABEL_HUA_END

} E_MAHJONG_LABEL;

//��Ϸ�ж����ı�ʶ��һ����˵�������ȼ���Խ�ߣ�����λԽ��
typedef enum
{
	ACT_GUO  = 0x0002L,  //��
	ACT_CHI  = 0x0004L,  //��
	ACT_PENG = 0x0008L,  //��
	ACT_GANG = 0x0010L,  //��
	ACT_TING = 0x0020L,  //��
	ACT_HU   = 0x1000L,  //��

	ACT_WAIT = 0x0001L  //�ȴ������Ƚ�λ�����������ǰΪ0��\
	��������Ϊ1
}E_MAHJONG_ACTION;


//�ö�����λ�������أ���ʾ�Ƿ���Ҫĳһ�����͵���
//00000000
enum emRandCardType
{
	Rand_hua =	1,		//00000001��һλ��ʾ�Ƿ�Ҫ����
	Rand_wan = 2,		//00000010�ڶ�λ��ʾ�Ƿ���Ҫ����
	Rand_tong = 4,		//00000100����λ��ʾ�Ƿ���Ҫ��Ͳ��
	Rand_tiao = 8,		//00001000����λ��ʾ�Ƿ���Ҫ������
	Rand_feng = 16,		//00010000����λ��ʾ�Ƿ���Ҫ����������
	Rand_zhong = 32,	//00100000��6λ��ʾ�Ƿ���Ҫ�����С���
	Rand_fabai = 64,	//01000000����λ��ʾ�Ƿ���Ҫ�����ƺͰװ�
};

enum emGangType
{
	gt_Unknown	= 0,	//δ֪����
	gt_MingGang	= 1,	//����
	gt_BuGang	= 2,	//����
	gt_AnGang	= 3,	//����
};

enum emHuType
{
	ht_Unknown	= 0,	//δ֪���
	ht_PingHu=1,
	ht_QiDui=2,			//�߶�
	ht_ShiSanYao=3,     //ʮ����
	ht_QiangGangHu=4,	
	ht_YouJin=5,        //�ν�
	ht_SanJinDao=6,		//����,
	ht_GangKai=7,		//�ܿ�
};


enum emSuanFenType
{
	sf_Unknown=-1,
	sf_PuTongZiMo=0,
	sf_YouJinZiMo=1,
	sf_QiangGangHu=2,
	sf_DianGangKai=3,
	sf_AnGangKai=4,
	sf_BuGangKai=5,
	

};


enum emHuSpecialWay
{
	hsw_Unknown		= 0,  //δ֪���
	hsw_TianHu		= 1,  //���
	hsw_DiHu		= 2,  //�غ�
	hsw_QiangGang	= 3,  //����
	hsw_DianGangKai		= 4,  //��ܸ��Ͽ���
	hsw_AnGangKai=5,		//���ܸ��Ͽ���
	hsw_BuGangKai=6,		//���ܸ��Ͽ���
	hsw_GangChong	= 7,  //�ܳ�

};

//�齫��Ϸ�����Ƕ���
enum MjResult
{
	Res_Unknown	= 0,  //δ֪���
	Res_FangPao	= 1,  //����
	Res_JiePao	= 2,  //����
	Res_ZiMo	= 3  //����
};

//��һ�������ׯ�ķ�ʽ
enum FirstNtType
{
	RAND_NT =1,	//���ѡ��һ��ׯ��
	MASTER_NT = 2,//������ׯ
};

enum NtType
{
	TURN_NT = 1,//��ׯģʽ
	HU_NT = 2,//������ҵ�ׯ
};

enum emType
{
	UNKNOWN		= 0,	//δ֪����
	Chi			= 1,	//��
	Peng		= 2,	//��
	MingGang	= 3,	//����
	BuGang		= 4,	//����
	AnGang		= 5,	//����
	SPECIAL_GANG = 6,	//����
	Liang        = 7,	 //���Ķ���

};
//���ƵĶ�����־
namespace MjChiFlag 
{
	const USHORT usT	= 1;			//��ͷ
	const USHORT usZ	= 2;			//����
	const USHORT usW	= 4;			//��β
	const USHORT usTZ	= 3;			//��ͷ ����
	const USHORT usTW	= 5;			//��ͷ ��β
	const USHORT usZW	= 6;			//���� ��β
	const USHORT usTZW	= 7;			//��ͷ ���� ��β
};

//��ҵĶ�����ϸ��Ϣ�ṹ////////////////////////////////////////////////////////////////////
struct TMjActInfo
{
	USHORT usFlags;	//������ʶ

	/*-�йس���-----------------------------------------------------*/
	int   byChiSels[3];  //���Գ��Ƶ�ѡ��
	int    iChiNums;	  //���Գ���ѡ������
	USHORT usChiFlags;		//�Եı�־

	/*-�йظ���-----------------------------------------------------*/
	int   byGangSels[MJ_MAX_CGP];	  //���Ըܵ�ѡ��
	int    iGangNums;				  //���Ը�ѡ������		
	int    nBuGangInPeng[MJ_MAX_CGP]; //�����ڽ�������һ��������

	int    iLongDui;///0��ʾɶ��û�� 3��ʾ���һ��ץ�������߶Ե���

    emGangType  nGType[4]; //������

	/*-�й�����-----------------------------------------------------*/
	int   byTingSels[HAND_MAX_NUM_MJ]; //�����ɴ��������ѡ��
	int    iTingNums; //�����ɴ��������ѡ������

	/*-�йغ���-----------------------------------------------------*/

	int  iHuTypeNums;	//������������		

	emHuType nHuType[MJ_MAX_HUTYPE]; //���ͱ�

	emHuSpecialWay nHuSpecWay[2]; //���������

	int  nHuGangIdx;	//���ܿؼ���λ��(��������)

	/*-����-----------------------------------------------------*/

	int nIndexSel; //���������ѡ��

	TAG_INIT(TMjActInfo)
	{
		::memset(this,0,sizeof(TMjActInfo));
	}

	//һЩ����////////////////////////////////////////////////////////////
public:

	//���Ҹ���ѡ�������
	int FindGangIndex(int byCard) const
	{
		for (int i = 0; i < iGangNums; ++i)
		{
			if (byCard == byGangSels[i])
			{
				return i;
			}
		}
		return -1;
	}

	//���Ҹ���ѡ�������
	int FindTingIndex(int byCard) const
	{
		for (int i = 0; i < iTingNums; ++i)
		{
			if (byCard == byTingSels[i])
			{
				return i;
			}
		}
		return -1;
	}

	////////////////////////////////////////////////////////////
};

//һ�����ƽ��
struct TMjCGPNode
{
    emType nType; //�������
	int val[4]; //������ֵ
	int from;	//���Ƶ���Դ
	int  nIdx;  //���Ƶ�λ��
	TAG_INIT(TMjCGPNode)
	{
		::memset(this,0,sizeof(TMjCGPNode));
	}
};


//ȡ�Ƶ���Ϣ,���鷵������ṹ��
struct TMjFetch
{
	int val;	//��ֵ
	int left; //��ǽʣ�������

	TMjFetch()
	{
		::memset(this,0,sizeof(TMjFetch));
	}
};

struct CheckHuStruct
{
	BYTE  data[HAND_MAX_NUM_MJ][2];   //�����ݣ�0��ֵ��1����
	BYTE  count;
	BYTE  maxPoint;
	void Add(BYTE pai)
	{
		if(GetPaiCount(pai)>0)
		{
			for(int i=0;i<HAND_MAX_NUM_MJ;i++)
			{
				if(data[i][0] == pai && pai != 255)
					data[i][1]++;
			}
		}
		else
		{
			data[count][0] = pai;
			data[count][1] = 1;
			count++;
		}
	};
	BYTE GetPaiCount(BYTE pai)
	{
		for(int i=0;i<HAND_MAX_NUM_MJ;i++)
		{
			if(data[i][0] == pai && pai != 255)
				return data[i][1];
		}
		return 0;
	};
	BYTE GetDataCount()
	{
		count = 0;
		for(int i=0;i<HAND_MAX_NUM_MJ;i++)
		{
			if(data[i][0] != 255)
				count++;
		}
		return count;
	};
	BYTE GetAllPaiCount()
	{
		BYTE num = 0;
		for(int i=0;i<HAND_MAX_NUM_MJ;i++)
		{
			if(data[i][0] != 255)
				num += data[i][1];
		}
		return num;
	};
	void SetPaiCount(BYTE pai,BYTE num)
	{
		for(int i=0;i<HAND_MAX_NUM_MJ;i++)
		{
			if(data[i][0] == pai && pai != 255)
			{
				data[i][1] = num;
			}
		}
	}
	void Init()
	{
		memset(data,255,sizeof(data));
		for(int i=0;i<HAND_MAX_NUM_MJ;i++)
		{
			data[i][1] = 0;
		}
		count=0;
		maxPoint = 0;
	};
	CheckHuStruct()
	{
		Init();
	}
};


#endif