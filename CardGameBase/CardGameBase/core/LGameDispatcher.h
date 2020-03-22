#pragma once

#include<functional>
#include <map>
#include <mutex>

/************************************************************************/
/* 
��Ϸ���̵��ȡ�����������֤��Ϸ�������,�����齫��Ϸ��Щ������ѭ���Ļ��ѡ���ԵĻ�����һֱ�ȴ�״̬,���Իص�����Ҫ�ֶ�ѡ����һ����
*/
/************************************************************************/

class CLGameDispatcher
{
	struct T_GameDispatcherTask
	{
		//int nTaskId;				// ����id
		unsigned int unElapseMs;	// ������ִ�������Ҫ�����೤ʱ�䣨���룩-->�ӳ�ʱ��ִ��
		std::function<void()> task;	//	������

		T_GameDispatcherTask()
		{

		}

		T_GameDispatcherTask(unsigned int unElapseMs, std::function<void()> task)
			: unElapseMs(unElapseMs)
			, task(task)
		{

		}
	};

public:
	CLGameDispatcher();
	~CLGameDispatcher();

	void start(int nTaskId);
	//void dispatcherTask();

	void add(int nTaskId, unsigned int after, std::function<void()> task)
	{
		m_mapDispatcherTask[nTaskId] = T_GameDispatcherTask(after, task);
	}

	void runTimer(T_GameDispatcherTask tGameDispatcherTask);

private:
	std::map<int, T_GameDispatcherTask> m_mapDispatcherTask;
	std::mutex _mutex;
};

