#pragma once
#include "CmpThink.h"



class CCleverThink : public CBasicThink
{
public:
	CCleverThink();
	void ThinkClever();					
	void CleverThink();										//聪明的算法
	int GetMaxScoreClever();								//获取最大分数
	void SetPaiOut(int* pOutPai,int nNum);
	virtual ~CCleverThink();

protected:
	bool IsSameTeamSeven(int pai,int &pos1,int nStart);
	void ThinkSeven();										//考虑七对
	void ThinkSevenOne();									//考虑七对判断后剩下的牌
	int FindPosOut(int pai);								//在出牌中查找牌
	int AddGoodOut(int pai);								//添加牌的优势分

protected:
	int m_nWjOutPai[80];									//玩家出了的牌
	int m_nWjOutPaiNum;
	int m_nSevenScore;										//考虑七对后的分数
	int m_nMaxScoreTotal;
	int m_nScoreSeven;
	int m_nMaxScoreSeven;
	int m_nRemainNumSeven;
	int m_nRemainSeven[14];

};


