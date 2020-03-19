//#pragma once
//
///************************************************************************/
///* 
//ָ��ǰ��û���һ�ඨʱ��
//*/
///************************************************************************/
//
//#include <vector>
//#include <mutex>
//
//class TimerCoreLink
//{
//public:
//	virtual bool OnEventGameClockKill(unsigned short wChairID, unsigned short wClockID, bool bOverTime) = 0;
//	virtual bool OnEventGameClockInfo(unsigned short wChairID, unsigned int nElapse, unsigned short wClockID) = 0;
//};
//
//class TimerCore// : public CSingleton<TimerCore>
//{
//public:
//	struct TimeClockData
//	{
//		unsigned short		wClockID;
//		unsigned short		wChairID;
//		unsigned int		nElapse;
//		int			nTimeLeave;
//		int         nTimeCount;
//		bool		bdeleteClock;
//	};
//public:
//	TimerCore();
//	~TimerCore();
//
//	virtual void KillGameClock(unsigned short wClockID, unsigned short wChairID = 0xffff);
//	virtual void StartGameClock(unsigned short wChairID, unsigned short wClockID, unsigned int nElapse);
//	void timerThread();
//	void StopGamePulse();
//	void OnTimerThreadSink();
//	void setDelegate(TimerCoreLink *plink);
//private:
//	std::mutex							m_mutexclock;
//	std::vector<TimeClockData *>		m_vClockActive;
//	std::vector<TimeClockData *>        m_vClockFree;
//	TimerCoreLink *						m_pTimerLink;
//};
//
//
//class CLClockActive
//{
//public:
//	CLClockActive();
//	~CLClockActive();
//
//	int curActiveUser()
//	{
//		return m_nCurActiveChairID;
//	}
//	void pointNext();
//
//private:
//	int m_nTotalSetNum;		// ���õ���λ���������ܰ�������λ
//	int m_nCurPlayNum;		// ���������
//	int m_nCurActiveChairID;
//};
//
