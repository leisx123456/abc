//Download by http://www.NewXing.com
// MajHu.h: interface for the CMajHu class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAJHU_H__00A958AE_66FF_4B60_9929_20194583CCB8__INCLUDED_)
#define AFX_MAJHU_H__00A958AE_66FF_4B60_9929_20194583CCB8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMajHu  
{
private:
	void ThinkFour();									//考虑只有四张的情况
	void DeleteStepTeam(int pai,int type);				//从手牌中删除顺子
	void DeleteSameTeam(int pai);						//从手牌中删除3个的
	void AddHupai(int pai);								//添加牌进手牌
	bool IsStepTeam(int pai,int type);					//是否有顺子
	bool IsSameTeam(int pai);							//是否有3个的
	void ThinkHu();										//循环删除牌中已有的组合考虑胡牌
	void ThinkThirty();									//考虑十三张牌的情况

	CUIntArray m_MajList,m_HupaiList;					//手牌与胡牌的链表
public:
	void GetHupai(CUIntArray *arr);						//获得胡牌组
	void SetMjPaiArray(CUIntArray *nPai);				//设置手里的牌
	void SetMjPaiArray(int *nPai,int nNum);				//设置手里的牌
};

#endif // !defined(AFX_MAJHU_H__00A958AE_66FF_4B60_9929_20194583CCB8__INCLUDED_)
