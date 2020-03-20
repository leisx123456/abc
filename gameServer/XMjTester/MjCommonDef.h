// ***************************************************************
//  SoundsDef   version:  1.0   ·  date: 7/06/2011
//  -------------------------------------------------------------

//	麻将游戏基本数据定义,适合大部分麻将游戏开发使用\

//  -------------------------------------------------------------
//  Copyright (C) 2011 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************
#pragma once

#ifndef MJCommon_DEF_HEAD_FILE

//游戏数据长度相关

#define MJ_MAX_CARDS				144		//麻将张数
#define MJ_MAX_WALLCARD				36		//牌墙能容纳的麻将控件数
#define MJ_MAX_WALLDUN				18		//牌墙能容纳的麻将墩数(数值是MJ_MAX_WALLCARD的一半)
#define MJ_MAX_HANDCARD				14		//手牌能容纳的麻将控件数
#define MJ_MAX_HUA					24		//花牌的数量
#define MJ_MAX_OUTED				18		//最大的出牌数量

namespace MjBase
{
	//关于风位的定义
	namespace MjDirect
	{
		const BYTE East	 = 0;	//东
		const BYTE South = 1;	//南
		const BYTE West	 = 2;	//西
		const BYTE North = 3;	//北
	}
	
	//关于麻将牌值的定义
	namespace MjKind
	{
		const BYTE Wan	= 0;	//万
		const BYTE Tiao	= 10;	//条
		const BYTE Bing	= 20;	//饼
		const BYTE Feng	= 30;	//风
		const BYTE Hua	= 40;	//花 
		const BYTE Unknown = 255;	//未知花型
	}

	namespace MjFeng
	{
		const BYTE Dong	 = 1;	//东
		const BYTE Xi	 = 2;	//西
		const BYTE Nan	 = 3;	//南
		const BYTE Bei	 = 4;	//北
		const BYTE Zhong = 5;	//中
		const BYTE Fa	 = 6;	//发
		const BYTE Bai	 = 7;	//白
	}

	namespace MjHua
	{
		const BYTE Chun	= 1;	//春
		const BYTE Xia	= 2;	//夏
		const BYTE Qiu	= 3;	//秋
		const BYTE Dong	= 4;	//冬
		const BYTE Mei	= 5;	//梅
		const BYTE Lan	= 6;	//兰
		const BYTE Zhu	= 7;	//竹
		const BYTE Ju	= 8;	//菊
	}

	//通过牌值返回牌的类型
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

	//判断一张牌是否有效
	static bool Verify(BYTE byPs)
	{
		return (MjKind::Unknown != GetKind(byPs));
	}

	//通过牌值返回牌的点数
	static BYTE GetDian(BYTE byPs)
	{
		return byPs % 10;
	}

	//取牌的信息，从牌墙取牌后，建议返回这个结构。
	struct TMjFetch
	{
		BYTE val;	//牌值
		BYTE from;	//从那个玩家门前的牌城取
		BYTE x;
		BYTE y;
		BYTE left; //牌墙剩余的牌数

		TMjFetch()
		{
			::memset(this,0,sizeof(TMjFetch));
		}
	};
}


#define MJCommon_DEF_HEAD_FILE
#endif