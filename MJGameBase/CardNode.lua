--region *.lua
--Date
--麻将牌的触摸类
--此文件由[BabeLua]插件自动生成

 --当向上移动超过这个距离认为要出牌
if MOVEOUT_DISTANCE == nil then
     MOVEOUT_DISTANCE = 80
end   
local MOVEDISTANCE =  100       --当牌移动了这么多的距离的时候认为移动了牌
local IS_TOUCH_SCALE = false    --点击放大效果

local MJAlgorithm = import(".MJAlgorithm")
local CardNode = class("CardNode", function()
    return cc.CSLoader:createNode("ui_csb/CardNode.csb")
end)


--[[
--创建一个玩家的手牌

@param value 当前的牌值
@param card_index 牌的位置（第几张牌）
@param gameLayerManager    游戏层级管理类
]]
function CardNode:ctor(value,card_index,MJHandCardLayer)

    self.MJHandCardLayer = MJHandCardLayer
    self.card_index = card_index      --第几张牌
    self.Image_card_bg = self:getChildByName("Image_card_bg")
    self.Image_card = self.Image_card_bg:getChildByName("Image_card")
    --可听标志
    self.Image_ting_tip = self.Image_card_bg:getChildByName("Image_ting_tip")
    self.Image_ting_tip:setVisible(false)
    --赖子标志
    self.Image_laizi_tip = self.Image_card_bg:getChildByName("Image_laizi_tip")
    self.Image_laizi_tip:setVisible(false)

    self.Image_card_cover = self.Image_card_bg:getChildByName("Image_card_cover")
    self.Image_card_cover:setVisible(false)

    self.isClick = false -- 0.3s内是否已经单击一次
    self.isUp = false -- 点击抬起
    self.isMove = false --是否移动
    self.moveDistance = 0.0 --移动的距离
    self.Value = value
    local touch_listener = cc.EventListenerTouchOneByOne:create()
    touch_listener:setSwallowTouches(false) 
    touch_listener:registerScriptHandler(handler(self, self.onTouchBegan), cc.Handler.EVENT_TOUCH_BEGAN)
    touch_listener:registerScriptHandler(handler(self, self.onTouchMoved), cc.Handler.EVENT_TOUCH_MOVED)        
    touch_listener:registerScriptHandler(handler(self, self.onTouchEnded), cc.Handler.EVENT_TOUCH_ENDED)  
    touch_listener:registerScriptHandler(handler(self, self.onTouchCancelled), cc.Handler.EVENT_TOUCH_CANCELLED)      
    local eventDispatcher = self:getEventDispatcher()      
    eventDispatcher:addEventListenerWithSceneGraphPriority(touch_listener, self.Image_card_bg) 

end

function CardNode:up()
    print("CardNode:up")
    self:setPositionY(CARD_POSITION_Y_UP)
    self.isUp = true
end

function CardNode:down()
    self:setPositionY(CARD_POSITION_Y)
    self.isUp = false
    
end

--归位
function CardNode:BackSelfPos(cardObj)
    if cardObj == nil then
        return
    end
    if cardObj.card_index == 14 then
        cardObj:setPosition((FIRST_LEFT_DIS + (cardObj.card_index-1)*CARDS_DISTANCE + 30) ,CARD_POSITION_Y)
    else 
        cardObj:setPosition((FIRST_LEFT_DIS + (cardObj.card_index-1)*CARDS_DISTANCE) ,CARD_POSITION_Y)
    end
end
function CardNode:onTouchBegan(touch, event)
    --print("CardNode:onTouchBegan")
    --每次点击开始，让没有弹起的牌，归位
    --self:BackSelfPos(self)
    if self:isVisible() == false then
        return false
    end

    --如果当前没有令牌 不能点击牌
    local GameScene = GameSceneModule:getInstance():getGameScene()

    if GameScene:getGameDataManager():getIsCanHandoutCard() == false then
        --print("当前没有令牌 不能出牌")
        return false
    end
    self.s_positon = touch:getLocation()
    self.s_positon_node = self:convertToNodeSpace(touch:getLocation())

    local p = self.Image_card_bg:convertToNodeSpace(touch:getLocation())
    local  rect = cc.rect(2, -30, self.Image_card_bg:getContentSize().width-4, self.Image_card_bg:getContentSize().height + 30)
    if cc.rectContainsPoint(rect, p) then
        
        if self.Image_card_cover:isVisible() == true then
            return false 
        end

        if IS_TOUCH_SCALE then
            self.Image_card_bg:setScale(1.1)
        end

        --当前牌最顶层
        self:setLocalZOrder(10)
        --TODO放大效果

        --TODO右边的牌向右移动

        self.begin_p = touch:getLocation()
        return true
    end 
    return false
end

function CardNode:onTouchMoved(touch, event)
    
    --点击在牌上的点跟随触点移动
    self:setPosition((touch:getLocation().x-self.s_positon_node.x),(touch:getLocation().y-self.s_positon_node.y))
    --牌中心点跟随触点移动
    --self:setPosition(touch:getLocation())

    --计算移动的距离
    local distance = cc.pGetDistance(self.begin_p,touch:getLocation())
    self.moveDistance = self.moveDistance + distance
    
    if self.moveDistance > MOVEDISTANCE then
        self.isMove = true
    end
end

function CardNode:onTouchEnded(touch, event)
    --print("CardNode:onTouchEnded")
    if IS_TOUCH_SCALE then
        self.Image_card_bg:setScale(1.0)
    end
    self:BackSelfPos(self)
    if self.card_index == 14 then
        self:setPosition((FIRST_LEFT_DIS + (self.card_index-1)*CARDS_DISTANCE + 30) ,CARD_POSITION_Y)
    else 
        self:setPosition((FIRST_LEFT_DIS + (self.card_index-1)*CARDS_DISTANCE) ,CARD_POSITION_Y)
    end
    local e_positon = touch:getLocation()

    local GameScene = GameSceneModule:getInstance():getGameScene()
    local HandCardLayer = GameScene.GameLayer.MJHandCardLayer

    if (e_positon.y - self.s_positon.y) > MOVEOUT_DISTANCE then
        --self.MJHandCardLayer:OutCardReqest(self.Value)
        HandCardLayer:OutCardReqest(self.Value)
    else
        if self.isUp then 
            if self.isMove == false then
                self:setPositionY(CARD_POSITION_Y)
                self.isUp = false
                --再次点击抬起的牌就是出牌
                --self.MJHandCardLayer:OutCardReqest(self.Value)
                HandCardLayer:OutCardReqest(self.Value)
            else
                self:setPositionY(CARD_POSITION_Y_UP)
            end
        else
            
            if self.isMove == false then
                HandCardLayer:resetPlayerHandCardsPosition()
                self:setPositionY(CARD_POSITION_Y_UP)
                self.isUp = true
                MJEffectManager:playOtherEffect(EffectEnum.DIANP,false)
                --传到MJHandCardLayer做查看听牌处理 
                HandCardLayer:onCardUp(self.Value)
                HandCardLayer:FindOutCard(self.Value)
            else
                self:setPositionY(CARD_POSITION_Y)
            end
        end    
        
    end
    self.isMove = false
    self.moveDistance = 0.0
    self:setLocalZOrder(0)
end


function CardNode:onTouchCancelled(touch, event)
    if IS_TOUCH_SCALE then
        self.Image_card_bg:setScale(1.0)
    end
    self:BackSelfPos(self)
    self.isMove = false
    self.moveDistance = 0.0
    self:setLocalZOrder(0)
end


--设置牌的牌面
function CardNode:setCardImage(value)
    print("setCardImage value = " .. value)
    if value <= 0 then
       print("Error !!! cardValue illegal")
    end
    self:BackSelfPos(self)
    --检测是否有牌面图片
    local isFileExist = cc.FileUtils:getInstance():isFileExist("res/game_res/MJGameBase/ui_res/Mahjong/Big/Big_"..tostring(value)..".png")
    if isFileExist == false then
        -- 当前牌非法强制提示下线
        -- 隐藏所有手牌
        GameSceneModule:getInstance():getGameScene().GameLayer.MJHandCardLayer:ui_close()
        -- 黑色阴影层遮住
--        local layer = cc.LayerColor:create(cc.convertColor(cc.c3b(0,0,0), "4b"),1280,720)
--        layer:setPosition(0,0)
--        GameSceneModule:getInstance():getGameScene():addChild(layer)

        print("当前麻将图片资源文件未找到 value = "..tostring(value))
        local GameScene = GameSceneModule:getInstance():getGameScene()
        GameScene:getGameLayerManager():showReconnectErrorBox("0x0001")
        self.MJHandCardLayer:ui_close()
    end
    
    self.Image_card:loadTexture("res/game_res/MJGameBase/ui_res/Mahjong/Big/Big_"..tostring(value)..".png")
    self.Value = value

    --是否显示赖子标志
    local allLaizes = GameSceneModule:getInstance():getGameScene():getGameDataManager().gameDataModule.AllLaizies
    if MJAlgorithm:IsInTable(value,allLaizes) then
        print("show lz tip")
        self:setLZTipVisible(true)
    else
        self:setLZTipVisible(false)
    end
end


--是否显示打这张牌可以听的标志
function CardNode:setTingTipVisible(visible)
    print("CardNode  "..tostring(self.Value).."  setTingTipVisible " .. tostring(visible))
    self.Image_ting_tip:setVisible(visible)
end

--是否显示是赖子标识
function CardNode:setLZTipVisible(visible)
    self.Image_laizi_tip:setVisible(visible)
end

--是否显示cover标识
function CardNode:setCoverTipVisible(visible)
    self.Image_card_cover:setVisible(visible)
end

return CardNode

--endregion
