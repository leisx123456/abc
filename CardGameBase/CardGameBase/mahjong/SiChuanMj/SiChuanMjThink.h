#pragma once
#include "../ai/LScoreBasicThink.h"
#include <vector>
#include <map>



//////////////////////////////////////////////////////////////////////////
// ����˼· 
// 1.�����������¾�������
// 2.����˼���ܲ�����һɫ�����������һ��ɫ���ڵ���10����������2�Ҷ�ȱ�˻�ɫ��������һɫ������
// 3.�������������5��ʱ����С��
// 4.������������������ܸܵ�����¾����ܣ������һ����˯��ʱ�Ͱ�����˼·���������Ʒ����ϴ�
// 20200407 �Ƚ�Ҫ��Ҫ���� ���Ǿ����Ƚ����Ƽ�����Ƶķ���֮�� �Ƿ��������֮ǰ�ķ���֮��
class CSiChuanMjThink : public CLScoreBasicThink
{
public:	
	CSiChuanMjThink();

public:
	CSiChuanMjThink* clone() const { return new CSiChuanMjThink(*this); }

	// ˼����ȱ������ȱ�Ļ�ɫ
	virtual int thinkDingQue(CLMjCard aCards[], unsigned int unCardCount);

	
	virtual void think(T_MjActInfo* pMjActInfo, CLMjCard aCards[], unsigned int unCardCount, T_WeaveCardsItem aWeaveItem[]
		, unsigned int unItemSize, CLMjCard cardOut, int nQueColor = -1, unsigned short usIgnoreFlags = 0);

	void think();
private:
	bool thinkHu(T_MjActHuInfo & tMjActHuInfo);

	// �������ʾҪ��
	bool thinkKong(T_MjActKongInfo & tMjActKongInfo);
	int thinkAfterKong(CLMjCard cardGangDest, E_KongType eKongType);

	bool thinkPong();
	int thinkAfterPong();

	CLMjCard thinkOutCard();


private:
	
	std::map<int, int> m_mapScore;
	int m_nTotalScore;
	
};


