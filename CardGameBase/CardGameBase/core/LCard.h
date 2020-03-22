#ifndef _L_CARD_H_
#define _L_CARD_H_
/******************************************************************************
**
** @file
** Card.h
** @brief
**
** @par
** 未处理C++异常
** @author
** leisx
** @date
** 2017-5-4
**
******************************************************************************/

// 运算符重载的继承
// 1、纠正:运算符重载的继承并不需要依赖模板实现
///2、Q:为什么C++赋值运算符重载函数不能被继承？  A:编译器会自动生成一个隐含和默认的“赋值运算符重载函数”
//, 如果不重新实现会被默认的覆盖， 需要依赖模板实现
// 犯了严重的错误， 模板函数右值m_nValue写成了m_nCardValue编译器没有检查出来，导致致命错误
template<class T>
class CLCard
{
public:
	CLCard();
	CLCard(int nCard);
	virtual ~CLCard();

	virtual void initCard();

	// 是否有效
	virtual bool isValid()
	{
		return m_nValue > 1;
	}

	int value()
	{
		return m_nValue;
	}

	// 牌的逻辑值
	virtual int logicValue(){ return m_nValue; }
	virtual int switchToCardIndex(){ return m_nValue; }

	// 牌的花色
	virtual int color(){ return 1; }

	void setValue(int nValue)
	{
		m_nValue = nValue;
	}

	void setScore(int nScore){ m_nScore = nScore; }
	void addScore(int nScore){ m_nScore += nScore; }
	int getScore(){ return m_nScore; }

	virtual bool isMagicCard()
	{
		return m_bMagicCard;
	}

	bool isLocked()
	{
		return m_bLock;
	}
	void lock(){ m_bLock = true; }
	void unLock(){ m_bLock = false; }

	T & operator = (const int & nCard);
	T & operator = (const T & rhs);

	// 重载类型转换操作符
	operator int();

	// operator+
	T operator+(T &rhs)
	{
		T card;
		card.m_nValue = m_nValue + rhs.m_nValue;
		return card;
	}

	// operator-
	CLCard operator-(CLCard &rhs)
	{
		CLCard card;
		card.m_nValue = m_nValue - rhs.m_nValue;
		return card;
	}

	// operator+=
	CLCard operator+=(CLCard &rhs)
	{
		m_nValue += rhs.m_nValue;
		return *this;
	}

	// 前置+
	CLCard operator++()
	{
		++m_nValue;
		return *this;
	}

	// 后置+
	CLCard operator++(int x)
	{
		CLCard card(m_nValue);
		++m_nValue;
		return card;
	}

	// 前置-
	CLCard operator--()
	{
		--m_nValue;
		return *this;
	}

	// 后置-
	CLCard operator--(int x)
	{
		CLCard card(m_nValue);
		--m_nValue;
		return card;
	}

	// operator==
	bool operator ==(int nCard)
	{
		return m_nValue == nCard;
	}
	bool operator == (CLCard rhs)
	{
		return m_nValue == rhs.m_nValue;
	}
	bool operator == (CLCard rhs) const
	{
		return m_nValue == rhs.m_nValue;
	}

	// operator >
	bool operator > (int nCard)
	{
		return m_nValue > nCard;
	}
	bool operator > (CLCard rhs)
	{
		return m_nValue > rhs.m_nValue;
	}

	// operator <
	bool operator < (int nCard) const;

	bool operator < (T rhs)
	{
		return m_nValue < rhs.m_nValue;
	}

	//// operator ^
	//CLCard operator ^ (CLCard rhs)
	//{
	//	m_nValue ^ rhs.m_nValue;
	//}

	//// operator /
	//int operator / (int nVar)
	//{
	//	return m_nValue / nVar;
	//}

	//// operator %
	//int operator % (int nVar)
	//{
	//	return m_nValue % nVar;
	//}

protected:
	int m_nValue;		// 牌值

	bool m_bMagicCard;		// 是否癞子牌

	int m_nScore;				//牌的分值、比重、权重、重要性
	bool m_bLock;

};

template<class T>
void CLCard<T>::initCard()
{

	m_bMagicCard = false;
	m_nScore = 0;
	m_bLock = false;
}



template<class T>
CLCard<T>::CLCard()
: m_nValue(0)
{
	initCard();
}

template<class T>
CLCard<T>::CLCard(int nCard)
: m_nValue(nCard)
{
	initCard(); 

}

template<class T>
CLCard<T>::~CLCard()
{

}

template<class T>
T & CLCard<T>::operator=(const int & nCard)
{
	m_nValue = nCard;
	return *this;
}

template<class T>
T & CLCard<T>::operator=(const T & rhs)
{
	m_nValue = rhs.m_nValue;
	return *this;
}

// CLCard card; int nCard = card
//card是CLCard类型，这里直接把nCard类型转换为int，然后赋值给nCard
//如果没有重载了int()，需要类型转换的话，这里必须写成 nCard = (int)card;
template<class T>
CLCard<T>::operator int()
{
	return m_nValue;
}

template<class T>
bool CLCard<T>::operator<(int nCard) const
{
	return m_nValue < nCard;
}

#endif //_L_CARD_H_