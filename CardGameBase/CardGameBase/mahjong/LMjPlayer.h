#ifndef _L_MJ_PLAYER_H_
#define _L_MJ_PLAYER_H_

/************************************************************************/
/*
�����ƵĹ�����صļ�¼
*/
/************************************************************************/
#include <vector>
#include <algorithm>
#include "core/LMjDef.h"
#include "core/LMjCard.h"
#include "LMjLogic.h"

using namespace std;

class CLMjPlayer
{
public:
	CLMjPlayer();
	~CLMjPlayer();

	//���״̬
	enum E_PlayerStatus
	{
		EPS_Waiting,
		EPS_ThinkOutCard,
		EPS_Choosing,
		EPS_Hu,
	};

	enum E_PlayerActiveState
	{
		P_Active,
		p_unActive
	};

	void dealCards(const CLMjCard arrHandCards[MAX_HAND_COUNT]);
	void getHandCards(CLMjCard* pArrHandCards);

	//������
	void drawCard(const CLMjCard & card);

	//���ñ��˳�����
	void setOtherOutCard(int nCard);

	//���ѳ��ƶ�����ɾ��һ������������
	void removeListOutCardAtLast();

	//��ҳ���
	int outCard(int nPlace);



	//���ú�
	void setHu(int nCard)
	{
		m_vecHu.push_back(nCard);
	}

	//��ó��˵��ƶ���
	//vector<int> getOutCard();

	// ����chair��ʱ��ת1�����
	int antiClockWise(int chair)
	{
		return (chair + PLAYER_NUM - 1) % PLAYER_NUM;
	}

	// ����chair˳ʱ��ת1�����
	int clockwise(int chair)
	{
		return (chair + 1) % PLAYER_NUM;
	}



private:
	CLMjLogic m_mjLogic;
	T_WeaveCardsItem m_arrWeaveCardsItem[4];	// �û��������

	CLMjCard m_arrHandCards[MAX_HAND_COUNT];	//�û�������
	int m_nHandNums;					//�û���������

	CLMjCard m_CardNew;	// ��ץ��������

	CLMjCard m_arrOutedCards[MJ_MAX_OUTED];	//�ѳ��Ʊ�(�����������ù�ȥ�����ܺ���)
	int m_nOutedTimes;					//����ѳ��ƵĴ���(���������ù�ȥ�����ܺ���)
	//int m_nOutedNums;					//�ѳ�������(�����������ù�ȥ�����ܺ���)

	vector<CLMjCard> m_vecTing;	//���ƶ���
	vector<CLMjCard> m_vecHu;	//���ƶ��У���չ��Ѫ���ɺ�
	vector<CLMjCard> m_lstCardOut;	//������
	vector<CLMjCard> m_vecCardsLouHu;	// ©������
	

	int m_nDrawTimes;	//ץ�ƴ���
	
	E_PlayerActiveState m_ePlayerActiveState;
	E_PlayerStatus m_ePlayerStatus;

	CLMjCard::E_MjCardColor m_eColorTBA;	//��ȱ����

	
	int	m_nOtherOutCard;	//���˳�����

	bool m_bIsHu;						//�Ƿ��Ѿ�����
	
	



	// ��player����Ϊ������ʱʹ��AI����
	//CLMjAI m_mjAI;
};

#endif //_L_MJ_PLAYER_H_
