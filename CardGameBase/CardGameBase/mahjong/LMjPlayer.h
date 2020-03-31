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
	// �û���Ϣ
	string strNickname;
	unsigned short cbScore;
	string strHeadPath;

	bool bOffline;
	bool bHouseOwner;

	bool bBanker;
	bool bQingUue;
	bool bReady;
	bool bBaoJiao;
	bool bHu;
	bool bBuyCard;
	CLMjCard::E_MjCardColor eTBACardColor;
	E_OperatorStatus eOperatorStatus;

	T_UserInfo()
		: strNickname("12345")
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

// ���ƻ�������
struct T_UserCardsData
{
	CLMjCard arrHandCards[MJ_MAX_HAND_COUNT];
	int nHandNums;

	T_WeaveCardsItem arrWeaveCardsItem[4];
	int nWeaveItemNums;

	int nColorQue;
	CLMjCard cardNew;
};

class IAbstractThink;

class CLMjPlayer
{
public:
	enum E_PlayerType
	{
		EP_People,
		EP_CmpEasy,
		EP_CmpNormal,
		EP_CmpClever
	};

	//���״̬
	enum E_PlayerStatus
	{
		EPS_Waiting,
		EPS_ThinkOutCard,
		EPS_Choosing,
		EPS_Hu,
	};

	//��һ״̬
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

	CLMjPlayer();
	CLMjPlayer(E_PlayerType ePlayerType);
	~CLMjPlayer();

	void init();
	//////////////////////////////////////////////////////////////////////////
	// �������
	bool isReboot() {return m_ePlayerType != EP_People; }


	//////////////////////////////////////////////////////////////////////////
	// ���״̬
	void ready() { m_bReady = true; }
	void cancelReady(){ m_bReady = false; }
	bool isReady() { return m_bReady; }
	
	bool isActive() { return m_ePlayerActiveState == P_Active; }
	void setActive(E_PlayerActiveState ePlayerActiveState){ m_ePlayerActiveState = ePlayerActiveState; }
	void cancelActive(){ m_ePlayerActiveState = p_unActive; }


	//////////////////////////////////////////////////////////////////////////
	// ��Ҷ���������

	// ������
	void dealCards(const CLMjCard arrHandCards[MJ_MAX_HAND_COUNT]);
	void getHandCards(CLMjCard* pArrHandCards);

	// ��ȱ
	void selectTBA(CLMjCard::E_MjCardColor eColorTBA) { m_eColorTBA = eColorTBA; }
	bool isAlreadyTBA(){ return m_eColorTBA > -1 && m_eColorTBA < 3; }
	int getDingQueColor() { return m_eColorTBA; }


	//����
	void drawCard(const CLMjCard & card);

	//��ҳ���
	bool outCard(const CLMjCard & card);
	CLMjCard getLatestOutCard();
	void getOutCardsValue(int* pArrOutCardsValue, int & nOutCardNums);

	// ��Ҷ���
	void execAction(T_ActRequest tActRequest);
	bool execPong(const CLMjCard & cardOut);
	bool execKong();
	bool execHu();





	//���ú�
	void setHu(int nCard)
	{
		m_vecHu.push_back(nCard);
	}

	//��ó��˵��ƶ���
	//vector<int> getOutCard();
//////////////////////////////////////////////////////////////////////////
// �߼��ж�

	// ��ȡ��Ҷ����Լ�����(�Լ��ǻ״̬) ����˳�����(�����ǻ״̬) ����ѡ��Ķ���
	bool selectActInfo(T_MjActInfo* pActInfo, CLMjCard cardOut, unsigned short usIgnoreFlags = 0);

	bool isCanPong(CLMjCard cardOut);
	bool isCanKong(CLMjCard cardOut, T_MjActInfo* pActInfo);
	bool isCanHu(CLMjCard cardOut);


	//////////////////////////////////////////////////////////////////////////
	// ai˼��
	CLMjCard::E_MjCardColor thinkDingQue();
	void think(T_ActRequest* pActRequest, CLMjCard cardDest, unsigned short usIgnoreFlags = 0);


private:
	//////////////////////////////////////////////////////////////////////////
	// ai
	E_PlayerType m_ePlayerType;
	IAbstractThink* m_pIAbstractThink;

	// 
	CLMjLogic m_mjLogic;
	T_WeaveCardsItem m_arrWeaveCardsItem[4];	// �û��������
	int m_nWeaveItemNums;

	CLMjCard m_arrHandCards[MJ_MAX_HAND_COUNT];	//�û�������
	int m_nHandNums;					//�û���������

	// --ȡ������Ϊ��������������14�������Ѽ��뵽������
	//CLMjCard m_CardNew;	// ��ץ��������

	vector<CLMjCard> m_vecCardOut;	//�ѳ��Ʊ�(�����������ù�ȥ�����ܺ���)
	int m_nOutedTimes;					//�ѳ�������(�����������ù�ȥ�����ܺ���)

	vector<CLMjCard> m_vecTing;	//���ƶ���
	vector<CLMjCard> m_vecHu;	//���ƶ��У���չ��Ѫ���ɺ�
	vector<CLMjCard> m_vecCardsLouHu;	// ©������
	

	int m_nDrawTimes;	//ץ�ƴ���
	
	E_PlayerActiveState m_ePlayerActiveState; // ��һ״̬
	E_PlayerStatus m_ePlayerStatus;

	CLMjCard::E_MjCardColor m_eColorTBA;	//��ȱ����

	

	bool m_bIsHu;						//�Ƿ��Ѿ�����
	
	
	//////////////////////////////////////////////////////////////////////////
	bool m_bReady;



};

#endif //_L_MJ_PLAYER_H_
