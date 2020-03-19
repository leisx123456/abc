//#include "LClockActive.h"
//#include <thread>
//
//#define TIMER_SPACE                                   25
//
//TimerCore::TimerCore()
//{
//	std::thread thd(&TimerCore::timerThread, this);
//	thd.detach();
//	m_pTimerLink = nullptr;
//}
//
//TimerCore::~TimerCore()
//{
//}
//
//void TimerCore::setDelegate(TimerCoreLink *plink)
//{
//	std::lock_guard<std::mutex> lock(m_mutexclock);
//	m_pTimerLink = plink;
//}
//
//void TimerCore::StartGameClock(unsigned short wChairID, unsigned short wClockID, unsigned int nElapse)
//{
//	std::lock_guard<std::mutex> lock(m_mutexclock);
//
//	TimeClockData *pClockData = nullptr;
//	if (m_vClockFree.size() > 0)
//	{
//		std::vector<TimeClockData *>::iterator itbegin = m_vClockFree.begin();
//		pClockData = *itbegin;
//		m_vClockFree.erase(itbegin);
//	}
//	else
//	{
//		pClockData = new TimeClockData;
//	}
//
//	pClockData->wChairID = wChairID;
//	pClockData->wClockID = wClockID;
//	pClockData->nElapse = nElapse;
//	pClockData->nTimeLeave = nElapse * 1000;
//	pClockData->nTimeCount = 0;
//	pClockData->bdeleteClock = false;
//	m_vClockActive.push_back(pClockData);
//
//	/*
//	m_vClockData.clear();
//	TimeClockData *pClockdata = new TimeClockData;
//	pClockdata->wChairID = wChairID;
//	pClockdata->wClockID = wClockID;
//	pClockdata->nElapse = nElapse;
//	pClockdata->bIsThreadRun = true;
//	m_vClockData.push_back(pClockdata);
//	OnEventGameClockInfo(pClockdata->wChairID, pClockdata->nElapse, pClockdata->wClockID);
//	*/
//	//std::thread thd(&TimerCore::timerThread, this, pClockdata);
//	//thd.detach();
//}
//
//void TimerCore::OnTimerThreadSink()
//{
//	std::lock_guard<std::mutex> lock(m_mutexclock);
//
//	std::vector<TimeClockData *> vdeleteItem;
//	for (int i = 0; i < m_vClockActive.size(); i++)
//	{
//		TimeClockData *pClockData = m_vClockActive[i];
//		if (pClockData->bdeleteClock || !m_pTimerLink)
//		{
//			vdeleteItem.push_back(pClockData);
//			continue;
//		}
//
//		pClockData->nTimeCount += TIMER_SPACE;
//		pClockData->nTimeLeave -= TIMER_SPACE;
//		if (pClockData->nTimeCount % 1000 == 0)
//		{
//			pClockData->nElapse--;
//			m_pTimerLink->OnEventGameClockInfo(pClockData->wChairID, pClockData->nElapse, pClockData->wClockID);
//		}
//
//		if (pClockData->nTimeLeave <= 0)
//		{
//			m_pTimerLink->OnEventGameClockKill(pClockData->wChairID, pClockData->wClockID, true);
//			vdeleteItem.push_back(pClockData);
//			continue;
//		}
//	}
//
//	for (int i = 0; i < vdeleteItem.size(); i++)
//	{
//		TimeClockData *pClockData = vdeleteItem[i];
//		std::vector<TimeClockData *>::iterator it = m_vClockActive.begin();
//		for (; it != m_vClockActive.end(); it++)
//		{
//			if (pClockData == *it)
//			{
//				m_vClockActive.erase(it);
//				m_vClockFree.push_back(pClockData);
//				break;
//			}
//		}
//	}
//}
//
//void TimerCore::timerThread()
//{
//	while (true)
//	{
//		SLEEP(TIMER_SPACE);
//		OnTimerThreadSink();
//	}
//}
//
//void TimerCore::KillGameClock(unsigned short wClockID, unsigned short wChairID /*= 0xffff*/)
//{
//	std::lock_guard<std::mutex> lock(m_mutexclock);
//	std::vector<TimeClockData *>::iterator it = m_vClockActive.begin();
//	for (; it != m_vClockActive.end(); it++)
//	{
//		TimeClockData *pClockinfo = *it;
//		//如果是0kill所有时钟
//		if (wClockID == 0)
//		{
//			pClockinfo->bdeleteClock = true;
//			continue;
//		}
//
//		//如果不是当前定时id则继续循环
//		if (pClockinfo->wClockID != wClockID)
//		{
//			continue;
//		}
//
//		//如果是无效椅子id则kill所有
//		if (wChairID == INVALID_CHAIR)
//		{
//			pClockinfo->bdeleteClock = true;
//			continue;
//		}
//
//		//指定时钟指定chairid处理
//		if (wClockID == pClockinfo->wClockID && wChairID == pClockinfo->wChairID)
//		{
//			pClockinfo->bdeleteClock = true;
//			break;
//		}
//	}
//}
//
//
//
//CLClockActive::CLClockActive()
//{
//}
//
//
//CLClockActive::~CLClockActive()
//{
//}
