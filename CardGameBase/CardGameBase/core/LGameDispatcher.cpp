
#include "LGameDispatcher.h"
#include "LCpp11Timer.h"

CLGameDispatcher::CLGameDispatcher()
{
}


CLGameDispatcher::~CLGameDispatcher()
{
}

void CLGameDispatcher::start(int nTaskId)
{
	if (m_mapDispatcherTask.empty())
	{
		return;
	}

	std::map<int, T_GameDispatcherTask>::iterator iter = m_mapDispatcherTask.find(nTaskId);
	if (iter == m_mapDispatcherTask.end())
	{
		return;
	}

	T_GameDispatcherTask tGameDispatcherTask = m_mapDispatcherTask.at(nTaskId);
	//std::thread([=](){
	//	std::this_thread::sleep_for(std::chrono::milliseconds(tGameDispatcherTask.unElapseMs));
	//	tGameDispatcherTask.task();
	//}).detach();

	std::thread t(&CLGameDispatcher::runTimer, this, tGameDispatcherTask);
	t.detach();
	//t.join();
}

void CLGameDispatcher::runTimer(T_GameDispatcherTask tGameDispatcherTask)
{
	//_mutex.lock();//¼ÓËø  
	std::this_thread::sleep_for(std::chrono::milliseconds(tGameDispatcherTask.unElapseMs));
	tGameDispatcherTask.task();
	//_mutex.unlock();//½âËø  
}


//void CLGameDispatcher::pause()
//{
//
//}


//void CLGameDispatcher::dispatcherTask()
//{
//	if (m_mapDispatcherTask.empty())
//	{
//		return;
//	}
//
//	T_GameDispatcherTask tGameDispatcherTask = m_mapDispatcherTask.front();
//
//	//LCpp11Timer t;
//	//t.asyncWait(tGameDispatcherTask.unElapseMs, &CLGameDispatcher::curTaskTimeOut);
//
//	std::thread([=](){
//		std::this_thread::sleep_for(std::chrono::milliseconds(tGameDispatcherTask.unElapseMs));
//		T_GameDispatcherTask tGameDispatcherTask = m_mapDispatcherTask.front();
//		tGameDispatcherTask.task();
//		m_mapDispatcherTask.pop();
//		dispatcherTask();
//	}).detach();
//}
