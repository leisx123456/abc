--region *.lua
--Date
--麻将游戏最底层的UI管理类
--此文件由[BabeLua]插件自动生成

local MJMessageID = import("..MJMessageID")
local MJGameBgLayer = class("MJGameBgLayer")
local ActModule = import("..module.ActModule")

local MJCMDLayer = import("....Test.MJCMDLayer")
local CONSOLE_UI_ID = 1001

function MJGameBgLayer:ctor(ui_project, ui_root)
    self.AnimPos = {
        cc.p(640,272),
        cc.p(973,413),
        cc.p(640,554),
        cc.p(304,413),
    }

    self.ui_project = ui_project
    self.ui_root = ui_root
   
    local desk_config =  GameSceneModule:getInstance():getGameScene():getDeskInfo()

    --局数
    self.Text_round = self.ui_project:getChildByName("Text_round")
    if GameSceneModule:getInstance():getGameScene():getGameType() == "tz1" then
        self.Text_round:setVisible(false)
    end    

    --房号
    self.Text_roomnum = self.ui_project:getChildByName("Text_roomnum")
    if desk_config ~= nil  then
        self.Text_roomnum:setText("房号：".. tostring(desk_config.roomKey))
    end
    
    --剩余牌
    self.Panel_cards_left = self.ui_project:getChildByName("Panel_cards_left")
    self.Panel_cards_left:setVisible(false)

    local Image_cards_remain_bg = self.Panel_cards_left:getChildByName("Image_cards_remain_bg")
    self.Image_cards_remain_bg = Image_cards_remain_bg
    self.Text_remain = Image_cards_remain_bg:getChildByName("Text_remain")
    
    --房间配置
    self.Text_room_config = self.ui_project:getChildByName("Text_room_config")

    self.Image_room_config_bg = self.ui_project:getChildByName("Image_room_config_bg")

    --水印
    self.Image_shuiyin = self.ui_project:getChildByName("Image_shuiyin")
    self.Image_shuiyin:setVisible(false)

    --系统时间
    self.Text_time = self.ui_project:getChildByName("Text_time")
    if GameSceneModule:getInstance():getGameScene():getGameType() == "tz1" then
        --金币场
         self.Text_roomnum:setVisible(false)
         self.Text_time:setPositionX(640)
    end
         --倒计时背景
    self.TimeCountBg = self.ui_project:getChildByName("TimeCountBg")
    self.TimeCountBg:setVisible(false)
    self.TimeImagebg = self.TimeCountBg:getChildByName("Image_North")

    self.Notice_Up = self.TimeCountBg:getChildByName("Notice_Up")
    self.Notice_Up:setVisible(false)
    self.Notice_left = self.TimeCountBg:getChildByName("Notice_left")
    self.Notice_left:setVisible(false)
    self.Notice_Right = self.TimeCountBg:getChildByName("Notice_Right")
    self.Notice_Right:setVisible(false)
    self.Notice_Down = self.TimeCountBg:getChildByName("Notice_Down")
    self.Notice_Down:setVisible(false)

    self.AtlasLabel_DJS = self.TimeCountBg:getChildByName("AtlasLabel_DJS")
    self.AtlasLabel_DJS:setString(0)
    --控制台按钮
    local Button_cheater = self.ui_project:getChildByName("Button_cheater")
    self.Button_cheater = Button_cheater
    Button_cheater:addClickEventListener(function()
        self.ui_root:addChild(MJCMDLayer:create( 2 , { } ,  function( args )
             dump( args )
             self:Cheat(args)
        end))
    end)
    --Button_cheater:setVisible(CHEATER_OPEN)
    Button_cheater:setVisible(false)

    if desk_config then
        self:updateRound(desk_config.progress + 1)
    end

    self.Text_time:registerScriptHandler(handler(self,self.onNodeEvent))
    self:onCardColorChange()
end

--旋转东南西北位置。这里的chair_index是逻辑位置，不是视图位置
function MJGameBgLayer:RotationTimeCountBg(chair_index)
    --获取自己的椅子号
    local clientChair = GameSceneModule:getInstance():getGameScene():getGameDataManager():getClientChair(chair_index)

    --local rotation = chair_index*90;

    local rotation = clientChair* 90
    self.TimeImagebg:setRotation(rotation) 
end

function MJGameBgLayer:onNodeEvent(event)

    if event == "enter" then
        performWithDelay( self.Text_time , function()
            self:startCountDown()
        end , 0.5)
    elseif event == "exit" then 
        self:stopCountDown()
    end
end

--是否显示剩余牌UI
function MJGameBgLayer:setLeftCardsVisible(visible)
    self.Panel_cards_left:setVisible(visible)
end

--更新剩余张数
function MJGameBgLayer:updateLeftCardsNum(num)
    self.Text_remain:setText(tostring(num))
end

function MJGameBgLayer:updateRound(curRound)
    local allRound = GameSceneModule:getInstance():getGameScene():getGameDataManager():getAllRound()
    if curRound and allRound then
        self.Text_round:setText("局数：".. curRound .. "/" .. allRound)
    else
        print("Error  get curRound or AllRound failed!!! curRound =  ",curRound,"allRound = ",allRound)
    end
end

--服务器下发手牌
function MJGameBgLayer:onFetchCards(resp_json)
    self:updateLeftCardsNum(resp_json.WallLeft)
    self:setLeftCardsVisible(true)
end

--庄家跳牌
function MJGameBgLayer:onTiaoCards(resp_json)
    self:updateLeftCardsNum(resp_json.WallLeft)
end

--某玩家收到令牌
function MJGameBgLayer:onGetToken(resp_json)
    self:updateLeftCardsNum(resp_json.WallLeft)
    self:onPointToTokenUser(resp_json.TokenbyUser)

    self:startMiddleCountDown()
    print("MJGameBgLayer:onGetToken end")
end

--中间开始倒计时
function MJGameBgLayer:startMiddleCountDown()
    self.TimeCountBg:setVisible(true)
    self:stopMiddleCount()
    self.AtlasLabel_DJS:setString(15)
    self.schedulerIDMiddle = cc.Director:getInstance():getScheduler():scheduleScriptFunc( handler(self , self.updateCountDownTime ) , 1.0 ,false)
    
end

--更新倒计时时间
function MJGameBgLayer:updateCountDownTime()
    --print("MJGameBgLayer:updateCountDownTime ")
    if self.schedulerIDMiddle ~= nil then
        self.curCountDountTime = self.curCountDountTime - 1
        if self.curCountDountTime>= 0 then
            if  self.AtlasLabel_DJS ~= nil then
                self.AtlasLabel_DJS:setString(self.curCountDountTime)
            end
        else
            --闪烁
             --播放倒计时音效
            if g_server_obj:get_server_type() ~= "video" then
                MJEffectManager:playCountdownEffect()
            end
            self:stopMiddleCount()
        end
    end
end


function MJGameBgLayer:onOutCardInfo(resp_json)
    --self:stopMiddleCount()
end

function MJGameBgLayer:stopMiddleCount()
    if self.schedulerIDMiddle ~= nil then    
        cc.Director:getInstance():getScheduler():unscheduleScriptEntry( self.schedulerIDMiddle )
        self.schedulerIDMiddle = nil
    end
    self.curCountDountTime = 15
    MJEffectManager:stopCountdownEffect()
end


--倒计时指向
function MJGameBgLayer:onPointToTokenUser(TokenbyUser)

    local UIChair = GameSceneModule:getInstance():getGameScene():getGameDataManager():getUIChairByServerChair(TokenbyUser)

    if UIChair == Direct.Down then
        self.Notice_Up:setVisible(false)
        self.Notice_Down:setVisible(true)
        self.Notice_left:setVisible(false)
        self.Notice_Right:setVisible(false)
    elseif UIChair == Direct.Right then  
        self.Notice_Up:setVisible(false)
        self.Notice_Down:setVisible(false)
        self.Notice_left:setVisible(false)
        self.Notice_Right:setVisible(true)
    elseif UIChair == Direct.Up then  
        self.Notice_Up:setVisible(true)
        self.Notice_Down:setVisible(false)
        self.Notice_left:setVisible(false)
        self.Notice_Right:setVisible(false)  
    elseif UIChair == Direct.Left then  
        self.Notice_Up:setVisible(false)
        self.Notice_Down:setVisible(false)
        self.Notice_left:setVisible(true)
        self.Notice_Right:setVisible(false)    
    end
    self:BlinkDirect()
end

--闪烁定时方向
function MJGameBgLayer:BlinkDirect()
--先停止所有动作
    self.Notice_Up:stopAllActions()
    self.Notice_Down:stopAllActions()
    self.Notice_left:stopAllActions()
    self.Notice_Right:stopAllActions()
    local blink = cc.Blink:create(1,1)
    local blinkmeta
    if self.Notice_Up:isVisible() then
        blinkmeta = self.Notice_Up
    elseif self.Notice_Down:isVisible() then
        blinkmeta = self.Notice_Down
    elseif self.Notice_left:isVisible() then
        blinkmeta = self.Notice_left
    elseif self.Notice_Right:isVisible() then
        blinkmeta = self.Notice_Right
    end           
    blinkmeta:runAction(cc.RepeatForever:create(blink))
end
--某玩家执行某个动作
function MJGameBgLayer:onActInfo(resp_json,isPlayEffect)
    print("MJGameBgLayer:onActInfo")

    local gamedataManager = GameSceneModule:getInstance():getGameScene():getGameDataManager()
    if resp_json.usActFlag ==  ActModule.MJActFlag.Chi then
        local ui_chair_index = gamedataManager:getUIChairByServerChair(resp_json.byToUser)
        local c_node = cc.CSLoader:createNode("animate/chl/chl.csb")
        local c_action = cc.CSLoader:createTimeline("animate/chl/chl.csb")
        c_node:runAction(c_action)
        c_action:gotoFrameAndPlay(0)
        c_node:setPosition(self.AnimPos[ui_chair_index])
        self.ui_root:addChild(c_node)

        c_action:setLastFrameCallFunc( function()
            c_node:removeFromParent()
            c_node = nil
        end)

    elseif resp_json.usActFlag ==  ActModule.MJActFlag.Peng then
        print("play peng action")
        local ui_chair_index = gamedataManager:getUIChairByServerChair(resp_json.byToUser)
        local p_node = cc.CSLoader:createNode("animate/peng/peng.csb")
        
        local p_action = cc.CSLoader:createTimeline("animate/peng/peng.csb")
        p_node:runAction(p_action)

        p_action:gotoFrameAndPlay(0,false)
        p_node:setPosition(self.AnimPos[ui_chair_index])
        self.ui_root:addChild(p_node)

        p_action:setLastFrameCallFunc( function()
            p_node:removeFromParent()
            p_node = nil
        end)
    elseif resp_json.usActFlag ==  ActModule.MJActFlag.Gang then
        local ui_chair_index = gamedataManager:getUIChairByServerChair(resp_json.byToUser)
        local g_node = cc.CSLoader:createNode("animate/gang/gang.csb")
        
        local g_action = cc.CSLoader:createTimeline("animate/gang/gang.csb")
        g_node:runAction(g_action)
        g_action:gotoFrameAndPlay(0)
        g_node:setPosition(self.AnimPos[ui_chair_index])
        self.ui_root:addChild(g_node)
        g_action:setLastFrameCallFunc( function()
            g_node:removeFromParent()
            g_node = nil
        end)
    elseif resp_json.usActFlag ==  ActModule.MJActFlag.Hu then
        
        for i,v in pairs(resp_json.byToUsers) do
            local ui_chair_index = gamedataManager:getUIChairByServerChair(v)
            local h_node = nil
            local h_action = nil
            if resp_json.isZiMo then
                h_node = cc.CSLoader:createNode("animate/zimo/zimo.csb")
                h_action = cc.CSLoader:createTimeline("animate/zimo/zimo.csb")

                if isPlayEffect == nil or isPlayEffect == true then
                    MJEffectManager:playZimoEffect(v + 1)
                end

            else
                h_node = cc.CSLoader:createNode("animate/hu/hu.csb")
                h_action = cc.CSLoader:createTimeline("animate/hu/hu.csb")

                if isPlayEffect == nil or isPlayEffect == true then
                    MJEffectManager:PlayActEffect(ActModule.MJActFlag.Hu,v + 1)
                end
            end
            h_node:runAction(h_action)
            h_action:gotoFrameAndPlay(0)
            h_node:setPosition(self.AnimPos[ui_chair_index])
            self.ui_root:addChild(h_node)
            h_action:setLastFrameCallFunc( function()
                h_node:removeFromParent()
                h_node = nil
            end)
        end
        
    end
    print("MJGameBgLayer:onActInfo end")
end

function MJGameBgLayer:onGameStart(resp_json)
    print("MJGameBgLayer:onGameStart")
    local start_node = cc.CSLoader:createNode("animate/duiju/duiju.csb")
    self.start_node = start_node
    local start_action = cc.CSLoader:createTimeline("animate/duiju/duiju.csb")
    start_node:runAction(start_action)
    start_action:gotoFrameAndPlay(0,false)
    start_node:setPosition(cc.p(640,360))
    self.ui_root:addChild(start_node)
    start_action:setLastFrameCallFunc( function()
        self.start_node:removeFromParent()
        MJEffectManager:playOtherEffect(EffectEnum.KAIS,false)
    end)

    --更新局数
    self:updateRound(resp_json.RoundTimes)
    print("MJGameBgLayer:onGameStart  end")
end

function MJGameBgLayer:onMakeNT(resp_json)
    local dataMnger = GameSceneModule:getInstance():getGameScene():getGameDataManager()
    local selfChair = dataMnger:getSelfChair()
    local ui_chair = dataMnger:getUIChairByServerChair(resp_json.NtUser)

    self:RotationTimeCountBg(-ui_chair+1)
end

function MJGameBgLayer:onAgreeGame()
    self:setLeftCardsVisible(false)
    self.TimeCountBg:setVisible(false)
end

function MJGameBgLayer:onGameStation(resp_json)
    print("MJGameBgLayer:onGameStation")
    
--    if resp_json.chair ~= nil then
--        self:RotationTimeCountBg(resp_json.chair)
--    end
    if resp_json.CheaterOpen then
        if resp_json.CheaterOpen == 1 then
              self.Button_cheater:setVisible(true)
        end
    end

    if resp_json.NTUser then
        local dataMnger = GameSceneModule:getInstance():getGameScene():getGameDataManager()
        
        local ui_chair = dataMnger:getUIChairByServerChair(resp_json.NTUser)

        self:RotationTimeCountBg(-ui_chair+1)
    end

    if resp_json.GSID==24 then          --跳牌状态
        
    elseif resp_json.GSID==GameStation.GS_PLAYING then       --玩家打牌状态
        
        --显示剩余牌数
        self:updateLeftCardsNum(resp_json.WallLeft)
        self:setLeftCardsVisible(true)

        self.TimeCountBg:setVisible(true)
        self:onPointToTokenUser(resp_json.TokenUser)
    end

end

--设置牌背颜色
function MJGameBgLayer:onCardColorChange()
    --print("MJGameBgLayer:onCardColorChange")
    self.Image_cards_remain_bg:loadTexture(COLOR_RES_PATH[UserData:getCardColor()] .."shengyuzhangshu.png")
    local pngStr = COLOR_RES_PATH[UserData:getCardColor()] .. "Front_Small_05.png"
end

--设置水印  pngname 图片名
function MJGameBgLayer:setShuiyinByName(pngname)
    self.Image_shuiyin:loadTexture("ui_res/shuiyins/" .. pngname .. ".png")
    self.Image_shuiyin:setVisible(true)
end

--是否显示水印
function MJGameBgLayer:setShuiYinVisible(visible)
    self.Image_shuiyin:setVisible(visible)
end

function MJGameBgLayer:startCountDown()
    self:update()
    self.schedulerID = cc.Director:getInstance():getScheduler():scheduleScriptFunc( handler(self , self.update ) , 1.0 ,false)
end

function MJGameBgLayer:onOutCardInfo(resp_json)
    
end

function MJGameBgLayer:stopCountDown()
    if self.schedulerID ~= nil then    
        cc.Director:getInstance():getScheduler():unscheduleScriptEntry( self.schedulerID )
        self.schedulerID = nil
    end
end

function MJGameBgLayer:update()
    if self.Text_time ~= nil then
        self.Text_time:setText(os.date("%H:%M", os.time()))
    else
        --print("Text_time is nil ")
    end
end

function MJGameBgLayer:Cheat(args)

    local dataMnger = GameSceneModule:getInstance():getGameScene():getGameDataManager()
    if args.type == 1 then
        print("fapai")

        
        --检验是否正确
        if #args.cards ~= #dataMnger:getSelfHandCards() then
            api_show_tips("配发牌张数不合法")
            return
        end
        local request = {}
        request.cards = {}
        for i,v in pairs (args.cards) do
            request.cards[i] = tonumber(v)
        end
    
        GameSceneModule:getInstance():getGameScene():getHelper():sendGameMessage(203, request)
--        GameSceneModule:getInstance():getGameScene():getGameDataManager():onCmdChangeCards(request.cards)
--        self.ui_root.MJHandCardLayer:onCmdChangeCards(request.cards)

    elseif args.type == 2 then
        print("peishoupai")

        --检验是否正确
        if #args.cards ~= #dataMnger:getSelfHandCards() then
            api_show_tips("配手牌不能大于或小于当前手牌张数")
            return
        end

        local request = {}
        request.cards = {}
        for i,v in pairs (args.cards) do
            request.cards[i] = tonumber(v)
        end
    
        GameSceneModule:getInstance():getGameScene():getHelper():sendGameMessage(204, request)
        GameSceneModule:getInstance():getGameScene():getGameDataManager():onCmdChangeCards(request.cards)
        self.ui_root.MJHandCardLayer:onCmdChangeCards(request.cards)
    elseif args.type == 3 then
        print("peizhuapai")

        --检验是否正确
        if #args.cards >1 then
            api_show_tips("抓下一张牌不能大于 1 张")
            return
        end

        local request = {}
        request.cardvalue = tonumber(args.cards[1])
        GameSceneModule:getInstance():getGameScene():getHelper():sendGameMessage(205, request)
    else
    end
end

function MJGameBgLayer:OnRcvGameMessage(msgID,msgData)
    if msgID == MJMessageID.ASS_GAME_BEGIN then
        
        self:onGameStart(msgData)
    elseif msgID == MJMessageID.ASS_MAKE_NT_INFO then
        self:onMakeNT(msgData)
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

return MJGameBgLayer

--endregion
