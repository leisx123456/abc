
#include "MjLogic.h"
#include "math.h"
#include <assert.h>


//////////////////////////////////////////////////////////////////////////

//静态变量
bool		CChiHuRight::m_bInit = false;
dword		CChiHuRight::m_dwRightMask[MAX_RIGHT_COUNT];

//构造函数
CChiHuRight::CChiHuRight()
{
	zeromemory(m_dwRight, sizeof(m_dwRight));

	if (!m_bInit)
	{
		m_bInit = true;
		for (byte i = 0; i < CountArray(m_dwRightMask); i++)
		{
			if (0 == i)
				m_dwRightMask[i] = 0;
			else
				m_dwRightMask[i] = (dword(pow(2, i - 1))) << 28;
		}
	}
}

//赋值符重载
CChiHuRight & CChiHuRight::operator = (dword dwRight)
{
	dword dwOtherRight = 0;
	//验证权位
	if (!IsValidRight(dwRight))
	{
		//验证取反权位
		assert(IsValidRight(~dwRight));
		if (!IsValidRight(~dwRight)) return *this;
		dwRight = ~dwRight;
		dwOtherRight = MASK_CHI_HU_RIGHT;
	}

	for (byte i = 0; i < CountArray(m_dwRightMask); i++)
	{
		if ((dwRight & m_dwRightMask[i]) || (i == 0 && dwRight < 0x10000000))
			m_dwRight[i] = dwRight & MASK_CHI_HU_RIGHT;
		else m_dwRight[i] = dwOtherRight;
	}

	return *this;
}

//与等于
CChiHuRight & CChiHuRight::operator &= (dword dwRight)
{
	bool bNavigate = false;
	//验证权位
	if (!IsValidRight(dwRight))
	{
		//验证取反权位
		assert(IsValidRight(~dwRight));
		if (!IsValidRight(~dwRight)) return *this;
		//调整权位
		dword dwHeadRight = (~dwRight) & 0xF0000000;
		dword dwTailRight = dwRight&MASK_CHI_HU_RIGHT;
		dwRight = dwHeadRight | dwTailRight;
		bNavigate = true;
	}

	for (byte i = 0; i < CountArray(m_dwRightMask); i++)
	{
		if ((dwRight&m_dwRightMask[i]) || (i == 0 && dwRight < 0x10000000))
		{
			m_dwRight[i] &= (dwRight&MASK_CHI_HU_RIGHT);
		}
		else if (!bNavigate)
			m_dwRight[i] = 0;
	}

	return *this;
}

//或等于
CChiHuRight & CChiHuRight::operator |= (dword dwRight)
{
	//验证权位
	if (!IsValidRight(dwRight)) return *this;

	for (byte i = 0; i < CountArray(m_dwRightMask); i++)
	{
		if ((dwRight&m_dwRightMask[i]) || (i == 0 && dwRight < 0x10000000))
			m_dwRight[i] |= (dwRight&MASK_CHI_HU_RIGHT);
	}

	return *this;
}

//与
CChiHuRight CChiHuRight::operator & (dword dwRight)
{
	CChiHuRight chr = *this;
	return (chr &= dwRight);
}

//与
CChiHuRight CChiHuRight::operator & (dword dwRight) const
{
	CChiHuRight chr = *this;
	return (chr &= dwRight);
}

//或
CChiHuRight CChiHuRight::operator | (dword dwRight)
{
	CChiHuRight chr = *this;
	return chr |= dwRight;
}

//或
CChiHuRight CChiHuRight::operator | (dword dwRight) const
{
	CChiHuRight chr = *this;
	return chr |= dwRight;
}

//是否权位为空
bool CChiHuRight::IsEmpty()
{
	for (byte i = 0; i < CountArray(m_dwRight); i++)
	if (m_dwRight[i]) return false;
	return true;
}

//设置权位为空
void CChiHuRight::SetEmpty()
{
	zeromemory(m_dwRight, sizeof(m_dwRight));
	return;
}

//获取权位数值
byte CChiHuRight::GetRightData(dword dwRight[], byte cbMaxCount)
{
	assert(cbMaxCount >= CountArray(m_dwRight));
	if (cbMaxCount < CountArray(m_dwRight)) return 0;

	memcpy(dwRight, m_dwRight, sizeof(dword)*CountArray(m_dwRight));
	return CountArray(m_dwRight);
}

//设置权位数值
bool CChiHuRight::SetRightData(const dword dwRight[], byte cbRightCount)
{
	assert(cbRightCount <= CountArray(m_dwRight));
	if (cbRightCount > CountArray(m_dwRight)) return false;

	zeromemory(m_dwRight, sizeof(m_dwRight));
	memcpy(m_dwRight, dwRight, sizeof(dword)*cbRightCount);

	return true;
}

//检查仅位是否正确
bool CChiHuRight::IsValidRight(dword dwRight)
{
	dword dwRightHead = dwRight & 0xF0000000;
	for (byte i = 0; i < CountArray(m_dwRightMask); i++)
	if (m_dwRightMask[i] == dwRightHead) return true;
	return false;
}

//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//静态变量

//扑克数据
//GYZJ
const byte MjLogic::m_cbCardDataArray_HZ[CARD_TOTAL_NUM] =
{
	0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09,						//万子
	0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09,						//万子
	0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09,						//万子
	0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09,						//万子
	0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19,						//索子
	0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19,						//索子
	0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19,						//索子
	0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19,						//索子
	0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29,						//同子
	0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29,						//同子
	0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29,						//同子
	0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29,						//同子
	/*0x35,0x35,0x35,0x35,
	0x36,0x36,0x36,0x36,
	0x37,0x37,0x37,0x37*/
};

//////////////////////////////////////////////////////////////////////////

//构造函数
MjLogic::MjLogic()
{
	m_cbMagicIndex = MAX_INDEX;
}

//析构函数
MjLogic::~MjLogic()
{
}

//混乱扑克
void MjLogic::RandCardData(byte cbCardData[], byte cbMaxCount)
{
	//混乱准备
	byte cbCardDataTemp[CountArray(m_cbCardDataArray_HZ)];
	memcpy(cbCardDataTemp, m_cbCardDataArray_HZ, sizeof(m_cbCardDataArray_HZ));

	//混乱扑克
	byte cbRandCount = 0, cbPosition = 0;
	do
	{
		cbPosition = rand() % (cbMaxCount - cbRandCount);
		cbCardData[cbRandCount++] = cbCardDataTemp[cbPosition];
		cbCardDataTemp[cbPosition] = cbCardDataTemp[cbMaxCount - cbRandCount];
	} while (cbRandCount<cbMaxCount);

	return;
}

//删除扑克
bool MjLogic::RemoveCard(byte cbCardIndex[MAX_INDEX], byte cbRemoveCard)
{
	//效验扑克
	assert(IsValidCard(cbRemoveCard));
	assert(cbCardIndex[SwitchToCardIndex(cbRemoveCard)]>0);

	//删除扑克
	byte cbRemoveIndex = SwitchToCardIndex(cbRemoveCard);
	if (cbCardIndex[cbRemoveIndex]>0)
	{
		cbCardIndex[cbRemoveIndex]--;
		return true;
	}

	//失败效验
	assert(false);

	return false;
}

//删除扑克
bool MjLogic::RemoveCard(byte cbCardIndex[MAX_INDEX], const byte cbRemoveCard[], byte cbRemoveCount)
{
	//删除扑克
	for (byte i = 0; i<cbRemoveCount; i++)
	{
		//效验扑克
		assert(IsValidCard(cbRemoveCard[i]));
		assert(cbCardIndex[SwitchToCardIndex(cbRemoveCard[i])]>0);

		//删除扑克
		byte cbRemoveIndex = SwitchToCardIndex(cbRemoveCard[i]);
		if (cbCardIndex[cbRemoveIndex] == 0)
		{
			//错误断言
			assert(false);

			//还原删除
			for (byte j = 0; j<i; j++)
			{
				assert(IsValidCard(cbRemoveCard[j]));
				cbCardIndex[SwitchToCardIndex(cbRemoveCard[j])]++;
			}

			return false;
		}
		else
		{
			//删除扑克
			--cbCardIndex[cbRemoveIndex];
		}
	}

	return true;
}

byte MjLogic::RemoveValueCardAll(byte cbCardData[], byte cbCardCount, byte cbRemoveCard)
{

	byte cbCardIndex[MAX_INDEX];			//手中扑克
	SwitchToCardIndex(cbCardData, cbCardCount, cbCardIndex);
	byte cbRemoveCardArray[MAX_INDEX];
	byte cbRemoveCout = cbCardIndex[SwitchToCardIndex(cbRemoveCard)];
	for (int i = 0; i<cbRemoveCout; i++)
	{
		cbRemoveCardArray[i] = cbRemoveCard;
	}
	RemoveValueCard(cbCardData, cbCardCount, cbRemoveCardArray, cbRemoveCout);
	return cbCardCount - cbRemoveCout;
}
bool MjLogic::RemoveValueCardOne(byte cbCardData[], byte cbCardCount, byte cbRemoveCard)
{
	byte cbRemoveCardArray[MAX_INDEX];
	cbRemoveCardArray[0] = cbRemoveCard;
	return RemoveValueCard(cbCardData, cbCardCount, cbRemoveCardArray, 1);
}
//删除扑克
bool MjLogic::RemoveValueCard(byte cbCardData[], byte cbCardCount, const byte cbRemoveCard[], byte cbRemoveCount)
{
	//检验数据
	assert(cbCardCount <= 14);
	assert(cbRemoveCount <= cbCardCount);

	//定义变量
	byte cbDeleteCount = 0, cbTempCardData[14];
	if (cbCardCount>CountArray(cbTempCardData))
		return false;
	memcpy(cbTempCardData, cbCardData, cbCardCount*sizeof(cbCardData[0]));

	//置零扑克
	for (byte i = 0; i<cbRemoveCount; i++)
	{
		for (byte j = 0; j<cbCardCount; j++)
		{
			if (cbRemoveCard[i] == cbTempCardData[j])
			{
				cbDeleteCount++;
				cbTempCardData[j] = 0;
				break;
			}
		}
	}

	//成功判断
	if (cbDeleteCount != cbRemoveCount)
	{
		assert(false);
		return false;
	}

	//清理扑克
	byte cbCardPos = 0;
	for (byte i = 0; i<cbCardCount; i++)
	{
		if (cbTempCardData[i] != 0)
			cbCardData[cbCardPos++] = cbTempCardData[i];
	}

	return true;
}

//有效判断
bool MjLogic::IsValidCard(byte cbCardData)
{
	byte cbValue = (cbCardData&MASK_VALUE);
	byte cbColor = (cbCardData&MASK_COLOR) >> 4;
	return (((cbValue >= 1) && (cbValue <= 9) && (cbColor <= 2)) || ((cbValue >= 1) && (cbValue <= 7) && (cbColor == 3)));
}

//扑克数目
byte MjLogic::GetCardCount(const byte cbCardIndex[MAX_INDEX])
{
	//数目统计
	byte cbCardCount = 0;
	for (byte i = 0; i<MAX_INDEX; i++)
		cbCardCount += cbCardIndex[i];

	return cbCardCount;
}

//获取组合
byte MjLogic::GetWeaveCard(byte cbWeaveKind, byte cbCenterCard, byte cbCardBuffer[4])
{
	//组合扑克
	switch (cbWeaveKind)
	{
	case ACTION_LEFT:		//上牌操作
	{
							//设置变量
							cbCardBuffer[0] = cbCenterCard;
							cbCardBuffer[1] = cbCenterCard + 1;
							cbCardBuffer[2] = cbCenterCard + 2;

							return 3;
	}
	case ACTION_RIGHT:		//上牌操作
	{
							//设置变量
							cbCardBuffer[0] = cbCenterCard;
							cbCardBuffer[1] = cbCenterCard - 1;
							cbCardBuffer[2] = cbCenterCard - 2;

							return 3;
	}
	case ACTION_CENTER:	//上牌操作
	{
							//设置变量
							cbCardBuffer[0] = cbCenterCard;
							cbCardBuffer[1] = cbCenterCard - 1;
							cbCardBuffer[2] = cbCenterCard + 1;

							return 3;
	}
	case ACTION_PENG:		//碰牌操作
	{
							//设置变量
							cbCardBuffer[0] = cbCenterCard;
							cbCardBuffer[1] = cbCenterCard;
							cbCardBuffer[2] = cbCenterCard;

							return 3;
	}
	case ACTION_GANG:		//杠牌操作
	{
							//设置变量
							cbCardBuffer[0] = cbCenterCard;
							cbCardBuffer[1] = cbCenterCard;
							cbCardBuffer[2] = cbCenterCard;
							cbCardBuffer[3] = cbCenterCard;

							return 4;
	}
	default:
	{
			   assert(false);
	}
	}

	return 0;
}

//动作等级
byte MjLogic::GetUserActionRank(byte cbUserAction)
{
	//胡牌等级
	if (cbUserAction&ACTION_CHI_HU) { return 4; }

	//杠牌等级
	if (cbUserAction&ACTION_GANG) { return 3; }

	//碰牌等级
	if (cbUserAction&ACTION_PENG) { return 2; }

	//上牌等级
	if (cbUserAction&(ACTION_RIGHT | ACTION_CENTER | ACTION_LEFT)) { return 1; }

	return 0;
}

//胡牌等级
word MjLogic::GetChiHuActionRank(const CChiHuRight & ChiHuRight)
{
	word wFanShu = 0;
	return wFanShu;
}

//吃牌判断
byte MjLogic::EstimateEatCard(const byte cbCardIndex[MAX_INDEX], byte cbCurrentCard)
{
	//参数效验
	assert(IsValidCard(cbCurrentCard));

	//过滤判断
	if (cbCurrentCard >= 0x31 || IsMagicCard(cbCurrentCard))
		return ACTION_NULL;

	//变量定义
	byte cbExcursion[3] = { 0, 1, 2 };
	byte cbItemKind[3] = { ACTION_LEFT, ACTION_CENTER, ACTION_RIGHT };

	//吃牌判断
	byte cbEatKind = 0, cbFirstIndex = 0;
	byte cbCurrentIndex = SwitchToCardIndex(cbCurrentCard);
	for (byte i = 0; i<CountArray(cbItemKind); i++)
	{
		byte cbValueIndex = cbCurrentIndex % 9;
		if ((cbValueIndex >= cbExcursion[i]) && ((cbValueIndex - cbExcursion[i]) <= 6))
		{
			//吃牌判断
			cbFirstIndex = cbCurrentIndex - cbExcursion[i];

			//吃牌不能包含有王霸
			if (m_cbMagicIndex != MAX_INDEX &&
				m_cbMagicIndex >= cbFirstIndex && m_cbMagicIndex <= cbFirstIndex + 2) continue;

			if ((cbCurrentIndex != cbFirstIndex) && (cbCardIndex[cbFirstIndex] == 0))
				continue;
			if ((cbCurrentIndex != (cbFirstIndex + 1)) && (cbCardIndex[cbFirstIndex + 1] == 0))
				continue;
			if ((cbCurrentIndex != (cbFirstIndex + 2)) && (cbCardIndex[cbFirstIndex + 2] == 0))
				continue;

			//设置类型
			cbEatKind |= cbItemKind[i];
		}
	}

	return cbEatKind;
}

//碰牌判断
byte MjLogic::EstimatePengCard(const byte cbCardIndex[MAX_INDEX], byte cbCurrentCard)
{
	//参数效验
	assert(IsValidCard(cbCurrentCard));

	//过滤判断
	if (IsMagicCard(cbCurrentCard))
		return ACTION_NULL;

	//碰牌判断
	return (cbCardIndex[SwitchToCardIndex(cbCurrentCard)] >= 2) ? ACTION_PENG : ACTION_NULL;
}

//杠牌判断
byte MjLogic::EstimateGangCard(const byte cbCardIndex[MAX_INDEX], byte cbCurrentCard)
{
	//参数效验
	assert(IsValidCard(cbCurrentCard));

	//过滤判断
	if (IsMagicCard(cbCurrentCard))
		return ACTION_NULL;

	//杠牌判断
	return (cbCardIndex[SwitchToCardIndex(cbCurrentCard)] == 3) ? ACTION_GANG : ACTION_NULL;
}

//杠牌分析
byte MjLogic::AnalyseGangCard(const byte cbCardIndex[MAX_INDEX], const T_WeaveItem WeaveItem[], byte cbWeaveCount, tagGangCardResult & GangCardResult)
{
	//设置变量
	byte cbActionMask = ACTION_NULL;
	zeromemory(&GangCardResult, sizeof(GangCardResult));

	//手上杠牌
	for (byte i = 0; i<MAX_INDEX; i++)
	{
		if (i == m_cbMagicIndex) continue;
		if (cbCardIndex[i] == 4)
		{
			cbActionMask |= ACTION_GANG;
			GangCardResult.cbCardData[GangCardResult.cbCardCount++] = SwitchToCardData(i);
		}
	}

	//组合杠牌
	for (byte i = 0; i<cbWeaveCount; i++)
	{
		if (WeaveItem[i].cbWeaveKind == ACTION_PENG)
		{
			if (cbCardIndex[SwitchToCardIndex(WeaveItem[i].cbCenterCard)] == 1)
			{
				cbActionMask |= ACTION_GANG;
				GangCardResult.cbCardData[GangCardResult.cbCardCount++] = WeaveItem[i].cbCenterCard;
			}
		}
	}

	return cbActionMask;
}




//扑克转换
byte MjLogic::SwitchToCardData(byte cbCardIndex)
{
	assert(cbCardIndex<34);
	return ((cbCardIndex / 9) << 4) | (cbCardIndex % 9 + 1);
}

//扑克转换
byte MjLogic::SwitchToCardIndex(byte cbCardData)
{
	assert(IsValidCard(cbCardData));
	return ((cbCardData&MASK_COLOR) >> 4) * 9 + (cbCardData&MASK_VALUE) - 1;
}

//扑克转换
byte MjLogic::SwitchToCardData(const byte cbCardIndex[MAX_INDEX], byte cbCardData[MAX_HAND_COUNT])
{
	//转换扑克
	byte cbPosition = 0;
	//钻牌
	if (m_cbMagicIndex != MAX_INDEX)
	{
		for (byte i = 0; i < cbCardIndex[m_cbMagicIndex]; i++)
			cbCardData[cbPosition++] = SwitchToCardData(m_cbMagicIndex);
	}
	for (byte i = 0; i<MAX_INDEX; i++)
	{
		if (i == m_cbMagicIndex) continue;
		if (cbCardIndex[i] != 0)
		{
			for (byte j = 0; j<cbCardIndex[i]; j++)
			{
				assert(cbPosition<MAX_HAND_COUNT);
				cbCardData[cbPosition++] = SwitchToCardData(i);
			}
		}
	}

	return cbPosition;
}

//扑克转换
byte MjLogic::SwitchToCardIndex(const byte cbCardData[], byte cbCardCount, byte cbCardIndex[MAX_INDEX])
{
	//设置变量
	zeromemory(cbCardIndex, sizeof(byte)*MAX_INDEX);

	//转换扑克
	for (byte i = 0; i<cbCardCount; i++)
	{
		assert(IsValidCard(cbCardData[i]));
		cbCardIndex[SwitchToCardIndex(cbCardData[i])]++;
	}

	return cbCardCount;
}

//钻牌
bool MjLogic::IsMagicCard(byte cbCardData)
{
	if (m_cbMagicIndex != MAX_INDEX)
		return SwitchToCardIndex(cbCardData) == m_cbMagicIndex;
	return false;
}

//排序,根据牌值排序
bool MjLogic::SortCardList(byte cbCardData[MAX_HAND_COUNT], byte cbCardCount)
{
	//数目过虑
	if (cbCardCount == 0 || cbCardCount>MAX_HAND_COUNT) return false;

	//排序操作
	bool bSorted = true;
	byte cbSwitchData = 0, cbLast = cbCardCount - 1;
	do
	{
		bSorted = true;
		for (byte i = 0; i<cbLast; i++)
		{
			if (cbCardData[i]>cbCardData[i + 1])
			{
				//设置标志
				bSorted = false;

				//扑克数据
				cbSwitchData = cbCardData[i];
				cbCardData[i] = cbCardData[i + 1];
				cbCardData[i + 1] = cbSwitchData;
			}
		}
		cbLast--;
	} while (bSorted == false);

	return true;
}

/*
// 胡法分析函数
*/

//大对子
bool MjLogic::IsPengPeng(const tagAnalyseItem *pAnalyseItem)
{
	for (byte i = 0; i < CountArray(pAnalyseItem->cbWeaveKind); i++)
	{
		if (pAnalyseItem->cbWeaveKind[i] & (ACTION_LEFT | ACTION_CENTER | ACTION_RIGHT))
			return false;
	}
	return true;
}

//清一色牌
bool MjLogic::IsQingYiSe(const byte cbCardIndex[MAX_INDEX], const T_WeaveItem WeaveItem[], const byte cbItemCount, const byte cbCurrentCard)
{
	//胡牌判断
	byte cbCardColor = 0xFF;

	for (byte i = 0; i<MAX_INDEX; i++)
	{
		if (i == m_cbMagicIndex) continue;
		if (cbCardIndex[i] != 0)
		{
			//花色判断
			if (cbCardColor != 0xFF)
				return false;

			//设置花色
			cbCardColor = (SwitchToCardData(i)&MASK_COLOR);

			//设置索引
			i = (i / 9 + 1) * 9 - 1;
		}
	}

	//如果手上只有王霸
	if (cbCardColor == 0xFF)
	{
		assert(m_cbMagicIndex != MAX_INDEX && cbCardIndex[m_cbMagicIndex] > 0);
		//检查组合
		assert(cbItemCount > 0);
		cbCardColor = WeaveItem[0].cbCenterCard&MASK_COLOR;
	}

	if ((cbCurrentCard&MASK_COLOR) != cbCardColor && !IsMagicCard(cbCurrentCard)) return false;

	//组合判断
	for (byte i = 0; i<cbItemCount; i++)
	{
		byte cbCenterCard = WeaveItem[i].cbCenterCard;
		if ((cbCenterCard&MASK_COLOR) != cbCardColor)	return false;
	}

	return true;
}

//七小对牌
bool MjLogic::IsQiXiaoDui(const byte cbCardIndex[MAX_INDEX], const T_WeaveItem WeaveItem[], const byte cbWeaveCount, const byte cbCurrentCard)
{
	//组合判断
	if (cbWeaveCount != 0) return false;

	//单牌数目
	byte cbReplaceCount = 0;

	//临时数据
	byte cbCardIndexTemp[MAX_INDEX];
	memcpy(cbCardIndexTemp, cbCardIndex, sizeof(cbCardIndexTemp));

	//插入数据
	byte cbCurrentIndex = SwitchToCardIndex(cbCurrentCard);
	cbCardIndexTemp[cbCurrentIndex]++;

	//计算单牌
	for (byte i = 0; i<MAX_INDEX; i++)
	{
		byte cbCardCount = cbCardIndexTemp[i];

		//王牌过滤
		if (i == m_cbMagicIndex) continue;

		//单牌统计
		if (cbCardCount == 1 || cbCardCount == 3) 	cbReplaceCount++;
	}

	//王牌不够
	if (m_cbMagicIndex != MAX_INDEX && cbReplaceCount > cbCardIndexTemp[m_cbMagicIndex] ||
		m_cbMagicIndex == MAX_INDEX && cbReplaceCount > 0)
		return false;

	return true;

}

//带幺
bool MjLogic::IsDaiYao(const tagAnalyseItem *pAnalyseItem)
{
	//检查牌眼
	byte cbCardValue = pAnalyseItem->cbCardEye&MASK_VALUE;
	if (cbCardValue != 1 && cbCardValue != 9) return false;

	for (byte i = 0; i < CountArray(pAnalyseItem->cbWeaveKind); i++)
	{
		if (pAnalyseItem->cbWeaveKind[i] & (ACTION_LEFT | ACTION_CENTER | ACTION_RIGHT))
		{
			byte j = 0;
			for (; j < 3; j++)
			{
				cbCardValue = pAnalyseItem->cbCardData[i][j] & MASK_VALUE;
				if (cbCardValue == 1 || cbCardValue == 9) break;
			}
			if (j == 3) return false;
		}
		else
		{
			cbCardValue = pAnalyseItem->cbCenterCard[i] & MASK_VALUE;
			if (cbCardValue != 1 && cbCardValue != 9) return false;
		}
	}
	return true;
}

//将对
bool MjLogic::IsJiangDui(const tagAnalyseItem *pAnalyseItem)
{
	//是否大对子
	if (!IsPengPeng(pAnalyseItem)) return false;

	//检查牌眼
	byte cbCardValue = pAnalyseItem->cbCardEye&MASK_VALUE;
	if (cbCardValue != 2 && cbCardValue != 5 && cbCardValue != 8) return false;

	for (byte i = 0; i < CountArray(pAnalyseItem->cbWeaveKind); i++)
	{
		if (pAnalyseItem->cbWeaveKind[i] & (ACTION_LEFT | ACTION_CENTER | ACTION_RIGHT))
		{
			byte j = 0;
			for (; j < 3; j++)
			{
				cbCardValue = pAnalyseItem->cbCardData[i][j] & MASK_VALUE;
				if (cbCardValue == 2 || cbCardValue == 5 || cbCardValue == 8) break;
			}
			if (j == 3) return false;
		}
		else
		{
			cbCardValue = pAnalyseItem->cbCenterCard[i] & MASK_VALUE;
			if (cbCardValue != 2 && cbCardValue != 5 && cbCardValue != 8) return false;
		}
	}
	return true;
}

//是否花猪
bool MjLogic::IsHuaZhu(const byte cbCardIndex[], const T_WeaveItem WeaveItem[], byte cbWeaveCount)
{
	byte cbColor[3] = { 0, 0, 0 };
	for (byte i = 0; i < MAX_INDEX; i++)
	{
		if (cbCardIndex[i] > 0)
		{
			byte cbCardColor = SwitchToCardData(i)&MASK_COLOR;
			cbColor[cbCardColor >> 4]++;

			i = (i / 9 + 1) * 9 - 1;
		}
	}
	for (byte i = 0; i < cbWeaveCount; i++)
	{
		byte cbCardColor = WeaveItem[i].cbCenterCard&MASK_COLOR;
		cbColor[cbCardColor >> 4]++;
	}
	//缺一门就不是花猪
	for (byte i = 0; i < CountArray(cbColor); i++)
	if (cbColor[i] == 0) return false;

	return true;
}

byte MjLogic::GetCardColor(byte cbCardDat)
{
	assert(IsValidCard(cbCardDat));
	return ((cbCardDat&MASK_COLOR) >> 4) + 1;
}

byte MjLogic::GetCardValue(byte cbCardDat)
{
	assert(IsValidCard(cbCardDat));
	return (cbCardDat&MASK_VALUE);
}


//////////////////////////////////////////////////////////////////////////