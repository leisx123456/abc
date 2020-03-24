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
#define PLAYER_NUM		    4
#define HOLD_CARD_NUM		13
#define ALL_TYPE_NUM		27		// 所有类型牌的个数

//游戏事件ID定义,(取值范围从10-49)（定时器的处理）

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

#define TIME_ID_FETCH_GUI              28          //抓赖子



const int SCORE_BASE = 5;
const int MAX_FAN = 3;
const long COIN_MUL = 200;



//麻将牌定义
#define CARD_BACK_VALUE 0x31    //牌背面值
#define CARD_STAND_VALUE 0x99      //立牌值
#define CARD_EMPTY	0x00				//无效牌值,数组占位

#define SPACE_WEAVE_MY		10		//每组组合牌之间的间隔-自己
#define SPACE_WEAVE_OTHER	4		//每组组合牌之间的间隔-其他

//////////////////////////////////////////////////////////////////////////
//逻辑掩码

#define	MASK_COLOR					0xF0								//花色掩码
#define	MASK_VALUE					0x0F								//数值掩码


//////////////////////////////////////////////////////////////////////////
//动作定义
#define ACTION_NULL					0x00								//没有
#define ACTION_LEFT					0x01								//左吃
#define ACTION_CENTER					0x02								//中吃
#define ACTION_RIGHT					0x04								//右吃
#define ACTION_PENG					0x08								//碰牌
#define ACTION_GANG					0x10								//杠牌
#define ACTION_LISTEN					0x20								//听牌
#define ACTION_CHI_HU					0x40								//吃胡
#define ACTION_ZI_MO					0x80								//自摸

//////////////////////////////////////////////////////////////////////////
//胡牌定义
#define HU_NULL					0x00										//非胡类型
#define CHK_CHI_HU					0x01										//胡类型
#define CHR_QIANG_GANG				0x00000001									//抢杠
#define CHR_GANG_SHANG_PAO			0x00000002									//杠上炮
#define CHR_GANG_KAI				0x00000004									//杠上花
#define CHR_TIAN_HU					0x00000008									//天胡
#define CHR_DI_HU					0x00000010									//地胡
#define CHR_DA_DUI_ZI				0x00000020									//大对子
#define CHR_QING_YI_SE				0x00000040									//清一色
#define CHR_QI_XIAO_DUI				0x00000080									//暗七对
#define CHR_DAI_YAO					0x00000100									//带幺
#define CHR_JIANG_DUI				0x00000200									//将对
#define CHR_SHU_FAN					0x00000400									//素番
#define CHR_QING_DUI				0x00000800									//清对
#define CHR_LONG_QI_DUI				0x00001000									//龙七对
#define CHR_QING_QI_DUI				0x00002000									//清七对
#define CHR_QING_YAO_JIU			0x00004000									//清幺九
#define CHR_QING_LONG_QI_DUI		0x00008000									//清龙七对

enum E_HuType
{
	EH_Fart,	              //屁胡		1番

	EH_Pong,	             //碰碰胡		2番

	EH_Qing,	            //清一色		3番

	EH_All_19,	              //幺九		3番

	EH_SevenPair,     	      //七对		3番

	EH_PongQing,	          //清碰		4番

	EH_Pong_258,	          //将碰		4番

	EH_DragonSevenPair,	     //龙七对		4番

	EH_QingSevenPair,	      //清七对		5番

	EH_QingAll_19,	          //清幺九		5番

	EH_QingDragonSevenPair,	  //青龙七对	6番

	EH_DayHu,	              //天胡		6番

	EH_LandHu,	               //地胡		6番

	EH_PensonHu	              //人和		6番
};

enum E_ExtraHuType
{

	EE_SelfDrawn,	 //自摸加番(地方加底)	1番

	EE_Kong_hua,	 //杠开		1番

	EE_Kong_pao,	 //杠炮		1番

	EE_Kong_qiang,	 //抢杠		1番

	EE_Kong,	    //根（杠）	1番
};




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





//////////////////////////////////////////////////////////////////////////
// 麻将数据通信数据结构定义

// 发手牌数据
struct T_MsgDealCards
{
	CLMjCard arrCardHand[4][14];
	int nPlayerCount;
	CLMjCard m_arrMjCardWall[4][34];
	
};

// 指派活动用户
struct T_MsgAppointActiveUser
{
	int nChairID;		// 活动玩家id
	int nDrawCardValue;	// 摸牌值，如果是0则表示不需要摸牌
	CLMjCard m_arrMjCardWall[4][34];
}; 




#endif // __L_MJ_DEF_H__