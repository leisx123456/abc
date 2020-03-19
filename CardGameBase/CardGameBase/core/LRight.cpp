#include "LRight.h"
#include <iostream>
#include <assert.h>


//构造函数
CChiHuRight::CChiHuRight()
{
	m_pRightMask = new unsigned long[m_unMaxRightCount];
	m_pRight = new unsigned long[m_unMaxRightCount];

	memset(m_pRight, 0, sizeof(m_pRight));

	if (!m_bInit)
	{
		m_bInit = true;
		for (unsigned char i = 0; i < m_unMaxRightCount; ++i)
		{
			if (0 == i)
				m_pRightMask[i] = 0;
			else
				m_pRightMask[i] = (unsigned long(pow(2, i - 1))) << 28;
		}
	}
}


CChiHuRight::~CChiHuRight()
{
	delete[] m_pRightMask;
	delete[] m_pRight;
}


//赋值符重载
CChiHuRight & CChiHuRight::operator = (unsigned long dwRight)
{
	unsigned long dwOtherRight = 0;
	//验证权位
	if (!isValidRight(dwRight))
	{
		//验证取反权位
		assert(isValidRight(~dwRight));
		if (!isValidRight(~dwRight)) return *this;
		dwRight = ~dwRight;
		dwOtherRight = MASK_CHI_HU_RIGHT;
	}

	for (unsigned char i = 0; i < m_unMaxRightCount; i++)
	{
		if ((dwRight & m_pRightMask[i]) || (i == 0 && dwRight < 0x10000000))
			m_pRight[i] = dwRight & MASK_CHI_HU_RIGHT;
		else m_pRight[i] = dwOtherRight;
	}

	return *this;
}

//与等于
CChiHuRight & CChiHuRight::operator &= (unsigned long dwRight)
{
	bool bNavigate = false;
	//验证权位
	if (!isValidRight(dwRight))
	{
		//验证取反权位
		assert(isValidRight(~dwRight));
		if (!isValidRight(~dwRight)) return *this;
		//调整权位
		unsigned long dwHeadRight = (~dwRight) & 0xF0000000;
		unsigned long dwTailRight = dwRight&MASK_CHI_HU_RIGHT;
		dwRight = dwHeadRight | dwTailRight;
		bNavigate = true;
	}

	for (unsigned char i = 0; i < m_unMaxRightCount; i++)
	{
		if ((dwRight&m_pRightMask[i]) || (i == 0 && dwRight < 0x10000000))
		{
			m_pRight[i] &= (dwRight&MASK_CHI_HU_RIGHT);
		}
		else if (!bNavigate)
			m_pRight[i] = 0;
	}

	return *this;
}

//或等于
CChiHuRight & CChiHuRight::operator |= (unsigned long dwRight)
{
	//验证权位
	if (!isValidRight(dwRight)) return *this;

	for (unsigned char i = 0; i < m_unMaxRightCount; i++)
	{
		if ((dwRight&m_pRightMask[i]) || (i == 0 && dwRight < 0x10000000))
			m_pRight[i] |= (dwRight&MASK_CHI_HU_RIGHT);
	}

	return *this;
}

//与
CChiHuRight CChiHuRight::operator & (unsigned long dwRight)
{
	CChiHuRight chr = *this;
	return (chr &= dwRight);
}

//与
CChiHuRight CChiHuRight::operator & (unsigned long dwRight) const
{
	CChiHuRight chr = *this;
	return (chr &= dwRight);
}

//或
CChiHuRight CChiHuRight::operator | (unsigned long dwRight)
{
	CChiHuRight chr = *this;
	return chr |= dwRight;
}

//或
CChiHuRight CChiHuRight::operator | (unsigned long dwRight) const
{
	CChiHuRight chr = *this;
	return chr |= dwRight;
}

bool CChiHuRight::isEmpty()
{
	for (unsigned char i = 0; i < m_unMaxRightCount; ++i)
	{
		if (m_pRight[i])
		{
			return false;
		}
	}
	return true;
}

void CChiHuRight::setEmpty()
{
	memset(m_pRight, 0, sizeof(m_pRight));
}

//获取权位数值
unsigned char CChiHuRight::getRightData(unsigned long dwRight[], unsigned char cbMaxCount)
{
	assert(cbMaxCount >= m_unMaxRightCount);
	if (cbMaxCount < m_unMaxRightCount) return 0;

	memcpy(dwRight, m_pRight, sizeof(unsigned long)* m_unMaxRightCount);
	return m_unMaxRightCount;
}

//设置权位数值
bool CChiHuRight::setRightData(const unsigned long dwRight[], unsigned char cbRightCount)
{
	assert(cbRightCount <= m_unMaxRightCount);
	if (cbRightCount > m_unMaxRightCount) return false;

	memset(m_pRight, 0, sizeof(m_pRight));
	memcpy(m_pRight, dwRight, sizeof(unsigned long)*cbRightCount);

	return true;
}

//检查仅位是否正确
bool CChiHuRight::isValidRight(unsigned long dwRight)
{
	unsigned long dwRightHead = dwRight & 0xF0000000;
	for (unsigned char i = 0; i < m_unMaxRightCount; i++)
	if (m_pRightMask[i] == dwRightHead) return true;
	return false;
}