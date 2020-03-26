//Download by http://www.NewXing.com
// MahjongDlg.h : header file
//

#include "Wanjia.h"
#include "CmpThink.h"
#include "CleverThink.h"
#include "ZuoMaj.h"
#include "MajHu.h"

#if !defined(AFX_MAHJONGDLG_H__BE37E4CF_7A4F_4452_8280_91342724CEDA__INCLUDED_)
#define AFX_MAHJONGDLG_H__BE37E4CF_7A4F_4452_8280_91342724CEDA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CMahjongDlg dialog

#define TITLE_EAT		100				//吃标题
#define TITLE_PENG		101				//碰标题
#define TITLE_GANG		102				//杠标题
#define TITLE_HU		103				//胡标题
#define TITLE_GUO		104				//过标题
#define TITLE_CLOSE		105				//关闭图标
#define TITLE_MIN		106				//最小化图标
#define TITLE_SET		107				//作弊图标

#define TimeSelect		20				//玩家选择的时间（秒）
#define TimeSezi		15				//色子动画的时间（步）
#define TimeRun			500			//电脑玩家处理牌的时间（毫秒）
									

class CMahjongDlg : public CDialog
{
// Construction
public:
	CMahjongDlg(CWnd* pParent = NULL);	// standard constructor
	void UpdateFrame();	
	void DrawTitle();									//画标题
	void DrawPai();										//画牌
	void DrawPaiUp(int nPai);							//画牌起来
	void DrawPaiQiang();								//画牌墙
	void DrawPaiOut();									//画出了的牌
	void DrawSelect();									//画吃牌的起来
	void DrawSezi();									//画色子
	void DrawActive(int nWho);							//画该谁主动
	void SetPai();										//设置开始时的牌
	void SetZhuang(int nWan);							//设置庄家
	void SetSezi(int nNum);								//设置色子
	void SortWjPai();									//排序玩家的牌
	int CmpThink(int nWho);								//电脑思考将要出那张牌			
	int  CheckNeedGet(int nWho,int pai);				//检测电脑是否需要这张牌
	void  CheckWhoGet();								//获得各玩家的最大优势分
	bool CheckPerson();									//检测人是否可以吃碰等
	int GetMaxWho(int& nMaxScore);						//获得最大优势是谁和其最大优势分是多少
	bool CheckLiuju();									//检测流局
	void  CmpActive(int nWho);							//电脑的行为
	void  PlayPaiSound(int nPai,bool bBoy=true);		//牌的声音
	void  PlayOtherSound(int nType,bool bBoy=true);		//别的声音
	void CountFenshu();									//计算胡牌后的分数
	void MoveTitle(CPoint point);						//鼠标在标题上的移动
	void OnClickTitle(CPoint point);					//点击标题
	void DrawUpPai(int nPlace);							//画牌起来
	void ShowFenshu();									//显示分数
	void ChangBoolFalse();								//把吃胡碰等布尔改成假
	void DrawAllpai();									//画所有的牌

	enum{S_HU,S_GAN,S_EAT,S_PEN,S_LIU};
	CDC m_bgDC,m_memDC,m_paiDC,m_titleDC,m_seziDC,m_numDC;
	int				m_nOutPai;							//出的牌
	bool			m_bStart;							//是否开始
	bool			m_bRun;								//系统运行
	bool			m_bCaton;							//开机动画
	int				m_nZhuang;							//记录庄家
	int				m_nSeNum;							//记录色之数
	int				m_nActive;							//主动权
	int				m_nPanduan;							//判断到谁头上了
	CUIntArray		m_arrHupai[4];						//玩家胡牌组
	CUIntArray		m_arrEat;							//能吃的组合						
	CUIntArray		m_arrGang;							//能杠的组合
	int				m_nCmpGoodScore[4];					//记录电脑的最佳分值
	bool m_bHu,m_bPeng,m_bChi,m_bGang;					//能否吃碰胡杠
	int				m_nGoodScore[4];					//优先级
	bool			m_nAbandon;							//放弃
	bool			m_bSelect;							//选择杠或者吃				
	CRect			m_rtTitle[8];						//标题区域
	int				m_nSelectGang;						//当前选择的杠
	int				m_nSelectEat;						//当前选择的吃
	int				m_nSelectTime;						//选择的时间
	int				m_nWaitTime;						//玩家等待时间
	bool			m_bWait;							//玩家等待状态
	int				m_nHuType;							//胡牌的种类
	int				m_nHuWho;							//谁胡的牌
	int				m_nFirst;							//第一次
	bool			m_bZuobi;							//作弊
	

// Dialog Data
	//{{AFX_DATA(CMahjongDlg)
	enum { IDD = IDD_MAHJONG_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMahjongDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
	CWanjia			m_wjMaj[4];							//4个玩家
	CZuoMaj			m_zmMaj;							//桌面牌墙
	HCURSOR			m_hCursorHand;						//光标
	HCURSOR			m_hCursor;							//光标

	int				m_nMousePai;						//鼠标所在的牌
	int				m_nChupaiUser;						//用户出牌
	int				m_nTitle;							//鼠标所在标题
	bool			m_bHandDown;						//鼠标下压是否下压
	bool			m_bwjState;							//是否玩家选择状态
	bool			m_bOver;							//是否结束

	// Generated message map functions
	//{{AFX_MSG(CMahjongDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAHJONGDLG_H__BE37E4CF_7A4F_4452_8280_91342724CEDA__INCLUDED_)
