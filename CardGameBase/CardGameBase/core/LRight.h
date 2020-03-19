#ifndef _L_RIGHT_H_
#define _L_RIGHT_H_
/******************************************************************************
**
** @file
** Card.h
** @brief
**
** @par
** 未处理C++异常
** @author
** leisx
** @date
** 2017-5-4
**
******************************************************************************/

#define MASK_CHI_HU_RIGHT			0x0fffffff

/*
//	权位类。
//  注意，在操作仅位时最好只操作单个权位.例如
//  CChiHuRight chr;
//  chr |= (chr_zi_mo|chr_peng_peng)，这样结果是无定义的。
//  只能单个操作:
//  chr |= chr_zi_mo;
//  chr |= chr_peng_peng;
*/

//#include <Windows.h>


class CChiHuRight
{


public:
	CChiHuRight();
	~CChiHuRight();
public:
	CChiHuRight & operator = (unsigned long dwRight);
	CChiHuRight & operator &= (unsigned long dwRight);
	CChiHuRight & operator |= (unsigned long dwRight);
	CChiHuRight operator & (unsigned long dwRight);
	CChiHuRight operator & (unsigned long dwRight) const;
	CChiHuRight operator | (unsigned long dwRight);
	CChiHuRight operator | (unsigned long dwRight) const;


public:
	//是否权位为空
	bool isEmpty();

	//设置权位为空
	void setEmpty();

	//获取权位数值
	unsigned char getRightData(unsigned long dwRight[], unsigned char cbMaxCount);

	//设置权位数值
	bool setRightData(const unsigned long dwRight[], unsigned char cbRightCount);

private:
	//检查权位是否正确
	bool isValidRight(unsigned long dwRight);


private:
	 bool m_bInit;
	unsigned long *m_pRightMask;
	unsigned long *m_pRight;
	unsigned int m_unMaxRightCount;

};


#endif //_L_RIGHT_H_