#ifndef __MJDEFINE_H__
#define __MJDEFINE_H__


#define CARD_TOTAL_NUM      108
#define PLAYER_NUM		    4
#define HOLD_CARD_NUM		13
#define ALL_TYPE_NUM		27		// 所有类型牌的个数
#define TimeTBA         20          
#define TimeSelect		20				//玩家选择的时间（秒）
#define TimeSezi		15				//色子动画的时间（步）
#define TimeRun			500			//电脑玩家处理牌的时间（毫秒）

//enum E_ExtraHuType
//{
//
//	EE_SelfDrawn,	 //自摸加番(地方加底)	1番
//
//	EE_Kong_hua,	 //杠开		1番
//
//	EE_Kong_pao,	 //杠炮		1番
//
//	EE_Kong_qiang,	 //抢杠		1番
//
//	EE_Kong,	    //根（杠）	1番
//};

//除了胡的名称加番以外的加番定义
#define HU_SELF_DRAWN       0x01
#define HU_KONG_HUA         0x02
#define HU_KONG_PAO         0x04
#define HU_KONG_QIANG       0x08
#define HU_GOLD_HOOK        0x10        //金钩钓
#define HU_HAI_DI_LAO		0x20		//海底捞
#define HU_HAI_DI_PAO		0x40		//海底炮

const int SCORE_BASE = 5;
const int MAX_FAN = 3;
const long COIN_MUL = 200;

//麻将机取牌方法-顺时针-反方向-clockwise
//骰子方向、玩家摸牌打牌顺序-逆时针-正方向-antiClockWise 
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
	E_Wind,            //风牌
	E_Wrigley,         //箭牌
	E_Flower,           //花牌
	E_OrderNum          //序数牌
};

enum E_OrderNumCardType
{
	EO_Circle,         //筒牌 0-8
	EO_Character,      //万牌 9-17
	EO_Bamboo,          //条牌 18-26
	EO_Invalid = 255    //没有其他牌了
};


enum E_Sex
{
	S_Boy,
	S_Girl
};

enum E_CardType
{
	//手牌
	CT_HAND_North,
	CT_HAND_East,
	CT_HAND_South,
	CT_HAND_West,

	//牌墙
	CT_WALL_North,
	CT_WALL_East,
	CT_WALL_South,
	CT_WALL_West,

	//胡牌
	CT_HU_North,
	CT_HU_East,
	CT_HU_South,
	CT_HU_West,

	//牌池(打出了的牌)
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
	EGS_TBA,                 //to be assured,定缺
	EGS_Playing,
	EGS_Over
};


//玩家状态
enum E_PlayerStatus
{
	EPS_DrawACard, //摸牌
	EPS_ThinkOutCard, //思考出牌
	EPS_OutCard, //刚出完牌
	EPS_Waiting, //等待其他人出牌
};

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






#endif // __MJDEFINE_H__