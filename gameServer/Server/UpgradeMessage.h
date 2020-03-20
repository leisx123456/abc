#ifndef UPGRADEMESSAGE_MOBAN_H
#define UPGRADEMESSAGE_MOBAN_H
#include "../Server/MjCommonDef.h"
#define TAG_INIT(tagName) \
	tagName(){Clear();} \
	void Clear()


#define PLAY_COUNT						4				//游戏人数
#define MAX_NIAO_COUNT					12				//最大捉鸟数量
#define NAME_ID                         40026400        ///游戏id
#define GET_SERVER_INI_NAME(str)		sprintf(str,"%s%d_s.ini",CINIFile::GetAppPath(),NAME_ID);
//******************************************************************************************
//数据包处理辅助标识(消息的处理)
//******************************************************************************************
#define ASS_GM_AGREE_GAME				1				//同意游戏
#define ASS_GM_GAME_STATION				2				//游戏状态

#define ASS_GM_ROOM_DISSMISS			10				//房间解散

#define MDM_GM_GAME_NOTIFY              180
#define ASS_UG_USER_SET					50				//用户设置游戏
#define ASS_UG_USER_SET_CHANGE			51				//玩家设置更改

#define ASS_GAME_BEGIN					120				//麻将游戏开始
#define ASS_AUTO_REQ					126				//来自客户端的托管请求
#define ASS_AUTO_INFO					127				//来服务端某玩家托管消息

#define ASS_FETCH_GUI_INFO        128         //服务器下发翻鬼信息

#define ASS_MAKE_NT_INFO				132				//服务器下发定庄的消息
#define ASS_FETCH_METHOD_INFO			133				//服务器下发起牌方法
//#define ASS_FETCH_METHOD_FINISH			135				//客户端报告服务器定起牌点画播放完成
#define ASS_FETCH_HANDCARDS				136				//服务器下发手牌数据
#define ASS_TIAO_CARDS					137				//服务器下发跳牌数据
#define ASS_BU_HUAS						138				//服务器为各玩家补花的数据
#define ASS_BU_HUAS_FINISH				139				//客户端发来的写成补花动画消息
#define ASS_TOKEN						140				//服务器为某玩发令牌
#define ASS_OUT_CARD_REQ				141				//客户端的出牌请求
#define ASS_OUT_CARD_INFO				142				//服务器报告某玩家的出牌结果
#define ASS_ACT_NOTIFY					143				//服务器通知某玩家有动作(如吃碰杠听等动作)
#define ASS_ACT_REQ						144				//客户端向服务器提出进行某动作的请求
#define ASS_ACT_INFO					145				//服务器返回某玩家动作执行结果
#define ASS_ANGANG_RESULT				146				//暗杠的即时结算消息

#define ASS_ROUND_FINISH				149				//来自服务端回合结束的消息
#define ASS_RESULT						150				//回合算分

//-----------------------huansanzhang 160---------------
#define ASS_CHANGE_CARD					160				//换三张
#define ASS_CHANGE_CARD_RESP			161				//某一方换三张的请求
#define ASS_CHANGE_CARD_FINISH			162				//换三张完成，将三张牌的数据发送给客户
#define ASS_DING_QUE					163				//定缺
#define ASS_DING_QUE_RESP				164				//定缺完成请求
#define ASS_JI_INFO						165				//发送鸡的信息给客户端

//dong add
#define ASS_PAIJU_INFO					176				//服务端向客户端发送牌局信息
#define ASS_GAME_OVER					177				//服务端向客户端发送牌局结束房间解散不可以继续游戏
#define ASS_ROOM_CONFIG					178				//服务端向客户端发送房间配置信息
#define ASS_ASSIGN_MAKENT_FINISH		179				//客户端向服务器发送播放完定庄动画
#define	ASS_GAME_REBIND					180				//断线重连
//----------------------------------------------------------------------------
#define ASS_PIAO_NOTIFY					170				//服务器通知客户端加飘
#define ASS_PIAO_REQ					171				//客户端报告服务器是否加飘
#define ASS_PIAO_RESP					172				//通知客户端飘的结果	
#define ASS_NIAOCARDS_INFO				173				//通知客户端鸟牌信息

//异常类消息
#define ASS_NO_PLAYER					155				//没有玩家进行游戏
#define ASS_AGREE						156				//玩家是否同意的消息
#define ASS_NOT_ENOUGH_MONEY			158				//玩家金币不足的提示
#define ASS_FORCE_QUIT_REQ				160				//玩家强退请求
#define ASS_QUIT_INFO					161				//服务端向客户端报告那个玩家退出的消息


//******************************************************************************************

//************************************************************************************************************

//游戏事件ID定义,(取值范围从10-49)（定时器的处理）

#define TIME_ASSIGN_DIRECT			10			//定玩家方向事件
#define TIME_MAKE_NT				11			//系统选庄事件
#define TIME_FETCH_METHOD			12			//系统确定起牌方向及起牌墩数据事件
#define TIME_RAND_CARD				13			//系统洗牌据事件
#define TIME_FETCH_HANDCARDS		14			//系统发手牌事件
#define TIME_TIAO_CARDS				15			//为各玩家跳牌事件
#define TIME_BU_HUAS				16			//为各玩家补花事件
#define TIME_GIVE_NEXT_TOKEN		17			//系统为某个玩家下发令牌事件,得令牌者抓牌出牌
#define TIME_GANG_HUA				18			//系统为某个玩家执行杠花事件
#define TIME_RESULT					19			//游戏结算事件
#define TIME_ROUND_FINISH			20			//一回合完成/事件


//dong add
#define TIME_FETCH_BIRD				21			//抓鸟
#define TIME_PIAO					22			//加飘

#define TIME_NT_MORE                23          //保护机制

#define TIME_AUTO_ACT				24			//自动执行动作
#define TIME_AUTO_OUTCARD			25			//自动出牌
#define TIME_AUTO_ACT_NO			26			//没有托管自动执行动作
#define TIME_AUTO_OUTCARD_NO		27			//没有托管自动出牌

#define TIME_FETCH_GUI              28          //抓赖子
//游戏结束标志定义
#define GF_NORMAL					10			//游戏正常结束
#define GF_SALE						11			//游戏安全结束
//****************************************************************************************


//******************************************************************************************
// 游戏状态定义
//******************************************************************************************

#define GS_WAIT_SETGAME					0	//等待东家设置状态
#define GS_WAIT_ARGEE					1	//等待同意设置
#define GS_CONFIG_DESK					20	//设置底注状态
#define GS_AGREE_NOTE					21	//同意底注状态
#define GS_ASSIGN_DIRECT				22	//分配各玩家方位的状态
#define GS_FETCH_HANDCARDS				23	//各玩家方位取手张牌状态
#define GS_TIAO_CARDS					24	//各玩家跳牌状态
#define GS_BU_HUA						25	//各玩家进行补花状态
#define GS_PLAYING						26	//各玩家进行打牌状态

#define GS_WAIT_PIAO                    27  //漂的状态
#define GS_WAIT_NEXT_ROUND				28	//等待下一回合状态
#define GS_PAIJU_FINISH					29	//牌局结束

//******************************************************************************************

// 录像的事件ID
enum Video_Event
{
	Video_Event_begin				= ASS_GAME_BEGIN ,			//游戏开始
	Video_Event_select_banker		= ASS_MAKE_NT_INFO ,		//选择庄家
	Video_Event_select_Piao			= ASS_PIAO_RESP ,			//选票
	Video_Event_send_card			= ASS_FETCH_HANDCARDS ,		//发牌
	Video_Event_tiao_card			= ASS_TIAO_CARDS ,			//庄家跳牌（摸的第一张牌）
	Video_Event_mo_card				= ASS_TOKEN ,				//摸牌
	Video_Event_out_card			= ASS_OUT_CARD_INFO ,		//出牌
	Video_Event_action				= ASS_ACT_INFO ,			//吃碰杠
	Video_Event_niao				= ASS_NIAOCARDS_INFO,		//鸟牌信息		
	Video_Event_resoult				= ASS_RESULT,				//结算
	Video_Event_gui					= ASS_FETCH_GUI_INFO,	//鬼牌信息
};



//有关桌面配置
struct TDeskCfg
{

	BYTE    byWaitAgree;	 //等待同意的时间
	BYTE    byOutCard;	   //出牌思考时间
	BYTE    byAct;		    //动作思考时间，如碰杠胡
	BYTE	byCheaterOpen;		//是否开启配牌器
	BYTE	byNextRound;	 //下一回合等待时间，是结算框上显示
	int		nFirstNtMode;	//首轮庄家的选择模式  1---随机庄家，2---东方位当庄，3---房主当庄
	int		nNtMode;		//确定庄家模式    1--胡牌玩家继续当庄，2--上次庄家的顺位当庄


	TDeskCfg()
	{
		::memset(this,0,sizeof(TDeskCfg));
	}
	
};


//服务端向各玩家发送的动作结果包
struct TActInfo 
{
	USHORT	usActFlag;				//动作类型
	BYTE	byUsers[PLAY_COUNT];	//动作所属者表,在吃、碰、杠、听动作中，动作一般只属一个玩家，只用byUsers[0]，但也有动作由多玩家执行，如一炮多响。
	int		iUserNums;				//动作所属者表长度
	BYTE	byFromUser;				//动作的触发者，该成员只用于胡牌类型，吃、碰、杠中该值无效，FromUser可从CGPNode中取得

	int	byHands[HAND_MAX_NUM_MJ]; //执行动作后人变化后的手牌
	int		iHandNums;		 //变化后的手牌数量

	TMjCGPNode CGPNode;		//如果有拦牌，在此处取数据

	bool    bQGang;			//如果是抢杠胡，在此处取数据
	int		nQGangIdx;		//抢那一个杠控件

	TActInfo()
	{
		::memset(this,0,sizeof(TActInfo));
	}
};

//一回合完成
struct TRoundFinish 
{
	int		iRoundTimes;						//当前局数
	int		iAllRoundTimes;						//总的局数
	int		iZimoTimes[PLAY_COUNT];				//自摸次数
	int		iJiePaoTimes[PLAY_COUNT];			//接炮次数
	int		iFangPaoTimes[PLAY_COUNT];			//点炮次数
	int		iMingGangTimes[PLAY_COUNT];			//明杠次数
	int		iAnGangTimes[PLAY_COUNT];			//暗杠次数
	int     iBuGangTimes[PLAY_COUNT];			//补杠次数
	int     iYouJinTimes[PLAY_COUNT];			//游金次数
	int		iLiuJuTimes[PLAY_COUNT];			//流局次数
	int		iGangKaiTimes[PLAY_COUNT];			//杠开次数
	int		iTotalScore[PLAY_COUNT];			//玩家总分
	TAG_INIT(TRoundFinish)
	{
		::memset(this,0,sizeof(TRoundFinish));

	}
};

//结算结构包
struct TResult
{
	bool m_bHuangZhuang;				//是否荒庄
	bool bHu[PLAY_COUNT];				//胡牌的玩家置true
	int  iHuNums;						//胡牌玩家的数量
	BYTE byFangPaoUser;					//放炮玩家
	BYTE byHuUser;					   //胡牌玩家
	char szHuRepo[PLAY_COUNT][128];		//各玩家的胡牌报告

	emHuSpecialWay nHuSpecWay[PLAY_COUNT][2];	//特殊胡方式
	emHuType nHuTypes[PLAY_COUNT][MJ_MAX_HUTYPE]; //各胡牌玩家的胡牌牌型表
	int iHuTypeNums[PLAY_COUNT];				//各胡牌玩家的胡牌牌型表的长度

	int		iHuPoint[PLAY_COUNT];		//各玩家胡牌的得失分
	int		iPiaoPoint[PLAY_COUNT];		//各玩家飘的得失分
	int		iNiaoPoint[PLAY_COUNT];		//各玩家的鸟分
	int		iGangPoint[PLAY_COUNT];		//各玩家的杠分
	int		iNTPoint[PLAY_COUNT];		//各玩家庄闲得分
	__int64	iTotalPoint[PLAY_COUNT];	//各玩家的总分
	__int64	iRealMoney[PLAY_COUNT];		//各玩家实际得失分
	int		iChPoint;					//本局所变动的分数

	int		iLeftMoney[PLAY_COUNT];		//各玩家手上剩余的钱
	int		iPoolPoint[PLAY_COUNT];		//各玩家的分池

	TAG_INIT(TResult)
	{
		::memset(this,0,sizeof(TResult));
		byFangPaoUser = 255;
		byHuUser = 255;
	}
};

//定义令牌事件通讯包,用于控制令牌发放的方式，如得令玩家能不能抓牌、抓牌方向等

struct TEvTokenParm
{
	//得令玩家能否抓牌
	bool bCanFetch;
	//令牌授予者,255为当前令牌得者的下家
	int byNextUser;
};


//定义回合结束事件通讯包\
用于表明该局是否这荒庄局
struct TEvRoundFiniParm
{
	//是否荒庄
	bool bHuang;
};


//服务器通知玩家有动作(即吃碰杠等)的数据包
struct TActNotify
{
	TMjActInfo info; //玩家的动作信息

	TAG_INIT(TActNotify)
	{
		::memset(this,0,sizeof(TActNotify));
	}
};

struct TOutCard
{
	enum emType
	{
		UNKNOWN		= 0,	//未知类型
		REQUEST		= 1,	//请求出牌
		INFO		= 2,	//出牌信息
	};

	emType nType; //请求包类型

	int byCard; //出的牌
	BYTE byUser; //出牌的玩家 用于INFO类型

	int byCards[HAND_MAX_NUM_MJ];
	int  iHandNums;

	TAG_INIT(TOutCard)
	{
		::memset(this,0,sizeof(TOutCard));
	}
};

#endif