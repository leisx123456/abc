#pragma once
#include "BasicThink.h"


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

