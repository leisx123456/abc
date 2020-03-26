//Download by http://www.NewXing.com
// ZuoMaj.h: interface for the CZuoMaj class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ZUOMAJ_H__697F1B51_B570_466A_A57E_5992D69AC855__INCLUDED_)
#define AFX_ZUOMAJ_H__697F1B51_B570_466A_A57E_5992D69AC855__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CZuoMaj  
{
private:
	int m_nMaj[136];							//麻将牌
	int m_nZuoPai[4][34];						//左面铺着的麻将（4个方向）
	int m_nMajNum;								//麻将数量
	int m_nWhere;								//麻将取到那个位置
	int m_nWho;									//该取哪个位置的麻将
	int m_nZhuang;								//庄家
	int m_nSezi;								//色子数
	int m_nTailWho;								//最开始拿的哪家的牌
public:
	CZuoMaj();
	virtual ~CZuoMaj();
	void Ruffle();								//洗牌
	void SortMaj();								//按四个方向摆放牌墙
	void SetStart(int nwho,int nwhere);			//设置起始位置（庄家和色子数）
	int GetPai();								//获取当前麻将
	void GetPaiqiang(int** nPaiqiang);			//获取牌墙
	int GetMajNum();							//获得剩余麻将数量
	int GetWho();								//获取该取那个方向
	int GetWhere();								//获取牌取到牌墙的那个位置
	int GetGangPai();							//获得杠了之后的牌
};

#endif // !defined(AFX_ZUOMAJ_H__697F1B51_B570_466A_A57E_5992D69AC855__INCLUDED_)
