#pragma once
#include "../core/LClockActive.h"
#include "LMjPlayer.h"
#include "../core/LDicePair.h"
#include "LMjMachine.h"
#include "LMjLogic.h"
/************************************************************************/
/* 
��һ�������࣬��һ���齫�������Ϊ��λ����Ϸ
*/
/************************************************************************/
// �齫����:
//���ڸ����齫������ͬ�����Ĵ��齫ֻҪ����Ͳ����û�о���Ĺ����ܽᣬ�����Ȳ��ô���
// �齫��ʹ�����黹��STL? ����
class CLMjDesk
{
public:
	CLMjDesk();
	virtual ~CLMjDesk();

	// ���ú���
	//////////////////////////////////////////////////////////////////////////
	// ��ȡ�¸���ҵ����� nPlayerCountΪʵ�ʲ���������
	static int nextPlayerIndex(int nCurIndex, int nPlayerCount = GAME_MJ_PLAYER_MAX);

	// �����齫�ӣ����Ĵ��齫ֻ������Ͳ
	virtual void allocation() = 0;
	virtual int playerCount() const { return GAME_MJ_PLAYER_MAX; }	// ������Ϸ��ʵ�������� ��ʾ2����  3 ��ʾ3����  4��ʾ4��
	virtual int mjNum() const { return GAME_MJ_CARD_COUNT_MAX; }	// ����������Ǿ����齫��Ϸʵ�ʼ�������������е�ֻ��������Ͳ����108

	//ϴ��
	void ruffle();

	//���ĸ�����ڷ���ǽ
	void initWall();

	// ҡ���ӣ�ʵ��Ϊcut����
	void rockDice();

	// deal����, ������
	void dealCards(CLMjCard pArrCardHand[][14]);

	//sort����
	void sortCard();

	// shuffleϴ��,cut����,deal����,sort����,draw����,play���,discard����

	// ���ƣ���ȡ��ǰ�齫
	CLMjCard drawCard();



	//������ʼλ�ã�ׯ�Һ�ɫ������
	void beginGetCardPoint();

	//���ʣ���齫����
	int getSurplusCardsOnWall();



	////add
	//void initHandCard();
	//void SortCardData(byte wviewid);
	//void outCard(byte cbViewId, byte cbCard);
	////void actionOperator(byte cbViewId);

	// ������ô���� ������Щ����������ȥʵ��
protected:
	CLMjLogic m_mjLogic;

	CLMjCard m_arrMjCard[GAME_MJ_CARD_COUNT_MAX];	// һ���齫 ���136��
	CLMjPlayer m_arrMjPlayer[GAME_MJ_PLAYER_MAX];	// �齫��� ���4��

	CLDicePair m_mjDicePair;	// 2���齫����

	CLMjCard m_arrMjCardWall[4][34];	// ��ǽ����m_arrMjCard���±�0��ʼ���ƣ�ׯ����������



	int m_nMjNumSurplus;								//ʣ����ǽ�齫����
	int m_nIndexStart;	// ������ҡ������ȷ�����Ƶĵط�index��index��Ӧm_arrMjCard���±꣬С��m_nMjNum
	int m_nIndexCurrent;	// ��ǰץ�Ƶ�λ��index��index��Ӧm_arrMjCard���±꣬С��m_nMjNum

	int m_nBanker;	//ׯ�� 0-3
	
	//int m_nWhere;								//�齫ȡ���Ǹ�λ��
	//int m_nWho;									//��ȡ�ĸ�������齫
	//int m_nTailWho;								//�ʼ�õ��ļҵ���,���ڼ�¼

	//CLMjCard m_nHandCard[4][MAX_HAND_COUNT];
	//int m_nHandCardCount[4];
	//T_WeaveItem m_nWeaveItemArray[4][MAX_WEAVE];		//����˿�
	//byte m_nWeaveCount[4];					//�����Ŀ
	//std::vector<CLMjCard>  m_vecOutCard[4];
};

