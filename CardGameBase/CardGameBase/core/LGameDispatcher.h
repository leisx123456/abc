#pragma once

#include<functional>
#include <map>
#include <mutex>

/************************************************************************/
/* 
游戏过程调度、分配器，保证游戏有序进行,但是麻将游戏有些步骤是循环的或带选择性的或者是一直等待状态,所以回调后需要手动选择下一任务
*/
/************************************************************************/

class CLGameDispatcher
{
	struct T_GameDispatcherTask
	{
		//int nTaskId;				// 任务id
		unsigned int unElapseMs;	// 该任务执行完成需要持续多长时间（毫秒）-->延迟时间执行
		std::function<void()> task;	//	任务函数

		T_GameDispatcherTask()
		{

		}

		T_GameDispatcherTask(unsigned int unElapseMs, std::function<void()> task)
			: unElapseMs(unElapseMs)
			, task(task)
		{

		}
	};

	struct T_DelayExec
	{
		unsigned int unElapseMs;
		std::function<void(int)> fnDelay;

		T_DelayExec()
		{

		}

		T_DelayExec(unsigned int unElapseMs, std::function<void(int)> fnDelay)
			: unElapseMs(unElapseMs)
			, fnDelay(fnDelay)
		{

		}
	};

public:
	CLGameDispatcher();
	~CLGameDispatcher();

	void start(int nTaskId);
	void delayExec(int nTaskId, int nChairID);
	//void dispatcherTask();

	void add(int nTaskId, unsigned int after, std::function<void()> task)
	{
		m_mapDispatcherTask[nTaskId] = T_GameDispatcherTask(after, task);
	}

	void addDelayFun(int nTaskId, unsigned int after, std::function<void(int)> fnDelay)
	{
		m_mapDelayFun[nTaskId] = T_DelayExec(after, fnDelay);
	}

	void runTimer(T_GameDispatcherTask tGameDispatcherTask);

private:
	std::map<int, T_GameDispatcherTask> m_mapDispatcherTask;
	std::map<int, T_DelayExec> m_mapDelayFun;
	std::mutex _mutex;
};

