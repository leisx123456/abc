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
	CLCmpThink* clone() const { return new CLCmpThink(*this); }

	// ˼����ȱ������ȱ�Ļ�ɫ
	virtual int thinkDingQue(CLMjCard aCards[], unsigned int unCardCount);

	virtual bool thinkHu(CLMjCard aCards[], unsigned int unCardCount
		, T_WeaveCardsItem aWeaveItem[], unsigned int unItemSize, CLMjCard cardDest, int nQueColor = -1);

	// �������ʾҪ��
	virtual bool thinkKong(CLMjCard aCards[], unsigned int unCardCount
		, T_WeaveCardsItem aWeaveItem[], unsigned int unItemSize, CLMjCard cardDest, int nQueColor = -1);

	virtual bool thinkPong(CLMjCard aCards[], unsigned int unCardCount
		, T_WeaveCardsItem aWeaveItem[], unsigned int unItemSize, CLMjCard cardDest, int nQueColor = -1);

	virtual CLMjCard thinkOutCard(CLMjCard aCards[], unsigned int unCardCount
		, T_WeaveCardsItem aWeaveItem[], unsigned int unItemSize, int nQueColor = -1);

	void think();

protected:
	int isExistQue(int nCardColor = -1);
	void markThree();
	void markTwo();
	void markOne();
	void markOne(CLMjCard & handCard);
	void thinkNoSingle();
	
	void addExtraScoreToSingleCard(CLMjCard & handCard);
	void commitScore();
	void saveBadlyCardByScore();
	int totalScore();

	int getMarkNum();
	void unLockAll();


private:
	
	std::map<int, int> m_mapScore;
	
};


