// GameDll.cpp : 定义 DLL 应用程序的入口点。
//

// Windows 头文件:
#include <windows.h>
#include "comstruct.h"
#include "IGameDesk.h"
#include "MjCommonDef.h"
#include "UpgradeMessage.h"
#include "CMJDesk.h"

#ifdef _MANAGED
#pragma managed(push, off)
#endif

//版本定义
#define GAME_MAX_VER					1				//现在最高版本
#define GAME_LESS_VER					1				//现在最低版本
#define GAME_CHANGE_VER					0				//修改版本

///支持类型定义
#define SUP_NORMAL_GAME					0x01			///普通游戏
#define SUP_MATCH_GAME					0x02			///比赛游戏
#define SUP_MONEY_GAME					0x04			///金币游戏

///开发库版本
#define DEV_LIB_VER						1				///开发库版本
#define SUPPER_TYPE						SUP_NORMAL_GAME|SUP_MATCH_GAME|SUP_MONEY_GAME
//文件名字定义
#define GAMENAME						TEXT("麻将Pure")

BOOL APIENTRY DllMain(HMODULE hModule,DWORD ul_reason_for_call,LPVOID lpReserved)
{
	return TRUE;
}

#ifdef _MANAGED
#pragma managed(pop)
#endif


//获取游戏组件信息
extern "C" __declspec(dllexport) BOOL GetServiceInfo(ServerDllInfoStruct * pServiceInfo, UINT uVer)
{
	//效验信息
	if ((uVer!=DEV_LIB_VER)||(pServiceInfo==NULL)) return FALSE;

	//写入信息
	memset(pServiceInfo,0,sizeof(ServerDllInfoStruct));
	pServiceInfo->uServiceVer=GAME_MAX_VER;
	pServiceInfo->uNameID= NAME_ID;
	pServiceInfo->uDeskPeople=PLAY_COUNT;
	pServiceInfo->uSupportType=SUPPER_TYPE;
	lstrcpy(pServiceInfo->szGameName,GAMENAME);
	lstrcpy(pServiceInfo->szWriter,TEXT("  "));
	wsprintf(pServiceInfo->szDllNote, TEXT("%s游戏组件"), GAMENAME);
	//lstrcpy(pServiceInfo->szDllNote,TEXT("牌类 -- 斗地主游戏组件"));
	return TRUE;
}

//创建游戏服务器对象
extern "C" __declspec(dllexport) bool CreateGameServer(IGameDesk **ppGameDesk)
{
   *ppGameDesk = new CMJDesk();
	return true;
}

//销毁已经创建的游戏服务器对象
extern "C" __declspec(dllexport) void DestroyGameServer(IGameDesk **ppIGameDesk)
{
	delete *ppIGameDesk;
	*ppIGameDesk = NULL;
}
