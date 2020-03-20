// ***************************************************************
//  GameAlgorithm   version:  1.0   ��  date: 13/7/2011
//  -------------------------------------------------------------

//		�齫��Ϸ���Թ��ߣ��ʺϴ�����齫����ʱʹ��\

//  -------------------------------------------------------------
//  Copyright (C) 2011 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************

#pragma once

#include "UI_ForLogic/IUserInterface.h"
#include "Common/UIMessageDefines.h"

#include "MjCommonDef.h"
using namespace MjBase;

#include <list>

interface IGameFrame;
interface IUserInterface;

class XMjTester
{

public:

	//����/����
	XMjTester(IGameFrame *pGameFrame,IUserInterface *pUI);

	virtual ~XMjTester(void);

public:

	//�󶨿����ò��ԵĿؼ�
	void BindMjCtrl(IMjHandCtrl* pMjHand,BYTE* pHandData,const int* pHandNums);

	//�󶨿����ò��ԵĿؼ�
	void BindMjCtrl(int nID,BYTE* pHandData,const int* pHandNums);

	//������������
	void SetHandCards(IMjHandCtrl* pCtrl,const BYTE byTiles[],int nCount);

	//���ó��ƿؼ�����
	void SetGiveCards(IMjGiveCtrl* pCtrl,const BYTE byTiles[],int nCount);

	//�õ���������
	void GetHandCards(IMjHandCtrl* pCtrl,BYTE byTiles[],int nCount);

	//���ÿ��õ��ƣ����ԡ��ܡ������ֶ�ѡʱʹ��
	void SetValidTile(IMjHandCtrl* pCtrl,const BYTE byTiles[],int nCount);

public:

	//�ػ���ϷUI��Ϣ
	int OnUIMessage(TUIMessage* pMessage);

/*-��Ϣ����-*/
protected:

	//�齫ֵ����
	void RecvMjForHand(UCHAR ucKey);

	//�齫ֵ����
	void RecvMjForFetch(UCHAR ucKey);

	//�齫ֵ����
	void RecvMjTingTest(UCHAR ucKey);

	//�齫ֵ����
	void RecvMjHuTest(UCHAR ucKey);

	//׼��������
	void OnBeginSetHands();

	//׼����ץ��
	void OnBeginSetFetch();

	//���Ʋ���
	void OnBeginTestTing();

	//���Ʋ���
	void OnBeginTestHu();

	//������ȷ��
	void OnSetHandsOK();

	//��ץ��ȷ��
	void OnSetFetchOK();

	//��������ȷ��
	void OnTestTingOK();

	//���Ժ���ȷ��
	void OnTestHuOK();

	//ȡ��������
	void OnCancelSetHands();
	
	//ȡ����ץ��
	void OnCancelSetFetch();

	//ȡ�����Ʋ���
	void OnCancelTestTing();

	//ȡ�����Ʋ���
	void OnCancelTestHu();

	//�������Ϸ
	void OnWantGameFinish();

	//���������
	void OnWantQuitCloseDesk();

	//��������˷�����
	void OnWantGiveToken();

	//�������ƿؼ�
	void UpdateHands();

	//����ץ�ƿؼ�
	void UpdateFetch();

/*-һЩ�㷨-*/
public:

	//����
	void sort(BYTE byCards[], int iNums,bool bFirstBig);


/*-������-*/
private:

	//��Ϸ���
	IGameFrame* m_pGameFrame;

	//��������
	IUserInterface* m_pUIShow;

/*-�ؼ���һЩ����-*/
private:

	//�Լ����ƿؼ�
	IMjHandCtrl* m_pMJMyHand;

	//�Լ�����ԭ����
	BYTE* m_byMyHands;

	//�Լ���������
	const int* m_pMyHandNums;

	//������״̬
	DWORD m_dwFlag;

	//������齫����
	std::list<int> m_lstMJData;

	//�����ץ��
	BYTE m_byFetch;


};