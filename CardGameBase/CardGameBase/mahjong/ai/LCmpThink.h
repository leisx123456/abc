#pragma once
#include "LBasicThink.h"
#include <vector>
#include <map>

// 分值权重定义
#define SCORE_Hu 1000            //
#define SCORE_Kong 500            //不影响牌的情况
#define SCORE_Triplet 400        //刻子 活动区-暗刻 固定区-明刻
#define SCORE_Sequence  300      //顺子
#define SCORE_Pair_Only 120      //将，全手牌只有一组对子
#define SCORE_Pair 100      //将
#define SCORE_Door_TwoHead 100          //门子，两头张
#define SCORE_Door_Middle  80       //卡张
#define SCORE_Door_Edge  70      //边缘门子
#define SCORE_Single_TwoHead 10          //单牌3-7
#define SCORE_Single_Middle 8           //单牌28
#define SCORE_Single_Edge 6			//单牌19
#define SCORE_SingleToSingle_TwoHead 3
#define SCORE_SingleToSingle_Middle 2
#define SCORE_SingleToSingle_Edge 1
#define SCORE_TBA 0                    //缺
#define SCORE_Badly 0

//////////////////////////////////////////////////////////////////////////
// 总体思路 
// 1.递归得到3张组合（顺子，刻字）最大数组合数的牌标记，锁定，余下未锁定的牌进入步骤2
// 2.递归得到2张组合（门子）最大数组合数的牌标记，锁定，余下未锁定的牌进入步骤3
// 3.
class CLCmpThink:public CLBasicThink
{
public:	
	CLCmpThink();

public:
	CLCmpThink* clone() const { return new CLCmpThink(*this); }

	// 思考定缺，返回缺的花色
	virtual int thinkDingQue(CLMjCard aCards[], unsigned int unCardCount);

	virtual bool thinkHu(CLMjCard aCards[], unsigned int unCardCount
		, T_WeaveCardsItem aWeaveItem[], unsigned int unItemSize, CLMjCard cardDest, int nQueColor = -1);

	// 返回真表示要杠
	virtual bool thinkKong(CLMjCard aCards[], unsigned int unCardCount
		, T_WeaveCardsItem aWeaveItem[], unsigned int unItemSize, CLMjCard cardDest, int nQueColor = -1);

	virtual bool thinkPong(CLMjCard aCards[], unsigned int unCardCount
		, T_WeaveCardsItem aWeaveItem[], unsigned int unItemSize, CLMjCard cardDest, int nQueColor = -1);

	virtual CLMjCard thinkOutCard(CLMjCard aCards[], unsigned int unCardCount
		, T_WeaveCardsItem aWeaveItem[], unsigned int unItemSize, int nQueColor = -1);

	void think();

protected:
	int isExistQue(int nCardColor = -1);
	void markThree();
	void markTwo();
	void markOne();
	void markOne(CLMjCard & handCard);
	void thinkNoSingle();
	
	void addExtraScoreToSingleCard(CLMjCard & handCard);
	void commitScore();
	void saveBadlyCardByScore();
	int totalScore();

	int getMarkNum();
	void unLockAll();


private:
	
	std::map<int, int> m_mapScore;
	
};


