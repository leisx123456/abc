#include "CMJLogic.h"
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <atlstr.h>
#include <algorithm>

bool CMJLogic::IsValidCard(int nCard)
{
	int nKindType = GetCardKind(nCard);
	if (-1 == nKindType)
	{
		return false;
	}

	int nValue = nCard%10;

	if (nKindType >= MJK_WAN && nKindType <= MJK_TONG)
	{
		return (LABEL_1 <= nValue && nValue <= LABEL_9);
	}
	else if (MJK_FENG == nKindType)
	{
		return (LABEL_1 <= nValue && nValue <= LABEL_7);
	}
	else if (MJK_ZI == nKindType)
	{
		return (LABEL_1 <= nValue && nValue <= LABEL_8);
	}
	return false;
}

int CMJLogic::GetCardKind(int nCard)
{
	int nKindType = nCard/10;
	if (nKindType>=MJK_WAN && nKindType <=MJK_ZI)
	{
		return nKindType;
	}
	return -1;
}

void CMJLogic::sort(int array[], int size)
{
	int temp;
	for (int i = 0; i < size - 1; ++i)
	{
		for (int j = i + 1; j < size; ++j)
		{
			if (array[i] > array[j])
			{
				temp = array[i];
				array[i] = array[j];
				array[j] = temp;
			}
		}
	}
}

bool CMJLogic::RemoveCard(int array[], int node_num, int mj)
{
	for (int i = 0; i < node_num; ++i)
	{
		if (array[i] == mj)
		{
			for (int j = i + 1; j < node_num; ++j)
			{
				array[j - 1] = array[j];
			}

			array[node_num - 1] = INVALID_MJ;
			return true;
		}
	}

	return false;
}

bool CMJLogic::ReomveCards(int array[],int& nSize,int delCards[],int nDelSize)
{
	//��������
	if (nDelSize>nSize) 
		return false;

	//�ж�Ҫɾ�������飬�Ƿ����Դ��������
	if (!IsCardsInArray(array,nSize,delCards,nDelSize))
	{
		return false;
	}

	for (int i = 0; i < nDelSize; i++)
	{
		RemoveCard(array,nSize--,delCards[i]);
	}

	return true;
}



bool CMJLogic::IsCanHu(int byOutCard,int byFetchCard,int byCards[],int iNums,const TMjCGPNode lstCGP[],int iCGPNums,TMjActInfo* pActInfo,int iBaoValue, TCHAR* szHuDescr,bool bQiangGang)
{
	bool bCanDanDiao = true;

	if (byOutCard < 0)
	{
		return false;
	}

	int iTempNums = iNums;

	int byTemp[HAND_MAX_NUM_MJ];

	::memcpy(byTemp,byCards,iTempNums * sizeof(int));

	//if((byOutCard != 255 && byOutCard != 0) && iNums <14 )
	//{
	//	byTemp[iTempNums++] = byOutCard;

	//}

	sort(byTemp,iTempNums);

	int byCardTemp[MJ_MAX_BAO_CARD];
	int nBao=0;
	if (iBaoValue!=255)
	{
		byCardTemp[0]=iBaoValue;
		nBao=GetCardNum(byTemp,iTempNums,iBaoValue);
	}


	int  bPingHu = IsCanHu_3x2(byTemp,iTempNums,byCardTemp,1,nBao,false);

	if (nBao>=3 && byFetchCard != 0)
	{
		//�����������Ͽ���ֱ�Ӻ���
		//pActInfo->nHuType[pActInfo->iHuTypeNums++] = ht_PingHu;
		//return true;

		pActInfo->nHuType[pActInfo->iHuTypeNums++]=ht_SanJinDao;
		bPingHu=true;
	}

	bool bHuQiDui=false;
	bHuQiDui=IsCanHuQiDui(byTemp,iTempNums,lstCGP,iCGPNums,byCardTemp,1);


	bool bShiSanYao=IsCanHuShiSanYao(byTemp,iTempNums,lstCGP,iCGPNums,byCardTemp,1);

	if (bPingHu || bHuQiDui || bShiSanYao)
	{
		if (bHuQiDui)
		{
			pActInfo->nHuType[pActInfo->iHuTypeNums++] = ht_QiDui;
			//if (IsCanHuHaoHuaQiDui(byTemp,iTempNums,lstCGP,iCGPNums,iBaoValue,nLaiZiTypeNum))
			//{
			//	pActInfo->nHuType[pActInfo->iHuTypeNums++]=ht_HaoHuaQiDui;
			//	bHaveOther=true;
			//}

			//if (IsCanHuShuangHaoHuaQiDui(byTemp,iTempNums,lstCGP,iCGPNums,iBaoValue,nLaiZiTypeNum))
			//{
			//	pActInfo->nHuType[pActInfo->iHuTypeNums++]=ht_ShuangHaoHuaQiDui;
			//	bHaveOther=true;
			//}

			//if (IsCanHuSanHaoHuaQiDui(byTemp,iTempNums,lstCGP,iCGPNums,iBaoValue,nLaiZiTypeNum))
			//{
			//	pActInfo->nHuType[pActInfo->iHuTypeNums++]=ht_SanHaoHuaQiDui;
			//	bHaveOther=true;
			//}

			//if (bHaveOther==false)
			//{
			//	pActInfo->nHuType[pActInfo->iHuTypeNums++]=ht_QiDui;
			//}

		}

		// 20181204����ʱҪ����
		if (bQiangGang)
		{
			byFetchCard = byOutCard;
		}

		if  ( IsHuYouJin(byTemp, iTempNums, lstCGP, iCGPNums, byCardTemp , 1, byFetchCard) )
		{
			pActInfo->nHuType[pActInfo->iHuTypeNums++] = ht_YouJin;
		}

		if (bShiSanYao)
		{
			pActInfo->nHuType[pActInfo->iHuTypeNums++]=ht_ShiSanYao;
		}

		if (pActInfo->iHuTypeNums==0)
		{
			pActInfo->nHuType[pActInfo->iHuTypeNums++] = ht_PingHu;
		}

		return true;
	}
	return false;
}


bool CMJLogic::IsHuYouJin(int const array[],int iNums,const TMjCGPNode lstCGP[],int iCGPNums,int nBaoValue[],int nLaiZiTypeNum,int byFetchCard)
{

	if (iNums < 1 || array == NULL)
	{
		return false;
	}

	if (byFetchCard==0)
	{
		return false;
	}

	if (nLaiZiTypeNum<=0)
	{
		return false;
	}


	int byTemp[HAND_MAX_NUM_MJ]={0};
	memcpy(byTemp,array,sizeof(int)*iNums);
	int desk_num = 0;
	int Single_num = 0;//�����Ƶ�����

	int nBaoNum[MJ_MAX_BAO_CARD];
	int nAllBaoNum=0;
	for (int i=0;i<nLaiZiTypeNum;++i)
	{
		nBaoNum[i]=GetCardNum(byTemp,iNums,nBaoValue[i]);
		nAllBaoNum+=nBaoNum[i];
	}

	if (nAllBaoNum<=0)
	{
		return false;
	}

	if (nAllBaoNum==1 && byFetchCard==nBaoValue[0])
	{
		return false;
	}

	//if (nAllBaoNum>=3)
	//{
	//	return true;
	//}


	for (int i=0;i<nLaiZiTypeNum;++i)
	{
		for (int j=0;j<nBaoNum[i];++j)
		{
			RemoveCard(byTemp, iNums--, nBaoValue[i]);//��ɾ�������е�����
		}

	}

	//�滻���װ�
	for (int i = 0; i<iNums; i++)
	{
		if (byTemp[i]==37)
		{
			byTemp[i]=nBaoValue[0];
		}
	}


	RemoveCard(byTemp, iNums--, byFetchCard);
	sort(byTemp, iNums);
	if (consist_3x(byTemp, iNums, nAllBaoNum-1))
	{
		return true;
	}

	if (nAllBaoNum>1)
	{
		for (int i = 0; i<nAllBaoNum-1; i++)
		{
			byTemp[iNums++] = nBaoValue[0];
		}
	}
	sort(byTemp,iNums);
	if (IsCanHuQiDui(byTemp, iNums, lstCGP, iCGPNums, nBaoValue, nLaiZiTypeNum))
	{
		return true;
	}


	return false;
}



bool CMJLogic::IsCanHuShiSanYao(int const array[],int iNums,const TMjCGPNode lstCGP[],int iCGPNums,int nBaoValue[],int nLaiZiTypeNum)
{
	if (iNums < 1 || array == NULL)
	{
		return false;
	}

	if (iCGPNums>0)
	{
		return false;
	}

	int byTemp[HAND_MAX_NUM_MJ]={0};
	memcpy(byTemp,array,sizeof(int)*iNums);
	int desk_num = 0;
	int Single_num = 0;//�����Ƶ�����

	int nBaoNum[MJ_MAX_BAO_CARD];
	int nAllBaoNum=0;
	for (int i=0;i<nLaiZiTypeNum;++i)
	{
		nBaoNum[i]=GetCardNum(byTemp,iNums,nBaoValue[i]);
		nAllBaoNum+=nBaoNum[i];
	}

	for (int i=0;i<nLaiZiTypeNum;++i)
	{
		for (int j=0;j<nBaoNum[i];++j)
		{
			RemoveCard(byTemp,iNums--,nBaoValue[i]);//��ɾ�������е�����
		}

	}

	int arry[13] = {1,9,11,19,21,29,31,32,33,34,35,36,37};
	int nArryNum[13]={0};
	bool bHave[13];
	int nMatchNum=0;
	int nAllMatchNum=0;
	memset(bHave,0,sizeof(bHave));

	int nLaiZiPos=0;
	//�����Ƿ���ʮ�����е���ֵ
	bool bLaiZiInTable=false;
	if (nLaiZiTypeNum>0)
	{
		if (InCards(nBaoValue[0],arry,13,nLaiZiPos))
		{
			bLaiZiInTable=true;
		}
	}

	for (int i=0;i<13;++i)
	{
		int nMatchValue=arry[i];
		int nNum=GetCardNum(byTemp,iNums,nMatchValue);
		if (nNum>0)
		{
			nArryNum[i]=nNum;
			bHave[i]=true;
			nMatchNum++;

			nAllMatchNum+=nNum;

		}

	}


	if (nArryNum[12]>1)
	{
		if (bLaiZiInTable==true)
		{
			nArryNum[nLaiZiPos]+=(nArryNum[12]-1);
			nArryNum[12]=1;
			bHave[nLaiZiPos]=true;
			nMatchNum++;

		}else
		{
			return false;
		}
	}

	if (nArryNum[10]>1 || nArryNum[11]>1 ||nArryNum[12]>1)
	{
		return false;
	}

	if (nMatchNum+nAllBaoNum<13 || nAllMatchNum+nAllBaoNum!=14)
	{
		return false;
	}

	return true;

}



bool CMJLogic::IsCanHuQiDui(int const array[],int iNums,const TMjCGPNode lstCGP[],int iCGPNums,int nBaoValue[],int nLaiZiTypeNum)
{
	if (iNums < 1 || array == NULL)
	{
		return false;
	}
	for (int i = 0;i<iCGPNums;i++)
	{
		if (lstCGP[i].nType !=emType::AnGang 
			|| lstCGP[i].nType != emType::BuGang
			|| lstCGP[i].nType != emType::MingGang)
		{
			return false;
		}
	}
	if (iNums %2 != 0)//������е���������ż��������һ�����ܴճ���С��
	{
		return false;
	}
	int byTemp[HAND_MAX_NUM_MJ]={0};
	memcpy(byTemp,array,sizeof(int)*iNums);
	int desk_num = 0;
	int Single_num = 0;//�����Ƶ�����

	int nBaoNum[MJ_MAX_BAO_CARD];
	int nAllBaoNum=0;
	for (int i=0;i<nLaiZiTypeNum;++i)
	{
		nBaoNum[i]=GetCardNum(byTemp,iNums,nBaoValue[i]);
		nAllBaoNum+=nBaoNum[i];
	}

	for (int i=0;i<nLaiZiTypeNum;++i)
	{
		for (int j=0;j<nBaoNum[i];++j)
		{
			RemoveCard(byTemp,iNums--,nBaoValue[i]);//��ɾ�������е�����
		}

	}

	for (int j = 0 ;j <iNums ;j++)
	{
		desk_num = GetCardNum(byTemp,iNums,byTemp[j]);
		if (desk_num == 4)
		{
			j+=3;//�ӵ������ƿ�ʼ���
		}
		else if (desk_num == 3)
		{
			j+=2;
			Single_num++;//˵������������һ���ģ����ܳɶ�
		}
		else if (desk_num == 2)
		{
			j+=1;
		}
		else if (desk_num ==1)
		{
			Single_num++;
		}
	}
	if (Single_num%2==0&&nAllBaoNum%2==0&&nAllBaoNum>=Single_num)//���������ż�������������ӵ�����һ����ż�������������ܵ��ڵ���������
	{
		return true;
	}
	if (Single_num %2 !=0&&nAllBaoNum%2 !=0 &&nAllBaoNum>=Single_num)
	{
		return true;
	}

	return false;
}

bool CMJLogic::IsCanHu_3x2(int const array[], int node_num, int bao_array[],int bao_type_num,int useable_bao_num, bool bMaster258Pair)
{

	if (node_num > HAND_MAX_NUM_MJ || node_num <= 0)
		return false;

	if (node_num % 3 != 2)
		return false;

	if (useable_bao_num == node_num)//���˵ȫ�Ǳ��ƣ��϶��ǿ��Ժ���
	{
		return true;
	}

	int NoBaoArray[HAND_MAX_NUM_MJ];
	memcpy(NoBaoArray, array, sizeof(int) * node_num);
	int nNoBaoNum = node_num;
	sort(NoBaoArray, node_num);

	for (int i = 0; i < node_num; ++i)
	{
		for (int j = 0; j < bao_type_num; j++)
		{
			if (array[i] == bao_array[j] && ! RemoveCard(NoBaoArray, nNoBaoNum--, bao_array[j]))///�������Ƽ��,����в����ƾͻ�ɾ����Ӧ�Ĳ�����.
			{
				return false;
			}
		}
	}

	if (bao_type_num>0)
	{
		for (int i=0;i<nNoBaoNum;++i)
		{
			if (NoBaoArray[i]==37)
			{
				NoBaoArray[i]=bao_array[0];
			}
		}
	}

	sort(NoBaoArray, nNoBaoNum);

	int	buf_size = sizeof(int) * nNoBaoNum;
	int	temp[HAND_MAX_NUM_MJ];

	int mod;
	for (int i = 0; i < nNoBaoNum; ++i)
	{
		if (bMaster258Pair)
		{
			if (NoBaoArray[i] > MJK_FENG)
				continue;
			mod = NoBaoArray[i] % 10;
			if (mod != 2 && mod != 5 && mod != 8 )
				continue;
		}

		if ((i < (nNoBaoNum-1)) && NoBaoArray[i] == NoBaoArray[i + 1])
		{
			if (nNoBaoNum == 2)
				return true;

			memcpy(temp, NoBaoArray, buf_size);
			if (! RemoveCard(temp, nNoBaoNum, NoBaoArray[i]))
			{
				continue;
			}

			if (! RemoveCard(temp, nNoBaoNum - 1, NoBaoArray[i]))
			{
				continue;
			}

			if (consist_3x(temp, nNoBaoNum - 2, useable_bao_num))
				return true;
		}
		else
		{
			memcpy(temp, NoBaoArray, buf_size);
			if (! RemoveCard(temp, nNoBaoNum, NoBaoArray[i]))
			{
				continue;
			}

			if (useable_bao_num < 1)
			{
				continue;
			}

			if (consist_3x(temp, nNoBaoNum - 1, useable_bao_num - 1))
				return true;
		}
	}
	//���˵ȫ����û��258���ԡ����ж�һ�£��Ƿ��ж�������������ϵ�����
	if ( bMaster258Pair && useable_bao_num >= 2)
	{
		memcpy(temp, NoBaoArray, buf_size);
		if (consist_3x(temp, nNoBaoNum, useable_bao_num - 2))
			return true;
	}
	return false;
}

bool CMJLogic::consist_3x(int const no_bao_array[], int node_num, int bao_num)
{
	if (node_num == 0)
		return true;

	if ((node_num + bao_num) % 3 != 0)
		return false;

	int temp[HAND_MAX_NUM_MJ];
	int buf_size = sizeof(int) * node_num;

	// as peng 
	memcpy(temp, no_bao_array, buf_size);
	if (! RemoveCard(temp, node_num, no_bao_array[0]))
	{
		return false;
	}

	int		used_bao_num = 0;
	bool	failed = false;
	if (! RemoveCard(temp, node_num - 1, no_bao_array[0]))
	{
		++used_bao_num;
		if (used_bao_num > bao_num)
		{
			failed = true;
		}
	}

	if (! failed)
	{
		if (! RemoveCard(temp, node_num - 2, no_bao_array[0]))
		{
			++used_bao_num;
			if (used_bao_num > bao_num)
			{
				failed = true;
			}
		}
	}

	if (! failed)
	{
		if (consist_3x(temp, node_num - (3 -  used_bao_num), bao_num - used_bao_num))
			return true;
	}

	// as chi

	memcpy(temp, no_bao_array, buf_size);
	int mj = no_bao_array[0];

	if (mj > 29)
		return false;

	if (! RemoveCard(temp, node_num, mj))
	{
		return false;
	}

	used_bao_num = 0;
	if (! RemoveCard(temp, node_num - 1, mj + 1) || ((mj + 1) / 10 !=  mj / 10))
	{
		++used_bao_num;
		if (used_bao_num > bao_num)
		{
			return false;
		}
	}

	if (! RemoveCard(temp, node_num - (2 -  used_bao_num), mj + 2) || ((mj + 2) / 10 !=  mj / 10))
	{
		++used_bao_num;
		if (used_bao_num > bao_num)
		{
			return false;
		}
	}

	if (consist_3x(temp, node_num - (3 -  used_bao_num), bao_num - used_bao_num))
		return true;

	return false;
}

int CMJLogic::GetCardNum(int const array[], int node_num, int mj)
{
	int num = 0;
	for (int i = 0; i < node_num; ++i)
	{
		if (array[i] == mj)
		{
			++num;
		}
	}
	return num;
}

int CMJLogic::FindCardIndex(int const array[], int node_num, int mj)
{
	for (int i = 0; i < node_num; ++i)
	{
		if (array[i] == mj)
		{
			return i;
		}
	}

	return -1;
}


bool CMJLogic::IsCardInArray(int const array[], int size, int mj)
{
	if (0 >= size || NULL == array)
	{
		return false;
	}

	for (int i = 0; i < size; i++)
	{
		if (array[i] == mj)
		{
			return true;
		}
	}
	return false;
}

bool CMJLogic::IsCardsInArray(int const array[],int size, int const cards[],int nCardsNum)
{
	if (0 >= size || NULL == array)
	{
		return false;
	}

	if (nCardsNum <=0 || NULL == cards)
	{
		return false;
	}
	int* nTemp = new int[size];
	int nTempSize = size;
	if (NULL == nTemp)
	{
		return false;
	}
	memcpy(nTemp,array,sizeof(int)*size);

	for (int i = 0; i < nCardsNum; i++)
	{
		if (!RemoveCard(nTemp,nTempSize--,cards[i]))
		{
			delete nTemp;
			nTemp = NULL;
			return false;
		}
	}

	delete nTemp;
	nTemp = NULL;
	return true;
}


void CMJLogic::ShuffleCards(int array[], int nSize, int nRandType)
{

	int index = 0;

	for(int i = 1; i <= 4; ++i)
	{

		for(int j = 1; j <= 9; ++j)
		{
			if (nRandType & Rand_wan)//��ʾ����
			{
				//���䡰��
				array[index++] = MJK_WAN + j;
			}

			if (nRandType & Rand_tiao)//��ʾ����
			{
				//���䡰����
				array[index++] = MJK_TIAO + j;
			}

			if (nRandType & Rand_tong)//��ʾͲ��
			{
				//���䡰Ͳ��
				array[index++] = MJK_TONG + j;
			}	
		}

		for (int j = 1; j <= 4; ++j)
		{
			if (nRandType & Rand_feng)
			{
				//���䡰�硱
				array[index++] = MJK_FENG + j;
			}

		}
		//���䡰�С�
		if (nRandType & Rand_zhong)
		{
			array[index++] = MJK_FENG + 5;
		}

		//���䡰���ס�
		if (nRandType & Rand_fabai)
		{
			array[index++] = MJK_FENG + 6;
			array[index++] = MJK_FENG + 7;
		}

	}

	for(int i = 1; i <= 8; ++i)
	{
		if (nRandType & Rand_hua)
		{
			//���仨��
			array[index++] =  MJK_ZI + i;
		}
	}

	///������
	::srand(::GetTickCount());

	int temp = 0;
	for(int i = 0; i < index; ++i)
	{
		temp = ::rand() % (index - i) + i;
		std::swap(array[temp],array[i]);
	}
}

void CMJLogic::ResetFeng(int* pCards, int iNums)
{
	for (int i = 0; i< iNums ; i++)
	{
		if ( pCards[i] <31)
		{
			pCards[i] = pCards[i] + 4 ;
		}
		else if(pCards[i]>MJK_FENG+LABEL_BEI)
		{
			pCards[i] = pCards[i] - 4 ;
		}
	}
}

void CMJLogic::ResetZFB(int* pCards, int iNums)
{
	for (int i = 0; i< iNums ; i++)
	{
		if ( pCards[i] < MJK_FENG+LABEL_ZHONG)
		{
			pCards[i] = 0 ;
		}
		else if(pCards[i] > MJK_FENG+LABEL_BAI)
		{
			pCards[i] = 0 ;
		}
	}
}

bool CMJLogic::IsCanChi(int nOutCard, const int array[], int iNums,bool bCanChiFeng)
{
	if (!IsValidCard(nOutCard))
		return false;
	//������ϵģ�1��9,��,��ֻ�ܳ�ͷ����β��
	if (!bCanChiFeng)
	{
		if (nOutCard>=MJK_FENG)
		{
			return false;
		}
	}
	int nTemp[2] = {0};
	//������Ϸ�ʽ -- -+ ++
	nTemp[0] = nOutCard - 1;
	nTemp[1] = nOutCard - 2;

	if ((nOutCard >= 31 && nOutCard <= 34))
	{
		//���ֿ���ѭ������Ҫ���⴦��
		ResetFeng(nTemp,2);
	}
	else if ((nOutCard >= 35 && nOutCard <= 37))
	{
		//�з������⴦��
		ResetZFB(nTemp,2);
	}

	if (IsValidCard(nTemp[0]) && IsValidCard(nTemp[1]))
	{
		if (IsCardsInArray(array,iNums,nTemp,2))
		{
			return true;
		}
	}

	nTemp[0] = nOutCard - 1;
	nTemp[1] = nOutCard + 1;
	if ((nOutCard >= 31 && nOutCard <= 34))
	{
		//���ֿ���ѭ������Ҫ���⴦��
		ResetFeng(nTemp,2);
	}
	else if ((nOutCard >= 35 && nOutCard <= 37))
	{
		//�з������⴦��
		ResetZFB(nTemp,2);
	}

	if (IsValidCard(nTemp[0]) && IsValidCard(nTemp[1]))
	{
		if (IsCardsInArray(array,iNums,nTemp,2))
		{
			return true;
		}
	}

	nTemp[0] = nOutCard + 1;
	nTemp[1] = nOutCard + 2;
	if ((nOutCard >= 31 && nOutCard <= 34))
	{
		//���ֿ���ѭ������Ҫ���⴦��
		ResetFeng(nTemp,2);
	}
	else if ((nOutCard >= 35 && nOutCard <= 37))
	{
		//�з������⴦��
		ResetZFB(nTemp,2);
	}

	if (IsValidCard(nTemp[0]) && IsValidCard(nTemp[1]))
	{
		if (IsCardsInArray(array,iNums,nTemp,2))
		{
			return true;
		}
	}


	return false;
}


bool CMJLogic::IsCanChi(BYTE byOutCard, const int byCards[], int iNums, TMjActInfo *pActInfo,bool bCanChiFeng)
{

	if (!bCanChiFeng)
	{
		if (byOutCard>=MJK_FENG)
		{
			return false;
		}
	}

	//���ֲ��ɳ�
	if (byOutCard == 255)
		return false;

	bool bT = false; 
	bool bZ = false; 
	bool bW = false;
	int byT[2];
	int byZ[2];
	int byW[2];
	byT[0] = byOutCard + 1;
	byT[1] = byOutCard + 2;

	byZ[0] = byOutCard - 1;
	byZ[1] = byOutCard + 1;

	byW[0] = byOutCard - 2;
	byW[1] = byOutCard - 1;

	if ((byOutCard >= 31 && byOutCard <= 34))
	{
		//���ֿ���ѭ������Ҫ���⴦��
		ResetFeng(byT,2);
		ResetFeng(byZ,2);
		ResetFeng(byW,2);
	}
	else if ((byOutCard >= 35 && byOutCard <= 37))
	{
		//�з������⴦��
		ResetZFB(byT,2);
		ResetZFB(byZ,2);
		ResetZFB(byW,2);
	}
	else
	{
		//��Ͳ���������⴦��
	}


	//����ͷ
	if (CheckArrCards(byT, 2))
		bT = CheckSubArrInside(byCards, iNums, byT, 2);

	//������
	if (CheckArrCards(byZ, 2))
		bZ = CheckSubArrInside(byCards, iNums, byZ, 2);

	//����β
	if (CheckArrCards(byW, 2))
		bW = CheckSubArrInside(byCards, iNums, byW, 2);

	if (bT)
		pActInfo->usChiFlags = MjChiFlag::usT;
	if (bZ)
		pActInfo->usChiFlags = MjChiFlag::usZ;
	if (bW)
		pActInfo->usChiFlags = MjChiFlag::usW;
	if (bT && bZ)
		pActInfo->usChiFlags = MjChiFlag::usTZ;
	if (bT && bW)
		pActInfo->usChiFlags = MjChiFlag::usTW;
	if (bZ && bW)
		pActInfo->usChiFlags = MjChiFlag::usZW;
	if (bT && bZ && bW)
		pActInfo->usChiFlags = MjChiFlag::usTZW;

	return (bT || bZ || bW);
}

//�ж������Ƿ�Ϊ�������������
//@param byCards���� iNums���Ƶ����� byPart������ iPart��������Ƶ�����
//@return true��������

bool CMJLogic::CheckSubArrInside(const int byCards[], int iNums, int byPart[], int iPart)
{
	bool bReturn = true;
	//����ÿ����Ԫ���Ƿ����
	for (int i = 0; i < iPart; i ++)
	{
		bool isFound = false;
		BYTE byTemp = byPart[i];
		for (int j = 0; j < iNums; j ++)
		{
			if (byTemp == byCards[j])
			{
				isFound = true;
				break;
			}
		}
		if (!isFound)
		{
			//ֻҪ��һ��û���ҵ�����Ԫ��  �ͷ���
			bReturn = false;
			break;
		}
	}

	return bReturn;
}

//������������Ƿ����齫����ֵ
//@param byCards���� iNums���Ƶ�����
//@return true����ȫ���齫
bool CMJLogic::CheckArrCards(int byCards[], int iNums)
{
	for (int i = 0; i < iNums; i ++)
	{
		BYTE byTemp = byCards[i];
		if ((byTemp >= 1 && byTemp <= 9) ||
			(byTemp >= 11 && byTemp <= 19) ||
			(byTemp >= 21 && byTemp <= 29) ||
			(byTemp >= 31 && byTemp <= 37))
		{}
		else
		{
			return false;
		}
	}
	return true;
}


//�жϿɷ���
//@param byOutCard�����ƣ�byCards�������ݣ�iNums������
//@return �ܷ���
bool CMJLogic::IsCanPeng(int nOutCard,const int array[],int iNums)
{
	if (!IsValidCard(nOutCard))
	{
		return false;
	}

	return (GetCardNum(array,iNums,nOutCard) >= 2);
}

//�жϿɷ��
//@param byOutCard ��Ҵ�����ƣ��������ܣ���byOutCard = 0ʱ�����жϰ����벹��\
byCards ��������\
iCardNums ��������\
lstCGP �����ܱ������ж�����\
iCGPNums �����ܱ���\
[OUT]pActInfo ���ɸܶ�����Ϣ\
byFetchCard ���ץ������
//@return �ܷ��
bool CMJLogic::IsCanGang(int byOutCard,const int byCards[],int iCardNums,\
	const TMjCGPNode lstCGP[],int iCGPNums,TMjActInfo* pActInfo,int byFetchCard)
{
	////���в����Ը�
	//if(byOutCard == E_MAHJONG_KIND::MJK_FENG + E_MAHJONG_LABEL::LABEL_ZHONG)
	//{
	//	return false;
	//}

	pActInfo->iGangNums = 0;

	if (byOutCard != 0) //�ж�����
	{
		if (GetCardNum(byCards,iCardNums,byOutCard) == 3)
		{
			//��Ӹ�ѡ���
			pActInfo->byGangSels[pActInfo->iGangNums] = byOutCard;
			pActInfo->nGType[pActInfo->iGangNums] = emGangType::gt_MingGang;
			pActInfo->iGangNums++;
		}
	}
	else //�жϰ��ܻ򲹸�
	{
		//�ҳ�����

		BYTE byTemp = 0;
		for (int i = 0; i < iCardNums; ++i)
		{
			if (byTemp != byCards[i])
			{
				if (InCards(byCards[i],pActInfo->byGangSels,pActInfo->iGangNums))
				{
					continue;
				}

				if (GetCardNum(byCards,iCardNums,byCards[i]) >= 4) //�а�����
				{

					byTemp = byCards[i];

					//��Ӹ�ѡ���
					pActInfo->byGangSels[pActInfo->iGangNums] = byCards[i];
					pActInfo->nGType[pActInfo->iGangNums] = emGangType::gt_AnGang;
					pActInfo->iGangNums++;


				}
			}
		}

		//�Ҳ���
		for (int i = 0; i < iCGPNums; ++i)
		{
			if (lstCGP[i].nType == emType::Peng) //������
			{
				//if (byFetchCard == lstCGP[i].val[0])//תת�齫©�ܲ��ܸ�
				if(GetCardNum(byCards,iCardNums,lstCGP[i].val[0])>0)
				{
					//��Ӹ�ѡ���
					pActInfo->byGangSels[pActInfo->iGangNums] = lstCGP[i].val[0];
					pActInfo->nGType[pActInfo->iGangNums] = emGangType::gt_BuGang;
					pActInfo->nBuGangInPeng[pActInfo->iGangNums] = i;
					pActInfo->iGangNums++;
				}
			}
		}
	}

	return (pActInfo->iGangNums > 0);
}
//bool CMJLogic::IsCanGang(int nCard,const int array[],int iCardNums,const TMjCGPNode lstCGP[],int iCGPNums)
//{
//	if (!IsValidCard(nCard))
//	{
//		return false;
//	}
//	if (nCard != 0) //�ж�����
//	{
//		if (3 == GetCardNum(array,iCardNums,nCard))//ֻ����3���������ܻ�ȡ��
//		{
//			//��Ӹ�ѡ���
//			return true;	
//		}
//	}
//	else //�жϰ��ܻ򲹸�
//	{		
//		//�ҳ�����
//		for (int i = 0; i < iCardNums; ++i)
//		{
//			if (GetCardNum(array,iCardNums,array[i]) > 3) //�а�����
//			{
//				return true;			
//			}
//		}
//
//		//�Ҳ���
//		for (int i = 0; i < iCGPNums; ++i)
//		{
//			if (lstCGP[i].nType == emType::Peng) //������
//			{
//				//if (GetACardCount(lstCGP[i].val[0],byCards,iCardNums) > 0) //�����������Ƶ��ƣ��������ɲ���
//				if (IsCardInArray(array, iCardNums,lstCGP[i].val[0]))
//				{
//					return true;
//				}
//			}
//		}
//	}
//
//	return false;
//}

bool CMJLogic::IsCanTing(const int array[],int iNums,int nBaoValue,bool bMaster258Pair)
{
	if (iNums <= 0 || NULL == array )
	{
		return false;
	}
	//�鿴һ�£��������ƻ��ɱ��ƣ����ܲ��ܺ���
	int nTemp [HAND_MAX_NUM_MJ] = {0};
	memcpy(nTemp,array,sizeof(int)*HAND_MAX_NUM_MJ);
	for (int i = 0; i < iNums; i++)
	{
		int nTempValue = nTemp[i];
		nTemp[i] = nBaoValue;
		int nUserBaoNum = GetCardNum(nTemp,iNums,nBaoValue);
		if (IsCanHu_3x2(nTemp, iNums, &nBaoValue,1,nUserBaoNum, bMaster258Pair))
		{
			return true;
		}
		nTemp[i] = nTempValue;
	}
	return false;
}

int CMJLogic::GetHuQidui(int const array[],int iNums, const TMjCGPNode lstCGP[], int iCGPNums, int nBaoValue,int nBaoNums)
{
	if (iNums < 1 || array == NULL)
	{
		return false;
	}
	int nRes = 0;
	if (iCGPNums>0)
	{
		return nRes;
	}

	if (HAND_MAX_NUM_MJ != iNums)
	{
		return false;
	}


	int	byTemp[HAND_MAX_NUM_MJ];
	::memcpy(byTemp, array, sizeof(int) * iNums);


	int dest_num = 0;
	int single_counter = 0;


	for (int i = 0; i < nBaoNums; ++i)
	{
		RemoveCard(byTemp, iNums--,nBaoValue);
	}

	bool bFindFourSame = false;
	for (int i = 0; i < iNums; ++i)
	{
		dest_num = GetCardNum(byTemp, iNums, byTemp[i]);
		if (dest_num == 4)
		{
			i += 3;
			bFindFourSame = true;
		}
		else if (dest_num == 2)
		{
			i += 1;
		}
		else if (dest_num == 3)
		{
			i+=2;
			++single_counter;
		}
		else 
		{
			++single_counter;
		}
	}

	if (nBaoNums >= single_counter)
	{
		nRes = bFindFourSame?2:1;
	}

	return nRes;
}


bool CMJLogic::IsHuJiangYiSe(int const array[],int iNums, const TMjCGPNode lstCGP[], int iCGPNums, int nBaoValue)
{
	if (iNums < 1 || array == NULL)
	{
		return false;
	}

	for (int i = 0; i < iCGPNums; i++)
	{
		if (lstCGP[i].nType == emType::Chi)
		{
			return false;
		}
	}

	for(int i = 0; i < iNums; i++)
	{
		if (array[i] == nBaoValue)
		{ 	
			continue;
		}


		int mod = array[i] % 10;
		if ((mod != LABEL_2) && (mod != LABEL_5) && (mod != LABEL_8))
		{
			return false;
		}
		if (array[i] > MJK_FENG)//�����޷�����
		{
			return false;
		}
	}

	for (int i = 0; i < iCGPNums; i++)
	{
		if (lstCGP[i].nType != emType::UNKNOWN && lstCGP[i].nType != emType::Chi)
		{
			if (lstCGP[i].val[0] > MJK_FENG)//�����޷�����
			{
				return false;
			}

			int mod = lstCGP[i].val[0] % 10;
			if ((mod != LABEL_2) && (mod != LABEL_5) && (mod != LABEL_8))
			{
				return false;
			}
		}
	}

	return true;
}

bool CMJLogic::IsHuQingYiSe(int const array[],int iNums,const TMjCGPNode lstCGP[],int iCGPNums, int nBaoValue,int nBaoNums)
{
	if (iNums < 1 || array == NULL)
	{
		return false;
	}

	int	byTemp[HAND_MAX_NUM_MJ];
	::memcpy(byTemp, array, sizeof(int) * iNums);

	for (int i = 0; i < nBaoNums; ++i)
	{
		RemoveCard(byTemp, iNums--,nBaoValue);
	}
	int hua = -1;
	if (iNums == 0)//�ж����ƵĻ�ɫ��
	{
		if (NULL != lstCGP)
		{
			hua = lstCGP[0].val[0] / 10;
		}
	}
	else
	{
		hua = byTemp[0] / 10;
	}

	if (hua != 1 && hua != 2 && hua != 0)
		return false;

	for (int i = 1; i < iNums; ++i)
	{
		if (hua != byTemp[i] / 10 && byTemp[i] != nBaoValue)
		{
			return false;
		}
	}

	for (int i = 0; i < iCGPNums; ++i)
	{
		if (hua != lstCGP[i].val[0] / 10)
		{
			return false;
		}
	}

	return true;
}

bool CMJLogic::IsHuPengPeng(int const array[],int iNums,const TMjCGPNode lstCGP[],int iCGPNums, int nBaoValue, int nBaoNums)
{

	if (iNums < 1 || array == NULL)
	{
		return false;
	}

	for (int i = 0; i < iCGPNums; i++)
	{
		if (lstCGP[i].nType == emType::Chi)
		{
			return false;
		}
	}


	if (HAND_MAX_NUM_MJ < iNums)
	{
		return false;
	}


	int	byTemp[HAND_MAX_NUM_MJ];
	::memcpy(byTemp, array, sizeof(int) * iNums);


	int dest_num = 0;
	int	peng_counter = 0;
	int	pair_counter = 0;
	int single_counter = 0;


	for (int i = 0; i < nBaoNums; ++i)
	{
		RemoveCard(byTemp, iNums--,nBaoValue);
	}
	if (0 == iNums)
	{
		return true;
	}

	for (int i = 0; i < iNums; ++i)
	{
		dest_num = GetCardNum(byTemp, iNums, byTemp[i]);
		if (dest_num == 3)
		{
			++peng_counter;
			i += 2;
		}
		else if (dest_num == 2)
		{
			++pair_counter;
			i += 1;
		}
		else 
		{
			++single_counter;
		}
	}


	return nBaoValue >= pair_counter - 1 + 2 * single_counter;
}



//�ж��Ƿ��Ƿ�һɫ����һɫȫ�����ɷ���ɵ�
bool CMJLogic::IsHuFengYiSe(int const array[], int iNums,const TMjCGPNode lstCGP[], int iCGPNums,int nBaoValue)
{
	if (NULL == array || iNums <=0)
	{
		return false;
	}
	//����ȫ���������ƻ������
	//ͳ�Ʒ�������
	for (int i = 0; i < iNums; i++)
	{
		if (array[i] != nBaoValue && !(array[i]>MJK_FENG && array[i] <= (MJK_FENG + LABEL_BAI)))
		{
			return false;
		}
	}
	//�ж�һ�������ǲ��Ƕ�������
	for (int i = 0; i < iCGPNums; i++)
	{
		//�ж�һ�£����Ƶĵ�һ��ֵ�ǲ�������
		for (int j = 0; j < 4; j++)
		{
			if (!(lstCGP[i].val[j]>MJK_FENG && lstCGP[i].val[j] <= (MJK_FENG + LABEL_BAI)))
			{
				return false;
			}
		}

	}
	return true;
}

///�õ����byStation����iNum����λ����
///@param byStation���λ�ã�bCW�Ƿ���Ϊ˳ʱ�뷽��(trueΪ˳ʱ�뷽��,falseΪ��ʱ�뷽��),\
iNum������ҵ�����Ĭ��ֵΪ1
///@param return���ص�Ŀ����ҵ�λ��
BYTE CMJLogic::GetNextUserStation(BYTE byStation,bool bCW,int PlayCount,int iNum)
{
	if (iNum <= 0)
	{
		return byStation;
	}

	BYTE byNextStation = byStation;

	if (bCW) //˳ʱ�뷽��
	{
		for (int i = 0; i < iNum; ++i)
		{
			byNextStation = (byNextStation + PlayCount - 1) % PlayCount;
		}
	}
	else//��ʱ�뷽��
	{
		for (int i = 1; i <= iNum; ++i)
		{
			byNextStation = (byNextStation + PlayCount + 1) % PlayCount;
		}
	}

	return byNextStation;
}
bool CMJLogic::Verify(int byPs)
{
	if (byPs >= 1 && byPs <= 9)
	{
		return true;
	}
	else if (byPs >= 11 && byPs <= 19)
	{
		return true;
	}
	else if (byPs >= 21 && byPs <= 29)
	{
		return true;
	}
	else if (byPs >= 31 && byPs <= 37)
	{
		return true;
	}
	else if (byPs >= 41 && byPs <= 48)
	{
		return true;
	}

	return false;
}

//����Դ������û��ָ������
//@param byCardҪ���ҵ��ƣ�bySrc��Դ��iSrcNums������
//@return
bool CMJLogic::InCards(int byCard,const int bySrc[],int iSrcNums)
{
	for (int i = 0; i < iSrcNums; ++i)
	{
		if (byCard == bySrc[i])
		{
			return true;
		}
	}

	return false;
}

//����Դ������û��ָ������
//@param byCardҪ���ҵ��ƣ�bySrc��Դ��iSrcNums������
//@return
bool CMJLogic::InCards(int byCard,const int bySrc[],int iSrcNums,int &nPos)
{
	for (int i = 0; i < iSrcNums; ++i)
	{
		if (byCard == bySrc[i])
		{
			nPos=i;
			return true;
		}
	}

	return false;
}

//ɾ��һ����
//@param byCardҪɾ������,bySrc[]��Դ,iSrcNums��Դ����
//@return �Ƿ�ɹ�ɾ��,����Դ��û���ҵ�byCard�ᵼ�·���false
bool CMJLogic::RemoveOneCard(int byCard,int bySrc[],int iSrcNums)
{
	int i = 0;

	bool bFind = false;

	for (; i < iSrcNums; ++i)
	{
		if (byCard == bySrc[i])
		{
			bySrc[i] = 0;
			bFind = true;
			break;
		}
	}

	if (i < iSrcNums)
	{
		for (; i < iSrcNums - 1; ++i)
		{
			bySrc[i] = bySrc[i + 1];
		}
	}

	return bFind;
}

///ɾ����
//@param byDst[]Ҫɾ����Ŀ����,iDstNumsĿ��������,bySrc[]��Դ,iSrcNums��Դ����
//@return ɾ���Ƶ�����
int CMJLogic::RemoveCards(const int byDst[],int iDstNums,int bySrc[],int iSrcNums)
{
	//��������
	if ((iDstNums>iSrcNums)) 
		return 0;

	//��Ҫɾ����������
	int iDeleteCount=0;
	for (int i=0;i<iDstNums;i++)
	{
		for (int j=0;j<iSrcNums;j++)
		{
			if (byDst[i]==bySrc[j])
			{
				iDeleteCount++;
				bySrc[j]=0;
				break;
			}
		}
	}
	Remove0(bySrc,iSrcNums);
	if (iDeleteCount!=iDstNums)
		return 0;

	return iDeleteCount;
}

//���0λ��
//@param byCards�����ݣ�iNum������
//@return
int CMJLogic::Remove0(int byCards[], int iNum)
{
	int iRemoveCount=0;
	for (int i=0;i<iNum;i++)
	{
		if (byCards[i]!=0) 
			byCards[i-iRemoveCount]=byCards[i];
		else iRemoveCount++;
	}
	return iRemoveCount;
}

int CMJLogic::getAutoOutCard(const int byCards[],int iNums)
{
	BYTE card = 0;

	CheckHuStruct CHStruct;

	CHStruct.Init();

	int iTempNums = iNums;
	int byTemp[HAND_MAX_NUM_MJ];
	::memcpy(byTemp,byCards,sizeof(byTemp));

	sort(byTemp,iTempNums);
	for(int i = 0; i < iTempNums; i++)
	{
		CHStruct.Add(byTemp[i]);
	}

	int singleCards[HAND_MAX_NUM_MJ];
	int iSingeCardsNum = 0;
	for (int i= 0; i< CHStruct.count ; i++)
	{
		if (CHStruct.data[i][1] == 1)
		{
			singleCards[iSingeCardsNum] = CHStruct.data[i][0];
			iSingeCardsNum++;
		}
	}
	if (iSingeCardsNum > 0)
	{
		for (int i = 0 ; i<iSingeCardsNum ; i++ )
		{
			if (isHaveNear2(singleCards[i],byTemp,iTempNums))
			{
				continue;
			}
			card = singleCards[i];
			break;
		}

		if (card == 0 )
		{
			sort(singleCards,iSingeCardsNum);
			card = singleCards[0];
		}
	}

	if (card > 0)
	{
		return card;
	}
	return byTemp[iTempNums - 1];
}

bool CMJLogic::isHaveNear2(int byCard ,int byCards[],int iNums)
{
	bool isHave = false;
	BYTE cardDian = GetDianShu(byCard);
	if (cardDian == 10)
	{
		return false;
	}
	else
	{
		for (int d = 1 ; d <= 2 ; d++)
		{
			int findValue = 0;
			if (cardDian - d > 0)
			{
				findValue = byCard - d;
				if (InCards(findValue,byCards,iNums))
				{
					isHave = true;
					break;
				}
			}

			if (cardDian + d < 10)
			{
				int findValue = byCard + d;
				if (InCards(findValue,byCards,iNums))
				{
					isHave = true;
					break;
				}
			}

		}
	}


	return isHave;
}

int CMJLogic::GetDianShu(int byValue)
{
	if (byValue < 30)
	{
		//��Ͳ��
		return byValue % 10;
	}
	else
	{
		//���Ƽ��ƹ̶�����10��
		return 10;
	}
}
