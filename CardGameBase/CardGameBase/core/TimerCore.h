#ifndef _INC_TimerCore_H_
#define _INC_TimerCore_H_

#include <vector>
#include <mutex>


#ifndef word
typedef unsigned short      word;
typedef unsigned int		uint;
#endif // !word


class TimerCoreLink
{
public:
	virtual bool OnEventGameClockKill(word wChairID, word wClockID, bool bOverTime) = 0;
	virtual bool OnEventGameClockInfo(word wChairID, uint nElapse, word wClockID) = 0;
};

class TimerCore
{
public:
	struct TimeClockData
	{
		word		wClockID;
		word		wChairID;
		uint		nElapse;
		int			nTimeLeave;
		int         nTimeCount;
		bool		bdeleteClock;
	};
public:
	TimerCore();
	~TimerCore();

	virtual void KillGameClock(word wClockID, word wChairID = 0xffff);
	virtual void StartGameClock(word wChairID, word wClockID, uint nElapse);
	void timerThread();
	void StopGamePulse();
	void OnTimerThreadSink();
	void setDelegate(TimerCoreLink *plink);
private:
	std::mutex							m_mutexclock;
	std::vector<TimeClockData *>		m_vClockActive;
	std::vector<TimeClockData *>        m_vClockFree;
	TimerCoreLink *						m_pTimerLink;
};



#endif