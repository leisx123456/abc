#pragma once
#include "LBasicThink.h"
#include <vector>


//////////////////////////////////////////////////////////////////////////
// 总体思路
class CLCmpThink:public CLBasicThink
{
public:	
	CLCmpThink();

public:
	virtual void thinkHu(CLMjCard aCards[], unsigned int unCardCount
		, T_WeaveCardsItem aWeaveItem[], unsigned int unItemSize, CLMjCard cardDest);

	virtual CLMjCard thinkOutCard(CLMjCard aCards[], unsigned int unCardCount
		, T_WeaveCardsItem aWeaveItem[], unsigned int unItemSize);

	void markMeld();

	void markOneToOne();
	

	void thinkNoSingle();
	

	void markSingle();


	int getMarkNum();
	

	void addScoreBySelf(CLMjCard & handCard);
	

	void addScoreByRelation(CLMjCard & handCard);
	

	void setMark(E_HandCardRelationType relationType);
	

	void setScore(E_HandCardRelationType relationType);
	
	void resetParticipated();
	
	


	void resetParticipatedInActive();

	void thinkSuplseFour();

	
};


