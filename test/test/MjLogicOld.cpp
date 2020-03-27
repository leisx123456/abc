
#include "MjLogic.h"
#include "math.h"
#include <assert.h>


//////////////////////////////////////////////////////////////////////////

//��̬����
bool		CChiHuRight::m_bInit = false;
dword		CChiHuRight::m_dwRightMask[MAX_RIGHT_COUNT];

//���캯��
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

//��ֵ������
CChiHuRight & CChiHuRight::operator = (dword dwRight)
{
	dword dwOtherRight = 0;
	//��֤Ȩλ
	if (!IsValidRight(dwRight))
	{
		//��֤ȡ��Ȩλ
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

//�����
CChiHuRight & CChiHuRight::operator &= (dword dwRight)
{
	bool bNavigate = false;
	//��֤Ȩλ
	if (!IsValidRight(dwRight))
	{
		//��֤ȡ��Ȩλ
		assert(IsValidRight(~dwRight));
		if (!IsValidRight(~dwRight)) return *this;
		//����Ȩλ
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

//�����
CChiHuRight & CChiHuRight::operator |= (dword dwRight)
{
	//��֤Ȩλ
	if (!IsValidRight(dwRight)) return *this;

	for (byte i = 0; i < CountArray(m_dwRightMask); i++)
	{
		if ((dwRight&m_dwRightMask[i]) || (i == 0 && dwRight < 0x10000000))
			m_dwRight[i] |= (dwRight&MASK_CHI_HU_RIGHT);
	}

	return *this;
}

//��
CChiHuRight CChiHuRight::operator & (dword dwRight)
{
	CChiHuRight chr = *this;
	return (chr &= dwRight);
}

//��
CChiHuRight CChiHuRight::operator & (dword dwRight) const
{
	CChiHuRight chr = *this;
	return (chr &= dwRight);
}

//��
CChiHuRight CChiHuRight::operator | (dword dwRight)
{
	CChiHuRight chr = *this;
	return chr |= dwRight;
}

//��
CChiHuRight CChiHuRight::operator | (dword dwRight) const
{
	CChiHuRight chr = *this;
	return chr |= dwRight;
}

//�Ƿ�ȨλΪ��
bool CChiHuRight::IsEmpty()
{
	for (byte i = 0; i < CountArray(m_dwRight); i++)
	if (m_dwRight[i]) return false;
	return true;
}

//����ȨλΪ��
void CChiHuRight::SetEmpty()
{
	zeromemory(m_dwRight, sizeof(m_dwRight));
	return;
}

//��ȡȨλ��ֵ
byte CChiHuRight::GetRightData(dword dwRight[], byte cbMaxCount)
{
	assert(cbMaxCount >= CountArray(m_dwRight));
	if (cbMaxCount < CountArray(m_dwRight)) return 0;

	memcpy(dwRight, m_dwRight, sizeof(dword)*CountArray(m_dwRight));
	return CountArray(m_dwRight);
}

//����Ȩλ��ֵ
bool CChiHuRight::SetRightData(const dword dwRight[], byte cbRightCount)
{
	assert(cbRightCount <= CountArray(m_dwRight));
	if (cbRightCount > CountArray(m_dwRight)) return false;

	zeromemory(m_dwRight, sizeof(m_dwRight));
	memcpy(m_dwRight, dwRight, sizeof(dword)*cbRightCount);

	return true;
}

//����λ�Ƿ���ȷ
bool CChiHuRight::IsValidRight(dword dwRight)
{
	dword dwRightHead = dwRight & 0xF0000000;
	for (byte i = 0; i < CountArray(m_dwRightMask); i++)
	if (m_dwRightMask[i] == dwRightHead) return true;
	return false;
}

//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//��̬����

//�˿�����
//GYZJ
const byte MjLogic::m_cbCardDataArray_HZ[CARD_TOTAL_NUM] =
{
	0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09,						//����
	0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09,						//����
	0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09,						//����
	0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09,						//����
	0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19,						//����
	0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19,						//����
	0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19,						//����
	0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19,						//����
	0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29,						//ͬ��
	0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29,						//ͬ��
	0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29,						//ͬ��
	0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29,						//ͬ��
	/*0x35,0x35,0x35,0x35,
	0x36,0x36,0x36,0x36,
	0x37,0x37,0x37,0x37*/
};

//////////////////////////////////////////////////////////////////////////

//���캯��
MjLogic::MjLogic()
{
	m_cbMagicIndex = MAX_INDEX;
}

//��������
MjLogic::~MjLogic()
{
}

//�����˿�
void MjLogic::RandCardData(byte cbCardData[], byte cbMaxCount)
{
	//����׼��
	byte cbCardDataTemp[CountArray(m_cbCardDataArray_HZ)];
	memcpy(cbCardDataTemp, m_cbCardDataArray_HZ, sizeof(m_cbCardDataArray_HZ));

	//�����˿�
	byte cbRandCount = 0, cbPosition = 0;
	do
	{
		cbPosition = rand() % (cbMaxCount - cbRandCount);
		cbCardData[cbRandCount++] = cbCardDataTemp[cbPosition];
		cbCardDataTemp[cbPosition] = cbCardDataTemp[cbMaxCount - cbRandCount];
	} while (cbRandCount<cbMaxCount);

	return;
}

//ɾ���˿�
bool MjLogic::RemoveCard(byte cbCardIndex[MAX_INDEX], byte cbRemoveCard)
{
	//Ч���˿�
	assert(IsValidCard(cbRemoveCard));
	assert(cbCardIndex[SwitchToCardIndex(cbRemoveCard)]>0);

	//ɾ���˿�
	byte cbRemoveIndex = SwitchToCardIndex(cbRemoveCard);
	if (cbCardIndex[cbRemoveIndex]>0)
	{
		cbCardIndex[cbRemoveIndex]--;
		return true;
	}

	//ʧ��Ч��
	assert(false);

	return false;
}

//ɾ���˿�
bool MjLogic::RemoveCard(byte cbCardIndex[MAX_INDEX], const byte cbRemoveCard[], byte cbRemoveCount)
{
	//ɾ���˿�
	for (byte i = 0; i<cbRemoveCount; i++)
	{
		//Ч���˿�
		assert(IsValidCard(cbRemoveCard[i]));
		assert(cbCardIndex[SwitchToCardIndex(cbRemoveCard[i])]>0);

		//ɾ���˿�
		byte cbRemoveIndex = SwitchToCardIndex(cbRemoveCard[i]);
		if (cbCardIndex[cbRemoveIndex] == 0)
		{
			//�������
			assert(false);

			//��ԭɾ��
			for (byte j = 0; j<i; j++)
			{
				assert(IsValidCard(cbRemoveCard[j]));
				cbCardIndex[SwitchToCardIndex(cbRemoveCard[j])]++;
			}

			return false;
		}
		else
		{
			//ɾ���˿�
			--cbCardIndex[cbRemoveIndex];
		}
	}

	return true;
}

byte MjLogic::RemoveValueCardAll(byte cbCardData[], byte cbCardCount, byte cbRemoveCard)
{

	byte cbCardIndex[MAX_INDEX];			//�����˿�
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
//ɾ���˿�
bool MjLogic::RemoveValueCard(byte cbCardData[], byte cbCardCount, const byte cbRemoveCard[], byte cbRemoveCount)
{
	//��������
	assert(cbCardCount <= 14);
	assert(cbRemoveCount <= cbCardCount);

	//�������
	byte cbDeleteCount = 0, cbTempCardData[14];
	if (cbCardCount>CountArray(cbTempCardData))
		return false;
	memcpy(cbTempCardData, cbCardData, cbCardCount*sizeof(cbCardData[0]));

	//�����˿�
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

	//�ɹ��ж�
	if (cbDeleteCount != cbRemoveCount)
	{
		assert(false);
		return false;
	}

	//�����˿�
	byte cbCardPos = 0;
	for (byte i = 0; i<cbCardCount; i++)
	{
		if (cbTempCardData[i] != 0)
			cbCardData[cbCardPos++] = cbTempCardData[i];
	}

	return true;
}

//��Ч�ж�
bool MjLogic::IsValidCard(byte cbCardData)
{
	byte cbValue = (cbCardData&MASK_VALUE);
	byte cbColor = (cbCardData&MASK_COLOR) >> 4;
	return (((cbValue >= 1) && (cbValue <= 9) && (cbColor <= 2)) || ((cbValue >= 1) && (cbValue <= 7) && (cbColor == 3)));
}

//�˿���Ŀ
byte MjLogic::GetCardCount(const byte cbCardIndex[MAX_INDEX])
{
	//��Ŀͳ��
	byte cbCardCount = 0;
	for (byte i = 0; i<MAX_INDEX; i++)
		cbCardCount += cbCardIndex[i];

	return cbCardCount;
}

//��ȡ���
byte MjLogic::GetWeaveCard(byte cbWeaveKind, byte cbCenterCard, byte cbCardBuffer[4])
{
	//����˿�
	switch (cbWeaveKind)
	{
	case ACTION_LEFT:		//���Ʋ���
	{
							//���ñ���
							cbCardBuffer[0] = cbCenterCard;
							cbCardBuffer[1] = cbCenterCard + 1;
							cbCardBuffer[2] = cbCenterCard + 2;

							return 3;
	}
	case ACTION_RIGHT:		//���Ʋ���
	{
							//���ñ���
							cbCardBuffer[0] = cbCenterCard;
							cbCardBuffer[1] = cbCenterCard - 1;
							cbCardBuffer[2] = cbCenterCard - 2;

							return 3;
	}
	case ACTION_CENTER:	//���Ʋ���
	{
							//���ñ���
							cbCardBuffer[0] = cbCenterCard;
							cbCardBuffer[1] = cbCenterCard - 1;
							cbCardBuffer[2] = cbCenterCard + 1;

							return 3;
	}
	case ACTION_PENG:		//���Ʋ���
	{
							//���ñ���
							cbCardBuffer[0] = cbCenterCard;
							cbCardBuffer[1] = cbCenterCard;
							cbCardBuffer[2] = cbCenterCard;

							return 3;
	}
	case ACTION_GANG:		//���Ʋ���
	{
							//���ñ���
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

//�����ȼ�
byte MjLogic::GetUserActionRank(byte cbUserAction)
{
	//���Ƶȼ�
	if (cbUserAction&ACTION_CHI_HU) { return 4; }

	//���Ƶȼ�
	if (cbUserAction&ACTION_GANG) { return 3; }

	//���Ƶȼ�
	if (cbUserAction&ACTION_PENG) { return 2; }

	//���Ƶȼ�
	if (cbUserAction&(ACTION_RIGHT | ACTION_CENTER | ACTION_LEFT)) { return 1; }

	return 0;
}

//���Ƶȼ�
word MjLogic::GetChiHuActionRank(const CChiHuRight & ChiHuRight)
{
	word wFanShu = 0;
	return wFanShu;
}

//�����ж�
byte MjLogic::EstimateEatCard(const byte cbCardIndex[MAX_INDEX], byte cbCurrentCard)
{
	//����Ч��
	assert(IsValidCard(cbCurrentCard));

	//�����ж�
	if (cbCurrentCard >= 0x31 || IsMagicCard(cbCurrentCard))
		return ACTION_NULL;

	//��������
	byte cbExcursion[3] = { 0, 1, 2 };
	byte cbItemKind[3] = { ACTION_LEFT, ACTION_CENTER, ACTION_RIGHT };

	//�����ж�
	byte cbEatKind = 0, cbFirstIndex = 0;
	byte cbCurrentIndex = SwitchToCardIndex(cbCurrentCard);
	for (byte i = 0; i<CountArray(cbItemKind); i++)
	{
		byte cbValueIndex = cbCurrentIndex % 9;
		if ((cbValueIndex >= cbExcursion[i]) && ((cbValueIndex - cbExcursion[i]) <= 6))
		{
			//�����ж�
			cbFirstIndex = cbCurrentIndex - cbExcursion[i];

			//���Ʋ��ܰ���������
			if (m_cbMagicIndex != MAX_INDEX &&
				m_cbMagicIndex >= cbFirstIndex && m_cbMagicIndex <= cbFirstIndex + 2) continue;

			if ((cbCurrentIndex != cbFirstIndex) && (cbCardIndex[cbFirstIndex] == 0))
				continue;
			if ((cbCurrentIndex != (cbFirstIndex + 1)) && (cbCardIndex[cbFirstIndex + 1] == 0))
				continue;
			if ((cbCurrentIndex != (cbFirstIndex + 2)) && (cbCardIndex[cbFirstIndex + 2] == 0))
				continue;

			//��������
			cbEatKind |= cbItemKind[i];
		}
	}

	return cbEatKind;
}

//�����ж�
byte MjLogic::EstimatePengCard(const byte cbCardIndex[MAX_INDEX], byte cbCurrentCard)
{
	//����Ч��
	assert(IsValidCard(cbCurrentCard));

	//�����ж�
	if (IsMagicCard(cbCurrentCard))
		return ACTION_NULL;

	//�����ж�
	return (cbCardIndex[SwitchToCardIndex(cbCurrentCard)] >= 2) ? ACTION_PENG : ACTION_NULL;
}

//�����ж�
byte MjLogic::EstimateGangCard(const byte cbCardIndex[MAX_INDEX], byte cbCurrentCard)
{
	//����Ч��
	assert(IsValidCard(cbCurrentCard));

	//�����ж�
	if (IsMagicCard(cbCurrentCard))
		return ACTION_NULL;

	//�����ж�
	return (cbCardIndex[SwitchToCardIndex(cbCurrentCard)] == 3) ? ACTION_GANG : ACTION_NULL;
}

//���Ʒ���
byte MjLogic::AnalyseGangCard(const byte cbCardIndex[MAX_INDEX], const T_WeaveItem WeaveItem[], byte cbWeaveCount, tagGangCardResult & GangCardResult)
{
	//���ñ���
	byte cbActionMask = ACTION_NULL;
	zeromemory(&GangCardResult, sizeof(GangCardResult));

	//���ϸ���
	for (byte i = 0; i<MAX_INDEX; i++)
	{
		if (i == m_cbMagicIndex) continue;
		if (cbCardIndex[i] == 4)
		{
			cbActionMask |= ACTION_GANG;
			GangCardResult.cbCardData[GangCardResult.cbCardCount++] = SwitchToCardData(i);
		}
	}

	//��ϸ���
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




//�˿�ת��
byte MjLogic::SwitchToCardData(byte cbCardIndex)
{
	assert(cbCardIndex<34);
	return ((cbCardIndex / 9) << 4) | (cbCardIndex % 9 + 1);
}

//�˿�ת��
byte MjLogic::SwitchToCardIndex(byte cbCardData)
{
	assert(IsValidCard(cbCardData));
	return ((cbCardData&MASK_COLOR) >> 4) * 9 + (cbCardData&MASK_VALUE) - 1;
}

//�˿�ת��
byte MjLogic::SwitchToCardData(const byte cbCardIndex[MAX_INDEX], byte cbCardData[MAX_HAND_COUNT])
{
	//ת���˿�
	byte cbPosition = 0;
	//����
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

//�˿�ת��
byte MjLogic::SwitchToCardIndex(const byte cbCardData[], byte cbCardCount, byte cbCardIndex[MAX_INDEX])
{
	//���ñ���
	zeromemory(cbCardIndex, sizeof(byte)*MAX_INDEX);

	//ת���˿�
	for (byte i = 0; i<cbCardCount; i++)
	{
		assert(IsValidCard(cbCardData[i]));
		cbCardIndex[SwitchToCardIndex(cbCardData[i])]++;
	}

	return cbCardCount;
}

//����
bool MjLogic::IsMagicCard(byte cbCardData)
{
	if (m_cbMagicIndex != MAX_INDEX)
		return SwitchToCardIndex(cbCardData) == m_cbMagicIndex;
	return false;
}

//����,������ֵ����
bool MjLogic::SortCardList(byte cbCardData[MAX_HAND_COUNT], byte cbCardCount)
{
	//��Ŀ����
	if (cbCardCount == 0 || cbCardCount>MAX_HAND_COUNT) return false;

	//�������
	bool bSorted = true;
	byte cbSwitchData = 0, cbLast = cbCardCount - 1;
	do
	{
		bSorted = true;
		for (byte i = 0; i<cbLast; i++)
		{
			if (cbCardData[i]>cbCardData[i + 1])
			{
				//���ñ�־
				bSorted = false;

				//�˿�����
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
// ������������
*/

//�����
bool MjLogic::IsPengPeng(const tagAnalyseItem *pAnalyseItem)
{
	for (byte i = 0; i < CountArray(pAnalyseItem->cbWeaveKind); i++)
	{
		if (pAnalyseItem->cbWeaveKind[i] & (ACTION_LEFT | ACTION_CENTER | ACTION_RIGHT))
			return false;
	}
	return true;
}

//��һɫ��
bool MjLogic::IsQingYiSe(const byte cbCardIndex[MAX_INDEX], const T_WeaveItem WeaveItem[], const byte cbItemCount, const byte cbCurrentCard)
{
	//�����ж�
	byte cbCardColor = 0xFF;

	for (byte i = 0; i<MAX_INDEX; i++)
	{
		if (i == m_cbMagicIndex) continue;
		if (cbCardIndex[i] != 0)
		{
			//��ɫ�ж�
			if (cbCardColor != 0xFF)
				return false;

			//���û�ɫ
			cbCardColor = (SwitchToCardData(i)&MASK_COLOR);

			//��������
			i = (i / 9 + 1) * 9 - 1;
		}
	}

	//�������ֻ������
	if (cbCardColor == 0xFF)
	{
		assert(m_cbMagicIndex != MAX_INDEX && cbCardIndex[m_cbMagicIndex] > 0);
		//������
		assert(cbItemCount > 0);
		cbCardColor = WeaveItem[0].cbCenterCard&MASK_COLOR;
	}

	if ((cbCurrentCard&MASK_COLOR) != cbCardColor && !IsMagicCard(cbCurrentCard)) return false;

	//����ж�
	for (byte i = 0; i<cbItemCount; i++)
	{
		byte cbCenterCard = WeaveItem[i].cbCenterCard;
		if ((cbCenterCard&MASK_COLOR) != cbCardColor)	return false;
	}

	return true;
}

//��С����
bool MjLogic::IsQiXiaoDui(const byte cbCardIndex[MAX_INDEX], const T_WeaveItem WeaveItem[], const byte cbWeaveCount, const byte cbCurrentCard)
{
	//����ж�
	if (cbWeaveCount != 0) return false;

	//������Ŀ
	byte cbReplaceCount = 0;

	//��ʱ����
	byte cbCardIndexTemp[MAX_INDEX];
	memcpy(cbCardIndexTemp, cbCardIndex, sizeof(cbCardIndexTemp));

	//��������
	byte cbCurrentIndex = SwitchToCardIndex(cbCurrentCard);
	cbCardIndexTemp[cbCurrentIndex]++;

	//���㵥��
	for (byte i = 0; i<MAX_INDEX; i++)
	{
		byte cbCardCount = cbCardIndexTemp[i];

		//���ƹ���
		if (i == m_cbMagicIndex) continue;

		//����ͳ��
		if (cbCardCount == 1 || cbCardCount == 3) 	cbReplaceCount++;
	}

	//���Ʋ���
	if (m_cbMagicIndex != MAX_INDEX && cbReplaceCount > cbCardIndexTemp[m_cbMagicIndex] ||
		m_cbMagicIndex == MAX_INDEX && cbReplaceCount > 0)
		return false;

	return true;

}

//����
bool MjLogic::IsDaiYao(const tagAnalyseItem *pAnalyseItem)
{
	//�������
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

//����
bool MjLogic::IsJiangDui(const tagAnalyseItem *pAnalyseItem)
{
	//�Ƿ�����
	if (!IsPengPeng(pAnalyseItem)) return false;

	//�������
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

//�Ƿ���
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
	//ȱһ�žͲ��ǻ���
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