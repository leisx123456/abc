--region *.lua
--Date
--此文件由[BabeLua]插件自动生成

local MJMessageID = import("..MJMessageID")
local SettingLayer = import('..view.SettingLayer')
local m_download = import("....common.download")
local MJMessageID = import("..MJMessageID")
local Algorithm = import("..."..GAME_ID..".Algorithm")

local MJMenuLayer = class("MJMenuLayer")
function MJMenuLayer:ctor(ui_project, ui_root)
    print("MJMenuLayer:ctor")
    self.ui_project = ui_project
    self.ui_root = ui_root
    --self.m_voiceLayer = nil

    --房主解散房间

    local Button_dismiss_desk = ui_project:getChildByName("Button_dismiss_desk")
    self.Button_dismiss_desk  = Button_dismiss_desk
    if GameSceneModule:getInstance():getGameScene():getGameDataManager():getMasterUserID() == GameSceneModule:getInstance():getGameScene():getGameDataManager():getSelfUserID() then
        --房主
        Button_dismiss_desk:addClickEventListener(function()
           print("房主解散房间")

           api_show_Msg_Tip(
           "是否立刻解散房间",
             function()
               --同意解散房间
               GameSceneModule:getInstance():getGameScene():reqDissmisDesk(0)
             end,
             function()

             end
             ,true)

        end)

    else
        --闲家
        Button_dismiss_desk:loadTextures("ui_res/tuichu.png","ui_res/tuichu.png","")
        Button_dismiss_desk:addClickEventListener(function()
           print("闲家申请离开房间")
           local  request = {}
           GameSceneModule:getInstance():getGameScene():getHelper():sendGameMessage(MJMessageID.ASS_QUIT_REQ,request)
           GameSceneModule:getInstance():getGameScene():reqLeaveDesk()
        end)
    end
    --邀请微信好友
    local Button_invite_wx = ui_project:getChildByName("Button_invite_wx")
    self.Button_invite_wx = Button_invite_wx
    Button_invite_wx:addClickEventListener(function()
       print("invite weixin friend")
       local deskInfo = GameSceneModule:getInstance():getGameScene():getDeskInfo()
       local RoomConfig = ""

       if deskInfo.desk_select_config.Role ~= nil then
            RoomConfig = RoomConfig.." "..deskInfo.desk_select_config.Role.desc
       end
       if deskInfo.desk_select_config.Round ~= nil then
            RoomConfig = RoomConfig.."   "..deskInfo.desk_select_config.Round.desc
       end
       if deskInfo.desk_select_config.Select ~= nil then
            for j,iditem in pairs(deskInfo.desk_select_config.Select) do
                RoomConfig = RoomConfig .." "..iditem.desc
            end
       end
       if deskInfo.desk_select_config.ZhuaNiao ~= nil then
            RoomConfig = RoomConfig.." "..deskInfo.desk_select_config.ZhuaNiao.desc
       end
      

        local titleStr = "转转麻将,房号:"
        titleStr = titleStr .. deskInfo.roomKey
        
        
        if deskInfo.desk_select_config.Round ~= nil then
            titleStr = titleStr .. "("
            titleStr = titleStr ..deskInfo.desk_select_config.Round.Round .."局"
            titleStr = titleStr .. ")"
        end

       GameSceneModule:getInstance():getGameScene():reqShareToWXSession(titleStr,RoomConfig)
    end
    ) 


   

    --聊天按钮
    local Button_chat = ui_project:getChildByName("Button_chat")
    Button_chat:addClickEventListener(function()
       print("click Button_chat button")

       GameSceneModule:getInstance():getGameScene():showChat()

    end
    )
    self.m_btnChat=Button_chat


    --设置
    local Button_set = ui_project:getChildByName("Button_set")
    Button_set:addClickEventListener(function()
       --print("click Button_set button")
       ui_root:addChild(SettingLayer:create(self.ui_root))
    end
    )

    --返回
    local Button_back = ui_project:getChildByName("Button_back")
    self.Button_back = Button_back
    Button_back:addClickEventListener(function()
       --print("click Button_back button")
       --GameApp:enterScene("room.view.hall.HallScene")
       --GameSceneModule:getInstance():getGameScene():reqLeaveRoom()
       --GameSceneModule:getInstance():getGameScene():Exit_game()

       local  curGameStation = GameSceneModule:getInstance():getGameScene():getGameDataManager():getGameStation()
       print("curGameStation    =    " .. curGameStation)

       if curGameStation <= GameStation.GS_WAIT_ARGEE then
            --游戏未开始 返回到大厅
            if GameSceneModule:getInstance():getGameScene():getGameDataManager():getMasterUserID() == GameSceneModule:getInstance():getGameScene():getGameDataManager():getSelfUserID() then
                GameSceneModule:getInstance():getGameScene():Exit_game()
            else
                GameSceneModule:getInstance():getGameScene():reqLeaveDesk()
               -- GameSceneModule:getInstance():getGameScene():Exit_game()
            end
       else
            --游戏已经开始 弹出设置界面 提示解散或者离开桌子
            ui_root:addChild(SettingLayer:create(self.ui_root))
       end
        
    end
    )

    
--    if GameSceneModule:getInstance():getGameScene():getGameType() ~= "tz1" then
--        local quickVoiceLayer = import("app.platform.common.QuickVoiceLayer")
--        local scene = GameSceneModule:getInstance():getGameScene()
--        self.m_voiceLayer = quickVoiceLayer:create( scene:getSelfInfo().dwUserID ,  handler(scene, scene.reqRecordSound) )
--        self.m_voiceLayer:set_voice_image("ui_res/voice.png")
--        self.m_voiceLayer:set_voice_postion( 1220 , 200 )
--        self.m_voiceLayer:setZOrder( 0 ) 
--        self.ui_root:addChild(self.m_voiceLayer)
--    end

    


    --飘菜单
    local Panel_piao_root = ui_project:getChildByName("Panel_piao")
    self.Panel_piao_root = Panel_piao_root
    for i=1,3 do
        local Button_piao = Panel_piao_root:getChildByName("Button_piao_" .. i)
        Button_piao:addClickEventListener(function()
          
           if i == 1 then
                print("click not piao ")
           elseif i== 2 then
                print("click piao 1")
           else
                print("click piao 2")
           end
           local  request = {}
           request.piaoType = i - 1
           GameSceneModule:getInstance():getGameScene():getHelper():sendGameMessage(MJMessageID.ASS_PIAO_REQ,request)
           self:setPiaoMenuVisible(false)
        end
        )
    end

    --查听按钮
    local Button_check_ting = ui_project:getChildByName("Button_check_ting")
    self.Button_check_ting = Button_check_ting
    Button_check_ting:setPosition( 1220 , 280 )
    local function touchEvent(sender,eventType)
        if eventType == ccui.TouchEventType.began then
            print("checkting began")
            local dataMnger = GameSceneModule:getInstance():getGameScene():getGameDataManager()
            local isCan7Dui = dataMnger:getIsCanHu7Dui()
            local selfCards = dataMnger:getSelfHandCards()
            local tingcards = Algorithm:getTingCards_L(clone(selfCards),isCan7Dui)
            if #tingcards > 0 then
               
                GameSceneModule:getInstance():getGameScene():getGameLayerManager():showCheckTingLayer(self.ui_root,tingcards,GameSceneModule:getInstance():getGameScene():getGameDataManager():getShowCards())
            else
                print("no ting cards why show check ting button")
            end
            
        elseif eventType == ccui.TouchEventType.moved then
            print("checkting moved")
        elseif eventType == ccui.TouchEventType.ended then
            print("checkting ended")
            GameSceneModule:getInstance():getGameScene():getGameLayerManager():hideCheckTingLayer()
        elseif eventType == ccui.TouchEventType.canceled then
            print("checkting canceled")
            GameSceneModule:getInstance():getGameScene():getGameLayerManager():hideCheckTingLayer()
        end
    end
    Button_check_ting:addTouchEventListener(touchEvent)
   
    Button_check_ting:setVisible(false)


    --打骰子按钮
    local Button_dashai = ui_project:getChildByName("Button_dashai")
    self.Button_dashai = Button_dashai

    Button_dashai:addClickEventListener(function()
        print("click dashaizi button")

        local  request = {}
        GameSceneModule:getInstance():getGameScene():getHelper():sendGameMessage(MJMessageID.ASS_USER_DASHAI_REQ,request)
        self.Button_dashai:setVisible(false)
    end
    )
    self.Button_dashai:setVisible(false)
    Panel_piao_root:setVisible(false)

    
    local dataManager = GameSceneModule:getInstance():getGameScene():getGameDataManager()
    dataManager:getSelfIsAuto()
    --托管
    local Button_auto = ui_project:getChildByName("Button_auto")
    self.Button_auto = Button_auto
    Button_auto:addClickEventListener(function()
       print("click Button_auto button")
       local request = {}

       if dataManager:getSelfIsAuto() == false then
            print("发送托管消息")
            request.IsAuto = true;--是否托管
            Button_auto:loadTextures("ui_res/quxiao.png","","")

       else
            print("发送取消托管消息")
            request.IsAuto = false;--是否托管
            Button_auto:loadTextures("ui_res/tuoguan.png","","")
       end


       GameSceneModule:getInstance():getGameScene():getHelper():sendGameMessage(MJMessageID.ASS_AUTO_REQ,request)
    end
    )
    Button_auto:setVisible(false)


    print("MJMenuLayer:ctor  end")

    if g_server_obj:get_server_type() == "video" then
        Button_chat:removeFromParent()
        Button_set:removeFromParent()
        Button_back:removeFromParent()
        self.Button_back = nil
        self.m_btnChat=nil
    end

    if GameSceneModule:getInstance():getGameScene():getGameType() == "tz1" then
       --金币房没有邀请微信好友 解散房间 和 快捷聊天按钮
       Button_invite_wx:setVisible(false)
       Button_dismiss_desk:setVisible(false)
       Button_chat:setVisible(false)
       --金币房间有托管按钮
       Button_auto:setVisible(true)
    end

end


function MJMenuLayer:onAutoInfo(resp_json)
    local dataManager = GameSceneModule:getInstance():getGameScene():getGameDataManager()
    if resp_json.IsAuto and resp_json.chair==dataManager:getSelfChair() then
        self.Button_auto:loadTextures("ui_res/quxiao.png","","") 
    end
end


--某玩家的出牌
function MJMenuLayer:onOutCardInfo(resp_json)
    print("MJMenuLayer:onOutCardInfo")


    local dataMnger = GameSceneModule:getInstance():getGameScene():getGameDataManager()
    if dataMnger:getIsOpenCheckTing() == false then
        return
    end
    
    local ui_chair_index = GameSceneModule:getInstance():getGameScene():getGameDataManager():getUIChairByServerChair(tonumber(resp_json.outCardUser))
    if ui_chair_index == 1 then
        local CurHandCards = clone(resp_json.HandCards)
        local isCan7Dui = GameSceneModule:getInstance():getGameScene():getGameDataManager():getIsCanHu7Dui()
        local outTingCards = Algorithm:getTingCards_L(CurHandCards,isCan7Dui)
        if #outTingCards >= 1 then
            self.Button_check_ting:setVisible(true)
        else
            self.Button_check_ting:setVisible(false)
        end
   end
    print("MJMenuLayer:onOutCardInfo end")

end



--是否显示叫飘菜单
function MJMenuLayer:setPiaoMenuVisible(visible)
    self.Panel_piao_root:setVisible(visible)
end

--有玩家加飘
function MJMenuLayer:onPiao(resp_json)
    self:setPiaoMenuVisible(true)
    
end

--继续游戏
function MJMenuLayer:onAgreeGame(resp_json)
    print("MJMenuLayer:onAgreeGame")
    self.Button_check_ting:setVisible(false)

end

----有玩家离开桌子
--function MJMenuLayer:onPlayerLeaveDesk( user_info , desk_info , code)

--    if code == 0 then
--        --成功离开桌子
--        print("成功离开桌子")
--        if user_info.bDeskStation < 0  then
--            return
--        end


--    elseif code == 1 then
--        --1.正在游戏不准离开桌子 
--        print("正在游戏不准离开桌子")
--    elseif code == 2 then 
--        --2.已经离开不需要再次离开
--        print("已经离开不需要再次离开")
--    end
--end

function MJMenuLayer:onPlayerSit(user_info)

end

function MJMenuLayer:onGameStart()
    print("MJMenuLayer:onGameStart=======")
    if self.Button_back then
        self.Button_back:setVisible(false)
    end
    

    if GameSceneModule:getInstance():getGameScene():getGameType() == "tz1" then
        self.Button_auto:setVisible(true)
    end

    self.Button_invite_wx:setVisible(false)
    self.Button_dismiss_desk:setVisible(false)
end

function MJMenuLayer:onMakeNT()

end

function MJMenuLayer:onGetToken(resp_json)
    if resp_json.TokenbyUser == GameSceneModule:getInstance():getGameScene():getGameDataManager():getSelfChair() then
        self.Button_check_ting:setVisible(false)
    end
   
end

--服务器下发手牌
function MJMenuLayer:onFetchCards(resp_json)
    
end



function MJMenuLayer:setPlayerHeadImage(userInfo)

end

function MJMenuLayer:onGameStation(resp_json)
    print("MJMenuLayer:onGameStation")
    if resp_json.GSID == nil then
        return
    end

    print("resp_json.GSID = " .. resp_json.GSID)
    if resp_json.GSID==24 then          --跳牌状态
        
    elseif resp_json.GSID == GameStation.GS_PLAYING then      --玩家打牌状态

        local dataMnger = GameSceneModule:getInstance():getGameScene():getGameDataManager()
        if dataMnger:getIsOpenCheckTing() == true then
            if resp_json.HandCards then
                local CurHandcards = clone(resp_json.HandCards[resp_json.chair + 1])

                local isCan7Dui = GameSceneModule:getInstance():getGameScene():getGameDataManager():getIsCanHu7Dui()
                local outTingCards = Algorithm:getTingCards_L(CurHandcards,isCan7Dui)
                if #outTingCards >= 1 then
                    self.Button_check_ting:setVisible(true)
                else
                    self.Button_check_ting:setVisible(false)
                end
            end
        end
        
       
        self.Button_invite_wx:setVisible(false)
        self.Button_dismiss_desk:setVisible(false)

        if GameSceneModule:getInstance():getGameScene():getGameType() == "tz1" then
            if resp_json.IsAuto then
                self.Button_auto:loadTextures("ui_res/quxiao.png","","")
            end
        end

    elseif resp_json.GSID == GameStation.GS_WAIT_NEXT_ROUND then    --等待下一局

          self.Button_invite_wx:setVisible(false)
          self.Button_dismiss_desk:setVisible(false)
          
        if resp_json.IsAgree[resp_json.chair+1] == false then
            --发送继续游戏请求
            local request = {}
            GameSceneModule:getInstance():getGameScene():getHelper():sendGameMessage(MJMessageID.ASS_GM_AGREE_GAME,request)
        end

   elseif  resp_json.GSID == GameStation.GS_WAIT_PIAO then

          self.Button_invite_wx:setVisible(false)
          self.Button_dismiss_desk:setVisible(false)
          if resp_json.PiaoResp[resp_json.chair + 1].bPiao  == false   then
             self:onPiao(resp_json)
          end
    elseif resp_json.GSID == GameStation.GS_WAIT_SETGAME then
        print("MJMenuLayer:onGameStation  GS_WAIT_SETGAME")    
        local sitDownUsersNum = #GameSceneModule:getInstance():getGameScene():getGameDataManager():getAllUserData()
        if sitDownUsersNum == PLAYER_COUNT then
            
            if GameSceneModule:getInstance():getGameScene():getGameDataManager():getSelfUserID() == GameSceneModule:getInstance():getGameScene():getGameDataManager():getMasterUserID() then
                print("check ip same ?")
                GameSceneModule:getInstance():getGameScene():check_ip_same(PLAYER_COUNT)
            else
                print("not match master userid")
            end
        end
       
        local request = {}
        GameSceneModule:getInstance():getGameScene():getHelper():sendGameMessage(MJMessageID.ASS_GM_AGREE_GAME,request)

    end

    if resp_json.GSID ~= GameStation.GS_WAIT_SETGAME then
        if self.Button_back then
            self.Button_back:setVisible(false)
        end
    end

    print("MJMenuLayer:onGameStation end")
end

function MJMenuLayer:OnRcvGameMessage(msgID,msgData)
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
        
    elseif msgID == MJMessageID.ASS_RESULT then
        
    elseif msgID == MJMessageID.ASS_NIAOCARDS_INFO then
        
    elseif msgID == MJMessageID.ASS_PAIJU_INFO then
        
    elseif msgID == MJMessageID.ASS_GM_AGREE_GAME then
        self:onAgreeGame(msgData)
    elseif msgID == MJMessageID.ASS_GM_GAME_STATION then
        self:onGameStation(msgData)
    elseif msgID == MJMessageID.ASS_AUTO_INFO then
         self:onAutoInfo(msgData)

    end

end


return MJMenuLayer

--endregion
