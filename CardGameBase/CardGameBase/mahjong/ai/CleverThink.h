#pragma once
#include "CmpThink.h"



class CCleverThink : public CBasicThink
{
public:
	CCleverThink();
	void ThinkClever();					
	void CleverThink();										//�������㷨
	int GetMaxScoreClever();								//��ȡ������
	void SetPaiOut(int* pOutPai,int nNum);
	virtual ~CCleverThink();

protected:
	bool IsSameTeamSeven(int pai,int &pos1,int nStart);
	void ThinkSeven();										//�����߶�
	void ThinkSevenOne();									//�����߶��жϺ�ʣ�µ���
	int FindPosOut(int pai);								//�ڳ����в�����
	int AddGoodOut(int pai);								//����Ƶ����Ʒ�

protected:
	int m_nWjOutPai[80];									//��ҳ��˵���
	int m_nWjOutPaiNum;
	int m_nSevenScore;										//�����߶Ժ�ķ���
	int m_nMaxScoreTotal;
	int m_nScoreSeven;
	int m_nMaxScoreSeven;
	int m_nRemainNumSeven;
	int m_nRemainSeven[14];

};


