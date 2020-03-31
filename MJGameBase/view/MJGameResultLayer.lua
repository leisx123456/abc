--region *.lua
--Date
--游戏结算类
--此文件由[BabeLua]插件自动生成


--local PlayersScoreLayer = import(".PlayersScoreLayer")



local MJMessageID = import("..MJMessageID")
local ActModule = import("..module.ActModule")
local m_download = import("....common.download")
--local Algorithm = import("..."..GAME_ID..".Algorithm")
local MJGameResultLayer = class("MJGameResultLayer",function() return cc.CSLoader:createNode("ui_csb/GameResultLayer.csb") end)

--resp_json需要的数据
--parent 结算层父层
--isShowFinishButton  是否显示查看牌局按钮
function MJGameResultLayer:ctor(parent,resp_json,isShowFinishButton,isPlayEffect)
    print("MJGameResultLayer:ctor")

    self.parent = parent
    self.MJGameDataManager = GameSceneModule:getInstance():getGameScene():getGameDataManager()
    self.gameLayerManager = GameSceneModule:getInstance():getGameScene():getGameLayerManager()
    self.nextShowCardIndex = {1,1,1,1}--下次要显示的牌的索引

    local Panel_gameresult = self:getChildByName("Panel_gameresult")
    local Image_bg = Panel_gameresult:getChildByName("Image_bg")

    local Image_title = Image_bg:getChildByName("Image_title") 

    if resp_json.IsHuangZhuang then
        Image_title:loadTexture("ui_res/Result/liuju.png")
    else
        if resp_json.TotalPoint[self.MJGameDataManager:getSelfChair() + 1] < 0 then
            Image_title:loadTexture("ui_res/Result/shule.png")
            Image_title:setContentSize(cc.size(471,76))
            if isPlayEffect == nil or isPlayEffect == true  then
                MJEffectManager:playOtherEffect(EffectEnum.LOSE,false)
            end

            
        else
            if isPlayEffect == nil or isPlayEffect == true  then
                MJEffectManager:playOtherEffect(EffectEnum.WIN,false)
            end
        end
    end


    --返回按钮
    local button_back = Image_bg:getChildByName("Button_back")
    button_back:addClickEventListener(function() 
        print("返回大厅====")
        GameSceneModule:getInstance():getGameScene():reqLeaveDesk()
    end)
    self.m_btnBack=button_back

    --分享按钮
    local button_share = Image_bg:getChildByName("Button_share")
    button_share:addClickEventListener(function() 
        print("touch share button")
         print("share")
        cc.utils:captureScreen(function(succeed, outputFile)
            if succeed then
               self.parent:reqShareImageToWXSession(outputFile)
            end
        end, 
        "kwx_share.jpg")
        print("share button touched")
    end)
    if GameSceneModule:getInstance():getGameScene():getGameType() == "tz1" then
        button_share:setVisible(true)
    else
        button_back:setVisible(false)
        button_share:setVisible(false)
    end

    --继续游戏
    local button_continue = Image_bg:getChildByName("Button_continue")
    button_continue:addClickEventListener(function() 
       
        local curGameScene = GameSceneModule:getInstance():getGameScene()
        if curGameScene.IsHaveUserStandUp then
            --如果有玩家站起
            curGameScene:reqChangeDesk()
            api_show_loading("正在为您换桌,请稍后...")
        else
           local request = {}
            GameSceneModule:getInstance():getGameScene():getHelper():sendGameMessage(MJMessageID.ASS_GM_AGREE_GAME,request)
            
        end

        self.gameLayerManager:hideGameResultLayer()
    end)


    --换桌
    local button_changedesk = Image_bg:getChildByName("Button_changedesk")
    button_changedesk:addClickEventListener(function() 
        print("发送换桌消息")

        --self:removeFromParent()
        --GameSceneModule:getInstance():getGameScene():reqChangeDesk()                                         
        GameSceneModule:getInstance():getGameScene():ChangeDesk()
        api_show_loading("正在换桌,请稍后...")
        print("发送换桌消息结束")
    end)

    self.m_btnChangeDesk=button_changedesk

    --当游戏结束后如果可以立即查看之前的牌局信息就显示这个按钮(但是要在返回大厅按钮做下处理)
    local button_checkplayers = Image_bg:getChildByName("Button_checkplayers")
    button_checkplayers:setPositionX(567)
    button_checkplayers:setVisible(false)
    button_checkplayers:addClickEventListener(function() 
        
        self.gameLayerManager:showPlayersScoreLayer()
        self.gameLayerManager:hideGameResultLayer()
    end)

    if GameSceneModule:getInstance():getGameScene():getGameType() ~= "tz1" then
        button_changedesk:setVisible(false)
        button_share:setPositionX(350.59)
        button_continue:setPositionX(742.56)
        button_checkplayers:setPositionX(742.56)
    end

    if isShowFinishButton~= nil and isShowFinishButton == true then
        button_checkplayers:setVisible(true)
        button_continue:setVisible(false)
    end
    
    self.Text_time = Image_bg:getChildByName("Text_time")
    self.Text_date = Image_bg:getChildByName("Text_date")
    self.Text_roomnum = Image_bg:getChildByName("Text_roomnum")
    
    local desk_config =  GameSceneModule:getInstance():getGameScene():getDeskInfo()
    if desk_config then
        self.Text_roomnum:setText("房号：".. tostring(desk_config.roomKey))
    end

    local Text_desknum = Image_bg:getChildByName("Text_desknum")
    Text_desknum:setVisible(false)

    self.Text_round = Image_bg:getChildByName("Text_round")
    
    
    --同桌游平台不显示房号和局数
    if GameSceneModule:getInstance():getGameScene():getGameType() == "tz1" then
        self.Text_roomnum:setVisible(false)
        self.Text_round:setVisible(false)
    end
    
    self.PlayersResultInfo = {}
    
    
    for i = 1,4 do
        
        self.PlayersResultInfo[i] = Image_bg:getChildByName("Panel_playerinfo_" .. i)

        if i > PLAYER_COUNT then
            self.PlayersResultInfo[i]:setVisible(false)

        else
             self.PlayersResultInfo[i].Image_NT_tip = self.PlayersResultInfo[i]:getChildByName("Image_NT_tip")

            --微信头像
            local Image_head = self.PlayersResultInfo[i]:getChildByName("Image_head")

            local allUserInfo = GameSceneModule:getInstance():getGameScene():getGameDataManager():getAllUserData()
            local player_info = allUserInfo[i]

            m_download:get_instance():set_head_image_and_auto_update( 
            Image_head ,
            player_info.avatarUrl ,
            tostring(player_info.dwUserID) )



            self.PlayersResultInfo[i].HandCards = {}
            self.PlayersResultInfo[i].Text_nick = self.PlayersResultInfo[i]:getChildByName("Text_nick")
            self.PlayersResultInfo[i].Text_paixing = self.PlayersResultInfo[i]:getChildByName("Text_paixing")
            self.PlayersResultInfo[i].Text_paixing:setVisible(false)

            local Panel_hand = self.PlayersResultInfo[i]:getChildByName("Panel_hand")
            --最多情况可以展示18张牌(4杠)
            for j = 1,18 do
                local HandCardNode = Panel_hand:getChildByName("Image_card_" .. j)
                --self.PlayersResultInfo[i].HandCards[j] = HandCardNode:getChildByName("Image_card")
                self.PlayersResultInfo[i].HandCards[j] = Panel_hand:getChildByName("Image_card_" .. j)
                self.PlayersResultInfo[i].HandCards[j]:setVisible(false)
                self.PlayersResultInfo[i].HandCards[j]:getChildByName("Image_card"):loadTexture( COLOR_RES_PATH[UserData:getCardColor()] .. "Front_Small_01.png")
            end

            self.PlayersResultInfo[i].Image_winorlost = self.PlayersResultInfo[i]:getChildByName("Image_winorlost")
        
            --总分
            self.PlayersResultInfo[i].Text_allscore = self.PlayersResultInfo[i]:getChildByName("Text_allscore")
            self.PlayersResultInfo[i].Text_allscore:setVisible(false)

            self.PlayersResultInfo[i].Text_allscore_num = self.PlayersResultInfo[i]:getChildByName("Text_allscore_num")
        
        end

    end
    
    --更新日期和时间
    self:updateBase()

    --更新每个玩家的牌
    self:updateCPGCards(resp_json.CGPNums,resp_json.CPGRecords)

    self:updateHandcards(resp_json.HandNums,resp_json.HandCards,resp_json.HuCards,resp_json.isHu,resp_json)

    --更新每个玩家输赢
    self:updateIsHu(resp_json.isHu)
    --更新每个玩家的总分
    self:updateTotalPoint(resp_json.TotalPoint)
    --更新局数
    self:updateRound(resp_json.Rounds)
   
    self:registerScriptHandler(handler(self,self.onNodeEvent))
    
    --人数变化更新UI位置
    if PLAYER_COUNT == 2 then
        self.PlayersResultInfo[2]:setPositionY(140)
    elseif PLAYER_COUNT == 3 then

    end


end

function MJGameResultLayer:onNodeEvent(event)

    if event == "enter" then
        
    elseif event == "exit" then
        
        if self.schedulerID then    
            cc.Director:getInstance():getScheduler():unscheduleScriptEntry( self.schedulerID )
            self.schedulerID = nil
        end
    end
end

function MJGameResultLayer:updateBase()
    print("MJGameResultLayer:updateBase")
    self.Text_date:setText(os.date("%Y-%m-%d", os.time()))

    if self.schedulerID ~= nil then    
        cc.Director:getInstance():getScheduler():unscheduleScriptEntry( self.schedulerID )
        self.schedulerID = nil
    end
    self:update()
    self.schedulerID = cc.Director:getInstance():getScheduler():scheduleScriptFunc( handler(self , self.update ) , 1.0 ,false)
    local PlayersInfoCache = self.MJGameDataManager:getAllUserData()
    for i = 0,PLAYER_COUNT-1 do
        --获取客户端椅子id
        
        local player_info = PlayersInfoCache[i + 1]
        self.PlayersResultInfo[i + 1].Text_nick:setText(api_get_ascll_sub_str_by_ui(player_info.nickName , 8))
        

        if self.MJGameDataManager:getSelfChair() == i then
            self.PlayersResultInfo[i + 1].Text_nick:setTextColor(cc.c3b(255,255,0))
        else
            self.PlayersResultInfo[i + 1].Text_nick:setTextColor(cc.c3b(255,255,255))
        end

        local NTChair = self.MJGameDataManager:getNTChair()

        if NTChair == i then 
            self.PlayersResultInfo[i + 1].Image_NT_tip:setVisible(true)
        else
            self.PlayersResultInfo[i + 1].Image_NT_tip:setVisible(false)
        end
    end
end



--更新房间号码和局数
function MJGameResultLayer:updateRound(round)
    print("MJGameResultLayer:updateRound")
    self.Text_round:setText("局数：" .. round)
end


function MJGameResultLayer:update()
    if self.Text_time ~= nil then
        self.Text_time:setText(os.date("%X", os.time()))
    else
        --print("Text_time is nil ")
    end
end

--更新玩家吃碰杠的牌
function MJGameResultLayer:updateCPGCards(CGPNums,CPGRecords)
    print("updateCPGCards")

    print("#CPGRecords  is " , #CPGRecords)
    if CGPNums == nil then
        return
    end

    for i = 1 ,#CGPNums  do
        if CGPNums[i] > 0 then
            print("CGPNums[i] = " .. CGPNums[i])
            
            local playerCPGRecords = CPGRecords[i]
            
            for j= 1,CGPNums[i] do
                self:showCPGCards( i ,playerCPGRecords[j])
            end
        end
    end
end

--显示吃碰杠牌
function MJGameResultLayer:showCPGCards(ui_index,cpgNode)
    print("MJGameResultLayer:showCPGCards")
    local showNums = 0
    if cpgNode.usFlags == ActModule.CPGType.Peng 
            or cpgNode.usFlags == ActModule.CPGType.Chi then
        --显示三张牌
        showNums = 3
    elseif cpgNode.usFlags == ActModule.CPGType.MingGang 
            or cpgNode.usFlags == ActModule.CPGType.BuGang 
            or cpgNode.usFlags == ActModule.CPGType.AnGang then
        --显示4张
        showNums = 4
    end

    for i = 1, showNums do
        local cardindex = self.nextShowCardIndex[ui_index]
        print("show card at index " .. cardindex)
        local card = self.PlayersResultInfo[ui_index].HandCards[cardindex]
        local image = card:getChildByName("Image_card"):getChildByName("Image_value")

        if cpgNode.usFlags == ActModule.CPGType.AnGang and i < 4 then
             
            card:getChildByName("Image_card"):loadTexture(COLOR_RES_PATH[UserData:getCardColor()] .. "Big_03.png")

            image:setVisible(false)
        else
             card:getChildByName("Image_card"):loadTexture(COLOR_RES_PATH[UserData:getCardColor()] .. "Front_Small_01.png")
             print("cpgNode.val[i] = " .. cpgNode.val[i])
             image:loadTexture("res/game_res/MJGameBase/ui_res/Mahjong/Small/Small_"..tostring(cpgNode.val[i])..".png")
             image:setVisible(true)
        end
        if i == 4 then
            -- 如果当前牌是杠的最后一张牌
            -- 要显示 补杠 明杠(听前杠 或者 听后杠)
            local pos_x = self.PlayersResultInfo[ui_index].HandCards[cardindex - 2]:getPositionX()
            local pos_y = card:getPositionY() + 10
            card:setPosition(cc.p(pos_x,pos_y))
            self:moveLeft(ui_index,self.nextShowCardIndex[ui_index] + 1)

        end

        if i == 1 then
            local tingTip = nil
            if cpgNode.usFlags == ActModule.CPGType.BuGang then
                --显示补杠角标
                print("this is a bugang")
                tingTip = cc.Sprite:create("bu.png")
            elseif cpgNode.usFlags == ActModule.CPGType.MingGang then
                
            end

            if tingTip then
                local tipSize = tingTip:getContentSize() 
                local cardsize = card:getContentSize()
                tingTip:setScale(0.5)
                tingTip:setPosition(cc.p( cardsize.width / 2 - 6,cardsize.height- 16 ))
                card:addChild( tingTip )
            end

            
        end
        card:setVisible(true)
        self.nextShowCardIndex[ui_index] = self.nextShowCardIndex[ui_index] + 1
    end
    self:moveRight(ui_index,self.nextShowCardIndex[ui_index])

end

--从index起将牌位置向右移动
function MJGameResultLayer:moveRight(ui_index ,cardindex )
    for i = cardindex,18 do
        local posX = self.PlayersResultInfo[ui_index].HandCards[i]:getPositionX()
        self.PlayersResultInfo[ui_index].HandCards[i]:setPositionX(posX + 10)
    end
end

--从index起将牌位置向左移动
function MJGameResultLayer:moveLeft(ui_index , cardindex)
    for i = cardindex,18 do
        local posX = self.PlayersResultInfo[ui_index].HandCards[i]:getPositionX()
        self.PlayersResultInfo[ui_index].HandCards[i]:setPositionX(posX - 37)
    end
end


--更新玩家手牌
function MJGameResultLayer:updateHandcards(HandNums,HandCards,HuCards,IsHu)
    print("MJGameResultLayer updateHandcards")
    
    
    for i = 1 ,PLAYER_COUNT  do
        
        --此处用datamanager里面的数据 会对手牌进行一次带癞子的排序
        HandCards[i] = self.MJGameDataManager:getUserData(i-1).HandCards
        if HandNums[i] <= 0 then
            return
        end

        for j = 1,HandNums[i] do
            local card = self.PlayersResultInfo[i].HandCards[self.nextShowCardIndex[i]]
            if card then
                local image = card:getChildByName("Image_card"):getChildByName("Image_value")
                local imagename = "res/game_res/MJGameBase/ui_res/Mahjong/Small/Small_"..tostring(HandCards[i][j])..".png"
               
                image:loadTexture(imagename)
                card:setVisible(true)
                self.nextShowCardIndex[i] = self.nextShowCardIndex[i] + 1
                if j == HandNums[i] and IsHu[i] then
                    card:setColor(SELECTED_COLOR)
                    self:moveRight(i,self.nextShowCardIndex[i] - 1)
                end 
            end
        end
    end
    print("MJGameResultLayer updateHandcards end")
end

--是否胡牌
function MJGameResultLayer:updateIsHu(isHu)
    --输赢
    for i= 1 ,PLAYER_COUNT do
        self.PlayersResultInfo[i].Image_winorlost:setVisible(isHu[i])
    end   
end

function MJGameResultLayer:updateTotalPoint(TotalPoint)
    for i = 1,PLAYER_COUNT do
        --总分
        self.PlayersResultInfo[i].Text_allscore_num:setText(tostring(TotalPoint[i]))
    end
end

return MJGameResultLayer

--endregion
