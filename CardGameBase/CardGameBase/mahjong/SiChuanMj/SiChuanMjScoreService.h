#pragma once
#include "core/LMjDef.h"

// �齫��ҵĻ��ַ�����,����ַ���
class CSiChuanMjScoreService
{
public:
	void addKongSettlementItem(int nKongUser, const T_WeaveCardsValueItem & tWeaveCardsValueItem, std::vector<int> vecHu
		, int nPlayerNum, const T_DeskConfig & tDeskConfig , int *pScoreChanged = nullptr);

	void addHuSettlement(int nHuUser, const T_UserHuInfo & tUserHuInfo, std::vector<int> vecHu
		, int nPlayerNum, const T_DeskConfig & tDeskConfig, int *pScoreChanged = nullptr);


	int huScore(const T_UserHuInfo & tUserHuInfo, const T_DeskConfig & tDeskConfig);

	//void createSettlementList(std::vector<int> vecHu, int nPlayerNum, const T_DeskConfig & tDeskConfig);
private:
	// ��ҵĽ����嵥
	T_SettlementList arrSettlementList[MJ_MAX_PLAYER];

	int m_arrCurScoreChanged[MJ_MAX_PLAYER];

};
