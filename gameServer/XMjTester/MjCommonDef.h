// ***************************************************************
//  SoundsDef   version:  1.0   ��  date: 7/06/2011
//  -------------------------------------------------------------

//	�齫��Ϸ�������ݶ���,�ʺϴ󲿷��齫��Ϸ����ʹ��\

//  -------------------------------------------------------------
//  Copyright (C) 2011 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************
#pragma once

#ifndef MJCommon_DEF_HEAD_FILE

//��Ϸ���ݳ������

#define MJ_MAX_CARDS				144		//�齫����
#define MJ_MAX_WALLCARD				36		//��ǽ�����ɵ��齫�ؼ���
#define MJ_MAX_WALLDUN				18		//��ǽ�����ɵ��齫����(��ֵ��MJ_MAX_WALLCARD��һ��)
#define MJ_MAX_HANDCARD				14		//���������ɵ��齫�ؼ���
#define MJ_MAX_HUA					24		//���Ƶ�����
#define MJ_MAX_OUTED				18		//���ĳ�������

namespace MjBase
{
	//���ڷ�λ�Ķ���
	namespace MjDirect
	{
		const BYTE East	 = 0;	//��
		const BYTE South = 1;	//��
		const BYTE West	 = 2;	//��
		const BYTE North = 3;	//��
	}
	
	//�����齫��ֵ�Ķ���
	namespace MjKind
	{
		const BYTE Wan	= 0;	//��
		const BYTE Tiao	= 10;	//��
		const BYTE Bing	= 20;	//��
		const BYTE Feng	= 30;	//��
		const BYTE Hua	= 40;	//�� 
		const BYTE Unknown = 255;	//δ֪����
	}

	namespace MjFeng
	{
		const BYTE Dong	 = 1;	//��
		const BYTE Xi	 = 2;	//��
		const BYTE Nan	 = 3;	//��
		const BYTE Bei	 = 4;	//��
		const BYTE Zhong = 5;	//��
		const BYTE Fa	 = 6;	//��
		const BYTE Bai	 = 7;	//��
	}

	namespace MjHua
	{
		const BYTE Chun	= 1;	//��
		const BYTE Xia	= 2;	//��
		const BYTE Qiu	= 3;	//��
		const BYTE Dong	= 4;	//��
		const BYTE Mei	= 5;	//÷
		const BYTE Lan	= 6;	//��
		const BYTE Zhu	= 7;	//��
		const BYTE Ju	= 8;	//��
	}

	//ͨ����ֵ�����Ƶ�����
	static BYTE GetKind(BYTE byPs)
	{
		if (byPs >= 1 && byPs <= 9)
		{
			return MjKind::Wan;
		}
		else if (byPs >= 11 && byPs <= 19)
		{
			return MjKind::Tiao;
		}
		else if (byPs >= 21 && byPs <= 29)
		{
			return MjKind::Bing;
		}
		else if (byPs >= 31 && byPs <= 37)
		{
			return MjKind::Feng;
		}
		else if (byPs >= 41 && byPs <= 48)
		{
			return MjKind::Hua;
		}

		return MjKind::Unknown;
	}

	//�ж�һ�����Ƿ���Ч
	static bool Verify(BYTE byPs)
	{
		return (MjKind::Unknown != GetKind(byPs));
	}

	//ͨ����ֵ�����Ƶĵ���
	static BYTE GetDian(BYTE byPs)
	{
		return byPs % 10;
	}

	//ȡ�Ƶ���Ϣ������ǽȡ�ƺ󣬽��鷵������ṹ��
	struct TMjFetch
	{
		BYTE val;	//��ֵ
		BYTE from;	//���Ǹ������ǰ���Ƴ�ȡ
		BYTE x;
		BYTE y;
		BYTE left; //��ǽʣ�������

		TMjFetch()
		{
			::memset(this,0,sizeof(TMjFetch));
		}
	};
}


#define MJCommon_DEF_HEAD_FILE
#endif