#pragma once

#include "core/LClockActive.h"
#include "core/LDicePair.h"

#include "mahjong/LMjPlayer.h"
#include "mahjong/LMjLogic.h"
#include "mahjong/LMjDesk.h"

/************************************************************************/
/* 
具体的麻将游戏桌面类
*/
/************************************************************************/
// 最普通的四人四川麻将
class CSiChuanMjDesk : public CLMjDesk
{
public:
	CSiChuanMjDesk();
	~CSiChuanMjDesk();

	// 分配麻将子，四川麻将只需万条筒
	virtual void allocation();
	virtual int mjNum() const { return 108; }



protected:

};

