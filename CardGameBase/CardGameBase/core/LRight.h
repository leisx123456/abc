#ifndef _L_RIGHT_H_
#define _L_RIGHT_H_
/******************************************************************************
**
** @file
** Card.h
** @brief
**
** @par
** δ����C++�쳣
** @author
** leisx
** @date
** 2017-5-4
**
******************************************************************************/

#define MASK_CHI_HU_RIGHT			0x0fffffff

/*
//	Ȩλ�ࡣ
//  ע�⣬�ڲ�����λʱ���ֻ��������Ȩλ.����
//  CChiHuRight chr;
//  chr |= (chr_zi_mo|chr_peng_peng)������������޶���ġ�
//  ֻ�ܵ�������:
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
	//�Ƿ�ȨλΪ��
	bool isEmpty();

	//����ȨλΪ��
	void setEmpty();

	//��ȡȨλ��ֵ
	unsigned char getRightData(unsigned long dwRight[], unsigned char cbMaxCount);

	//����Ȩλ��ֵ
	bool setRightData(const unsigned long dwRight[], unsigned char cbRightCount);

private:
	//���Ȩλ�Ƿ���ȷ
	bool isValidRight(unsigned long dwRight);


private:
	 bool m_bInit;
	unsigned long *m_pRightMask;
	unsigned long *m_pRight;
	unsigned int m_unMaxRightCount;

};


#endif //_L_RIGHT_H_