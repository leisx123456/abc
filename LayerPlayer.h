#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "core/LMjDef.h"

USING_NS_CC;
using namespace cocostudio;
using namespace ui;
using namespace std;


class NodePlayer;

class LayerPlayer 
	: public Layer
{
public:
	virtual bool init();

	LayerPlayer();
	CREATE_FUNC(LayerPlayer);

	void showUserReady(int nChairID);
	void showUserDingQue(T_MsgDingQue tMsgDingQue);
	void showActiveUser(int nChairID);

private:
	NodePlayer* _nodePlayer[4];

};

