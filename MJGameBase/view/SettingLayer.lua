--region *.lua
--Date
--此文件由[BabeLua]插件自动生成

local MJMessageID = import("..MJMessageID")
local SettingLayer = class("SettingLayer",function() 
        return cc.CSLoader:createNode("ui_csb/SettingLayer.csb") 
    end)
local SET_CLOSE = 0
local SET_OPEN = 1

function SettingLayer:ctor(ui_root)
    
    self.ui_root = ui_root
    local Panel_root = self:getChildByName("Panel_setting")
    local Image_bg = Panel_root:getChildByName("Image_bg")

    --关闭按钮
    local button_close = Image_bg:getChildByName("Button_close")
    button_close:addClickEventListener(function() 
        self:removeFromParent()
    end)

    
    --方言开关
    local Button_langua_off = Image_bg:getChildByName("Button_langua_off")
    local Button_langua_on = Image_bg:getChildByName("Button_langua_on")
    self:ButtonToggle( Button_langua_off , Button_langua_on , UserData:getLanguage() , function(cur_state)
            UserData:setLanguage( cur_state )
        end )

    --语音开关(是否手动播放)
    local button_voice_off = Image_bg:getChildByName("Button_voice_off")
    local button_voice_on = Image_bg:getChildByName("Button_voice_on")
    self:ButtonToggle( button_voice_on , button_voice_off , UserData:getAutoPlaySound() , function(cur_state)
            UserData:setAutoPlaySound(cur_state)
        end)

    local Text_language = Image_bg:getChildByName("Text_language")
    if CUR_LANGUAGE_PREFER == LANGUAGE_PREFER.ONLY_PUTONG then
        --如果只有普通话
        Button_langua_off:setVisible(false)
        Button_langua_on:setVisible(false)
        UserData:setLanguage( SET_OPEN )
        Text_language:setVisible(false)
        
        --将语音按钮位置提前
        button_voice_off:setPosition( Button_langua_off:getPosition() )
        button_voice_on:setPosition( Button_langua_on:getPosition() )
        Image_bg:getChildByName("Text_voice"):setPosition( Text_language:getPosition() )
    elseif CUR_LANGUAGE_PREFER == LANGUAGE_PREFER.ONLY_FANGYAN then
        --如果只有方言
        Button_langua_off:setVisible(false)
        Button_langua_on:setVisible(false)
        UserData:setLanguage( SET_CLOSE )
        Text_language:setVisible(false)
    end


    --音效开关
    local button_effect_off = Image_bg:getChildByName("Button_effect_off")
    local button_effect_on = Image_bg:getChildByName("Button_effect_on")
    self:ButtonToggle( button_effect_off , button_effect_on , UserData:getSound() , function(cur_state)
            UserData:setSound( cur_state )
        end )

    --背景音乐开关
    local button_music_off = Image_bg:getChildByName("Button_music_off")
    local button_music_on = Image_bg:getChildByName("Button_music_on")
    self:ButtonToggle( button_music_off , button_music_on , UserData:getMusic() , function(cur_state)
            UserData:setMusic( cur_state )
            if cur_state == 1 then
                if IS_PLAY_GAME_MUSIC then
                    Music:playMusic("sound/gamebg.mp3")
                end
            else
                if IS_PLAY_GAME_MUSIC then
                    Music:stopMusic()
                end
            end
        end )

    

    --同桌游平台不显示自动手动
    if GameSceneModule:getInstance():getGameScene():getGameType() == "tz1" then
        button_voice_off:setVisible(false)
        button_voice_on:setVisible(false)
        Image_bg:getChildByName("Text_voice"):setVisible(false)
    end



    --更换牌背
    local Image_cardcolor_bg = Image_bg:getChildByName("Image_cardcolor_bg")
    local button_change_cardcolor_left = Image_cardcolor_bg:getChildByName("Button_left")
    local button_change_cardcolor_right = Image_cardcolor_bg:getChildByName("Button_right")
    self.Image_card_color = Image_cardcolor_bg:getChildByName("Image_card_color")--牌背图
    button_change_cardcolor_left:addClickEventListener(function() 
        print("button_change_cardcolor_left")
        local newColor = UserData:getCardColor() - 1
        if newColor < 1 then
            newColor = newColor + 3
        end
        self:setCardColor(newColor)
    end)
    button_change_cardcolor_right:addClickEventListener(function() 
        print("button_change_cardcolor_right")
        local newColor = UserData:getCardColor() + 1
        if newColor > 3 then
            newColor = newColor % 3
        end
        self:setCardColor(newColor)
    end)


    --更换桌布
    local Image_bgcolor_bg = Image_bg:getChildByName("Image_bgcolor_bg")
    local button_change_bgcolor_left = Image_bgcolor_bg:getChildByName("Button_left")
    local button_change_bgcolor_right = Image_bgcolor_bg:getChildByName("Button_right")
    self.Image_bg_color = Image_bgcolor_bg:getChildByName("Image_bg_color")--桌布图
    button_change_bgcolor_left:addClickEventListener(function() 
        print("button_change_bgcolor_left")
        local newColor = UserData:getBgColor() - 1
        if newColor < 1 then
            newColor = newColor + 3
        end
        self:setBgColor(newColor)

    end)
    button_change_bgcolor_right:addClickEventListener(function() 
        print("button_change_bgcolor_right")
        local newColor = UserData:getBgColor() + 1
        if newColor > 3 then
            newColor = newColor % 3
        end
        self:setBgColor(newColor)

    end)

    --离开
    local Button_leave = Image_bg:getChildByName("Button_leave")
    local Image_leave_txt = Button_leave:getChildByName("Image_leave_txt")
    --房主显示解散房间
    local GameScene = GameSceneModule:getInstance():getGameScene()
    local deskInfo = GameScene:getDeskInfo()
    local chair = GameSceneModule:getInstance():getGameScene():getGameDataManager():getSelfChair()
    
    local allUserInfo = GameSceneModule:getInstance():getGameScene():getGameDataManager():getAllUserData()
    local player_info = allUserInfo[chair+1]


    if GameSceneModule:getInstance():getGameScene():getGameType() == "tz1" then
        --金币场
        Button_leave:addClickEventListener(function() 
            print("Button_leave-----")
            GameSceneModule:getInstance():getGameScene():Exit_game()
        end)
    else
        --开房模式
        if deskInfo.mastUserID  == player_info.dwUserID then
             print("set release room png")
             Image_leave_txt:loadTexture("ui_res/Setting/ReleaseRoom.png")
        else
            local  curGameStation = GameSceneModule:getInstance():getGameScene():getGameDataManager():getGameStation();
            print("curGameStation    =    " , curGameStation)
           if curGameStation <= GameStation.GS_WAIT_ARGEE then
                --游戏未开始
            else
                --游戏已经开始
                Image_leave_txt:loadTexture("ui_res/Setting/ReleaseRoom.png")
            end

        end

        Button_leave:addClickEventListener(function() 
            print("Button_leave-----")
            --申请站起
            local GameScene = GameSceneModule:getInstance():getGameScene()
            local deskInfo = GameScene:getDeskInfo()
            local chair = GameScene:getGameDataManager():getSelfChair()
            local allUserInfo = GameScene:getGameDataManager():getAllUserData()
            local player_info = allUserInfo[chair+1]
        
        
            --玩家离开游戏
            local  curGameStation =  GameScene:getGameDataManager():getGameStation();
            print("curGameStation    =    " .. curGameStation)
            if curGameStation <= GameStation.GS_WAIT_ARGEE then
                print("GameScene:reqDissmisDesk    game not start ")
                --游戏未开始 申请离开桌子
                if deskInfo.mastUserID  == player_info.dwUserID then
                    --房主申请解散房间

                    api_show_Msg_Tip(
                   "是否立刻解散房间",
                     function()
                        --同意解散房间
                        GameScene:reqDissmisDesk(0)
                     end,
                     function()
               
                     end
                     ,true)
                
                else
                    local  request = {}
                    GameSceneModule:getInstance():getGameScene():getHelper():sendGameMessage(MJMessageID.ASS_QUIT_REQ,request)
                    GameSceneModule:getInstance():getGameScene():reqLeaveDesk()
                end
            else
                print("GameScene:reqDissmisDesk    type = 1 ")
                GameScene:reqDissmisDesk(1)

                --请求解散房间
            end
        
            self:removeFromParent()
        end)

    end

    

    --显示之前设置的牌背和桌布颜色
    self:setCardColor(UserData:getCardColor())
    self:setBgColor(UserData:getBgColor())


    self:registerScriptHandler(handler(self,self.onNodeEvent))
end

function SettingLayer:onNodeEvent(event)

    if event == "enter" then
        g_pushBombBox(self)
    elseif event == "exit" then
        g_popBombBox(self)
    end
end


function SettingLayer:ButtonToggle(Button_close, Button_open, cur_state, function_call)
    Button_close:addClickEventListener(function()
        function_call(SET_OPEN)
        Button_open:setVisible(true)
        Button_close:setVisible(false)
    end)
    Button_open:addClickEventListener(function()
        function_call(SET_CLOSE)
        Button_open:setVisible(false)
        Button_close:setVisible(true)
    end)
    if cur_state==SET_OPEN then
        Button_open:setVisible(true)
        Button_close:setVisible(false)
    end
    if cur_state==SET_CLOSE then
        Button_open:setVisible(false)
        Button_close:setVisible(true)
    end
end

--设置牌背的颜色
function SettingLayer:setCardColor(color)
    print("SettingLayer:setCardColor color = " .. color)
    UserData:setCardColor(color)
    self.Image_card_color:loadTexture(COLOR_RES_PATH[color] .. "paibei.png")

    self.ui_root.MJGameBgLayer:onCardColorChange()
    self.ui_root.MJHandCardLayer:onCardColorChange()
    self.ui_root.MJOutCardLayer:onCardColorChange()
    self.ui_root.MJPlayersHeadLayer:onCardColorChange()
end

--设置桌布的颜色
function SettingLayer:setBgColor(color)
    print("SettingLayer:setBgColor color = " .. color)
    UserData:setBgColor(color)
    self.Image_bg_color:loadTexture(COLOR_RES_PATH[color] .. "zhuobu.png")
    self.ui_root:onBgColorChange()
end

return SettingLayer

--endregion
