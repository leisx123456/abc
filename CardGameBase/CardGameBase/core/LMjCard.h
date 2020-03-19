#pragma once
#include "LCard.h"
#include "LMjDef.h"

#define MJ_COLOR_MASK	0xf0
#define MJ_VALUE_MASK	0x0f

// 从模板类中派生普通类
class CLMjCard : public CLCard<CLMjCard>
{
public:

	// 一副完整的麻将牌共144张。包括字牌、花牌、序数牌。
	enum E_MjCardColor
	{
		/*序数牌 */
		EM_Circle,         //筒牌 0-8
		EM_Character,      //万牌 9-17
		EM_Bamboo,          //条牌 18-26

		/* 字牌, 包含风牌和箭牌*/
		EM_Wind,            //风牌 -东、南、西、北风
		EM_Wrigley,         //箭牌 -中、发、白

		/* 花牌 */
		EM_Flower,           //花牌 -春、夏、秋、冬、梅、竹、兰、菊

		EM_Invalid = 255    //没有其他牌了
	};

	// 麻将视图视角方向
	enum E_ViewDirection
	{
		EV_South,
		EV_West,
		EV_North,
		EV_East
	};

	// 麻将姿势 - 3种， （长方体，6个面，对立面是相同的，分3组）， 但麻将是分反背面的
	enum E_ViewPosture
	{
		EV_Stand,	//站立
		EV_FacingDown,	// 麻将牌面朝下
		EV_FacingUp		// 麻将牌面朝上
	};

	// 按牌区分麻将牌
	enum E_CardResType
	{
		EC_Hand,		// 手牌
		EC_Weave,		// 组合牌
		EC_Out,			// 已出的牌
		EC_Wall,		// 牌墙
		EC_Hu			// 胡的牌
	};

	CLMjCard();
	CLMjCard(int nCard);
	CLMjCard(E_CardResType eCardResType, E_ViewDirection eViewDirection, int nCard);

	virtual bool isValid();

	void initCard();

	// 把CLCard申明为类模板后就不用重载这些操作符函数了，这就是类模板的强大
	//CLMjCard & operator = (const int & nCard);
	//CLMjCard & operator = (const CLMjCard & rhs);


	int color()
	{
		return m_nValue & MJ_COLOR_MASK;
	}

	E_ViewDirection getViewDirection()
	{
		return m_eViewDirection;
	}
	void setViewDirection(E_ViewDirection eViewDirection)
	{
		m_eViewDirection = eViewDirection;
	}

	E_ViewPosture getViewPosture()
	{
		return m_eViewPosture;
	}
	void setViewPosture(E_ViewPosture eViewPosture)
	{
		m_eViewPosture = eViewPosture;
	}

	int switchToResValue()
	{
		unsigned char cbValue = (m_nValue & MASK_VALUE);
		unsigned char cbColor = (m_nValue & MASK_COLOR) >> 4;
		return cbColor * 10 + cbValue;
	}

protected:
	E_CardResType m_eCardResType; //
	E_ViewPosture m_eViewPosture;
	E_ViewDirection m_eViewDirection;	//
};

// 一副麻将
extern const CLMjCard g_arrMjCardPair[];

// 组合牌子项
struct T_WeaveCardsItem
{
	unsigned char byWeaveKind;						//组合类型
	CLMjCard cardCenter;						//中心扑克
	unsigned char byPublicCard;						//公开标志
	unsigned char byProvideUser;					//供应用户
	CLMjCard aCards[4];					//组合数据
	T_WeaveCardsItem();
};