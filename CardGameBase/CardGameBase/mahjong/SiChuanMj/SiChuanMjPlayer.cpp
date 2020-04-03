#include "SiChuanMjPlayer.h"
#include "SiChuanMjLogic.h"

//在构造函数不要调用虚函数。在基类构造的时候，虚函数是非虚，不会走到派生类中，既是采用的静态绑定。
//显然的是：当我们构造一个子类的对象时，先调用基类的构造函数，构造子类中基类部分，子类还没有构造，还没有初始化，
//如果在基类的构造中调用虚函数，如果可以的话就是调用一个还没有被初始化的对象，那是很危险的，
//所以C++中是不可以在构造父类对象部分的时候调用子类的虚函数实现。但是不是说你不可以那么写程序
CSiChuanMjPlayer::CSiChuanMjPlayer(E_PlayerType ePlayerType, int nChairID)
: CLMjPlayer(ePlayerType, nChairID)
{
	initLogic();
}

void CSiChuanMjPlayer::initLogic()
{
	m_pMjLogic = new CSiChuanMjLogic();
}

