#include "SiChuanMjPlayer.h"
#include "SiChuanMjLogic.h"

//�ڹ��캯����Ҫ�����麯�����ڻ��๹���ʱ���麯���Ƿ��飬�����ߵ��������У����ǲ��õľ�̬�󶨡�
//��Ȼ���ǣ������ǹ���һ������Ķ���ʱ���ȵ��û���Ĺ��캯�������������л��ಿ�֣����໹û�й��죬��û�г�ʼ����
//����ڻ���Ĺ����е����麯����������ԵĻ����ǵ���һ����û�б���ʼ���Ķ������Ǻ�Σ�յģ�
//����C++���ǲ������ڹ��츸����󲿷ֵ�ʱ�����������麯��ʵ�֡����ǲ���˵�㲻������ôд����
CSiChuanMjPlayer::CSiChuanMjPlayer(E_PlayerType ePlayerType, int nChairID)
: CLMjPlayer(ePlayerType, nChairID)
{
	initLogic();
}

void CSiChuanMjPlayer::initLogic()
{
	m_pMjLogic = new CSiChuanMjLogic();
}

