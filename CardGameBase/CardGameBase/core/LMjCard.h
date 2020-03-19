#pragma once
#include "LCard.h"
#include "LMjDef.h"

#define MJ_COLOR_MASK	0xf0
#define MJ_VALUE_MASK	0x0f

// ��ģ������������ͨ��
class CLMjCard : public CLCard<CLMjCard>
{
public:

	// һ���������齫�ƹ�144�š��������ơ����ơ������ơ�
	enum E_MjCardColor
	{
		/*������ */
		EM_Circle,         //Ͳ�� 0-8
		EM_Character,      //���� 9-17
		EM_Bamboo,          //���� 18-26

		/* ����, �������ƺͼ���*/
		EM_Wind,            //���� -�����ϡ���������
		EM_Wrigley,         //���� -�С�������

		/* ���� */
		EM_Flower,           //���� -�����ġ������÷����������

		EM_Invalid = 255    //û����������
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

	virtual bool isValid();

	void initCard();

	// ��CLCard����Ϊ��ģ���Ͳ���������Щ�����������ˣ��������ģ���ǿ��
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

// һ���齫
extern const CLMjCard g_arrMjCardPair[];

// ���������
struct T_WeaveCardsItem
{
	unsigned char byWeaveKind;						//�������
	CLMjCard cardCenter;						//�����˿�
	unsigned char byPublicCard;						//������־
	unsigned char byProvideUser;					//��Ӧ�û�
	CLMjCard aCards[4];					//�������
	T_WeaveCardsItem();
};