//Download by http://www.NewXing.com
// ZuoMaj.cpp: implementation of the CZuoMaj class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Mahjong.h"
#include "ZuoMaj.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/*************************************************************/
/*函数功能：构造函数（设置几个参数）
/*入口参数：无
/*返回类型：无
/*************************************************************/
CZuoMaj::CZuoMaj()
{
	m_nWhere = 0;
	m_nWho = -1;
	m_nTailWho = -1;
}

CZuoMaj::~CZuoMaj()
{

}

/*************************************************************/
/*函数功能：获得杠牌
/*入口参数：无
/*返回类型：整型
/*************************************************************/
int CZuoMaj::GetGangPai()
{	
	int nPai = m_nZuoPai[m_nTailWho][m_nSezi-1];
	m_nZuoPai[m_nTailWho][m_nSezi-1] = -1;
	m_nZuoPai[m_nTailWho][m_nSezi-2] = -1;
	m_nSezi = m_nSezi-2;
	//如果要拿的那方向的牌没有了就移到下一家
	if(m_nSezi <= 0)
	{
		m_nSezi = 33;
		m_nTailWho++;
		if(m_nTailWho > 3)
		{
			m_nTailWho = 0;
		}
	}
	m_nMajNum = m_nMajNum-2;
	return nPai;
}

/*************************************************************/
/*函数功能：获得牌墙剩余牌的数量
/*入口参数：无
/*返回类型：整型
/*************************************************************/
int CZuoMaj::GetMajNum()
{
	return m_nMajNum;
}

/*************************************************************/
/*函数功能：洗牌
/*入口参数：无
/*返回类型：void
/*************************************************************/
void CZuoMaj::Ruffle()
{
	int nTemp[136];
	int i,j,nI,k,nJ;
	for(i = 0;i < 4;i++)
	{
		for(j = 0;j < 34;j++)
		{
			m_nZuoPai[i][j] = -1;
		}
	}
	for(i = 0,j = 0;i < 136;i++)
	{
		m_nMaj[i] = j;
		if( i != 0 && i%4 == 0 ) 
		{
			j++;
		}
	}

	//设置随即数种子
	srand((unsigned)time(NULL));
	for(nI = 0;nI < 136;nI++)
	{
		k = rand()%(136-nI);
		nTemp[nI] = m_nMaj[k];
		for(nJ = k;nJ < 135-nI;nJ++)
		{
			m_nMaj[nJ] = m_nMaj[nJ+1];
		}
	}
	for(nI = 0;nI < 136;nI++)
	{
		m_nMaj[nI] = nTemp[nI];
	}
	m_nMajNum = 136;
}

/*************************************************************/
/*函数功能：把牌分派到4面牌墙
/*入口参数：无
/*返回类型：void
/*************************************************************/
void CZuoMaj::SortMaj()
{
	int nI,nJ=0;
	//按顺序排序
	for(nI=0;nI<136;nI=nI+4,nJ++)
	{
		for(int i=0;i<4;i++)
		{
			m_nZuoPai[i][nJ]=m_nMaj[nI+i];
		}
	}
}

/*************************************************************/
/*函数功能：获取一张牌
/*入口参数：无
/*返回类型：整型
/*************************************************************/
int CZuoMaj::GetPai()
{
	int pai;
	m_nMajNum--;
	pai=m_nZuoPai[m_nWho][m_nWhere];
	m_nZuoPai[m_nWho][m_nWhere]=-1;
	if(m_nWhere==33)
	{
		m_nWhere=0;
		m_nWho=m_nWho-1;
		if(m_nWho<0)
		{
			m_nWho=3;
		}
	}
	else
	{
		m_nWhere++;
	}
	return pai;
}

/*************************************************************/
/*函数功能：设置庄家和色子数
/*入口参数：整型（庄家），整型（从那里开始拿）
/*返回类型：void
/*************************************************************/
void CZuoMaj::SetStart(int nwho,int nwhere)
{
	m_nWhere=nwhere;
	int nA=nwho;
	for(int i = 0;i < nwhere/2-1;i++)
	{
		nA=nA-1;
		if(nA<0)
		{
			nA=3;
		}
	}
	m_nWho=nA;
	m_nTailWho=nA;
	m_nZhuang=nwho;
	m_nSezi=nwhere;
}

/*************************************************************/
/*函数功能：获得牌拿到哪个位置
/*入口参数：无
/*返回类型：整型
/*************************************************************/
int CZuoMaj::GetWhere()
{
	return m_nWhere;
}

/*************************************************************/
/*函数功能：获得该取谁的牌
/*入口参数：无
/*返回类型：整型
/*************************************************************/
int CZuoMaj::GetWho()
{
	return m_nWho;
}

/*************************************************************/
/*函数功能：获得牌墙
/*入口参数：指针的指针（二唯数组4个方向每个方向34张牌）
/*返回类型：void
/*************************************************************/
void CZuoMaj::GetPaiqiang(int** nPaiqiang)
{
	int i,j;
	for(i=0;i<4;i++)
	{
		for(j=0;j<34;j++)
		{
			nPaiqiang[i][j]=m_nZuoPai[i][j];
		}
	}
}
