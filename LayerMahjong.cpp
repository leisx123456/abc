#include "LayerMahjong.h"
#include "scene/SceneDesk.h"
//#include "sprite/SpriteCard.h"
//#include "assist/MjPointManager.h"


//LayerMahjong* LayerMahjong::create(Node* pNodeRoot)
//{
//	auto pLayerMahjong = new(std::nothrow) LayerMahjong();
//	if (pLayerMahjong && pLayerMahjong->init())
//	{
//		pLayerMahjong->autorelease();
//		return pLayerMahjong;
//	}
//	else
//	{
//		delete pLayerMahjong;
//		pLayerMahjong = NULL;
//		return NULL;
//	}
//}

bool LayerMahjong::init()
{
	if (!Layer::init())
	{
		return false;
	}
	//loadCacheRes();
	_cardSelected = nullptr;

	_rootNode = CSLoader::createNode("LayerMahjong.csb");
	addChild(_rootNode);

	initPaiQiang();
	initHandCard();
	initOutCard();

	auto listener = EventListenerTouchOneByOne::create();
	if (nullptr == listener) {
		log("null");
	}
	listener->onTouchBegan = CC_CALLBACK_2(LayerMahjong::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(LayerMahjong::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(LayerMahjong::onTouchEnded, this);
	listener->setSwallowTouches(true);//不向下传递触摸
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);


	//scheduleOnce(schedule_selector(LayerMahjong::testfun), 2.0f);
	return true;
}



bool LayerMahjong::onTouchBegan(Touch *touch, Event *unused_event)
{

	// 只有当onTouchBegan的返回值是true时才执行后面的两个触摸事件
	return true;
}

void LayerMahjong::onTouchMoved(Touch *touch, Event *unused_event)
{
	Point touchPoint = touch->getLocationInView();
	touchPoint = CCDirector::sharedDirector()->convertToGL(touchPoint);
	if (_cardSelected != NULL)
	{
		_cardSelected->setPosition(touchPoint);
	}
}

void LayerMahjong::onTouchEnded(Touch *touch, Event *unused_event)
{
	Point ptLocal = touch->getLocationInView();
	Point ptGL = Director::getInstance()->convertToGL(ptLocal);
	for (int i = 0; i < 14; ++i)
	{
		if (_sptArrHandCard[0][i]->boundingBox().containsPoint(ptGL))
		{
			if (_cardSelected == _sptArrHandCard[0][i])
			{
				if (bActive)
				{
					_sptArrHandCard[0][i]->onTouched();

					// 说明要出这张牌，请求服务器出牌
					T_MjActOutInfo tMjActOutInfo;
					tMjActOutInfo.nOutCardValue = _cardSelected->value();
					tMjActOutInfo.nOutCardUser = 0;
					SceneDesk * desk = (SceneDesk *)getParent();	// 如果有两层需要2次getParent()
					desk->onUserOutCardRequest(0, tMjActOutInfo);
					return;
				}

			}
			else
			{
				// touch了新的牌 旧的要弹下
				if (_cardSelected)
				{
					_cardSelected->popDown();
				}

			}
			_sptArrHandCard[0][i]->onTouched();
			_cardSelected = _sptArrHandCard[0][i];
			if (!_cardSelected->isPopUp())
			{
				_cardSelected = nullptr;
			}

			break;
		}
	}

	if (_cardSelected != NULL)
	{
		Rect rect = _cardSelected->boundingBox();

		// 请求服务器出牌
		if (ptGL.y > rect.getMaxY())
		{
			//T_MjActOutInfo tMjActOutInfo;
			//tMjActOutInfo.nOutCardValue = _selectedCard->value();
			//tMjActOutInfo.nOutCardUser = 0;
			//SceneDesk * desk = (SceneDesk *)getParent();	// 如果有两层需要2次getParent()
			//desk->onUserOutCard(0, tMjActOutInfo);
			//_selectedCard = NULL;
		}
	}
	//_shadowCard->setVisible(false);
}



void LayerMahjong::SortCard(byte wviewid)
{
	//CMjMachine::SortCardData(wviewid);
	//BYTE cardIndex[MAX_INDEX] = { 0 };
	//MjLogic::getInstance()->SwitchToCardIndex(m_pLocal->m_kHandCardData, m_pLocal->m_kHandCardCout, cardIndex);
}



void LayerMahjong::sortCards(CSpriteMjCard* aCards[], unsigned int unCardCount)
{
	int cardValueTemp;
	for (int i = 0; i < unCardCount - 1; ++i)
	{
		for (int j = i + 1; j < unCardCount; ++j)
		{
			if (aCards[i]->value() > aCards[j]->value())
			{
				cardValueTemp = aCards[i]->value();
				aCards[i]->setValue(aCards[j]->value());
				aCards[j]->setValue(cardValueTemp);
			}
		}
	}
}


void LayerMahjong::setHandleCard(byte cbCardData, byte wBankerViewid)
{

}



void LayerMahjong::showHandCard(byte cbViewId)
{
	//_nodeHand[cbViewId]->removeAllChildren();
	//Point ptHand = MjPointManager::getInstance()->_oriHandCard[cbViewId];
	//Size sizeCard = MjPointManager::getInstance()->_sizeHandDesign[cbViewId];
	//Vec2 vec2Valid = getMjVector(cbViewId) * getValidDirectionValue(cbViewId, sizeCard);
	////组合牌
	//for (int i = 0; i < m_nWeaveCount[cbViewId]; ++i)
	//{
	//	T_WeaveItem tWeaveItem = m_nWeaveItemArray[cbViewId][i];
	//	int nMjNum = 3;
	//	if (tWeaveItem.cbWeaveKind == ACTION_GANG)
	//	{
	//		nMjNum = 4;
	//	}
	//	for (int j = 0; j < nMjNum; ++j)
	//	{
	//		auto mjCard = NodeMjCard::create(cbViewId, tWeaveItem.cbCenterCard);
	//		//如果是杠牌第四张牌特殊处理
	//		if (4 == nMjNum)
	//		{
	//			mjCard->setPosition(ptHand - vec2Valid);
	//			mjCard->setLocalZOrder(100);
	//			_nodeHand[cbViewId]->addChild(mjCard);
	//			break;
	//		}
	//		ptHand = ptHand + vec2Valid;
	//		mjCard->setPosition(ptHand);
	//		_nodeHand[cbViewId]->addChild(mjCard);
	//	}
	//	if (cbViewId == user_view_south)
	//	{
	//		ptHand = ptHand + SPACE_WEAVE_MY * getMjVector(cbViewId);
	//	}
	//	else
	//	{
	//		ptHand = ptHand + SPACE_WEAVE_OTHER * getMjVector(cbViewId);
	//	}
	//}
	////手牌
	//for (int i = 0; i < m_nHandCardCount[cbViewId]; ++i)
	//{
	//	auto mjCard = NodeMjCard::create(cbViewId, m_nHandCard[cbViewId][i]);
	//	ptHand = ptHand + vec2Valid;
	//	mjCard->setPosition(ptHand);
	//	_nodeHand[cbViewId]->addChild(mjCard);
	//}
}


void LayerMahjong::initPaiQiang()
{

	for (int i = 0; i < 108; ++i)
	{
		_sptArrCardWall[i] = (CSpriteMjCard*)_rootNode->getChildByName(StringUtils::format("card_wall_%d", i));
	}
	/* old */
	//Point ptSouth = MjPointManager::getInstance()->_oriHandCard[0];
	//Point ptSouthOri = ptSouth;
	//Size sizeCardSouth = MjPointManager::getInstance()->_sizeHandDesign[user_view_south];

	//Point ptNorth = MjPointManager::getInstance()->_oriHandCard[2];
	//Point ptNorthOri = ptNorth;
	//Size sizeCardNorth = MjPointManager::getInstance()->_sizeHandDesign[user_view_north];
	//for (int i = 0; i < 28; i = i + 2)
	//{
	//	//本人面前的牌墙，理牌时先显示下面再显示上面一张，实际摸牌先摸上面一张，实际对应牌墙1,0,3,2,5,4,7,6
	//	//自己在屏幕下方牌墙摸牌从屏幕右->左, 
	//	NodeMjCard *mjCardBottom = NodeMjCard::create(0, m_nSouthWall[i + 1]);
	//	addChild(mjCardBottom);
	//	ptSouth.x = ptSouth.x + sizeCardSouth.width * (i / 2);
	//	ptSouth.y = ptSouthOri.y + sizeCardSouth.height * 0.3 * (i % 2);
	//	mjCardBottom->setPosition(ptSouth);
	//	_sptArrSouthWall[i + 1] = mjCardBottom;

	//	NodeMjCard *mjCardTop = NodeMjCard::create(0, m_nSouthWall[i]);
	//	addChild(mjCardTop);
	//	ptSouth.y = ptSouthOri.y + sizeCardSouth.height * 0.3 * (i % 2);
	//	mjCardTop->setPosition(ptSouth);
	//	_sptArrSouthWall[i] = mjCardTop;


	//	//对家
	//	//牌墙摸牌屏幕左->右
	//	mjCardBottom = NodeMjCard::create(user_view_north, m_nNorthWal[i + 1]);
	//	addChild(mjCardBottom);
	//	ptNorth.x = ptNorth.x + sizeCardNorth.width * (i / 2);
	//	ptNorth.y = ptNorthOri.y + sizeCardNorth.height * 0.3 * (i % 2);
	//	mjCardBottom->setPosition(ptNorth);
	//	_sptArrNorthWal[i + 1] = mjCardBottom;

	//	mjCardTop = NodeMjCard::create(user_view_north, m_nNorthWal[i]);
	//	addChild(mjCardTop);
	//	ptNorth.y = ptNorthOri.y + sizeCardNorth.height*0.3*((i + 1) % 2);
	//	mjCardTop->setPosition(ptNorthOri);
	//	_sptArrNorthWal[i] = mjCardTop;
	//}

	//Point ptWest = MjPointManager::getInstance()->_oriHandCard[1];
	//Point ptWestOri = ptWest;
	//Size sizeCardOther = MjPointManager::getInstance()->_sizeHandDesign[user_view_east];

	//Point ptEast= MjPointManager::getInstance()->_oriHandCard[2];
	//Point ptEastOri = ptEast;
	//for (int i = 0; i < 26; i = i + 2)
	//{
	//	//上家
	//	//牌墙摸牌上->下
	//	NodeMjCard *mjCardBottom = NodeMjCard::create(user_view_east, m_nEastWall[i + 1]);
	//	addChild(mjCardBottom);
	//	ptEast.y = ptEast.y - sizeCardOther.height * 0.7 * (i / 2);
	//	mjCardBottom->setPosition(ccp(ptEast.x, ptEast.y + sizeCardOther.height*0.4*(i % 2)));
	//	_sptArrEastWall[i + 1] = mjCardBottom;

	//	NodeMjCard *mjCardTop = NodeMjCard::create(user_view_east, m_nEastWall[i]);
	//	addChild(mjCardTop);
	//	ptEast.y = ptEast.y - sizeCardOther.height * 0.7 * ((i + 1) / 2);
	//	mjCardTop->setPosition(ptEast.x, ptEast.y + sizeCardOther.height*0.4*((i + 1) % 2));
	//	_sptArrEastWall[i] = mjCardTop;


	//	//下家
	//	//牌墙摸牌下->上
	//	mjCardBottom = NodeMjCard::create(user_view_west, m_nWestWall[i + 1]);
	//	addChild(mjCardBottom);
	//	ptWest.y = ptWest.y - sizeCardOther.height * 0.7 * (i / 2);
	//	mjCardBottom->setPosition(ccp(ptWest.x, ptEast.y + sizeCardOther.height*0.4*(i % 2)));
	//	_sptArrWestWall[i + 1] = mjCardBottom;

	//	mjCardTop = NodeMjCard::create(user_view_west, m_nWestWall[i]);
	//	addChild(mjCardTop);
	//	ptWest.y = ptWest.y - sizeCardOther.height * 0.7 * ((i + 1) / 2);
	//	mjCardTop->setPosition(ccp(ptWest.x, ptWest.y + sizeCardOther.height*0.4*((i + 1) % 2)));
	//	_sptArrWestWall[i] = mjCardBottom;

	//}
}



void LayerMahjong::updatePaiQiang()
{
	/*for (int i = 0; i < 28; ++i)
	{
	if (m_nSouthWall[i] == CARD_EMPTY)
	{
	if (_sptArrSouthWall[i] != nullptr)
	{
	_sptArrSouthWall[i]->removeFromParent();
	_sptArrSouthWall[i] = nullptr;
	}
	}
	else
	{
	_sptArrSouthWall[i]->setDisplayCard(user_view_south, m_nSouthWall[i]);
	}

	if (m_nNorthWal[i] == CARD_EMPTY)
	{
	if (_sptArrNorthWal[i] != nullptr)
	{
	_sptArrNorthWal[i]->removeFromParent();
	_sptArrNorthWal[i] = nullptr;
	}
	}
	else
	{
	_sptArrNorthWal[i]->setDisplayCard(user_view_north, m_nNorthWal[i]);
	}
	}

	for (int i = 0; i < 26; i = i + 2)
	{
	if (m_nEastWall[i] == CARD_EMPTY)
	{
	if (_sptArrEastWall[i] != nullptr)
	{
	_sptArrEastWall[i]->removeFromParent();
	_sptArrEastWall[i] = nullptr;
	}
	}
	else
	{
	_sptArrEastWall[i]->setDisplayCard(user_view_east, m_nEastWall[i]);
	}

	if (m_nEastWall[i] == CARD_EMPTY)
	{
	if (_sptArrEastWall[i] != nullptr)
	{
	_sptArrEastWall[i]->removeFromParent();
	_sptArrEastWall[i] = nullptr;
	}
	}
	else
	{
	_sptArrEastWall[i]->setDisplayCard(user_view_east, m_nEastWall[i]);
	}

	}*/
}

//void LayerMachine::updatePaiQiangA(byte cbViewId)
//{
//}

void LayerMahjong::showOutCard()
{

}

void LayerMahjong::showOutCard(byte cbCard)
{

}

void LayerMahjong::showOutCard(byte cbViewId, byte cbCard)
{
	outCard(cbViewId, cbCard);

}

void LayerMahjong::initHandCard()
{
	//CLMjMachine::initHandCard();
	////CMjMachine::SortCardData()
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 14; ++j)
		{
			_sptArrHandCard[i][j] = (CSpriteMjCard*)_rootNode->getChildByName(StringUtils::format("card_hand_p%d_%d", i, j));
			_sptArrHandCard[i][j]->setValue(CARD_EMPTY);
		}
	}
	//updatePaiQiang();
}


Vec2 LayerMahjong::getMjVector(byte cbViewId)
{
	// 	E_WallGetDirection eWallGetDirection = E_WallGetDirection(cbViewId);
	Vec2 vec2(0, 0);
	// 	switch (eWallGetDirection)
	// 	{
	// 	case EW_South:
	// 		vec2 = Vec2(1, 0);
	// 		break;
	// 	case EW_West:
	// 		vec2 = Vec2(0, -1);
	// 		break;
	// 	case EW_North:
	// 		vec2 = Vec2(-1, 0);
	// 		break;
	// 	case EW_East:
	// 		vec2 = Vec2(0, 1);
	// 		break;
	// 	default:
	// 		break;
	// 	}
	return vec2;
}

float LayerMahjong::getValidDirectionValue(byte cbViewId, Size size)
{
	// 	E_WallGetDirection eWallGetDirection = E_WallGetDirection(cbViewId);
	float value = 0;
	// 	switch (eWallGetDirection)
	// 	{
	// 	case EW_South:
	// 		value = size.width;
	// 		break;
	// 	case EW_West:
	// 		value = size.height;
	// 		break;
	// 	case EW_North:
	// 		value = size.width;
	// 		break;
	// 	case EW_East:
	// 		value = size.height;
	// 		break;
	// 	default:
	// 		break;
	// 	}
	return value;
}

void LayerMahjong::initOutCard()
{
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 30; ++j)
		{
			_sptArrOutCard[i][j] = (CSpriteMjCard*)_rootNode->getChildByName(StringUtils::format("card_out_p%d_%d", i, j));
			_sptArrOutCard[i][j]->setValue(CARD_EMPTY);

		}
	}
}

void LayerMahjong::dealCards(T_MsgDealCards tMsgDealCards)
{
	for (int i = 0; i < tMsgDealCards.nPlayerCount; i++)
	{
		for (int j = 0; j < 14; j++)
		{
			_sptArrHandCard[i][j]->setValue(tMsgDealCards.arrCardHand[i][j]);
			_sptArrHandCard[i][j]->setViewPosture(CLMjCard::EV_Stand);
		}
	}

	for (int i = 0; i < tMsgDealCards.nMjNumAllocation; ++i)
	{
		_sptArrCardWall[i]->setValue(tMsgDealCards.arrMjCardsPair[i]);
	}

}


void LayerMahjong::outCard(byte cbViewId, byte cbCard)
{
	// 更新手牌
	if (cbViewId == 0)
	{
		int nOutCardIndex = 0;
		Point ptStart = _cardSelected->getPosition();
		Point ptEnd = _sptArrOutCard[0][nOutCardIndex]->getPosition();
		_sptArrOutCard[0][nOutCardIndex]->setValue(cbCard);
		_sptArrOutCard[0][nOutCardIndex]->setPosition(ptStart);
		_sptArrOutCard[0][nOutCardIndex]->runAction(MoveTo::create(0.3, ptEnd));

		// 玩家出牌的时候要把第14张新牌位置的牌插入到出牌的那个位置，然后整理前13张牌
		if (_sptArrOutCard[0][13]->isValid())
		{
			_cardSelected->setValue(_sptArrOutCard[0][13]->value());
			_cardSelected->popDown();
			_cardSelected = nullptr;

			_sptArrOutCard[0][13]->empty();

		}

		//sortCards(_sptArrHandCard[0], 13);
		// 整理数据 整理实际的数目
		// 再给精灵赋值
		
	}
	else
	{
		_sptArrHandCard[cbViewId][14]->empty();
	}

	// 更新出牌区

}







//// 加载麻将资源 --old
//void LayerMahjong::loadCacheRes()
//{
//	//各方向玩家4个
//	for (int i = 0; i < CARD_DIRECTION_NUM; ++i)
//	{
//		for (int j = 0; j < (IMG_MJ_ROW_COUNT * IMG_MJ_COLUMN_COUNT -1); ++j)
//		{
//			byte cardvalue = s_ArrCardValue[j];
//			byte wviewid = i;
//			SpriteFrame* sptframe = nullptr;
//			// 加载牌立值资源
//			if (cardvalue == CARD_STAND_VALUE)
//			{
//				if (i == user_view_west)
//				{
//					Size sizeMj = ImageView::create("SmallRes/majhone_left.png")->getContentSize();
//					auto sptframe = SpriteFrame::create("SmallRes/majhone_left.png", Rect(0, 0, sizeMj.width, sizeMj.height));
//				}
//				else if (i == user_view_east)
//				{
//					Size sizeMj = ImageView::create("RCMahjong/SmallRes/majhone_right.png")->getContentSize();
//					sptframe = SpriteFrame::create("SmallRes/majhone_right.png", Rect(0, 0, sizeMj.width, sizeMj.height));
//				}
//			}
//			else
//			{
//				// 加载牌面值资源
//				char ch[64];
//				sprintf(ch, "SmallRes/majhone_%2d.png", i);
//				string szfile = ch;
//				Size sizeMj = ImageView::create(szfile)->getContentSize();
//
//				float nunitwidth = sizeMj.width / IMG_MJ_COLUMN_COUNT;
//				float nunitheight = sizeMj.height / IMG_MJ_ROW_COUNT;
//				int nRowIndex = j / IMG_MJ_COLUMN_COUNT;
//				int nColumnIndex = j % IMG_MJ_COLUMN_COUNT;
//
//				Rect rcunit = Rect(nColumnIndex * nunitwidth, nRowIndex * nunitheight, nunitwidth, nunitheight);
//				sptframe = SpriteFrame::create(szfile, rcunit);
//
//				if (cardvalue == CARD_BACK_VALUE && wviewid == user_view_south)
//				{
//					wviewid = user_view_north;
//					cardvalue = CARD_STAND_VALUE;
//				}
//			}
//			if (sptframe != nullptr)
//			{
//				char ch[64];
//				sprintf(ch, MJ_FRAME_RES, wviewid, cardvalue);
//				SpriteFrameCache::getInstance()->addSpriteFrame(sptframe, ch);
//			}
//		}
//	}
//
//	// 加载自摸牌提示资源
//	// 胡牌顺序-自摸
//	Size sizeMj = ImageView::create("SmallRes/hu_byuser.png")->getContentSize();
//	float nunitwidth = sizeMj.width / 5;
//	float nunitheight = sizeMj.height / 4;
//	for (int i = 0; i < GAME_PLAYER; ++i)
//	{
//		Rect rcunit = Rect(0, i * nunitheight, nunitwidth, nunitheight);
//		auto sptframe = SpriteFrame::create("SmallRes/hu_byuser.png", rcunit);
//		char ch[64];
//		sprintf(ch, IMG_ZIMO_RES, i);
//		SpriteFrameCache::getInstance()->addSpriteFrame(sptframe, ch);
//	}
//
//
//	// 加载胡牌资源
//	// 胡牌顺序-1胡, 2胡, 3胡, 4胡 
//	for (int i = 0; i < GAME_PLAYER; ++i)
//	{
//		for (int j = 0; j < GAME_PLAYER; ++j)
//		{
//			Rect rcunit = Rect(nunitwidth *(i + 1), j * nunitheight, nunitwidth, nunitheight);
//			auto sptframe = SpriteFrame::create("SmallRes/hu_byuser.png", rcunit);
//			char ch[64];
//			sprintf(ch, IMG_HU_RES, i, j);
//			SpriteFrameCache::getInstance()->addSpriteFrame(sptframe, ch);
//		}
//	}
//}