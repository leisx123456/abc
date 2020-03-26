//Download by http://www.NewXing.com
// CleverThink.cpp: implementation of the CCleverThink class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Mahjong.h"
#include "CleverThink.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCleverThink::CCleverThink()
{
	m_nScoreSeven = 0;
	m_nMaxScoreSeven = 0;
	m_nRemainNumSeven = 0;
	m_nMaxScoreTotal = 0;
	m_nWjOutPaiNum = 0;
}

CCleverThink::~CCleverThink()
{

}

void CCleverThink::SetPaiOut(int* pOutPai,int nNum)
{
	m_nWjOutPaiNum = 0;
	for(int i = 0;i < nNum;i++)
	{
		m_nWjOutPai[i] = pOutPai[i];
		m_nWjOutPaiNum++;
	}
}

int CCleverThink::FindPosOut(int pai)
{
	int nNum = 0;
	for(int i = 0;i < m_nWjOutPaiNum;i++)
	{
		if(m_nWjOutPai[i] == pai)
		{
			nNum++;
		}
	}
	return nNum;
}

void CCleverThink::ThinkSeven()
{
	int i,pos;
	for(i = 0;i < m_nMjNum;i++)
	{
		if(m_bMjSelect[i] == false)
		{
			m_bMjSelect[i] = true;
			if(IsSameTeamSeven(m_nMajiang[i],pos,i))
			{
				m_nScoreSeven += 220;
				m_nCheckNum++;
				m_bMjSelect[pos] = true;
				ThinkSeven();
				m_nScoreSeven -= 220;
				m_nCheckNum--;
			}
		}
	}
	if( m_nScoreSeven > m_nMaxScoreSeven )
	{
		m_nRemainNumSeven = 0;
		m_nMaxScoreSeven=m_nScoreSeven;
		for( i = 0;i <  m_nMjNum;i++ )
		{
			if( m_bMjSelect[i]==false )
			{
				m_nRemainSeven[m_nRemainNumSeven] = m_nMajiang[i];
				m_nRemainNumSeven++;
			}
		}
	}	
}

int CCleverThink::AddGoodOut(int pai)
{
	int nNum,nGood = 0;
	nNum = FindPosOut(pai);
	nGood = (4 - nNum) * 2;									//根据数量添加分数

	if( pai >= 27 )
	{
		return (nGood+10);
	}

	if( pai%9 != 0 && FindPos(pai-1) != -1 )
	{
		if( pai%9 == 1 ) 
		{
			nGood += 3;
		}
		else
		{
			nGood += 1;
		}
	}
	if( pai%9 != 8 && FindPos(pai+1) != -1 )
	{
		if( pai%9 == 7 ) 
		{
			nGood += 3;
		}
		else
		{
			nGood += 1;
		}
	}
	//如果剩余的牌中有比其少二的牌（如3—5,1—3等）
	if( pai%9 > 1 && FindPos(pai-2) != -1 )
	{
		nGood += 2;
	}
	//如果剩余的牌中有比其多二的牌
	if( pai%9 < 7 && FindPos(pai+2) != -1 )
	{
		nGood += 2;
	}

	return nGood;
}

void CCleverThink::ThinkSevenOne()
{
	int nPai;
	int nGood[14];
	int nMin = 33;
	//找出最差的一张牌
	for(int i = 0;i < m_nRemainNumSeven;i++ )
	{
		nPai = m_nRemainSeven[i];
		//如果是字
		if( nPai >= 27 )
		{
			nGood[i] = 10;
		}
		//如果是一或者九
		else if( nPai%9 == 0 || nPai%9 == 8 )
		{
			nGood[i]=6;
		}
		else 
		{
			nGood[i]=2;
		}

		nGood[i] += AddGoodOut(nPai);

		if( nGood[i] < nMin )
		{
			nMin = nGood[i];
			m_nBadly = nPai;
		}
	}
}

/*void CCleverThink::ThinkClever()
{
	int i;
	//如果没有一个组合
	if( m_nMaxScoreThree==0 || m_nRemainNumThree==0 )
	{
		m_nRemainNumThree=m_nMjNum;
		for( i = 0;i < m_nRemainNumThree;i++ )
			m_nRemainThree[i] = m_nMajiang[i];
	}
	if(m_nMjNum >= 13)
	{
		for(i = 0;i < 14;i++)
			m_bMjSelect[i] = false;
		ThinkSeven();
	}
	if((m_nMaxScoreThree + m_nMaxScoreTwo) > m_nMaxScoreSeven)
	{
		//考虑两张的情况
		ThinkTwo();
		if( m_nMaxScoreTwo == 0  )
		{
			m_nRemainNumTwo = m_nRemainNumThree;
			for( i = 0;i < m_nRemainNumTwo;i++ )
				m_nRemainTwo[i] = m_nRemainThree[i];
		}
		m_nMaxScoreTotal = m_nMaxScoreThree + m_nMaxScoreTwo;
		//如果两张牌中都是组合
		if(  m_nRemainNumTwo == 0  )
		{
			CheckOnlyTwo();
			return;
		}

		ThinkOne();
	}
	else
	{
		m_nGoodTeamThree[0] = -1;
		m_nGoodTeamThree[1] = -1;
		m_nGoodTeamThree[2] = -1;
		m_nMaxScoreTotal = m_nMaxScoreSeven;
		ThinkSevenOne();
	}
}*/

int CCleverThink::GetMaxScoreClever()
{
	return m_nMaxScoreTotal;
}

bool CCleverThink::IsSameTeamSeven(int pai,int &pos1,int nStart)
{
	pos1 = FindPos(pai,nStart);
	return pos1 == -1 ? false:true;
}
