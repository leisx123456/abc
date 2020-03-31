--region *.lua
--Date
--此文件由[BabeLua]插件自动生成



local MJPlayersHeadLayer = import("..."..GAME_ID..".view.PlayersHeadLayer")

local MJMessageID = import("..MJMessageID")

local MJCPGMenuLayer = import("..."..GAME_ID..".view.CPGMenuLayer")
local MJHandCardLayer = import("..."..GAME_ID..".view.HandCardLayer")
local MJOutCardLayer = import("..."..GAME_ID..".view.OutCardLayer")
local MJGameBgLayer = import("..."..GAME_ID..".view.GameBgLayer")
local MJMenuLayer = import("..."..GAME_ID..".view.MenuLayer")

local GameLayer = class("GameLayer" ,function(gameScene)
    return CCLayer:create()
end)

function GameLayer:ctor(parent)
    print("GameLayer:ctor")
    self.parent = parent

    local gameLayer =  cc.CSLoader:createNode("ui_csb/MJUIBaseLayer.csb") 
    parent:addChild(gameLayer)

    local Image_root = gameLayer:getChildByName("Image_root")
    self.Image_root = Image_root
    print("GameLayer:ctor     1111")
    self.MJGameBgLayer = MJGameBgLayer.new(Image_root:getChildByName("Panel_base"), self)

    print("GameLayer:ctor     2222")
    self.MJCPGMenuLayer = MJCPGMenuLayer.new(Image_root:getChildByName("Panel_chipeng"), self)
    self.MJCPGMenuLayer:ui_close()
    print("GameLayer:ctor     3333")
    self.MJHandCardLayer = MJHandCardLayer:create(Image_root:getChildByName("Panel_mj_hand"), self)
    self.MJHandCardLayer:ui_close()
    print("GameLayer:ctor     4444")
    self.MJOutCardLayer = MJOutCardLayer:create(Image_root:getChildByName("Panel_mj_out"), self)
    self.MJOutCardLayer:hide_allDestcards()
    print("GameLayer:ctor     5555")
    self.MJPlayersHeadLayer = MJPlayersHeadLayer.new(Image_root:getChildByName("Panel_player"), self)
    self.MJPlayersHeadLayer:ui_close()
    print("GameLayer:ctor     6666")
    self.MJMenuLayer = MJMenuLayer.new(Image_root:getChildByName("Panel_menu"), self)


    
    self:onBgColorChange()

    
    
    if CUR_LANGUAGE_PREFER == LANGUAGE_PREFER.ONLY_PUTONG then
        --如果只有普通话
        UserData:setLanguage( 1 )
    elseif CUR_LANGUAGE_PREFER == LANGUAGE_PREFER.ONLY_FANGYAN then
        --如果只有方言
        UserData:setLanguage( 0 )
    end

    local touch_listener = cc.EventListenerTouchOneByOne:create()
    touch_listener:setSwallowTouches(false) 
    touch_listener:registerScriptHandler(handler(self, self.onTouchBegan), cc.Handler.EVENT_TOUCH_BEGAN)
    touch_listener:registerScriptHandler(handler(self, self.onTouchMoved), cc.Handler.EVENT_TOUCH_MOVED)        
    touch_listener:registerScriptHandler(handler(self, self.onTouchEnded), cc.Handler.EVENT_TOUCH_ENDED)  
    touch_listener:registerScriptHandler(handler(self, self.onTouchCancelled), cc.Handler.EVENT_TOUCH_CANCELLED)     
    local eventDispatcher = self:getEventDispatcher()      
    eventDispatcher:addEventListenerWithSceneGraphPriority(touch_listener, self) 
    print("GameLayer:ctor    end")
end

function GameLayer:onBgColorChange()
    print("GameLayer:onBgColorChange")
    self.Image_root:loadTexture(COLOR_RES_PATH[UserData:getBgColor()].."BJ.jpg")
end

function GameLayer:onTouchBegan(touch, event)
    --print("GameLayer:onTouchBegan")
    local cardNode = self.MJHandCardLayer:getHandCards()
    local checkTingBg = nil
    if self.MJHandCardLayer.CheckTingLayer ~= nil then
        checkTingBg = self.MJHandCardLayer.CheckTingLayer.Image_bg
    end
    local IsClickCardNode = false
    local IsClickTingLayer = false
    for i = 1, 14 do
        local node = cardNode[i]
        if i == 14 then
           node = self.MJHandCardLayer.all_hand_cards[1].needOutCard
        end
        if node and node.Image_card_bg:isVisible() then
            local p = node.Image_card_bg:convertToNodeSpace(touch:getLocation())
            
            local  rect = cc.rect(0, 0, node.Image_card_bg:getContentSize().width, node.Image_card_bg:getContentSize().height)
            if cc.rectContainsPoint(rect, p) then
                
                IsClickCardNode = true
            end 
        end
    end
    if checkTingBg and checkTingBg:isVisible() then
        local p = checkTingBg:convertToNodeSpace(touch:getLocation())
        local  rect = cc.rect(0, 0, checkTingBg:getContentSize().width, checkTingBg:getContentSize().height)
        if cc.rectContainsPoint(rect, p) then
            
            IsClickTingLayer = true
        end 
    end

    --如果说并没有点击牌和查听框，那么就将牌放下，查听框隐藏
    if IsClickCardNode == false and IsClickTingLayer == false then
       self.MJHandCardLayer:resetPlayerHandCardsPosition()
    end
end

function GameLayer:onTouchMoved(touch, event)
    
end

function GameLayer:onTouchEnded(touch, event)
    print("GameLayer:onTouchEnded")

end

function GameLayer:onTouchCancelled(touch, event)

end

function GameLayer:OnRcvGameMessage(msgID,msgData)

    self.MJPlayersHeadLayer:OnRcvGameMessage(msgID,msgData)
    self.MJGameBgLayer:OnRcvGameMessage(msgID,msgData)
    self.MJHandCardLayer:OnRcvGameMessage(msgID,msgData)
    self.MJOutCardLayer:OnRcvGameMessage(msgID,msgData)
    self.MJMenuLayer:OnRcvGameMessage(msgID,msgData)
    self.MJCPGMenuLayer:OnRcvGameMessage(msgID,msgData)
end

return GameLayer


--endregion
