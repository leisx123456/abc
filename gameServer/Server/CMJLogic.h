#ifndef CMJLOGIC_H
#define CMJLOGIC_H
#include "MjCommonDef.h"
#include "UpgradeMessage.h"
class CMJLogic
{
public:
//-----------------------------------麻将子曾删改查操作方法---------------------------------------------
	//函数说明：判断麻将值是否非法
	//参数说明：nCard 麻将值
	//返回值：true为合法，false为非法
	static bool IsValidCard(int nCard);

	//函数说明：排列麻将值数组
	//参数说明：array麻将值数组，node_num 麻将值数组大小
	//返回值：无
	static void sort(int array[], int node_num);

	//函数说明：从麻将数组里面的删除指定值
	//参数说明：array麻将值数组，node_num 麻将值数组大小，mj为指定要删除麻将值
	//返回值：true为成功，false为失败
	static bool RemoveCard(int array[], int node_num, int mj);


	//函数说明：从麻将数组中删除指定麻将数组
	//参数说明：array源数组，nSize源数组大小,delCards要删除的麻将数组,nDelSize要删除的麻将数组的大小
	//返回值：true为成功，false为失败
	static bool ReomveCards(int array[],int& nSize,int delCards[],int nDelSize);

	//函数说明：判断麻将值在麻将数组里面的索引值
	//参数说明：array源数组，nSize源数组大小,mj为要查找的值
	//返回值：-1 表示不存在该麻将值，否则为索引位置
	static int FindCardIndex(int const array[], int size, int mj);

	//函数说明：从麻将数组中，获取某一麻将值得具体个数
	//参数说明：array源数组，nSize源数组大小,mj为要查找的值
	//返回值：麻将值得具体个数
	static int GetCardNum(int const array[], int size, int mj);

	//函数说明：获取麻将子类型
	//参数说明：nCard 麻将值
	//返回值：-1表示类型非法
	static int GetCardKind(int nCard);

	//函数说明：麻将数组中，是否存在某指定麻将值
	//参数说明：array源数组，nSize源数组大小,mj为要查找的值
	//返回值：true为成功，false为失败
	static bool IsCardInArray(int const array[], int size, int mj);

	//函数说明：麻将数组中，是否存在某指定麻将数组
	//参数说明：array源数组，nSize源数组大小，cards为要查找的麻将数组，nCardsNum为查找麻将数组大小。
	//返回值：true为成功，false为失败
	static bool IsCardsInArray(int const array[],int size, int const cards[],int nCardsNum);
//-------------------------------------------------------------end----------------------------------------------------


//----------------------------------------------------------麻将胡牌类型和洗牌算法---------------------------------------

	//函数说明：麻将洗牌算法
	//参数说明：array为保存麻将子的数组，nSize 洗多少张牌，nRandType洗牌类型(需要带入那些麻将子)
	//返回值：true为合法，false为非法
	static void ShuffleCards(int array[], int nSize, int nRandType);


	static int GetHuQidui(int const array[], int iNums, const TMjCGPNode lstCGP[], int iCGPNums, int nBaoValue,int nBaoNums);

	//判断是否是风一色，风一色全部都由风组成的
	static bool IsHuFengYiSe(int const array[], int iNums,const TMjCGPNode lstCGP[], int iCGPNums,int nBaoValue);

	static bool IsHuJiangYiSe(int const array[], int iNums, const TMjCGPNode lstCGP[], int iCGPNums, int nBaoValue);

	//判断是否清一色，由一种花色的序数牌组成和各牌，无字
	static bool IsHuQingYiSe(int const array[],int iNums,const TMjCGPNode lstCGP[],int iCGPNums, int nBaoValue,int nBaoNums);

	//判断是否碰碰胡
	static bool IsHuPengPeng(int const array[],int iNums,const TMjCGPNode lstCGP[],int iCGPNums, int nBaoValue, int nBaoNums);

	static bool IsCanHuQiDui(int const array[],int iNums,const TMjCGPNode lstCGP[],int iCGPNums,int nBaoValue[MJ_MAX_BAO_CARD],int nLaiZiTypeNum);


	static bool IsCanHuShiSanYao(int const array[],int iNums,const TMjCGPNode lstCGP[],int iCGPNums,int nBaoValue[MJ_MAX_BAO_CARD],int nLaiZiTypeNum);

	static bool IsHuYouJin(int const array[],int iNums,const TMjCGPNode lstCGP[],int iCGPNums,int nBaoValue[MJ_MAX_BAO_CARD],int nLaiZiTypeNum,int byFetchCard);
//----------------------------------------------------------end--------------------------------------------------------


//-----------------------------------------------------------麻将动作类型判断----------------------------------------------------
	//函数说明：判断是否达到全是刻字和句子，一对对牌的胡牌条件
	//参数说明：array麻将值数组，node_num 麻将值数组大小,bao_array 癞子牌数组,bao_type_num 多少种癞子,userable_bao_num癞子牌数量,bMaster258Pair是否需要2、5、8做将牌
	//返回值：true为可胡，false为不可胡
	static bool IsCanHu_3x2(int const array[], int node_num, int bao_array[], int bao_type_num ,int useable_bao_num, bool bMaster258Pair);


	static bool IsCanHu(int byOutCard,int byFetchCard,int byCards[],int iNums,const TMjCGPNode lstCGP[],int iCGPNums,TMjActInfo* pActInfo,int iBaoValue, TCHAR* szHuDescr,bool bQiangGang);

	//函数说明：判断麻将值是复合三个刻字或者句子
	//参数说明：array麻将值数组，node_num 麻将值数组大小，bao_num 癞子数量
	//返回值：true为合法，false为非法
	static bool consist_3x(int const array[], int node_num, int bao_num);

	//判断可否吃(没有财神)
	static bool IsCanChi(int nOutCard, const int array[], int iNums,bool bCanChiFeng);

	static bool IsCanChi(BYTE byOutCard, const int byCards[], int iNums, TMjActInfo *pActInfo,bool bCanChiFeng = false);
	
	//检测数组是否为主数组的子数组
	static bool CheckSubArrInside(const int byCards[], int iNums, int byPart[], int iPart);
	//检测数组里面的是否都是麻将值
	static bool CheckArrCards(int byCards[], int iNums);

	

	//判断可否碰
	static bool IsCanPeng(int nOutCard,const int array[],int iNums);

	//判断可否杠
	//static bool IsCanGang(int nCard,const int array[],int iCardNums,const TMjCGPNode lstCGP[],int iCGPNums);

	//判断可否杠
	static bool IsCanGang(int byOutCard,const int byCards[],int iCardNums,\
		const TMjCGPNode lstCGP[],int iCGPNums,TMjActInfo* pActInfo,int byFetchCard);


	//判断可否听
	static bool IsCanTing(const int array[],int iNums,int nBaoValue,bool bMaster258Pair);

	//重新调整风
	static void ResetFeng(int *byCards, int iNums);
	//重新调整中发白
	static void ResetZFB(int *byCards, int iNums);
	///获取下一个玩家的座位号
	static BYTE GetNextUserStation(BYTE byStation,bool bCW,int PlayCount,int iNum = 1);
	//获取一张自动出的牌
	static int getAutoOutCard(const int byCards[],int iNums);
	//一副牌中是否有某张牌上下临近的两张牌
	static bool isHaveNear2(int byCard ,int byCards[],int iNums);
	//获取一个牌的点数
	static int GetDianShu(int byValue);
	//----------------------------------------------------------牌值数据遍历-----------------------------------------------------------------
	///判断该牌值是否有效
	static bool Verify(int byPs);
	//查找源中有有没有指定的牌
	static bool InCards(int byCard,const int bySrc[],int iSrcNums);
     
	static bool InCards(int byCard,const int bySrc[],int iSrcNums,int &nPos);
	///删除一张张牌
	static bool RemoveOneCard(int byCard,int bySrc[],int iSrcNums);

	//删除牌
	static int RemoveCards(const int byDst[],int iDstNums,int bySrc[],int iSrcNums);
	//清除0位牌
	static int Remove0(int byCards[], int iNum);
//--------------------------------------------------------------------end--------------------------------------------------------------------
};
#endif