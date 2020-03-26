//Download by http://www.NewXing.com
// Wanjia.cpp: implementation of the CWanjia class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Mahjong.h"
#include "Wanjia.h"
#include "MajHu.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/*************************************************************/
/*函数功能：构造
/*入口参数：无
/*返回类型：无
/*************************************************************/
CWanjia::CWanjia()
{
	m_nFenShu = 500;
	m_nNewPai = -1;
	m_nOutNum = 0;
	m_nStartPai = 0;
	m_nState = -1;
	m_bBoy = true;
	int i;
	for(i = 0;i < 30;i++)
		m_nPaiOut[i]=-1;
	for(i = 0;i < 13;i++)
		m_nPaiInhand[i]=-1;
	for(i = 0;i < 4;i++)
		m_nGang[i] = 0;
}

/*************************************************************/
/*函数功能：获取性别
/*入口参数：无
/*返回类型：布尔，真就是男，假就是女
/*************************************************************/
bool CWanjia::GetSex()
{
	return m_bBoy;
}

/*************************************************************/
/*函数功能：是否有顺子
/*入口参数：顺子的种类type
/*返回类型：布尔,真就是有顺子，假就是没有
/*************************************************************/
bool CWanjia::IsStepTeam(int type)
{
	int nPai1,nPai2;
	if(type == 1)
	{
		nPai1 = m_nOutPai-1;
		nPai2 = m_nOutPai+1;
	}
	else if(type == 0)
	{
		nPai1 = m_nOutPai+1;
		nPai2 = m_nOutPai+2;
	}
	else
	{
		nPai1 = m_nOutPai-1;
		nPai2 = m_nOutPai-2;
	}
	//判断是否是同种样式或者是字
	if( nPai1/9 != nPai2/9 || nPai1/9 != m_nOutPai/9 || nPai1 >= 27 || nPai2 >= 27  )
	{
		return false;
	}
	int pos1,pos2;
	pos1 = FindPaiPos(nPai1,m_nStartPai);
	pos2 = FindPaiPos(nPai2,m_nStartPai);
	//牌1牌2都存在就为真,其余都为假
    return (pos1 != -1 && pos2 != -1);
}

/*************************************************************/
/*函数功能：考虑是否能胡牌
/*入口参数：无
/*返回类型：布尔，真就是有
/*************************************************************/
bool CWanjia::ThinkHupai()
{
	int a[13],i;
	CMajHu mf;
	for(i = 0;i < 13-m_nStartPai;i++)
		a[i] = m_nPaiInhand[i + m_nStartPai];

	m_arrHupai.RemoveAll();
	mf.SetMjPaiArray(a,13 - m_nStartPai);
	mf.GetHupai(&m_arrHupai);

	for( i = 0;i < (int)m_arrHupai.GetSize();i++ )
	{
		//根据状态判断是否胡牌
		if(( m_nOutPai == (int)m_arrHupai.GetAt(i) && m_nState != 1)
			|| (m_nNewPai == (int)m_arrHupai.GetAt(i) && m_nState == 1))
			return true;
	}
	return false;
}

/*************************************************************/
/*函数功能：检测是否有杠和碰
/*入口参数：是否有碰，布尔变量m_bPeng的引用
/*返回类型：布尔，真就是有
/*************************************************************/
bool CWanjia::CheckGang(bool& m_bPeng)
{
	int pos1,pos2,pos3;
	bool bGang = false;
	pos1 = FindPaiPos(m_nOutPai,m_nStartPai);
	if(pos1 != -1)
	{
		pos2=FindPaiPos(m_nOutPai,pos1+1);
		if(pos2 != -1)
		{	
			m_bPeng = true;
			pos3 = FindPaiPos(m_nOutPai,pos2+1);
			if(pos3 != -1)
			{
				bGang = true;
			}
		}
	}
	return bGang;
}

/*************************************************************/
/*函数功能：检测是否能暗杠
/*入口参数：能杠的牌组合
/*返回类型：布尔，真就是有
/*************************************************************/
bool CWanjia::CheckGangSelf(CUIntArray& m_arrGang)
{
	m_arrGang.RemoveAll();
	int i;
	for(i = 0;i < m_nStartPai/3;i++)
	{
		if(m_nPaiInhand[3*i] == m_nPaiInhand[3*i+1] 
			&& m_nPaiInhand[3*i] == m_nPaiInhand[3*i+2])
		{
			int pos=FindPaiPos(m_nPaiInhand[3*i],m_nStartPai);
			if(pos != -1)
			{
				m_arrGang.Add(m_nPaiInhand[3*i]);
			}
			else
			{
				if(m_nPaiInhand[3*i] == m_nNewPai)
				{
					m_arrGang.Add(m_nNewPai);
				}
			}
		}
	}

	SortPai(m_nStartPai);

	for(i = m_nStartPai;i < 10;i++)
	{
		if(m_nPaiInhand[i] == m_nPaiInhand[i+1] && m_nPaiInhand[i] == m_nPaiInhand[i+2])
		{
			if(m_nPaiInhand[i] == m_nPaiInhand[i+3] || m_nPaiInhand[i] == m_nNewPai)
			{
				m_arrGang.Add(m_nPaiInhand[i]);
			}
		}
	}
	if(m_arrGang.GetSize() > 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

/*************************************************************/
/*函数功能：检测能否可以吃
/*入口参数：吃牌组合
/*返回类型：布尔，真就是可以
/*************************************************************/
bool CWanjia::CheckEat(CUIntArray& m_arrEat)
{
	m_arrEat.RemoveAll();
	int pos1,pos2;

	if(IsStepTeam(0))
	{
		pos1 = FindPaiPos(m_nOutPai+1,m_nStartPai);
		pos2 = FindPaiPos(m_nOutPai+2,m_nStartPai);
		m_arrEat.Add(pos1);
		m_arrEat.Add(pos2);
	}
	if(IsStepTeam(1))
	{
		pos1 = FindPaiPos(m_nOutPai-1,m_nStartPai);
		pos2 = FindPaiPos(m_nOutPai+1,m_nStartPai);
		m_arrEat.Add(pos1);
		m_arrEat.Add(pos2);
	}
	if(IsStepTeam(2))
	{
		pos1 = FindPaiPos(m_nOutPai-2,m_nStartPai);
		pos2 = FindPaiPos(m_nOutPai-1,m_nStartPai);
		m_arrEat.Add(pos1);
		m_arrEat.Add(pos2);
	}
	if(m_arrEat.GetSize() > 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}


CWanjia::~CWanjia()
{
	
}

/*************************************************************/
/*函数功能：获得玩家的状态
/*入口参数：无
/*返回类型：整形变量（1就是出牌状态，0就是等待状态）
/*************************************************************/
int CWanjia::GetState()
{
	return m_nState;
}

/*************************************************************/
/*函数功能：设置手中的新牌
/*入口参数：整形变量（牌）
/*返回类型：void
/*************************************************************/
void CWanjia::SetNewPai(int Pai)
{
	m_nState = 1;
	if(Pai >= 0 && Pai < 34)
		m_nNewPai = Pai;
	else
		m_nNewPai = -1;
}

/*************************************************************/
/*函数功能：设置别人出的牌（以便考虑）
/*入口参数：整形牌
/*返回类型：void
/*************************************************************/
void CWanjia::SetOutPai(int Pai)
{
	if(Pai >=0 && Pai < 34)
		m_nOutPai = Pai;
	else
		m_nOutPai = -1;
}

/*************************************************************/
/*函数功能：获得杠牌
/*入口参数：无
/*返回类型：杠牌数组的指针
/*************************************************************/
int* CWanjia::GetGangPai()
{
	return m_nGang;
}

/*************************************************************/
/*函数功能：获得新牌
/*入口参数：无
/*返回类型：整形（新牌）
/*************************************************************/
int CWanjia::GetNewPai()
{
	return m_nNewPai;
}

/*************************************************************/
/*函数功能：获得出了的牌的组合
/*入口参数：void
/*返回类型：出了的牌组合的指针
/*************************************************************/
int* CWanjia::GetOutPai(void)
{
		return m_nPaiOut;
}

/*************************************************************/
/*函数功能：获得出牌数
/*入口参数：无
/*返回类型：整型（出牌数）
/*************************************************************/
int CWanjia::GetOutNum()
{
	return m_nOutNum;
}

/*************************************************************/
/*函数功能：获得分数
/*入口参数：无
/*返回类型：整型
/*************************************************************/
int CWanjia::GetFenshu()
{
	return m_nFenShu;
}

/*************************************************************/
/*函数功能：设置吃
/*入口参数：整型（吃的种类）
/*返回类型：void
/*************************************************************/
void CWanjia::SetChi(int type)
{
	if(m_nOutPai!=-1)
	{
		int pos1,pos2,i;
		switch(type)
		{
		case 2:
			{
				//尾吃
				pos1 = FindPaiPos(m_nOutPai-2,m_nStartPai);
				for( i = pos1;i > m_nStartPai;i--)
				{
					m_nPaiInhand[i] = m_nPaiInhand[i-1];
				}
				m_nPaiInhand[m_nStartPai] = m_nOutPai-2;

				pos2 = FindPaiPos(m_nOutPai-1,m_nStartPai);				
				for( i=pos2;i > m_nStartPai+1;i-- )
				{
					m_nPaiInhand[i] = m_nPaiInhand[i-1];
				}
				
				m_nPaiInhand[m_nStartPai+1] = m_nOutPai-1;
				m_nNewPai=m_nPaiInhand[12];
				for( i = 12;i > m_nStartPai+2;i-- )
				{
					m_nPaiInhand[i]=m_nPaiInhand[i-1];
				}
				m_nPaiInhand[m_nStartPai+2] = m_nOutPai;

				m_nStartPai += 3;
				m_nOutPai = -1;
			}
			break;
		case 1:
			{
				//中吃
				pos1 = FindPaiPos(m_nOutPai-1,m_nStartPai);
				for( i = pos1;i > m_nStartPai;i-- )
				{
					m_nPaiInhand[i] = m_nPaiInhand[i-1];
				}
				m_nPaiInhand[m_nStartPai] = m_nOutPai-1;
				
				pos2 = FindPaiPos(m_nOutPai+1,m_nStartPai);				
				for( i = pos2;i > m_nStartPai+1;i-- )
				{
					m_nPaiInhand[i] = m_nPaiInhand[i-1];
				}
				m_nPaiInhand[m_nStartPai+1] = m_nOutPai+1;

				m_nNewPai = m_nPaiInhand[12];
				for( i = 12;i > m_nStartPai+1;i-- )
				{
					m_nPaiInhand[i] = m_nPaiInhand[i-1];
				}
				m_nPaiInhand[m_nStartPai+1] = m_nOutPai;

				m_nStartPai += 3;
				m_nOutPai = -1;
			}
			break;
		case 0:
			{
				//头吃
				pos1 = FindPaiPos(m_nOutPai+1,m_nStartPai);				
				for( i = pos1;i > m_nStartPai;i-- )
				{
					m_nPaiInhand[i] = m_nPaiInhand[i-1];
				}
				m_nPaiInhand[m_nStartPai] = m_nOutPai+1;

				pos2 = FindPaiPos(m_nOutPai+2,m_nStartPai);				
				for( i = pos2;i > m_nStartPai+1;i-- )
				{
					m_nPaiInhand[i] = m_nPaiInhand[i-1];
				}
				m_nPaiInhand[m_nStartPai+1] = m_nOutPai+2;

				m_nNewPai = m_nPaiInhand[12];
				for( i = 12;i > m_nStartPai;i-- )
				{
					m_nPaiInhand[i] = m_nPaiInhand[i-1];
				}
				m_nPaiInhand[m_nStartPai] = m_nOutPai;

				m_nStartPai += 3;
				m_nOutPai = -1;
			}
			break;
		}
	}
	m_nState=1;
}

/*************************************************************/
/*函数功能：设置碰
/*入口参数：无
/*返回类型：void
/*************************************************************/
void CWanjia::SetPeng()
{
	if(m_nOutPai!=-1)
	{
		int pos1,pos2,i;
		pos1 = FindPaiPos(m_nOutPai,m_nStartPai);
		for( i = pos1;i > m_nStartPai;i-- )
		{
			m_nPaiInhand[i] = m_nPaiInhand[i-1];
		}
		m_nPaiInhand[m_nStartPai] = m_nOutPai;
		
		pos2 = FindPaiPos(m_nOutPai,pos1+1);
		for( i = pos2;i > m_nStartPai;i-- )
		{
			m_nPaiInhand[i] = m_nPaiInhand[i-1];
		}
		m_nPaiInhand[m_nStartPai+1] = m_nOutPai;
		
		m_nNewPai = m_nPaiInhand[12];
		for( i = 12;i > m_nStartPai+1;i-- )
		{
			m_nPaiInhand[i] = m_nPaiInhand[i-1];
		}
		m_nPaiInhand[m_nStartPai+2] = m_nOutPai;
		
		m_nStartPai += 3;
		m_nOutPai = -1;
	}
	m_nState = 1;
}

/*************************************************************/
/*函数功能：设置杠
/*入口参数：整型（要杠的那张牌）
/*返回类型：void
/*************************************************************/
void CWanjia::SetGang(int nPai)
{
	int i,pos1,pos2,pos3;
	pos1 = FindPaiPos(nPai,m_nStartPai);
	pos2 = FindPaiPos(nPai,pos1+1);
	pos3 = FindPaiPos(nPai,pos2+1);
	//如果出牌等于要杠的那张牌(1明杠，2暗杠)
	if(m_nOutPai == nPai)
	{
		for( i = pos1;i > m_nStartPai;i-- )
		{
			m_nPaiInhand[i] = m_nPaiInhand[i-1];
		}
		m_nPaiInhand[m_nStartPai] = nPai;

		pos2 = FindPaiPos(nPai,pos1+1);
		for( i = pos2;i > m_nStartPai+1;i-- )
		{
			m_nPaiInhand[i] = m_nPaiInhand[i-1];
		}
		m_nPaiInhand[m_nStartPai+1] = nPai;

		pos3 = FindPaiPos(nPai,pos2+1);
		for( i = pos3;i > m_nStartPai+2;i-- )
		{
			m_nPaiInhand[i] = m_nPaiInhand[i-1];
		}
		m_nPaiInhand[m_nStartPai+2] = nPai;

		int pos4 = FindPaiPos(nPai,pos3+1);
		m_nPaiInhand[pos4] = m_nNewPai;
		m_nNewPai = -1;

		m_nGang[m_nStartPai/3] = 1;
		m_nStartPai += 3;	
		m_nOutPai = -1;
	}
	else
	{
		if( pos1 != -1 && pos2 != -1 && pos3 != -1 )//在没有固定的牌中找不到就表明是暗杠
		{
			m_nPaiInhand[pos1] = m_nPaiInhand[m_nStartPai];
			m_nPaiInhand[pos2] = m_nPaiInhand[m_nStartPai+1];
			m_nPaiInhand[pos3] = m_nPaiInhand[m_nStartPai+2];

			if(nPai == m_nNewPai)
			{
				m_nNewPai = -1;
			}
			else
			{
				int pos4 = FindPaiPos(nPai,pos3+1);
				m_nPaiInhand[pos4] = m_nNewPai;
				m_nNewPai = -1;
			}
			m_nPaiInhand[m_nStartPai+2] = nPai;
			m_nPaiInhand[m_nStartPai+1] = nPai;
			m_nPaiInhand[m_nStartPai] = nPai;
			
			m_nGang[m_nStartPai/3] = 2;
			m_nStartPai += 3;
			SortPai(m_nStartPai);
		}
		else			
		{
			pos1 = FindPaiPos(nPai,0,m_nStartPai);

			if(nPai == m_nNewPai)
			{
				m_nNewPai = -1;

			}
			else
			{
				int pos4 = FindPaiPos(nPai,m_nStartPai);
				m_nPaiInhand[pos4] = m_nNewPai;
				m_nNewPai = -1;
			}
			m_nGang[pos1/3] = 1;
			SortPai(m_nStartPai);
		}
	}
}

/*************************************************************/
/*函数功能：设置性别（设置一次性别就变一次）
/*入口参数：无
/*返回类型：void
/*************************************************************/
void CWanjia::SetGirl()
{
	m_bBoy =! m_bBoy;
}

/*************************************************************/
/*函数功能：设置手中的牌
/*入口参数：指针（牌的数组），整型（牌的数量）
/*返回类型：void
/*************************************************************/
void CWanjia::SetPaiInHand(int* paiArr,int nNum)
{
	m_nNewPai = -1;
	m_nOutNum = 0;
	m_nStartPai = 0;
	m_nState = -1;
	for(int i = 0;i< 4;i++)
	{
		m_nGang[i]=0;
	}
	for(i = 0;i < nNum;i++)
	{
		m_nPaiInhand[i] = paiArr[i];
	}
	for(i = 0;i < 30;i++)
	{
		m_nPaiOut[i] = -1;
	}
}

/*************************************************************/
/*函数功能：设置分数
/*入口参数：整型（分数）
/*返回类型：void
/*************************************************************/
void CWanjia::SetFenshu(int nFen)
{
	m_nFenShu = m_nFenShu + nFen;
}

/*************************************************************/
/*函数功能：移除最近出的一张牌
/*入口参数：void
/*返回类型：void
/*************************************************************/
void CWanjia::RemoveOutPai(void)
{
	m_nPaiOut[m_nOutNum-1] = -1;
	m_nOutNum--;
}

/*************************************************************/
/*函数功能：电脑出牌
/*入口参数：整型（牌）
/*返回类型：整型（出的那长牌）
/*************************************************************/
int CWanjia::CmpOutPai(int nPai)
{
	if(m_nNewPai != nPai)
	{
		int pos;
		pos = FindPaiPos(nPai,m_nStartPai);
		if(pos != -1)
		{
			m_nPaiInhand[pos] = m_nNewPai;
		}
		SortPai(m_nStartPai);
	}
	m_nPaiOut[m_nOutNum] = nPai;
	m_nNewPai = -1;
	m_nOutPai = -1;
	m_nOutNum++;
	m_nState = -1;
	return nPai;
}	

/*************************************************************/
/*函数功能：按位置出牌
/*入口参数：整型（牌的位置）
/*返回类型：整型（出的那长牌）
/*************************************************************/
int CWanjia::OutPai(int nPlace)
{
	if(nPlace == 14)
	{
		m_nPaiOut[m_nOutNum] = m_nNewPai;
	}
	else
	{
		m_nPaiOut[m_nOutNum] = m_nPaiInhand[nPlace];
		m_nPaiInhand[nPlace] = m_nNewPai;
	}
	SortPai(m_nStartPai);
	m_nNewPai = -1;
	m_nOutPai = -1;
	m_nOutNum++;
	m_nState = -1;
	return m_nPaiOut[m_nOutNum-1];
}

/*************************************************************/
/*函数功能：获得手牌
/*入口参数：void
/*返回类型：指针（牌的数组）
/*************************************************************/
int* CWanjia::GetPaiInHand(void)
{
	return m_nPaiInhand;
}

/*************************************************************/
/*函数功能：获得牌开始的位置（既没有吃碰杠的牌的开始位置）
/*入口参数：无
/*返回类型：整型（位置）
/*************************************************************/
int CWanjia::GetPaiStart()
{
	return m_nStartPai;
}

/*************************************************************/
/*函数功能：整理牌
/*入口参数：整型（整理开始位置），整型（整理结束位置）
/*返回类型：void
/*************************************************************/
void CWanjia::SortPai(int start,int end)
{
	int i,j;
	for(i = start;i < end-1;i++)
	{
		for(j = i;j < end;j++)
		{
			if(m_nPaiInhand[i] > m_nPaiInhand[j])
			{
				m_nPaiInhand[i] = m_nPaiInhand[i]^m_nPaiInhand[j];
				m_nPaiInhand[j] = m_nPaiInhand[i]^m_nPaiInhand[j];
				m_nPaiInhand[i] = m_nPaiInhand[i]^m_nPaiInhand[j];
			}
		}
	}
}

/*************************************************************/
/*函数功能：查找指定牌的位置
/*入口参数：整型（那张牌），整型（从那里开始找），整型（找到那里结束）
/*返回类型：整型（找到的位置，找不到返回-1）
/*************************************************************/
int CWanjia::FindPaiPos(int nPai,int nStart,int nEnd)
{
	for( int i = nStart;i < nEnd;i++ )
	{
		if( m_nPaiInhand[i] == nPai )
		{
			return i;
		}
	}
	return -1;
}
