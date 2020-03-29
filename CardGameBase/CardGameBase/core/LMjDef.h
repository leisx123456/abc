#ifndef __L_MJ_DEF_H__
#define __L_MJ_DEF_H__
namespace LxMahjone
{

}

//////////////////////////////////////////////////////////////////////////
//公共宏定义

#define KIND_ID						324									//游戏 I D

#define GAME_MJ_CARD_COUNT_MAX					144		// 一副麻将最大张数							//游戏人数
#define GAME_MJ_PLAYER_MAX					4									//游戏人数
#define GAME_NAME					TEXT("成都麻将")					//游戏名字
#define GAME_GENRE					(GAME_GENRE_SCORE|GAME_GENRE_MATCH|GAME_GENRE_GOLD)	//游戏类型

//游戏状态
#define GS_MJ_FREE					GS_FREE								//空闲状态
#define GS_MJ_PLAY					(GS_PLAYING+1)						//游戏状态

#define INVALID_CHAIR				0xFFFF								//无效椅子
#define INVALID_TABLE				0xFFFF								//无效桌子

//常量定义
#define MAX_WEAVE					4									//最大组合
#define MAX_INDEX					42									//最大索引
//#define MAX_INDEX					34									//最大索引
//#define MAX_INDEX					27									//最大索引
#define MAX_HAND_COUNT				14									//最大数目
#define MJ_MAX_OUTED				30									//最大数目
#define CARD_TOTAL_NUM				108									//最大库存
#define CARD_EMPTY					0


//游戏事件ID定义,(取值范围从10-49)（定时器的处理）

#define TIME_ID_Ready			9			// 选座
#define TIME_ID_CHOOSE_A_SEAT			10			// 选座
#define TIME_ID_CHOOSE_BANKER				11			// 选庄
#define TIME_ID_SHUFFLE_CARD				13			// 系统洗牌据事件
#define TIME_ID_ROCK_DICE			12			// 系统确定起牌方向及起牌墩数据事件
#define TIME_ID_FETCH_HANDCARDS		14			//系统发手牌事件
#define TIME_ID_TIAO_CARDS				15			//为各玩家跳牌事件
#define TIME_ID_TBA         16          
//#define TIME_ID_BU_HUAS				16			//为各玩家补花事件
#define TIME_ID_GIVE_NEXT_TOKEN		17			//系统为某个玩家下发令牌事件,得令牌者抓牌出牌
#define TIME_ID_GANG_HUA				18			//系统为某个玩家执行杠花事件
#define TIME_ID_RESULT					19			//游戏结算事件
#define TIME_ID_ROUND_FINISH			20			//一回合完成/事件

//dong add
#define TIME_ID_FETCH_BIRD				21			//抓鸟
#define TIME_ID_PIAO					22			//加飘

#define TIME_ID_NT_MORE                23          //保护机制

#define TIME_ID_AUTO_ACT				24			//自动执行动作
#define TIME_ID_AUTO_OUTCARD			25			//自动出牌
#define TIME_ID_AUTO_ACT_NO			26			//没有托管自动执行动作
#define TIME_ID_AUTO_OUTCARD_NO		27			//没有托管自动出牌



const int SCORE_BASE = 5;
const int MAX_FAN = 3;
const long COIN_MUL = 200;


//////////////////////////////////////////////////////////////////////////
//逻辑掩码

#define	MASK_COLOR					0xF0								//花色掩码
#define	MASK_VALUE					0x0F								//数值掩码





//麻将机取牌方法-顺时针-反方向
enum E_WallGetDirection
{
	EW_South,
	EW_West,
	EW_North,
	EW_East
};

//骰子方向、玩家摸牌打牌顺序-逆时针-正方向
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
	EGS_TBA,                 //to be assured,定缺
	EGS_Playing,
	EGS_Over
};

//一个栏牌结点
struct TMjCGPNode
{
	int nType; //结点类型
	int val[4]; //栏牌数值
	int from;	//栏牌的来源
	int  nIdx;  //栏牌的位置
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
//活动玩家定义
// 成为活动玩家的来源：1.第一轮系统指定庄家 2. 继承，上个活动玩家出牌没有人响应，由系统按逆时针指定 3.响应，对上个活动玩家出的牌响应了动作。
// 结束标志:出牌
struct T_ActiveUser
{
	int nActiveUser;
	ActiveUserType eActiveUserType;

	// 继承型,当前活动玩家摸的牌
	int nNewCard;

	// 响应型
	int nGotCard;
	int nFormUser;

	// 自己打出的牌
	int nOutCard;

	T_ActiveUser()
		: nActiveUser(0)
		, eActiveUserType(EA_FristGot)
	{

	}

};

// 玩家的动作详细信息结构////////////////////////////////////////////////////////////////////
// 枚举所有动作
enum E_ActionTypeFlags
{
	EA_Wait		= 0x0001L,  //等待动作比较位，玩家请求动作前为0，请求动作后为1
	EA_Pass		= 0x0002L,  //过
	EA_Eat		= 0x0004L,  //吃
	EA_Pong		= 0x0008L,	//碰牌
	EA_Kong		= 0x0010L,	//杠牌
	EA_Listen	= 0x0020L,
	EA_hu		= 0x0040L,	//胡

	// 特殊动作
	EA_OutCard	= 0x0080L,

};

enum E_EatTypeFlags
{
	EA_EatLeft		= 0x01,	//左吃
	EA_EatCenter	= 0x02,	//中吃
	EA_EatRight		= 0x04, //右吃
};

struct T_MjActEatInfo
{
	int arrEatSelect[3];  //可以吃牌的选择
	int nEatSelectNums;	  //可以吃牌选择数量
	unsigned short usEatFlags;	//吃的标志
};

enum E_KongType
{
	EK_KongAn,	//下雨-暗杠
	EK_KongBa,	//刮风-巴杠,补杠
	EK_KongDian	//刮风-点杠
};

struct T_MjActKongInfo
{
	int arrKongSelect[MAX_WEAVE];	  //可以杠的选择
	int nKongSelectNums;				  //可以杠选择数量		
	int arrBuGangInPeng[MAX_WEAVE]; //补杠在建立在那一个碰牌上
	E_KongType arrKongType[4]; //杠类型

	int nCurSelectIndex;

	//查找杠牌选择的索引
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
	int arrListenSelect[14]; //听，可打出牌听的选择
	int nListenSelectNums; //听，可打出牌听的选择数量
	//查找杠牌选择的索引
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

//胡牌定义
enum E_MjHuType
{
	EH_Unknown = 0,	//未知标记
	EH_3x_2 = 1,	// 3x+2
	EH_SevenPair = 2,			//七对
	EH_ShiSanYao = 3,     //十三幺
	EH_JiangJiangHu = 4,
	EH_YouJin = 5,        //游金
	EH_SanJinDao = 6,		//三金倒,

};

enum E_MjHuName
{
	// 基本番型
	EHN_Ping,			//平胡
	EHN_Pong,	             //碰碰胡,大对子
	EHN_Qing,	            //清一色
	EHN_All_19,	              //幺九
	EHN_SevenPair,     	      //七对
	EHN_JinGouDiao,			// 金钩钓
	// 特殊番型
	EHN_258_Pong,	          //将碰
	EHN_Dai_19,	// 带19
	// 复合番型
	EHN_QingPong,	          //清碰,清对
	EHN_DragonSevenPair,	     //龙七对
	EHN_QingSevenPair,	      //清七对
	EHN_QingDragonSevenPair,	  //青龙七对
	EHN_Qing_19,	          //清幺九

	EHN_HuNameMax
};

enum E_MjHuWay
{
	EHW_Unknown,  //未知标记
	// 普通方式
	EHW_JiePao,  //接炮
	EHW_ZiMo,  //自摸
	// 特殊方式
	EHW_DayHu,	              //天胡
	EHW_LandHu,	               //地胡
	EHW_PensonHu,	              //人和
	EHW_QiangGang,  //抢杠
	EHW_DianGangKai,  //点杠杠上开花
	EHW_AnGangKai,		//暗杠杠上开花
	EHW_BuGangKai,		//补杠杠上开花
	EHW_GangChong,  //杠冲

};


enum E_MjSuanFenType
{
	// E_MjHuWay  + E_MjHuName + 根
};

// 玩家胡牌数据
struct T_MjActHuInfo
{
	int nHuNameNums;	//胡牌类型数量		
	E_MjHuName arrHuName[EHN_HuNameMax]; //牌型表
	E_MjHuWay eMjHuWay; //特殊胡类型
	int  nHuGangIdx;	//抢杠控件的位置(用于抢杠)
};

// 出牌信息
struct T_MjActOutInfo
{
	int nOutCardUser;
	int nOutCardValue;

	int m_nOutedNums; // 出牌数，不包含别人拿去碰杠胡的牌， 用于刷新界面出牌
};

// 玩家动作信息
struct T_MjActInfo
{
	unsigned short usActFlags;	//动作标识

	T_MjActEatInfo tMjActEatInfo;
	T_MjActKongInfo tMjActKongInfo;
	T_MjActListenInfo tMjActListenInfo;
	T_MjActHuInfo tMjActHuInfo;

	

	// 特殊动作
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
// 麻将数据通信数据结构定义

// 发手牌数据
struct T_MsgDealCards
{
	int arrCardHand[4][14];
	int nPlayerCount;
	int arrMjCardsPair[GAME_MJ_CARD_COUNT_MAX];
	int nMjNumAllocation;
	
};

// 指派活动用户, 
struct T_MsgAppointActiveUser
{
	T_ActiveUser tActiveUser;
	T_MjActInfo tMjActInfo;
	int arrMjCardsPair[GAME_MJ_CARD_COUNT_MAX]; // 一副麻将
	int nMjNumAllocation;	// 一副麻将分配数
}; 



// 响应活动用户
struct T_MsgResponseToActiveUser
{
	int nActiveID;		// 活动玩家id
	int nResponseID[4];	// 响应的玩家id，可以是活动玩家自己

	// 响应玩家可以具备哪些动作行为
	T_MjActInfo tMjActInfo[4];

};



// 动作结果
struct T_MsgActResultInfo
{
	unsigned short	usActFlags;				//动作类型
	int	arrActUsers[4];	//动作所属者表,在吃、碰、杠、听动作中，动作一般只属一个玩家，只用byUsers[0]，但也有动作由多玩家执行，如一炮多响。
	int nActUserNums;				//动作所属者人数

	int	byFromUser;				//动作的触发者，该成员只用于胡牌类型，吃、碰、杠中该值无效，FromUser可从CGPNode中取得
	int	byHands[14]; //执行动作后人变化后的手牌
	int iHandNums;		 //变化后的手牌数量

	TMjCGPNode CGPNode;		//如果有拦牌，在此处取数据

	bool bQGang;			//如果是抢杠胡，在此处取数据
	int nQGangIdx;		//抢那一个杠控件

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
	// 特殊动作
	T_MjActOutInfo tMjActOutInfo;
};

#endif // __L_MJ_DEF_H__