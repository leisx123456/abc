#include "LMjCard.h"
#include <iostream>
#include <assert.h>

// 一副麻将
const CLMjCard g_arrMjCardPair[] =
{
	0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09,						//万子
	0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09,						//万子
	0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09,						//万子
	0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09,						//万子
	0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19,						//索子
	0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19,						//索子
	0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19,						//索子
	0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19,						//索子
	0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29,						//同子
	0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29,						//同子
	0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29,						//同子
	0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29,						//同子
	0x31, 0x31, 0x31, 0x31,														// 东风
	0x32, 0x32, 0x32, 0x32,														// 南风
	0x33, 0x33, 0x33, 0x33,														// 西风
	0x34, 0x34, 0x34, 0x34,														// 北风
	0x35, 0x35, 0x35, 0x35,														// 中
	0x36, 0x36, 0x36, 0x36,														// 发
	0x37, 0x37, 0x37, 0x37,														// 白
	0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48,								// 春夏秋冬梅兰竹菊
	0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48,								// 春夏秋冬梅兰竹菊
	0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48,								// 春夏秋冬梅兰竹菊
	0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48								// 春夏秋冬梅兰竹菊
};



CLMjCard::CLMjCard()
: m_eCardResType(EC_Hand)
, m_eViewDirection(EV_South)
, m_eViewPosture(EV_Stand)
, CLCard(CARD_EMPTY)
{
	initCard();
}

CLMjCard::CLMjCard(int nCard)
: m_eCardResType(EC_Hand)
, m_eViewDirection(EV_South)
, m_eViewPosture(EV_Stand)
, CLCard(nCard)
{
	initCard();
}

CLMjCard::CLMjCard(E_CardResType eCardResType, E_ViewDirection eViewDirection, int nCard)
: m_eCardResType(eCardResType)
, m_eViewDirection(eViewDirection)
, m_eViewPosture(EV_Stand)
, CLCard(nCard)
{
	initCard();
}



bool CLMjCard::isValid() const
{
	unsigned char cbValue = (m_nValue & MASK_VALUE);
	unsigned char cbColor = (m_nValue & MASK_COLOR) >> 4;
	return (((cbValue >= 1) && (cbValue <= 9) && (cbColor <= 2)) 
		|| ((cbValue >= 1) && (cbValue <= 7) && (cbColor == 3))
		|| ((cbValue >= 1) && (cbValue <= 8) && (cbColor == 4)));

}

int CLMjCard::switchToCardIndex()
{
	/*return logicValue()*/
	assert(isValid());
	return color() * 9 + logicValue() - 1;
}

CLMjCard CLMjCard::switchToCardValue(int nCardIndex)
{
	assert(nCardIndex < MJ_MAX_INDEX);
	return CLMjCard(((nCardIndex / 9) << 4) | (nCardIndex % 9 + 1));
}

void CLMjCard::initCard()
{
	m_bLocked = false;
	m_nRelation = EHC_Relation_Invaild;
}


void CLMjCard::empty()
{
	CLCard::empty();
	m_bLocked = false;
	m_nRelation = EHC_Relation_Invaild;
}

bool CLMjCard::isTBA(E_MjCardColor eMjCardColor)
{
	return color() == (int)eMjCardColor;
}

CLMjCard & CLMjCard::operator=(const CLMjCard & rhs)
{
	m_eCardResType = rhs.m_eCardResType;
	m_eViewPosture = rhs.m_eViewPosture;
	m_eViewDirection = rhs.m_eViewDirection;

	m_nValue = rhs.m_nValue;
	m_nScore = rhs.m_nScore;
	m_bLocked = rhs.m_bLocked;
	m_nRelation = rhs.m_nRelation;
	return *this;
}

bool CLMjCard::isRelation3()
{
	return m_nRelation == EHC_Sequence || m_nRelation == EHC_Triplet;
}

bool CLMjCard::isRelation2()
{
	return m_nRelation == EHC_Pair 
		|| m_nRelation == EHC_Door_TwoHead
		|| m_nRelation == EHC_Door_Middle
		|| m_nRelation == EHC_Door_Edge;
}

bool CLMjCard::isRelation1()
{
	return m_nRelation == EHC_Single_34567
		|| m_nRelation == EHC_Single_28
		|| m_nRelation == EHC_Single_19;
}












//CLMjCard & CLMjCard::operator=(const int & nCard)
//{
//	CLCard::operator=(static_cast<CLCard>(nCard));
//	return *this;
//}
//
//CLMjCard & CLMjCard::operator=(const CLMjCard & rhs)
//{
//	CLCard::operator=(static_cast<CLCard>(rhs));
//	return *this;
//}




T_WeaveCardsItem & T_WeaveCardsItem::operator=(const T_WeaveCardsItem & rhs)
{

	byWeaveKind = rhs.byWeaveKind;
	cardCenter = rhs.cardCenter;
	cardPublic = rhs.cardPublic;
	byProvideUser = rhs.byProvideUser;
	for (int i = 0; i < 4; ++i)
	{
		aCards[i] = rhs.aCards[i];
	}
	return *this;
}

T_WeaveCardsItem & T_WeaveCardsItem::operator=(const T_WeaveCardsValueItem & tWeaveCardsValueItem)
{
	byWeaveKind = tWeaveCardsValueItem.byWeaveKind;
	cardCenter = tWeaveCardsValueItem.cardCenter;
	cardPublic = tWeaveCardsValueItem.cardPublic;
	byProvideUser = tWeaveCardsValueItem.byProvideUser;
	for (int i = 0; i < 4; ++i)
	{
		aCards[i] = tWeaveCardsValueItem.aCards[i];
	}
	return *this;
}

T_WeaveCardsItem::operator T_WeaveCardsValueItem()
{
	T_WeaveCardsValueItem tWeaveCardsValueItem;
	tWeaveCardsValueItem.byWeaveKind = byWeaveKind;
	tWeaveCardsValueItem.byWeaveKind = cardCenter;
	tWeaveCardsValueItem.byWeaveKind = cardPublic;
	tWeaveCardsValueItem.byWeaveKind = byProvideUser;
	for (int i = 0; i < 4; ++i)
	{
		tWeaveCardsValueItem.byWeaveKind = aCards[i];
	}
}
