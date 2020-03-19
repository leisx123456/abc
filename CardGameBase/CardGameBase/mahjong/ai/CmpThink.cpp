
//#include "stdafx.h"
//#include "Mahjong.h"
#include "CmpThink.h"
#include <Windows.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCmpThink::CCmpThink()
{
	
}

/*************************************************************/
/*函数功能：给单个的牌加利益分
/*入口参数：整型
/*返回类型：整型
/*************************************************************/
int CCmpThink::AddGood(int pai)
{
	int nGood = 0;
	if( pai >= 27 )
	{
		return 0;
	}
	if( pai%9 != 0 && FindPos(pai-1) != -1 )	//如果剩余的牌中有比其少一的牌
	{
		if( pai%9 != 1 )						//如果当前牌不是二类即加3
		{
			nGood += 3;
		}
		else
		{
			nGood += 1;
		}
	}
	if( pai%9 != 8 && FindPos(pai+1) != -1 )	//如果剩余的牌中有比起多一个的牌
	{
		if( pai%9 != 7 )
		{
			nGood += 3;
		}
		else
		{
			nGood += 1;
		}
	}
	if( pai%9 > 1 && FindPos(pai-2) != -1 )		//如果剩余的牌中有比其少二的牌（如3—5,1_3等）
	{
		nGood += 2;
	}
	
	if( pai%9 < 7 && FindPos(pai+2) != -1 )		//如果剩余的牌中有比其多二的牌
	{
		nGood += 2;
	}
	return nGood;
}

/*************************************************************/
/*函数功能：电脑考虑
/*入口参数：无
/*返回类型：void
/*************************************************************/
void CCmpThink::Think()
{
	ThinkThree();											//考虑三张组合的情况
	int i;

	if( m_nMaxScoreThree == 0 || m_nRemainNumThree == 0 )	//如果没有一个组合
	{
		m_nRemainNumThree = m_nMjNum;
		for( i = 0;i < m_nRemainNumThree;i++ )
		{
			m_nRemainThree[i] = m_nMajiang[i];
		}
	}

	ThinkTwo();												//考虑两张的情况
	if( m_nMaxScoreTwo == 0  )
	{
		m_nRemainNumTwo = m_nRemainNumThree;
		for( i = 0;i < m_nRemainNumTwo;i++ )
		{
			m_nRemainTwo[i] = m_nRemainThree[i];
		}
	}

	if(  m_nRemainNumTwo == 0  )							//如果两张牌中都是组合
	{
		CheckOnlyTwo();
		return;
	}

	ThinkOne();
}

/*************************************************************/
/*函数功能：考虑3张
/*入口参数：无
/*返回类型：void
/*************************************************************/
void CCmpThink::ThinkThree()
{
	int pos1,pos2;
	for( int i = 0;i < m_nMjNum;i++ )
	{
		if( m_bMjSelect[i] == false )
		{
			m_bMjSelect[i] = true;
            if( IsSameTeam(m_nMajiang[i],pos1,pos2 ) )
			{
				m_nPaiTeamThree[m_nCheckNum*3] = m_nMajiang[i];
				m_nPaiTeamThree[m_nCheckNum*3+1] = m_nMajiang[i];
				m_nPaiTeamThree[m_nCheckNum*3+2] = m_nMajiang[i];

				if( m_nMajiang[i] == m_nNeedPai )					//设置打出的牌能否组合
				{
					SetTeamPai(0);
				}
				
				m_nScoreThree += 300;
				m_nCheckNum++;
				m_bMjSelect[pos1] = true;
				m_bMjSelect[pos2] = true;
				ThinkThree();
				m_bMjSelect[pos1] = true;
				m_bMjSelect[pos2] = true;
				m_nScoreThree -= 300;
				m_nCheckNum--;
			}
			if( IsStepTeam(m_nMajiang[i],pos1,pos2 ) )
			{
				if( m_nMajiang[i] == m_nNeedPai )
				{
					SetTeamPai(1);
				}
				else if( m_nMajiang[pos1] == m_nNeedPai )
				{
					SetTeamPai(2);
				}
				else if( m_nMajiang[pos2] == m_nNeedPai )
				{
					SetTeamPai(3);
				}
				
				m_nPaiTeamThree[m_nCheckNum*3] = m_nMajiang[i];
				m_nPaiTeamThree[m_nCheckNum*3+1] = m_nMajiang[i]+1;
				m_nPaiTeamThree[m_nCheckNum*3+2] = m_nMajiang[i]+2;
				
				m_nScoreThree += 300;
				m_nCheckNum++;
				m_bMjSelect[pos1] = true;
				m_bMjSelect[pos2] = true;
				ThinkThree();
				m_bMjSelect[pos1] = false;
				m_bMjSelect[pos2] = false;
				m_nScoreThree -= 300;
				m_nCheckNum--;
			}
			m_bMjSelect[i] = false;
		}
	}
		
	if( m_nScoreThree > m_nMaxScoreThree )
	{
		m_nRemainNumThree = 0;
		m_nMaxScoreThree = m_nScoreThree;
		for(int i = 0;i < 3;i++)
		{
			m_nTotalTeam[i] = m_nGoodTeamThree[i];
		}
		int good = 0;
		for(int i = 0;i < m_nMjNum;i++ )
		{
			if( m_bMjSelect[i] == false )
			{
				m_nRemainThree[m_nRemainNumThree] = m_nMajiang[i];
				m_nRemainNumThree++;
			}
		}
	}	
}

/*************************************************************/
/*函数功能：考虑3张后的两张组合
/*入口参数：无
/*返回类型：void
/*************************************************************/
void CCmpThink::ThinkTwo()
{
	int pos1;
	for( int i = 0;i < m_nRemainNumThree;i++ )
		if( m_bMjSelect[i] == false )
		{
			m_bMjSelect[i] = true;
			if( IsSameTeamThree(m_nRemainThree[i],pos1,i ) )				//是否有对子
			{
				int nGoodSame = 90;
				if( m_bHaveDui == false )									//如果没将就设置将
				{
					nGoodSame = 120;
					m_bHaveDui = true;
				}
				m_nScoreTwo += nGoodSame;
				m_bMjSelect[pos1] = true;
				ThinkTwo();
				m_bMjSelect[pos1] = false;
				if( nGoodSame == 120 )
				{
					m_bHaveDui = false;
				}
				m_nScoreTwo -= nGoodSame;
			}

			if( IsStepTeamThree(m_nRemainThree[i],1,pos1,i ) )				//是否有门子
			{
				int nGoodstep;
				if( IsEdge( m_nRemainThree[i],m_nRemainThree[i]+1 ) )
				{
					nGoodstep = 80;
				}
				else 
				{
					nGoodstep = 100;
				}
				m_nScoreTwo += nGoodstep;
				m_bMjSelect[pos1] = true;
				ThinkTwo();
				m_bMjSelect[pos1] = false;
				m_nScoreTwo -= nGoodstep;
			}
			if( IsStepTeamThree(m_nRemainThree[i],2,pos1,i ) )
			{
				int nGoodstep;

				if( IsEdge( m_nRemainThree[i],m_nRemainThree[i]+2 ) )		//如果是边缘门子就加70
				{
					nGoodstep = 70;
				}
				else 
				{
					nGoodstep = 90;
				}
				m_nScoreTwo += nGoodstep;
				m_bMjSelect[pos1] = true;
				ThinkTwo();
				m_bMjSelect[pos1] = false;
				m_nScoreTwo -= nGoodstep;
			}
			m_bMjSelect[i] = false;
		}
		if( m_nScoreTwo > m_nMaxScoreTwo)
		{
			m_nRemainNumTwo = 0;
			m_nMaxScoreTwo = m_nScoreTwo;
			for(int i = 0;i < m_nRemainNumThree;i++ )
			{
				if( m_bMjSelect[i] == false )
				{
					m_nRemainTwo[m_nRemainNumTwo++] = m_nRemainThree[i];
				}
			}
		}
}

/*************************************************************/
/*函数功能：考虑一张的情况
/*入口参数：无
/*返回类型：void
/*************************************************************/
void CCmpThink::ThinkOne()
{
	int pai;
	int nGood[14];
	int nMin = 33;
	for(int i = 0;i < m_nRemainNumTwo;i++ )	//找出最差的一张牌
	{
		pai = m_nRemainTwo[i];

		if( pai >= 27 )						//如果是字
		{
			nGood[i] = 2;
		}
		else if( pai%9 == 0 || pai%9 == 8 )	//如果是一或者九
		{
			nGood[i] = 6;
		}
		else 
		{
			nGood[i] = 10;
		}

		nGood[i] += AddGood(pai);
		if( nGood[i] < nMin )
		{
			nMin = nGood[i];
			m_nBadly = pai;
		}
	}
}

/*************************************************************/
/*函数功能：检测只剩两张的情况
/*入口参数：无
/*返回类型：void
/*************************************************************/
void CCmpThink::CheckOnlyTwo()
{
	int two[14],num = 0;
	bool bSameHave = false;
	int max = 100;
	int score[8];
	bool bTeamHave = false;
	for( int i = 0;i < m_nRemainNumThree;i++ )
		two[num++] = m_nRemainThree[i];

	if( num == 2 )												//如果只有两张牌
	{
		if(two[0] == two[1])									//如果仅剩的两张牌相同就胡牌
		{
			m_nBadly = 1000;
			return;
		}
		for( int j = 0;j < m_nMjNum-1;j++ )						//如果两张牌不同就凑一对将
		{
			if( m_nMajiang[j] == m_nMajiang[j+1] 
				&& m_nMajiang[j] == m_nMajiang[j+2] )			//看是否有三个的
			{
				bTeamHave = true;
				m_nBadly = m_nMajiang[j];
				break;
			}
		}
		if( bTeamHave == false )
		{
			m_nRemainNumTwo = 2;
			m_nRemainTwo[0] = two[0];
			m_nRemainTwo[1] = two[1];
			ThinkOne();
		}
	}
	else
	{
		for( int j = 0;j < num/2;j++ )					//循环纪录每张牌的分数
		{
			if( two[j*2] == two[j*2 + 1] )
			{
				if( bSameHave )
				{
					score[j] = 6;
				}
				else 
				{
					score[j] = 8;
					bSameHave = true;
				}
			}
			else if( two[j*2] == two[j*2+1] - 1 )
			{
				if( two[j*2]%9 == 0 || two[j*2+1]%9 == 8 )
				{
					score[j] = 4;
				}
				else
				{
					score[j] = 7;
				}
			}
			else
			{
				if( two[j*2]%9 == 0 || two[j*2+1]%9 == 8 )
					score[j] = 3;
				else
					score[j] = 5;
			}
			
		}
		for( int k = 0;k < num/2;k++)						//找出分数最小的一张牌就是最差的一张
		{
			if( score[k] < max )
			{
				max = score[k];
				if( two[k*2] % 9 == 0 )
				{
					m_nBadly = two[k*2];
				}
				else if( two[k*2+1] % 9 == 8 )
				{
					m_nBadly = two[k*2+1];
				}
				else 
				{
					m_nBadly = two[k*2 + rand()%2];
				}
			}
		}
	}
}