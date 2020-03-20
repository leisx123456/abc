// ***************************************************************
//  SoundsDef   version:  1.0   ·  date: 2/09/2017
//  -------------------------------------------------------------

//	麻将游戏基本数据定义,适合大部分麻将游戏开发使用\

//  -------------------------------------------------------------
//  Copyright (C) 2011 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************
#include <Windows.h>
#ifndef MJCommon_DEF_HEAD_FILE

//该宏定义控制是否开启麻将吃牌逻辑
//#define IS_HAVECHI						1					//麻将是否有吃
#define IS_TONGPAO						1					//是否开启通炮玩法
#define MJCommon_DEF_HEAD_FILE

#define MJ_MAX_CARDS					108					//麻将张数
#define MJ_MAX_CARDS_ZHONG				136					//麻将张数(带红中)
#define HAND_MAX_NUM_MJ					14					//最大手牌数量
#define MJ_MAX_OUTED					68					//最大的出牌数量（2，3，4人的时候要取最大）
#define MJ_MAX_CGP						12					//最大的栏牌数
#define MJ_MAX_HUTYPE					11					//牌型种数
#define MJ_MAX_LOUHU					15					//最大漏胡张数
#define MJ_MAX_BAO_CARD					1					//财神牌的数量
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

//游戏中动作的标识，一般来说动作优先级别越高，开关位越高
typedef enum
{
	ACT_GUO  = 0x0002L,  //过
	ACT_CHI  = 0x0004L,  //吃
	ACT_PENG = 0x0008L,  //碰
	ACT_GANG = 0x0010L,  //杠
	ACT_TING = 0x0020L,  //听
	ACT_HU   = 0x1000L,  //胡

	ACT_WAIT = 0x0001L  //等待动作比较位，玩家请求动作前为0，\
	请求动作后为1
}E_MAHJONG_ACTION;


//用二进制位来做开关，表示是否需要某一种类型的牌
//00000000
enum emRandCardType
{
	Rand_hua =	1,		//00000001第一位表示是否要带花
	Rand_wan = 2,		//00000010第二位表示是否需要带万
	Rand_tong = 4,		//00000100第三位表示是否需要带筒子
	Rand_tiao = 8,		//00001000第四位表示是否需要带条子
	Rand_feng = 16,		//00010000第五位表示是否需要带东南西北
	Rand_zhong = 32,	//00100000第6位表示是否需要带红中。。
	Rand_fabai = 64,	//01000000第七位表示是否需要带发财和白班
};

enum emGangType
{
	gt_Unknown	= 0,	//未知类型
	gt_MingGang	= 1,	//明杠
	gt_BuGang	= 2,	//补杠
	gt_AnGang	= 3,	//暗杠
};

enum emHuType
{
	ht_Unknown	= 0,	//未知标记
	ht_PingHu=1,
	ht_QiDui=2,			//七对
	ht_ShiSanYao=3,     //十三幺
	ht_QiangGangHu=4,	
	ht_YouJin=5,        //游金
	ht_SanJinDao=6,		//三金倒,
	ht_GangKai=7,		//杠开
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
	hsw_Unknown		= 0,  //未知标记
	hsw_TianHu		= 1,  //天胡
	hsw_DiHu		= 2,  //地胡
	hsw_QiangGang	= 3,  //抢杠
	hsw_DianGangKai		= 4,  //点杠杠上开花
	hsw_AnGangKai=5,		//暗杠杠上开花
	hsw_BuGangKai=6,		//补杠杠上开花
	hsw_GangChong	= 7,  //杠冲

};

//麻将游戏结果标记定义
enum MjResult
{
	Res_Unknown	= 0,  //未知标记
	Res_FangPao	= 1,  //放炮
	Res_JiePao	= 2,  //接炮
	Res_ZiMo	= 3  //自摸
};

//第一盘玩家坐庄的方式
enum FirstNtType
{
	RAND_NT =1,	//随机选择一个庄家
	MASTER_NT = 2,//房主当庄
};

enum NtType
{
	TURN_NT = 1,//轮庄模式
	HU_NT = 2,//胡牌玩家当庄
};

enum emType
{
	UNKNOWN		= 0,	//未知类型
	Chi			= 1,	//吃
	Peng		= 2,	//碰
	MingGang	= 3,	//明杠
	BuGang		= 4,	//补杠
	AnGang		= 5,	//暗杠
	SPECIAL_GANG = 6,	//暗杠
	Liang        = 7,	 //亮的动作

};
//吃牌的动作标志
namespace MjChiFlag 
{
	const USHORT usT	= 1;			//吃头
	const USHORT usZ	= 2;			//吃中
	const USHORT usW	= 4;			//吃尾
	const USHORT usTZ	= 3;			//吃头 吃中
	const USHORT usTW	= 5;			//吃头 吃尾
	const USHORT usZW	= 6;			//吃中 吃尾
	const USHORT usTZW	= 7;			//吃头 吃中 吃尾
};

//玩家的动作详细信息结构////////////////////////////////////////////////////////////////////
struct TMjActInfo
{
	USHORT usFlags;	//动作标识

	/*-有关吃牌-----------------------------------------------------*/
	int   byChiSels[3];  //可以吃牌的选择
	int    iChiNums;	  //可以吃牌选择数量
	USHORT usChiFlags;		//吃的标志

	/*-有关杠牌-----------------------------------------------------*/
	int   byGangSels[MJ_MAX_CGP];	  //可以杠的选择
	int    iGangNums;				  //可以杠选择数量		
	int    nBuGangInPeng[MJ_MAX_CGP]; //补杠在建立在那一个碰牌上

	int    iLongDui;///0表示啥都没有 3表示最后一张抓的是龙七对的牌

    emGangType  nGType[4]; //杠类型

	/*-有关听牌-----------------------------------------------------*/
	int   byTingSels[HAND_MAX_NUM_MJ]; //听，可打出牌听的选择
	int    iTingNums; //听，可打出牌听的选择数量

	/*-有关胡牌-----------------------------------------------------*/

	int  iHuTypeNums;	//胡牌类型数量		

	emHuType nHuType[MJ_MAX_HUTYPE]; //牌型表

	emHuSpecialWay nHuSpecWay[2]; //特殊胡类型

	int  nHuGangIdx;	//抢杠控件的位置(用于抢杠)

	/*-其它-----------------------------------------------------*/

	int nIndexSel; //玩家作出的选择

	TAG_INIT(TMjActInfo)
	{
		::memset(this,0,sizeof(TMjActInfo));
	}

	//一些操作////////////////////////////////////////////////////////////
public:

	//查找杠牌选择的索引
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

	//查找杠牌选择的索引
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

//一个栏牌结点
struct TMjCGPNode
{
    emType nType; //结点类型
	int val[4]; //栏牌数值
	int from;	//栏牌的来源
	int  nIdx;  //栏牌的位置
	TAG_INIT(TMjCGPNode)
	{
		::memset(this,0,sizeof(TMjCGPNode));
	}
};


//取牌的信息,建议返回这个结构。
struct TMjFetch
{
	int val;	//牌值
	int left; //牌墙剩余的牌数

	TMjFetch()
	{
		::memset(this,0,sizeof(TMjFetch));
	}
};

struct CheckHuStruct
{
	BYTE  data[HAND_MAX_NUM_MJ][2];   //牌数据，0牌值，1张数
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