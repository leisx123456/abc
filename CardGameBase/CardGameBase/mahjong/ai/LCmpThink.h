#pragma once
#include "LBasicThink.h"
#include <vector>
#include <map>

// ��ֵȨ�ض���
#define SCORE_Hu 1000            //
#define SCORE_Kong 500            //��Ӱ���Ƶ����
#define SCORE_Triplet 400        //���� ���-���� �̶���-����
#define SCORE_Sequence  300      //˳��
#define SCORE_Pair_Only 120      //����ȫ����ֻ��һ�����
#define SCORE_Pair 100      //��
#define SCORE_Door_TwoHead 100          //���ӣ���ͷ��
#define SCORE_Door_Middle  80       //����
#define SCORE_Door_Edge  70      //��Ե����
#define SCORE_Single_TwoHead 10          //����3-7
#define SCORE_Single_Middle 8           //����28
#define SCORE_Single_Edge 6			//����19
#define SCORE_SingleToSingle_TwoHead 3
#define SCORE_SingleToSingle_Middle 2
#define SCORE_SingleToSingle_Edge 1
#define SCORE_TBA 0                    //ȱ
#define SCORE_Badly 0

//////////////////////////////////////////////////////////////////////////
// ����˼· 
// 1.�ݹ�õ�3����ϣ�˳�ӣ����֣��������������Ʊ�ǣ�����������δ�������ƽ��벽��2
// 2.�ݹ�õ�2����ϣ����ӣ��������������Ʊ�ǣ�����������δ�������ƽ��벽��3
// 3.
class CLCmpThink:public CLBasicThink
{
public:	
	CLCmpThink();

public:
	virtual void thinkHu(CLMjCard aCards[], unsigned int unCardCount
		, T_WeaveCardsItem aWeaveItem[], unsigned int unItemSize, CLMjCard cardDest);

	virtual CLMjCard thinkOutCard(CLMjCard aCards[], unsigned int unCardCount
		, T_WeaveCardsItem aWeaveItem[], unsigned int unItemSize, int nCardColor = -1);

	void markDingQue(int nCardColor = -1);
	void markThree();
	void markTwo();
	void markOne();
	void markOne(CLMjCard & handCard);
	void thinkNoSingle();
	
	void commitScore();
	void setScore(E_HandCardRelationType relationType);
	void addExtraScoreToSingleCard(CLMjCard & handCard);

	int getMarkNum();
	
	void setMark(E_HandCardRelationType relationType);
	


	
	void resetParticipated();
	
	


	void resetParticipatedInActive();

	//void thinkSuplseFour();
private:

	std::map<int, int> m_mapScore;
	
};


