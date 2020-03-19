#pragma once

#include<functional>
#include<chrono>
#include<thread>
#include<atomic>
#include<memory>
#include<mutex>
#include<condition_variable>

/*���÷�ʽ

int _tmain(int argc, _TCHAR* argv[])
{
LCpp11Timer t;

t.startTimer(1000, std::bind(EchoFunc, "hello world!")); // �󶨲���
std::this_thread::sleep_for(std::chrono::seconds(4));
std::cout << "try to expire timer!" << std::endl;
t.killTimer();

//������ִ�ж�ʱ����
t.startTimer(1000, std::bind(EchoFunc, "hello c++11!"));
std::this_thread::sleep_for(std::chrono::seconds(4));
std::cout << "try to expire timer!" << std::endl;
t.killTimer();

std::this_thread::sleep_for(std::chrono::seconds(2));

//ִֻ��һ�ζ�ʱ����
//ͬ��
t.syncWait(1000, EchoFunc, "hello world!");
//�첽
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
����Ѷȣ���
*/
/************************************************************************/
class LCpp11Timer
{
public:
	LCpp11Timer();
	LCpp11Timer(const LCpp11Timer& t);
	~LCpp11Timer();

	//������ִ�ж�ʱ����
	void startTimer(int interval, std::function<void()> task);

	void killTimer();	// expire

	// �ȴ�һ��ʱ���ͬ��ִ��һ��
	template<typename callable, class... arguments>
	void syncWait(int after, callable&& f, arguments&&... args)
	{
		std::function<typename std::result_of<callable(arguments...)>::type()> task(std::bind(std::forward<callable>(f), std::forward<arguments>(args)...));
		std::this_thread::sleep_for(std::chrono::milliseconds(after));
		task();
	}

	// �ȴ�һ��ʱ����첽ִ��һ��
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
	// ����ԭ�ӱ���
	// ԭ�Ӳ����Ǹ����ɷָ�Ĳ����� ��ϵͳ�������߳��У� �����ܹ۲쵽ԭ�Ӳ��������һ����������Ҫô�������ˣ� Ҫô����û���� ֻ�������ֿ���.
	std::atomic<bool> m_expired; //�Ƿ�ʧЧ
	std::atomic<bool> m_try_to_expire;
	std::mutex m_mutex;
	// ��������
	std::condition_variable m_cond_expired;
};

