--region *.lua
--Date
--牌局信息层
--此文件由[BabeLua]插件自动生成

local m_download = import("....common.download")

local PlayersScoreLayer = class("MJPlayersScoreLayer",function() return cc.CSLoader:createNode("ui_csb/PlayersScoreLayer.csb") end)

function PlayersScoreLayer:ctor(parent,PaijuInfo)
    print("PlayersScoreLayer:ctor")
    self.parent = parent
    local Panel_root = self:getChildByName("Panel_players_score")
    local Image_bg = Panel_root:getChildByName("Image_bg")

    local button_share = Image_bg:getChildByName("Button_share")
    button_share:addClickEventListener(function() 
        print("share button touched")
        
        cc.utils:captureScreen(function(succeed, outputFile)
            if succeed then
               self.parent:reqShareImageToWXSession(outputFile)
            end
        end, 
        "kwx_share.jpg")
        print("share button touched")
    end)



    local button_back = Image_bg:getChildByName("Button_back")
    button_back:addClickEventListener(function ()
        print("back button touched")
        self.parent:Exit_game()
    end)

  
      -- 隐藏分享按钮
    if g_config.has_share == false then
        button_share:setVisible(false)
    end
    if button_share:isVisible() == false then
        button_back:setPositionX(55)
    end
    
    self.Text_round = Image_bg:getChildByName("Text_round")
    self.Text_round:setVisible(false)

    self.Text_desknum=Image_bg:getChildByName("Text_desknum")
    self.Text_desknum:setVisible(false)


    self.PlayersScoreInfo = {}
    for i = 1,4 do
        self.PlayersScoreInfo[i] = Image_bg:getChildByName("Panel_playerinfo_" .. i)
        --头像
        self.PlayersScoreInfo[i].Image_Head = self.PlayersScoreInfo[i]:getChildByName("Image_Head")
        --房主
        self.PlayersScoreInfo[i].Image_RoomMaster = self.PlayersScoreInfo[i].Image_Head:getChildByName("Image_RoomMaster")
        self.PlayersScoreInfo[i].Image_RoomMaster:setVisible(false)
        
        --大赢家
        self.PlayersScoreInfo[i].Image_winner =  self.PlayersScoreInfo[i]:getChildByName("Image_winner")
        self.PlayersScoreInfo[i].Image_winner:setVisible(false)
        --最佳炮手
        self.PlayersScoreInfo[i].Image_paoshou =  self.PlayersScoreInfo[i]:getChildByName("Image_paoshou")
        self.PlayersScoreInfo[i].Image_paoshou:setVisible(false)

        --昵称
        self.PlayersScoreInfo[i].Text_nick = self.PlayersScoreInfo[i]:getChildByName("Text_nick")
        --ID
        self.PlayersScoreInfo[i].Text_id = self.PlayersScoreInfo[i]:getChildByName("Text_id")
        --自摸次数
        self.PlayersScoreInfo[i].Text_zm = self.PlayersScoreInfo[i]:getChildByName("Text_zm")
        --接炮次数
        self.PlayersScoreInfo[i].Text_jp = self.PlayersScoreInfo[i]:getChildByName("Text_jp")
        --点炮次数
        self.PlayersScoreInfo[i].Text_dp = self.PlayersScoreInfo[i]:getChildByName("Text_dp")
        --暗杠次数
        self.PlayersScoreInfo[i].Text_ag = self.PlayersScoreInfo[i]:getChildByName("Text_ag")
        --明杠次数
        self.PlayersScoreInfo[i].Text_mg = self.PlayersScoreInfo[i]:getChildByName("Text_mg")
        --总分数
        self.PlayersScoreInfo[i].Text_score = self.PlayersScoreInfo[i]:getChildByName("Text_score")

        
    end

    self:updateRound(PaijuInfo.RoundTimes,PaijuInfo.AllRoundTimes)
    self:updateUserInfo(PaijuInfo.FangPaoTimes,PaijuInfo.TotalScore)
    self:updatePlayersScoreInfo(PaijuInfo)

    
    --时间
    self.Text_time = Image_bg:getChildByName("Text_time")
    --日期
    self.Text_date = Image_bg:getChildByName("Text_date")
    
    self:updateDateAndTime()
    --房号和桌号
    self.Text_roomnum = Image_bg:getChildByName("Text_roomnum")
    
    local desk_config =  GameSceneModule:getInstance():getGameScene():getDeskInfo()
    self.Text_roomnum:setText("房号：".. tostring(desk_config.roomKey))
    

    self:registerScriptHandler(handler(self,self.onNodeEvent))

    --人数变化
    if PLAYER_COUNT == 2 then
        --隐藏三四号位置的UI
        self.PlayersScoreInfo[3]:setVisible(false)
        self.PlayersScoreInfo[4]:setVisible(false)
        --更新位置
        self.PlayersScoreInfo[1]:setPositionX(175)
        self.PlayersScoreInfo[2]:setPositionX(650)

    elseif PLAYER_COUNT == 3 then
        --隐藏四号位置的UI
        self.PlayersScoreInfo[4]:setVisible(false)
        --更新位置
        self.PlayersScoreInfo[1]:setPositionX(120)
        self.PlayersScoreInfo[2]:setPositionX(415)
        self.PlayersScoreInfo[3]:setPositionX(710)
    end
    
    print("PlayersScoreLayer:ctor end")
end

function PlayersScoreLayer:onNodeEvent(event)

    if event == "enter" then
        --g_pushBombBox(self)
    elseif event == "exit" then
        --g_popBombBox(self)
    end
end



--更新房间局数
function PlayersScoreLayer:updateRound(round,allround)
    self.Text_round:setText("局数：" .. round .. "/"  .. allround)
end


--获取一个表中的最大值及其索引
function PlayersScoreLayer:maximum(a)
    print("in maximum")
    local mi = 1             -- maximum index
    local m = a[mi]          -- maximum value
    for i,val in ipairs(a) do
        print("i= " .. i)
       if val > m then
           mi = i
           m = val
       end
    end
    return m, mi
end


--更新玩家信息
function PlayersScoreLayer:updateUserInfo(FangPaoTimes,TotalScore)
    
    local maxdpTimes,maxdpIndex = self:maximum(FangPaoTimes)
    local maxScore,maxScoreIndex = self:maximum(TotalScore)
    print("PlayersScoreLayer:updateUserInfo 11")
    for i = 0,PLAYER_COUNT-1 do
        print("PlayersScoreLayer:updateUserInfo i= " .. i)

        
        
            --这个时候获取 可能玩家已经被踢出桌子 因为桌子已经解散  需要从缓存中读取玩家信息
        
        local allUserInfo = GameSceneModule:getInstance():getGameScene():getGameDataManager():getAllUserData()
        local player_info = allUserInfo[i+1]

        --昵称
        self.PlayersScoreInfo[i + 1].Text_nick:setText(api_get_ascll_sub_str_by_ui(player_info.nickName , 8))
        
        
        --ID
        self.PlayersScoreInfo[i + 1].Text_id:setString(player_info.dwUserID)
            
        --更新玩家头像
        m_download:get_instance():set_head_image_and_auto_update( 
        self.PlayersScoreInfo[i+1].Image_Head ,
        player_info.avatarUrl ,
        tostring(player_info.dwUserID) )
        --房主
        local desk_config =  GameSceneModule:getInstance():getGameScene():getDeskInfo()
        if player_info.dwUserID == desk_config.mastUserID then
            self.PlayersScoreInfo[i+1].Image_RoomMaster:setVisible(true)
        end

        --最佳炮手
        if maxdpTimes == FangPaoTimes[i+1] and maxdpTimes > 0 then
            self.PlayersScoreInfo[i+1].Image_paoshou:setVisible(true)
        end

        --大赢家
        if maxScore == TotalScore[i+1] and maxScore > 0 then
            self.PlayersScoreInfo[i+1].Image_winner:setVisible(true)
        end
    end
end




--更新玩家牌局信息
function PlayersScoreLayer:updatePlayersScoreInfo(PaijuInfo)
    for i = 1 , PLAYER_COUNT do 
        self.PlayersScoreInfo[i].Text_zm:setString("自摸次数:" .. PaijuInfo.ZimoTimes[i])
        self.PlayersScoreInfo[i].Text_jp:setString("接炮次数:" .. PaijuInfo.JiePaoTimes[i])
        self.PlayersScoreInfo[i].Text_dp:setString("点炮次数:" .. PaijuInfo.FangPaoTimes[i])
        self.PlayersScoreInfo[i].Text_ag:setString("暗杠次数:" .. PaijuInfo.AnGangTimes[i])
        self.PlayersScoreInfo[i].Text_mg:setString("明杠次数:" .. PaijuInfo.MingGangTimes[i])
        self.PlayersScoreInfo[i].Text_score:setString(PaijuInfo.TotalScore[i])
    end
end

function PlayersScoreLayer:updateDateAndTime()
    --更新日期
    self.Text_date:setText(os.date("%Y-%m-%d", os.time()))

    --更新时间
    if self.schedulerID ~= nil then    
        cc.Director:getInstance():getScheduler():unscheduleScriptEntry( self.schedulerID )
        self.schedulerID = nil
    end
    self:update()
    self.schedulerID = cc.Director:getInstance():getScheduler():scheduleScriptFunc( handler(self , self.update ) , 1.0 ,false)

end

function PlayersScoreLayer:update()
    if self.Text_time ~= nil then
        self.Text_time:setText(os.date("%H:%M", os.time()))
    else
        --print("Text_time is nil ")
    end
end



return PlayersScoreLayer


--endregion
