//Download by http://www.NewXing.com
// Wanjia.h: interface for the CWanjia class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WANJIA_H__53022020_6C21_472A_85BA_3339F0D05AC9__INCLUDED_)
#define AFX_WANJIA_H__53022020_6C21_472A_85BA_3339F0D05AC9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CWanjia  
{

private:	
	int			m_nFenShu;									//玩家的分数
	int			m_nPaiInhand[13];							//手牌
	int			m_nStartPai;								//牌的开始位置
	int			m_nGang[4];									//固定牌中是否有杠牌
	int			m_nPaiOut[30];								//打出的牌
	int			m_nOutNum;									//出牌数
	int			m_nNewPai;									//新牌（玩家抓到或别人打出的牌）
	int			m_nOutPai;									//别人出的牌
	int			m_nState;									//状态
	bool		m_bBoy;										//是否男孩
	CUIntArray m_arrHupai;									//听牌
public:
	CWanjia();
	virtual ~CWanjia();
	void SetNewPai(int Pai);								//设置新牌
	void SetOutPai(int Pai);								//设置别人出的牌
	void SetGirl();											//设置成女孩
	int GetNewPai();										//获得新牌
	bool GetSex();											//获取玩家性别
	int* GetOutPai();										//获得出了的牌
	int GetOutNum();										//获得出牌数
	int GetFenshu();										//获得玩家分数
	int GetState();											//获得玩家状态
	int* GetGangPai();										//获得杠牌
	void SetChi(int type);									//设置吃
	void SetPeng();											//设置碰
	void SetGang(int nPai);									//设置杠
	void SetPaiInHand(int* paiArr,int nNum);				//设置手中的牌
	void SetFenshu(int nFen);								//设置玩家分数
	void RemoveOutPai(void);								//删除一张打出的牌
	int OutPai(int nPlace);									//打出出牌
	int CmpOutPai(int nPai);								//计算机出牌
	int* GetPaiInHand();									//获得手中的牌
	int GetPaiStart();										//获得手中开始的牌的位置
	void SortPai(int start=0,int end=13);					//整理牌
	int FindPaiPos(int nPai,int nStart=0,int nEnd=13);		//找出牌
	bool IsStepTeam(int type);								//是否是顺子
	bool ThinkHupai();										//考虑是否胡牌
	bool CheckGang(bool& m_bPeng);							//考虑是否可以杠和碰
	bool CheckGangSelf(CUIntArray& m_arrGang);				//考虑自己是否可以杠
	bool CheckEat(CUIntArray& m_arrEat);					//考虑是否可以吃
};

#endif // !defined(AFX_WANJIA_H__53022020_6C21_472A_85BA_3339F0D05AC9__INCLUDED_)
