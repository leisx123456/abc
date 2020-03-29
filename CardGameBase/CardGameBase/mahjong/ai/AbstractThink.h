#pragma once
#include "../../core/LMjCard.h"



class IAbstractThink
{
public:
	IAbstractThink();

	//////////////////////////////////////////////////////////////////////////
	//�ӿڶ���
	virtual ~IAbstractThink();

	// ˼����ȱ������ȱ�Ļ�ɫ
	virtual int thinkDingQue(CLMjCard aCards[], unsigned int unCardCount) = 0;
	virtual void think(T_ActRequest* pActRequest, CLMjCard aCards[], unsigned int unCardCount, T_WeaveCardsItem aWeaveItem[]
		, unsigned int unItemSize, CLMjCard cardOut, int nQueColor = -1, unsigned short usIgnoreFlags = 0) = 0;

	//virtual bool thinkHu(CLMjCard aCards[], unsigned int unCardCount
	//	, T_WeaveCardsItem aWeaveItem[], unsigned int unItemSize, CLMjCard cardDest, int nQueColor = -1) = 0;

	//// �������ʾҪ��
	//virtual bool thinkKong(CLMjCard aCards[], unsigned int unCardCount
	//	, T_WeaveCardsItem aWeaveItem[], unsigned int unItemSize, CLMjCard cardDest, int nQueColor = -1) = 0;

	//virtual bool thinkPong(CLMjCard aCards[], unsigned int unCardCount
	//	, T_WeaveCardsItem aWeaveItem[], unsigned int unItemSize, CLMjCard cardDest, int nQueColor = -1) = 0;

	//virtual bool thinkChi(CLMjCard aCards[], unsigned int unCardCount
	//	, T_WeaveCardsItem aWeaveItem[], unsigned int unItemSize, CLMjCard cardDest, int nQueColor = -1) {
	//	return false;
	//}

	//virtual CLMjCard thinkOutCard(CLMjCard aCards[], unsigned int unCardCount
	//	, T_WeaveCardsItem aWeaveItem[], unsigned int unItemSize, int nCardColor = -1) = 0;
};
