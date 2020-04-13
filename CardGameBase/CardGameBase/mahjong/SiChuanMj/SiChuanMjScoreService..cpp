#include "SiChuanMjScoreService.h"
#include <math.h>

void CSiChuanMjScoreService::addKongSettlementItem(int nKongUser, const T_WeaveCardsValueItem & tWeaveCardsValueItem, std::vector<int> vecHu
	, int nPlayerNum, const T_DeskConfig & tDeskConfig, int *pScoreChanged /*= nullptr*/)
{
	memset(m_arrCurScoreChanged, 0, sizeof(m_arrCurScoreChanged));
	if (tWeaveCardsValueItem.byWeaveKind == 2 || tWeaveCardsValueItem.byWeaveKind == 3)//�����벹��
	{
		int nKongUnitScore = 2 * tDeskConfig.bBaseScore;
		if (tWeaveCardsValueItem.byWeaveKind == 3)
		{
			nKongUnitScore = tDeskConfig.bBaseScore;
		}
		int nBeKongNum = 0;
		for (int i = 0; i < nPlayerNum; ++i)
		{
			if (i == nKongUser)
			{
				continue;
			}
			std::vector<int>::iterator it = std::find(vecHu.begin(), vecHu.end(), i);
			if (it != vecHu.end())
			{
				continue;
			}
			++nBeKongNum;
			m_arrCurScoreChanged[i] = -nKongUnitScore;
		}
		m_arrCurScoreChanged[nKongUser] = nBeKongNum * nKongUnitScore;
	}
	else
	{
		m_arrCurScoreChanged[nKongUser] = 2 * tDeskConfig.bBaseScore;
		m_arrCurScoreChanged[tWeaveCardsValueItem.byProvideUser] = -2 * tDeskConfig.bBaseScore;
	}

	for (int i = 0; i < nPlayerNum; ++i)
	{
		if (m_arrCurScoreChanged[i] == 0)
		{
			continue;
		}
		T_KongScoreItem tKongLostItem(nKongUser, tWeaveCardsValueItem, m_arrCurScoreChanged[i]);
		arrSettlementList[i].arrKongScoreItem[arrSettlementList[i].nKongNum++] = tKongLostItem;
	}

	if (pScoreChanged == nullptr)
	{
		return;
	}
	memcpy(pScoreChanged, m_arrCurScoreChanged, sizeof(m_arrCurScoreChanged));
}

/*

*/
void CSiChuanMjScoreService::addHuSettlement(int nHuUser, const T_UserHuInfo & tUserHuInfo, std::vector<int> vecHu
	, int nPlayerNum, const T_DeskConfig & tDeskConfig, int *pScoreChanged /*= nullptr*/)
{
	memset(m_arrCurScoreChanged, 0, sizeof(m_arrCurScoreChanged));

	int nHuScore = 0;
	if (tUserHuInfo.isZiMoType())
	{
		for (int i = 0; i < nPlayerNum; ++i)
		{
			if (i == nHuUser)
			{
				continue;
			}
			if (arrUserHuInfo[i].nHuIndex > arrUserHuInfo[nHuUser].nHuIndex)
			{
				T_HuScoreItem tLostScoreItem(nHuUser, arrUserHuInfo[nHuUser]);
				arrSettlementList[i].addHuLostItem(tLostScoreItem);
			}
		}
	}
	else
	{
		T_HuScoreItem tLostScoreItem(arrUserHuInfo[nHuUser].byFangPaoUser, arrUserHuInfo[nHuUser]);
		arrSettlementList[arrUserHuInfo[nHuUser].byFangPaoUser].addHuLostItem(tLostScoreItem);
	}


}

int CSiChuanMjScoreService::huScore(const T_UserHuInfo & tUserHuInfo, const T_DeskConfig & tDeskConfig)
{
	if (!bHu)
	{
		return;
	}
	// E_MjHuWay  + E_MjHuName + ��service
	int nUnit = 0;
	int nFan = 0;
	//E_MjHuWay
	switch (eMjHuWay)
	{
	case EHW_Unknown:
		break;
	case EHW_JiePao:
		break;
	case EHW_ZiMo:
		nUnit++;//�����ӵ�
		break;
	case EHW_DayHu:
		nUnit++;
		nFan += 6;
		goto end;
	case EHW_LandHu:
		nUnit++;
		nFan += 6;
		goto end;
	case EHW_PensonHu:
		nFan += 6;
		goto end;
	case EHW_QiangGang:
		nFan++;
		break;
	case EHW_DianGangKai:
		nUnit++;
		nFan++;
		break;
	case EHW_AnGangKai:
		nUnit++;
		nFan++;
		break;
	case EHW_BuGangKai:
		nUnit++;
		nFan++;
		break;
	default:
		break;
	}

	// E_MjHuName
	switch (eMjHuName)
	{
	case EHN_Ping:
		break;
	case EHN_Pong:
		nFan++;
		break;
	case EHN_Qing:
		nFan += 2;
		break;
	case EHN_SevenPair:
		nFan += 2;
		break;
	case EHN_JinGouDiao:
		nFan += 2;
		break;
	case EHN_DragonSevenPair:
		nFan += 3;
		break;
	case EHN_QingPong:
		nFan += 3;
		break;
	case EHN_QingSevenPair:
		nFan += 4;
		break;
	case EHN_QingDragonSevenPair:
		nFan += 5;
		break;
	case EHN_QingJinGouDiao:
		nFan += 4;
		break;
	default:
		break;
	}

	// �ط��������
	if (tDeskConfig.bAbleHaiDiLao)
	{
		if (bHaiDiLao)
		{
			nFan++;
		}
	}

	//gen
	nFan += nGeng;	// ���ͷ���+��

end:
	nTotalFan = nFan;
	nFinalFan = nTotalFan;
	if (nTotalFan > tDeskConfig.nMaxFan)
	{
		nFinalFan = tDeskConfig.nMaxFan;
	}

	int nFinalMultiple = pow(2, nFinalFan); //����
	nFinalUnit = nUnit + nFinalMultiple;
	nFinalScore = nFinalUnit * tDeskConfig.bBaseScore;
}


//void T_MsgResult::createSettlementList(std::vector<int> vecHu, int nPlayerNum, const T_DeskConfig & tDeskConfig)
//{
//	// �������뱻�������֮���������������ȸ�һ������ҽ��㣬�ٸ���������ҽ���,�������
//	for (int i = 0; i < vecHu.size(); i++)
//	{
//		int nHuUser = vecHu.at(i);
//		addHuSettlement(nHuUser, arrUserHuInfo[nHuUser], nPlayerNum);
//	}
//
//	// ���ɸ��嵥
//
//
//	for (int i = 0; i < nPlayerNum; ++i)
//	{
//		for (int j = 0; j < arrKongNum[i]; ++j)
//		{
//			addKongSettlementItem(i, arrWeaveCardsKong[i][j]);
//		}
//	}
//
//	// ����
//	for (int i = 0; i < nPlayerNum; ++i)
//	{
//		arrSettlementList[i].calculateScore(tDeskConfig);
//	}
//}


//void T_SettlementList::calculateScore(const T_DeskConfig & tDeskConfig)
//{
//	tUserHuInfo.calculateScore(tDeskConfig);
//
//	nHuLostScore = 0;
//	for (int i = 0; i < nHuLostItemNum; ++i)
//	{
//		nHuLostScore += arrHuLostItem[i].calculateScore(tDeskConfig);
//	}
//
//	nKongScore = 0;
//	for (int i = 0; i < nKongNum; ++i)
//	{
//		nKongScore += arrWeaveCardsKongGot[i].calculateScore(tDeskConfig);
//	}
//
//	nKongScore = 0;
//	for (int i = 0; i < nKongNum; ++i)
//	{
//		nKongScore += arrKongScoreItem[i].calculateScore(tDeskConfig);
//	}
//
//	nRealScore = tUserHuInfo.nFinalScore - nHuLostScore + nKongScore - nKongScore;
//}
