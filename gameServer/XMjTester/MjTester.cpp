#include "StdAfx.h"
#include "MjTester.h"
#include "IGameFrame.h"
#include "Message.h"
#include "../GameMessage/GameAlgorithm.h"

#define TM_SET_HAND		0x0001L
#define TM_SET_FETCH	0x0002L
#define TM_TEST_TING	0x0004L
#define TM_TEST_HU		0x0008L

//���캯��
XMjTester::XMjTester(IGameFrame *pGameFrame,IUserInterface *pUI)
{
	//���ƿؼ�
	m_pMJMyHand = NULL;

	//��Ϸ���
	m_pGameFrame = pGameFrame;

	//��������
	m_pUIShow = pUI;

	//�Լ�����ԭ����
	m_byMyHands = NULL;

	//�Լ���������
	m_pMyHandNums = NULL;

	//�����ץ��
	m_byFetch = 0;

	//������״̬
	m_dwFlag = 0;
}

//���캯��
XMjTester::~XMjTester(void)
{

}

//�󶨿����ò��ԵĿؼ�
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

//�󶨿����ò��ԵĿؼ�
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

//������������
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

//���ó��ƿؼ�����
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

//�õ���������
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

//���ÿ��õ��ƣ����ԡ��ܡ������ֶ�ѡʱʹ��\
�����ѡ����ƣ���ʱ���ϵ��ƿɰ���5���6��������5���6��������������������ƻᰵ����
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

//�ػ���ϷUI��Ϣ
int XMjTester::OnUIMessage(TUIMessage* pMessage)
{
	if (pMessage->_uMessageType == UI_KEYDOWN)
	{
		if (pMessage->_ch == VK_F3) //��������׼��
		{
			OnBeginTestTing();
		}
		else if (pMessage->_ch == VK_F4) //���Ժ���׼��
		{
			OnBeginTestHu();
		}
		else if (pMessage->_ch == VK_F5) //׼��������
		{
			OnBeginSetHands();
		}
		else if (pMessage->_ch == VK_F6) //׼��������
		{
			OnBeginSetFetch();
		}
		else if (pMessage->_ch == VK_F8) //�������Ϸ
		{
			OnWantGameFinish();
		}
		else if (pMessage->_ch == VK_F9) //���������
		{
			OnWantQuitCloseDesk();
		}
		else if (pMessage->_ch == VK_F10) //��������˷�����
		{
			OnWantGiveToken();
		}
		else if (pMessage->_ch == VK_RETURN) //ȷ��
		{
			if (m_dwFlag & TM_SET_HAND) //������ȷ��
			{
				OnSetHandsOK();
			}
			else if (m_dwFlag & TM_SET_FETCH) //��ץ��ȷ��
			{
				OnSetFetchOK();
			}
			else if (m_dwFlag & TM_TEST_TING) //������ȷ��
			{
				OnTestTingOK();
			}
			else if (m_dwFlag & TM_TEST_HU) //���Ժ���ȷ��
			{
				OnTestHuOK();
			}
		}
		else if (pMessage->_ch == VK_ESCAPE) 
		{
			if (m_dwFlag & TM_SET_HAND) //ȡ��������
			{
				OnCancelSetHands();
			}
			else if (m_dwFlag & TM_SET_FETCH) //ȡ����ץ��
			{
				OnCancelSetFetch();
			}
			else if (m_dwFlag & TM_TEST_TING) //ȡ�����Ʋ���
			{
				OnCancelTestTing();
			}
			else if (m_dwFlag & TM_TEST_HU) //ȡ�����Ʋ���
			{
				OnCancelTestHu();
			}
		}
		else
		{
			if (m_dwFlag & TM_SET_HAND) //��������
			{
				RecvMjForHand(pMessage->_ch);
			}
			else if (m_dwFlag & TM_SET_FETCH) //����ץ��
			{
				RecvMjForFetch(pMessage->_ch);
			}
			else if (m_dwFlag & TM_TEST_TING) //��������Ե�����
			{
				RecvMjTingTest(pMessage->_ch);
			}
			else if (m_dwFlag & TM_TEST_HU) //��������Ժ�������
			{
				RecvMjHuTest(pMessage->_ch);
			}
		}
	}
	return 0;
}


//�齫ֵ����
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

	//����硢����
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

	//������ͬ������
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

	if (m_dwFlag & TM_SET_HAND) //������ȷ��
	{
		UpdateHands();
	}

	return;
}

//�齫ֵ����
void XMjTester::RecvMjForFetch(UCHAR ucKey)
{
	//����硢���ơ�����
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

	//������ͬ������
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

	if (m_dwFlag & TM_SET_FETCH) //������ȷ��
	{
		UpdateFetch();
	}

	return;
}

//�齫ֵ����
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

	//����硢����
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

	//������ͬ������
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

	if (m_dwFlag & TM_TEST_TING) //������ȷ��
	{
		UpdateHands();
	}

	return;
}

//�齫ֵ����
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

	//����硢����
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

	//������ͬ������
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

	if (m_dwFlag & TM_TEST_HU) //������ȷ��
	{
		UpdateHands();
	}

	return;
}

//׼��������
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

	//�ò�����״̬
	m_dwFlag |= TM_SET_HAND;

	m_pMJMyHand->SetIsUseMouse(false);

	//��������Ա�����������ֵ

	int iCards[MJ_MAX_HANDCARD];
	m_pMJMyHand->GetHandInfo(iCards,*m_pMyHandNums);
	m_pMJMyHand->SetInvalidTile(iCards,*m_pMyHandNums);

	//��������뻺���
	m_lstMJData.clear();

	return;
}

//׼����ץ��
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

	//�ò�����״̬
	m_dwFlag |= TM_SET_FETCH;


	int iCards[MJ_MAX_HANDCARD];

	m_pMJMyHand->GetHandInfo(iCards,*m_pMyHandNums);

	if (*m_pMyHandNums < MJ_MAX_HANDCARD) //�����ץ��״̬ʱ
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


//���Ʋ���
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

	//�ò�����״̬
	m_dwFlag |= TM_TEST_TING;

	m_pMJMyHand->SetIsUseMouse(false);

	//��������Ա�����������ֵ

	int iCards[MJ_MAX_HANDCARD];
	m_pMJMyHand->GetHandInfo(iCards,*m_pMyHandNums);
	m_pMJMyHand->SetInvalidTile(iCards,*m_pMyHandNums);

	//��������뻺���
	m_lstMJData.clear();

	return;
}

//���Ʋ���
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

	//�ò�����״̬
	m_dwFlag |= TM_TEST_HU;

	m_pMJMyHand->SetIsUseMouse(false);

	//��������Ա�����������ֵ

	int iCards[MJ_MAX_HANDCARD];
	m_pMJMyHand->GetHandInfo(iCards,*m_pMyHandNums);
	m_pMJMyHand->SetInvalidTile(iCards,*m_pMyHandNums);

	//��������뻺���
	m_lstMJData.clear();

	return;
}

//ȡ����ȷ��
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
		str.Format("����%d����û����",*m_pMyHandNums - iCount);
		AfxMessageBox(str);
		return;
	}

	TXTestChHand tagCH;
	GetHandCards(m_pMJMyHand,tagCH.byCards,iCount);

	sort(tagCH.byCards,iCount,false);

	tagCH.iHandNums = iCount;
	m_pGameFrame->SendGameData(ASS_TEST_CH_HAND,&tagCH,sizeof(TXTestChHand));

	//���±�����������
	::memcpy(m_byMyHands,tagCH.byCards,iCount);

	SetHandCards(m_pMJMyHand,tagCH.byCards,iCount);

	//�ò�����״̬
	m_dwFlag &= ~TM_SET_HAND;

	m_pMJMyHand->SetIsUseMouse(true);
	m_pMJMyHand->SetInvalidTile(NULL,0);

	return;
}

//��ץ��ȷ��
void XMjTester::OnSetFetchOK()
{
	if (!m_pMJMyHand)
	{
		return;
	}

	TXTestChFetch tagCH;
	tagCH.byCard = m_byFetch;
	m_pGameFrame->SendGameData(ASS_TEST_CH_FETCH,&tagCH,sizeof(TXTestChFetch));

	//��ԭ������ʾ
	SetHandCards(m_pMJMyHand,m_byMyHands,*m_pMyHandNums);

	//�ò�����״̬
	m_dwFlag &= ~TM_SET_FETCH;

	m_pMJMyHand->SetIsUseMouse(true);
	m_pMJMyHand->SetGiveTileMode(false);
	m_pMJMyHand->SetInvalidTile(NULL,0);

	return;
}

//��������ȷ��
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


//���Ժ���ȷ��
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
	::strcpy(szHuDecr,_TEXT("���Ƽ�⣺"));

	if (CGameAlgorithm::IsCanHu(255,byHands,iCount,NULL,0,&actinfo,szHuDecr))
	{
		m_pMJMyHand->SetIsLaydown(true);
	}
	else
	{
		::strcat(szHuDecr,"���ܺ�");
	}

	m_pGameFrame->InsertNormalMessage(szHuDecr);

	return;
}

//ȡ��������
void XMjTester::OnCancelSetHands()
{
	if (!m_pMJMyHand)
	{
		return;
	}

	//�ò�����״̬
	m_dwFlag &= ~TM_SET_HAND;

	//��������Ա�����������ֵ
	SetHandCards(m_pMJMyHand,m_byMyHands,*m_pMyHandNums);

	m_pMJMyHand->SetIsUseMouse(true);
	m_pMJMyHand->SetInvalidTile(NULL,0);

	return;
}

//ȡ����ץ��
void XMjTester::OnCancelSetFetch()
{
	if (!m_pMJMyHand)
	{
		return;
	}

	//�ò�����״̬
	m_dwFlag &= ~TM_SET_FETCH;

	//��������Ա�����������ֵ
	SetHandCards(m_pMJMyHand,m_byMyHands,*m_pMyHandNums);

	m_pMJMyHand->SetGiveTileMode(false);

	m_pMJMyHand->SetIsUseMouse(true);
	m_pMJMyHand->SetInvalidTile(NULL,0);

	return;
}

//ȡ�����Ʋ���
void XMjTester::OnCancelTestTing()
{
	if (!m_pMJMyHand)
	{
		return;
	}

	//��ԭ������ʾ
	SetHandCards(m_pMJMyHand,m_byMyHands,*m_pMyHandNums);

	//�ò�����״̬
	m_dwFlag &= ~TM_TEST_TING;

	m_pMJMyHand->SetIsUseMouse(true);
	m_pMJMyHand->SetInvalidTile(NULL,0);

	return;
}

//ȡ�����Ʋ���
void XMjTester::OnCancelTestHu()
{
	if (!m_pMJMyHand)
	{
		return;
	}

	m_pMJMyHand->SetIsLaydown(false);

	//��ԭ������ʾ
	SetHandCards(m_pMJMyHand,m_byMyHands,*m_pMyHandNums);

	//�ò�����״̬
	m_dwFlag &= ~TM_TEST_HU;

	m_pMJMyHand->SetIsUseMouse(true);
	m_pMJMyHand->SetInvalidTile(NULL,0);

	return;
}


//�������Ϸ
void XMjTester::OnWantGameFinish()
{
	if (AfxMessageBox(_TEXT("����������Ϸ��"),MB_YESNO) == IDYES)
	{
		m_pGameFrame->SendGameData(ASS_TEST_GAMEFINISH,NULL,0);
	}
}

//���������
void XMjTester::OnWantQuitCloseDesk()
{
	if (AfxMessageBox(_TEXT("��������������"),MB_YESNO) == IDYES)
	{
		m_pGameFrame->SendGameData(ASS_TEST_CLOSE_DESK,NULL,0);
	}
}

//��������˷�����
void XMjTester::OnWantGiveToken()
{
	if (AfxMessageBox(_TEXT("��������˷���һ������ƣ�"),MB_YESNO) == IDYES)
	{
		m_pGameFrame->SendGameData(ASS_TEST_GIVETOKEN,NULL,0);
	}
}

//�������ƿؼ�
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

//����ץ�ƿؼ�
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

//����
//@param byCards�����ݣ�iNum��������bFirstBig = trueΪ�Ӵ�С����
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