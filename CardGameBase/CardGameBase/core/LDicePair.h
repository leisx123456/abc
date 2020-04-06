#pragma once
#include <time.h>
#include <stdlib.h>
#include "LMjDef.h"

#define DICE_MAX_VALUE	6	// 骰子最大值

unsigned long long GetCurrentTimeMsec();

class CLDice
{
public:
	CLDice()
	{
		m_nValue = 0;
	}

	void rock()
	{
		//srand((unsigned)time(NULL));
		
		m_nValue = rand() % DICE_MAX_VALUE + 1;
	}

	void rock(int nValue) { m_nValue = nValue; }

	int value()
	{
		return m_nValue;
	}

private:
	int m_nValue;	// 当前正面朝上的点数

};


class CLDicePair
{
public:
	CLDicePair();
	~CLDicePair();

	// 骰子总点数
	int total()
	{
		return m_diceFirst.value() + m_diceSecond.value();
	}

	// 较小骰子点数
	int min()
	{
		return m_diceFirst.value() < m_diceSecond.value() ? m_diceFirst.value() : m_diceSecond.value();
	}

	void rock()
	{
#ifdef TEST_LOCAL_LX
		m_diceFirst.rock(4);
		m_diceSecond.rock(1);
#else
		srand(GetCurrentTimeMsec());
		m_diceFirst.rock();
		m_diceSecond.rock();
#endif // TEST_HU


	}

	int dice1(){ return m_diceFirst.value(); }
	int dice2(){ return m_diceSecond.value(); }

	protected:
	CLDice m_diceFirst;
	CLDice m_diceSecond;
};

