#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "SpriteMjCard.h"
//#include "mahjong/LMjMachine.h"
#include "mahjong/LMjLogic.h"

USING_NS_CC;
using namespace cocostudio;
using namespace ui;

class NodeMjCard;

class LayerMahjong : public Layer
{
public:
	virtual bool init();

	//static LayerMahjong* create(Node* pNodeRoot);
	CREATE_FUNC(LayerMahjong);

	//void loadCacheRes();
	void initPaiQiang();
	void initHandCard();
	void initOutCard();


	void updatePaiQiang();



	//void updatePaiQiangA(byte cbViewId);

	



	void showHandCard(byte cbViewId);

	void showOutCard(byte cbViewId, byte cbCard);
	void showOutCard(byte cbCard);
	void showOutCard();



	void dealCards(T_MsgDealCards tMsgDealCards);


	//������
	void drawHuCard(int nCard);

	//����
	void outCard(byte cbViewId, byte cbCard);

	//����
	void drawACard();

	void setHandleCard(byte cbCardData, byte wBankerViewid);

	void SortCard(byte wviewid);
	void sortCards(CSpriteMjCard* aCards[], unsigned int unCardCount);

	Vec2 getMjVector(byte cbViewId); //��ȡ��������
	float getValidDirectionValue(byte cbViewId, Size size); //��ȡ��Ч����ֵ


	bool onTouchBegan(Touch *touch, Event *unused_event);
	void onTouchMoved(Touch *touch, Event *unused_event);
	void onTouchEnded(Touch *touch, Event *unused_event);

private:
	Node* _rootNode;
	// ��ǽ
	CSpriteMjCard* _sptArrCardWall[108];

	// ����
	CSpriteMjCard* _sptArrHandCard[4][14];
	CSpriteMjCard* _cardSelected;    //��ѡ�������

	// �ѳ�����
	CSpriteMjCard* _sptArrOutCard[4][30];

	// �����
	//Node* _sptArrWeaveCard[4][16];
	CLMjLogic m_mjLogic;
	 
};

