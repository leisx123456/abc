#ifndef __L_MJ_CARD_H__
#define __L_MJ_CARD_H__

#include "LCard.h"
#include "LMjDef.h"


#define MJ_COLOR_MASK	0xf0
#define MJ_VALUE_MASK	0x0f


// 定义麻将与其他牌能形成的最佳关系
enum E_HandCardRelationType
{
	EHC_Hu,
	EHC_Kong,
	EHC_Triplet,
	EHC_Sequence,
	EHC_Pair_only,
	EHC_Pair,
	EHC_Door_TwoHead,
	EHC_Door_Middle,
	EHC_Door_Edge,
	EHC_Single_34567,
	EHC_Single_28,
	EHC_Single_19,
	EHC_SingleToSingle_TwoHead,
	EHC_SingleToSingle_Middle,
	EHC_ingleToSingle_Edge,
	EHC_Tba,
	EHC_Badly,
	EHC_Relation_Invaild
};

// 从模板类中派生普通类
class CLMjCard : public CLCard<CLMjCard>
{
public:

	// 一副完整的麻将牌共144张。包括字牌、花牌、序数牌。
	enum E_MjCardColor
	{
		/*序数牌 */
		EM_Character,      //万牌 0x01-0x09
		EM_Bamboo,          //条牌 0x11-0x19
		EM_Circle,         //筒牌 0x21-0x29

		/* 字牌, 包含风牌和箭牌*/
		EM_Word,
		//EM_Wind,            //风牌 -东、南、西、北风
		//EM_Wrigley,         //箭牌 -中、发、白

		/* 花牌 */
		EM_Flower,           //花牌 -春、夏、秋、冬、梅、竹、兰、菊

		EM_Max_Color,
		EM_Invalid_Color = 255    //没有其他牌了
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

	virtual bool isValid() const;
	virtual void empty();

	virtual int logicValue(){ return m_nValue & MASK_VALUE; }
	virtual int switchToCardIndex();
	static CLMjCard switchToCardValue(int nCardIndex);

	virtual int color() const { return (m_nValue & MASK_COLOR) >> 4; }

	void initCard();
	bool isTBA(E_MjCardColor eMjCardColor);
	// 是否序数牌
	bool isOrderNumCard(){ return color() > -1 && color() < 3 && orderNumValue() > 0 && orderNumValue() < 10; }
	// 序数值
	int orderNumValue() const{ return m_nValue & MASK_VALUE; }
	// 是否幺九序数牌，即1,9万 1,9条 1,9同
	bool isYaoJiuOrderNum_19(){ return (m_nValue & MASK_VALUE) == 1 || (m_nValue & MASK_VALUE) == 9; }
	bool isJaing_28(){ return (m_nValue & MASK_VALUE) == 2 || (m_nValue & MASK_VALUE) == 8; }

	// 把CLCard申明为类模板后就不用重载这些操作符函数了，这就是类模板的强大
	//CLMjCard & operator = (const int & nCard);
	CLMjCard & operator = (const CLMjCard & rhs); //赋值函数需要重载



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

	bool isLocked() const
	{
		return m_bLocked;
	}
	void lock(){ m_bLocked = true; }
	void lock(int nRelation){ m_nRelation = nRelation; m_bLocked = true; }
	void unLock(bool releaseRelationship = false)
	{
		if (releaseRelationship)
		{
				m_nRelation = EHC_Relation_Invaild;
		}
		m_bLocked = false;
	}
	void setRelation(int nRelation){ m_nRelation = nRelation; }
	int getRelation(){ return m_nRelation; }

	bool isRelation3();
	bool isRelation2();
	bool isRelation1();

protected:
	E_CardResType m_eCardResType; //
	E_ViewPosture m_eViewPosture;
	E_ViewDirection m_eViewDirection;	//

	// ai相关
	bool m_bLocked;
	int m_nRelation;
	
};

// 一副麻将
extern const CLMjCard g_arrMjCardPair[];

// 组合牌子项
struct T_WeaveCardsItem
{

	enum E_WeaveCardsType
	{
		EW_Sequence, // 顺子
		EW_Triplet,	// 刻子
		EW_KongAn,	//下雨-暗杠
		EW_KongBa,	//刮风-巴杠
		EW_KongDian,	//刮风-点杠
		EW_InVaild
	};

	bool isKong() { return byWeaveKind == EW_KongAn || byWeaveKind == EW_KongBa || byWeaveKind == EW_KongDian; }

	unsigned char byWeaveKind;						//组合类型
	CLMjCard aCards[4];					//组合数据
	CLMjCard cardCenter;						//中心牌
	CLMjCard cardPublic;						//目标牌
	
	unsigned char byProvideUser;					//供应用户

	int arrNotHuUser[MJ_MAX_PLAYER];
	int nNotHuUserNum;

	T_WeaveCardsItem()
	{
		clear();
	}

	void clear()
	{
		byWeaveKind = EW_InVaild;
		cardCenter.empty();
		byProvideUser = 0;
		cardPublic.empty();
		for (int i = 0; i < 4; ++i)
		{
			aCards[i].empty();
		}
		memset(arrNotHuUser, 0, sizeof(arrNotHuUser));
		nNotHuUserNum = 0;
	}

	T_WeaveCardsItem & operator = (const T_WeaveCardsItem & rhs);
	T_WeaveCardsItem & operator = (const T_WeaveCardsValueItem & tWeaveCardsValueItem);
	operator T_WeaveCardsValueItem();
};


#endif //__L_MJ_CARD_H__