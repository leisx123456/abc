#ifndef _L_CARD_H_
#define _L_CARD_H_
/******************************************************************************
**
** @file
** Card.h
** @brief
**
** @par
** δ����C++�쳣
** @author
** leisx
** @date
** 2017-5-4
**
******************************************************************************/

// ��������صļ̳�
// 1������:��������صļ̳в�����Ҫ����ģ��ʵ��
///2��Q:ΪʲôC++��ֵ��������غ������ܱ��̳У�  A:���������Զ�����һ��������Ĭ�ϵġ���ֵ��������غ�����
//, ���������ʵ�ֻᱻĬ�ϵĸ��ǣ� ��Ҫ����ģ��ʵ��
// �������صĴ��� ģ�庯����ֵm_nValueд����m_nCardValue������û�м�������������������
template<class T>
class CLCard
{
public:
	CLCard();
	CLCard(int nCard);
	virtual ~CLCard();

	virtual void initCard();

	// �Ƿ���Ч
	virtual bool isValid()
	{
		return m_nValue > 1;
	}

	int value()
	{
		return m_nValue;
	}

	// �Ƶ��߼�ֵ
	virtual int logicValue(){ return m_nValue; }
	virtual int switchToCardIndex(){ return m_nValue; }

	// �ƵĻ�ɫ
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

	// ��������ת��������
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

	// ǰ��+
	CLCard operator++()
	{
		++m_nValue;
		return *this;
	}

	// ����+
	CLCard operator++(int x)
	{
		CLCard card(m_nValue);
		++m_nValue;
		return card;
	}

	// ǰ��-
	CLCard operator--()
	{
		--m_nValue;
		return *this;
	}

	// ����-
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
	int m_nValue;		// ��ֵ

	bool m_bMagicCard;		// �Ƿ������

	int m_nScore;				//�Ƶķ�ֵ�����ء�Ȩ�ء���Ҫ��
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
//card��CLCard���ͣ�����ֱ�Ӱ�nCard����ת��Ϊint��Ȼ��ֵ��nCard
//���û��������int()����Ҫ����ת���Ļ����������д�� nCard = (int)card;
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