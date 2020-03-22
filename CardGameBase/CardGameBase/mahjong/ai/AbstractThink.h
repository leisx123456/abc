#pragma once
#include "../../core/LMjCard.h"

// ��ֵȨ�ض���
#define SCORE_Hu 1000            //
#define SCORE_Kong 500            //��Ӱ���Ƶ����
#define SCORE_Triplet 400        //���� ���-���� �̶���-����
#define SCORE_Sequence  300      //˳��
#define SCORE_Pair_Only 120      //����ȫ����ֻ��һ�����
#define SCORE_Door_TwoHead 100          //���ӣ���ͷ��
#define SCORE_Door_Middle  80       //����
#define SCORE_Door_Edge  70      //��Ե����
#define SCORE_Single_34567 9          //����2-8
#define SCORE_Single_28 6           //����1,9
#define SCORE_Single_19 3			//����19
#define SCORE_TBA 0                    //ȱ


enum E_HandCardRelationType
{
	EHC_Group,      //һ��˳�ӡ����ӻ����
	EHC_OneToOne,     //��һ���γ���
	EHC_Single        //���ƣ�����Χ���κ�2�Ŷ������γ���
};


//�Ƿ����γ�˳��
enum E_SequenceType
{
	ES_MiddleCard,                   //����-����-Ƕ��
	ES_LeftEdgeCard,                  //����-Ŀ����������
	ES_RightEdgeCard                  //����-Ŀ���������ҽϴ�
};

//�Ƿ����γ�����(����)
enum E_DoorType
{
	ED_DoorTwoHead,                //��ͷ�� 23
	ED_DoorMiddle,                 //Ƕ�� 02,35,68
	ED_DoorEdge                    //���� 01,78
};


class IAbstractThink
{
public:
	IAbstractThink();

	//////////////////////////////////////////////////////////////////////////
	//�ӿڶ���
	virtual ~IAbstractThink();

	virtual int thinkDingQue(CLMjCard aCards[], unsigned int unCardCount, T_WeaveCardsItem aWeaveItem[], unsigned int unItemSize) = 0;

	virtual void thinkHu(CLMjCard aCards[], unsigned int unCardCount
		, T_WeaveCardsItem aWeaveItem[], unsigned int unItemSize, CLMjCard cardDest) = 0;

	virtual CLMjCard thinkOutCard(CLMjCard aCards[], unsigned int unCardCount
		, T_WeaveCardsItem aWeaveItem[], unsigned int unItemSize) = 0;
};
