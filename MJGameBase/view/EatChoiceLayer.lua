--region *.lua
--Date
--此文件由[BabeLua]插件自动生成
--吃的选择层


local ActModule = import("..module.ActModule")
local MJMessageID = import("..MJMessageID")
local EatChoiceLayer = class("EatChoiceLayer",function() 
        return cc.CSLoader:createNode("ui_csb/EatChoiceLayer.csb") 
    end)


--resp_json服务器返回的json数据
--callback 吃的选择回调 参数为选择的index
-- usChiFlags 自定义吃的选择

function EatChoiceLayer:ctor(parent,choicesNum,resp_json,callback,usChiFlags)
    dump(resp_json,6)
    self.parent = parent
    self.choicesNum = choicesNum
    self.resp_json = resp_json
    self.callback = callback

    local Panel_root = self:getChildByName("Panel_eat_choices")

    local Panel_eat_2_choices = Panel_root:getChildByName("Panel_eat_2_choices")
    local Panel_eat_3_choices = Panel_root:getChildByName("Panel_eat_3_choices")

    if choicesNum == 2 then
        Panel_eat_3_choices:setVisible(false)
        self.Panel_eat_choices = Panel_eat_2_choices
    elseif choicesNum == 3 then
        Panel_eat_2_choices:setVisible(false)
        self.Panel_eat_choices = Panel_eat_3_choices
    end
    
    local Image_bg = self.Panel_eat_choices:getChildByName("Image_bg")
    self.Image_bg = Image_bg

    self.Choices = {}
    for i = 1,choicesNum do
        self.Choices[i] = Image_bg:getChildByName("Panel_eat_choice_" .. i)
        self.Choices[i].Cards = {}
        for j = 1 , 3 do 
            local card_bg = self.Choices[i]:getChildByName("Image_card_" .. j):getChildByName("Image_card")
            card_bg:addClickEventListener(function()
                self:chooseEat(i)
            end)
            self.Choices[i].Cards[j] = card_bg:getChildByName("Image_value")
        end
    end
    
    --上家打出的牌
    local dataMnger = GameSceneModule:getInstance():getGameScene():getGameDataManager()
    --local lastValue = self.parent.MJOutCardLayer.outCardValue
    local lastValue = dataMnger:getLastOutCard()
    if lastValue == nil then
        print(" ERROR !!!获取上家打出的牌失败")
        return
    end

    --客户端显示 可选择的吃牌信息
    local cards = {}

    local eatFlag = resp_json.usChiFlags

    if usChiFlags ~= nil then
        eatFlag = usChiFlags
        self.resp_json.usChiFlags = usChiFlags
    end
    print("eatFlag   = ",eatFlag)

    if eatFlag == ActModule.EatType.usTZ then
        cards = {{lastValue-1,lastValue,lastValue+1},{lastValue,lastValue+1,lastValue+2}}
    elseif eatFlag == ActModule.EatType.usTW then
        cards = {{lastValue-2,lastValue-1,lastValue},{lastValue,lastValue+1,lastValue+2}}
    elseif eatFlag == ActModule.EatType.usZW then
        cards = {{lastValue-2,lastValue-1,lastValue},{lastValue-1,lastValue,lastValue+1}}
    else
        cards = {{lastValue-2,lastValue-1,lastValue},{lastValue-1,lastValue,lastValue+1},{lastValue,lastValue+1,lastValue+2}}
    end

    if lastValue >= 31 and lastValue<= 34 then
        --风牌吃的时候显示特殊处理
        for i,cardsitem in pairs (cards) do
            for j = 1,3 do
                if cardsitem[j] <31 then
                    cardsitem[j] = cardsitem[j] + 4
                elseif cardsitem[j] >34 then
                    cardsitem[j] = cardsitem[j] - 4
                end
            end
        end
    end

    local touch_listener = cc.EventListenerTouchOneByOne:create()
    touch_listener:setSwallowTouches(false) 
    touch_listener:registerScriptHandler(handler(self, self.onTouchBegan), cc.Handler.EVENT_TOUCH_BEGAN)
    local eventDispatcher = self:getEventDispatcher()      
    eventDispatcher:addEventListenerWithSceneGraphPriority(touch_listener, self.Image_bg) 

    self:initEatChoiceCards(cards)
    
end

function EatChoiceLayer:onTouchBegan(touch, event)
    
    local p = self.Image_bg:convertToNodeSpace(touch:getLocation())
    local  rect = cc.rect(0, 0, self.Image_bg:getContentSize().width, self.Image_bg:getContentSize().height)
    if cc.rectContainsPoint(rect, p) then
        
    else
        self:removeFromParent()
        local gameScene = GameSceneModule:getInstance():getGameScene()
        gameScene.GameLayer.MJCPGMenuLayer:reShowMenuLayer()
        gameScene.GameLayer.MJCPGMenuLayer.m_EatChoiceLayer = nil
    end
end


function EatChoiceLayer:initEatChoiceCards(cards)
    
    for i = 1,self.choicesNum do
        for j = 1 , 3 do
            self.Choices[i].Cards[j]:loadTexture("res/game_res/MJGameBase/ui_res/Mahjong/Big/Big_" .. cards[i][j] .. ".png")
            self.Choices[i].Cards[j].Value = cards[i][j]
        end
    end
end

--选择吃法
function EatChoiceLayer:chooseEat(index)
    print("eatchoice  index = " .. index)
    local eat_cards = {}
    for i = 1 , 3 do
        print( "cards value = " .. self.Choices[index].Cards[i].Value)
        eat_cards[i] = self.Choices[index].Cards[i].Value
    end
    
    
    local eatFlag = self.resp_json.usChiFlags
    local request = {}
    request.ActFlag = ActModule.MJActFlag.Chi

    --吃的选择扩展
    if self.callback then
        self.callback(index)
        self:removeFromParent()
        local gameScene = GameSceneModule:getInstance():getGameScene()
        gameScene.GameLayer.MJCPGMenuLayer.m_EatChoiceLayer = nil
        return
    end

    --根据点选的位置 选择吃法  
    if index == 3 then
        request.EatFlag = ActModule.EatType.usT
        
    elseif index == 2 then
        if eatFlag == ActModule.EatType.usTZ or eatFlag == ActModule.EatType.usTW then
            request.EatFlag = ActModule.EatType.usT
        else
            request.EatFlag = ActModule.EatType.usZ
        end

    else
        if eatFlag == ActModule.EatType.usTZ then
            request.EatFlag = ActModule.EatType.usZ
        else
            request.EatFlag = ActModule.EatType.usW
        end
    end
    
    GameSceneModule:getInstance():getGameScene():getHelper():sendGameMessage(MJMessageID.ASS_ACT_REQ,request)

    self:removeFromParent()
    local gameScene = GameSceneModule:getInstance():getGameScene()
    gameScene.GameLayer.MJCPGMenuLayer.m_EatChoiceLayer = nil
end

return EatChoiceLayer
--endregion
