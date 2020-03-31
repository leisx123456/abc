--region *.lua
--Date
--此文件由[BabeLua]插件自动生成
--

local MJMessageID = import("..MJMessageID")
local CardNode = import("..CardNode")
local ActModule = import("..module.ActModule")
local MJMessageID = import("..MJMessageID")
local Algorithm = import("..."..GAME_ID..".Algorithm")


local MJHandCardLayer = class("MJHandCardLayer")


function MJHandCardLayer:ctor(ui_project, ui_root)

    self.ui_project = ui_project
    self.ui_root = ui_root

    
    self.all_hand_cards = {}            --所有玩家的手牌table
    self.all_cards = {}                 --保存所有玩家栏牌的值
    self.Panel_cpg_cards = {}           --所有玩家的吃碰杠牌的Panel数组
    self.Panel_handcards = {}           --所有玩家的手牌的Panel数组

    for i = 1,4 do
        self.all_hand_cards[i] = {}
        self.all_hand_cards[i].cpgCards = {}
        self.all_hand_cards[i].handCards = {}
        self.all_hand_cards[i].resultCards = {}
        self.all_cards[i] = {}
        self.all_cards[i].cpg_cards= {}
        --初始化吃碰杠牌栏
        local playerHandCards = self.ui_project:getChildByName("Panel_player_".. i .."_hand")
        local Panel_cpg_card = playerHandCards:getChildByName("Panel_cpg_cards")

        self.Panel_cpg_cards[i] = Panel_cpg_card
        --是2号位置和4号位置的时候，初始化手牌和栏牌的位置
        if i == 2 or i==4 then
            Panel_cpg_card:setPositionY(-54)
        end
        for j = 1,4 do
            self.all_hand_cards[i].cpgCards[j] = Panel_cpg_card:getChildByName("Panel_cards_" ..j)
            self.all_hand_cards[i].cpgCards[j].cards = {}
            self.all_cards[i].cpg_cards[j] = {}
            self.all_cards[i].cpg_cards[j].value = {}
            for k = 1,4 do
                self.all_hand_cards[i].cpgCards[j].cards[k] = self.all_hand_cards[i].cpgCards[j]:getChildByName("Image_card_"..k):getChildByName("Image_card")
                self.all_cards[i].cpg_cards[j].value[k] = 0
                self.all_hand_cards[i].cpgCards[j].cards[k]:setColor(UNSELECTED_COLOR)
            end
            self.all_hand_cards[i].cpgCards[j]:setVisible(false)
        end
        
        --初始化手牌
        if i == 1 then  --玩家1的手牌由代码生成
            for j = 1,14 do
                --local card = CardNode:create(0,j,self,GameSceneModule:getInstance():getGameScene():getGameLayerManager())
                local card = CardNode:create(0,j,self,self.ui_root.MJHandCardLayer)
                if j == 14 then
                    card:setPosition((FIRST_LEFT_DIS + (j-1)* CARDS_DISTANCE + 30) ,CARD_POSITION_Y)
                    
                    self.all_hand_cards[i].needOutCard = card --最右边需要打出去的牌
                else 
                    card:setPosition((FIRST_LEFT_DIS + (j-1)* CARDS_DISTANCE) ,CARD_POSITION_Y)
                    
                    self.all_hand_cards[i].handCards[j] = card
                end
                card:setVisible(false)
                self.ui_project:addChild(card)
            end
        else
            local Panel_inhand = playerHandCards:getChildByName("Panel_inhand_cards")
            self.Panel_handcards[i] = Panel_inhand
             --是2号位置和4号位置的时候，初始化手牌和栏牌的位置
            if i == 2 or i==4 then
               Panel_inhand:setPositionY(240)
            end
            for j = 1 , 14 do
                local CardNode = Panel_inhand:getChildByName("Image_card_" .. j); 
                if j == 14 then
                    
                    self.all_hand_cards[i].needOutCard = CardNode:getChildByName("Image_card")
                    self.all_hand_cards[i].needOutCard:setVisible(false)
                else
                    self.all_hand_cards[i].handCards[j] = CardNode:getChildByName("Image_card")
                end
                
            end
        end

        --初始化结算的牌 
        for j = 1,14 do
            local ui_resultCards = playerHandCards:getChildByName("Panel_result_cards")
            self.all_hand_cards[i].resultCards[j] = ui_resultCards:getChildByName("Image_card_" .. j):getChildByName("Image_card")
            self.all_hand_cards[i].resultCards[j]:setVisible(false)
        end
        self:updateCardPos(i,0)
    end
    self:onCardColorChange()
    
    --人数处理
    if PLAYER_COUNT == 2 then
        --需要隐藏相应位置的UI
        self.ui_project:getChildByName("Panel_player_2_hand"):setVisible(false)
        self.ui_project:getChildByName("Panel_player_4_hand"):setVisible(false)

    elseif PLAYER_COUNT == 3 then
        --需要隐藏相应位置的UI
        self.ui_project:getChildByName("Panel_player_3_hand"):setVisible(false)
    end
    print("MJHandCardLayer:ctor  end")
end

function MJHandCardLayer:ui_open()
    self.ui_project:setVisible(true)
end

function MJHandCardLayer:ui_close()
    self.ui_project:setVisible(false)
end


--玩家所有牌回到初始位置
function MJHandCardLayer:resetPlayerHandCardsPosition()
    for i=1,13 do
        self.all_hand_cards[1].handCards[i]:down()
    end
    self.all_hand_cards[1].needOutCard:down()
end

--[[
玩家拿到令牌
]]
function MJHandCardLayer:onGetToken(resp_json)
    print("MJHandCardLayer:onGetToken")

    --如果拿到令牌有抓到牌
    if resp_json.TokenbFetch then 
        local ui_chair_index = GameSceneModule:getInstance():getGameScene():getGameDataManager():getUIChairByServerChair(resp_json.TokenbyUser)
        self:onFetchOneCard(ui_chair_index,resp_json.TokenFetcherVal)
    else
        self:checkTingTips()
    end 
    print("MJHandCardLayer:onGetToken end")
end


--某玩家的出牌
function MJHandCardLayer:onOutCardInfo(resp_json)
    print("MJHandCardLayer:onOutCardInfo")
    GameSceneModule:getInstance():getGameScene():getGameDataManager():onSelfOutCardRequest(resp_json.outCardValue)
    local ui_chair_index = GameSceneModule:getInstance():getGameScene():getGameDataManager():getUIChairByServerChair(tonumber(resp_json.outCardUser))
    if ui_chair_index == 1 then

        --录像处理
        if self.all_hand_cards[ui_chair_index].needOutCard:isVisible() then
            self.all_hand_cards[ui_chair_index].needOutCard:setVisible(false)
            self:updateSelfCards()
        end
    else
        --录像处理
        if g_server_obj:get_server_type()=="video" then
            print(" --  录像  ---- 其他玩家出牌")
            self.all_hand_cards[ui_chair_index].resultCards[14]:setVisible(false)
            self.all_hand_cards[ui_chair_index].needOutCard:setVisible(false)
            self:updateResultCardValue(ui_chair_index,resp_json.HandCards)
        else
            self.all_hand_cards[ui_chair_index].needOutCard:setVisible(false)
        end
    end
    MJEffectManager:PlayCardEffect(resp_json.outCardValue,resp_json.outCardUser + 1)
    self:updateSelfCards()
     --隐藏查听提示
    self:hideAllOutTingTips()
    print("--------------------------------HandCardLayer:onOutCardInfo")
end

--[[
--玩家自己打出一张牌请求
@param card_value 牌值
]]
function MJHandCardLayer:OutCardReqest(card_value)
   
    --如果不可以出牌直接返回
    if GameSceneModule:getInstance():getGameScene():getGameDataManager():getIsCanHandoutCard() == false then
        return
    end
    GameSceneModule:getInstance():getGameScene():getGameLayerManager():hideCheckTingLayer()
   -- GameSceneModule:getInstance():getGameScene():getGameDataManager():onSelfOutCardRequest(card_value)

    --隐藏颜色异常处理
    self:FindOutCard(nil)
    self.all_hand_cards[1].needOutCard:setVisible(false)
    --self:updateSelfCards()

    --隐藏查听提示
    self:hideAllOutTingTips()
    
    --发送出牌请求
    local request = {}
    request.cardvalue = tonumber(card_value)
    request.outcardtype = 1
    request.chair = GameSceneModule:getInstance():getGameScene():getGameDataManager():getSelfChair()
    GameSceneModule:getInstance():getGameScene():getHelper():sendGameMessage(MJMessageID.ASS_OUT_CARD_REQ,request)

    GameSceneModule:getInstance():getGameScene():getGameDataManager():setIsCanHandoutCard(false)
    
    GameSceneModule:getInstance():getGameScene():getGameLayerManager():hideCheckTingLayer()
    self.ui_root.MJCPGMenuLayer:ui_close()
    self:resetPlayerHandCardsPosition()

end

--隐藏可听牌的提示
function MJHandCardLayer:hideAllOutTingTips()
    for i,card in pairs (self.all_hand_cards[1].handCards) do
        card:setTingTipVisible(false)
    end
    self.all_hand_cards[1].needOutCard:setTingTipVisible(false)
end

--服务器下发手牌
function MJHandCardLayer:onFetchCards(resp_json)

    self:ui_open()
    local allUserData = GameSceneModule:getInstance():getGameScene():getGameDataManager():getAllUserData()
    local cards = { }
    for i = 1, PLAYER_COUNT do
        cards[i] = allUserData[i].HandCards
    end
    
    local self_chair = GameSceneModule:getInstance():getGameScene():getGameDataManager():getSelfChair()
    for chair = 0, PLAYER_COUNT-1 do
        local index = GameSceneModule:getInstance():getGameScene():getGameDataManager():getUIChairByServerChair(chair)
        
        for i = 1, 13 do
            if index == 1 then

                if self:CheckCardValue(cards[chair + 1][i]) == false then
                    
                    -- 当前牌非法强制提示下线
                    local GameScene = GameSceneModule:getInstance():getGameScene()
                    GameScene:getGameLayerManager():showReconnectErrorBox("0x0000")

                end

                self.all_hand_cards[index].handCards[i]:setCardImage(cards[chair + 1][i])
                self.all_hand_cards[index].handCards[i]:setVisible(true)
                self.all_hand_cards[index].handCards[i].Value = cards[chair + 1][i]
            else
                self:setResultCardValue(index, i, cards[chair + 1][i])
            end
        end
    end
    
end

--检查牌的值是否合法
function MJHandCardLayer:CheckCardValue(value)
    
    if value <= 0 or value > 37 then
        return false
    end

    return true
end



--庄家跳牌
function MJHandCardLayer:onTiaoCards(resp_json)

    local ui_chair_index = GameSceneModule:getInstance():getGameScene():getGameDataManager():getUIChairByServerChair(resp_json.NTUser)
    self:onFetchOneCard(ui_chair_index,resp_json.tiaoFetch)

end

--实时计算牌的位置
function MJHandCardLayer:updateCardPos(ui_chair_index,cpg_nums)

    local panel_cpg = self.Panel_cpg_cards[ui_chair_index]
    local panel_handcard = self.Panel_handcards[ui_chair_index]


    if ui_chair_index == 4 then
        if 0 == cpg_nums then
            panel_cpg:setPositionY(-54)
            panel_handcard:setPositionY(240)
        elseif 1 == cpg_nums then
            panel_cpg:setPositionY(-54)
            panel_handcard:setPositionY(213)
        elseif 2 == cpg_nums then
            panel_cpg:setPositionY(-38)
            panel_handcard:setPositionY(198)
        elseif 3 == cpg_nums then
            panel_cpg:setPositionY(-22)
            panel_handcard:setPositionY(183)
        elseif 4 == cpg_nums then
            panel_cpg:setPositionY(-6)
            panel_handcard:setPositionY(165)
        end
    end

    if ui_chair_index == 2 then
        if 0 == cpg_nums then
            panel_cpg:setPositionY(0)
            panel_handcard:setPositionY(240)
        elseif 1 == cpg_nums then
            panel_cpg:setPositionY(83)
            panel_handcard:setPositionY(246)
        elseif 2 == cpg_nums then
            panel_cpg:setPositionY(67)
            panel_handcard:setPositionY(261)
        elseif 3 == cpg_nums then
            panel_cpg:setPositionY(51)
            panel_handcard:setPositionY(278)
        elseif 4 == cpg_nums then
            panel_cpg:setPositionY(35)
            panel_handcard:setPositionY(295)
        end
    end
end

--[[
处理抓一张牌
@param chair_index 椅子位置
@param card_value 牌值
]]
function MJHandCardLayer:onFetchOneCard(chair_index,card_value)
    
    MJEffectManager:playOtherEffect(EffectEnum.ZHUAP,false)
    if chair_index == 1 then
        self.all_hand_cards[chair_index].needOutCard:setCardImage(card_value)
        self.all_hand_cards[chair_index].needOutCard.Value = card_value
        
        self:checkTingTips()
        self.all_hand_cards[chair_index].needOutCard:setVisible(true)

    else
        self:setResultCardValue( chair_index , 14 , card_value)
    end

    --调整位置
    for j = 1,13 do
        self.all_hand_cards[1].handCards[j]:down()
    end


end

--检查是否显示查听标志
function MJHandCardLayer:checkTingTips()

    if g_server_obj:get_server_type()=="video" then
        return
    end

    local dataMnger = GameSceneModule:getInstance():getGameScene():getGameDataManager()
    if dataMnger:getIsOpenCheckTing() == false then
        return
    end

    --检查是否有七对子的房间选项配置
    local isCan7Dui = dataMnger:getIsCanHu7Dui()

    --获取打几哪张牌可以听
    local selfChair = dataMnger:getSelfChair()
    local selfHandCards = dataMnger:getSelfHandCards()
    local outTingCards = Algorithm:getOutTingCards_L(clone(selfHandCards),dataMnger:getLaizi(),isCan7Dui)
    if #outTingCards > 0 then
        dataMnger:setIsHaveOutTingCards(true)
        
        for i,v in pairs (outTingCards) do
            for j,card in pairs (self.all_hand_cards[1].handCards) do
                if card.Value == v then
                    card:setTingTipVisible(true)
                end
            end

            if self.all_hand_cards[1].needOutCard.Value == v then
                self.all_hand_cards[1].needOutCard:setTingTipVisible(true)
            end
            
        end
    else    
        
        dataMnger:setIsHaveOutTingCards(false)
    end
end


--收到托管消息
function MJHandCardLayer:onAuto(msgData)
    --托管所有牌置灰
    
    if  GameSceneModule:getInstance():getGameScene():getGameType() ~= "tz1" then
        return
    end

    local isSelfAuto = GameSceneModule:getInstance():getGameScene():getGameDataManager():getSelfIsAuto();
    if isSelfAuto then
        for j =1,13 do
            self.all_hand_cards[1].handCards[j]:setCoverTipVisible(true)
        end
        self.all_hand_cards[1].needOutCard:setCoverTipVisible(true)
    else
        for j =1,13 do
            self.all_hand_cards[1].handCards[j]:setCoverTipVisible(false)
        end
        self.all_hand_cards[1].needOutCard:setCoverTipVisible(false)
    end
end

--获取当前牌值对应的麻将子名
function MJHandCardLayer:getImageName(chair_index,card_value)
    if chair_index == 1 then
        return "res/game_res/MJGameBase/ui_res/Mahjong/Big/Big_".. tostring(card_value)..".png"
    else
        return "res/game_res/MJGameBase/ui_res/Mahjong/Small/Small_".. tostring(card_value)..".png"
    end
end

--[[
某玩家执行了某个动作
]]
function MJHandCardLayer:onActInfo(resp_json)
    print("MJHandCardLayer:onActInfo")
    GameSceneModule:getInstance():getGameScene():getGameLayerManager():hideCheckTingLayer()

    if resp_json.usActFlag ==  ActModule.MJActFlag.Guo then
        
    elseif resp_json.usActFlag ==  ActModule.MJActFlag.Chi then
        MJEffectManager:PlayActEffect(ActModule.CPGType.Chi,resp_json.byToUser + 1)
        self:showActFrom(ActModule.CPGType.Chi,resp_json.byFromUser,resp_json.byToUser,resp_json.nIdx+1)
        self:onEatCard(resp_json)
        
    elseif resp_json.usActFlag ==  ActModule.MJActFlag.Peng then
        MJEffectManager:PlayActEffect(ActModule.CPGType.Peng,resp_json.byToUser + 1)
        self:showActFrom(ActModule.CPGType.Peng,resp_json.byFromUser,resp_json.byToUser,resp_json.nIdx+1)
        self:onPengCard(resp_json)
        
    elseif resp_json.usActFlag ==  ActModule.MJActFlag.Gang then
        self:showActFrom(resp_json.gangType,resp_json.byFromUser,resp_json.byToUser,resp_json.nIdx+1)
        self:onGangCard(resp_json)
    elseif resp_json.usActFlag == ActModule.MJActFlag.Ting then    
    elseif resp_json.usActFlag ==  ActModule.MJActFlag.Hu then
        self:onHuCard(resp_json)
    end

    if g_server_obj:get_server_type()=="video" then

        if resp_json.usActFlag ==  ActModule.MJActFlag.Hu then
            for i,v in pairs(resp_json.byToUsers) do
                local chair_index = GameSceneModule:getInstance():getGameScene():getGameDataManager():getUIChairByServerChair(v)
                if chair_index ~= 1 then
                    self:updateResultCardValue(chair_index,resp_json.HandCards[v+1])
                end
            end
        else
            local chair_index = GameSceneModule:getInstance():getGameScene():getGameDataManager():getUIChairByServerChair(resp_json.byToUser)
            if chair_index ~= 1 then
                self:updateResultCardValue(chair_index,resp_json.HandCards)
            end
        end
    end
    
    print("MJHandCardLayer:onActInfo end")
end

--处理吃牌
function MJHandCardLayer:onEatCard(resp_json)
    print("MJHandCardLayer:onEatCard")
    self:hideAllOutTingTips()
    local chair_index = GameSceneModule:getInstance():getGameScene():getGameDataManager():getUIChairByServerChair(resp_json.byToUser)
    --保存牌栏的位置
    local nIndex = resp_json.nIdx + 1
    
    local eatcards = resp_json.cpgCards
    for i = 1,4 do
        local card = self.all_hand_cards[chair_index].cpgCards[nIndex].cards[i]
        if i == 4 then
            card:setVisible(false)
        else
            local image_name = self:getImageName(chair_index,eatcards[i])
            self.all_cards[chair_index].cpg_cards[nIndex].value[i] = eatcards[i]
            card:getChildByName("Image_value"):loadTexture(image_name)
            card:getChildByName("Image_value"):setVisible(true)
            card:setVisible(true)
        end
    end
    self.all_hand_cards[chair_index].cpgCards[nIndex]:setVisible(true)
    
    self:RemoveCards(chair_index,3,true)

    --更新左右两边牌的位置
    self:updateCardPos(chair_index,nIndex)
    --对自己的手牌进行更新
    if chair_index == 1 then
        --更新所有手牌的牌值
        self:updateSelfCards()
    end

end


--[[
从手里拿走几张牌 只是隐藏 非真正移除
@param chair_index 椅子位置
@param removeNum移除了几张牌
@param isShowNeedoutCard是否显示要出的牌
]]
function MJHandCardLayer:RemoveCards(chair_index ,removeNum,isShowNeedoutCard)
    
    for i = 1, 13 do
        --print("start check visible")
        local handcard = self.all_hand_cards[chair_index].handCards[i]
        if handcard == nil then
            print("gethandcard failed")
        end

        if handcard:isVisible() then
            handcard:setVisible(false)
            if chair_index == 1 then
                handcard.Value = -1
            end
            removeNum = removeNum - 1
            if removeNum == 0 then
                break
            end
        end
    end

    if g_server_obj:get_server_type() ~= "video" then
    self.all_hand_cards[chair_index].needOutCard:setVisible(isShowNeedoutCard)
    end


end


--处理碰牌
function MJHandCardLayer:onPengCard(resp_json)
    print("MJHandCardLayer:onPengCard")
    self:hideAllOutTingTips()
    local chair_index = GameSceneModule:getInstance():getGameScene():getGameDataManager():getUIChairByServerChair(resp_json.byToUser)
    print("chair_index = " .. chair_index)
    --更新牌栏
    local nIndex = resp_json.nIdx + 1
    local pengcards = resp_json.cpgCards


    for i = 1,4 do
        local card = self.all_hand_cards[chair_index].cpgCards[nIndex].cards[i]
        if i == 4 then
            card:setVisible(false)
        else
            local image_name = self:getImageName(chair_index,pengcards[i])
           -- self.all_hand_cards[chair_index].cpgCards[nIndex].cards[i].Value = pengcards[i]
            self.all_cards[chair_index].cpg_cards[nIndex].value[i] = pengcards[i]
            card:getChildByName("Image_value"):loadTexture(image_name)
            card:getChildByName("Image_value"):setVisible(true)
            card:setVisible(true)
        end
    end
    self.all_hand_cards[chair_index].cpgCards[nIndex]:setVisible(true)
    self:RemoveCards(chair_index,3,true)
    self:updateCardPos(chair_index,nIndex)
    --对自己的手牌进行更新
    if chair_index == 1 then
        --更新所有手牌的牌值
        self:updateSelfCards()
    end
   
end

--处理杠牌
function MJHandCardLayer:onGangCard(resp_json)
    print("MJHandCardLayer:onGangCard")
    self:hideAllOutTingTips()
    local chair_index = GameSceneModule:getInstance():getGameScene():getGameDataManager():getUIChairByServerChair(resp_json.byToUser)
    --print("chair_index = " .. chair_index)

    local gangType = resp_json.gangType --杠的类型
    if gangType ~= ActModule.CPGType.BuGang then
        self:RemoveCards(chair_index,3,false)
    end

    --明杠暗杠补杠同一个音效
    MJEffectManager:PlayActEffect(gangType,resp_json.byToUser + 1)

    self.all_hand_cards[chair_index].needOutCard:setVisible(false)

    --更新牌栏
    local nIndex = resp_json.nIdx + 1
    
    for i = 1,4 do
        local card = self.all_hand_cards[chair_index].cpgCards[nIndex].cards[i]
        local value = card:getChildByName("Image_value")
        if gangType == ActModule.CPGType.AnGang then
            --暗杠牌栏处理
            
            --记录下 当前位置的杠类型 (暗杠换肤的时候需要用到)
             self.all_hand_cards[chair_index].cpgCards[nIndex].isAnGang = true


            local isShow = false 
            if IS_SHOW_ANGANG_CARD then
                if i == 4 then
                    isShow = true
                end
            else
                if i == 4 and chair_index == 1 then
                    isShow = true
                end
            end

            --if i == 4 and chair_index == 1 then
            if isShow then
                --只有自己的牌才显示暗杠的是什么
                local image_name = self:getImageName(chair_index,resp_json.cpgCards[i])
                self.all_cards[chair_index].cpg_cards[nIndex].value[i] = resp_json.cpgCards[i]
                value:loadTexture(image_name)
                value:setVisible(true)
            else
                --前三张牌翻面显示
                if chair_index == 2 or chair_index == 4  then
                   card:loadTexture(COLOR_RES_PATH[UserData:getCardColor()] .. "Front_Small_06.png")
                   value:setVisible(false)
                elseif chair_index == 3  then 
                   card:loadTexture(COLOR_RES_PATH[UserData:getCardColor()] .. "Front_Small_05.png")
                   value:setVisible(false)
                elseif chair_index == 1  then
                   card:loadTexture(COLOR_RES_PATH[UserData:getCardColor()] .. "Big_03.png")
                   value:setVisible(false)
                end
            end
        else
            
            local image_name = self:getImageName(chair_index,resp_json.cpgCards[i])
            self.all_cards[chair_index].cpg_cards[nIndex].value[i] = resp_json.cpgCards[i]
            value:loadTexture(image_name)
            value:setVisible(true)

           
        end
        --更新左右两边牌的位置  如果是补杠  不能直接传nIndex
        if gangType ~= ActModule.CPGType.BuGang then
            self:updateCardPos(chair_index,nIndex)
        end

        
        card:setVisible(true)
        
    end
    self.all_hand_cards[chair_index].cpgCards[nIndex]:setVisible(true)
    
    --对自己的手牌进行更新
    if chair_index == 1 then
        self:updateSelfCards()
    end
    print("MJHandCardLayer:onGangCard over")
end

--胡牌UI处理
function MJHandCardLayer:onHuCard(resp_json)
     --print("MJHandCardLayer:onHuCard")

     local dataMnger = GameSceneModule:getInstance():getGameScene():getGameDataManager()

     --结算牌红中也要放在前面
     local HandCardsTemp = {}
     for l = 1, #resp_json.HandCards - 1  do
         HandCardsTemp[l] = resp_json.HandCards[l]
     end
     
     for l = 1, #resp_json.HandCards -1  do
         resp_json.HandCards[l] = HandCardsTemp[l]
     end

     --显示胡牌玩家的结算牌
     for i,v in pairs(resp_json.byToUsers) do

        local handcards = dataMnger:getHandCards(v)
        local ui_chair_index = dataMnger:getUIChairByServerChair(v)
        local msgHandcards
        if g_server_obj:get_server_type()=="video" then
            msgHandcards = resp_json.HandCards[v+1] --录像胡牌，json里有所有人的手牌
        else
            msgHandcards = resp_json.HandCards[i]
        end
        for j = 1, 14 do
            local card = self.all_hand_cards[ui_chair_index].resultCards[j]
            if j <= 14 - #msgHandcards then
                card:setVisible(false)
            else
                local image_name = self:getImageName(ui_chair_index, handcards[j + #msgHandcards -14])
                card:getChildByName("Image_value"):loadTexture(image_name)
                card:setVisible(true)
                print("计算牌Y位置",card:getPositionY())

            end
            --隐藏胡牌玩家手牌
            if j ~= 14 then
                self.all_hand_cards[ui_chair_index].handCards[j]:setVisible(false)
            end
        end

        self.all_hand_cards[ui_chair_index].needOutCard:setVisible(false)

     end

    --如果有玩家还没选择(吃碰杠)操作也直接隐藏
    self.ui_root.MJCPGMenuLayer:ui_close()
end



----更新牌栏摆放 展示是哪家吃的牌
--function MJHandCardLayer:showActFrom(actType,from,to,index)

--    if IS_SHOW_CPG_FROM then
--         print("显示吃碰杠牌 actType = ",actType,"from = ",from , "to = ",to , "index = ",index)
--        --保存牌栏的位置
--        local dataManager = GameSceneModule:getInstance():getGameScene():getGameDataManager()
--        local from_ui_chair = dataManager:getUIChairByServerChair(from)
--        local to_ui_chair = dataManager:getUIChairByServerChair(to)
--        print("=======>>from_ui_chair=",from_ui_chair,"to_ui_chair=",to_ui_chair)
--        local cpgCards = self.all_hand_cards[to_ui_chair].cpgCards[index].cards
--        local cardColor = UserData:getCardColor()

--        if ActModule.CPGType.AnGang == actType then
--        elseif ActModule.CPGType.MingGang == actType then
--            --
--        elseif ActModule.CPGType.BuGang == actType then
--            --补杠直接在碰的基础上显示第四张牌
--        elseif ActModule.CPGType.Chi == actType then
--            --吃牌肯定吃上家
--            self.all_hand_cards[to_ui_chair].cpgCards[index]:removeFromParent()
--            self.all_hand_cards[to_ui_chair].cpgCards[index] = cc.CSLoader:createNode("ui_csb/CPGNode"..to_ui_chair.."_"..from_ui_chair..".csb")

--        elseif ActModule.CPGType.Peng == actType then
--            self.all_hand_cards[to_ui_chair].cpgCards[index]:removeFromParent()
--            self.all_hand_cards[to_ui_chair].cpgCards[index] = cc.CSLoader:createNode("ui_csb/CPGNode"..to_ui_chair.."_"..from_ui_chair..".csb")
--        end
--    end


--end



--[[显示当前的牌是谁喂的
    card            需要展示动作来源的牌
    from            谁喂的牌
    chair_index  UI上的椅子位置
]]
function MJHandCardLayer:showActFrom(cpgType, from, to, index)
    if IS_SHOW_CPG_FROM then
        local dataManager = GameSceneModule:getInstance():getGameScene():getGameDataManager()

        local chair_index = dataManager:getUIChairByServerChair(to)
        print("MJHandCardLayer:showActFrom  from = ", from, "to= ", to, "to_ui= ", chair_index, "index= ", index,"type=",cpgType)
        local card = self.all_hand_cards[chair_index].cpgCards[index].cards[1]

        local from_ui_chair_station = dataManager:getUIChairByServerChair(from)

        local imageName = "ui_res/arrows/" .. from_ui_chair_station .. ".png"
        if cpgType==ActModule.CPGType.AnGang then
            if card.tingTip~=nil then
                card.tingTip:setVisible(false)
            end    
        else
            if card.tingTip ~= nil then
                card.tingTip:setTexture(imageName)
                card.tingTip:setVisible(true)
            else
                local tingTip = cc.Sprite:create(imageName)
                tingTip:setAnchorPoint(ccp(0.5, 0.5))
                local tipSize = tingTip:getContentSize()
                local cardsize = card:getContentSize()


                if chair_index == 2 then
                    tingTip:setPosition(cc.p(cardsize.width, cardsize.height - 14))
                elseif chair_index == 3 then
                    tingTip:setPosition(cc.p(cardsize.width - 17, cardsize.height - 42))
                elseif chair_index == 4 then
                    tingTip:setPosition(cc.p(0, cardsize.height - 12))
                else
                    tingTip:setPosition(cc.p(cardsize.width / 2 - 3, cardsize.height / 2 - 28))
                end

                card:addChild(tingTip)
                card.tingTip = tingTip
            end
        end   
    end


end







--回合算分
function MJHandCardLayer:onGameResult(resp_json)
    print("MJHandCardLayer recv game result")
    
    
end

--继续游戏
function MJHandCardLayer:onAgreeGame(resp_json)
    --print("MJHandCardLayer:onAgreeGame")
    local dataMnger = GameSceneModule:getInstance():getGameScene():getGameDataManager()

    local ui_chair_index = dataMnger:getUIChairByServerChair(resp_json.chair)
    print("MJHandCardLayer:onAgreeGame ui_chair_index="..ui_chair_index)
    
    if dataMnger:getSelfChair() ~= resp_json.chair then
        return
    end
    --重新初始化手牌
    self:ui_close() 
    
    for i = 1,4 do
    
        for j = 1,4 do
            self.all_hand_cards[i].cpgCards[j]:setVisible(false)
            if self.all_hand_cards[i].cpgCards[j].isAnGang then
                self.all_hand_cards[i].cpgCards[j].isAnGang = false
            end
            for k = 1,4 do
                local card = self.all_hand_cards[i].cpgCards[j].cards[k]
                self.all_cards[i].cpg_cards[j].value[k] = 0
                self.all_hand_cards[i].cpgCards[j].cards[k]:setColor(UNSELECTED_COLOR)
                card:loadTexture(COLOR_RES_PATH[UserData:getCardColor()] .."Big_01.png")
                local value = card:getChildByName("Image_value")
                value:setVisible(true)

            end
        end
        for j = 1,14 do
            if j == 14 then
                self.all_hand_cards[i].needOutCard :setVisible(false)
            else 
                self.all_hand_cards[i].handCards[j]:setVisible(true)
            end
        end

        for j = 1,14 do
            self.all_hand_cards[i].resultCards[j]:setVisible(false)
        end
    end
    self:hideAllOutTingTips()
end





--自定义排序
function MJHandCardLayer:mySort(cards)
    
end


--对玩家的手牌进行更新
function MJHandCardLayer:updateSelfCards()
   
    local dataMnger = GameSceneModule:getInstance():getGameScene():getGameDataManager()
    local selfChair = dataMnger:getSelfChair()
    local currentcards = clone(dataMnger:getAllUserData()[selfChair + 1].HandCards)
    --更新手里的牌
    local j = 1;
    for i = 1, 13 do
        if self.all_hand_cards[1].handCards[i]:isVisible() then
            if #currentcards >= j then
                self.all_hand_cards[1].handCards[i]:setCardImage(currentcards[j])
                self.all_hand_cards[1].handCards[i].Value = currentcards[j]
                j=j+1
            end
        end
    end

    if self.all_hand_cards[1].needOutCard:isVisible() then
        if #currentcards >= j then
            self.all_hand_cards[1].needOutCard:setCardImage(currentcards[j])
            self.all_hand_cards[1].needOutCard.Value = currentcards[j]
        end
    end

    if g_server_obj:get_server_type() == "video" then
        if #currentcards >= j then
            self.all_hand_cards[1].needOutCard:setCardImage(currentcards[j])
            self.all_hand_cards[1].needOutCard.Value = currentcards[j]
            self.all_hand_cards[1].needOutCard:setVisible(true)
        end
    end

end

function MJHandCardLayer:onCardUp(cardvalue)
    print("MJHandCardLayer:onCardUp,cardvalue is ",cardvalue)
    local dataMnger = GameSceneModule:getInstance():getGameScene():getGameDataManager()
    if dataMnger:getIsOpenCheckTing() == false then
        return
    end

    if dataMnger:getIsHaveOutTingCards() == false then
        return
    end

    GameSceneModule:getInstance():getGameScene():getGameLayerManager():hideCheckTingLayer()

    if self.all_hand_cards[1].needOutCard:isVisible() then

        
        local selfChair = dataMnger:getSelfChair()
        
        local currentcards = clone(dataMnger:getSelfHandCards())
        for i,v in pairs (currentcards) do
            if v == cardvalue then
                table.remove(currentcards,i)
                break
            end
        end
        
        
        local isCan7Dui = GameSceneModule:getInstance():getGameScene():getGameDataManager():getIsCanHu7Dui()
        
        local tingcards = Algorithm:getTingCards_L(currentcards,isCan7Dui)
        print("tingcards is ",tingcards)
        if #tingcards > 0 then
            GameSceneModule:getInstance():getGameScene():getGameLayerManager():showCheckTingLayer(self.ui_root,tingcards,GameSceneModule:getInstance():getGameScene():getGameDataManager():getShowCards())
        end
    end   
end

--牌数据的异色处理
function MJHandCardLayer:FindOutCard(cardvalue)
    if self.all_hand_cards[1].needOutCard:isVisible() then
        --出出去的牌做异色处理
        self.ui_root.MJOutCardLayer:setOutCardColor(cardvalue)
        --栏牌里面的牌的异色处理
        self:setOutCardColor(cardvalue)
    end
end
--更新栏牌里面的牌数据
function MJHandCardLayer:setOutCardColor(cardvalue)
    for i = 1,4 do
        for j = 1,4 do
            for k = 1,4 do
                --首先得看一下，这牌有没有显示出来
                if self.all_hand_cards[i].cpgCards[j].cards[k] ~= nil and self.all_hand_cards[i].cpgCards[j].cards[k]:isVisible() then
                    if self.all_cards[i].cpg_cards[j].value[k] == cardvalue then
                        self.all_hand_cards[i].cpgCards[j].cards[k]:setColor(SELECTED_COLOR)
                    else
                        self.all_hand_cards[i].cpgCards[j].cards[k]:setColor(UNSELECTED_COLOR)
                    end
                end
            end
        end
    end  
end

function MJHandCardLayer:onPlayerStandUp( user_info , desk_info , code)

    print("HandCardUIManager:onPlayerStandUp code = " , code)
    local GameScene = GameSceneModule:getInstance():getGameScene()
    if code == 50 and GameScene:getGameType() == "tz1" then
        local ui_chair_index = GameScene:getGameDataManager():getUIChairByServerChair(user_info.bDeskStation)
        if ui_chair_index == 1 then
            self:ui_close()
        end
    end
end




--收到当前游戏状态信息
function MJHandCardLayer:onGameStation(resp_json)
    print("MJHandCardLayer:onGameStation")

    local dataMnger = GameSceneModule:getInstance():getGameScene():getGameDataManager()

    if resp_json.GSID == nil then
        return
    end
    print("resp_json.GSID = " .. resp_json.GSID )

    if resp_json.GSID==24 then          --跳牌状态
        
    elseif resp_json.GSID== GameStation.GS_PLAYING then      --玩家打牌状态
        
        --当前得令者
        if resp_json.TokenUser ~= resp_json.chair then
            self.all_hand_cards[1].needOutCard:setVisible(false)
        end

        GameSceneModule:getInstance():getGameScene():getGameDataManager():setNTChair(resp_json.NTUser)
        for i = 1,PLAYER_COUNT  do
            local ui_chair_index = GameSceneModule:getInstance():getGameScene():getGameDataManager():getUIChairByServerChair(i-1)

            -- 录像功能先隐藏所有的吃碰杠牌
            if g_server_obj:get_server_type() == "video" then
                for j = 1, 4 do
                    self.all_hand_cards[ui_chair_index].cpgCards[j]:setVisible(false)
                end

                --录像正在播放时隐藏结算牌墙(从最后点后退会保留下来)
                for j = 1,13 do
                    self.all_hand_cards[ui_chair_index].resultCards[j]:setVisible(false)
                    self.all_hand_cards[ui_chair_index].handCards[j]:setVisible(false)                 
                end
                self.all_hand_cards[ui_chair_index].needOutCard:setVisible(false)
            end

            --隐藏手里的牌
            local start = resp_json.CGPNums[i] * 3
            local nCGPNums = 0
            if start > 0 then
                for j=1,start do
                     self.all_hand_cards[ui_chair_index].handCards[j]:setVisible(false)   
                end
                --显示吃碰杠的牌
                
                for j = 1,resp_json.CGPNums[i]  do
                    nCGPNums = nCGPNums+1
                    --
                    self:showActFrom(resp_json.CPGRecords[i][j].usFlags,resp_json.CPGRecords[i][j].from,i-1,j)
                    if resp_json.CPGRecords[i][j].usFlags == ActModule.CPGType.AnGang then
                        --暗杠牌栏处理
                        for k =1,4 do
                            local card = self.all_hand_cards[ui_chair_index].cpgCards[j].cards[k]
                            local value = card:getChildByName("Image_value")
                           
                            local isShow = false 
                            if IS_SHOW_ANGANG_CARD then
                                if k == 4 then
                                    isShow = true
                                end
                            else
                                if k == 4 and ui_chair_index == 1 then
                                    isShow = true
                                end
                            end
                           
                            if isShow then
                                --只有自己的牌才显示暗杠的是什么
                                local image_name = self:getImageName(ui_chair_index,resp_json.CPGRecords[i][j].val[k])
                                self.all_cards[ui_chair_index].cpg_cards[j].value[k] = resp_json.CPGRecords[i][j].val[k]
                                value:loadTexture(image_name)
                                value:setVisible(true)
                            else
                                --前三张牌翻面显示
                                if ui_chair_index == 2 or ui_chair_index == 4  then
                                    card:loadTexture(COLOR_RES_PATH[UserData:getCardColor()] .. "Front_Small_06.png")
                                    value:setVisible(false)
                                elseif ui_chair_index == 3  then 
                                    card:loadTexture(COLOR_RES_PATH[UserData:getCardColor()] .. "Front_Small_05.png")
                                    value:setVisible(false)
                                elseif ui_chair_index == 1  then
                                    card:loadTexture(COLOR_RES_PATH[UserData:getCardColor()] .. "Big_03.png")
                                    value:setVisible(false)
                                end
                            end
                        end
                        self.all_hand_cards[ui_chair_index].cpgCards[j]:setVisible(true)
                        --记录下 当前位置的杠类型 (暗杠换肤的时候需要用到)
                        self.all_hand_cards[ui_chair_index].cpgCards[j].isAnGang = true
                         
                    else 
                       self:showCPGCards(ui_chair_index,j,resp_json.CPGRecords[i][j].val)
                       
                    end

                  
                end
                --断线重新回来，需要更新牌位置
                self:updateCardPos(ui_chair_index,nCGPNums)
            end

            --显示手牌
            if ui_chair_index == 1 then
                local userData = dataMnger:getUserData(resp_json.chair)
                local selfHandCards = userData.HandCards

                for j,v in pairs(selfHandCards) do
                    if j+start == 14 then
                        if self:CheckCardValue(v) then
                            self.all_hand_cards[1].needOutCard:setCardImage(v)
                            self.all_hand_cards[1].needOutCard:setVisible(true)
                        end
                        --断线重连最后一张手牌up
                        --self.all_hand_cards[1].needOutCard:up()
                        self:checkTingTips()
                    elseif j+start < 14 then
                        if self:CheckCardValue(v) then
                            self.all_hand_cards[1].handCards[j+start]:setCardImage(v)
                            self.all_hand_cards[1].handCards[j+start]:setVisible(true)
                        end
                    end
                end
                self:updateSelfCards()
            else
                --录像显示其他人的手牌
                if start > 0 then
                    for k = 1,start do
                        self.all_hand_cards[ui_chair_index].resultCards[k]:setVisible(false)
                    end
                end
                if resp_json.HandCards then
                    for k,v in pairs(resp_json.HandCards[i]) do
                        self:setResultCardValue( ui_chair_index , start + k , v )
                    end
                end
                

            end
        end

        if GameSceneModule:getInstance():getGameScene():getGameType() == "tz1" then
            
            if resp_json.IsAuto then
                for j =1,13 do
                    self.all_hand_cards[1].handCards[j]:setCoverTipVisible(true)
                end
            end
        end

        self:ui_open()
    end
    print("MJHandCardLayer:onGameStation end")
end



function MJHandCardLayer:showCPGCards(chair_index,nIndex,cpgcards)
    
    for i = 1,4 do
        local card = self.all_hand_cards[chair_index].cpgCards[nIndex].cards[i]
        if i == 4 then
            if cpgcards[i] <= 0 then
                card:setVisible(false)
            else
                local image_name = self:getImageName(chair_index,cpgcards[i])
                self.all_cards[chair_index].cpg_cards[nIndex].value[i] = cpgcards[i]
                card:getChildByName("Image_value"):loadTexture(image_name)
                card:getChildByName("Image_value"):setVisible(true)
            end
        else
            local image_name = self:getImageName(chair_index,cpgcards[i])
            self.all_cards[chair_index].cpg_cards[nIndex].value[i] = cpgcards[i]
            card:getChildByName("Image_value"):loadTexture(image_name)
            card:getChildByName("Image_value"):setVisible(true)
        end
    end
   
    self.all_hand_cards[chair_index].cpgCards[nIndex]:setVisible(true)
end



--设置牌背颜色
function MJHandCardLayer:onCardColorChange()
    --print("MJHandCardLayer:onCardColorChange")
    for i = 1,4  do
        --cpgCards
        local cpg_new_texture = nil
        if i == 1 then
            cpg_new_texture = COLOR_RES_PATH[UserData:getCardColor()] .. "Big_01.png"
        elseif i == 3 then
            cpg_new_texture = COLOR_RES_PATH[UserData:getCardColor()] .. "Front_Small_01.png"
        else
            cpg_new_texture = COLOR_RES_PATH[UserData:getCardColor()] .. "Front_Small_03.png"
        end
        local cpg_new_texture_back = nil
        if i == 1 then
            cpg_new_texture_back = COLOR_RES_PATH[UserData:getCardColor()] .. "Big_03.png"
        elseif i == 3 then
            cpg_new_texture_back = COLOR_RES_PATH[UserData:getCardColor()] .. "Front_Small_05.png"
        else
            cpg_new_texture_back = COLOR_RES_PATH[UserData:getCardColor()] .. "Front_Small_06.png"
        end

        for j = 1,4 do
            if self.all_hand_cards[i].cpgCards[j].isAnGang then
                for k = 1,4 do
                    if k == 4 then

                        local isShow = false 
                        if IS_SHOW_ANGANG_CARD then
                            if k == 4 then
                                isShow = true
                            end
                        else
                            if k == 4 and i == 1 then
                                isShow = true
                            end
                        end
                        --如果暗杠第四张牌不展示
                        if isShow then
                            self.all_hand_cards[i].cpgCards[j].cards[k]:loadTexture(cpg_new_texture)
                        else
                            self.all_hand_cards[i].cpgCards[j].cards[k]:loadTexture(cpg_new_texture_back)
                        end
                        
                        
                    else
                        self.all_hand_cards[i].cpgCards[j].cards[k]:loadTexture(cpg_new_texture_back)
                    end
                end
            else
                for k = 1,4 do
                    self.all_hand_cards[i].cpgCards[j].cards[k]:loadTexture(cpg_new_texture)
                end
            end 
        end
       
        local hand_new_texture = nil
        if i == 1 then
            hand_new_texture = COLOR_RES_PATH[UserData:getCardColor()] .. "Big_02.png"
        elseif i == 3 then
            hand_new_texture = COLOR_RES_PATH[UserData:getCardColor()] .. "Front_Small_02.png"
        else
            hand_new_texture = COLOR_RES_PATH[UserData:getCardColor()] .. "Front_Small_04.png"
        end
        for j = 1, 14 do
            if j == 14 then 
                --needOutCard
                if i == 1 then
                  local image = self.all_hand_cards[i].needOutCard:getChildByName("Image_card_bg")
                    image:loadTexture(hand_new_texture)
                 else
                    self.all_hand_cards[i].needOutCard:loadTexture(hand_new_texture)
                 end
            else
                 --handCards 
                 if i == 1 then
                  local image = self.all_hand_cards[i].handCards[j]:getChildByName("Image_card_bg")
                    image:loadTexture(hand_new_texture)
                 else
                    self.all_hand_cards[i].handCards[j]:loadTexture(hand_new_texture)
                 end
            end 
        end                          
        --resultCards
        local result_new_texture = nil
        if i == 1 then
            result_new_texture = COLOR_RES_PATH[UserData:getCardColor()] .. "Big_01.png"
        elseif i == 3 then
            result_new_texture = COLOR_RES_PATH[UserData:getCardColor()] .. "Front_Small_01.png"
        else
            result_new_texture = COLOR_RES_PATH[UserData:getCardColor()] .. "Front_Small_03.png"
        end
        for j = 1, 14 do
            self.all_hand_cards[i].resultCards[j]:loadTexture(result_new_texture)
        end  

    end
    --print("MJHandCardLayer:onCardColorChange end")
end



------------------------------------------------------------------------------------------------
--录像相关
--设置结算牌的牌面并显示 ui_chair_index 玩家的位置索引  2-4   card_index:牌的位置索引 1-14
function MJHandCardLayer:setResultCardValue(ui_chair_index,card_index,card_value)
    
    --print("MJHandCardLayer:setResultCardValue ui_chair_index = " .. ui_chair_index )
    if card_value == nil or card_value <= 0 then
        if card_index == 14 then
            self.all_hand_cards[ui_chair_index].needOutCard:setVisible(true)
        end
        return
    end

    local card = self.all_hand_cards[ui_chair_index].resultCards[card_index]
    local image_name = self:getImageName(ui_chair_index,card_value)
    if card then
        card:getChildByName("Image_value"):loadTexture(image_name)
        card:setVisible(true)
    end
    if card_index == 14 then
        self.all_hand_cards[ui_chair_index].needOutCard:setVisible(false)
    elseif card_index < 14 then
        self.all_hand_cards[ui_chair_index].handCards[card_index]:setVisible(false)
    end

end


--刷新玩家的结算牌  ui_chair_index  玩家的座位号  ,  card_values要显示的牌数组
function MJHandCardLayer:updateResultCardValue(ui_chair_index,card_values)

    print(" --  录像 --  更新玩家 " .. ui_chair_index .. "的结算牌")
     local cardNum = #card_values

     local handleCardNum = 14

     if (cardNum % 3) ~= 2 then
         handleCardNum = 13
     end

     for j = 1 , handleCardNum do
        local card = self.all_hand_cards[ui_chair_index].resultCards[j]
        if j <=  handleCardNum-cardNum then
                card:setVisible(false)
            else
                --print(" show card at index " .. j)
                local image_name = self:getImageName(ui_chair_index,card_values[j+ cardNum - handleCardNum])
                card:getChildByName("Image_value"):loadTexture(image_name)
                card:setVisible(true)
            end
     end
end

function MJHandCardLayer:getHandCards()
    return self.all_hand_cards[1].handCards
end


------------------------------------------------------------------------------------------------
--调试相关

--命令行篡改手牌
function MJHandCardLayer:onCmdChangeCards(cards)

    print("MJHandCardLayer:onCmdChangeCards")
    self:updateSelfCards()
end

------------------------------------------------------------------------------------------------


function MJHandCardLayer:OnRcvGameMessage(msgID,msgData)
    if msgID == MJMessageID.ASS_GAME_BEGIN then
    elseif msgID == MJMessageID.ASS_AUTO_INFO then
        self:onAuto(msgData)
    elseif msgID == MJMessageID.ASS_MAKE_NT_INFO then
        
    elseif msgID == MJMessageID.ASS_FETCH_HANDCARDS then
        self:onFetchCards(msgData)
    elseif msgID == MJMessageID.ASS_TOKEN then
        self:onGetToken(msgData)
    elseif msgID == MJMessageID.ASS_TIAO_CARDS then
        self:onTiaoCards(msgData)
    elseif msgID == MJMessageID.ASS_OUT_CARD_INFO then
        self:onOutCardInfo(msgData)
    elseif msgID == MJMessageID.ASS_ACT_NOTIFY then
        
    elseif msgID == MJMessageID.ASS_ACT_INFO then
        self:onActInfo(msgData)
    elseif msgID == MJMessageID.ASS_RESULT then
        self:onGameResult(msgData)
    elseif msgID == MJMessageID.ASS_PIAO_NOTIFY then
        
    elseif msgID == MJMessageID.ASS_PIAO_INFO then
        
    elseif msgID == MJMessageID.ASS_NIAOCARDS_INFO then
        
    elseif msgID == MJMessageID.ASS_PAIJU_INFO then
        
    elseif msgID == MJMessageID.ASS_GM_AGREE_GAME then
        self:onAgreeGame(msgData)
    elseif msgID == MJMessageID.ASS_GM_GAME_STATION then
        self:onGameStation(msgData)
    end

end

return MJHandCardLayer

--endregion
