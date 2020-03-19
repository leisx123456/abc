#include "TimerCore.h"
#include <thread>

//#include <ctime>

// 如果定义了 __GNUC__ 就假定是 使用gcc 编译器,为Linux平台  否则 认为是 Window 平台,不可否认宏是丑陋的
#if defined(__GNUC__)
//下面是依赖 Linux 实现,等待毫秒数
#include <unistd.h>
#include <sys/time.h>
#define SLEEPMS(m)  usleep(m * 1000)
#else 
// 这里创建等待函数 以毫秒为单位 , 需要依赖操作系统实现
#include <Windows.h>
//extern int gettimeofday(struct timeval* tv, void* tz);
//#define localtime_r(t, tm) localtime_s(tm, t)

#define SLEEPMS(m)  Sleep(m)
#endif //__GNUC__

#define TIMER_SPACE                                   25

TimerCore::TimerCore()
{
	std::thread thd(&TimerCore::timerThread, this);
	thd.detach();
	m_pTimerLink = nullptr;
	//clock();
}

TimerCore::~TimerCore()
{
}

void TimerCore::setDelegate(TimerCoreLink *plink)
{
	std::lock_guard<std::mutex> lock(m_mutexclock);
	m_pTimerLink = plink;
}

void TimerCore::StartGameClock(word wChairID, word wClockID, uint nElapse)
{
	std::lock_guard<std::mutex> lock(m_mutexclock);

	TimeClockData *pClockData = nullptr;
	if (m_vClockFree.size() > 0)
	{
		std::vector<TimeClockData *>::iterator itbegin = m_vClockFree.begin();
		pClockData = *itbegin;
		m_vClockFree.erase(itbegin);
	}
	else
	{
		pClockData = new TimeClockData;
	}
	 
	pClockData->wChairID = wChairID;
	pClockData->wClockID = wClockID;
	pClockData->nElapse = nElapse;
	pClockData->nTimeLeave = nElapse * 1000;
	pClockData->nTimeCount = 0;
	pClockData->bdeleteClock = false;
	m_vClockActive.push_back(pClockData);


}

void TimerCore::OnTimerThreadSink()
{
	std::lock_guard<std::mutex> lock(m_mutexclock);

	std::vector<TimeClockData *> vdeleteItem;
	for (int i = 0; i < m_vClockActive.size(); i++)
	{
		TimeClockData *pClockData = m_vClockActive[i];
		if (pClockData->bdeleteClock || !m_pTimerLink)
		{
			vdeleteItem.push_back(pClockData);
			continue;
		}

		pClockData->nTimeCount += TIMER_SPACE;
		pClockData->nTimeLeave -= TIMER_SPACE;
		if (pClockData->nTimeCount % 1000 == 0)
		{
			pClockData->nElapse--;
			m_pTimerLink->OnEventGameClockInfo(pClockData->wChairID, pClockData->nElapse, pClockData->wClockID);
		}

		if (pClockData->nTimeLeave <= 0)
		{
			m_pTimerLink->OnEventGameClockKill(pClockData->wChairID, pClockData->wClockID, true);
			vdeleteItem.push_back(pClockData);
			continue;
		}
	}

	for (int i = 0; i < vdeleteItem.size(); i++)
	{
		TimeClockData *pClockData = vdeleteItem[i];
		std::vector<TimeClockData *>::iterator it = m_vClockActive.begin();
		for (; it != m_vClockActive.end(); it++)
		{
			if (pClockData == *it)
			{
				m_vClockActive.erase(it);
				m_vClockFree.push_back(pClockData);
				break;
			}
		}
	}
}

void TimerCore::timerThread()
{
	while (true)
	{
		SLEEPMS(TIMER_SPACE);
		OnTimerThreadSink();
	}
}

void TimerCore::KillGameClock(word wClockID, word wChairID /*= 0xffff*/)
{
	std::lock_guard<std::mutex> lock(m_mutexclock);
	std::vector<TimeClockData *>::iterator it = m_vClockActive.begin();
	for (; it != m_vClockActive.end(); it++)
	{
		TimeClockData *pClockinfo = *it;
		//如果是0kill所有时钟
		if (wClockID == 0)
		{
			pClockinfo->bdeleteClock = true;
			continue;
		}

		//如果不是当前定时id则继续循环
		if (pClockinfo->wClockID != wClockID)
		{
			continue;
		}

		//如果是无效椅子id则kill所有
		if (wChairID == 0xffff)
		{
			pClockinfo->bdeleteClock = true;
			continue;
		}

		//指定时钟指定chairid处理
		if (wClockID == pClockinfo->wClockID && wChairID == pClockinfo->wChairID)
		{
			pClockinfo->bdeleteClock = true;
			break;
		}
	}
}
