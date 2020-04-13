// std_set_shared_ptr_test.cpp : 定义控制台应用程序的入口点。
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

	////set容器中存放类对象智能指针时,自定义比较函数方式1:friend方式重载<
	//friend	bool operator <(const shared_ptr< song>& left, const shared_ptr< song>&  right)
	//{
	//	//return false;
	//	if (left->m_id == right->m_id)     //根据id去重
	//		return false;
	//	else
	//	{
	//		if (left->m_hot != right->m_hot)
	//		{
	//			return left->m_hot < right->m_hot;      //降序
	//		}
	//		else
	//		{
	//			return left->m_id < right->m_id;
	//		}

	//	}
	//}
};




//set容器中存放类对象智能指针时,自定义比较函数方式2:指定set_share_ptr_compptr为函数对象(必须申明为const引用)
struct set_share_ptr_compptr
{
	bool operator()(const shared_ptr <song>& left, const shared_ptr<song>& right)  //重载（）运算符
	{
		if (left->m_id == right->m_id)     //根据id去重
			return false;
		else
		{
			if (left->m_hot != right->m_hot)
			{
				return left->m_hot > right->m_hot;      //降序
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
	bool operator()(const song* left, const song* right)  //重载（）运算符
	{
		if (left->m_id == right->m_id)     //根据id去重
			return false;

			if (left->m_hot != right->m_hot)
			{
				return left->m_hot > right->m_hot;      //降序
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
	// 存储类对象智能指针,使用类成员函数作为比较函数:
	std::cout << "set----" << endl;
	std::set<song*, set_ptr_compptr> mySet;      //使用重载的<
	auto s1 = new song(10, 100);
	auto s2 = new song(20, 200);
	auto s3 = new song(20, 300);//重复的插不进
	auto s4 = new song(30, 300);
	auto s5 = new song(40, 200);
	auto s6 = new song(40, 200);//重复的插不进
	auto s7 = new song(50, 400);//重复的插不进
	{
		auto s7 = new song(2, 2);
		// s7生命结束
	}

	mySet.insert(s1);    //插入s1
	mySet.insert(s2);    //插入s2
	mySet.insert(s3);    //s3和s2的id相同，不插入
	mySet.insert(s4);    //插入s4
	mySet.insert(s5);    //插入s5,看和s4是如何排序的
	mySet.insert(s6);    //看是否插的进
	mySet.insert(s7);    //看是否插的进

	for (auto& it : mySet)
	{
		std::cout << "id:" << it->m_id << ",hot:" << it->m_hot << endl;
	}
}



//set存储智能指针
void test2()
{
	// 存储类对象智能指针,使用类成员函数作为比较函数:
	std::cout << "set----" << endl;
	std::set<shared_ptr< song>> mySet;      //使用重载的<
	auto s1 = std::make_shared<song>(10, 100);
	auto s2 = std::make_shared<song>(20, 200);
	auto s3 = std::make_shared<song>(20, 300);//重复的插不进
	auto s4 = std::make_shared<song>(30, 300);
	auto s5 = std::make_shared<song>(40, 200);
	auto s6 = std::make_shared<song>(40, 200);//重复的插不进
	auto s7 = std::make_shared<song>(50, 400);//重复的插不进
	{
		auto s7 = std::make_shared<song>(2, 2);
		// s7生命结束
	}

	mySet.insert(s1);    //插入s1
	mySet.insert(s2);    //插入s2
	mySet.insert(s3);    //s3和s2的id相同，不插入
	mySet.insert(s4);    //插入s4
	mySet.insert(s5);    //插入s5,看和s4是如何排序的
	mySet.insert(s6);    //看是否插的进
	mySet.insert(s7);    //看是否插的进

	//输出一次
	print_set(mySet);

	//修改一个值,再看输出
	for (auto &info : mySet)
	{
		if (info->m_id == 10)
		{
			info->m_hot = 1000;
			break;
		}
	}
	cout << "修改后输出" << endl;

	std::set<shared_ptr< song>> mySet1;      //使用重载的<
	mySet.swap(mySet1);
	//输出一次
	print_set(mySet);
	print_set(mySet1);

	cout << "修改后再插入新值:" << endl;
	mySet1.insert(std::make_shared<song>(60, 600));

	print_set(mySet1);

	/*
	修改容器对象成员变量值,不会引起重新排序,即使插入新值也只是新值进行排序,被修改的值不会排序
	*/


	cout << "set存储类对象智能指针,使用函数对象作为比较函数:" << endl;
	std::set<shared_ptr< song>, set_share_ptr_compptr> mySet2;    //使用外部定义的的()
	for (auto&val : mySet1)
	{
		mySet2.emplace(val);
	}
}

//测试遍历一个set删除另外一个set中的元素
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

	//删除第一个
	set2.erase(*(set1.begin())); //用元素关键值删除
	set2.erase(set2.begin());//用迭代器删除

	//遍历set2,删除set1中的元素
	for (auto it = set2.begin(); it != set2.end();)
	{
		set1.erase(*it);
		if ((*it)->m_id != 9)
		{
			set2.erase(it++);//迭代器先加再删除
			//it = set2.erase(it);//等价于上面那种写法
		}
		else
		{
			it++;
		}
	}

	//因为自定义了比较函数,比较函数中当m_id相同时就可以判断是同一个元素,所以可以通过构建临时对象来删除;
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

