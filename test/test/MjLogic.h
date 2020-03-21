#ifndef MJ_LOGIC_FILE
#define MJ_LOGIC_FILE

#include <vector>
#include "MjCard.h"
#include "define/MjDef.h"

//////////////////////////////////////////////////////////////////////////



//�˿˶���
#define HEAP_FULL_COUNT				26									//����ȫ��

#define MAX_RIGHT_COUNT				1									//���Ȩλdword����		

//��������
struct tagKindItem
{
	byte							cbWeaveKind;						//�������
	byte							cbCenterCard;						//�����˿�
	byte							cbCardIndex[3];						//�˿�����
	byte							cbValidIndex[3];					//ʵ���˿�����
};

//�������
struct T_WeaveItem
{
	byte							cbWeaveKind;						//�������
	byte							cbCenterCard;						//�����˿�
	byte							cbPublicCard;						//������־
	word							wProvideUser;						//��Ӧ�û�
};

//�������
struct CMD_WeaveItem
{
	byte							cbWeaveKind;						//�������
	byte							cbCenterCard;						//�����˿�
	byte							cbPublicCard;						//������־
	word							wProvideUser;						//��Ӧ�û�
	//GYZJ
	byte		cbCardData[4];		//�������
};

//���ƽ��
struct tagGangCardResult
{
	byte							cbCardCount;						//�˿���Ŀ,�������
	byte							cbCardData[4];						//�˿�����
};

//��������
struct tagAnalyseItem
{
	byte							cbCardEye;							//�����˿�,���ۣ�����
	bool                            bMagicEye;                          //�����Ƿ������� --�����齫
	byte							cbWeaveKind[4];						//�������
	byte							cbCenterCard[4];					//�����˿�
	byte                            cbCardData[4][4];                   //ʵ���˿�
};

//////////////////////////////////////////////////////////////////////////


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
class CChiHuRight
{	


public:
	//���캯��
	CChiHuRight();

	//���������
public:
	//��ֵ��
	CChiHuRight & operator = ( dword dwRight );

	//�����
	CChiHuRight & operator &= ( dword dwRight );
	//�����
	CChiHuRight & operator |= ( dword dwRight );

	//��
	CChiHuRight operator & ( dword dwRight );
	CChiHuRight operator & ( dword dwRight ) const;

	//��
	CChiHuRight operator | ( dword dwRight );
	CChiHuRight operator | ( dword dwRight ) const;

	//���ܺ���
public:
	//�Ƿ�ȨλΪ��
	bool IsEmpty();

	//����ȨλΪ��
	void SetEmpty();

	//��ȡȨλ��ֵ
	byte GetRightData( dword dwRight[], byte cbMaxCount );

	//����Ȩλ��ֵ
	bool SetRightData( const dword dwRight[], byte cbRightCount );

private:
	//���Ȩλ�Ƿ���ȷ
	bool IsValidRight( dword dwRight );

	//��̬����
private:
	static bool						m_bInit;
	static dword					m_dwRightMask[MAX_RIGHT_COUNT];

	//Ȩλ����
private:
	dword							m_dwRight[MAX_RIGHT_COUNT];

};


//////////////////////////////////////////////////////////////////////////


// cbCardIndex[MAX_INDEX] ����Ԫ������--��ֵ, ��ֵԪ��ֵ--���Ƶ�����0-4
// ��ֵ��stl��ѡ��
class MjLogic
{

public:
	MjLogic();
	virtual ~MjLogic();

	//���ƺ���
public:
	//ϴ��
	void ruffleCard(CMjCard cbCardData[], byte cbMaxCount);

	//ɾ���˿�
	bool RemoveCard(byte cbCardIndex[MAX_INDEX], byte cbRemoveCard);
	//ɾ���˿�
	bool RemoveCard(byte cbCardIndex[MAX_INDEX], const byte cbRemoveCard[], byte cbRemoveCount);
	//ɾ���˿�
	bool RemoveValueCard(byte cbCardData[], byte cbCardCount, const byte cbRemoveCard[], byte cbRemoveCount);
	//ɾ���˿�
	byte RemoveValueCardAll(byte cbCardData[], byte cbCardCount, byte cbRemoveCard);
	//ɾ���˿�
	bool RemoveValueCardOne(byte cbCardData[], byte cbCardCount, byte cbRemoveCard);
	//��������
	void SetMagicIndex(byte cbMagicIndex) { m_cbMagicIndex = cbMagicIndex; }
	//����
	bool IsMagicCard(byte cbCardData);

	//��������
public:
	//��Ч�ж�
	bool IsValidCard(byte cbCardData);
	//�˿���Ŀ
	byte GetCardCount(const byte cbCardIndex[MAX_INDEX]);
	//����˿�
	byte GetWeaveCard(byte cbWeaveKind, byte cbCenterCard, byte cbCardBuffer[4]);

	//�ȼ�����
public:
	//�����ȼ�
	byte GetUserActionRank(byte cbUserAction);
	//���Ƶȼ�
	word GetChiHuActionRank(const CChiHuRight & ChiHuRight);

	//�����ж�
public:
	//�����ж�
	byte EstimateEatCard(const byte cbCardIndex[MAX_INDEX], byte cbCurrentCard);
	//�����ж�
	byte EstimatePengCard(const byte cbCardIndex[MAX_INDEX], byte cbCurrentCard);
	//�����ж�
	byte EstimateGangCard(const byte cbCardIndex[MAX_INDEX], byte cbCurrentCard);

	//�����ж�
public:
	//���Ʒ���
	byte AnalyseGangCard(const byte cbCardIndex[MAX_INDEX], const T_WeaveItem WeaveItem[], byte cbWeaveCount, tagGangCardResult & GangCardResult);
	//�Ժ�����
	byte AnalyseChiHuCard(const byte cbCardIndex[MAX_INDEX], const T_WeaveItem WeaveItem[], byte cbWeaveCount, byte cbCurrentCard, CChiHuRight &ChiHuRight);
	//���Ʒ���
	byte AnalyseTingCard(const byte cbCardIndex[MAX_INDEX], const T_WeaveItem WeaveItem[], byte cbWeaveCount);
	//�Ƿ�����
	bool IsTingCard(const byte cbCardIndex[MAX_INDEX], const T_WeaveItem WeaveItem[], byte cbWeaveCount);
	//�Ƿ���
	bool IsHuaZhu(const byte cbCardIndex[MAX_INDEX], const T_WeaveItem WeaveItem[], byte cbWeaveCount);

	//���Ʒ���
	byte AnalyseGangCard(const byte cbCardIndex[MAX_INDEX], const CMD_WeaveItem WeaveItem[], byte cbWeaveCount, tagGangCardResult & GangCardResult);

	//ת������
public:
	//�˿�ת��
	byte SwitchToCardData(byte cbCardIndex);
	//�˿�ת��
	byte SwitchToCardIndex(byte cbCardData);
	//�˿�ת��
	byte SwitchToCardData(const byte cbCardIndex[MAX_INDEX], byte cbCardData[MAX_HAND_COUNT]);
	//�˿�ת��
	byte SwitchToCardIndex(const byte cbCardData[], byte cbCardCount, byte cbCardIndex[MAX_INDEX]);


	//����,������ֵ����
	bool SortCardList(byte cbCardData[MAX_HAND_COUNT], byte cbCardCount);
	//��������
protected:
	//�����
	bool IsPengPeng(const tagAnalyseItem *pAnalyseItem);
	//��һɫ��
	bool IsQingYiSe(const byte cbCardIndex[MAX_INDEX], const T_WeaveItem WeaveItem[], const byte cbItemCount, const byte cbCurrentCard);
	//��С����
	bool IsQiXiaoDui(const byte cbCardIndex[MAX_INDEX], const T_WeaveItem WeaveItem[], const  byte cbWeaveCount, const byte cbCurrentCard);
	//����
	bool IsDaiYao(const tagAnalyseItem *pAnalyseItem);
	//����
	bool IsJiangDui(const tagAnalyseItem *pAnalyseItem);
public:
	byte GetCardColor(byte cbCardDat);
	byte GetCardValue(byte cbCardDat);


	//��������
protected:
	byte							m_cbMagicIndex;						//��������
};


//////////////////////////////////////////////////////////////////////////

#endif // MJ_LOGIC_FILE
