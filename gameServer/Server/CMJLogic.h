#ifndef CMJLOGIC_H
#define CMJLOGIC_H
#include "MjCommonDef.h"
#include "UpgradeMessage.h"
class CMJLogic
{
public:
//-----------------------------------�齫����ɾ�Ĳ��������---------------------------------------------
	//����˵�����ж��齫ֵ�Ƿ�Ƿ�
	//����˵����nCard �齫ֵ
	//����ֵ��trueΪ�Ϸ���falseΪ�Ƿ�
	static bool IsValidCard(int nCard);

	//����˵���������齫ֵ����
	//����˵����array�齫ֵ���飬node_num �齫ֵ�����С
	//����ֵ����
	static void sort(int array[], int node_num);

	//����˵�������齫���������ɾ��ָ��ֵ
	//����˵����array�齫ֵ���飬node_num �齫ֵ�����С��mjΪָ��Ҫɾ���齫ֵ
	//����ֵ��trueΪ�ɹ���falseΪʧ��
	static bool RemoveCard(int array[], int node_num, int mj);


	//����˵�������齫������ɾ��ָ���齫����
	//����˵����arrayԴ���飬nSizeԴ�����С,delCardsҪɾ�����齫����,nDelSizeҪɾ�����齫����Ĵ�С
	//����ֵ��trueΪ�ɹ���falseΪʧ��
	static bool ReomveCards(int array[],int& nSize,int delCards[],int nDelSize);

	//����˵�����ж��齫ֵ���齫�������������ֵ
	//����˵����arrayԴ���飬nSizeԴ�����С,mjΪҪ���ҵ�ֵ
	//����ֵ��-1 ��ʾ�����ڸ��齫ֵ������Ϊ����λ��
	static int FindCardIndex(int const array[], int size, int mj);

	//����˵�������齫�����У���ȡĳһ�齫ֵ�þ������
	//����˵����arrayԴ���飬nSizeԴ�����С,mjΪҪ���ҵ�ֵ
	//����ֵ���齫ֵ�þ������
	static int GetCardNum(int const array[], int size, int mj);

	//����˵������ȡ�齫������
	//����˵����nCard �齫ֵ
	//����ֵ��-1��ʾ���ͷǷ�
	static int GetCardKind(int nCard);

	//����˵�����齫�����У��Ƿ����ĳָ���齫ֵ
	//����˵����arrayԴ���飬nSizeԴ�����С,mjΪҪ���ҵ�ֵ
	//����ֵ��trueΪ�ɹ���falseΪʧ��
	static bool IsCardInArray(int const array[], int size, int mj);

	//����˵�����齫�����У��Ƿ����ĳָ���齫����
	//����˵����arrayԴ���飬nSizeԴ�����С��cardsΪҪ���ҵ��齫���飬nCardsNumΪ�����齫�����С��
	//����ֵ��trueΪ�ɹ���falseΪʧ��
	static bool IsCardsInArray(int const array[],int size, int const cards[],int nCardsNum);
//-------------------------------------------------------------end----------------------------------------------------


//----------------------------------------------------------�齫�������ͺ�ϴ���㷨---------------------------------------

	//����˵�����齫ϴ���㷨
	//����˵����arrayΪ�����齫�ӵ����飬nSize ϴ�������ƣ�nRandTypeϴ������(��Ҫ������Щ�齫��)
	//����ֵ��trueΪ�Ϸ���falseΪ�Ƿ�
	static void ShuffleCards(int array[], int nSize, int nRandType);


	static int GetHuQidui(int const array[], int iNums, const TMjCGPNode lstCGP[], int iCGPNums, int nBaoValue,int nBaoNums);

	//�ж��Ƿ��Ƿ�һɫ����һɫȫ�����ɷ���ɵ�
	static bool IsHuFengYiSe(int const array[], int iNums,const TMjCGPNode lstCGP[], int iCGPNums,int nBaoValue);

	static bool IsHuJiangYiSe(int const array[], int iNums, const TMjCGPNode lstCGP[], int iCGPNums, int nBaoValue);

	//�ж��Ƿ���һɫ����һ�ֻ�ɫ����������ɺ͸��ƣ�����
	static bool IsHuQingYiSe(int const array[],int iNums,const TMjCGPNode lstCGP[],int iCGPNums, int nBaoValue,int nBaoNums);

	//�ж��Ƿ�������
	static bool IsHuPengPeng(int const array[],int iNums,const TMjCGPNode lstCGP[],int iCGPNums, int nBaoValue, int nBaoNums);

	static bool IsCanHuQiDui(int const array[],int iNums,const TMjCGPNode lstCGP[],int iCGPNums,int nBaoValue[MJ_MAX_BAO_CARD],int nLaiZiTypeNum);


	static bool IsCanHuShiSanYao(int const array[],int iNums,const TMjCGPNode lstCGP[],int iCGPNums,int nBaoValue[MJ_MAX_BAO_CARD],int nLaiZiTypeNum);

	static bool IsHuYouJin(int const array[],int iNums,const TMjCGPNode lstCGP[],int iCGPNums,int nBaoValue[MJ_MAX_BAO_CARD],int nLaiZiTypeNum,int byFetchCard);
//----------------------------------------------------------end--------------------------------------------------------


//-----------------------------------------------------------�齫���������ж�----------------------------------------------------
	//����˵�����ж��Ƿ�ﵽȫ�ǿ��ֺ;��ӣ�һ�Զ��Ƶĺ�������
	//����˵����array�齫ֵ���飬node_num �齫ֵ�����С,bao_array ���������,bao_type_num ���������,userable_bao_num���������,bMaster258Pair�Ƿ���Ҫ2��5��8������
	//����ֵ��trueΪ�ɺ���falseΪ���ɺ�
	static bool IsCanHu_3x2(int const array[], int node_num, int bao_array[], int bao_type_num ,int useable_bao_num, bool bMaster258Pair);


	static bool IsCanHu(int byOutCard,int byFetchCard,int byCards[],int iNums,const TMjCGPNode lstCGP[],int iCGPNums,TMjActInfo* pActInfo,int iBaoValue, TCHAR* szHuDescr,bool bQiangGang);

	//����˵�����ж��齫ֵ�Ǹ����������ֻ��߾���
	//����˵����array�齫ֵ���飬node_num �齫ֵ�����С��bao_num �������
	//����ֵ��trueΪ�Ϸ���falseΪ�Ƿ�
	static bool consist_3x(int const array[], int node_num, int bao_num);

	//�жϿɷ��(û�в���)
	static bool IsCanChi(int nOutCard, const int array[], int iNums,bool bCanChiFeng);

	static bool IsCanChi(BYTE byOutCard, const int byCards[], int iNums, TMjActInfo *pActInfo,bool bCanChiFeng = false);
	
	//��������Ƿ�Ϊ�������������
	static bool CheckSubArrInside(const int byCards[], int iNums, int byPart[], int iPart);
	//�������������Ƿ����齫ֵ
	static bool CheckArrCards(int byCards[], int iNums);

	

	//�жϿɷ���
	static bool IsCanPeng(int nOutCard,const int array[],int iNums);

	//�жϿɷ��
	//static bool IsCanGang(int nCard,const int array[],int iCardNums,const TMjCGPNode lstCGP[],int iCGPNums);

	//�жϿɷ��
	static bool IsCanGang(int byOutCard,const int byCards[],int iCardNums,\
		const TMjCGPNode lstCGP[],int iCGPNums,TMjActInfo* pActInfo,int byFetchCard);


	//�жϿɷ���
	static bool IsCanTing(const int array[],int iNums,int nBaoValue,bool bMaster258Pair);

	//���µ�����
	static void ResetFeng(int *byCards, int iNums);
	//���µ����з���
	static void ResetZFB(int *byCards, int iNums);
	///��ȡ��һ����ҵ���λ��
	static BYTE GetNextUserStation(BYTE byStation,bool bCW,int PlayCount,int iNum = 1);
	//��ȡһ���Զ�������
	static int getAutoOutCard(const int byCards[],int iNums);
	//һ�������Ƿ���ĳ���������ٽ���������
	static bool isHaveNear2(int byCard ,int byCards[],int iNums);
	//��ȡһ���Ƶĵ���
	static int GetDianShu(int byValue);
	//----------------------------------------------------------��ֵ���ݱ���-----------------------------------------------------------------
	///�жϸ���ֵ�Ƿ���Ч
	static bool Verify(int byPs);
	//����Դ������û��ָ������
	static bool InCards(int byCard,const int bySrc[],int iSrcNums);
     
	static bool InCards(int byCard,const int bySrc[],int iSrcNums,int &nPos);
	///ɾ��һ������
	static bool RemoveOneCard(int byCard,int bySrc[],int iSrcNums);

	//ɾ����
	static int RemoveCards(const int byDst[],int iDstNums,int bySrc[],int iSrcNums);
	//���0λ��
	static int Remove0(int byCards[], int iNum);
//--------------------------------------------------------------------end--------------------------------------------------------------------
};
#endif