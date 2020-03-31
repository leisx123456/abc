// TestTemp.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>

template<class T>
class AA
{
public:
	AA();
	AA(int value);
	virtual ~AA();

	//////////////////////////////////////////////////////////////////////////
	/*fun1*/
	bool operator < (T rhs)
	{
		return m_nValue < rhs.m_nValue;
	}

	int m_nValue;
};

template<class T>
AA<T>::~AA()
{

}

template<class T>
AA<T>::AA(int value)
: m_nValue(value)
{

}

template<class T>
AA<T>::AA()
{

}

//////////////////////////////////////////////////////////////////////////
/*fun2*/
template<class T>
bool operator < (T const & lts, T const & rhs)
{
	return lts.m_nValue < rhs.m_nValue;
}




/////
//////////////////////////////////////////////////////////////////////////
class BB : public AA<BB>
{
public:
	BB() : AA(){}
	BB(int value) : AA(value){}
};


class CC : public BB
{
public:
	CC() : BB(){}
	CC(int value) : BB(value){}

private:

};

// 类模板
template<class AA>
class COperateAA
{
public:
	bool compAA(AA aa1, AA aa2);
};


template<class AA>
bool COperateAA<AA>::compAA(AA aa1, AA aa2)
{
	return aa1 < aa2;
}

template<>
bool COperateAA<int>::compAA(int a, int b)
{
	return a < b;
}

template<class AA>
class COperateAA<AA*> //(需要用到指针)
{
public:

	bool compAA(AA* paa1, AA* paa2);
};

template<class AA>
bool COperateAA<AA*>::compAA(AA* paa1, AA* paa2)
{
	return *paa1 < *paa2;
}





class COperateBB : public COperateAA<BB>
{

};



int _tmain(int argc, _TCHAR* argv[])
{

	AA<int> aa(10);
	AA<int> aa2(11);
	if (aa < aa2)	//fun2 必须实现fun2
	{
	std::cout << "true" << std::endl;
	}

	BB bb1(12);
	BB bb2(13);
	if (bb1 < bb2)	//fun2
	{
		std::cout << "true" << std::endl;
	}

	CC cc1(14);
	CC cc2(15);
	if (cc1 < cc2)	//fun2 说明oprate<被继承下来了
	{
		std::cout << "true" << std::endl;
	}

	CC* pcc1 = &cc1;
	CC* pcc2 = &cc2;

	// fun2存在的情况下都调用fun2
	BB* pbb1 = &bb1;
	BB* pbb2 = &bb2;
	COperateBB oprateBB;
	oprateBB.compAA(bb1, bb2);
	//oprateBB.compAA(pbb1, pbb2); // 没有被特化

	oprateBB.compAA(cc1, cc2);

	// ok
	COperateAA<BB*> oprateAA;
	bool bLessThan = oprateAA.compAA(pbb1, pbb2);
	std::cout << "bLessThan:" << std::boolalpha << bLessThan << std::endl;
	COperateAA<CC*> oprateAA2;
	bLessThan = oprateAA2.compAA(pcc1, pcc2);
	std::cout << "bLessThan:" << std::boolalpha << bLessThan << std::endl;

	getchar();

	return 0;
}

