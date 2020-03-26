//Download by http://www.NewXing.com
// CleverThink.h: interface for the CCleverThink class.
//
//////////////////////////////////////////////////////////////////////
#include "CmpThink.h"


#if !defined(AFX_CLEVERTHINK_H__FE1EF685_5882_4BDB_ACF3_7F989EEC0B19__INCLUDED_)
#define AFX_CLEVERTHINK_H__FE1EF685_5882_4BDB_ACF3_7F989EEC0B19__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

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

#endif // !defined(AFX_CLEVERTHINK_H__FE1EF685_5882_4BDB_ACF3_7F989EEC0B19__INCLUDED_)
