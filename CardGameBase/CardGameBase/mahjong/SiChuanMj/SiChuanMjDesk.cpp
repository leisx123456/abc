#include "SiChuanMjDesk.h"


CSiChuanMjDesk::CSiChuanMjDesk()
{
}


CSiChuanMjDesk::~CSiChuanMjDesk()
{
}


void CSiChuanMjDesk::allocation()
{
	for (int i = 0; i < mjNum(); ++i)
	{
		m_arrMjCard[i] = g_arrMjCardPair[i];
	}
	//memcpy(m_arrMjCard, g_arrMjCardPair, 108 * sizeof(CLMjCard));
}


