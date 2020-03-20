#include "StdAfx.h"
#include "MjTester.h"
#include "IGameFrame.h"
#include "Message.h"
#include "../GameMessage/GameAlgorithm.h"

#define TM_SET_HAND		0x0001L
#define TM_SET_FETCH	0x0002L
#define TM_TEST_TING	0x0004L
#define TM_TEST_HU		0x0008L

//构造函数
XMjTester::XMjTester(IGameFrame *pGameFrame,IUserInterface *pUI)
{
	//手牌控件
	m_pMJMyHand = NULL;

	//游戏框架
	m_pGameFrame = pGameFrame;

	//界面引擎
	m_pUIShow = pUI;

	//自己手牌原数据
	m_byMyHands = NULL;

	//自己手牌数量
	m_pMyHandNums = NULL;

	//输入的抓牌
	m_byFetch = 0;

	//测试器状态
	m_dwFlag = 0;
}

//构造函数
XMjTester::~XMjTester(void)
{

}

//绑定可利用测试的控件
void XMjTester::BindMjCtrl(IMjHandCtrl* pMjHand,BYTE* pHandData,const int* pHandNums)
{
	m_pMJMyHand = pMjHand;

	if (!m_pMJMyHand)
	{
		AfxMessageBox("XMjTester:BindMjCtrl fail.");
	}

	m_byMyHands = pHandData;

	m_pMyHandNums = pHandNums;

	return;
}

//绑定可利用测试的控件
void XMjTester::BindMjCtrl(int nID,BYTE* pHandData,const int* pHandNums)
{
	m_pMJMyHand = dynamic_cast<IMjHandCtrl*>(m_pUIShow->GetIControlbyID(nID));

	m_byMyHands = pHandData;

	if (!m_pMJMyHand)
	{
		AfxMessageBox("XMjTester:BindMjCtrl fail.");
	}

	m_pMyHandNums = pHandNums;

	return;
}

//设置手牌数据
void XMjTester::SetHandCards(IMjHandCtrl* pCtrl,const BYTE byTiles[],int nCount)
{
	if (!byTiles || nCount <= 0)
	{
		pCtrl->SetHandInfo(NULL,0);
		return;
	}

	int* pTile = new int[nCount];
	if (!pTile)
	{
		return;
	}

	for (int i = 0; i < nCount; ++i)
	{
		pTile[i] = byTiles[i];
	}

	pCtrl->SetHandInfo(pTile,nCount);

	delete pTile;
	pTile = NULL;

	return;
}

//设置出牌控件数据
void XMjTester::SetGiveCards(IMjGiveCtrl* pCtrl,const BYTE byTiles[],int nCount)
{
	if (!byTiles || nCount <= 0)
	{
		pCtrl->SetGiveInfo(NULL,0);
		return;
	}

	int* pTile = new int[nCount];
	if (!pTile)
	{
		return;
	}

	for (int i = 0; i < nCount; ++i)
	{
		pTile[i] = byTiles[i];
	}

	pCtrl->SetGiveInfo(pTile,nCount);

	delete pTile;
	pTile = NULL;

	return;
}

//得到手牌数据
void XMjTester::GetHandCards(IMjHandCtrl* pCtrl,BYTE byTiles[],int nCount)
{
	if (!byTiles || nCount <= 0)
	{
		return;
	}

	int* pTile = new int[nCount];
	if (!pTile)
	{
		return;
	}

	pCtrl->GetHandInfo(pTile,nCount);

	for (int i = 0; i < nCount; ++i)
	{
		byTiles[i] = pTile[i];
	}

	delete pTile;
	pTile = NULL;

	return;
}

//设置可用的牌，当吃、杠、听出现多选时使用\
如玩家选择杠牌，这时手上的牌可暗杠5万或6万，则手上5万和6万的两张牌明亮，其它牌会暗掉。
void XMjTester::SetValidTile(IMjHandCtrl* pCtrl,const BYTE byTiles[],int nCount)
{
	BYTE byInvHands[MJ_MAX_HANDCARD];

	int iHandNums = pCtrl->GetTileCount();

	if (iHandNums < nCount)
	{
		return;
	}

	GetHandCards(pCtrl,byInvHands,iHandNums);

	int iDelNums = CGameAlgorithm::RemoveCards(byTiles,nCount,byInvHands,iHandNums);

	int iInvHands[MJ_MAX_HANDCARD];

	for (int i = 0; i < iHandNums - iDelNums; ++i)
	{
		iInvHands[i] = byInvHands[i];
	}

	//pCtrl->SetIsUseMouse(false);

	pCtrl->SetInvalidTile(iInvHands,iHandNums - iDelNums);

	return;
}

//截获游戏UI消息
int XMjTester::OnUIMessage(TUIMessage* pMessage)
{
	if (pMessage->_uMessageType == UI_KEYDOWN)
	{
		if (pMessage->_ch == VK_F3) //测试听牌准备
		{
			OnBeginTestTing();
		}
		else if (pMessage->_ch == VK_F4) //测试胡牌准备
		{
			OnBeginTestHu();
		}
		else if (pMessage->_ch == VK_F5) //准备配手牌
		{
			OnBeginSetHands();
		}
		else if (pMessage->_ch == VK_F6) //准备配手牌
		{
			OnBeginSetFetch();
		}
		else if (pMessage->_ch == VK_F8) //想结束游戏
		{
			OnWantGameFinish();
		}
		else if (pMessage->_ch == VK_F9) //想快速收桌
		{
			OnWantQuitCloseDesk();
		}
		else if (pMessage->_ch == VK_F10) //触发服务端发令牌
		{
			OnWantGiveToken();
		}
		else if (pMessage->_ch == VK_RETURN) //确认
		{
			if (m_dwFlag & TM_SET_HAND) //设手牌确定
			{
				OnSetHandsOK();
			}
			else if (m_dwFlag & TM_SET_FETCH) //设抓牌确定
			{
				OnSetFetchOK();
			}
			else if (m_dwFlag & TM_TEST_TING) //测试听确定
			{
				OnTestTingOK();
			}
			else if (m_dwFlag & TM_TEST_HU) //测试胡牌确定
			{
				OnTestHuOK();
			}
		}
		else if (pMessage->_ch == VK_ESCAPE) 
		{
			if (m_dwFlag & TM_SET_HAND) //取消配手牌
			{
				OnCancelSetHands();
			}
			else if (m_dwFlag & TM_SET_FETCH) //取消配抓牌
			{
				OnCancelSetFetch();
			}
			else if (m_dwFlag & TM_TEST_TING) //取消听牌测试
			{
				OnCancelTestTing();
			}
			else if (m_dwFlag & TM_TEST_HU) //取消胡牌测试
			{
				OnCancelTestHu();
			}
		}
		else
		{
			if (m_dwFlag & TM_SET_HAND) //输入手牌
			{
				RecvMjForHand(pMessage->_ch);
			}
			else if (m_dwFlag & TM_SET_FETCH) //输入抓牌
			{
				RecvMjForFetch(pMessage->_ch);
			}
			else if (m_dwFlag & TM_TEST_TING) //输入待测试的听牌
			{
				RecvMjTingTest(pMessage->_ch);
			}
			else if (m_dwFlag & TM_TEST_HU) //输入待测试胡的手牌
			{
				RecvMjHuTest(pMessage->_ch);
			}
		}
	}
	return 0;
}


//麻将值接收
void XMjTester::RecvMjForHand(UCHAR ucKey)
{
	if (ucKey == VK_BACK && m_lstMJData.size() > 0)
	{
		m_lstMJData.pop_back();
	}

	if (m_lstMJData.size() >= *m_pMyHandNums)
	{
		return;
	}

	//键入风、箭牌
	switch(ucKey)
	{
		case 'D':
			m_lstMJData.push_back(MjKind::Feng + MjFeng::Dong);
			break;
		case 'X':
			m_lstMJData.push_back(MjKind::Feng + MjFeng::Nan);
			break;
		case 'A':
			m_lstMJData.push_back(MjKind::Feng + MjFeng::Xi);
			break;
		case 'W':
			m_lstMJData.push_back(MjKind::Feng + MjFeng::Bei);
			break;
		case 'S':
			m_lstMJData.push_back(MjKind::Feng + MjFeng::Zhong);
			break;
		case 'K':
			m_lstMJData.push_back(MjKind::Feng + MjFeng::Fa);
			break;
		case 'L':
			m_lstMJData.push_back(MjKind::Feng + MjFeng::Bai);
			break;
		default:
			break;
	}

	//健入万、同、条牌
	if (ucKey >= '1' && ucKey <= '9')
	{
		BYTE byIndex = ucKey - 0x30;

		if (GetKeyState(VK_LSHIFT) & 0x8000)
		{
			m_lstMJData.push_back(MjKind::Tiao + byIndex);
		}
		else if (GetKeyState(VK_LCONTROL) & 0x8000)
		{
			m_lstMJData.push_back(MjKind::Bing + byIndex);
		}
		else
		{
			m_lstMJData.push_back(MjKind::Wan + byIndex);
		}
	}

	if (m_dwFlag & TM_SET_HAND) //设手牌确定
	{
		UpdateHands();
	}

	return;
}

//麻将值接收
void XMjTester::RecvMjForFetch(UCHAR ucKey)
{
	//键入风、箭牌、花牌
	switch(ucKey)
	{
		case 'D':
			m_byFetch = MjKind::Feng + MjFeng::Dong;
			break;
		case 'X':
			m_byFetch = MjKind::Feng + MjFeng::Nan;
			break;
		case 'A':
			m_byFetch = MjKind::Feng + MjFeng::Xi;
			break;
		case 'W':
			m_byFetch = MjKind::Feng + MjFeng::Bei;
			break;
		case 'S':
			m_byFetch = MjKind::Feng + MjFeng::Zhong;
			break;
		case 'K':
			m_byFetch = MjKind::Feng + MjFeng::Fa;
			break;
		case 'L':
			m_byFetch = MjKind::Feng + MjFeng::Bai;
			break;

		case 'E':
			m_byFetch = MjKind::Hua + MjHua::Chun;
			break;
		case 'R':
			m_byFetch = MjKind::Hua + MjHua::Xia;
			break;
		case 'T':
			m_byFetch = MjKind::Hua + MjHua::Qiu;
			break;
		case 'Y':
			m_byFetch = MjKind::Hua + MjHua::Dong;
			break;
		case 'U':
			m_byFetch = MjKind::Hua + MjHua::Mei;
			break;
		case 'I':
			m_byFetch = MjKind::Hua + MjHua::Lan;
			break;
		case 'O':
			m_byFetch = MjKind::Hua + MjHua::Zhu;
			break;
		case 'P':
			m_byFetch = MjKind::Hua + MjHua::Ju;
			break;
		default:
			break;
	}

	//健入万、同、条牌
	if (ucKey >= '1' && ucKey <= '9')
	{
		BYTE byIndex = ucKey - 0x30;

		if (GetKeyState(VK_LSHIFT) & 0x8000)
		{
			m_byFetch = MjKind::Tiao + byIndex;
		}
		else if (GetKeyState(VK_LCONTROL) & 0x8000)
		{
			m_byFetch = MjKind::Bing + byIndex;
		}
		else
		{
			m_byFetch = MjKind::Wan + byIndex;
		}
	}

	if (m_dwFlag & TM_SET_FETCH) //设手牌确定
	{
		UpdateFetch();
	}

	return;
}

//麻将值接收
void XMjTester::RecvMjTingTest(UCHAR ucKey)
{
	if (ucKey == VK_BACK && m_lstMJData.size() > 0)
	{
		m_lstMJData.pop_back();
	}

	if (m_lstMJData.size() >= 14)
	{
		return;
	}

	//键入风、箭牌
	switch(ucKey)
	{
	case 'D':
		m_lstMJData.push_back(MjKind::Feng + MjFeng::Dong);
		break;
	case 'X':
		m_lstMJData.push_back(MjKind::Feng + MjFeng::Nan);
		break;
	case 'A':
		m_lstMJData.push_back(MjKind::Feng + MjFeng::Xi);
		break;
	case 'W':
		m_lstMJData.push_back(MjKind::Feng + MjFeng::Bei);
		break;
	case 'S':
		m_lstMJData.push_back(MjKind::Feng + MjFeng::Zhong);
		break;
	case 'K':
		m_lstMJData.push_back(MjKind::Feng + MjFeng::Fa);
		break;
	case 'L':
		m_lstMJData.push_back(MjKind::Feng + MjFeng::Bai);
		break;
	default:
		break;
	}

	//健入万、同、条牌
	if (ucKey >= '1' && ucKey <= '9')
	{
		BYTE byIndex = ucKey - 0x30;

		if (GetKeyState(VK_LSHIFT) & 0x8000)
		{
			m_lstMJData.push_back(MjKind::Tiao + byIndex);
		}
		else if (GetKeyState(VK_LCONTROL) & 0x8000)
		{
			m_lstMJData.push_back(MjKind::Bing + byIndex);
		}
		else
		{
			m_lstMJData.push_back(MjKind::Wan + byIndex);
		}
	}

	if (m_dwFlag & TM_TEST_TING) //设手牌确定
	{
		UpdateHands();
	}

	return;
}

//麻将值接收
void XMjTester::RecvMjHuTest(UCHAR ucKey)
{
	m_pMJMyHand->SetIsLaydown(false);

	if (ucKey == VK_BACK && m_lstMJData.size() > 0)
	{
		m_lstMJData.pop_back();
	}

	if (m_lstMJData.size() >= 14)
	{
		return;
	}

	//键入风、箭牌
	switch(ucKey)
	{
	case 'D':
		m_lstMJData.push_back(MjKind::Feng + MjFeng::Dong);
		break;
	case 'X':
		m_lstMJData.push_back(MjKind::Feng + MjFeng::Nan);
		break;
	case 'A':
		m_lstMJData.push_back(MjKind::Feng + MjFeng::Xi);
		break;
	case 'W':
		m_lstMJData.push_back(MjKind::Feng + MjFeng::Bei);
		break;
	case 'S':
		m_lstMJData.push_back(MjKind::Feng + MjFeng::Zhong);
		break;
	case 'K':
		m_lstMJData.push_back(MjKind::Feng + MjFeng::Fa);
		break;
	case 'L':
		m_lstMJData.push_back(MjKind::Feng + MjFeng::Bai);
		break;
	default:
		break;
	}

	//健入万、同、条牌
	if (ucKey >= '1' && ucKey <= '9')
	{
		BYTE byIndex = ucKey - 0x30;

		if (GetKeyState(VK_LSHIFT) & 0x8000)
		{
			m_lstMJData.push_back(MjKind::Tiao + byIndex);
		}
		else if (GetKeyState(VK_LCONTROL) & 0x8000)
		{
			m_lstMJData.push_back(MjKind::Bing + byIndex);
		}
		else
		{
			m_lstMJData.push_back(MjKind::Wan + byIndex);
		}
	}

	if (m_dwFlag & TM_TEST_HU) //设手牌确定
	{
		UpdateHands();
	}

	return;
}

//准备配手牌
void XMjTester::OnBeginSetHands()
{
	if (!m_pMJMyHand)
	{
		return;
	}

	if (m_dwFlag)
	{
		return;
	}

	//置测试器状态
	m_dwFlag |= TM_SET_HAND;

	m_pMJMyHand->SetIsUseMouse(false);

	//清空手牌以便给玩家设置牌值

	int iCards[MJ_MAX_HANDCARD];
	m_pMJMyHand->GetHandInfo(iCards,*m_pMyHandNums);
	m_pMJMyHand->SetInvalidTile(iCards,*m_pMyHandNums);

	//清空牌输入缓冲表
	m_lstMJData.clear();

	return;
}

//准备配抓牌
void XMjTester::OnBeginSetFetch()
{
	if (!m_pMJMyHand)
	{
		return;
	}

	if (m_dwFlag)
	{
		return;
	}

	//置测试器状态
	m_dwFlag |= TM_SET_FETCH;


	int iCards[MJ_MAX_HANDCARD];

	m_pMJMyHand->GetHandInfo(iCards,*m_pMyHandNums);

	if (*m_pMyHandNums < MJ_MAX_HANDCARD) //玩家在抓牌状态时
	{
		iCards[*m_pMyHandNums] = MjKind::Feng + MjFeng::Fa;
		m_pMJMyHand->SetHandInfo(iCards,*m_pMyHandNums + 1);
		m_pMJMyHand->SetInvalidTile(iCards,*m_pMyHandNums + 1);
	}
	else
	{
		iCards[*m_pMyHandNums - 1] = MjKind::Feng + MjFeng::Fa;
		m_pMJMyHand->SetHandInfo(iCards,*m_pMyHandNums);
		m_pMJMyHand->SetInvalidTile(iCards,*m_pMyHandNums);
	}

	m_byFetch = MjKind::Feng + MjFeng::Fa;

	m_pMJMyHand->SetGiveTileMode(true);
	m_pMJMyHand->SetIsUseMouse(false);


	return;
}


//听牌测试
void XMjTester::OnBeginTestTing()
{
	if (!m_pMJMyHand)
	{
		return;
	}

	if (m_dwFlag)
	{
		return;
	}

	//置测试器状态
	m_dwFlag |= TM_TEST_TING;

	m_pMJMyHand->SetIsUseMouse(false);

	//清空手牌以便给玩家设置牌值

	int iCards[MJ_MAX_HANDCARD];
	m_pMJMyHand->GetHandInfo(iCards,*m_pMyHandNums);
	m_pMJMyHand->SetInvalidTile(iCards,*m_pMyHandNums);

	//清空牌输入缓冲表
	m_lstMJData.clear();

	return;
}

//胡牌测试
void XMjTester::OnBeginTestHu()
{
	if (!m_pMJMyHand)
	{
		return;
	}

	if (m_dwFlag)
	{
		return;
	}

	//置测试器状态
	m_dwFlag |= TM_TEST_HU;

	m_pMJMyHand->SetIsUseMouse(false);

	//清空手牌以便给玩家设置牌值

	int iCards[MJ_MAX_HANDCARD];
	m_pMJMyHand->GetHandInfo(iCards,*m_pMyHandNums);
	m_pMJMyHand->SetInvalidTile(iCards,*m_pMyHandNums);

	//清空牌输入缓冲表
	m_lstMJData.clear();

	return;
}

//取消配确定
void XMjTester::OnSetHandsOK()
{
	if (!m_pMJMyHand)
	{
		return;
	}

	int iCount = m_lstMJData.size();

	if (*m_pMyHandNums != iCount)
	{
		CString str;
		str.Format("还差%d张牌没设置",*m_pMyHandNums - iCount);
		AfxMessageBox(str);
		return;
	}

	TXTestChHand tagCH;
	GetHandCards(m_pMJMyHand,tagCH.byCards,iCount);

	sort(tagCH.byCards,iCount,false);

	tagCH.iHandNums = iCount;
	m_pGameFrame->SendGameData(ASS_TEST_CH_HAND,&tagCH,sizeof(TXTestChHand));

	//更新本地手牌数据
	::memcpy(m_byMyHands,tagCH.byCards,iCount);

	SetHandCards(m_pMJMyHand,tagCH.byCards,iCount);

	//置测试器状态
	m_dwFlag &= ~TM_SET_HAND;

	m_pMJMyHand->SetIsUseMouse(true);
	m_pMJMyHand->SetInvalidTile(NULL,0);

	return;
}

//配抓牌确定
void XMjTester::OnSetFetchOK()
{
	if (!m_pMJMyHand)
	{
		return;
	}

	TXTestChFetch tagCH;
	tagCH.byCard = m_byFetch;
	m_pGameFrame->SendGameData(ASS_TEST_CH_FETCH,&tagCH,sizeof(TXTestChFetch));

	//还原手牌显示
	SetHandCards(m_pMJMyHand,m_byMyHands,*m_pMyHandNums);

	//置测试器状态
	m_dwFlag &= ~TM_SET_FETCH;

	m_pMJMyHand->SetIsUseMouse(true);
	m_pMJMyHand->SetGiveTileMode(false);
	m_pMJMyHand->SetInvalidTile(NULL,0);

	return;
}

//测试听牌确定
void XMjTester::OnTestTingOK()
{
	if (!m_pMJMyHand)
	{
		return;
	}

	int iCount = m_lstMJData.size();

	BYTE byHands[MJ_MAX_HANDCARD];
	GetHandCards(m_pMJMyHand,byHands,iCount);

	MjBase::TMjActInfo tagActInfo;

	if (CGameAlgorithm::IsCanTing(byHands,iCount,&tagActInfo))
	{
		BYTE byTingList[MJ_MAX_HANDCARD];
		BYTE* pTingList = byTingList;

		for (int i = 0; i < tagActInfo.iTingNums; ++i)
		{
			int iCardNums = CGameAlgorithm::GetACardCount(tagActInfo.byTingSels[i],byHands,iCount);
			::memset(pTingList,tagActInfo.byTingSels[i],iCardNums);
			pTingList += iCardNums;
		}

		SetValidTile(m_pMJMyHand,byTingList,pTingList - byTingList);
	}
}


//测试胡牌确定
void XMjTester::OnTestHuOK()
{
	if (!m_pMJMyHand)
	{
		return;
	}

	int iCount = m_lstMJData.size();

	BYTE byHands[MJ_MAX_HANDCARD];
	GetHandCards(m_pMJMyHand,byHands,iCount);

	MjBase::TMjActInfo actinfo;

// 	TMjCGPNode cgp[1];
// 	cgp->nType = TMjCGPNode::MingGang;
// 	cgp->val[0] = 22;
// 	cgp->val[1] = 22;
// 	cgp->val[2] = 22;
// 	cgp->val[3] = 22;
	
	TCHAR szHuDecr[128];
	::strcpy(szHuDecr,_TEXT("胡牌检测："));

	if (CGameAlgorithm::IsCanHu(255,byHands,iCount,NULL,0,&actinfo,szHuDecr))
	{
		m_pMJMyHand->SetIsLaydown(true);
	}
	else
	{
		::strcat(szHuDecr,"不能胡");
	}

	m_pGameFrame->InsertNormalMessage(szHuDecr);

	return;
}

//取消配手牌
void XMjTester::OnCancelSetHands()
{
	if (!m_pMJMyHand)
	{
		return;
	}

	//置测试器状态
	m_dwFlag &= ~TM_SET_HAND;

	//清空手牌以便给玩家设置牌值
	SetHandCards(m_pMJMyHand,m_byMyHands,*m_pMyHandNums);

	m_pMJMyHand->SetIsUseMouse(true);
	m_pMJMyHand->SetInvalidTile(NULL,0);

	return;
}

//取消配抓牌
void XMjTester::OnCancelSetFetch()
{
	if (!m_pMJMyHand)
	{
		return;
	}

	//置测试器状态
	m_dwFlag &= ~TM_SET_FETCH;

	//清空手牌以便给玩家设置牌值
	SetHandCards(m_pMJMyHand,m_byMyHands,*m_pMyHandNums);

	m_pMJMyHand->SetGiveTileMode(false);

	m_pMJMyHand->SetIsUseMouse(true);
	m_pMJMyHand->SetInvalidTile(NULL,0);

	return;
}

//取消听牌测试
void XMjTester::OnCancelTestTing()
{
	if (!m_pMJMyHand)
	{
		return;
	}

	//还原手牌显示
	SetHandCards(m_pMJMyHand,m_byMyHands,*m_pMyHandNums);

	//置测试器状态
	m_dwFlag &= ~TM_TEST_TING;

	m_pMJMyHand->SetIsUseMouse(true);
	m_pMJMyHand->SetInvalidTile(NULL,0);

	return;
}

//取消胡牌测试
void XMjTester::OnCancelTestHu()
{
	if (!m_pMJMyHand)
	{
		return;
	}

	m_pMJMyHand->SetIsLaydown(false);

	//还原手牌显示
	SetHandCards(m_pMJMyHand,m_byMyHands,*m_pMyHandNums);

	//置测试器状态
	m_dwFlag &= ~TM_TEST_HU;

	m_pMJMyHand->SetIsUseMouse(true);
	m_pMJMyHand->SetInvalidTile(NULL,0);

	return;
}


//想结束游戏
void XMjTester::OnWantGameFinish()
{
	if (AfxMessageBox(_TEXT("决定结束游戏？"),MB_YESNO) == IDYES)
	{
		m_pGameFrame->SendGameData(ASS_TEST_GAMEFINISH,NULL,0);
	}
}

//想快速收桌
void XMjTester::OnWantQuitCloseDesk()
{
	if (AfxMessageBox(_TEXT("决定立刻收桌？"),MB_YESNO) == IDYES)
	{
		m_pGameFrame->SendGameData(ASS_TEST_CLOSE_DESK,NULL,0);
	}
}

//触发服务端发令牌
void XMjTester::OnWantGiveToken()
{
	if (AfxMessageBox(_TEXT("触发服务端发下一玩家令牌？"),MB_YESNO) == IDYES)
	{
		m_pGameFrame->SendGameData(ASS_TEST_GIVETOKEN,NULL,0);
	}
}

//更新手牌控件
void XMjTester::UpdateHands()
{
	if (!m_pMJMyHand)
	{
		return;
	}

	int  iCards[MJ_MAX_HANDCARD];
	int* pos = iCards;

	std::list<int>::iterator itMJData = m_lstMJData.begin();
	while (itMJData != m_lstMJData.end())
	{
		*(pos++) = *itMJData;
		itMJData++;
	}

	m_pMJMyHand->SetHandInfo(iCards,m_lstMJData.size());
	m_pMJMyHand->SetInvalidTile(iCards,m_lstMJData.size());
}

//更新抓牌控件
void XMjTester::UpdateFetch()
{
	if (!m_pMJMyHand)
	{
		return;
	}

	int iCards[MJ_MAX_HANDCARD];

	m_pMJMyHand->GetHandInfo(iCards,*m_pMyHandNums);

	if (*m_pMyHandNums < MJ_MAX_HANDCARD)
	{
		iCards[*m_pMyHandNums] = m_byFetch;
		m_pMJMyHand->SetHandInfo(iCards,*m_pMyHandNums + 1);
		m_pMJMyHand->SetInvalidTile(iCards,*m_pMyHandNums + 1);
	}
	else
	{
		iCards[*m_pMyHandNums - 1] = m_byFetch;
		m_pMJMyHand->SetHandInfo(iCards,*m_pMyHandNums);
		m_pMJMyHand->SetInvalidTile(iCards,*m_pMyHandNums);
	}

	return;
}

//排序
//@param byCards牌数据，iNum牌数量，bFirstBig = true为从大到小排列
//@return
void XMjTester::sort(BYTE byCards[], int iNums,bool bFirstBig)
{
	if(byCards == NULL)
		return ;

	for (int i =0; i < iNums - 1; i++)
	{
		for(int j = 0; j < iNums - i -1; j++)
		{
			if(bFirstBig)
			{
				if(byCards[j] < byCards[j+1])
				{
					int iTempBig = byCards[j];
					byCards[j] = byCards[j+1];
					byCards[j+1] = iTempBig;		
				}
			}
			else
			{
				if(byCards[j] > byCards[j+1])
				{
					int iTempBig = byCards[j];
					byCards[j] = byCards[j+1];
					byCards[j+1] = iTempBig;		
				}
			}
		}	
	}

	return;
}