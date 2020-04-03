#ifndef _L_MJ_LOGIC_H_
#define _L_MJ_LOGIC_H_

#include <iostream>
#include "../core/LMjCard.h"
#include "../core/LCardOperate.h"

//////////////////////////////////////////////////////////////////////////
// �齫�߼�������
class CLMjLogic : public CLCardOperate<CLMjCard>
{
public:
	CLMjLogic();
	//bool isExistPair(CLMjCard aCards[]);

	// //ת��
	int switchToCardData(const unsigned int arrCardIndex[MJ_MAX_INDEX], CLMjCard aCards[MJ_MAX_HAND_COUNT]);
	int switchToCardIndex(CLMjCard aCards[], unsigned int unCardCount, unsigned int arrCardIndex[MJ_MAX_INDEX]);

	// �Ƿ�����
	bool isCanPong(CLMjCard aCards[], unsigned int unCardCount, const CLMjCard & cardOut);

	// �Ƿ��ܸ�
	bool isCanKong(CLMjCard aCards[], unsigned int unCardCount, T_WeaveCardsItem aWeaveItem[], unsigned int unItemSize, CLMjCard cardOut, T_MjActKongInfo & tMjActKongInfo);
	bool isCanDianKong(CLMjCard aCards[], unsigned int unCardCount, T_WeaveCardsItem aWeaveItem[], unsigned int unItemSize,
		CLMjCard cardOut);
	bool isCanAnKong(CLMjCard aCards[], unsigned int unCardCount, CLMjCard & cardResult);
	bool isCanBuKong(CLMjCard aCards[], unsigned int unCardCount, T_WeaveCardsItem aWeaveItem[], unsigned int unItemSize, CLMjCard & cardResult);

	// ����һ�����(������ʼλ�õ���)
	bool removeTriplet(CLMjCard aCards[], unsigned int unCardCount, int nBeginPos = 0);

	// ����һ��˳��
	bool removeSequence(CLMjCard aCards[], unsigned int unCardCount, int nBeginPos = 0);

	// ���������������
	int getMagicNum(CLMjCard aCards[], unsigned int unCardCount);
	// ���ƺ�����������Ļ�ɫ����
	int getColorCount(CLMjCard aCards[], unsigned int unCardCount, T_WeaveCardsItem aWeaveItem[], unsigned int unItemSize);

	//�Ƿ���
	bool isFlowerPig(CLMjCard aCards[], unsigned int unCardCount, T_WeaveCardsItem aWeaveItem[], unsigned int unItemSize, int colorQue = -1);

	bool isCanHu_3x2(CLMjCard aCards[], unsigned int unCardCount, bool bMust258Pair = false);
	bool isCanHu_7Pair(CLMjCard aCards[], unsigned int unCardCount, bool bMust258Pair = false);

	// ���ڿɺ��������ĺ�������
	bool isPongPong(CLMjCard aCards[], unsigned int unCardCount, T_WeaveCardsItem aWeaveItem[], unsigned int unItemSize);
	bool isQingYiSe(CLMjCard aCards[], unsigned int unCardCount, T_WeaveCardsItem aWeaveItem[], unsigned int unItemSize);
	bool isDragonSevenPair(CLMjCard aCards[], unsigned int unCardCount, bool bMust258Pair = false);
	int genNum(CLMjCard aCards[], unsigned int unCardCount, T_WeaveCardsItem aWeaveItem[], unsigned int unItemSize);

	/* �����ж�, ����ֻ�ж��������3x2,û�п���7�Եȣ� ���������������ط�����������ʵ��*/
	virtual bool isCanHu(CLMjCard aCards[]
		, unsigned int unCardCount
		, T_WeaveCardsItem aWeaveItem[]
		, unsigned int unItemSize
		, T_MjActHuInfo & tMjActHuInfo
		, const CLMjCard & cardOut = CARD_EMPTY
		, bool bMust258Pair = false);
};



#endif // _L_MJ_LOGIC_H_
