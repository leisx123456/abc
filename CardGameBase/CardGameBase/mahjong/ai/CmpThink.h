#pragma once
#include "BasicThink.h"


class CCmpThink:public CBasicThink
{
public:	
	CCmpThink();											//���캯��
	void CheckOnlyTwo();									//�����޳����ź������ӻ���ӵ����
	void Think();											//˼���ó�����ֵ

protected:
	int AddGood(int pai);
	void ThinkThree();										//�������ŵ����(�����е�����޳�)
	void ThinkTwo();										//(����������޳���)����2�ŵ����
	void ThinkOne();										//(����������޳���)����1���Ƶ������
};

