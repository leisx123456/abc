--region *.lua
--Date
--此文件由[BabeLua]插件自动生成
--杠的选择层


local ActModule = import("..module.ActModule")
local MJMessageID = import("..MJMessageID")
local GangChoiceLayer = class("GangChoiceLayer",function() 
        return cc.CSLoader:createNode("ui_csb/GangChoiceLayer.csb") 
    end)

function GangChoiceLayer:ctor(parent,choicesNum,resp_json,actFlag)
    print("GangChoiceLayer:ctor")
    
    self.parent = parent
    self.choicesNum = choicesNum
    self.actFlag = actFlag
    self.resp_json = resp_json
    local Panel_root = self:getChildByName("Panel_gang_choices")
    local Panel_gang_2_choices = Panel_root:getChildByName("Panel_gang_2_choices")
    local Panel_gang_3_choices = Panel_root:getChildByName("Panel_gang_3_choices")
    print("GangChoiceLayer:ctor  0000")

    if choicesNum == 2 then
        Panel_gang_3_choices:setVisible(false)
        self.Panel_gang_choices = Panel_gang_2_choices
    elseif choicesNum == 3 then
        Panel_gang_2_choices:setVisible(false)
        self.Panel_gang_choices = Panel_gang_3_choices
    end

    local Image_bg = self.Panel_gang_choices:getChildByName("Image_bg")
    self.Image_bg = Image_bg
    print("GangChoiceLayer:ctor  1111")

    
    for i = 1,choicesNum do
        local choice = Image_bg:getChildByName("Panel_gang_choice_" .. i)
        
        for j = 1 , 4 do 
            local card_bg = choice:getChildByName("Image_card_" .. j):getChildByName("Image_card")
            card_bg:addClickEventListener(function()
                self:chooseGang(i)
            end)
            local card = card_bg:getChildByName("Image_value")
            card:loadTexture("res/game_res/MJGameBase/ui_res/Mahjong/Big/Big_" .. resp_json.byGangSels[i] .. ".png")

        end
    end

    local touch_listener = cc.EventListenerTouchOneByOne:create()
    touch_listener:setSwallowTouches(false) 
    touch_listener:registerScriptHandler(handler(self, self.onTouchBegan), cc.Handler.EVENT_TOUCH_BEGAN)
    local eventDispatcher = self:getEventDispatcher()      
    eventDispatcher:addEventListenerWithSceneGraphPriority(touch_listener, self.Image_bg) 


    print("GangChoiceLayer:ctor  end")

    --byGangSels[i]
end

function GangChoiceLayer:onTouchBegan(touch, event)
    
    local p = self.Image_bg:convertToNodeSpace(touch:getLocation())
    local  rect = cc.rect(0, 0, self.Image_bg:getContentSize().width, self.Image_bg:getContentSize().height)
    if cc.rectContainsPoint(rect, p) then

    else
        self:removeFromParent()
        local gameScene = GameSceneModule:getInstance():getGameScene()
        gameScene.GameLayer.MJCPGMenuLayer:reShowMenuLayer()
        gameScene.GameLayer.MJCPGMenuLayer.m_GangChoiceLayer = nil
    end
end

--选择杠法
function GangChoiceLayer:chooseGang(index)
    print("choose gang " .. index)
    local request = {}
    request.ActFlag = ActModule.MJActFlag.Gang
    if self.actFlag then
        request.ActFlag = self.actFlag
    end
    request.nSelIndex = index - 1
    local gameScene = GameSceneModule:getInstance():getGameScene()
    gameScene:getHelper():sendGameMessage(MJMessageID.ASS_ACT_REQ,request)
    self:removeFromParent()
    
    gameScene.GameLayer.MJCPGMenuLayer.m_GangChoiceLayer = nil
end

return GangChoiceLayer

--endregion
