#pragma once
#include "LBasicThink.h"
#include <vector>


//////////////////////////////////////////////////////////////////////////
// 总体思路
class CLCmpThink:public CLBasicThink
{
public:	
	CLCmpThink();

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
	
	int findPosInActiveHandCards(CLMjCard handCard);


	void resetParticipatedInActive();



	
};


