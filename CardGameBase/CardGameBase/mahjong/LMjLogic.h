#ifndef _L_MJ_LOGIC_H_
#define _L_MJ_LOGIC_H_

#include <iostream>
#include "../core/LMjCard.h"
#include "../core/LCardOperate.h"
	
class CLMjLogic : public CLCardOperate<CLMjCard>
{
public:
	CLMjLogic();

	//bool isExistPair(CLMjCard aCards[]);

	// �Ƿ�����
	bool isCanPong(CLMjCard aCards[], unsigned int unCardCount, const CLMjCard & cardDest);

	// �Ƿ��ܸ�
	bool isCanKong(CLMjCard aCards[], unsigned int unCardCount, T_WeaveCardsItem aWeaveItem[], unsigned int unItemSize,
		CLMjCard cardDest);

	// ����һ�����(������ʼλ�õ���)
	bool removeTriplet(CLMjCard aCards[], unsigned int unCardCount, int nBeginPos = 0);

	// ����һ��˳��
	bool removeSequence(CLMjCard aCards[], unsigned int unCardCount, int nBeginPos = 0);

	// ���������������
	int getMagicNum(CLMjCard aCards[], unsigned int unCardCount);


	/* �����ж� */
	bool isCanHu(CLMjCard aCards[]
		, unsigned int unCardCount
		, T_WeaveCardsItem aWeaveItem[]
		, unsigned int unItemSize
		, const CLMjCard & cardOtherOut = CARD_EMPTY
		, const CLMjCard & cardGet = CARD_EMPTY
		, bool bMust258Pair = false);

	bool isCanHu_3x2(CLMjCard aCards[], unsigned int unCardCount, bool bMust258Pair = false);
	bool isCanHu_7Pair(CLMjCard aCards[], unsigned int unCardCount, bool bMust258Pair = false);


};



#endif // _L_MJ_LOGIC_H_
