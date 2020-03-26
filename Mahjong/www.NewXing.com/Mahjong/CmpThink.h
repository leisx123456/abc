//Download by http://www.NewXing.com
// CmpThink.h: interface for the CCmpThink class.
//
//////////////////////////////////////////////////////////////////////
#include "BasicThink.h"


#if !defined(AFX_CMPTHINK_H__69461101_C669_42EB_B306_5099EC88B2D3__INCLUDED_)
#define AFX_CMPTHINK_H__69461101_C669_42EB_B306_5099EC88B2D3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CCmpThink:public CBasicThink
{
public:	
	CCmpThink();											//构造函数
	void CheckOnlyTwo();									//考虑剔除两张后都是门子或对子的情况
	void Think();											//思考得出最大分值

protected:
	int AddGood(int pai);
	void ThinkThree();										//考虑三张的情况(把已有的组合剔除)
	void ThinkTwo();										//(在三张组合剔除后)考虑2张的情况
	void ThinkOne();										//(在两张组合剔除后)考虑1张牌的最差牌
};

#endif // !defined(AFX_CMPTHINK_H__69461101_C669_42EB_B306_5099EC88B2D3__INCLUDED_)
