#pragma once

#include<functional>
#include<chrono>
#include<thread>
#include<atomic>
#include<memory>
#include<mutex>
#include<condition_variable>

/*调用方式

int _tmain(int argc, _TCHAR* argv[])
{
LCpp11Timer t;

t.startTimer(1000, std::bind(EchoFunc, "hello world!")); // 绑定参数
std::this_thread::sleep_for(std::chrono::seconds(4));
std::cout << "try to expire timer!" << std::endl;
t.killTimer();

//周期性执行定时任务
t.startTimer(1000, std::bind(EchoFunc, "hello c++11!"));
std::this_thread::sleep_for(std::chrono::seconds(4));
std::cout << "try to expire timer!" << std::endl;
t.killTimer();

std::this_thread::sleep_for(std::chrono::seconds(2));

//只执行一次定时任务
//同步
t.syncWait(1000, EchoFunc, "hello world!");
//异步
t.asyncWait(1000, EchoFunc, "hello c++11!");

std::this_thread::sleep_for(std::chrono::seconds(2));


//std::vector<std::thread> ths;
//for (int i = 0; i < 10; i++)
//	ths.push_back(std::thread(append_numer, i));
//for (int i = 0; i < 10; i++)
//	ths[i].join();
//std::cout << stream.str();

while (true)
{

}
return 0;
}
*/
/************************************************************************/
/* 
理解难度：大
*/
/************************************************************************/
class LCpp11Timer
{
public:
	LCpp11Timer();
	LCpp11Timer(const LCpp11Timer& t);
	~LCpp11Timer();

	//周期性执行定时任务
	void startTimer(int interval, std::function<void()> task);

	void killTimer();	// expire

	// 等待一段时间后同步执行一次
	template<typename callable, class... arguments>
	void syncWait(int after, callable&& f, arguments&&... args)
	{
		std::function<typename std::result_of<callable(arguments...)>::type()> task(std::bind(std::forward<callable>(f), std::forward<arguments>(args)...));
		std::this_thread::sleep_for(std::chrono::milliseconds(after));
		task();
	}

	// 等待一段时间后异步执行一次
	template<typename callable, class... arguments>
	void asyncWait(int after, callable&& f, arguments&&... args)
	{
		std::function<typename std::result_of<callable(arguments...)>::type()> task(std::bind(std::forward<callable>(f), std::forward<arguments>(args)...));
		std::thread([after, task](){
			std::this_thread::sleep_for(std::chrono::milliseconds(after));
			task();
		}).detach();
	}

private:
	// 声明原子变量
	// 原子操作是个不可分割的操作。 在系统的所有线程中， 不可能观察到原子操作完成了一半的情况：它要么就是做了， 要么就是没做， 只有这两种可能.
	std::atomic<bool> m_expired; //是否失效
	std::atomic<bool> m_try_to_expire;
	std::mutex m_mutex;
	// 条件变量
	std::condition_variable m_cond_expired;
};

