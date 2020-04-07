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

//////////////////////////////////////////////////////////////////////////
// �齫��ҳ�����
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


	CLMjPlayer();
	CLMjPlayer(E_PlayerType ePlayerType, int nChairID);
	virtual ~CLMjPlayer();

	void init();
	//////////////////////////////////////////////////////////////////////////
	// �������
	bool isReboot() {return m_ePlayerType != EP_People; }
	virtual void initLogic() = 0;

	//////////////////////////////////////////////////////////////////////////
	// ���״̬
	void ready() { m_bReady = true; }
	void cancelReady(){ m_bReady = false; }
	bool isReady() { return m_bReady; }
	
	bool isActive() { return m_ePlayerActiveState == P_Active; }
	void setActive(E_PlayerActiveState ePlayerActiveState){ m_ePlayerActiveState = ePlayerActiveState; }
	void cancelActive(){ m_ePlayerActiveState = p_unActive; }
	bool isAlreadyHu() { return m_tUserHuInfo.bHu; }


	//////////////////////////////////////////////////////////////////////////
	// ��Ҷ���������

	// ������
	void dealCards(const CLMjCard arrHandCards[MJ_MAX_HAND_COUNT]);
	void getHandCards(CLMjCard* pArrHandCards, int & nHandNums);
	void getHandCards(int pArrHandCards[], int & nHandNums);

	// ��ȱ
	void selectTBA(CLMjCard::E_MjCardColor eColorTBA) { m_eColorTBA = eColorTBA; }
	bool isAlreadyTBA(){ return m_eColorTBA > -1 && m_eColorTBA < 3; }
	int getDingQueColor() { return m_eColorTBA; }


	//����
	void drawCard(const CLMjCard & card);

	// ����
	void sordCards();

	//��ҳ���
	bool outCard(const CLMjCard & card);
	CLMjCard getLatestOutCard();
	void getOutCardsValue(int* pArrOutCardsValue, int & nOutCardNums);
	void removeLatestOutCard();
	int outCardNums() { return m_vecCardOut.size(); }

	// ��Ҷ���

	bool execPong(unsigned char byProvideUser, const CLMjCard & cardOut);
	bool execKong(unsigned char byProvideUser, const CLMjCard & cardOut/*, int nCurSelectIndex*/);
	bool execHu(unsigned char byProvideUser, int nHuIndex, const CLMjCard & cardOut);
	void getWeaveCardsItems(T_WeaveCardsItem* pWeaveCardsItem, int & nWeaveItemNums);
	T_WeaveCardsItem & getLatestWeaveCardsItem();
	int weaveItemNums() { return m_nWeaveItemNums; }


	//���ú�
	void setHu(int nCard)
	{
		m_vecHu.push_back(nCard);
	}
	T_UserHuInfo & userHuInfo()
	{
		return m_tUserHuInfo;
	}
	int huIndex();

	//��ó��˵��ƶ���
	//vector<int> getOutCard();
//////////////////////////////////////////////////////////////////////////
// �߼��ж�

	// ��ȡ��Ҷ����Լ�����(�Լ��ǻ״̬) ����˳�����(�����ǻ״̬) ����ѡ��Ķ���
	bool selectActInfo(CLMjCard cardOut, unsigned short usIgnoreFlags = 0);
	T_MjActInfo* actInfo() { return &m_tMjActInfo; }

	bool isCanPong(CLMjCard cardOut);
	bool isCanKong(CLMjCard cardOut, T_MjActInfo* pActInfo);
	bool isCanHu(CLMjCard cardOut);


	//////////////////////////////////////////////////////////////////////////
	// ai˼��
	CLMjCard::E_MjCardColor thinkDingQue();
	void think(CLMjCard cardOut, unsigned short usIgnoreFlags = 0);

protected:
	//////////////////////////////////////////////////////////////////////////


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

	CLMjLogic* m_pMjLogic;

	// ������� ai or people
	E_PlayerType m_ePlayerType;

	// ai
	IAbstractThink* m_pIAbstractThink;

	// ������ҵ�ǰ����ִ�е���Щ����(�������ͨ��)
	T_MjActInfo m_tMjActInfo;

	//  ������ұ��ֺ�����Ϣ
	T_UserHuInfo m_tUserHuInfo;
	
	//////////////////////////////////////////////////////////////////////////
	bool m_bReady;


	//////////////////////////////////////////////////////////////////////////
	// �������
	int m_nChairID;

	//bool bOffline;
	//bool bHouseOwner;

	bool bBanker;
	//bool bQingUue;
	//bool bBaoJiao;
	bool bHu;
	//bool bBuyCard;
	CLMjCard::E_MjCardColor eTBACardColor;
	E_OperatorStatus eOperatorStatus;

};

#endif //_L_MJ_PLAYER_H_
