#ifndef MJ_LOGIC_FILE
#define MJ_LOGIC_FILE

#include <vector>
#include "MjCard.h"
#include "define/MjDef.h"

//////////////////////////////////////////////////////////////////////////



//扑克定义
#define HEAP_FULL_COUNT				26									//堆立全牌

#define MAX_RIGHT_COUNT				1									//最大权位dword个数		

//类型子项
struct tagKindItem
{
	byte							cbWeaveKind;						//组合类型
	byte							cbCenterCard;						//中心扑克
	byte							cbCardIndex[3];						//扑克索引
	byte							cbValidIndex[3];					//实际扑克索引
};

//组合子项
struct T_WeaveItem
{
	byte							cbWeaveKind;						//组合类型
	byte							cbCenterCard;						//中心扑克
	byte							cbPublicCard;						//公开标志
	word							wProvideUser;						//供应用户
};

//组合子项
struct CMD_WeaveItem
{
	byte							cbWeaveKind;						//组合类型
	byte							cbCenterCard;						//中心扑克
	byte							cbPublicCard;						//公开标志
	word							wProvideUser;						//供应用户
	//GYZJ
	byte		cbCardData[4];		//组合数据
};

//杠牌结果
struct tagGangCardResult
{
	byte							cbCardCount;						//扑克数目,最大四张
	byte							cbCardData[4];						//扑克数据
};

//分析子项
struct tagAnalyseItem
{
	byte							cbCardEye;							//牌眼扑克,将眼，将对
	bool                            bMagicEye;                          //牌眼是否是王霸 --沱江麻将
	byte							cbWeaveKind[4];						//组合类型
	byte							cbCenterCard[4];					//中心扑克
	byte                            cbCardData[4][4];                   //实际扑克
};

//////////////////////////////////////////////////////////////////////////


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
class CChiHuRight
{	


public:
	//构造函数
	CChiHuRight();

	//运算符重载
public:
	//赋值符
	CChiHuRight & operator = ( dword dwRight );

	//与等于
	CChiHuRight & operator &= ( dword dwRight );
	//或等于
	CChiHuRight & operator |= ( dword dwRight );

	//与
	CChiHuRight operator & ( dword dwRight );
	CChiHuRight operator & ( dword dwRight ) const;

	//或
	CChiHuRight operator | ( dword dwRight );
	CChiHuRight operator | ( dword dwRight ) const;

	//功能函数
public:
	//是否权位为空
	bool IsEmpty();

	//设置权位为空
	void SetEmpty();

	//获取权位数值
	byte GetRightData( dword dwRight[], byte cbMaxCount );

	//设置权位数值
	bool SetRightData( const dword dwRight[], byte cbRightCount );

private:
	//检查权位是否正确
	bool IsValidRight( dword dwRight );

	//静态变量
private:
	static bool						m_bInit;
	static dword					m_dwRightMask[MAX_RIGHT_COUNT];

	//权位变量
private:
	dword							m_dwRight[MAX_RIGHT_COUNT];

};


//////////////////////////////////////////////////////////////////////////


// cbCardIndex[MAX_INDEX] 数字元素索引--牌值, 数值元素值--该牌的张数0-4
// 数值和stl的选择
class MjLogic
{

public:
	MjLogic();
	virtual ~MjLogic();

	//控制函数
public:
	//洗牌
	void ruffleCard(CMjCard cbCardData[], byte cbMaxCount);

	//删除扑克
	bool RemoveCard(byte cbCardIndex[MAX_INDEX], byte cbRemoveCard);
	//删除扑克
	bool RemoveCard(byte cbCardIndex[MAX_INDEX], const byte cbRemoveCard[], byte cbRemoveCount);
	//删除扑克
	bool RemoveValueCard(byte cbCardData[], byte cbCardCount, const byte cbRemoveCard[], byte cbRemoveCount);
	//删除扑克
	byte RemoveValueCardAll(byte cbCardData[], byte cbCardCount, byte cbRemoveCard);
	//删除扑克
	bool RemoveValueCardOne(byte cbCardData[], byte cbCardCount, byte cbRemoveCard);
	//设置钻牌
	void SetMagicIndex(byte cbMagicIndex) { m_cbMagicIndex = cbMagicIndex; }
	//钻牌
	bool IsMagicCard(byte cbCardData);

	//辅助函数
public:
	//有效判断
	bool IsValidCard(byte cbCardData);
	//扑克数目
	byte GetCardCount(const byte cbCardIndex[MAX_INDEX]);
	//组合扑克
	byte GetWeaveCard(byte cbWeaveKind, byte cbCenterCard, byte cbCardBuffer[4]);

	//等级函数
public:
	//动作等级
	byte GetUserActionRank(byte cbUserAction);
	//胡牌等级
	word GetChiHuActionRank(const CChiHuRight & ChiHuRight);

	//动作判断
public:
	//吃牌判断
	byte EstimateEatCard(const byte cbCardIndex[MAX_INDEX], byte cbCurrentCard);
	//碰牌判断
	byte EstimatePengCard(const byte cbCardIndex[MAX_INDEX], byte cbCurrentCard);
	//杠牌判断
	byte EstimateGangCard(const byte cbCardIndex[MAX_INDEX], byte cbCurrentCard);

	//动作判断
public:
	//杠牌分析
	byte AnalyseGangCard(const byte cbCardIndex[MAX_INDEX], const T_WeaveItem WeaveItem[], byte cbWeaveCount, tagGangCardResult & GangCardResult);
	//吃胡分析
	byte AnalyseChiHuCard(const byte cbCardIndex[MAX_INDEX], const T_WeaveItem WeaveItem[], byte cbWeaveCount, byte cbCurrentCard, CChiHuRight &ChiHuRight);
	//听牌分析
	byte AnalyseTingCard(const byte cbCardIndex[MAX_INDEX], const T_WeaveItem WeaveItem[], byte cbWeaveCount);
	//是否听牌
	bool IsTingCard(const byte cbCardIndex[MAX_INDEX], const T_WeaveItem WeaveItem[], byte cbWeaveCount);
	//是否花猪
	bool IsHuaZhu(const byte cbCardIndex[MAX_INDEX], const T_WeaveItem WeaveItem[], byte cbWeaveCount);

	//杠牌分析
	byte AnalyseGangCard(const byte cbCardIndex[MAX_INDEX], const CMD_WeaveItem WeaveItem[], byte cbWeaveCount, tagGangCardResult & GangCardResult);

	//转换函数
public:
	//扑克转换
	byte SwitchToCardData(byte cbCardIndex);
	//扑克转换
	byte SwitchToCardIndex(byte cbCardData);
	//扑克转换
	byte SwitchToCardData(const byte cbCardIndex[MAX_INDEX], byte cbCardData[MAX_HAND_COUNT]);
	//扑克转换
	byte SwitchToCardIndex(const byte cbCardData[], byte cbCardCount, byte cbCardIndex[MAX_INDEX]);


	//排序,根据牌值排序
	bool SortCardList(byte cbCardData[MAX_HAND_COUNT], byte cbCardCount);
	//胡法分析
protected:
	//大对子
	bool IsPengPeng(const tagAnalyseItem *pAnalyseItem);
	//清一色牌
	bool IsQingYiSe(const byte cbCardIndex[MAX_INDEX], const T_WeaveItem WeaveItem[], const byte cbItemCount, const byte cbCurrentCard);
	//七小对牌
	bool IsQiXiaoDui(const byte cbCardIndex[MAX_INDEX], const T_WeaveItem WeaveItem[], const  byte cbWeaveCount, const byte cbCurrentCard);
	//带幺
	bool IsDaiYao(const tagAnalyseItem *pAnalyseItem);
	//将对
	bool IsJiangDui(const tagAnalyseItem *pAnalyseItem);
public:
	byte GetCardColor(byte cbCardDat);
	byte GetCardValue(byte cbCardDat);


	//变量定义
protected:
	byte							m_cbMagicIndex;						//钻牌索引
};


//////////////////////////////////////////////////////////////////////////

#endif // MJ_LOGIC_FILE
