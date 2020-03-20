// GameDll.cpp : ���� DLL Ӧ�ó������ڵ㡣
//

// Windows ͷ�ļ�:
#include <windows.h>
#include "comstruct.h"
#include "IGameDesk.h"
#include "MjCommonDef.h"
#include "UpgradeMessage.h"
#include "CMJDesk.h"

#ifdef _MANAGED
#pragma managed(push, off)
#endif

//�汾����
#define GAME_MAX_VER					1				//������߰汾
#define GAME_LESS_VER					1				//������Ͱ汾
#define GAME_CHANGE_VER					0				//�޸İ汾

///֧�����Ͷ���
#define SUP_NORMAL_GAME					0x01			///��ͨ��Ϸ
#define SUP_MATCH_GAME					0x02			///������Ϸ
#define SUP_MONEY_GAME					0x04			///�����Ϸ

///������汾
#define DEV_LIB_VER						1				///������汾
#define SUPPER_TYPE						SUP_NORMAL_GAME|SUP_MATCH_GAME|SUP_MONEY_GAME
//�ļ����ֶ���
#define GAMENAME						TEXT("�齫Pure")

BOOL APIENTRY DllMain(HMODULE hModule,DWORD ul_reason_for_call,LPVOID lpReserved)
{
	return TRUE;
}

#ifdef _MANAGED
#pragma managed(pop)
#endif


//��ȡ��Ϸ�����Ϣ
extern "C" __declspec(dllexport) BOOL GetServiceInfo(ServerDllInfoStruct * pServiceInfo, UINT uVer)
{
	//Ч����Ϣ
	if ((uVer!=DEV_LIB_VER)||(pServiceInfo==NULL)) return FALSE;

	//д����Ϣ
	memset(pServiceInfo,0,sizeof(ServerDllInfoStruct));
	pServiceInfo->uServiceVer=GAME_MAX_VER;
	pServiceInfo->uNameID= NAME_ID;
	pServiceInfo->uDeskPeople=PLAY_COUNT;
	pServiceInfo->uSupportType=SUPPER_TYPE;
	lstrcpy(pServiceInfo->szGameName,GAMENAME);
	lstrcpy(pServiceInfo->szWriter,TEXT("  "));
	wsprintf(pServiceInfo->szDllNote, TEXT("%s��Ϸ���"), GAMENAME);
	//lstrcpy(pServiceInfo->szDllNote,TEXT("���� -- ��������Ϸ���"));
	return TRUE;
}

//������Ϸ����������
extern "C" __declspec(dllexport) bool CreateGameServer(IGameDesk **ppGameDesk)
{
   *ppGameDesk = new CMJDesk();
	return true;
}

//�����Ѿ���������Ϸ����������
extern "C" __declspec(dllexport) void DestroyGameServer(IGameDesk **ppIGameDesk)
{
	delete *ppIGameDesk;
	*ppIGameDesk = NULL;
}
