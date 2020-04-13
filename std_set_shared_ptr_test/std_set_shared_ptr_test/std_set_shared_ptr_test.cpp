// std_set_shared_ptr_test.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <iostream>
#include <set>
#include <memory>
#include <functional>
#include <unordered_set>
using namespace std;


struct song
{
	int m_id;
	int m_hot;
	song(int id, int hot)
	{

		this->m_id = id;
		this->m_hot = hot;
	}

	~song() 
	{
		cout << "~song:m_id=" << m_id << ",m_hot=" << m_hot << endl;
	}

	////set�����д�����������ָ��ʱ,�Զ���ȽϺ�����ʽ1:friend��ʽ����<
	//friend	bool operator <(const shared_ptr< song>& left, const shared_ptr< song>&  right)
	//{
	//	//return false;
	//	if (left->m_id == right->m_id)     //����idȥ��
	//		return false;
	//	else
	//	{
	//		if (left->m_hot != right->m_hot)
	//		{
	//			return left->m_hot < right->m_hot;      //����
	//		}
	//		else
	//		{
	//			return left->m_id < right->m_id;
	//		}

	//	}
	//}
};




//set�����д�����������ָ��ʱ,�Զ���ȽϺ�����ʽ2:ָ��set_share_ptr_compptrΪ��������(��������Ϊconst����)
struct set_share_ptr_compptr
{
	bool operator()(const shared_ptr <song>& left, const shared_ptr<song>& right)  //���أ��������
	{
		if (left->m_id == right->m_id)     //����idȥ��
			return false;
		else
		{
			if (left->m_hot != right->m_hot)
			{
				return left->m_hot > right->m_hot;      //����
			}
			else
			{
				return left->m_id > right->m_id;
			}

		}
	}

};

struct set_ptr_compptr
{
	bool operator()(const song* left, const song* right)  //���أ��������
	{
		if (left->m_id == right->m_id)     //����idȥ��
			return false;

			if (left->m_hot != right->m_hot)
			{
				return left->m_hot > right->m_hot;      //����
			}
			else
			{
				return left->m_id > right->m_id;
			}


	}

};

void print_set(std::set<shared_ptr< song>>& prSet)
{
	for (auto& it : prSet)
	{
		std::cout << "id:" << it->m_id << ",hot:" << it->m_hot << endl;
	}
}

void test1()
{
	// �洢���������ָ��,ʹ�����Ա������Ϊ�ȽϺ���:
	std::cout << "set----" << endl;
	std::set<song*, set_ptr_compptr> mySet;      //ʹ�����ص�<
	auto s1 = new song(10, 100);
	auto s2 = new song(20, 200);
	auto s3 = new song(20, 300);//�ظ��Ĳ岻��
	auto s4 = new song(30, 300);
	auto s5 = new song(40, 200);
	auto s6 = new song(40, 200);//�ظ��Ĳ岻��
	auto s7 = new song(50, 400);//�ظ��Ĳ岻��
	{
		auto s7 = new song(2, 2);
		// s7��������
	}

	mySet.insert(s1);    //����s1
	mySet.insert(s2);    //����s2
	mySet.insert(s3);    //s3��s2��id��ͬ��������
	mySet.insert(s4);    //����s4
	mySet.insert(s5);    //����s5,����s4����������
	mySet.insert(s6);    //���Ƿ��Ľ�
	mySet.insert(s7);    //���Ƿ��Ľ�

	for (auto& it : mySet)
	{
		std::cout << "id:" << it->m_id << ",hot:" << it->m_hot << endl;
	}
}



//set�洢����ָ��
void test2()
{
	// �洢���������ָ��,ʹ�����Ա������Ϊ�ȽϺ���:
	std::cout << "set----" << endl;
	std::set<shared_ptr< song>> mySet;      //ʹ�����ص�<
	auto s1 = std::make_shared<song>(10, 100);
	auto s2 = std::make_shared<song>(20, 200);
	auto s3 = std::make_shared<song>(20, 300);//�ظ��Ĳ岻��
	auto s4 = std::make_shared<song>(30, 300);
	auto s5 = std::make_shared<song>(40, 200);
	auto s6 = std::make_shared<song>(40, 200);//�ظ��Ĳ岻��
	auto s7 = std::make_shared<song>(50, 400);//�ظ��Ĳ岻��
	{
		auto s7 = std::make_shared<song>(2, 2);
		// s7��������
	}

	mySet.insert(s1);    //����s1
	mySet.insert(s2);    //����s2
	mySet.insert(s3);    //s3��s2��id��ͬ��������
	mySet.insert(s4);    //����s4
	mySet.insert(s5);    //����s5,����s4����������
	mySet.insert(s6);    //���Ƿ��Ľ�
	mySet.insert(s7);    //���Ƿ��Ľ�

	//���һ��
	print_set(mySet);

	//�޸�һ��ֵ,�ٿ����
	for (auto &info : mySet)
	{
		if (info->m_id == 10)
		{
			info->m_hot = 1000;
			break;
		}
	}
	cout << "�޸ĺ����" << endl;

	std::set<shared_ptr< song>> mySet1;      //ʹ�����ص�<
	mySet.swap(mySet1);
	//���һ��
	print_set(mySet);
	print_set(mySet1);

	cout << "�޸ĺ��ٲ�����ֵ:" << endl;
	mySet1.insert(std::make_shared<song>(60, 600));

	print_set(mySet1);

	/*
	�޸����������Ա����ֵ,����������������,��ʹ������ֵҲֻ����ֵ��������,���޸ĵ�ֵ��������
	*/


	cout << "set�洢���������ָ��,ʹ�ú���������Ϊ�ȽϺ���:" << endl;
	std::set<shared_ptr< song>, set_share_ptr_compptr> mySet2;    //ʹ���ⲿ����ĵ�()
	for (auto&val : mySet1)
	{
		mySet2.emplace(val);
	}
}

//���Ա���һ��setɾ������һ��set�е�Ԫ��
void test3()
{
	set < shared_ptr<song>> set1, set2;

	for (int i = 0; i < 10; i++)
	{
		set1.insert(make_shared<song>(i, i * 10));
	}
	for (auto& str : set1)
	{
		set2.insert(str);
	}

	//ɾ����һ��
	set2.erase(*(set1.begin())); //��Ԫ�عؼ�ֵɾ��
	set2.erase(set2.begin());//�õ�����ɾ��

	//����set2,ɾ��set1�е�Ԫ��
	for (auto it = set2.begin(); it != set2.end();)
	{
		set1.erase(*it);
		if ((*it)->m_id != 9)
		{
			set2.erase(it++);//�������ȼ���ɾ��
			//it = set2.erase(it);//�ȼ�����������д��
		}
		else
		{
			it++;
		}
	}

	//��Ϊ�Զ����˱ȽϺ���,�ȽϺ����е�m_id��ͬʱ�Ϳ����ж���ͬһ��Ԫ��,���Կ���ͨ��������ʱ������ɾ��;
	set2.erase(make_shared<song>(9, 90));
}




int _tmain(int argc, _TCHAR* argv[])
{
	test1();
	//test2();
	//test3();
	getchar();
	return 0;
}

