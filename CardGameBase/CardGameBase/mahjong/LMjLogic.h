#ifndef _L_MJ_LOGIC_H_
#define _L_MJ_LOGIC_H_

#include <iostream>
#include "../core/LMjCard.h"
#include "../core/LCardOperate.h"
	







class CLMjLogic : public CLCardOperate<CLMjCard>
{
public:
	CLMjLogic();
	virtual bool isValidCard(const CLMjCard & card);

	//virtual void initMjCards() = 0;	// ÿ���齫���淨���齫��Ŀ��һ������Ҫ�Լ�ʵ��

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

private:
	unsigned char* m_pByMjCards;	// �齫��
	int m_nTotalCards;				// �齫������
	unsigned int m_nMaxHandCards;	// ���������
	int m_nMagicNum;				// �����
};



#endif // _L_MJ_LOGIC_H_
