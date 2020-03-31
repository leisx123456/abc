--region *.lua
--Date
--游戏中每个玩家头像信息UI管理类
--此文件由[BabeLua]插件自动生成

local MJMessageID = import("..MJMessageID")
local m_download = import("....common.download")
local MJMessageID = import("..MJMessageID")
local MJPlayersHeadLayer = class("MJPlayersHeadLayer")


--正在游戏状态下每个玩家的头像位置
local PlayingPos = {

    cc.p(88.00,256.27),
    cc.p(1184.96,395.00),
    cc.p(359.19,600),
    cc.p(88.00,493.00),
}

--等待开始状态下每个玩家的头像位置
local WaitingPos = {

    cc.p(640,150),
    cc.p(1180,400),
    cc.p(640,600),
    cc.p(100,400),
}



function MJPlayersHeadLayer:ctor(ui_project, ui_root, playingpos, waitingpos)

    if playingpos ~= nil then
        PlayingPos = playingpos
    end
    if waitingpos ~= nil then
        WaitingPos = waitingpos
    end
    self.ui_project = ui_project
    self.ui_root = ui_root
    self.PlayerHeads = {}
    self.curCountDountTime = 15 --倒计时还有多少时间
    self.curCountDownIndex = nil--当前倒计时索引

    for i = 1 , 4 do
        self.PlayerHeads[i] = self.ui_project:getChildByName("Panel_player_" .. i)
        self.PlayerHeads[i]:setVisible(false)
        self.PlayerHeads[i]:setPosition(WaitingPos[i])
        self.PlayerHeads[i].Image_head_wx = self.PlayerHeads[i]:getChildByName("Image_head_wx")
        
        local Image_head_bg = self.PlayerHeads[i].Image_head_wx:getChildByName("Image_head_bg")

        self.PlayerHeads[i].Image_ready = self.PlayerHeads[i].Image_head_wx:getChildByName("Image_ready")
        self.PlayerHeads[i].Image_ready:setVisible(false)

        if i == 2 then
            self.PlayerHeads[i].Image_ready:setPositionX(-90)
        end

        Image_head_bg:addClickEventListener(
            function()
                local popPositons = {
                        cc.p(150,190),
                        cc.p(830,360),
                        cc.p(440,560),
                        cc.p(150,430),
                    }
                local  curGameStation =  GameSceneModule:getInstance():getGameScene():getGameDataManager():getGameStation();
       
                if curGameStation <= GameStation.GS_WAIT_ARGEE then
                     popPositons = {
                            cc.p(640 + 50,150),
                            cc.p(830,448),
                            cc.p(640 + 50,560),
                            cc.p(100,448),
                        }
                end
                print("Image Head clicked")
                local allUserInfo = GameSceneModule:getInstance():getGameScene():getGameDataManager():getAllUserData()

                local curServerChair = GameSceneModule:getInstance():getGameScene():getGameDataManager():getServerChairByUIChair(i)

                if allUserInfo[curServerChair + 1] then
                    GameSceneModule:getInstance():getGameScene():getGameLayerManager():showPlayerInfoLayer(self.ui_root,allUserInfo[curServerChair + 1],popPositons[i])
                end
            end)
       

        self.PlayerHeads[i].Image_head_bg = Image_head_bg

        self.PlayerHeads[i].Text_nick = Image_head_bg:getChildByName("Text_nick")
        self.PlayerHeads[i].Text_score = Image_head_bg:getChildByName("Text_score")

      

        self.PlayerHeads[i].Image_NT = Image_head_bg:getChildByName("NT_1")
        self.PlayerHeads[i].Image_NT:setVisible(false)

        self.PlayerHeads[i].Image_paodeng = Image_head_bg:getChildByName("Image_paodeng")
        self.PlayerHeads[i].Image_paodeng:setVisible(false)

        self.PlayerHeads[i].CountDownTimer = nil
        
        
        self.PlayerHeads[i].Image_offline = Image_head_bg:getChildByName("Image_offline")
        self.PlayerHeads[i].Image_offline:setVisible(false)

        self.PlayerHeads[i].MasterTip=Image_head_bg:getChildByName("Image_MasterTip")
        if self.PlayerHeads[i].MasterTip~=nil then
            self.PlayerHeads[i].MasterTip:setVisible(false)
        end


    end
    self:onCardColorChange()
    
    --人数处理
    self:showSitUsers() 

end


function MJPlayersHeadLayer:setPlayingPos(pos)
    PlayingPos=pos
end

--显示已经坐下的玩家
function MJPlayersHeadLayer:showSitUsers()
    --如果自己的椅子号都还没有,此时不能画玩家的头像
    local selfChair = GameSceneModule:getInstance():getGameScene():getGameDataManager():getSelfChair()
    if selfChair < 0 then
        return
    end
    for k,v in pairs(self.PlayerHeads) do
        v:setVisible(false)
    end
    dump(GameSceneModule:getInstance():getGameScene():getSitDownPlayerMap())
    for key, sitPlayer in pairs(GameSceneModule:getInstance():getGameScene():getSitDownPlayerMap()) do
        
        local ui_chair_index = GameSceneModule:getInstance():getGameScene():getGameDataManager():getUIChairByServerChair(sitPlayer.bDeskStation)
        
        self.PlayerHeads[ui_chair_index].Text_nick:setText(api_get_ascll_sub_str_by_ui(sitPlayer.nickName , 8))
        self.PlayerHeads[ui_chair_index].Text_score:setText(sitPlayer.dwMoney)
        self.PlayerHeads[ui_chair_index].Image_head_wx:loadTexture("ui_res/default_boy.png")
       
        m_download:get_instance():set_head_image_and_auto_update( self.PlayerHeads[ui_chair_index].Image_head_wx ,sitPlayer.avatarUrl , tostring(sitPlayer.dwUserID) )

        --玩家可能离线
        if sitPlayer.status == 20 then
            self.PlayerHeads[ui_chair_index].Image_offline:setVisible(true)
        else
            self.PlayerHeads[ui_chair_index].Image_offline:setVisible(false)
        end
        
        --显示当前玩家的头像
        self.PlayerHeads[ui_chair_index]:setVisible(true)
        self:ShowReadImage(ui_chair_index,true)

        self:updateIsMater(sitPlayer.bDeskStation)
        self:ui_open()
    end
end


function MJPlayersHeadLayer:updateIsMater(bDeskStation)
    
    if GameSceneModule:getInstance():getGameScene():getGameType() ~= "tz1" then
        local deskInfo =  GameSceneModule:getInstance():getGameScene():getDeskInfo()    
        local ui_chair_index=GameSceneModule:getInstance():getGameScene():getGameDataManager():getUIChairByServerChair(bDeskStation)
        local allUserInfo = GameSceneModule:getInstance():getGameScene():getGameDataManager():getAllUserData()
        local player_info = allUserInfo[bDeskStation+1]

        if self.PlayerHeads[ui_chair_index].MasterTip~=nil then
            self.PlayerHeads[ui_chair_index].MasterTip:setVisible(deskInfo.mastUserID==player_info.dwUserID)
        end

    end


end

function MJPlayersHeadLayer:onPlayerSit(user_info)

     --过滤不是自己桌子的玩家
    local desk_config =  GameSceneModule:getInstance():getGameScene():getDeskInfo()
    if desk_config and desk_config.deskIndex == user_info.deskIndex then

        local ui_chair_index = GameSceneModule:getInstance():getGameScene():getGameDataManager():getUIChairByServerChair(user_info.bDeskStation)

        self.PlayerHeads[ui_chair_index]:setVisible(true)
        self.PlayerHeads[ui_chair_index].Text_nick:setText(api_get_ascll_sub_str_by_ui(user_info.nickName, 8))
        self.PlayerHeads[ui_chair_index].Text_score:setText(user_info.dwMoney)
        self.PlayerHeads[ui_chair_index].Image_head_wx:loadTexture("ui_res/default_boy.png")
        self:updateIsMater(user_info.bDeskStation)
        m_download:get_instance():set_head_image_and_auto_update(self.PlayerHeads[ui_chair_index].Image_head_wx, user_info.avatarUrl, tostring(user_info.dwUserID))
    end

end

--有玩家站起来
function MJPlayersHeadLayer:onPlayerStandUp( user_info , desk_info , code)
    print("MJPlayersHeadLayer:onPlayerStandUp")
    self:onPlayerLeaveDesk( user_info , desk_info , code)
end
function MJPlayersHeadLayer:onPlayerLeaveDesk( user_info , desk_info , code)
    print("MJPlayersHeadLayer:onPlayerLeaveDesk")
    if (code == 0 or code ==50 ) and user_info ~= nil and user_info.bDeskStation >= 0 then
        dump(user_info)
        local ui_chair_index = GameSceneModule:getInstance():getGameScene():getGameDataManager():getUIChairByServerChair(user_info.bDeskStation)
        print("ui_chair_index=",ui_chair_index)
        if ui_chair_index == 1 then  --说明站起的玩家是当前玩家自己
              return
        end
        self.PlayerHeads[ui_chair_index]:setVisible(false)
    end
end

--[[
@desciption 设置某一玩家的昵称和分数
@param chair_index 椅子位置
@param nickname 玩家昵称
@param score 玩家分数
@param isNT 是否是庄家
@return 
]]--

function MJPlayersHeadLayer:updatePlayerNameAndScore(chair_index,nickname,score,isNT)

    self.PlayerHeads[tonumber(chair_index)].Text_nick:setText(api_get_ascll_sub_str_by_ui(nickname,8))
    self.PlayerHeads[tonumber(chair_index)].Text_score:setText(tostring(score))
    self.PlayerHeads[tonumber(chair_index)].Image_NT:setVisible(isNT)
end


--更新某个玩家分数
function MJPlayersHeadLayer:updatePlayersScore(chair_index , score)
    self.PlayerHeads[tonumber(chair_index)].Text_score:setText(tostring(score))
end

function MJPlayersHeadLayer:onGameStart()
    for i = 1,4  do
        print("MJPlayersHeadLayer:onGameStart hide imageready")
        self:ShowReadImage(i,false)
    end



    self:ui_open()
end

--[[
定庄
@param server_chair  服务器椅子id
]]
function MJPlayersHeadLayer:onMakeNT(resp_json)
    print("MJPlayersHeadLayer:onMakeNT")

    --收到定庄消息 庄家为 server_chair
    GameSceneModule:getInstance():getGameScene():getGameDataManager():setNTChair(resp_json.NTUser)
   
    if resp_json.RoundTimes == 1 then
        self:updatePlayerInfo(resp_json.NtUser)
        self:hideAllHeadNTTips()
        self:ui_open()
        performWithDelay(self.ui_root,function()
            self:MakeNTAnimate(resp_json.NtUser)
        end,
        0.5)
    else
        self:updatePlayerInfo(resp_json.NtUser)
        self:ui_open()

    end 
    print("MJPlayersHeadLayer:onMakeNT  over")
end

--播放选庄动画 
function MJPlayersHeadLayer:MakeNTAnimate(nt_server_chair)
    print("MJPlayersHeadLayer:MakeNTAnimate")

    if self.makeNTLayer ~= nil then
        self.makeNTLayer:removeFromParent()
        self.makeNTLayer = nil
    end

    local makeNTLayer = cc.CSLoader:createNode("ui_csb/MakeNTLayer.csb")
    self.makeNTLayer = makeNTLayer
    self.ui_root:addChild(makeNTLayer)

    local ui_chair_index = GameSceneModule:getInstance():getGameScene():getGameDataManager():getUIChairByServerChair(nt_server_chair)
    local AnimateSeq = {}

    for i = 0,23 + ui_chair_index do
        AnimateSeq[i+1] = i%4 + 1
    end
    
    self:HeadBlink(1,AnimateSeq,ui_chair_index)

end

--头像闪烁
function MJPlayersHeadLayer:HeadBlink(index , AnimateSeq,ui_chair_index)
    --print("MJPlayersHeadLayer:HeadBlink")
 
    
    if index <= #AnimateSeq then
        --blink
        MJEffectManager:playOtherEffect(EffectEnum.PAOD,false)
        self.PlayerHeads[AnimateSeq[index]].Image_paodeng:setVisible(true)

        local blink = cc.Blink:create(0.1,1)
        local callfunc = cc.CallFunc:create(function ()
            
            self.PlayerHeads[AnimateSeq[index]].Image_paodeng:setVisible(false)
		     index = index + 1
             self:HeadBlink(index,AnimateSeq,ui_chair_index)
	    end)
        local sequence = cc.Sequence:create(blink, callfunc)

        self.PlayerHeads[AnimateSeq[index]].Image_paodeng:runAction(sequence)
    else
        --over
        self.PlayerHeads[AnimateSeq[#AnimateSeq]].Image_paodeng:setVisible(true)
        
        --
        local panel = self.makeNTLayer:getChildByName("Panel_MakeNT")
        local NTTip = panel:getChildByName("Image_nt")
        local point_end  = self.PlayerHeads[ui_chair_index].Image_NT:convertToWorldSpace(cc.p(0, 0))
        point_end.x = point_end.x + NTTip:getContentSize().width/2
        point_end.y = point_end.y + NTTip:getContentSize().height/2
        local moveto = cc.MoveTo:create(0.5,point_end)
        local callfunc = cc.CallFunc:create(function ()

            
            self:showHeadNTTip(ui_chair_index)
            --移除当前层 
            self.makeNTLayer:removeFromParent()
            self.makeNTLayer = nil
            --向服务器发送选庄动画结束消息

             local request = {}
             GameSceneModule:getInstance():getGameScene():getHelper():sendGameMessage(MJMessageID.ASS_ASSIGN_MAKENT_FINISH,request)
        end)

        local sequence = cc.Sequence:create(moveto, callfunc)
        NTTip:runAction(sequence)
        
    end

end


--服务器下发手牌
function MJPlayersHeadLayer:onFetchCards(resp_json)
    
end


function MJPlayersHeadLayer:onGameResult(resp_json)
    
    --更新所有玩家的总分数
    local deskInfo = GameSceneModule:getInstance():getGameScene():getDeskInfo()
     local isJinbi = false
    if deskInfo then
        local role = deskInfo.desk_select_config.Role
        if role then
            if role.id == 1 then
                isJinbi = true
            end
        end
    end

    if GameSceneModule:getInstance():getGameScene():getGameType() ~= "tz1" and isJinbi == false then
        for i = 1,PLAYER_COUNT do
            local ui_chair_index = GameSceneModule:getInstance():getGameScene():getGameDataManager():getUIChairByServerChair(i-1)
            self.PlayerHeads[ui_chair_index].Text_score:setText(tostring(resp_json.TotalScore[i]))
        end
    end
end


--更新所有玩家的头像  nt_chair 庄家的id
function MJPlayersHeadLayer:updatePlayerInfo(nt_chair)
    
    print("update Player Info nt_chair = " .. nt_chair)
    --玩家自己的椅子号
    local selfChair = GameSceneModule:getInstance():getGameScene():getGameDataManager():getSelfChair()
    if selfChair == nil then
        print("error selfChair is nil 自己的椅子号不能为空")
        return 
    end

    if selfChair < 0 then
        for i,player_info in pairs(GameSceneModule:getInstance():getGameScene():getGameDataManager():getAllUserData()) do
            if player_info.dwUserID == GameSceneModule:getInstance():getGameScene():getGameDataManager():getSelfUserID() then
                
                GameSceneModule:getInstance():getGameScene():getGameDataManager():setSelfChair(player_info.bDeskStation)
                selfChair = player_info.bDeskStation
                break
            end
        end
    end


    for i = 0,PLAYER_COUNT - 1 do
        
        --获取客户端椅子id
        local current_server_chair = (selfChair + i)%PLAYER_COUNT;
        local ui_chair_index = GameSceneModule:getInstance():getGameScene():getGameDataManager():getUIChairByServerChair(current_server_chair)
        self.PlayerHeads[ui_chair_index]:setVisible(true)
        self.PlayerHeads[ui_chair_index]:setPosition(PlayingPos[ui_chair_index])
        if PLAYER_COUNT == 2 and ui_chair_index == 3 then
            self.PlayerHeads[ui_chair_index]:setPositionX(240)
        end
        local allUserInfo = GameSceneModule:getInstance():getGameScene():getGameDataManager():getAllUserData()
        local player_info = allUserInfo[current_server_chair+1]

        local isNt = false

        if nt_chair == current_server_chair then
            isNt = true
        end
        if player_info ~= nil then
            
            --更新头像
            local ui_chair_index = GameSceneModule:getInstance():getGameScene():getGameDataManager():getUIChairByServerChair(player_info.bDeskStation)
            m_download:get_instance():set_head_image_and_auto_update( self.PlayerHeads[ui_chair_index].Image_head_wx ,player_info.avatarUrl , tostring(player_info.dwUserID) )
            print("updatePlayerInfo 更新ui_chair_index = ",ui_chair_index,"的玩家分数为 : ",player_info.dwMoney)
            self:updatePlayerNameAndScore(ui_chair_index,player_info.nickName,player_info.dwMoney,isNt)
            self:updateIsMater(i)
        else
            print("!!!获取玩家缓存信息失败!!!")
        end
    end
end

--显示头像庄家标志
function MJPlayersHeadLayer:showHeadNTTip(ui_nt_chair)
    --
     self.PlayerHeads[ui_nt_chair].Image_paodeng:setVisible(false)

     for i = 1,4 do
        if i == ui_nt_chair then
            self.PlayerHeads[i].Image_NT:setVisible(true)
        else
            self.PlayerHeads[i].Image_NT:setVisible(false)
        end
    end
end

--隐藏
function MJPlayersHeadLayer:hideAllHeadNTTips()
     for i = 1,4 do
        self.PlayerHeads[i].Image_NT:setVisible(false)
    end
end


--继续游戏处理
function MJPlayersHeadLayer:onAgreeGame(resp_json)
    print("MJPlayersHeadLayer:onAgreeGame begin")

    local dataMnger = GameSceneModule:getInstance():getGameScene():getGameDataManager()
--    if dataMnger:getSelfChair() ~= resp_json.chair then
--        return
--    end

    local ui_chair_index = GameSceneModule:getInstance():getGameScene():getGameDataManager():getUIChairByServerChair(resp_json.chair)
    print("MJPlayersHeadLayer:onAgreeGame resp_json.chair:"..resp_json.chair.."chair_index"..ui_chair_index)
    for i = 1,4 do
        --self.PlayerHeads[i].Image_piao:setVisible(false)
        --显示玩家准备图标
        if i == ui_chair_index then   
            print("MJPlayersHeadLayer:onAgreeGame show readimage true"..i)
            self:ShowReadImage(i,true)
        end
    end

end

function MJPlayersHeadLayer:ShowReadImage(ui_chair_index,visiable)
    print("MJPlayersHeadLayer:ShowReadImage "..ui_chair_index.."visiable "..tostring(visiable))

    if GameSceneModule:getInstance():getGameScene():getGameDataManager():getGameStation() == GameStation.GS_PLAYING and visiable then
        return
    end

    self.PlayerHeads[ui_chair_index].Image_ready:setVisible(visiable)
end


--断线重连处理
function MJPlayersHeadLayer:onGameStation(resp_json)
    print("MJPlayersHeadLayer:onGameStation")
    
    if resp_json.GSID==GameStation.GS_WAIT_ARGEE or resp_json.GSID==GameStation.GS_WAIT_SETGAME then          --跳牌状态
        self:showSitUsers()
    elseif resp_json.GSID == GameStation.GS_PLAYING then      --玩家打牌状态
        for i = 1,4 do
            self:ShowReadImage(i,false)
        end

        self:updatePlayerInfo(resp_json.NTUser)
        self:showHeadNTTip(GameSceneModule:getInstance():getGameScene():getGameDataManager():getUIChairByServerChair(resp_json.NTUser))

        --倒计时断线重连处理
         if resp_json.actResp.usFlags <= 0 or resp_json.TokenUser ==resp_json.chair  then
             
             local ui_chair_index = GameSceneModule:getInstance():getGameScene():getGameDataManager():getUIChairByServerChair(resp_json.TokenUser)
             self.curCountDownIndex = ui_chair_index
             self:hideAllCountDown()
             
        end

        --如果加飘 要做玩家加飘断线重连处理
        local deskInfo = GameSceneModule:getInstance():getGameScene():getDeskInfo()
        local isPiao = false
        if deskInfo and deskInfo.desk_select_config.Select ~= nil then
            for i,v in pairs(deskInfo.desk_select_config.Select) do
                if v.id == 4 then
                    isPiao = true
                    break
                end
            end
        end
       
        
        self:showUserNetCutState(resp_json.chair)
        self:ui_open()
    elseif  resp_json.GSID==GameStation.GS_WAIT_PIAO then 
       self:updatePlayerInfo(resp_json.NTUser)
       self:showHeadNTTip(GameSceneModule:getInstance():getGameScene():getGameDataManager():getUIChairByServerChair(resp_json.NTUser))
      
       --不显示'准备'标识
       for i = 1,4 do
           self.PlayerHeads[i].Image_ready:setVisible(false)
       end
       
       self:showUserNetCutState(resp_json.chair)
       self:ui_open()
    elseif resp_json.GSID == GameStation.GS_WAIT_NEXT_ROUND then
        self:updatePlayerInfo(resp_json.NTUser)
        self:showUserNetCutState(resp_json.chair)
        print("MJPlayersHeadLayer:onGameStation set imageready state")
        for i = 1,#resp_json.IsAgree  do
            local ui_chair_index = GameSceneModule:getInstance():getGameScene():getGameDataManager():getUIChairByServerChair(i-1)
            self:ShowReadImage(ui_chair_index,resp_json.IsAgree[i])
        end
        
       
        self:ui_open()
    end
   
end

--更新每个玩家的掉线状态
function MJPlayersHeadLayer:showUserNetCutState(selfChair)
    
    local sitDownUsers = GameSceneModule:getInstance():getGameScene():getSitDownPlayerMap()
    for key, sitPlayer in pairs(sitDownUsers) do
        local ui_chair_index = GameSceneModule:getInstance():getGameScene():getGameDataManager():getUIChairByServerChair(sitPlayer.bDeskStation)
        if sitPlayer.status == 20 then
            print("MJPlayersHeadLayer 椅子号 ui_chair_index ",ui_chair_index)
            self.PlayerHeads[ui_chair_index].Image_offline:setVisible(true)
        end
    end
end


--设置牌背颜色
function MJPlayersHeadLayer:onCardColorChange()
    --print("MJPlayersHeadLayer:onCardColorChange")
    for i = 1 ,4  do
         local Image_head_bg = self.PlayerHeads[i].Image_head_wx:getChildByName("Image_head_bg")
        Image_head_bg:loadTexture( COLOR_RES_PATH[UserData:getCardColor()] .. "touxiangkuang.png")
    end
    --print("MJPlayersHeadLayer:onCardColorChange end")
end

--某玩家收到令牌
function MJPlayersHeadLayer:onGetToken(resp_json)
    print("MJPlayersHeadLayer:onGetToken")

    
    local ui_chair_index = GameSceneModule:getInstance():getGameScene():getGameDataManager():getUIChairByServerChair(resp_json.TokenbyUser)
    self.curCountDownIndex = ui_chair_index
    self:hideAllCountDown()
    
    print("MJPlayersHeadLayer:onGetToken over")
end

--某玩家出牌
function MJPlayersHeadLayer:onOutCardInfo(resp_json)
    print("MJPlayersHeadLayer:onOutCardInfo")
    
    self:hideAllCountDown()
    self:stopCurCountDownOverBlink()
end

function MJPlayersHeadLayer:onActInfo(resp_json)
    print("MJPlayersHeadLayer:onActInfo")
    
    self:hideAllCountDown()
    self:stopCurCountDownOverBlink()
end


function MJPlayersHeadLayer:onOtherPlayerOffline(user_info)

    if user_info.bDeskStation >= 0  and user_info.bDeskStation < PLAYER_COUNT then
        local ui_chair_index = GameSceneModule:getInstance():getGameScene():getGameDataManager():getUIChairByServerChair(user_info.bDeskStation)
        self.PlayerHeads[ui_chair_index].Image_offline:setVisible(true)
    end
end

function MJPlayersHeadLayer:onPlayerJoinDesk( user_info , desk_info , code )
    --重现连接回来
    print("MJPlayersHeadLayer 有玩家加入桌子",code)
    if code == 0 or code == nil or code==50 then
        if user_info.bDeskStation >= 0  and user_info.bDeskStation < PLAYER_COUNT then
            local ui_chair_index = GameSceneModule:getInstance():getGameScene():getGameDataManager():getUIChairByServerChair(user_info.bDeskStation)
            print("MJPlayersHeadLayer" , ui_chair_index,"号玩家隐藏头像离线标志")
            self.PlayerHeads[ui_chair_index].Image_offline:setVisible(false)
        end
    end
    
end

--隐藏某个头像倒计时
function MJPlayersHeadLayer:hideCountDownAt(ui_chair_index)

end

function MJPlayersHeadLayer:hideAllCountDown()
    for i= 1,4 do
        self:hideCountDownAt(i)
    end
end

--停止当前倒计时结束闪烁动画
function MJPlayersHeadLayer:stopCurCountDownOverBlink()
    print("MJPlayersHeadLayer:stopCurCountDownOverBlink")
    self.curCountDountTime = 15
   
end

function MJPlayersHeadLayer:ui_open()
    self.ui_project:setVisible(true)
end

function MJPlayersHeadLayer:ui_close()
    self.ui_project:setVisible(false)
end

function MJPlayersHeadLayer:OnRcvGameMessage(msgID,msgData)
    if msgID == MJMessageID.ASS_GAME_BEGIN then
        self:onGameStart(msgData)
    elseif msgID == MJMessageID.ASS_MAKE_NT_INFO then
        self:onMakeNT(msgData)
    elseif msgID == MJMessageID.ASS_FETCH_HANDCARDS then
        self:onFetchCards(msgData)
    elseif msgID == MJMessageID.ASS_TOKEN then
        self:onGetToken(msgData)
    elseif msgID == MJMessageID.ASS_TIAO_CARDS then
        
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

return MJPlayersHeadLayer

--endregion
