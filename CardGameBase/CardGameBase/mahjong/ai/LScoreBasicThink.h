#pragma once
#include "LBasicThink.h"
#include <vector>
#include <map>

// 分值权重定义 - 按走快速胡牌法定义，杠牌的分值减少
#define SCORE_Hu 1000            //
#define SCORE_Kong 500            //不影响牌的情况
#define SCORE_Triplet 400        //刻子 活动区-暗刻 固定区-明刻
#define SCORE_Sequence  300      //顺子
#define SCORE_Pair_Only 150      //将，全手牌只有一组对子
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

// 基于分值形式的思考方法
//////////////////////////////////////////////////////////////////////////
// 本类为走快速胡牌的基本方法
// 1.递归得到3张组合（顺子，刻字）最大数组合数的牌标记，锁定，余下未锁定的牌进入步骤2
// 2.递归得到2张组合（门子）最大数组合数的牌标记，锁定，余下未锁定的牌进入步骤3
// 3.从单中选最差的牌
class CLScoreBasicThink : public CLBasicThink
{
public:	
	CLScoreBasicThink();

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


protected:
	
	std::map<int, int> m_mapScore;
	int m_nTotalScore;
	
};


