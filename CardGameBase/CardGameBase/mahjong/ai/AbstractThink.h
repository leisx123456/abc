#pragma once
#include "../../core/LMjCard.h"

// 分值权重定义
#define SCORE_Hu 1000            //
#define SCORE_Kong 500            //不影响牌的情况
#define SCORE_Triplet 400        //刻子 活动区-暗刻 固定区-明刻
#define SCORE_Sequence  300      //顺子
#define SCORE_Pair_Only 120      //将，全手牌只有一组对子
#define SCORE_Door_TwoHead 100          //门子，两头张
#define SCORE_Door_Middle  80       //卡张
#define SCORE_Door_Edge  70      //边缘门子
#define SCORE_Single_34567 9          //单牌2-8
#define SCORE_Single_28 6           //单牌1,9
#define SCORE_Single_19 3			//单牌19
#define SCORE_TBA 0                    //缺


enum E_HandCardRelationType
{
	EHC_Group,      //一个顺子、刻子或杠子
	EHC_OneToOne,     //差一张形成组
	EHC_Single        //单牌，和周围牌任何2张都不能形成组
};


//是否能形成顺子
enum E_SequenceType
{
	ES_MiddleCard,                   //卡张-夹张-嵌张
	ES_LeftEdgeCard,                  //边张-目标牌在最左
	ES_RightEdgeCard                  //边张-目标牌在最右较大
};

//是否能形成门子(塔子)
enum E_DoorType
{
	ED_DoorTwoHead,                //两头张 23
	ED_DoorMiddle,                 //嵌张 02,35,68
	ED_DoorEdge                    //边张 01,78
};


class IAbstractThink
{
public:
	IAbstractThink();

	//////////////////////////////////////////////////////////////////////////
	//接口定义
	virtual ~IAbstractThink();

	virtual int thinkDingQue(CLMjCard aCards[], unsigned int unCardCount, T_WeaveCardsItem aWeaveItem[], unsigned int unItemSize) = 0;

	virtual void thinkHu(CLMjCard aCards[], unsigned int unCardCount
		, T_WeaveCardsItem aWeaveItem[], unsigned int unItemSize, CLMjCard cardDest) = 0;

	virtual CLMjCard thinkOutCard(CLMjCard aCards[], unsigned int unCardCount
		, T_WeaveCardsItem aWeaveItem[], unsigned int unItemSize) = 0;
};
