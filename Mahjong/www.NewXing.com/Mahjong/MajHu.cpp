//Download by http://www.NewXing.com
// MajHu.cpp: implementation of the CMajHu class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Mahjong.h"
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
/*函数功能：设置牌组
/*入口参数：牌组集合
/*返回类型：void
/*************************************************************/
void CMajHu::SetMjPaiArray(CUIntArray *nPai)
{
	for(int i = 0;i < nPai->GetSize();i++)
	{
		m_MajList.Add(nPai->GetAt(i));
	}
}

/*************************************************************/
/*函数功能：设置牌组
/*入口参数：指针，整型
/*返回类型：void
/*************************************************************/
void CMajHu::SetMjPaiArray(int *nPai,int nNum)
{
	for(int i = 0;i < nNum;i++)
	{
		m_MajList.Add(nPai[i]);
	}
}

/*************************************************************/
/*函数功能：是否有相同组
/*入口参数：整型
/*返回类型：布尔
/*************************************************************/
bool CMajHu::IsSameTeam(int pai)
{
	for(int i = 0;i < m_MajList.GetSize();i++)
	{
		if(pai == (int)m_MajList.GetAt(i))			//如果存在第一张
		{
			for(int j = i+1;j < m_MajList.GetSize();j++)
			{
				if(pai == (int)m_MajList.GetAt(j))
				{
					return true;
				}
			}
		}
	}
	return false;
}

/*************************************************************/
/*函数功能：是否有顺子
/*入口参数：整型，整型
/*返回类型：布尔
/*************************************************************/
bool CMajHu::IsStepTeam(int pai,int type)
{
	int nPai1,nPai2;
	if(type == 1)
	{
		nPai1 = pai-1;
		nPai2 = pai+1;
	}
	else if(type == 2)
	{
		nPai1 = pai+1;
		nPai2 = pai+2;
	}
	else
	{
		nPai1 = pai-1;
		nPai2 = pai-2;
	}
	if( nPai1/9 != nPai2/9 || nPai1/9 != pai/9 
		|| nPai1 >= 27 || nPai2 >= 27  )	//判断是否是同种样式或者是字
	{
		return false;
	}
	bool b1 = false,b2 = false;
	int i,nPaiTemp;
	for(i = 0;i < m_MajList.GetSize();i++)
	{
		nPaiTemp = m_MajList.GetAt(i);
		if(nPai1 == nPaiTemp )
		{
			b1 = true;
			break;
		}
	}
	for(i = 0;i < m_MajList.GetSize();i++)
	{
		nPaiTemp = m_MajList.GetAt(i);
		if(nPai2 == nPaiTemp )
		{
			b2 = true;
			break;
		}
	}
	//牌1牌2都存在就为真,其余都为假
    return (b1 && b2);
}

/*************************************************************/
/*函数功能：删除3张一样的
/*入口参数：整型
/*返回类型：void
/*************************************************************/
void CMajHu::DeleteSameTeam(int pai)
{
	
	int i,nPai;
	for(i = 0;i < m_MajList.GetSize();i++)
	{
		nPai = m_MajList.GetAt(i);
		//判断是否存在该牌，存在就移除
		if(pai == nPai )
		{
			m_MajList.RemoveAt(i);
			break;
		}
	}
	for(i = 0;i < m_MajList.GetSize();i++)
	{
		nPai = m_MajList.GetAt(i);
		if(pai == nPai )
		{
			m_MajList.RemoveAt(i);
			break;
		}
	}
}

/*************************************************************/
/*函数功能：删除顺子
/*入口参数：整型，整型（种类）
/*返回类型：void
/*************************************************************/
void CMajHu::DeleteStepTeam(int pai, int type)
{
	int nPai1,nPai2,nPaiTemp;

	if(type == 1)						//第一种情况（中间的顺子）
	{
		nPai1 = pai-1;
		nPai2 = pai+1;
	}
	else if(type == 2)					//第二种情况（在开头的顺子）
	{
		nPai1 = pai+1;
		nPai2 = pai+2;
	}
	else								//第三种情况（在最后的顺子）
	{
		nPai1 = pai-1;
		nPai2 = pai-2;
	}
	for(int i = 0;i < m_MajList.GetSize();i++)
	{
		nPaiTemp = m_MajList.GetAt(i);
		if(nPai1 == nPaiTemp )			//如果有这张牌就删除
		{
			m_MajList.RemoveAt(i);
			break;
		}
	}
	for(i = 0;i < m_MajList.GetSize();i++)
	{
		nPaiTemp = m_MajList.GetAt(i);
		if(nPai2 == nPaiTemp )
		{
			m_MajList.RemoveAt(i);
			break;
		}
	}
}

/*************************************************************/
/*函数功能：添加胡牌的牌
/*入口参数：整型
/*返回类型：void
/*************************************************************/
void CMajHu::AddHupai(int pai)
{
	if(pai < 0 || pai > 33)
	{
		return;
	}
	int i,nPai;
	for(i = 0;i < m_HupaiList.GetSize();i++)
	{
		nPai = m_HupaiList.GetAt(i);
		if(pai == nPai )				//判断是否有相同的牌在胡牌集合中
		{
			return;
		}
		if(pai < nPai)				//按顺序添加在胡牌组的相应位置以便查找
		{
			break;	
		}
	}
	m_HupaiList.InsertAt(i,pai);
}

/*************************************************************/
/*函数功能：获得胡牌组
/*入口参数：胡牌组集合
/*返回类型：void
/*************************************************************/
void CMajHu::GetHupai(CUIntArray *arr)
{
	if((m_MajList.GetSize()-1)%3)
	{
		return;
	}
	if(m_MajList.GetSize() == 13)			//考虑七对的情况
	{
		ThinkThirty();
	}
	if(m_MajList.GetSize() == 1)			//如果只有一张就只添加当前这一只
	{
		arr->Add(m_MajList.GetAt(0));
		return;
	}
	ThinkHu();
    for(int i = 0;i < m_HupaiList.GetSize();i++)
	{
		arr->Add(m_HupaiList.GetAt(i));
	}
}

/*************************************************************/
/*函数功能：考虑只有4张牌的情况
/*入口参数：无
/*返回类型：void
/*************************************************************/
void CMajHu::ThinkFour()
{
	int nPai1,nPai2,nPai3,nPaiTemp,i,j;
	//循环遍历是否只胡将
	for(i = 0;i < 4;i++)
	{
		nPai1 = m_MajList.GetAt(i);
		m_MajList.RemoveAt(i);
		
		if(IsSameTeam(nPai1))
		{
			DeleteSameTeam(nPai1);
			AddHupai(m_MajList.GetAt(0));
			m_MajList.Add(nPai1);
			m_MajList.Add(nPai1);
		}
		
		if(IsStepTeam(nPai1,1))
		{
			DeleteStepTeam(nPai1,1);
			AddHupai(m_MajList.GetAt(0));
			m_MajList.Add(nPai1-1);
			m_MajList.Add(nPai1+1);
		}
		
		if(IsStepTeam(nPai1,2))
		{
			DeleteStepTeam(nPai1,2);
			AddHupai(m_MajList.GetAt(0));
			m_MajList.Add(nPai1+1);
			m_MajList.Add(nPai1+2);
		}
		
		if(IsStepTeam(nPai1,3))
		{
			DeleteStepTeam(nPai1,3);
			AddHupai(m_MajList.GetAt(0));
			m_MajList.Add(nPai1-2);
			m_MajList.Add(nPai1-1);
		}
		m_MajList.Add(nPai1);
	}
	//循环遍历4张牌中是否有（一对将带门子的胡法）
	for(i = 0;i < 4;i++)
	{
		nPai1 = m_MajList.GetAt(i);
		m_MajList.RemoveAt(i);
		for(j = 0;j < 3;j++)
		{
			nPaiTemp = m_MajList.GetAt(j);
			//判断是否有一对将，有则判断是否有门子
			if(nPai1 == nPaiTemp  )
			{
				m_MajList.RemoveAt(j);
				//剩余的两张牌
				nPai2=m_MajList.GetAt(0);
				nPai3=m_MajList.GetAt(1);
				//如果剩余的两张牌相同（即是胡两个将）
				if(nPai2 == nPai3  )
				{
					AddHupai(nPai1);
					AddHupai(nPai2);
				}
				//如果剩余的两张牌不同并且在一种样式以内，并且都不是字
				if( nPai2/9 == nPai3/9 && nPai2 < 27 && nPai3 < 27 )
				{
					//判断两张牌的门子是那种
					switch(nPai2 - nPai3)
					{
					case 1://倒序门子如6-5，7-6等
						if((nPai2-8)%9)//判断是否边界8
						{
							AddHupai(nPai2+1);
						}
						if((nPai2-1)%9)//判断是否边界1
						{
							AddHupai(nPai2-2);
						}
						break;
					case -1://顺序门子如5-6，7-8等
						if(nPai2%9)//判断是否边界0
						{
							AddHupai(nPai2-1);
						}
						if((nPai2+1-8)%9)//判断是否边界7
						{
							AddHupai(nPai2+2);
						}
						break;
					case 2://倒序间隔一张的门子如4-2，6-4等
						if( nPai2%9)
						{
							AddHupai(nPai2-1);
						}
						break;
					case -2://顺序间隔一张的门子如2-4，4-6等
						if( (nPai2-8)%9)
						{
							AddHupai(nPai2+1);
						}
						break;
					default:
						break;
					}
				}
				m_MajList.Add(nPai1);
			}
		}
		m_MajList.Add(nPai1);	
	}
}

/*************************************************************/
/*函数功能：考虑十三张是否七对的情况
/*入口参数：无
/*返回类型：void
/*************************************************************/
void CMajHu::ThinkThirty()
{
	int i,j,nPai;
	bool bTemp = false;
	CUIntArray m_MajTemp;
	for(i = 0;i < m_MajList.GetSize();i++)
	{
		m_MajTemp.Add(m_MajList.GetAt(i));
	}
	for(i = 0;i < m_MajTemp.GetSize();i++)
	{
		nPai = m_MajTemp.GetAt(i);
		m_MajTemp.RemoveAt(i);
		bTemp = false;
		for(j=0;j<m_MajTemp.GetSize();j++)			//看是否有对子
		{
			if((int)m_MajTemp.GetAt(j)==nPai)		//有就移除这张牌
			{
				m_MajTemp.RemoveAt(j);
				bTemp = true;
			}
		}
		if( !bTemp )								//没有就把刚才移除的加上
		{
			m_MajTemp.Add(nPai);
		}
	}
	if(m_MajTemp.GetSize() == 1)					//如果最后只剩一张就听牌
	{
		m_HupaiList.Add(m_MajTemp.GetAt(0));
	}
}

/*************************************************************/
/*函数功能：考虑胡牌
/*入口参数：无
/*返回类型：void
/*************************************************************/
void CMajHu::ThinkHu()
{
	int nPai,i;
	//如果循环遍历只剩下4张就考虑
	if(m_MajList.GetSize() <= 4)
	{
		ThinkFour();
		return;
	}
	//清楚牌中已有的组合
	for(i = 0;i < m_MajList.GetSize();i++)
	{
		nPai = m_MajList.GetAt(i);
		m_MajList.RemoveAt(i);
		if(IsSameTeam(nPai))
		{
			DeleteSameTeam(nPai);
			ThinkHu();
			m_MajList.Add(nPai);
			m_MajList.Add(nPai);
		}

		if(IsStepTeam(nPai,1))
		{
			DeleteStepTeam(nPai,1);
			ThinkHu();
			m_MajList.Add(nPai-1);
			m_MajList.Add(nPai+1);		
		}
		if(IsStepTeam(nPai,2))
		{
			DeleteStepTeam(nPai,2);
			ThinkHu();
			m_MajList.Add(nPai+1);
			m_MajList.Add(nPai+2);
		}
		if(IsStepTeam(nPai,3))
		{
			DeleteStepTeam(nPai,3);
			ThinkHu();
			m_MajList.Add(nPai-1);
			m_MajList.Add(nPai-2);
		}
		m_MajList.Add(nPai);
	}
	
}