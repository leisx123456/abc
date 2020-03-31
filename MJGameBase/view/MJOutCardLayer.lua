--region *.lua
--Date
--此文件由[BabeLua]插件自动生成



local MJMessageID = import("..MJMessageID")
local MJOutCardLayer = class("MJOutCardLayer")
local ActModule = import("..module.ActModule")
local UI_MAX_OUTCARD_NUM = 36

local OutCardStartPos = {
    cc.p(640,-127),
    cc.p(1110,650-200),
    cc.p(-345,162),
    cc.p(-250,230-200),
}


function MJOutCardLayer:ctor(ui_project, ui_root)

    self.ui_project = ui_project
    self.ui_root = ui_root
    self.all_out_cards = {}                
    self.all_cards = {}
    
    for i = 1,4 do
        self.all_out_cards[i] = self.ui_project:getChildByName("Panel_player_".. i .."_out")
        self.all_out_cards[i].deskCards = {}        --桌面上所有打出的牌UI（包括不显示的牌）
        self.all_cards[i] = {}
         self.all_cards[i].value ={}
        for j = 1 , UI_MAX_OUTCARD_NUM do
            local card = self.all_out_cards[i]:getChildByName("Image_card_" .. j)
            
            if card ~= nil then
                self.all_cards[i].value[j] = 0
                self.all_out_cards[i].deskCards[j] = card
                self.all_out_cards[i].deskCards[j].endPos=cc.p(card:getChildByName("Image_card"):getPosition())
                card:setColor(UNSELECTED_COLOR)
            end
        end

        --玩家打出的牌 还没有放在桌面上（可以吃碰杠胡）
        local Panel_player_handouts = self.ui_project:getChildByName("Panel_player_handouts")
        self.all_out_cards[i].handoutCard = Panel_player_handouts:getChildByName("Image_handout_" .. i)
        self.all_out_cards[i].handoutCard:setVisible(false)
    end

    self.Image_out_tip = self.ui_project:getChildByName("Image_out_tip")
    self.Image_out_tip:setVisible(false)
    

    --人数相应处理 TODO 二人出的牌太多可能放不下(需要多创建很多打出的牌UI)
    if PLAYER_COUNT == 2 then
        NEW_UI_MAX_OUTCARD_NUM = 48

        for i = 1,4 do
            local X_WIDTH = self.all_out_cards[1].deskCards[2]:getPositionX() - self.all_out_cards[1].deskCards[1]:getPositionX()
            local sign = 1
            if self.all_out_cards[1].deskCards[1]:getPositionY() > self.all_out_cards[1].deskCards[36]:getPositionY() then --由上到下排列
                sign = -1
            end
            if i == 1 then  
                local startPosX ,startPosY = self.all_out_cards[i].deskCards[36]:getPosition()
                for j = 1,NEW_UI_MAX_OUTCARD_NUM do
                    self.all_cards[i].value[j] = 0
                    local index = NEW_UI_MAX_OUTCARD_NUM - j + 1

                    local XLen = index%16 == 0 and 16 or index%16
                    local YLen = math.floor((index-1)/16)*sign
                    local x = startPosX  - (XLen ) * X_WIDTH + 108
                    local y = startPosY -  YLen * 48

                    if j>36 then
                        self.all_out_cards[i].deskCards[j] = cc.CSLoader:createNode("ui_csb/CardNode2.csb")
                        self.all_out_cards[i].deskCards[j]:setPosition(cc.p(x,y))
                        self.all_out_cards[i]:addChild(self.all_out_cards[i].deskCards[j],YLen)
       
                    else
--                        self.all_out_cards[i].deskCards[j]:removeFromParent()
--                        self.all_out_cards[i].deskCards[j] = cc.CSLoader:createNode("ui_csb/CardNode2.csb")
                        self.all_out_cards[i].deskCards[j]:setPosition(cc.p(x,y))
                        --self.all_out_cards[i]:addChild(self.all_out_cards[i].deskCards[j],YLen)
                        self.all_out_cards[i].deskCards[j]:setZOrder(YLen)
                    end
                    self.all_out_cards[i].deskCards[j].endPos=cc.p(self.all_out_cards[i].deskCards[j]:getChildByName("Image_card"):getPosition())
                end
            elseif i == 3 then
                local startPosX ,startPosY = self.all_out_cards[i].deskCards[1]:getPosition()
                local sign = 1
                if self.all_out_cards[i].deskCards[1]:getPositionY() < self.all_out_cards[i].deskCards[36]:getPositionY() then --由上到下排列
                    sign = -1
                end
                for j = 1,NEW_UI_MAX_OUTCARD_NUM do
                    self.all_cards[i].value[j] = 0
                    local index = j
                    local XLen = index%16 == 0 and 16 or index%16
                    local YLen = math.floor((index-1)/16) * sign
                    local x = startPosX  + (16-XLen ) * X_WIDTH - X_WIDTH*13
                    local y = startPosY -  YLen * 48
                    --print("x = ",x," y = ",y)
                    if index>36 then
                        self.all_out_cards[i].deskCards[index] = cc.CSLoader:createNode("ui_csb/CardNode2.csb")
                        self.all_out_cards[i].deskCards[index]:setPosition(cc.p(x,y))
                        self.all_out_cards[i]:addChild(self.all_out_cards[i].deskCards[index],YLen)
                    else
--                        self.all_out_cards[i].deskCards[index]:removeFromParent()
--                        self.all_out_cards[i].deskCards[index] = cc.CSLoader:createNode("ui_csb/CardNode2.csb")
                        self.all_out_cards[i].deskCards[index]:setPosition(cc.p(x,y))
                        --self.all_out_cards[i]:addChild(self.all_out_cards[i].deskCards[index],YLen)
                        self.all_out_cards[i].deskCards[index]:setZOrder(YLen)
                    end
                    self.all_out_cards[i].deskCards[index].endPos=cc.p(self.all_out_cards[i].deskCards[index]:getChildByName("Image_card"):getPosition())
                end
            else    
                for j = 1,NEW_UI_MAX_OUTCARD_NUM do
                    self.all_cards[i].value[j] = 0
                    local startPosX ,startPosY = self.all_out_cards[i].deskCards[1]:getPosition()
                    local index = j
                    local XLen = index%16 == 0 and 16 or index%16
                    local YLen = math.floor((index-1)/16)
                    local x = startPosX  + (XLen ) * X_WIDTH - X_WIDTH*14
                    local y = startPosY -  YLen * 48
                    --print("x = ",x," y = ",y)
                    if index>36 then
                        self.all_out_cards[i].deskCards[index] = cc.CSLoader:createNode("ui_csb/CardNode2.csb")
                        self.all_out_cards[i].deskCards[index]:setPosition(cc.p(x,y))
                        self.all_out_cards[i]:addChild(self.all_out_cards[i].deskCards[index],YLen)
                    else
                        self.all_out_cards[i].deskCards[index]:setPosition(cc.p(x,y))
                    end
                    self.all_out_cards[i].deskCards[index].endPos=cc.p(self.all_out_cards[i].deskCards[index]:getChildByName("Image_card"):getPosition())
                end
            end
        end

        UI_MAX_OUTCARD_NUM = NEW_UI_MAX_OUTCARD_NUM
    else
        UI_MAX_OUTCARD_NUM = 36
    end

    self:onCardColorChange()
    print("**** UI_MAX_OUTCARD_NUM = " , UI_MAX_OUTCARD_NUM)

    print("MJOutCardLayer ctor end")

end

function MJOutCardLayer:ui_open()
    self.ui_project:setVisible(true)
end

function MJOutCardLayer:hide_allDestcards()
    for i = 1,4 do
        for j = 1,UI_MAX_OUTCARD_NUM do
            self.all_out_cards[i].deskCards[j]:getChildByName("Image_card"):setVisible(false)
            self.all_out_cards[i].deskCards[j]:getChildByName("Image_card"):setColor(UNSELECTED_COLOR)
            local image_card = self.all_out_cards[i].deskCards[j]:getChildByName("Image_card")
            if i == 1 or i == 3 then
                image_card:loadTexture(COLOR_RES_PATH[UserData:getCardColor()] .. "Front_Small_01.png")
            else
                image_card:loadTexture(COLOR_RES_PATH[UserData:getCardColor()] .. "Front_Small_03.png")
            end
            local image = image_card:getChildByName("Image_value")
            image:setVisible(true)
        end
    end
end

function MJOutCardLayer:ui_close()
    self.ui_project:setVisible(false)
end


--桌面展示打出的牌 chair_index 椅子位置 isNiaoCard 是否为鸟牌   cardValue打出的牌      outCardTime打出牌的时间
function MJOutCardLayer:showDeskCard(chair_index,isNiaoCard,isAction,card_index,value,outCardTime)
    
    if card_index == nil or card_index <= 0 then
        return
    end

    print("MJOutCardLayer:showDeskCard  chair_index = ",chair_index," card_index = ",card_index)
    if card_index > UI_MAX_OUTCARD_NUM then
        --超出第三排 从第一排显示
        card_index = card_index % UI_MAX_OUTCARD_NUM
    end
    local outcard = self.all_out_cards[chair_index].deskCards[card_index]:getChildByName("Image_card")

    local endPos=cc.p(0,0)
    if self.all_out_cards[chair_index].deskCards[card_index].endPos~=nil then
        endPos=self.all_out_cards[chair_index].deskCards[card_index].endPos
    end
    outcard:setPosition(endPos)
    local image = outcard:getChildByName("Image_value")
    local pos = image:convertToWorldSpace(cc.p(0, 0))

    local fTime=0.2
    if outCardTime~=nil then
        fTime=outCardTime
    end

    outcard:stopAllActions()
    if isAction then
        
        outcard:setPosition(OutCardStartPos[chair_index])
        outcard:setVisible(true)
        local moveto = cc.MoveTo:create(fTime,endPos)
        local callback = cc.CallFunc:create(function()

        end)
        local sequence = cc.Sequence:create(moveto,callback)
        outcard:runAction(sequence)
    else
        outcard:setVisible(true)
    end

   
        
       

    if isNiaoCard then
        outcard:setColor(SELECTED_COLOR)
    end
    --更新桌面上打出的牌 和 闪烁的光标位置
    
--    local value = self.outCardValue
    self.all_cards[chair_index].value[card_index] = value
    if value ~= nil then
        image:loadTexture("res/game_res/MJGameBase/ui_res/Mahjong/Small/Small_"..tostring(value)..".png")

        --上个打出牌玩家的牌上的提示位置微调

        if chair_index == 1 then
            pos.x  = pos.x + 15
            pos.y  = pos.y + 105
        elseif chair_index == 3 then
            pos.x  = pos.x+15
            pos.y  = pos.y +105
        elseif chair_index == 2 then
            pos.x  = pos.x - 35+16
            pos.y  = pos.y + 30+50
        else
            pos.x  = pos.x + 20
            pos.y  = pos.y - 2+65
        end


        --print("update image out tip position ")
       performWithDelay(self.ui_project,
            function()
                self.Image_out_tip:setPosition(pos)
                self.Image_out_tip:setVisible(true)
            end,
        fTime)

        self.Image_out_tip:stopAllActions()
        local actionUp = cc.JumpBy:create(2, cc.p(0,0), 5, 2)
        self.Image_out_tip:runAction(cc.RepeatForever:create(actionUp))

    end
    self.all_out_cards[chair_index].deskCards[card_index]:setVisible(true)
end

--[[--
--将打出的牌并放到桌子上
]]
function MJOutCardLayer:onPutOneCardOnDesk(card_index,value)
    
    local outCardUIIndex = GameSceneModule:getInstance():getGameScene():getGameDataManager():getLastOutUIChair()
    self:showDeskCard(outCardUIIndex,false,true,card_index,value)

end

--[[
--设置打出的那张放大的牌是否显示    
]]
function MJOutCardLayer:setHandoutCardVisible(visible)

    print("***** @@@@@ 设置放大的出的牌 显示 ",visible)
    local dataManager = GameSceneModule:getInstance():getGameScene():getGameDataManager()
    local outCardUIIndex = dataManager:getLastOutUIChair()
    
    if  outCardUIIndex > 0 then
         local handoutCard = self.all_out_cards[outCardUIIndex].handoutCard
        performWithDelay(self.ui_root,
        function()
            handoutCard:setVisible(visible)
        end,
        0.3)
    end


end
--得到令牌
function MJOutCardLayer:onGetToken()

    self:setHandoutCardVisible(false)
    print("MJOutCardLayer:onGetToken end")
end


--某个玩家出牌
function MJOutCardLayer:onOutCardInfo(resp_json)
    print("MJOutCardLayer:onOutCardInfo")
    for i = 1,4 do
        self.all_out_cards[i].handoutCard:setVisible(false)
    end
    local ui_chair_index = GameSceneModule:getInstance():getGameScene():getGameDataManager():getUIChairByServerChair(tonumber(resp_json.outCardUser))
    local cardbg =  self.all_out_cards[tonumber(ui_chair_index)].handoutCard:getChildByName("Image_card")
    local image = cardbg:getChildByName("Image_value")
    image:loadTexture("res/game_res/MJGameBase/ui_res/Mahjong/Big/Big_"..tostring(resp_json.outCardValue)..".png")
    self.all_out_cards[tonumber(ui_chair_index)].handoutCard:setVisible(true)
    --self.outCardValue = resp_json.outCardValue  -- 某个玩家打出的牌
    

    --直接将牌放到桌面上
    self:onPutOneCardOnDesk(resp_json.outCardNum,resp_json.outCardValue)
    print("MJOutCardLayer:onOutCardInfo end")
end

--某个玩家可以执行某个动作
function MJOutCardLayer:onActNotify(resp_json)
    print(" MJOutCardLayer:onActNotify")
    if resp_json == nil then
        local callback = function()
            print("delay over")
            self:setHandoutCardVisible(false)
        end
        --延迟执行
        performWithDelay(self.ui_project,callback,0.5)
    else
        --自己有动作执行
        if resp_json.usFlags == 0 then
            local callback = function()
                print("delay over")
                self:setHandoutCardVisible(false)
            end
            --延迟执行
            performWithDelay(self.ui_project,callback,0.5)
        end
    end

end

--某个玩家执行了动作
function MJOutCardLayer:onActInfo(resp_json)
    print("MJOutCardLayer:onActInfo")
    for i = 1,4 do
        self.all_out_cards[i].handoutCard:setVisible(false)
    end
    
    if resp_json.usActFlag == ActModule.MJActFlag.Guo then

        self:setHandoutCardVisible(false)
    elseif resp_json.usActFlag == ActModule.MJActFlag.Chi or resp_json.usActFlag == ActModule.MJActFlag.Peng then
        --隐藏显示出牌的提示图标
        self.Image_out_tip:setVisible(false)
        --将吃碰杠的这张牌从桌子上移除(暗杠和补杠不用拾起)
        self:pickupCardOndesk(resp_json.byFromUser,resp_json.OutCardNum)
    elseif resp_json.usActFlag == ActModule.MJActFlag.Gang then
        self.Image_out_tip:setVisible(false)
        if resp_json.gangType == ActModule.CPGType.MingGang then
            self:pickupCardOndesk(resp_json.byFromUser,resp_json.OutCardNum)
        end
    end
    print("MJOutCardLayer:onActInfo  end")
end

--吃碰杠胡的时候要将玩家打出的这张牌隐藏
function MJOutCardLayer:pickupCardOndesk(from,outNum)
    print("MJOutCardLayer:pickupCardOndesk")
    if outNum < 0 then
        return
    end
    local dataManager = GameSceneModule:getInstance():getGameScene():getGameDataManager()
    local fromUiChair = dataManager:getUIChairByServerChair(from)
    self.all_out_cards[fromUiChair].deskCards[outNum+1]:setVisible(false)    

end 


--回合算分
function MJOutCardLayer:onGameResult(resp_json)
    print("MJOutCardLayer:onGameResult")

end

--继续游戏
function MJOutCardLayer:onAgreeGame(resp_json)
    print("MJOutCardLayer:onAgreeGame")

    local dataMnger = GameSceneModule:getInstance():getGameScene():getGameDataManager()

    if dataMnger:getSelfChair() ~= resp_json.chair then
        return
    end

    self.Image_out_tip:setVisible(false)
    for i = 1,4 do
        self.all_out_cards[i].handoutCard:setVisible(false)
    end
    self:hide_allDestcards()
end

function MJOutCardLayer:onPlayerStandUp( user_info , desk_info , code)

    print("OutCardUIManager:onPlayerStandUp code = " , code)
    local GameScene = GameSceneModule:getInstance():getGameScene()
    if code == 50 and GameScene:getGameType() == "tz1" then
        local ui_chair_index = GameScene:getGameDataManager():getUIChairByServerChair(user_info.bDeskStation)
        if ui_chair_index == 1 then
            self:ui_close()
        end
    end
end

function MJOutCardLayer:onGameStart(resp_json)
    if GameSceneModule:getInstance():getGameScene():getGameType() == "tz1" then
        self:ui_open()
    end

end


function MJOutCardLayer:onGameStation(resp_json)
    
    if resp_json.GSID==24 then          --跳牌状态
        
    elseif resp_json.GSID==26 then      --玩家打牌状态

        if g_server_obj:get_server_type()=="video" then
            self:hide_allDestcards()

        end
        --显示已经出的牌
        for i= 1,PLAYER_COUNT do
            i = (i-1 + resp_json.TokenUser) % PLAYER_COUNT  + 1
            if resp_json.OutedNums[i] > 0 then
                local ui_chair_index = GameSceneModule:getInstance():getGameScene():getGameDataManager():getUIChairByServerChair(i-1)
                for j = 1,resp_json.OutedNums[i] do
                    --self.outCardValue = resp_json.OutedCards[i][j]
                    self:showDeskCard(ui_chair_index,false,false,j,resp_json.OutedCards[i][j])
                    -- 上个出牌玩家的出牌提示
                    self.Image_out_tip:setVisible(false)
                end
            end
        end
    end
end


--设置牌背颜色
function MJOutCardLayer:onCardColorChange()
    
    for i= 1,4 do
        local cardbg = self.all_out_cards[i].handoutCard:getChildByName("Image_card")
        cardbg:loadTexture( COLOR_RES_PATH[UserData:getCardColor()] .. "Big_01.png")

        if i == 1 or i == 3 then
            for j = 1,UI_MAX_OUTCARD_NUM  do
                self.all_out_cards[i].deskCards[j]:getChildByName("Image_card"):loadTexture( COLOR_RES_PATH[UserData:getCardColor()] .. "Front_Small_01.png")
            end
        else 
            for j = 1,UI_MAX_OUTCARD_NUM  do
                self.all_out_cards[i].deskCards[j]:getChildByName("Image_card"):loadTexture( COLOR_RES_PATH[UserData:getCardColor()] .. "Front_Small_03.png")
            end
        end
    end  
end

function MJOutCardLayer:setOutCardColor(cardvalue)
    print("MJOutCardLayer:setOutCardColor")
  for i = 1,4 do
        for j = 1 , UI_MAX_OUTCARD_NUM do
            if self.all_out_cards[i].deskCards[j] ~= nil and self.all_out_cards[i].deskCards[j]:isVisible() then
                --找到了打出来的牌了，做一下异色处理
                if self.all_cards[i].value[j] == cardvalue then
                    self.all_out_cards[i].deskCards[j]:getChildByName("Image_card"):setColor(SELECTED_COLOR)
                else
                    self.all_out_cards[i].deskCards[j]:getChildByName("Image_card"):setColor(UNSELECTED_COLOR)   
                end
            end
        end
  end
end



function MJOutCardLayer:OnRcvGameMessage(msgID,msgData)
    if msgID == MJMessageID.ASS_GAME_BEGIN then
        
        self:onGameStart()
    elseif msgID == MJMessageID.ASS_MAKE_NT_INFO then
        
    elseif msgID == MJMessageID.ASS_FETCH_HANDCARDS then
        
    elseif msgID == MJMessageID.ASS_TOKEN then
        self:onGetToken(msgData)
    elseif msgID == MJMessageID.ASS_TIAO_CARDS then
        
    elseif msgID == MJMessageID.ASS_OUT_CARD_INFO then
        self:onOutCardInfo(msgData)
    elseif msgID == MJMessageID.ASS_ACT_NOTIFY then
        self:onActNotify(msgData)
    elseif msgID == MJMessageID.ASS_ACT_INFO then
        self:onActInfo(msgData)
    elseif msgID == MJMessageID.ASS_RESULT then
        self:onGameResult(msgData)
    elseif msgID == MJMessageID.ASS_PAIJU_INFO then
        
    elseif msgID == MJMessageID.ASS_GM_AGREE_GAME then
        self:onAgreeGame(msgData)
    elseif msgID == MJMessageID.ASS_GM_GAME_STATION then
        self:onGameStation(msgData)
    end


end


return MJOutCardLayer

--endregion
