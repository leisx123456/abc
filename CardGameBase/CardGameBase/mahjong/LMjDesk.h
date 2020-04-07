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
// �齫��ʹ�����黹��std::array, std::vector? ��ʹ������ɣ�һ�������̶�ֵ�����Ǹ�Ч, ���ڿ�����std::array(c++11)
class CLMjDesk
{
public:
	CLMjDesk();
	virtual ~CLMjDesk();

	// ���ú���
	//////////////////////////////////////////////////////////////////////////
	// ��ȡ��nUnit����λ������ 
	static int nextPlayerIndex(int nCurIndex, int nPlayerCount = MJ_MAX_PLAYER, bool bClockWise = true, int nUnit = 1);
	virtual int playerCount() const { return MJ_MAX_PLAYER; }	// ������Ϸ��ʵ�������� ��ʾ2����  3 ��ʾ3����  4��ʾ4��



	// shuffleϴ��,cut����,deal����,sort����,draw����,play���,discard����shuffle��������ָ����˳��,��������Ƶ�˳��,Ҳ���ǡ�ϴ�ơ�.
	//cut������.���׵�ʱ����һ�С������ơ�,��������������,ʵ����û�иĹ��Ƶ�...
	virtual void allocation() = 0; // �����齫�ӣ�����������Ǿ����齫��Ϸʵ�ʼ������,���Ĵ��齫ֻ������Ͳ
	virtual int mjNumAllocation() const { return MJ_MAX_CARD_COUNT; }	// �����齫����������������Ǿ����齫��Ϸʵ�ʼ�������������е�ֻ��������Ͳ����108

	void ruffle();	//ϴ��
	void initWall(); //���ĸ�����ڷ���ǽ
	void rockDice(); // ҡ���ӣ�ʵ��Ϊcut����
	void dealCards(CLMjCard pArrCardHand[][14]);	// deal����, ������
	void sortCard();	//sort����

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

	CLMjCard m_arrMjCardsPair[MJ_MAX_CARD_COUNT];	// һ���齫
	CLMjPlayer* m_pArrMjPlayer[MJ_MAX_PLAYER];	// �齫��� ���4��

	CLDicePair m_mjDicePair;	// 2���齫����

	CLMjCard m_arrMjCardWall[4][34];	// ��ǽ����m_arrMjCard���±�0��ʼ���ƣ�ׯ����������
	CLMjCard m_cardOut;



	int m_nMjNumSurplus;								//ʣ����ǽ�齫����
	int m_nIndexStart;	// ������ҡ������ȷ�����Ƶĵط�index��index��Ӧm_arrMjCard���±꣬С��m_nMjNum
	int m_nIndexCurrent;	// ��ǰץ�Ƶ�λ��index��index��Ӧm_arrMjCard���±꣬С��m_nMjNum

	int m_nBanker;	//ׯ�� 0-3
	int m_nActiveUser;	//��ǰ��û� -1, 0-3
	int m_nCurExecActUser;	//��ǰִ�ж������û� -1,0-3

	//int m_nAlreadyHuNum;	// ��ǰ�Ѻ�������0-3
	int m_nHuOrderNum;	// ��ʱ����������һ�ڶ���, ��������������ǲ������
	std::vector<int> _vecHu;	// ����id����
	
	//bool bBuKongCard;
	bool m_bHuangZhuang;

	T_DeskConfig m_tDeskConfig;

};

