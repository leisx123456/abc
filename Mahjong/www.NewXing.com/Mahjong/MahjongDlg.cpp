//Download by http://www.NewXing.com
// MahjongDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Mahjong.h"
#include "MahjongDlg.h"

#include <wingdi.h>

#include <mmsystem.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/////////////////////////////////////////////////////////////////////////////
// CMahjongDlg dialog



CMahjongDlg::CMahjongDlg(CWnd* pParent /*=NULL*/)
: CDialog(CMahjongDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMahjongDlg)
	// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hCursor = AfxGetApp()->LoadCursor (IDC_CURSOR);
	m_hCursorHand = AfxGetApp()->LoadCursor (IDC_CURSOR_Hand);
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMahjongDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMahjongDlg)
	// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMahjongDlg, CDialog)
//{{AFX_MSG_MAP(CMahjongDlg)
ON_WM_PAINT()
ON_WM_QUERYDRAGICON()
ON_WM_TIMER()
ON_WM_MOUSEMOVE()
ON_WM_SETCURSOR()
ON_WM_LBUTTONUP()
ON_WM_LBUTTONDOWN()
ON_WM_CREATE()
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMahjongDlg message handlers

BOOL CMahjongDlg::OnInitDialog()
{
	SetWindowText("麻将");
	MoveWindow(0,0,800,600);
	CenterWindow();

	
	m_bgDC.CreateCompatibleDC(GetDC());
	m_paiDC.CreateCompatibleDC(GetDC());
	m_memDC.CreateCompatibleDC(GetDC());
	m_titleDC.CreateCompatibleDC(GetDC());	
	m_seziDC.CreateCompatibleDC(GetDC());
	m_numDC.CreateCompatibleDC(GetDC());

	CBitmap bm;
	bm.LoadBitmap(IDB_BITMAP_Bg);
	m_bgDC.SelectObject(&bm);
	bm.DeleteObject();
	bm.LoadBitmap(IDB_BITMAP_Maj);
	m_paiDC.SelectObject(&bm);
	bm.DeleteObject();
	bm.LoadBitmap(IDB_BITMAP_Title);
	m_titleDC.SelectObject(&bm);
	bm.DeleteObject();
	bm.LoadBitmap(IDB_BITMAP_Sezi);
	m_seziDC.SelectObject(&bm);
	bm.DeleteObject();
	bm.LoadBitmap(IDB_BITMAP_Num);
	m_numDC.SelectObject(&bm);
	bm.DeleteObject();


	bm.CreateCompatibleBitmap(GetDC(),800,600);
	m_memDC.SelectObject(&bm);
	bm.DeleteObject();
	m_bgDC.BitBlt(692,20,18,18,&m_titleDC,108,80,SRCCOPY);
	m_bgDC.BitBlt(710,20,18,18,&m_titleDC,54,80,SRCCOPY);
	m_bgDC.BitBlt(728,20,18,18,&m_titleDC,0,80,SRCCOPY);
	
	m_memDC.BitBlt(0,0,800,600,&m_bgDC,0,0,SRCCOPY);
	m_memDC.BitBlt(370,400,55,28,&m_titleDC,0,127,SRCCOPY);
	m_nMousePai=-1;

	//初始化参数
	m_bHandDown=false;
	m_bOver=false;
	m_bZuobi=false;
	m_bStart=true;
	m_bRun=false;
	m_bCaton=false;

	m_nSelectEat=-1;
	m_nSelectGang=-1;

	//设置玩家1和3为女孩
	m_wjMaj[1].SetGirl();
	m_wjMaj[3].SetGirl();
	SetZhuang(0);

	//设置几个标题的区域
	m_rtTitle[0].SetRect(590,444,614,468);
	m_rtTitle[1].SetRect(619,444,643,468);
	m_rtTitle[2].SetRect(648,444,672,468);
	m_rtTitle[3].SetRect(678,444,702,468);
	m_rtTitle[4].SetRect(713,444,737,468);
	m_rtTitle[5].SetRect(710,20,728,38);
	m_rtTitle[6].SetRect(728,20,746,38);
	m_rtTitle[7].SetRect(692,20,710,38);

	//播放背景音乐
	sndPlaySound("Sounds\\bg.wav",SND_ASYNC|SND_LOOP);

	CDialog::OnInitDialog();
	
	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMahjongDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting
		
		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);
		
		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;
		
		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
		UpdateFrame();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMahjongDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

/*************************************************************/
/*函数功能：播放除麻将以外的声音
/*入口参数：整型（种类），布尔（是否男）
/*返回类型：void
/*************************************************************/
void CMahjongDlg::PlayOtherSound(int nType,bool bBoy)
{
	CString strFile;
	switch(nType)
	{
	case S_HU:
		strFile = "胡.wav";
		break;
	case S_GAN:
		strFile = "杠.wav";
		break;
	case S_EAT:
		strFile = "吃.wav";
		break;
	case S_PEN:
		strFile = "碰.wav";
		break;
	case S_LIU:
		strFile = "流局.wav";
		break;
	}
	
	if( bBoy )
	{
		strFile = "boy\\"+strFile;
	}
	else
	{
		strFile = "girl\\"+strFile;
	}
	
	::sndPlaySound("Sounds\\"+strFile,SND_ASYNC|SND_NODEFAULT);		
}

/*************************************************************/
/*函数功能：播放麻将声音
/*入口参数：整体（麻将），布尔（是否男）
/*返回类型：void
/*************************************************************/
void CMahjongDlg::PlayPaiSound(int nPai,bool bBoy)
{
	CString strFile;
	if( nPai < 9 )
	{
		strFile.Format("%d万.wav",nPai%9+1);
	}
	else if( nPai < 18 )
	{
		strFile.Format("%d条.wav",nPai%9+1);
	}
	else if( nPai < 27 )
	{
		strFile.Format("%d饼.wav",nPai%9+1);
	}
	else
	{
		switch(nPai)
		{
		case 27:
			strFile = "东风.wav";
			break;
		case 28:
			strFile = "南风.wav";
			break;
		case 29:
			strFile = "西风.wav";
			break;
		case 30:
			strFile = "北风.wav";
			break;
		case 31:
			strFile = "中.wav";
			break;
		case 32:
			strFile = "发.wav";
			break;
		case 33:
			strFile = "白.wav";
			break;
		}
	}
	if( bBoy )
	{
		strFile = "boy\\"+strFile;
	}
	else
	{
		strFile = "girl\\"+strFile;
	}
	::sndPlaySound("Sounds\\"+strFile,SND_ASYNC);		
	
}

/*************************************************************/
/*函数功能：电脑的状态为出牌时思考
/*入口参数：整型（那一家电脑）
/*返回类型：整型（返回要出的牌，胡了返回1000，杠返回负数）
/*************************************************************/
int CMahjongDlg::CmpThink(int nWho)
{
	int a[14],i,j,k;
	int nStar,nNew;
	nStar = m_wjMaj[nWho].GetPaiStart();
	nNew = m_wjMaj[nWho].GetNewPai();

	if(m_wjMaj[nWho].ThinkHupai())							//考虑是否自摸
	{
		return 1000;
	}
	for(i = 0;i < 13-nStar;i++)
	{
		a[i]  =m_wjMaj[nWho].GetPaiInHand()[i+nStar];
	}
	a[13-nStar] = nNew;
	CUIntArray caGang;

	CCmpThink ct;
	ct.SetMjPaiArray(a,14-nStar);
	ct.Think();
	
	m_nCmpGoodScore[nWho] = ct.GetMaxScore();					//组合后记录最大分

	if(m_wjMaj[nWho].CheckGangSelf(caGang))						//如果手里的牌能杠，就检测是否杠后不影响分
	{
		for(i = 0;i < caGang.GetSize();i++)
		{	
			for(j = 0,k = 0;j < 13-nStar;j++)
			{
				if(m_wjMaj[nWho].GetPaiInHand()[i+nStar] != (int)caGang.GetAt(i))
				{
					a[k] = m_wjMaj[nWho].GetPaiInHand()[i+nStar];
					k++;
				}
			}
			if((int)caGang.GetAt(i) != nNew)
			{
				a[k] = nNew;
				k++;
			}
			CCmpThink ctTemp;
			ctTemp.SetMjPaiArray(a,k);
			ctTemp.Think();
			if(ctTemp.GetMaxScore() >= m_nCmpGoodScore[nWho]-300)
			{
				return caGang.GetAt(i)-100;
			}
		}

	}
	int ret = ct.GetBadly();									//获得最差的一张牌
	return ret;
}

/*************************************************************/
/*函数功能：更新屏幕
/*入口参数：无
/*返回类型：void
/*************************************************************/
void CMahjongDlg::UpdateFrame()
{
	if(m_bRun)
	{
		//画箭头
		DrawActive(m_nActive);	
		int nFirst,nSecond;
		//画中间的秒表
		if(m_bwjState || (m_bWait && m_wjMaj[0].GetState() == 1))
		{
			if(m_bwjState)
			{
				nFirst = m_nSelectTime/10;
				nSecond = m_nSelectTime%10;
			}
			if(m_bWait && m_wjMaj[0].GetState() == 1)
			{
				nFirst = m_nWaitTime/10;
				nSecond = m_nWaitTime%10;
			}		
			COLORREF rf = m_numDC.GetPixel(0,0);
			TransparentBlt(m_memDC.GetSafeHdc(),394,264,17,29,m_numDC.GetSafeHdc(),nFirst*17,0,17,29,rf);
			TransparentBlt(m_memDC.GetSafeHdc(),414,264,17,29,m_numDC.GetSafeHdc(),nSecond*17,0,17,29,rf);
		}
	}
	//显示缓存DC
	CClientDC dc(this);
	dc.BitBlt(0,0,800,600,&m_memDC,0,0,SRCCOPY);
}

/*************************************************************/
/*函数功能：打印分数
/*入口参数：无
/*返回类型：void
/*************************************************************/
void CMahjongDlg::ShowFenshu()
{
	//打印分数
	int nGet,nRemove;
	char str[8][128];
	switch(m_nHuType)
	{
	case 1:
		{
			nGet = 150;
			nRemove = -50;
			sprintf(str[7],"有人点炮了");
		}
		break;
	case 2:
		{
			nGet = 300;
			nRemove = -100;
			sprintf(str[7],"俺自摸了");
		}
		break;
	case 3:
		{
			nGet = 450;
			nRemove = -150;
			sprintf(str[7],"哈哈,大胡");
		}
	case 4:
		{
			nGet = 0;
			nRemove = 0;
			sprintf(str[7],"哎,流局");
		}
		break;
	}

	//根据谁胡的牌显示分数统计结果
	switch(m_nHuWho)
	{
	case 0:	
		{
			sprintf(str[0],"下家分数:  %-6d ( +%d )",m_wjMaj[0].GetFenshu(),nGet);	
			sprintf(str[1],"右家分数:  %-6d ( %d )",m_wjMaj[1].GetFenshu(),nRemove);
			sprintf(str[2],"上家分数:  %-6d ( %d )",m_wjMaj[2].GetFenshu(),nRemove);
			sprintf(str[3],"左家分数:  %-6d ( %d )",m_wjMaj[3].GetFenshu(),nRemove);
		}
		break;
	case 1:
		{
			sprintf(str[0],"右家分数:  %-6d ( +%d )",m_wjMaj[1].GetFenshu(),nGet);	
			sprintf(str[1],"下家分数:  %-6d ( %d )",m_wjMaj[0].GetFenshu(),nRemove);
			sprintf(str[2],"上家分数:  %-6d ( %d )",m_wjMaj[2].GetFenshu(),nRemove);
			sprintf(str[3],"左家分数:  %-6d ( %d )",m_wjMaj[3].GetFenshu(),nRemove);
		}
		break;
	case 2:
		{
			sprintf(str[0],"上家分数:  %-6d ( +%d )",m_wjMaj[2].GetFenshu(),nGet);	
			sprintf(str[1],"下家分数:  %-6d ( %d )",m_wjMaj[0].GetFenshu(),nRemove);
			sprintf(str[2],"右家分数:  %-6d ( %d )",m_wjMaj[1].GetFenshu(),nRemove);
			sprintf(str[3],"左家分数:  %-6d ( %d )",m_wjMaj[3].GetFenshu(),nRemove);
		}
		break;
	case 3:
		{
			sprintf(str[0],"左家分数:  %-6d ( +%d )",m_wjMaj[3].GetFenshu(),nGet);	
			sprintf(str[1],"下家分数:  %-6d ( %d )",m_wjMaj[0].GetFenshu(),nRemove);
			sprintf(str[2],"右家分数:  %-6d ( %d )",m_wjMaj[1].GetFenshu(),nRemove);
			sprintf(str[3],"上家分数:  %-6d ( %d )",m_wjMaj[2].GetFenshu(),nRemove);
		}
		break;
	}
	sprintf(str[4],"赢家：");
	sprintf(str[5],"输家：");
	sprintf(str[6],"%6d",nGet);
	CFont ft,*oldft;
	//设置字体
	ft.CreatePointFont(110,"宋体",NULL);
	oldft=m_memDC.SelectObject(&ft);
	m_memDC.SetTextColor(RGB(198,145,86));
	m_memDC.SetBkMode(TRANSPARENT);
	CRect rect0(320,223,800,243);
	CRect rect1(320,270,800,290);
	CRect rect2(320,290,800,310);
	CRect rect3(320,310,800,330);
	CRect rect4(240,223,800,243);
	CRect rect5(240,260,800,300);
	CRect rect6(300,360,800,400);
	CRect rect7(330,170,480,200);
	m_memDC.DrawText(str[0],rect0,DT_LEFT | DT_SINGLELINE | DT_VCENTER     );
	m_memDC.DrawText(str[1],rect1,DT_LEFT | DT_SINGLELINE | DT_VCENTER     );
	m_memDC.DrawText(str[2],rect2,DT_LEFT | DT_SINGLELINE | DT_VCENTER     );
	m_memDC.DrawText(str[3],rect3,DT_LEFT | DT_SINGLELINE | DT_VCENTER     );
	ft.DeleteObject();
	ft.CreatePointFont(130,"华文行楷",NULL);
	m_memDC.SelectObject(ft);
	m_memDC.SetTextColor(RGB(198,145,86));
	m_memDC.DrawText(str[4],rect4,DT_LEFT | DT_SINGLELINE | DT_VCENTER     );
	m_memDC.DrawText(str[5],rect5,DT_LEFT | DT_SINGLELINE | DT_VCENTER     );
	ft.DeleteObject();
	ft.CreatePointFont(180,"黑体",NULL);
	m_memDC.SelectObject(ft);
	m_memDC.SetTextColor(RGB(198,145,86));
	m_memDC.DrawText(str[6],rect6,DT_LEFT | DT_SINGLELINE | DT_VCENTER     );
	ft.DeleteObject();
	ft.CreatePointFont(180,"隶书",NULL);
	m_memDC.SelectObject(ft);
	m_memDC.SetTextColor(RGB(232,210,140));
	m_memDC.DrawText(str[7],rect7,DT_CENTER | DT_SINGLELINE | DT_VCENTER     );
	m_memDC.SelectObject(oldft);

}

/*************************************************************/
/*函数功能：设置庄家
/*入口参数：整型
/*返回类型：void
/*************************************************************/
void CMahjongDlg::SetZhuang(int nWan)
{
	if(nWan >= 0&&nWan < 4)
	{
		m_nZhuang = nWan;
	}
	else
	{
		m_nZhuang = 0;
	}
}

/*************************************************************/
/*函数功能：设置色子数
/*入口参数：整型
/*返回类型：void
/*************************************************************/
void CMahjongDlg::SetSezi(int nNum)
{
	if(nNum >= 0&&nNum < 12)
	{
		m_nSeNum = nNum;
	}
	else
	{
		m_nSeNum = 12;
	}
}

/*************************************************************/
/*函数功能：设置初始化的牌
/*入口参数：无
/*返回类型：void
/*************************************************************/
void CMahjongDlg::SetPai()
{
	//初始化各参数
	m_nMousePai = -1;
	m_bStart = false;
	m_bRun = true;
	ChangBoolFalse();
	m_nOutPai = -1;

	m_bWait = true;

	m_nSelectTime = TimeSelect;
	m_nWaitTime = TimeSelect;

	m_bwjState = false;
	m_nAbandon = false;
	m_bSelect = false;

	//发牌给各玩家 
	int nWanjia[4][13] = {0};
	int i,j = 0;
	m_zmMaj.SetStart(m_nZhuang,m_nSeNum*2);
	for(i = 0;i < 13;i++)
	{
		nWanjia[0][i] = m_zmMaj.GetPai();
		nWanjia[1][i] = m_zmMaj.GetPai();
		nWanjia[2][i] = m_zmMaj.GetPai();
		nWanjia[3][i] = m_zmMaj.GetPai();
	}
	//初始化利益分数
	for(i=0;i<4;i++)
	{
		m_nGoodScore[i]=0;
	}
	int nTemp=m_nZhuang;
	for(i=0;i<4;i++)
	{
		m_wjMaj[nTemp].SetPaiInHand(nWanjia[i],13);
		nTemp++;
		if(nTemp>3)
			nTemp=0;
	}
	m_nActive=m_nZhuang;
	//取得电脑最佳分值
	for(i=1;i<4;i++)
	{	
		int a[14];
		for(int j = 0;j < 13;j++)
		{
			a[j] = m_wjMaj[i].GetPaiInHand()[j];
		}
		CCmpThink ct;
		ct.SetMjPaiArray(a,13);
		ct.Think();
		m_nCmpGoodScore[i]=ct.GetMaxScore();
	}
	m_memDC.BitBlt(0,0,800,600,&m_bgDC,0,0,SRCCOPY);
	DrawActive(m_nZhuang);
	SortWjPai();
	DrawAllpai();
	UpdateFrame();
}

/*************************************************************/
/*函数功能：画中间的箭头和庄家字样
/*入口参数：整型（当前是谁活动）
/*返回类型：void
/*************************************************************/
void CMahjongDlg::DrawActive(int nWho)
{
	m_memDC.BitBlt(360,231,103,95,&m_bgDC,360,231,SRCCOPY);
	COLORREF rf=m_numDC.GetPixel(1,1);
	TransparentBlt(m_memDC.GetSafeHdc(),385,255,60,48,m_numDC.GetSafeHdc(),0,55,60,48,rf);
	switch(m_nZhuang)				//根据庄家画
	{
	case 0:
		TransparentBlt(m_memDC.GetSafeHdc(),400,325,25,25,m_numDC.GetSafeHdc(),173,0,25,25,rf);
		break;
	case 1:	
		TransparentBlt(m_memDC.GetSafeHdc(),462,265,25,25,m_numDC.GetSafeHdc(),198,0,25,25,rf);
		break;
	case 2:	
		TransparentBlt(m_memDC.GetSafeHdc(),400,206,25,25,m_numDC.GetSafeHdc(),173,0,25,25,rf);
		break;
	case 3:
		TransparentBlt(m_memDC.GetSafeHdc(),334,265,25,25,m_numDC.GetSafeHdc(),173,0,25,25,rf);
		break;
	}
	switch(nWho)					//根据谁活动画箭头
	{
	case 0:	
		TransparentBlt(m_memDC.GetSafeHdc(),402,300,22,25,m_numDC.GetSafeHdc(),0,30,22,25,rf);
		break;
	case 1:
		TransparentBlt(m_memDC.GetSafeHdc(),438,265,25,26,m_numDC.GetSafeHdc(),22,30,25,26,rf);
		break;
	case 2:
		TransparentBlt(m_memDC.GetSafeHdc(),402,231,22,25,m_numDC.GetSafeHdc(),50,30,22,25,rf);
		break;
	case 3:
		TransparentBlt(m_memDC.GetSafeHdc(),360,265,25,26,m_numDC.GetSafeHdc(),72,30,25,26,rf);
		break;
	}
}

/*************************************************************/
/*函数功能：画所有的牌
/*入口参数：无
/*返回类型：void
/*************************************************************/
void CMahjongDlg::DrawAllpai()
{
	DrawPai();
	DrawPaiQiang();
	DrawPaiOut();
	DrawPaiUp(m_nMousePai);
	DrawTitle();
}

/*************************************************************/
/*函数功能：画标题
/*入口参数：无
/*返回类型：void
/*************************************************************/
void CMahjongDlg::DrawTitle()
{
	//画标题
	m_memDC.BitBlt(692,20,18*3,18,&m_bgDC,692,20,SRCCOPY);
	if(m_nMousePai == TITLE_MIN)
	{
		m_memDC.BitBlt(710,20,18,18,&m_titleDC,54+18,80,SRCCOPY);
	}
	else if(m_nMousePai == TITLE_CLOSE)
	{
		m_memDC.BitBlt(728,20,18,18,&m_titleDC,18,80,SRCCOPY);
	}
	else if(m_nMousePai == TITLE_SET)
	{
		m_memDC.BitBlt(692,20,18,18,&m_titleDC,108+18,80,SRCCOPY);
	}
	//运行状态下的吃碰杠等标题
	if(m_bRun)
	{
		m_memDC.BitBlt(580,440,162,34,&m_titleDC,0,0,SRCCOPY);
		if(m_bGang)
		{
			if(m_nMousePai == TITLE_GANG)
				m_memDC.BitBlt(646,444,26,26,&m_titleDC,108,54,SRCCOPY);
			m_memDC.BitBlt(648,446,22,22,&m_titleDC,43,34,SRCCOPY);
		}
		if(m_bHu)
		{
			if(m_nMousePai == TITLE_HU)
				m_memDC.BitBlt(676,444,26,26,&m_titleDC,108,54,SRCCOPY);
			m_memDC.BitBlt(678,446,22,22,&m_titleDC,64,34,SRCCOPY);
		}
		//选择状态下的
		if(m_bwjState)
		{
			if(m_bChi)
			{
				if(m_nMousePai == TITLE_EAT)
					m_memDC.BitBlt(590,444,26,26,&m_titleDC,108,54,SRCCOPY);
				m_memDC.BitBlt(592,446,22,22,&m_titleDC,0,34,SRCCOPY);
			}
			if(m_bPeng)
			{
				if(m_nMousePai == TITLE_PENG)
					m_memDC.BitBlt(617,444,26,26,&m_titleDC,108,54,SRCCOPY);
				m_memDC.BitBlt(619,446,22,22,&m_titleDC,21,35,SRCCOPY);
			}

			if(m_nMousePai == TITLE_GUO)
			{		
				m_memDC.BitBlt(710,444,26,26,&m_titleDC,108,54,SRCCOPY);
			}
			m_memDC.BitBlt(712,446,22,22,&m_titleDC,86,34,SRCCOPY);
		}
	}
	//系统开始时的开始按钮
	if(m_bStart)
	{
		m_memDC.BitBlt(370,400,55,28,&m_bgDC,370,400,SRCCOPY);
		if(m_nTitle == 3)
		{
			if(m_bHandDown)
			{
				m_memDC.BitBlt(370,400,55,28,&m_titleDC,112,127,SRCCOPY);
			}
			else
			{
				m_memDC.BitBlt(370,400,55,28,&m_titleDC,56,127,SRCCOPY);
			}
		}
		else
		{
			m_memDC.BitBlt(370,400,55,28,&m_titleDC,0,127,SRCCOPY);
		}
	}
	//系统结束时的确定按钮
	if(m_bOver)
	{
		m_memDC.BitBlt(220,130,370,311,&m_seziDC,0,99,SRCCOPY);		
		COLORREF rf = m_titleDC.GetPixel(113,101);
		ShowFenshu();
		if(m_nTitle == 4)	
		{
			if(m_bHandDown)
			{
				TransparentBlt(m_memDC.GetSafeHdc(),370,455,55,28,m_titleDC.GetSafeHdc(),112,100,55,28,rf);
			}
			else
			{
				TransparentBlt(m_memDC.GetSafeHdc(),370,455,55,28,m_titleDC.GetSafeHdc(),56,100,55,28,rf);
			}
		}
		else
		{
			TransparentBlt(m_memDC.GetSafeHdc(),370,455,55,28,m_titleDC.GetSafeHdc(),0,100,55,28,rf);
		}
	}
}

/*************************************************************/
/*函数功能：画玩家的手牌
/*入口参数：无
/*返回类型：void
/*************************************************************/
void CMahjongDlg::DrawPai()
{
	int i;	
	int* nPaiInhand[4],nStart[4];

	for( i = 0;i < 4;i++ )
	{
		nStart[i] = m_wjMaj[i].GetPaiStart();
		nPaiInhand[i] = m_wjMaj[i].GetPaiInHand();
	}
	DrawTitle();

	if(m_bOver || m_bZuobi)
	{	
		//右边玩家的牌
		m_memDC.BitBlt(680,105,31,323,&m_bgDC,680,110,SRCCOPY);
		if(m_wjMaj[1].GetState() == 1)
		{
			m_memDC.BitBlt(680,119,31,28,&m_paiDC,m_wjMaj[1].GetNewPai()*33+1,167,SRCCOPY);
		}
		for(i = 12;i >= 0;i--)
		{
			m_memDC.BitBlt(680,400-i*21,31,28,&m_paiDC,nPaiInhand[1][i]*33+1,167,SRCCOPY);
		}
		//最上面玩家的
		m_memDC.BitBlt(210,50,343,36,&m_bgDC,216,50,SRCCOPY);
		if(m_wjMaj[2].GetState() == 1)									//画新抓的牌
			m_memDC.BitBlt(222,50,23,35,&m_paiDC,m_wjMaj[2].GetNewPai()*24+1,197,SRCCOPY);
		for(i = 12;i >= 0;i--)
		{
			m_memDC.BitBlt(528-23*i,50,23,35,&m_paiDC,nPaiInhand[2][i]*24+1,197,SRCCOPY);
		}
		//左边玩家的牌
		m_memDC.BitBlt(110,100,31,345,&m_bgDC,110,130,SRCCOPY);

		if(m_wjMaj[3].GetState() == 1)									//画新抓的牌
		{
			m_memDC.BitBlt(110,410,31,28,&m_paiDC,m_wjMaj[3].GetNewPai()*33+1,138,SRCCOPY);
		}
		for(i = 0;i < 13;i++)
		{
			m_memDC.BitBlt(110,130+i*21,31,28,&m_paiDC,nPaiInhand[3][i]*33+1,138,SRCCOPY);
		}
	}
	else
	{
		//右边玩家的牌
		m_memDC.BitBlt(680,105,31,323,&m_bgDC,680,110,SRCCOPY);
		if(m_wjMaj[1].GetState() == 1)
			m_memDC.BitBlt(680,105,15,39,&m_paiDC,35*45+19,1,SRCCOPY);
		if(nStart[1]>0)
		{
			for(i = 12;i >= nStart[1];i--)
			{
				m_memDC.BitBlt(680,383-i*21,15,39,&m_paiDC,35*45+1+18,1,SRCCOPY);
			}
			for(i = nStart[1]-1;i >= 0;i--)
			{
				m_memDC.BitBlt(680,400-i*21,31,28,&m_paiDC,nPaiInhand[1][i]*33+1,167,SRCCOPY);
			}
		}
		else
		{
			for(i = m_wjMaj[1].GetPaiStart();i < 13;i++)
			{
				m_memDC.BitBlt(680,130+i*21,15,39,&m_paiDC,35*45+1+18,1,SRCCOPY);
			}
		}
		//最上面玩家的
		m_memDC.BitBlt(210,50,343,36,&m_bgDC,216,50,SRCCOPY);
		if(m_wjMaj[2].GetState() == 1)									//画新抓的牌
		{
			m_memDC.BitBlt(213,50,24,36,&m_paiDC,34*24+1,196,SRCCOPY);
		}
		if(nStart[2] > 0)
		{
			for(i = 12;i >= nStart[2];i--)
			{
				m_memDC.BitBlt(528-24*i,50,24,36,&m_paiDC,34*24+1,196,SRCCOPY);	
			}
			for(i = nStart[2]-1;i >= 0;i--)
			{
				m_memDC.BitBlt(528-23*i,50,23,35,&m_paiDC,nPaiInhand[2][i]*24,197,SRCCOPY);
			}
		}
		else
		{
			for(i = 0;i < 13;i++)
			{
				m_memDC.BitBlt(240+24*i,50,24,36,&m_paiDC,34*24+1,196,SRCCOPY);
			}
		}
		//左边玩家的牌
		m_memDC.BitBlt(110,100,31,345,&m_bgDC,110,130,SRCCOPY);
		for(i = 0;i < nStart[3];i++)
		{
			m_memDC.BitBlt(110,130+i*21,31,28,&m_paiDC,nPaiInhand[3][i]*33+1,138,SRCCOPY);
		}
		for(i = nStart[3];i < 13;i++)
		{
			m_memDC.BitBlt(110,130+i*21,15,39,&m_paiDC,35*45+2,1,SRCCOPY);
		}
		if(m_wjMaj[3].GetState() == 1)
		{
			m_memDC.BitBlt(110,133+i*21,15,39,&m_paiDC,35*45+2,1,SRCCOPY);
		}
	}
	//右边玩家的杠牌
	for(i=0;i<4;i++)
	{
		if(m_wjMaj[1].GetGangPai()[i] != 0)
		{
			if(m_wjMaj[1].GetGangPai()[i] == 1)
			{
				m_memDC.BitBlt(680,396-(3*i+1)*21,31,28,&m_paiDC,nPaiInhand[1][3*i]*33,167,SRCCOPY);
			}
			else
			{
				m_memDC.BitBlt(680,396-(3*i+1)*21,31,28,&m_paiDC,34*33,167,SRCCOPY);
			}
		}
	}
	//上面玩家的杠牌
	for(i=0;i<4;i++)
	{
		if(m_wjMaj[2].GetGangPai()[i] != 0)
		{
			if(m_wjMaj[2].GetGangPai()[i] == 1)
			{
				m_memDC.BitBlt(528-23*(3*i+1),45,23,35,&m_paiDC,nPaiInhand[2][3*i]*24,197,SRCCOPY);
			}
			else
			{
				m_memDC.BitBlt(528-23*(3*i+1),45,23,35,&m_paiDC,35*24+1,197,SRCCOPY);
			}
		}
	}
	//左面玩家的杠牌
	for(i=0;i<4;i++)
	{
		if(m_wjMaj[3].GetGangPai()[i] != 0)
		{
			if(m_wjMaj[3].GetGangPai()[i] == 1)
			{
				m_memDC.BitBlt(110,125+(3*i+1)*21,31,28,&m_paiDC,nPaiInhand[3][3*i]*33+1,138,SRCCOPY);
			}
			else
			{
				m_memDC.BitBlt(110,125+(3*i+1)*21,31,28,&m_paiDC,34*33,138,SRCCOPY);
			}
		}
	}
}

/*************************************************************/
/*函数功能：画牌墙
/*入口参数：无
/*返回类型：void
/*************************************************************/
void CMahjongDlg::DrawPaiQiang()
{
	int i,j;
	int nPaiqiang[4][34];
	int* nP[4];
	for( i = 0;i < 4;i++ )
		nP[i]=nPaiqiang[i];
	m_zmMaj.GetPaiqiang(nP);

	//本人面前的牌墙
	m_memDC.BitBlt(251,430,320,33,&m_bgDC,251,430,SRCCOPY);
	for( i = 0,j = 0;j < 34;j = j+2,i++ )
	{	
		if( nPaiqiang[0][j+1] != -1 )
		{
			m_memDC.BitBlt(540-17*i,435,17,28,&m_paiDC,35*24+2,234,SRCCOPY);
		}
		if( nPaiqiang[0][j] != -1 )
		{
			m_memDC.BitBlt(540-17*i,430,17,28,&m_paiDC,35*24+2,234,SRCCOPY);
		}
	}
	//上面玩家的牌墙
	m_memDC.BitBlt(270,105,340,40,&m_bgDC,270,105,SRCCOPY);
	for( i = 0,j = 0;j < 34;j = j+2,i++ )
	{	
		if(nPaiqiang[2][j+1] != -1)
		{
			m_memDC.BitBlt(270+17*i,110,17,28,&m_paiDC,35*24+2,234,SRCCOPY);
		}
		if( nPaiqiang[2][j] != -1 )
		{
			m_memDC.BitBlt(270+17*i,105,17,28,&m_paiDC,35*24+2,234,SRCCOPY);
		}
	}
	//左边玩家的牌墙
	m_memDC.BitBlt(170,155,24,250,&m_bgDC,170,155,SRCCOPY);
	for( i = 0,j = 33;j >= 0;j = j-2,i++ )
	{
		if(nPaiqiang[3][j] != -1)
		{
			m_memDC.BitBlt(170,160+14*i,24,19,&m_paiDC,34*24+1,233,SRCCOPY);
		}
		if(nPaiqiang[3][j-1] != -1)
		{
			m_memDC.BitBlt(170,155+14*i,24,19,&m_paiDC,34*24+1,233,SRCCOPY);
		}
	}
	//右边玩家的牌墙
	m_memDC.BitBlt(620,155,24,280,&m_bgDC,620,155,SRCCOPY);
	for(i = 0,j = 0;j < 34;j = j+2,i++)
	{
		if(nPaiqiang[1][j+1] != -1)
		{
			m_memDC.BitBlt(620,160+14*i,24,19,&m_paiDC,34*24+1,233,SRCCOPY);
		}
		if(nPaiqiang[1][j] != -1)
		{
			m_memDC.BitBlt(620,155+14*i,24,19,&m_paiDC,34*24+1,233,SRCCOPY);
		}
	}
}

/*************************************************************/
/*函数功能：画各玩家出的牌
/*入口参数：无
/*返回类型：void
/*************************************************************/
void CMahjongDlg::DrawPaiOut()
{
	int i;
	int* nOutPai[4];
	for(i = 0;i < 4;i++)
	{
		nOutPai[i] = m_wjMaj[i].GetOutPai();
	}

	m_memDC.BitBlt(257,390,230,35,&m_bgDC,257,390,SRCCOPY);
	m_memDC.BitBlt(280,363,184,35,&m_bgDC,280,363,SRCCOPY);
	m_memDC.BitBlt(303,336,184,35,&m_bgDC,303,336,SRCCOPY);

	//本人出的牌
	if(m_wjMaj[0].GetOutNum() > 10)
	{
		if(m_wjMaj[0].GetOutNum() > 18)
		{	
			for( i = 18;i < m_wjMaj[0].GetOutNum();i++ )
			{
				m_memDC.BitBlt(303+23*(i-18),336,23,35,&m_paiDC,(nOutPai[0][i])*24+1,234,SRCCOPY);
			}
			for(i = 10;i < 18;i++)
			{
				m_memDC.BitBlt(280+23*(i-10),363,23,35,&m_paiDC,(nOutPai[0][i])*24+1,234,SRCCOPY);
			}
			for(i = 0;i < 10;i++)
			{
				m_memDC.BitBlt(257+23*i,390,23,35,&m_paiDC,(nOutPai[0][i])*24+1,234,SRCCOPY);
			}
		}
		else
		{
			for(i = 10;i < m_wjMaj[0].GetOutNum();i++)
			{
				m_memDC.BitBlt(280+23*(i-10),363,23,35,&m_paiDC,(nOutPai[0][i])*24+1,234,SRCCOPY);
			}
			for(i = 0;i < 10;i++)
			{
				m_memDC.BitBlt(257+23*i,390,23,35,&m_paiDC,(nOutPai[0][i])*24+1,234,SRCCOPY);
			}
		}
	}
	else
	{
		for(i = 0;i < m_wjMaj[0].GetOutNum();i++)
		{
			m_memDC.BitBlt(257+23*i,390,23,35,&m_paiDC,(nOutPai[0][i])*24+1,234,SRCCOPY);
		}
	}
		
	//右边玩家出的牌
	int nI = m_wjMaj[1].GetOutNum(),j;
	if(nI<11)
	{
		m_memDC.BitBlt(565,210,31,200,&m_bgDC,565,200,SRCCOPY);
		for(i = nI-1,j = 0;i >= 0;i--,j++)
		{
			m_memDC.BitBlt(565,400-nI*19+19*j,31,27,&m_paiDC,(nOutPai[1][i])*33+1,168,SRCCOPY);
		}
	}
	else
	{
		if(nI < 19)
		{
			m_memDC.BitBlt(534,210,31,200,&m_bgDC,534,210,SRCCOPY);
			for(i = nI-11,j = 0;i >= 0;i--,j++)
			{
				m_memDC.BitBlt(534,363-(nI-11)*19+19*j,31,27,&m_paiDC,(nOutPai[1][i+10])*33+1,168,SRCCOPY);
			}
		}
		else
		{
			m_memDC.BitBlt(503,220,31,150,&m_bgDC,503,220,SRCCOPY);
			for(i = nI-18,j = 0;i >= 0;i--,j++)
			{
				m_memDC.BitBlt(503,333-(nI-19)*19+19*j,31,27,&m_paiDC,(nOutPai[1][i+18])*33+1,168,SRCCOPY);
			}
		}
	}
		
	//左边玩家出的牌
	nI = m_wjMaj[3].GetOutNum();
	m_memDC.BitBlt(215,170,31,20*10+7,&m_bgDC,215,170,SRCCOPY);
	m_memDC.BitBlt(246,190,31,20*8+7,&m_bgDC,246,190,SRCCOPY);
	m_memDC.BitBlt(277,210,31,20*6+7,&m_bgDC,277,210,SRCCOPY);
	for(i=0;i<nI;i++)
	{
		if(i < 10)
		{
			m_memDC.BitBlt(215,170+20*i,31,27,&m_paiDC,(nOutPai[3][i])*33+1,138,SRCCOPY);
		}
		else if(i < 18)
		{
			m_memDC.BitBlt(246,190+20*(i-10),31,27,&m_paiDC,(nOutPai[3][i])*33+1,138,SRCCOPY);
		}
		else
		{
			m_memDC.BitBlt(277,210+20*(i-18),31,27,&m_paiDC,(nOutPai[3][i])*33+1,138,SRCCOPY);
		}
	}
	
	//上面玩家出的牌
	nI=m_wjMaj[2].GetOutNum();
	m_memDC.BitBlt(320,150,260,35,&m_bgDC,320,150,SRCCOPY);
	m_memDC.BitBlt(340,175,184,35,&m_bgDC,340,175,SRCCOPY);
	m_memDC.BitBlt(360,200,138,35,&m_bgDC,360,200,SRCCOPY);
	for(i = 0;i < nI;i++)
	{
		if(i < 10)
		{
			m_memDC.BitBlt(550-23*i,150,23,35,&m_paiDC,(nOutPai[2][i])*24+1,197,SRCCOPY);
		}
		else if(i < 18)
		{
			m_memDC.BitBlt(550-23*(i-9),175,23,35,&m_paiDC,(nOutPai[2][i])*24+1,197,SRCCOPY);
		}
		else
		{
			m_memDC.BitBlt(550-23*(i-17),200,23,35,&m_paiDC,(nOutPai[2][i])*24+1,197,SRCCOPY);
		}
	}
		
}

/*************************************************************/
/*函数功能：画选择状态下的小红箭头
/*入口参数：无
/*返回类型：void
/*************************************************************/
void CMahjongDlg::DrawSelect()
{
	int i;
	if(!m_bSelect)
	{
		m_memDC.BitBlt(100,485,602,11,&m_bgDC,100,485,SRCCOPY);
		return;
	}
	//画选择状态下牌上的箭头
	if(m_wjMaj[0].GetState() == 1)
	{
		int pos;
		for(i = 0;i < m_arrGang.GetSize();i++)
		{
			if(m_wjMaj[0].GetNewPai() == (int)m_arrGang.GetAt(i))
			{
				TransparentBlt(m_memDC.GetSafeHdc(),670+12,485, 15,11,m_titleDC.GetSafeHdc(),108,34,15,11,RGB(255,255,255));
			}
			else
			{
				pos =  m_wjMaj[0].FindPaiPos(m_arrGang.GetAt(i),m_wjMaj[0].GetPaiStart());
				TransparentBlt(m_memDC.GetSafeHdc(),100+pos*43+15,485, 15,11,m_titleDC.GetSafeHdc(),108,34,15,11,RGB(255,255,255));
			}
		}
	}
	else
	{
		if(m_arrEat.GetSize() > 2)
		{
			for(i = 0;i < m_arrEat.GetSize();i++)
			{
				TransparentBlt(m_memDC.GetSafeHdc(),115+m_arrEat.GetAt(i)*43,475, 15,11,m_titleDC.GetSafeHdc(),108,34,15,11,RGB(255,255,255));
			}
		}
	}
}

/*************************************************************/
/*函数功能：画本人的牌和牌起来的状态
/*入口参数：鼠标点中的牌
/*返回类型：void
/*************************************************************/
void CMahjongDlg::DrawPaiUp(int nPai)
{	int i;
	m_memDC.BitBlt(100,475,620,78,&m_bgDC,100,475,SRCCOPY);		//本人的牌
	//是否结束
	if(m_bOver)
	{
		for(i = 0;i < 13;i++)
		{
			m_memDC.BitBlt(100+43*i,487,43,66,&m_paiDC,m_wjMaj[0].GetPaiInHand()[i]*45+1,71,SRCCOPY);
		}
		if(m_wjMaj[0].GetState() == 1)
		{
			m_memDC.BitBlt(670,487,43,66,&m_paiDC,m_wjMaj[0].GetNewPai()*45+1,71,SRCCOPY);
		}
	}
	else
	{
		for(i = 0;i < m_wjMaj[0].GetPaiStart();i++)
		{
			m_memDC.BitBlt(100+43*i,487,43,66,&m_paiDC,m_wjMaj[0].GetPaiInHand()[i]*45+1,71,SRCCOPY);
		}
		for(i = m_wjMaj[0].GetPaiStart();i < 13;i++)
		{
			m_memDC.BitBlt(100+43*i,485,43,68,&m_paiDC,m_wjMaj[0].GetPaiInHand()[i]*45+1,1,SRCCOPY);
		}
		if(m_wjMaj[0].GetState() == 1)
		{
			m_memDC.BitBlt(670,485,43,68,&m_paiDC,m_wjMaj[0].GetNewPai()*45+1,1,SRCCOPY);
		}
	}
	//杠牌
	for(i = 0;i < 4;i++)
	{
		if(m_wjMaj[0].GetGangPai()[i] != 0)
		{
			if(m_wjMaj[0].GetGangPai()[i] == 1)
			{
				m_memDC.BitBlt(100+43*(3*i+1),477,43,66,&m_paiDC,m_wjMaj[0].GetPaiInHand()[3*i]*45+1,71,SRCCOPY);
			}
			else
			{
				m_memDC.BitBlt(100+43*(3*i+1),477,43,66,&m_paiDC,34*45+1,72,SRCCOPY);
			}
		}
	}
	//选择状态下的牌只有个别或成组起来
	if(m_bSelect)
	{
		if(m_wjMaj[0].GetState() == 1)
		{
			int pos;
			for(i=0;i<m_arrGang.GetSize();i++)
			{
				if(m_wjMaj[0].GetNewPai() == (int)m_arrGang.GetAt(i))
				{
					pos = 14;
				}
				else
				{
					pos = m_wjMaj[0].FindPaiPos(m_arrGang.GetAt(i),m_wjMaj[0].GetPaiStart());
				}
				if(nPai == pos)
				{
					DrawUpPai(pos);
					m_nSelectGang = (int)m_arrGang.GetAt(i);
					break;
				}
				else 
				{
					m_nSelectGang = -1;
				}
			}	
		}
		else
		{
			for(i = 0;i < (int)m_arrEat.GetSize()/2;i++)
			{
				if(nPai == (int)m_arrEat.GetAt(2*i) || nPai == (int)m_arrEat.GetAt(2*i+1))
				{
					DrawUpPai((int)m_arrEat.GetAt(2*i));
					DrawUpPai((int)m_arrEat.GetAt(2*i+1));			
					m_nSelectEat = m_wjMaj[0].GetPaiInHand()[(int)m_arrEat.GetAt(2*i)];
					break;
				}
				else
				{
					m_nSelectEat = -1;
				}

			}
		}
		DrawSelect();
		return;
	}
	DrawUpPai(nPai);
}

/*************************************************************/
/*函数功能：单独画一张牌起来
/*入口参数：整型（要起来的牌的位置）
/*返回类型：void
/*************************************************************/
void CMahjongDlg::DrawUpPai(int nPlace)
{
	if( nPlace == 14 )
	{
		m_memDC.BitBlt(670,485, 43,68,&m_bgDC,670,485,SRCCOPY);
		if( m_wjMaj[0].GetPaiInHand()[13] != -1 )
		{
			m_memDC.BitBlt(670,475, 43,68,&m_paiDC,(m_wjMaj[0].GetNewPai())*45+1,1,SRCCOPY);
		}
	}
	else if(nPlace >= 0 && nPlace < 14)
	{
		m_memDC.BitBlt(100+nPlace*43,485, 43,68,&m_bgDC,100+nPlace*43,485,SRCCOPY);
		m_memDC.BitBlt(100+nPlace*43,475, 43,68,&m_paiDC,(m_wjMaj[0].GetPaiInHand()[nPlace])*45+1,1,SRCCOPY);
	}
}


/*************************************************************/
/*函数功能：整理各玩家的牌
/*入口参数：无
/*返回类型：void
/*************************************************************/
void CMahjongDlg::SortWjPai()
{
	for(int i = 0;i < 4;i++)
	{
		m_wjMaj[i].SortPai();
	}
}

/*************************************************************/
/*函数功能：检测是否流局
/*入口参数：无
/*返回类型：布尔
/*************************************************************/
bool CMahjongDlg::CheckLiuju()
{
	//如果桌面上的牌剩下不到10张就流局
	if(m_zmMaj.GetMajNum() <= 10)
	{
		PlayOtherSound(S_LIU);
		m_nHuWho = 0;
		m_nHuType = 4;
		m_bOver = true;
		m_bRun = false;
		DrawAllpai();
		ShowFenshu();
		UpdateFrame();
		KillTimer(1);
		return true;
	}
	else
	{
		return false;
	}
}


void CMahjongDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default	

	switch(nIDEvent)
	{
	case 1:
		{
			if(m_bRun&&!m_bwjState&&!m_bSelect)
			{
				//如果有出牌的话看这张牌该谁要
				if(m_nOutPai!=-1)
				{
					//获得各玩家的分数
					static bool blTemp=false;
					if(!blTemp && m_nChupaiUser == 0)
					{
						blTemp = true;
						return;
					}
					blTemp = false;
					CheckWhoGet();
					//如果用户放弃就把用户的分数归零
					if(m_nAbandon)
					{
						m_nAbandon = false;
						m_nGoodScore[0] = m_nChupaiUser;
					}
					//看谁得分最大
					int nWho,nMax;
					nWho = GetMaxWho(nMax);
					m_nActive = nWho;
					//如果用户有机会吃碰等就让用户选择是否
					if(0 == nWho&&nMax >= 100)
					{
						m_bwjState = true;
						DrawTitle();
						UpdateFrame();
						SetTimer(3,1000,NULL);
						OnTimer(3);
						return;
					}
				}
				switch(m_nActive)
				{
				case 0:
					{
						//如果用户状态是出牌不是就抓一张牌
						if(m_wjMaj[0].GetState() != 1)
						{
							if(CheckLiuju()) return;
							sndPlaySound("Sounds\\draw.wav",SND_ASYNC|SND_NODEFAULT);
							m_wjMaj[0].SetNewPai(m_zmMaj.GetPai());
							CheckPerson();
							DrawTitle();
							m_nActive = 0;
							m_nOutPai = -1;
							DrawAllpai();
							UpdateFrame();
						}
						else
						{
							//否则就按等待时间递减
							m_nWaitTime--;
							if(m_nWaitTime < TimeSelect/2)
							{
								sndPlaySound("Sounds\\hurry.wav",SND_ASYNC|SND_NODEFAULT);
							}
							//如果等待时间用完就自动出一张牌
							if(m_nWaitTime < 0)
							{		
								m_nOutPai = m_wjMaj[0].GetNewPai();
								m_wjMaj[0].OutPai(14);
								m_nChupaiUser = 0;
								m_nOutPai = m_wjMaj[0].GetOutPai()[m_wjMaj[0].GetOutNum()-1];	
								m_nActive = 1;
								PlayPaiSound(m_nOutPai);
								DrawAllpai();
								UpdateFrame();
								m_nWaitTime = TimeSelect;
								return;
							}
							UpdateFrame();
						}
						break;
					}
				case 1:
					{
						CmpActive(1);
					}
					break;
				case 2:
					{
						CmpActive(2);
					}
					break;
				case 3:
					{
						CmpActive(3);
					}
					break;
				}
			}
		}
		break;
	case 2:
		{
			DrawSezi();
		}
		break;
	case 3:
		{
			//选择时的数字秒表
			if(m_bwjState)
			{
				m_nSelectTime = m_nSelectTime-1;
				//时间到一半提示
				if(m_nSelectTime < TimeSelect/2)
				{
					sndPlaySound("Sounds\\hurry.wav",SND_ASYNC|SND_NODEFAULT);
				}
				//时间用完自动放弃
				if(m_nSelectTime < 0)
				{	
					m_bwjState = false;
					m_nAbandon = true;
					m_nSelectTime = TimeSelect;
					OnTimer(1);
					KillTimer(3);
				}
				UpdateFrame();
			}
		}
		break;
	}
	CDialog::OnTimer(nIDEvent);
}

/*************************************************************/
/*函数功能：画色子滚动的动画
/*入口参数：无
/*返回类型：void
/*************************************************************/
void  CMahjongDlg::DrawSezi()
{
	if(m_bCaton)
	{
		static bool bRuff = false;
		//洗牌一次
		if(!bRuff)
		{
			sndPlaySound("Sounds\\deal.wav",SND_ASYNC|SND_NODEFAULT);
			m_zmMaj.Ruffle();
			m_zmMaj.SortMaj();
			bRuff = true;	
			m_memDC.BitBlt(0,0,800,600,&m_bgDC,0,0,SRCCOPY);		
			DrawPaiQiang();
			UpdateFrame();
		}
		static CPoint pt(0,0);
		COLORREF rf = m_seziDC.GetPixel(1,1);
		m_memDC.BitBlt(0,0,800,600,&m_bgDC,0,0,SRCCOPY);		
		DrawPaiQiang();
		//两个色子的转动
		TransparentBlt(m_memDC.GetSafeHdc(),380,260,32,33,m_seziDC.GetSafeHdc(),pt.x+32,pt.y,32,33,rf);
		TransparentBlt(m_memDC.GetSafeHdc(),410,250,32,33,m_seziDC.GetSafeHdc(),pt.x,pt.y,32,33,rf);
		UpdateFrame();
		pt.x += 32;
		if(pt.x >= 256)
		{
			pt.y += 33;
			if(pt.y >= 66)
			{
				pt.x = 0;
				pt.y = 0;
			}
		}
		static int nTemp = 0;
		nTemp += 1;
		//转动完就开始
		if(nTemp >= TimeSezi)
		{
			nTemp = 0;
			srand((unsigned)time(NULL));
			int nFirst,nSecond;
			//随即生成两个色子数
			nFirst = rand()%6;
			nSecond = rand()%6;
			SetSezi(nFirst+nSecond+2);
			m_memDC.BitBlt(0,0,800,600,&m_bgDC,0,0,SRCCOPY);		
			DrawPaiQiang();
			TransparentBlt(m_memDC.GetSafeHdc(),380,260,32,33,m_seziDC.GetSafeHdc(),nFirst*32,66,32,33,rf);
			TransparentBlt(m_memDC.GetSafeHdc(),410,250,32,33,m_seziDC.GetSafeHdc(),nSecond*32,66,32,33,rf);
			UpdateFrame();
			Sleep(1000);
			m_bRun = true;
			bRuff = false;
			SetPai();
			m_bCaton = false;
		}
	}
}

/*************************************************************/
/*函数功能：统计分数
/*入口参数：无
/*返回类型：void
/*************************************************************/
void  CMahjongDlg::CountFenshu()
{	
	int nGet,nRemove;
	switch(m_nHuType)
	{
	case 1:
		nGet = 150;
		nRemove = -50;
		break;
	case 2:
		nGet = 300;
		nRemove = -100;
		break;
	case 3:
		nGet = 450;
		nRemove = -150;
		break;
	}
	//根据胡牌统计分数
	for(int i = 0;i<4;i++)
	{
		if(i == m_nHuWho)
		{
			m_wjMaj[i].SetFenshu(nGet);
		}
		else
		{
			m_wjMaj[i].SetFenshu(nRemove);
		}
	}
}

/*************************************************************/
/*函数功能：活动电脑的行为
/*入口参数：整型（该那家电脑）
/*返回类型：void
/*************************************************************/
void  CMahjongDlg::CmpActive(int nWho)
{
	if(m_wjMaj[nWho].GetState() == 1)
	{
		int ret;
		ret = CmpThink(nWho);
		if(ret == 1000)				//既自摸了
		{
			m_nHuWho=nWho;
			m_nHuType = 2;
			m_nMousePai = -1;
			CountFenshu();
			m_bOver = true;
			m_bRun = false;
			m_nZhuang=(m_nZhuang+1)%4;
			KillTimer(1);
			m_nOutPai = -1;
			PlayOtherSound(S_HU,m_wjMaj[nWho].GetSex());
			Sleep(200);
			sndPlaySound("Sounds\\countfan.wav",SND_ASYNC|SND_NODEFAULT);
			DrawPai();
			DrawPaiOut();
			DrawPaiUp(m_nMousePai);
			DrawTitle();
		}
		else if(ret < 0)			//表明可以杠
		{
			PlayOtherSound(S_GAN,m_wjMaj[nWho].GetSex());
			m_wjMaj[nWho].SetGang(ret+100);
			m_wjMaj[nWho].SetNewPai(m_zmMaj.GetGangPai());
			m_nActive=nWho;
			DrawPai();
			DrawPaiQiang();
		}
		else if(ret >= 0 && ret <= 34)
		{
			m_wjMaj[nWho].CmpOutPai(ret);//电脑出牌	
			m_nChupaiUser = nWho;	
			m_nOutPai = ret;
			sndPlaySound("Sounds\\give.wav",SND_ASYNC|SND_NODEFAULT);
			//Sleep(100);
			PlayPaiSound(m_nOutPai,m_wjMaj[nWho].GetSex());
			DrawPai();
			DrawPaiOut();
		}
	}
	else
	{
		if(m_nGoodScore[nWho] >= 1000)	//别人放炮了
		{
			m_nHuWho = nWho;
			m_nHuType = 1;
			m_nMousePai = -1;
			CountFenshu();
			m_wjMaj[nWho].SetNewPai(m_nOutPai);
			m_wjMaj[m_nChupaiUser].RemoveOutPai();
			m_bOver = true;
			m_bRun = false;
			m_nZhuang = (m_nZhuang+1)%4;
			KillTimer(1);
			PlayOtherSound(S_HU,m_wjMaj[nWho].GetSex());
			Sleep(200);
			sndPlaySound("Sounds\\countfan.wav",SND_ASYNC|SND_NODEFAULT);
			DrawAllpai();
			DrawTitle();
		}
		else if(m_nGoodScore[nWho] >= 800)	//杠
		{
			PlayOtherSound(S_GAN,m_wjMaj[nWho].GetSex());
			m_wjMaj[nWho].SetGang(m_nOutPai);
			m_wjMaj[nWho].SetNewPai(m_zmMaj.GetGangPai());
			m_wjMaj[m_nChupaiUser].RemoveOutPai();
			m_nOutPai = -1;
			m_nActive = nWho;
			DrawAllpai();
		}
		else if(m_nGoodScore[nWho] >= 500)	//碰
		{	
			PlayOtherSound(S_PEN,m_wjMaj[nWho].GetSex());
			m_wjMaj[nWho].SetPeng();			
			m_wjMaj[m_nChupaiUser].RemoveOutPai();
			m_nOutPai = -1;
			m_nActive = nWho;
			DrawAllpai();
		}
		else if(m_nGoodScore[nWho] >= 100)	//吃
		{	
			PlayOtherSound(S_EAT,m_wjMaj[nWho].GetSex());					
			m_wjMaj[m_nChupaiUser].RemoveOutPai();
			m_nOutPai = -1;
			//看是那种吃法
			if(m_nGoodScore[nWho] >= 300)
			{
				m_wjMaj[nWho].SetChi(2);
			}
			else if(m_nGoodScore[nWho] >= 200)
			{
				m_wjMaj[nWho].SetChi(1);
			}
			else
			{
				m_wjMaj[nWho].SetChi(0);
			}
			m_nActive = nWho;
			DrawAllpai();
		}
		if(m_nGoodScore[nWho] < 100)		//都不是就抓牌
		{
			if(CheckLiuju()) return;
			sndPlaySound("Sounds\\draw.wav",SND_ASYNC|SND_NODEFAULT);
			m_wjMaj[nWho].SetNewPai(m_zmMaj.GetPai());
			m_nActive = nWho;
			CheckPerson();
			m_nOutPai = -1;
			DrawPaiQiang();
			DrawPai();
		}
	}
	UpdateFrame();	
	return;
}

/*************************************************************/
/*函数功能：获得利益最大的完家和分数
/*入口参数：整型引用（最大分数）
/*返回类型：整型（完家）
/*************************************************************/
int CMahjongDlg::GetMaxWho(int& nMaxScore)
{
	nMaxScore = 0;
	int nMaxWho;
	//获得最大分数 
	for(int i = 0;i < 4;i++)
	{
		if(i != m_nChupaiUser)
		{
			if(m_nGoodScore[i] > nMaxScore)
			{
				nMaxWho = i;
				nMaxScore = m_nGoodScore[i];
			}
		}
	}
	return nMaxWho;
}

/*************************************************************/
/*函数功能：检测本人的状态
/*入口参数：无
/*返回类型：布尔
/*************************************************************/
bool CMahjongDlg::CheckPerson()
{
	ChangBoolFalse();
	m_wjMaj[0].SetOutPai(m_nOutPai);
	m_bHu = m_wjMaj[0].ThinkHupai();
	//看用户是否可以吃
	if(m_nChupaiUser == 3)
	{
		m_bChi = m_wjMaj[0].CheckEat(m_arrEat);
	}
	//看用户是否可以杠和碰
	if(m_wjMaj[0].GetState() == 1)
	{
		m_bGang = m_wjMaj[0].CheckGangSelf(m_arrGang);
	}
	else
	{
		m_bGang = m_wjMaj[0].CheckGang(m_bPeng);
	}
	return (m_bChi||m_bHu||m_bPeng||m_bGang);
}

/*************************************************************/
/*函数功能：鼠标在标题上移动
/*入口参数：坐标
/*返回类型：boid
/*************************************************************/
void CMahjongDlg::MoveTitle(CPoint point)
{
	bool blFind=false;
	CRect rt;
	//用户鼠标是否移动到右上角的小图标上
	if(m_rtTitle[7].PtInRect(point))
	{
		if(m_nMousePai == TITLE_SET)
		{
			return;
		}
		else
		{
			m_nMousePai = TITLE_SET;
			blFind = true;
		}
	}
	if(m_rtTitle[6].PtInRect(point))
	{
		if(m_nMousePai == TITLE_CLOSE)
		{
			return;
		}
		else
		{
			m_nMousePai = TITLE_CLOSE;
			blFind = true;
		}
	}
	if(m_rtTitle[5].PtInRect(point))
	{
		if(m_nMousePai == TITLE_MIN)
		{
			return;
		}
		else
		{
			m_nMousePai = TITLE_MIN;
			blFind = true;
		}
	}
	if(m_bRun && !blFind)
	{
		//是否移动到杠吃等图标上
		if(m_bGang)
		{
			if(m_rtTitle[2].PtInRect(point))
			{
				if(m_nMousePai == TITLE_GANG)
				{
					return;
				}
				else
				{
					m_nMousePai = TITLE_GANG;
					blFind = true;
				}
			}
		}		
		if(m_bHu)
		{
			if(m_rtTitle[3].PtInRect(point))
			{
				if(m_nMousePai == TITLE_HU)
				{
					return;
				}
				else
				{
					m_nMousePai = TITLE_HU;
					blFind = true;
				}
			}
		}
		if(m_bChi)
		{
			if(m_rtTitle[0].PtInRect(point))
			{
				if(m_nMousePai == TITLE_EAT)
				{
					return;
				}
				else
				{
					m_nMousePai = TITLE_EAT;
					blFind=true;
				}
			}
		}
		if(m_bPeng)
		{
			if(m_rtTitle[1].PtInRect(point))
			{
				if(m_nMousePai == TITLE_PENG)
				{
					return;
				}
				else
				{
					m_nMousePai = TITLE_PENG;
					blFind=true;
				}
			}
		}

		if(m_rtTitle[4].PtInRect(point))
		{
			if(m_nMousePai == TITLE_GUO)
			{
				return;
			}
			else
			{
				m_nMousePai = TITLE_GUO;
				blFind=true;
			}
		}

		rt.SetRect(100,485,713,68+485);
		//是否移动到牌上去了
		if(rt.PtInRect(point) && !blFind)
		{
			for( int i = m_wjMaj[0].GetPaiStart();i < 13;i++ )
			{
				rt.SetRect(100+i*43,485,100+i*43+43,68+485 );
				if( rt.PtInRect(point) )
				{
					m_nMousePai = i;
					blFind = true;
				}
			}
			rt.SetRect(670,485,713,68+485);
			if(rt.PtInRect(point)&&m_wjMaj[0].GetState() == 1)
			{
				m_nMousePai = 14;
				blFind = true;
			}
		}
	}	
	if(!blFind)
	{
		m_nMousePai = -1;
	}
	DrawTitle();
	if(m_bRun)
	{
		DrawPaiUp(m_nMousePai);
	}
	if(m_nFirst == m_nMousePai) 
	{
		return;
	}
	UpdateFrame();	
	m_nFirst = m_nMousePai;
}


void CMahjongDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	//如果是放动画就直接退出
	if(m_bCaton)
		return;
	MoveTitle(point);

	CRect rt;	
	//开始
	if(m_bStart)
	{
		rt.SetRect(370,400,425,428);
		if(rt.PtInRect(point))
		{
			m_nTitle = 3;
			DrawTitle();
			UpdateFrame();
			return;
		}
		m_nTitle = -1;
		DrawTitle();
		UpdateFrame();
	}
	//结束
	if(m_bOver)
	{
		rt.SetRect(370,450,425,478);
		if(rt.PtInRect(point))
		{
			m_nTitle = 4;
			DrawTitle();
			Invalidate(false);
			return;
		}
		m_nTitle = -1;
		DrawTitle();
		Invalidate(false);
	}

	CDialog::OnMouseMove(nFlags, point);
}

BOOL CMahjongDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	// TODO: Add your message handler code here and/or call default
	//设置鼠标
	if(m_bStart || m_bOver)
	{
		if(m_nTitle == 3||m_nTitle == 4)
		{
			SetCursor(m_hCursorHand);
		}
		else
		{
			SetCursor(m_hCursor);
		}
	}
	else
	{
		if(m_nMousePai < 0 || m_nMousePai >= 100)
		{
			SetCursor(m_hCursor);
		}
		else
		{
			SetCursor(m_hCursorHand);
		}
	}
	return TRUE;
	return CDialog::OnSetCursor(pWnd, nHitTest, message);
}

/*************************************************************/
/*函数功能：鼠标在标题上点击
/*入口参数：坐标
/*返回类型：void
/*************************************************************/
void CMahjongDlg::OnClickTitle(CPoint point)
{
	//如果点击了标题中的一个
	if((m_nMousePai == TITLE_PENG)||(m_nMousePai == TITLE_EAT)
		||(m_nMousePai == TITLE_GANG)||(m_nMousePai == TITLE_HU)||(m_nMousePai == TITLE_GUO))
	{
		ChangBoolFalse();
		m_nSelectTime = TimeSelect;
		DrawTitle();
		UpdateFrame();
	}
	if(m_nMousePai == TITLE_GANG)
	{	
		if(m_wjMaj[0].GetState() != 1)	//别人打牌的杠
		{
			m_bwjState = false;
			m_wjMaj[m_nChupaiUser].RemoveOutPai();
			m_wjMaj[0].SetGang(m_nOutPai);
			PlayOtherSound(S_GAN,m_wjMaj[0].GetSex());
			m_nActive = 0;
			m_nOutPai = -1;
			m_wjMaj[0].SetNewPai(m_zmMaj.GetGangPai());
			CheckPerson();
			DrawAllpai();
			UpdateFrame();
			OnTimer(1);
		}
		else
		{
			if(m_arrGang.GetSize() == 1)
			{
				m_bwjState = false;
				m_wjMaj[0].SetGang(m_arrGang.GetAt(0));	
				PlayOtherSound(S_GAN,m_wjMaj[0].GetSex());
				m_nActive = 0;
				m_wjMaj[0].SetNewPai(m_zmMaj.GetGangPai());
				CheckPerson();
				DrawAllpai();
				UpdateFrame();
				OnTimer(1);
			}
			else
			{
				m_bwjState = false;
				m_nActive = 0;
				m_bSelect = true;
				DrawTitle();
				UpdateFrame();
			}
		}
	}
	//玩家胡牌
	if(m_nMousePai == TITLE_HU)
	{
		m_bwjState = false;
		PlayOtherSound(S_HU,m_wjMaj[0].GetSex());
		Sleep(200);
		sndPlaySound("Sounds\\countfan.wav",SND_ASYNC|SND_NODEFAULT);
		m_nHuWho = 0;
		if(m_nOutPai == -1)
			m_nHuType = 2;
		else
		{
			m_nHuType = 1;
			m_wjMaj[m_nChupaiUser].RemoveOutPai();
			m_wjMaj[0].SetNewPai(m_nOutPai);
		}
		CountFenshu();
		m_bOver = true;
		m_bRun = false;
		m_nOutPai = -1;
		m_nZhuang = (m_nZhuang+1)%4;
		DrawAllpai();
		ShowFenshu();
		UpdateFrame();
		KillTimer(1);
	}
	//玩家的等待选择状态下点击了相关标题
	if(m_bwjState)
	{
		if(m_nMousePai == TITLE_EAT)
		{
			m_bwjState = false;
			//看是否只有一对可以吃，是就直接吃
			if(m_arrEat.GetSize() == 2)
			{
				int nType;
				if( m_wjMaj[0].GetPaiInHand()[(int)m_arrEat.GetAt(0)] == (m_nOutPai-2) )
				{
					nType = 2;
				}
				else if( m_wjMaj[0].GetPaiInHand()[(int)m_arrEat.GetAt(0)] == (m_nOutPai-1) )
				{
					nType = 1;
				}
				else
				{
					nType = 0;
				}
				PlayOtherSound(S_EAT,m_wjMaj[0].GetSex());
				m_wjMaj[0].SetChi(nType);
				m_nActive = 0;
				m_nOutPai = -1;
				m_wjMaj[3].RemoveOutPai();
				DrawAllpai();
				UpdateFrame();
				OnTimer(1);
			}
			else if(m_arrEat.GetSize() > 2)			//不是就让其选择
			{
				m_bwjState = false;
				ChangBoolFalse();
				m_nActive = 0;
				m_bSelect = true;
				DrawTitle();
				DrawSelect();
				UpdateFrame();
			}
		}
		//碰
		if(m_nMousePai == TITLE_PENG)
		{
			m_bwjState=false;
			m_wjMaj[0].SetPeng();
			m_wjMaj[m_nChupaiUser].RemoveOutPai();
			m_nOutPai = -1;
			m_nActive = 0;
			PlayOtherSound(S_PEN,m_wjMaj[0].GetSex());
			DrawAllpai();
			UpdateFrame();
			OnTimer(1);
		}
		//过
		if(m_nMousePai == TITLE_GUO)
		{
			m_bwjState = false;
			m_nAbandon = true;
			UpdateFrame();
			m_nSelectTime = TimeSelect;
			KillTimer(3);
			OnTimer(1);
		}
	}
}


void CMahjongDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	m_bHandDown=false;
	//是否点击了右上角的小图标
	if(m_nMousePai == TITLE_MIN)
	{
		CloseWindow();
	}
	else if(m_nMousePai == TITLE_CLOSE)
	{
		DestroyWindow();
	}
	else if(m_nMousePai == TITLE_SET)			//作弊
	{
		m_bZuobi =! m_bZuobi;
		if(m_bRun)
		{
			DrawPai();
			UpdateFrame();
		}
	}
	//开始状态
	if(m_bStart)
	{
		if(m_nTitle == 3)
		{
			sndPlaySound("Sounds\\start.wav",SND_ASYNC);
			m_bCaton = true;
			m_bStart = false;

			SetTimer(2,50,NULL);
			SetTimer(1,TimeRun,NULL);
			SetCursor(m_hCursor);
			return;
		}
	}
	//结束状态
	if(m_bOver)
	{
		if(m_nTitle == 4)
		{
			m_bStart = true;
			m_bOver = false;
			m_bRun = false;

			m_memDC.BitBlt(0,0,800,600,&m_bgDC,0,0,SRCCOPY);
			m_memDC.BitBlt(370,400,55,28,&m_titleDC,0,127,SRCCOPY);		
			SetCursor(m_hCursor);
			UpdateFrame();
			sndPlaySound("Sounds\\bg.wav",SND_ASYNC|SND_LOOP);
			return;
		}
	}
	//运行状态
	if(m_bRun)
	{
		OnClickTitle(point);
		//如果是吃或杠的选择状态
		if(m_bSelect)
		{
			//如果玩家的状态是出牌就表明是杠的选择状态否则是吃的选择状态
			if(m_wjMaj[0].GetState() == 1 )
			{
				if(m_nSelectGang != -1)
				{
					m_bSelect = false;
					PlayOtherSound(S_GAN);
					m_wjMaj[0].SetGang(m_nSelectGang);
					m_wjMaj[0].SetNewPai(m_zmMaj.GetGangPai());
					m_nActive = 0;	
					m_nSelectGang = -1;
					DrawPaiUp(m_nMousePai);
					UpdateFrame();
				}
			}
			else
			{
				if(m_nSelectEat != -1)
				{
					int type;
					PlayOtherSound(S_EAT);
					m_bSelect = false;
					if(m_nOutPai-2 == m_nSelectEat)
					{
						type = 2;
					}
					else if(m_nOutPai-1 == m_nSelectEat)
					{
						type = 1;
					}
					else
					{
						type = 0;
					}
					m_wjMaj[0].SetChi(type);
					m_wjMaj[m_nChupaiUser].RemoveOutPai();
					m_nActive = 0;	
					m_nOutPai = -1;
					m_nSelectEat = -1;
					DrawAllpai();
					UpdateFrame();
				}
			}
			return;
		}
	}
	//玩家出牌
	if(m_bRun &&m_nMousePai != -1 && m_nMousePai < 100 && m_wjMaj[0].GetState() == 1)
	{
		m_wjMaj[0].OutPai(m_nMousePai);
		m_nWaitTime = TimeSelect;
		m_nOutPai = m_wjMaj[0].GetOutPai()[m_wjMaj[0].GetOutNum()-1];	
		ChangBoolFalse();
		m_nChupaiUser = 0;
		DrawAllpai();
		UpdateFrame();	
		sndPlaySound("Sounds\\give.wav",SND_ASYNC|SND_NODEFAULT);	
		PlayPaiSound(m_nOutPai,m_wjMaj[0].GetSex());
		return;
	}
	CDialog::OnLButtonUp(nFlags, point);
}

/*************************************************************/
/*函数功能：改变本人的状态
/*入口参数：无
/*返回类型：void
/*************************************************************/
void CMahjongDlg::ChangBoolFalse()
{
	m_bHu = false;
	m_bChi = false;
	m_bPeng = false;
	m_bGang = false;
}

/*************************************************************/
/*函数功能：统计各完家的分数
/*入口参数：无
/*返回类型：void
/*************************************************************/
void  CMahjongDlg::CheckWhoGet()
{	
	int a[4],i,j,k;
															
	for(i = 0;i < 4;i++)									//初始化各玩家分值
	{
		m_nGoodScore[i] = 0;
		if(i != 0)
		{
			a[i] = CheckNeedGet(i,m_nOutPai);	
		}
	}
	CheckPerson();
	i = m_nChupaiUser;										//获得当前谁出的牌
	for(j = 0,k = 3;j < 3;j++,k--)
	{
		i++;
		if(i > 3)											//获得各玩家的优势分
		{
			i = 0;
			if(m_bHu)
			{
				m_nGoodScore[i] = k+1000;
			}
			else if(m_bPeng||m_bGang)
			{
				m_nGoodScore[i] = k+500;
			}
			else if(m_bChi)
			{
				m_nGoodScore[i] = k+100;
			}
			else
			{
				m_nGoodScore[i] = k;
			}
		}
		else
		{
			m_nGoodScore[i] = k+a[i];
		}
	}
}

/*************************************************************/
/*函数功能：检测电脑对这张牌的需要
/*入口参数：整型（那家电脑），整型（牌）
/*返回类型：整型（返回需要的分数）
/*************************************************************/
int CMahjongDlg::CheckNeedGet(int nWho,int pai)
{
	int a[14],i,j;
	int nStar;
	m_wjMaj[nWho].SetOutPai(pai);								//设置电脑玩家的出牌
	nStar = m_wjMaj[nWho].GetPaiStart();						//获得手牌有效的开始

	if(m_wjMaj[nWho].ThinkHupai())								//考虑听牌
	{
		return 1000;
	}
	bool bPeng;
	if(m_wjMaj[nWho].CheckGang(bPeng))							//电脑考虑杠牌
	{
		for(i = 0,j = 0;i < 13-nStar;i++)
		{
			if(m_wjMaj[nWho].GetPaiInHand()[i] != pai)
			{
				a[j] = m_wjMaj[nWho].GetPaiInHand()[i+nStar];
				j++;
			}
		}
		CCmpThink ctTemp;
		ctTemp.SetMjPaiArray(a,j);
		ctTemp.Think();
		if(ctTemp.GetMaxScore() >= m_nCmpGoodScore[nWho] - 300)//如果移除那几张牌后分数不变就表示可以杠
		{
			return 800;
		}

	}
	for(i = 0;i < 13-nStar;i++)	
	{
		a[i] = m_wjMaj[nWho].GetPaiInHand()[i+nStar];
	}
	
	a[13-nStar] = pai;
	CCmpThink ct;											//思考
	ct.SetMjPaiArray(a,14-nStar);
	ct.Think();												//电脑考虑

	int ret = ct.GetBadly();								//获得最差的那张牌
	if( ret == pai )return 0;								//如果最差的牌等于当前玩家出的牌返回0
	if( ct.GetMaxScore() > m_nCmpGoodScore[nWho]+150 )		//如果当前出的牌组合后高于最大分就可以吃碰
	{		
		int pai1,pai2,pai3;								
		ct.GetGoodTeam(pai1,pai2,pai3);						//获得当前出的牌的最佳组合
		if( pai1 == pai2 )
			return 500;
		else
		{
			nWho--;
			if(nWho<0)
				nWho=3;
			if(m_nChupaiUser == nWho)						//如果是上一家出的牌即可以吃
			{
				if( pai1 == pai )
					return 100;
				else if( pai2 == pai )
					return 200;
				else
					return 300;
			}
		}
	}
	return 0;
}

void CMahjongDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	m_bHandDown = true;
	if(m_bOver||m_bStart)
	{
		DrawTitle();
		UpdateFrame();
	}
	CDialog::OnLButtonDown(nFlags, point);
	MSG message;
	while(::PeekMessage(&message,this->m_hWnd,WM_LBUTTONDOWN,WM_LBUTTONDOWN,PM_REMOVE));
}

int CMahjongDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	
	return 0;
}
