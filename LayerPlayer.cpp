#include "LayerPlayer.h"
#include "../node/NodePlayer.h"
USING_NS_CC;


using namespace cocostudio::timeline;


LayerPlayer::LayerPlayer()
{

}


bool LayerPlayer::init()
{
	if (!Layer::init())
	{
		return false;
	}


	Node* pNodeRoot = CSLoader::createNode("LayerPlayer.csb");
	addChild(pNodeRoot);
	
	Node* pNodeUi[4];
	for (int i = 0; i < 4; i++)
	{
		pNodeUi[i] = pNodeRoot->getChildByName(StringUtils::format("FileNode_%d", i));
	}
	pNodeRoot->removeAllChildren();
	for (int i = 0; i < 4; i++)
	{
		_nodePlayer[i] = NodePlayer::create(pNodeUi[i]);
		pNodeRoot->addChild(_nodePlayer[i]);
	}

	return true;
}

void LayerPlayer::showUserReady(int nChairID)
{
	_nodePlayer[nChairID]->setReadyFlag(true);
}

void LayerPlayer::showUserDingQue(T_MsgDingQue tMsgDingQue)
{
	for (int i = 0; i < 4; i++)
	{
		_nodePlayer[i]->setTBAType(CLMjCard::E_MjCardColor(tMsgDingQue.arrCardColor[i]));
	}
}

void LayerPlayer::showActiveUser(int nChairID)
{
	for (int i = 0; i < 4; i++)
	{
		_nodePlayer[i]->stopHeadFalsh();
	}
	_nodePlayer[nChairID]->playHeadFalsh();
}
