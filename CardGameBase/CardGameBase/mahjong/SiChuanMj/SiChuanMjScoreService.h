#pragma once
#include "core/LMjDef.h"

// 麻将玩家的积分服务类,即算分服务
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
	// 玩家的结算清单
	T_SettlementList arrSettlementList[MJ_MAX_PLAYER];

	int m_arrCurScoreChanged[MJ_MAX_PLAYER];

};
