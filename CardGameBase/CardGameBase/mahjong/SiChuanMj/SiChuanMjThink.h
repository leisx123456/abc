#pragma once
#include "../ai/LScoreBasicThink.h"
#include <vector>
#include <map>



//////////////////////////////////////////////////////////////////////////
// 总体思路 
// 1.能做大的情况下尽量大牌
// 2.首先思考能不能清一色，如果手上有一花色大于等于10张且至少有2家定缺此花色，则往清一色方向走
// 3.如果手上有至少5对时走七小对
// 4.能碰的情况尽量碰，能杠的情况下尽量杠，如果有一家是睡宽床时就按正常思路尽量往胡牌方向上打
// 20200407 比较要不要碰杠 还是决定比较手牌加组合牌的分数之和 是否大于碰杠之前的分数之和
class CSiChuanMjThink : public CLScoreBasicThink
{
public:	
	CSiChuanMjThink();

public:
	CSiChuanMjThink* clone() const { return new CSiChuanMjThink(*this); }

	// 思考定缺，返回缺的花色
	virtual int thinkDingQue(CLMjCard aCards[], unsigned int unCardCount);

	
	virtual void think(T_MjActInfo* pMjActInfo, CLMjCard aCards[], unsigned int unCardCount, T_WeaveCardsItem aWeaveItem[]
		, unsigned int unItemSize, CLMjCard cardOut, int nQueColor = -1, unsigned short usIgnoreFlags = 0);

	void think();
private:
	bool thinkHu(T_MjActHuInfo & tMjActHuInfo);

	// 返回真表示要杠
	bool thinkKong(T_MjActKongInfo & tMjActKongInfo);
	int thinkAfterKong(CLMjCard cardGangDest, E_KongType eKongType);

	bool thinkPong();
	int thinkAfterPong();

	CLMjCard thinkOutCard();


private:
	
	std::map<int, int> m_mapScore;
	int m_nTotalScore;
	
};


