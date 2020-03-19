#pragma once

#include "core/LClockActive.h"
#include "core/LDicePair.h"

#include "mahjong/LMjPlayer.h"
#include "mahjong/LMjLogic.h"
#include "mahjong/LMjDesk.h"

/************************************************************************/
/* 
������齫��Ϸ������
*/
/************************************************************************/
// ����ͨ�������Ĵ��齫
class CSiChuanMjDesk : public CLMjDesk
{
public:
	CSiChuanMjDesk();
	~CSiChuanMjDesk();

	// �����齫�ӣ��Ĵ��齫ֻ������Ͳ
	virtual void allocation();
	virtual int mjNum() const { return 108; }



protected:

};

