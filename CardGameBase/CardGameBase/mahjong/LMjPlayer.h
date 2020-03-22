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

enum E_OperatorStatus
{
	EO_Choosing,         //ѡ����
	EO_Choosed,				//��ѡ��
	EO_TBA_Choosing,		//��ȱ��
	EO_TBA_Choosed,			//�Ѷ�ȱ
	EO_NULL					//δ����
};

struct T_UserInfo
{
	string strNickname;
	unsigned short cbScore;
	bool bOffline;
	string strHeadPath;
	bool bBanker;
	bool bHouseOwner;
	//		img_dingque
	bool bReady;
	bool bBaoJiao;
	bool bHu;
	bool bBuyCard;
	CLMjCard::E_MjCardColor eTBACardColor;
	E_OperatorStatus eOperatorStatus;

	T_UserInfo()
		: strNickname("")
		, cbScore(0)
		, bOffline(false)
		, strHeadPath("")
		, bBanker(false)
		, bHouseOwner(false)
		, bReady(false)
		, bBaoJiao(false)
		, bHu(false)
		, bBuyCard(false)
		, eTBACardColor(CLMjCard::EM_Invalid_Color)
		, eOperatorStatus(EO_NULL)
	{

	}
};

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

	enum E_Sex
	{
		S_Boy,
		S_Girl
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

	// ��ȱ
	void selectTBA(CLMjCard::E_MjCardColor eColorTBA);

	void setReady(bool bReady);
	bool isReady() { return m_bReady; }

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
	
	
	//////////////////////////////////////////////////////////////////////////
	bool m_bReady;


	// ��player����Ϊ������ʱʹ��AI����
	//CLMjAI m_mjAI;
};

#endif //_L_MJ_PLAYER_H_
