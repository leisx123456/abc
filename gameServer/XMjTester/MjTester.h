// ***************************************************************
//  GameAlgorithm   version:  1.0   ·  date: 13/7/2011
//  -------------------------------------------------------------

//		麻将游戏测试工具，适合大多数麻将开发时使用\

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

	//构造/析构
	XMjTester(IGameFrame *pGameFrame,IUserInterface *pUI);

	virtual ~XMjTester(void);

public:

	//绑定可利用测试的控件
	void BindMjCtrl(IMjHandCtrl* pMjHand,BYTE* pHandData,const int* pHandNums);

	//绑定可利用测试的控件
	void BindMjCtrl(int nID,BYTE* pHandData,const int* pHandNums);

	//设置手牌数据
	void SetHandCards(IMjHandCtrl* pCtrl,const BYTE byTiles[],int nCount);

	//设置出牌控件数据
	void SetGiveCards(IMjGiveCtrl* pCtrl,const BYTE byTiles[],int nCount);

	//得到手牌数据
	void GetHandCards(IMjHandCtrl* pCtrl,BYTE byTiles[],int nCount);

	//设置可用的牌，当吃、杠、听出现多选时使用
	void SetValidTile(IMjHandCtrl* pCtrl,const BYTE byTiles[],int nCount);

public:

	//截获游戏UI消息
	int OnUIMessage(TUIMessage* pMessage);

/*-消息处理-*/
protected:

	//麻将值接收
	void RecvMjForHand(UCHAR ucKey);

	//麻将值接收
	void RecvMjForFetch(UCHAR ucKey);

	//麻将值接收
	void RecvMjTingTest(UCHAR ucKey);

	//麻将值接收
	void RecvMjHuTest(UCHAR ucKey);

	//准备配手牌
	void OnBeginSetHands();

	//准备配抓牌
	void OnBeginSetFetch();

	//听牌测试
	void OnBeginTestTing();

	//胡牌测试
	void OnBeginTestHu();

	//配手牌确定
	void OnSetHandsOK();

	//配抓牌确定
	void OnSetFetchOK();

	//测试听牌确定
	void OnTestTingOK();

	//测试胡牌确定
	void OnTestHuOK();

	//取消配手牌
	void OnCancelSetHands();
	
	//取消配抓牌
	void OnCancelSetFetch();

	//取消听牌测试
	void OnCancelTestTing();

	//取消胡牌测试
	void OnCancelTestHu();

	//想结束游戏
	void OnWantGameFinish();

	//想快速收桌
	void OnWantQuitCloseDesk();

	//触发服务端发令牌
	void OnWantGiveToken();

	//更新手牌控件
	void UpdateHands();

	//更新抓牌控件
	void UpdateFetch();

/*-一些算法-*/
public:

	//排序
	void sort(BYTE byCards[], int iNums,bool bFirstBig);


/*-框架相关-*/
private:

	//游戏框架
	IGameFrame* m_pGameFrame;

	//界面引擎
	IUserInterface* m_pUIShow;

/*-控件及一些数据-*/
private:

	//自己手牌控件
	IMjHandCtrl* m_pMJMyHand;

	//自己手牌原数据
	BYTE* m_byMyHands;

	//自己手牌数量
	const int* m_pMyHandNums;

	//测试器状态
	DWORD m_dwFlag;

	//输入的麻将数据
	std::list<int> m_lstMJData;

	//输入的抓牌
	BYTE m_byFetch;


};