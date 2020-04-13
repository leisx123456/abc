// std_set_ptr_test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <set>
#include <iostream>

#define BMTS_SEQ_IS_LESS(seq1,seq2)\
	((unsigned short)((unsigned short)(seq1)-(unsigned short)(seq2)) > (unsigned short)(1 << 15))

class ProtocolOperat{
public:
	ProtocolOperat(void*data){
		p = data;
	}
	int GetDataSeq(){
		return *(unsigned short*)p;
	}
	void*p;
};

//
struct node{

	void* mpData;
};
typedef node* NodePtr;

// 在对象外部定义仿函数
struct NodeSortCriterion{
	// operator() 实现对象当函数使用
	bool operator() (const NodePtr& a, const NodePtr& b) const
	{
		ProtocolOperat p1(a->mpData);
		ProtocolOperat p2(b->mpData);
		unsigned short r1 = p1.GetDataSeq();
		unsigned short r2 = p2.GetDataSeq();
		bool ret = BMTS_SEQ_IS_LESS(r1, r2);
		return ret;
	}
};

typedef std::set<NodePtr, NodeSortCriterion> testSet;
using namespace std;


int _tmain(int argc, _TCHAR* argv[])
{
	unsigned short i1 = 1;
	unsigned short i2 = 0xFFFF;
	unsigned short i3 = 1;
	NodePtr p1 = new node;
	NodePtr p2 = new node;
	NodePtr p3 = new node;
	p1->mpData = &i1;
	p2->mpData = &i2;
	p3->mpData = &i3;
	testSet intset;
	pair<testSet::iterator, bool> ret = intset.insert(p2);
	if (ret.second == false){
		printf("false1\n");
	}
	ret = intset.insert(p1);
	if (ret.second == false){
		printf("false2\n");
	}
	ret = intset.insert(p3);
	if (ret.second == false){
		printf("false3\n");
	}
	getchar();
	return 0;

}

