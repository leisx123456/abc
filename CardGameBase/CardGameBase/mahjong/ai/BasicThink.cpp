//#include "stdafx.h"
//#include "Mahjong.h"
#include "BasicThink.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBasicThink::CBasicThink():m_bHaveDui(false)
{

}

CBasicThink::~CBasicThink()
{

}
/*************************************************************/
/*函数功能：设置麻将组
/*入口参数：指针（麻将数组），整型（数量）
/*返回类型：void
/*************************************************************/
void CBasicThink::SetMjPaiArray(int *nPailist, int nNum)
{
	for( int i = 0;i < 14;i++ )
	{
		m_bMjSelect[i] = false;
	}
	m_nCheckNum = 0;
	m_nMaxScoreThree = m_nMaxScoreTwo = m_nMaxScoreOne = 0;
	m_nMjNum = nNum;
	m_nScoreThree = m_nScoreTwo = 0;
	m_nBadly = -1;
	
	for(int j = 0;j < nNum;j++)
	{
		m_nMajiang[j] = nPailist[j];
	}

	m_nNeedPai = nPailist[nNum-1];				//设置需要的牌为最后一张
	SortMj(m_nMajiang,nNum);					//整理牌
}

/*************************************************************/
/*函数功能：整理牌
/*入口参数：指针（牌的数组），整型（结束位置）
/*返回类型：void
/*************************************************************/
void CBasicThink::SortMj(int *who,int end)
{
	int i,j,nPai;
	for( i = 0 ; i < end-1 ; i++ )
	{
		for( j = i+1 ; j < end ; j++ )
            if( who[i] > who[j] )
			{
				nPai =  who[i];
				who[i] = who[j];
				who[j] = nPai;
			}
	}
}

/*************************************************************/
/*函数功能：获得考虑后的最大分数
/*入口参数：无
/*返回类型：整型
/*************************************************************/
int CBasicThink::GetMaxScore()
{
	return m_nMaxScoreThree + m_nMaxScoreTwo;
}

/*************************************************************/
/*函数功能：获得最差的那张牌
/*入口参数：无
/*返回类型：整型
/*************************************************************/
int CBasicThink::GetBadly()
{
	return m_nBadly;
}

/*************************************************************/
/*函数功能：获得别人出牌后组合的最佳组合
/*入口参数：整型，整型，整型
/*返回类型：void
/*************************************************************/
void CBasicThink::GetGoodTeam(int &pai1, int &pai2, int &pai3)
{
	pai1=m_nTotalTeam[0];
	pai2=m_nTotalTeam[1];
	pai3=m_nTotalTeam[2];
}

/*************************************************************/
/*函数功能：查找牌
/*入口参数：整型（要找的那长牌），整型（开始位置）
/*返回类型：整型（位置）
/*************************************************************/
int CBasicThink::FindPos(int pai,int nStart)
{
	for( int i = nStart;i < m_nMjNum;i++ )
	{
		if( m_nMajiang[i] == pai && m_bMjSelect[i] == false )
		{
			return i;
		}
	}
	return -1;
}

/*************************************************************/
/*函数功能：从检测完三张后剩余的牌中查找牌
/*入口参数：整型（要找的那长牌），整型（开始位置）
/*返回类型：整型（位置）
/*************************************************************/
int CBasicThink::FindPosThree(int pai,int nStart)
{
	for( int i = nStart;i < m_nRemainNumThree;i++ )
	{
		if( m_nRemainThree[i] == pai && m_bMjSelect[i] == false )
		{
			return i;
		}
	}
	return -1;
}

/*************************************************************/
/*函数功能：是否有3张一样的
/*入口参数：整型，整型，整型，
/*返回类型：布尔，真就是有
/*************************************************************/
bool CBasicThink::IsSameTeam(int pai,int &pos1,int &pos2)
{
	pos1 = FindPos(pai);
	if( pos1 == -1 )
	{
		return false;
	}
	pos2 = FindPos(pai,pos1+1);
	if( pos2 == -1 )
	{
		return false;
	}
    return true;
}

/*************************************************************/
/*函数功能：是否有3张顺子的
/*入口参数：整型，整型，整型，
/*返回类型：布尔，真就是有
/*************************************************************/
bool CBasicThink::IsStepTeam(int pai,int &pos1,int &pos2)
{
    if( pai >= 27 ) 
	{
		return false;
	}
	int nPai1 = pai+1;
	int nPai2 = pai+2;
	//判断是否有字是否在一个类别中
	if( nPai1/9 != nPai2/9 || nPai1/9 != pai/9 || nPai1 >= 27 || nPai2 >= 27 )
	{
		return false;
	}
	pos1 = FindPos(nPai1);
	pos2 = FindPos(nPai2);
	if( pos1 != -1 && pos2 != -1)
	{
		return true;
	}
	else
	{
		return false;
	}
}

/*************************************************************/
/*函数功能：检测两张组合是否是边缘的（0或8）
/*入口参数：整型，整型
/*返回类型：布尔，真就是有
/*************************************************************/
bool CBasicThink::IsEdge(int pai1,int pai2)
{
	//pai1 < pai2
	if( pai1%9 == 0 || pai2%9 == 8 )
	{
		return true;
	}
	return false;
}

/*************************************************************/
/*函数功能：在三张检测后查找是否有对子
/*入口参数：整型，整型，整型，
/*返回类型：布尔，真就是有
/*************************************************************/
bool CBasicThink::IsSameTeamThree(int pai,int &pos1,int nStart)
{
	pos1 = FindPosThree(pai,nStart);
	return pos1 == -1 ? false:true;
}

/*************************************************************/
/*函数功能：在三长检测后查找是否有两个的组合
/*入口参数：整型，整型，整型，整型
/*返回类型：布尔，真就是有
/*************************************************************/
bool CBasicThink::IsStepTeamThree(int pai,int type,int &pos1,int nStart)
{
    if( pai >= 27 ) 
	{
		return false;
	}
	int nPai1;
	if(type == 1)
	{
		nPai1 = pai+1;
	}
	else 
	{
		nPai1 = pai+2;
	}
	//判断两牌是否同色是否是字
	if(  nPai1/9 != pai/9 || nPai1 >= 27  )
	{
		return false;
	}
	pos1 = FindPosThree(nPai1,nStart);
	return pos1 == -1 ? false:true;
	
}

/*************************************************************/
/*函数功能：设置3张的组合
/*入口参数：整型（种类）
/*返回类型：void
/*************************************************************/
void CBasicThink::SetTeamPai( int type)
{
	switch(type)
	{
	case 0:		//3张一样的
		m_nGoodTeamThree[0] = m_nGoodTeamThree[1] = m_nGoodTeamThree[2] = m_nNeedPai;
		break;
	case 1:
		m_nGoodTeamThree[0] = m_nNeedPai;
		m_nGoodTeamThree[1] = m_nNeedPai+1;
		m_nGoodTeamThree[2] = m_nNeedPai+2;
		break;
	case 2:
		m_nGoodTeamThree[0] = m_nNeedPai-1;
		m_nGoodTeamThree[1] = m_nNeedPai;
		m_nGoodTeamThree[2] = m_nNeedPai+1;
		break;
	case 3:
		m_nGoodTeamThree[0] = m_nNeedPai-2;
		m_nGoodTeamThree[1] = m_nNeedPai-1;
		m_nGoodTeamThree[2] = m_nNeedPai;
		break;
	}
}

