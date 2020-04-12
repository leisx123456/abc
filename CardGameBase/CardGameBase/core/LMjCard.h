#ifndef __L_MJ_CARD_H__
#define __L_MJ_CARD_H__

#include "LCard.h"
#include "LMjDef.h"


#define MJ_COLOR_MASK	0xf0
#define MJ_VALUE_MASK	0x0f


// �����齫�����������γɵ���ѹ�ϵ
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

// ��ģ������������ͨ��
class CLMjCard : public CLCard<CLMjCard>
{
public:

	// һ���������齫�ƹ�144�š��������ơ����ơ������ơ�
	enum E_MjCardColor
	{
		/*������ */
		EM_Character,      //���� 0x01-0x09
		EM_Bamboo,          //���� 0x11-0x19
		EM_Circle,         //Ͳ�� 0x21-0x29

		/* ����, �������ƺͼ���*/
		EM_Word,
		//EM_Wind,            //���� -�����ϡ���������
		//EM_Wrigley,         //���� -�С�������

		/* ���� */
		EM_Flower,           //���� -�����ġ������÷����������

		EM_Max_Color,
		EM_Invalid_Color = 255    //û����������
	};


	// �齫��ͼ�ӽǷ���
	enum E_ViewDirection
	{
		EV_South,
		EV_West,
		EV_North,
		EV_East
	};

	// �齫���� - 3�֣� �������壬6���棬����������ͬ�ģ���3�飩�� ���齫�Ƿַ������
	enum E_ViewPosture
	{
		EV_Stand,	//վ��
		EV_FacingDown,	// �齫���泯��
		EV_FacingUp		// �齫���泯��
	};

	// ���������齫��
	enum E_CardResType
	{
		EC_Hand,		// ����
		EC_Weave,		// �����
		EC_Out,			// �ѳ�����
		EC_Wall,		// ��ǽ
		EC_Hu			// ������
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
	// �Ƿ�������
	bool isOrderNumCard(){ return color() > -1 && color() < 3 && orderNumValue() > 0 && orderNumValue() < 10; }
	// ����ֵ
	int orderNumValue() const{ return m_nValue & MASK_VALUE; }
	// �Ƿ��۾������ƣ���1,9�� 1,9�� 1,9ͬ
	bool isYaoJiuOrderNum_19(){ return (m_nValue & MASK_VALUE) == 1 || (m_nValue & MASK_VALUE) == 9; }
	bool isJaing_28(){ return (m_nValue & MASK_VALUE) == 2 || (m_nValue & MASK_VALUE) == 8; }

	// ��CLCard����Ϊ��ģ���Ͳ���������Щ�����������ˣ��������ģ���ǿ��
	//CLMjCard & operator = (const int & nCard);
	CLMjCard & operator = (const CLMjCard & rhs); //��ֵ������Ҫ����



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

	// ai���
	bool m_bLocked;
	int m_nRelation;
	
};

// һ���齫
extern const CLMjCard g_arrMjCardPair[];

// ���������
struct T_WeaveCardsItem
{

	enum E_WeaveCardsType
	{
		EW_Sequence, // ˳��
		EW_Triplet,	// ����
		EW_KongAn,	//����-����
		EW_KongBa,	//�η�-�͸�
		EW_KongDian,	//�η�-���
		EW_InVaild
	};

	bool isKong() { return byWeaveKind == EW_KongAn || byWeaveKind == EW_KongBa || byWeaveKind == EW_KongDian; }

	unsigned char byWeaveKind;						//�������
	CLMjCard aCards[4];					//�������
	CLMjCard cardCenter;						//������
	CLMjCard cardPublic;						//Ŀ����
	
	unsigned char byProvideUser;					//��Ӧ�û�

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