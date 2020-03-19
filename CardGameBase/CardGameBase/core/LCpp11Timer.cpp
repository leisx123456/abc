
#include "LCpp11Timer.h"


LCpp11Timer::LCpp11Timer()
: m_expired(true)
, m_try_to_expire(false)
{
}

LCpp11Timer::LCpp11Timer(const LCpp11Timer& tOther)
{
	/*load()	*/
	//参数: Order //内存顺序。排序不能为 memory_order_release 或 memory_order_acq_rel。
	//返回值:  在 *this中存储的检索值。
	m_expired = tOther.m_expired.load(); // 在指定的内存约束内，检索在 *this中的存储值，。
	m_try_to_expire = tOther.m_try_to_expire.load();
}


LCpp11Timer::~LCpp11Timer()
{
	killTimer();
}

void LCpp11Timer::startTimer(int interval, std::function<void()> task)
{
	// 如果没有失效直接返回
	if (m_expired == false)
	{
		return;
	}
	m_expired = false;
	// 启动线程
	std::thread([this, interval, task]()
	{
		while (!m_try_to_expire)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(interval));
			task();
		}
		//lock_guard对象并不负责管理mutex对象的生命周期，lock_guard对象只是简化了mutex对象的上锁和解锁操作，方便线程对互斥量上锁，即在某个lock_guard对象的生命周期内，
		//它所管理的锁对象会一直保持上锁状态；而lock_guard的生命周期结束之后，它所管理的锁对象会被解锁。程序员可以非常方便地使用lock_guard，而不用担心异常安全问题。
		std::lock_guard<std::mutex> locker(m_mutex);
		m_expired = true;
		m_cond_expired.notify_one();
	}).detach();
}

void LCpp11Timer::killTimer()
{
	if (m_expired)
	{
		return;
	}
	if (m_try_to_expire)
	{
		return;
	}
	m_try_to_expire = true;
	{
		std::unique_lock<std::mutex> locker(m_mutex);
		m_cond_expired.wait(locker, [this] {
			return m_expired == true;
		});

		if (m_expired == true)
		{
			m_try_to_expire = false;
		}
	}
}
