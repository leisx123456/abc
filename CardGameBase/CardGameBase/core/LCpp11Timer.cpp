
#include "LCpp11Timer.h"


LCpp11Timer::LCpp11Timer()
: m_expired(true)
, m_try_to_expire(false)
{
}

LCpp11Timer::LCpp11Timer(const LCpp11Timer& tOther)
{
	/*load()	*/
	//����: Order //�ڴ�˳��������Ϊ memory_order_release �� memory_order_acq_rel��
	//����ֵ:  �� *this�д洢�ļ���ֵ��
	m_expired = tOther.m_expired.load(); // ��ָ�����ڴ�Լ���ڣ������� *this�еĴ洢ֵ����
	m_try_to_expire = tOther.m_try_to_expire.load();
}


LCpp11Timer::~LCpp11Timer()
{
	killTimer();
}

void LCpp11Timer::startTimer(int interval, std::function<void()> task)
{
	// ���û��ʧЧֱ�ӷ���
	if (m_expired == false)
	{
		return;
	}
	m_expired = false;
	// �����߳�
	std::thread([this, interval, task]()
	{
		while (!m_try_to_expire)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(interval));
			task();
		}
		//lock_guard���󲢲��������mutex������������ڣ�lock_guard����ֻ�Ǽ���mutex����������ͽ��������������̶߳Ի���������������ĳ��lock_guard��������������ڣ�
		//����������������һֱ��������״̬����lock_guard���������ڽ���֮�����������������ᱻ����������Ա���Էǳ������ʹ��lock_guard�������õ����쳣��ȫ���⡣
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
