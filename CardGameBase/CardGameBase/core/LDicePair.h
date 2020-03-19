#pragma once
#include <time.h>
#include <stdlib.h>

#define DICE_MAX_VALUE	6	// �������ֵ

class CLDice
{
public:
	CLDice()
	{
		rock();
	}

	void rock()
	{
		srand((unsigned)time(NULL));
		m_nValue = rand() % DICE_MAX_VALUE + 1;
	}

	int value()
	{
		return m_nValue;
	}

private:
	int m_nValue;	// ��ǰ���泯�ϵĵ���

};


class CLDicePair
{
public:
	CLDicePair();
	~CLDicePair();

	// �����ܵ���
	int total()
	{
		return m_diceFirst.value() + m_diceSecond.value();
	}

	// ��С���ӵ���
	int min()
	{
		return m_diceFirst.value() < m_diceSecond.value() ? m_diceFirst.value() : m_diceSecond.value();
	}

	void rock()
	{
		m_diceFirst.rock();
		m_diceSecond.rock();
	}

	protected:
	CLDice m_diceFirst;
	CLDice m_diceSecond;
};

