#ifndef _SI_CHUAN_MJ_PLAYER_H_
#define _SI_CHUAN_MJ_PLAYER_H_
#include "mahjong/LMjPlayer.h"

class CSiChuanMjPlayer : public CLMjPlayer
{
public:
	CSiChuanMjPlayer(E_PlayerType ePlayerType, int nChairID);
	virtual void initLogic();

private:

};


#endif //_SI_CHUAN_MJ_PLAYER_H_
