--region *.lua
--Date
--此文件由[BabeLua]插件自动生成




GameSceneModule = import("..module.GameSceneModule")


print("=============================1======================")
--if io.open("app/platform/game/"..GAME_ID.."/manager/EffectManager.lua")~=nil then

if  EFFECT_MANAGER_ID~=nil and EFFECT_MANAGER_ID==GAME_ID then
    print("=============================1")
    MJEffectManager = import("..."..GAME_ID..".manager.EffectManager")
else
    print("=============================2")
    MJEffectManager = import("..manager.MJEffectManager")
end

local GameLayer = import('.GameLayer')

local MJGameResultLayer = import(".MJGameResultLayer")
--local GameResultLayerTag = 1001
local GameSceneBase = import("...GameSceneBase")

local GameVideoLayer = import("..."..GAME_ID..".view.GameVideoLayer")

--local GameVideoLayer = import(".MJGameVideoLayer")


local MJMessageID = import("..MJMessageID")

local m_download = import("....common.download")

--具体某个麻将的数据管理类



local GameDataManager = import("..."..GAME_ID..".manager.GameDataManager")

local GameLayerManager = import("..."..GAME_ID..".manager.GameLayerManager")

local GameDataModule = import("..."..GAME_ID..".module.GameDataModule")


local VoiceBox = import("...VoiceBox")
local ChatBox = import("...ChatBox")

local MJGameSceneBase = class("MJGameSceneBase" , GameSceneBase)


function MJGameSceneBase:onCreate()
  
    self.deskActive = false
    self.userAgree = { false , false , false , false , false }
    
    self:registered_child(self)
     


    self.GameDataModule = GameDataModule:create()
    self.GameDataManager = GameDataManager:create(self.GameDataModule)
    
    self.GameLayerManager = GameLayerManager:create()
    local deskInfo = self:getDeskInfo()

    if deskInfo then
        if deskInfo.desk_select_config.PlayerCount ~= nil then
            if deskInfo.desk_select_config.PlayerCount.Player~=nil then
                PLAYER_COUNT=deskInfo.desk_select_config.PlayerCount.Player
            else
                PLAYER_COUNT = deskInfo.desk_select_config.PlayerCount.PlayerCount
            end
            
        end

        if deskInfo.desk_select_config.Round ~= nil then
            self.GameDataManager:setAllRound(deskInfo.desk_select_config.Round.Round)
        end

    end


    if PLAYER_COUNT == 2 then
        EMOJI_POSITIONS = {
            cc.p(80,245),
            cc.p(1185,420),
            cc.p(288,613),
            cc.p(80,493)
        }
        
        CHAT_POSITIONS = {
        cc.p(80 + 50,245),
        cc.p(1185 - 50,420),
        cc.p(278 + 50,623),
        cc.p(80 + 50,493)
    }

    end


    GameSceneModule:getInstance():init(self)

    local GameLayer = GameLayer:create(self)
    self.GameLayer = GameLayer
    self:addChild(GameLayer)

    

    self.PaijuInfo = {} --牌局信息
  
    --设置已坐下的玩家
    self:setDeskPlayerHeadImage()
  
    if g_server_obj:get_server_type() == "video" and self.GameVideoLayer== nil then
        self.GameVideoLayer = GameVideoLayer:create( self )
        self.GameVideoLayer:init_event()

        self:addChild( self.GameVideoLayer )
        self.GameVideoLayer:setZOrder( 99999 )
    end

    self:registerScriptHandler(handler(self,self.onNodeEvent))
    
end



function MJGameSceneBase:setDeskPlayerHeadImage()
    for key, var in pairs(self:getSitDownPlayerMap()) do
        self.GameLayer.MJMenuLayer:setPlayerHeadImage(var)  
    end
end

function MJGameSceneBase:onNodeEvent(event)

    if event == "enter" then
       --返回键开启
       --g_setMainLayerDelegate(self)
       AudioEngine.preloadMusic( "sound/other/daojs.mp3" )

       self:loadResources()

       if IS_PLAY_GAME_MUSIC then
            performWithDelay(self,function()
                if UserData:getMusic() == 1 then
                    Music:playMusic("sound/gamebg.mp3")
                end
            end,0.1)
       end

    elseif event == "exit" then

        AudioEngine.unloadEffect( "sound/other/daojs.mp3" )
        self:onDestroy()
        self:releaseResources()
    end

end

--加载资源
function MJGameSceneBase:loadResources()

    AudioEngine.preloadMusic( "game_res/sound/mjaudio/other/daojs.mp3" )

    function imageLoaded(texture)
        --print("image loaded")
    end

--    for i = 1,37 do
--        if i%10 ~= 0 then
--            cc.Director:getInstance():getTextureCache():addImageAsync("game_res/"..GAME_ID.."/ui_res/Mahjong/Big/Big_"..tostring(i)..".png", imageLoaded)
--            cc.Director:getInstance():getTextureCache():addImageAsync("game_res/"..GAME_ID.."/ui_res/Mahjong/Smal/Smal_"..tostring(i)..".png", imageLoaded)
--        end
--    end

end

--释放资源
function MJGameSceneBase:releaseResources()

    AudioEngine.unloadEffect( "game_res/sound/mjaudio/other/daojs.mp3" )

    cc.Director:getInstance():getTextureCache():removeAllTextures()
--    for i = 1,37 do
--        if i%10 ~= 0 then
--            cc.Director:getInstance():getTextureCache():removeTextureForKey("game_res/"..GAME_ID.."/ui_res/Mahjong/Big/Big_"..tostring(i)..".png")
--            cc.Director:getInstance():getTextureCache():removeTextureForKey("game_res/"..GAME_ID.."/ui_res/Mahjong/Smal/Smal_"..tostring(i)..".png")
--        end
--    end

end

function MJGameSceneBase:onDestroy()
 
    self.GameDataManager:init()
    self.GameLayer.MJGameBgLayer:stopMiddleCount()

    --移除当前游戏的搜索路径
    local curSearchPath = cc.FileUtils:getInstance():getSearchPaths()

    local newSearchPath = {}
    local index = 1

    local isHave = false

    for i,v in pairs(curSearchPath )do
        local findIndex  = string.find(v,"MJGameBase")
        if findIndex == nil then
            findIndex = string.find(v,tostring(GAME_ID))
        end

        if findIndex == nil then
            newSearchPath[index] = v
            index = index + 1
        end
    end
    cc.FileUtils:getInstance():setSearchPaths(newSearchPath)


    for k,v in pairs(package.loaded) do
        local findIndex = string.find(k,"MJGameBase")
        if findIndex ~= nil then
            package.loaded[k]=nil
        end
    end

    for k,v in pairs(package.loaded) do
        local findIndex = string.find(k,tostring(GAME_ID))
        if findIndex ~= nil then
            package.loaded[k]=nil
        end
    end

--    package.loaded["app.platform.game.MJGameBase.view.MJGameSceneBase"]=nil
--    package.loaded["app.platform.game.MJGameBase.view.GameLayer"]=nil

end

function MJGameSceneBase:ChangeDesk()
        print("请求站起")
        g_server_obj:send_w_server_msg(102,1,1,true,nil)
        print("请求换桌")
        --BasicLuaSendRoomMessage(CBzGameManager:GetI():GetRoomHandle(),102,11,0,nil)
        self:reqChangeDesk(true)

end

-------------------------------------------平台消息---------------------------------------


--@desc 有玩家加入房间
function MJGameSceneBase:playerJoinRoom( user_info )
    
end

--@desc  有玩家离开房间
-- @param code 1.您正在游戏中，不允许退出 2.您已经离开，不需要请求再次离开
function MJGameSceneBase:playerLeaveRoom( user_info , code )
    print("GameScene:playerLeaveRoom code = " .. code)
    if code == 0 then
        print("玩家" .. user_info.dwUserID .. "离开房间")
    elseif code == 1 then
        api_show_Msg_Box("您正在游戏中，不允许退出")
    elseif code == 2 then
        api_show_Msg_Box("您已经离开，不需要请求再次离开")
    end

end


--@desc  加入桌子
--@param code 1.桌子号不属于本房间 5.桌子人数已满人 -1.其它原因
function MJGameSceneBase:playerJoinDesk( user_info , desk_info , code )

    print("有玩家加入桌子",code)

    for key, sitPlayer in pairs(self:getSitDownPlayerMap()) do
        if sitPlayer.dwUserID == self:getSelfInfo().dwUserID  then
            print("当前自己已经在坐下队列里")
            if user_info.dwUserID == self:getSelfInfo().dwUserID then
                self.GameDataManager:setSelfChair(user_info.bDeskStation)
                
                self.GameLayer.MJPlayersHeadLayer:showSitUsers()
                self.GameDataManager:resetEmojiShowPositions()
                self.GameDataManager:cacheUserInfo()
                break
            end
        end
    end

    self.GameLayer.MJPlayersHeadLayer:onPlayerJoinDesk( user_info , desk_info , code )
    
end

-- 有玩家离开桌子
--@desc  离开桌子
--@param code 1.正在游戏不准离开桌子 2.已经离开不需要再次离开
function MJGameSceneBase:playerLeaveDesk( user_info , desk_info  , code )
    print("CGameScene:playerLeaveDesk------------code = " .. code)

    if code == 0 then
        print("玩家离开桌子")
        self.GameLayer.MJPlayersHeadLayer:onPlayerLeaveDesk( user_info , desk_info , code)
    elseif code == 1 then
        api_show_Msg_Box("正在游戏不准离开桌子")
    elseif code == 2 then
        api_show_Msg_Box("已经离开不需要再次离开")
    end

end


-- 有玩家坐下
--@param code 1.位置上已有玩家 2.分数不足 3.座位不正确 4.玩家未进房间 5.不允许坐下
-- 6.房间已解散 7.被房主禁了 8.桌子索引不属于本房间 9.桌子已满人 10.玩家已经坐下 -1.其它原因
function MJGameSceneBase:playerSitDesk( user_info , desk_info , code  )
    --auto
    print("MJGameSceneBase:playerSitDesk-------------------------code=",code)
    if not user_info then
       print("user_info-------------------------error")
       return
    end

    if code == 0 then
        --如果是自己坐下就保存BasePosChair
        if self:getSelfInfo().dwUserID == tonumber(user_info.dwUserID) then
            print("------设置自己的椅子号 " .. user_info.bDeskStation)

            self.GameDataManager:setSelfChair(user_info.bDeskStation)

            if GameSceneModule:getInstance():getGameScene():getGameType() == "tz1" then
                local request = { }
                GameSceneModule:getInstance():getGameScene():getHelper():sendGameMessage(MJMessageID.ASS_GM_AGREE_GAME, request)
            end

        end

        self.GameLayer.MJPlayersHeadLayer:showSitUsers()
        self.GameDataManager:resetEmojiShowPositions()



        self.GameDataManager:cacheUserInfo()
        self.GameLayer.MJPlayersHeadLayer:onPlayerSit(user_info)

    elseif code == 1 then
        api_show_Msg_Box("位置上已有玩家")
    elseif code == 2 then
        api_show_Msg_Box("分数不足")
    elseif code == 3 then
        api_show_Msg_Box("座位不正确")
    elseif code == 4 then
        api_show_Msg_Box("玩家未进房间")
    elseif code == 5 then
        api_show_Msg_Box("不允许坐下")
    elseif code == 6 then
        api_show_Msg_Box("房间已解散")
    elseif code == 7 then
        api_show_Msg_Box("被房主禁了")
    elseif code == 8 then
        api_show_Msg_Box("桌子索引不属于本房间")
    elseif code == 9 then
        api_show_Msg_Box("桌子已满人")
    elseif code == 10 then
        api_show_Msg_Box("玩家已经坐下")
    elseif code == 50 then
        
        print("同桌游平台玩家成功坐下")
        if self.GameDataManager:getSelfChair() == tonumber(user_info.dwUserID) then
            print("------设置自己的椅子号 " .. user_info.bDeskStation)
            self.GameDataManager:setSelfChair(tonumber(user_info.bDeskStation))
            self.GameLayer.MenuManager:showSitUsers()
            self.GameDataManager:resetEmojiShowPositions()

            if GameSceneModule:getInstance():getGameScene():getGameType() == "tz1" then
                local request = { }
                GameSceneModule:getInstance():getGameScene():getHelper():sendGameMessage(MJMessageID.ASS_GM_AGREE_GAME, request)
            end
        end
        self.GameDataManager:cacheUserInfo()
        self.GameLayer.MJMenuLayer:onPlayerSit(user_info)
        self.GameLayer.MJPlayersHeadLayer:onPlayerJoinDesk( user_info , desk_info , code )
    end

end

--@desc  站起
--@param code 1.玩家正在游戏中 2.玩家不在房间  -1.其它原因
function MJGameSceneBase:playerStandUp( user_info , desk_info , code ) 
    print("GameScene:playerStandUp")

    if code ~= nil then
        print("GameScene:playerStandUp code == ",code)
    end

    self.GameLayer.MJHandCardLayer:onPlayerStandUp( user_info , desk_info , code)
    self.GameLayer.MJOutCardLayer:onPlayerStandUp( user_info , desk_info , code)

    if code == 0 then
        --Music:playEffect("res/sound/game/game_stand.mp3", false)
        self.GameLayer.MJPlayersHeadLayer:onPlayerStandUp( user_info , desk_info , code)

        local GameStation = self.GameDataManager:getGameStation()
        print("GameStation == " .. GameStation)
        if GameStation <= 1  then
            if user_info.bDeskStation >= 0 and user_info.bDeskStation < PLAYER_COUNT then
                for i,player in pairs(self.GameDataManager:getAllUserData()) do
                    if player.bDeskStation == user_info.bDeskStation then
                        self.GameDataManager:initUserInfo(i)
                        print("清除玩家缓存信息")
                    end
                end
            end
            if user_info.dwUserID == self:getSelfInfo().dwUserID then
                self:Exit_game()
            end
        end
    elseif code == 1 then
        api_show_Msg_Box("正在游戏中,站起失败")
    elseif code == 2 then
        api_show_Msg_Box("玩家不在房间")
     elseif code== 50  then
            self.GameLayer.MJPlayersHeadLayer:onPlayerStandUp( user_info , desk_info , code)
            local GameStation = self.GameDataManager:getGameStation()
            print("GameStation == " .. GameStation)
            if GameStation <= 1  then
                if user_info.bDeskStation >= 0 and user_info.bDeskStation < PLAYER_COUNT then
                    for i,player in pairs(self.GameDataManager:getAllUserData()) do
                        if player.bDeskStation == user_info.bDeskStation then
                            self.GameDataManager:initUserInfo(i)
                            print("清除玩家缓存信息")
                        end
                    end
                end
           end
    end
end

--@desc  托管
function MJGameSceneBase:playerTrusteeship( user_info , desk_info , code ) 
end


--@desc  语音           有玩家发送了语音聊天信息
--@param code 1.房间禁止发话 2.不在房间 3.非法音频ID 4.重复使用音频ID -1.其它原因
function MJGameSceneBase:playerRecorder( user_info , desk_info , recordInfo , code ) 
    if UserData:getAutoPlaySound() == 1 then
        lua_to_plat:playRecorder(recordInfo.voiceID)
    end
end

--@desc  语音           有玩家发送了语音聊天信息
--@param code 1.房间禁止发话 2.不在房间 3.非法音频ID 4.重复使用音频ID -1.其它原因
function MJGameSceneBase:playerRecorderNew( user_info , desk_info , recordInfo , code )
    if user_info.bDeskStation >= 0 and recordInfo and recordInfo.voiceID and recordInfo.duration then
        
        
        -- UserData:getAutoPlaySound() 在美术效果上设置反了， 这里统一用 0 为自动播放

        local ui_chair = self.GameDataManager:getUIChairByServerChair( user_info.bDeskStation )
        local voiceBox = VoiceBox:create( recordInfo.voiceID , recordInfo.duration , self.direction_list[ui_chair] , UserData:getAutoPlaySound() == 1 )
        voiceBox:setPosition( self.pos_chat_list[ui_chair] )
        voiceBox:setZOrder( self.chat_zorder )
        self:addChild(voiceBox)
    end
end


-- 聊天           有玩家发送了文字聊天信息
function MJGameSceneBase:playerChat( user_info , desk_info , chatInfo , code  ) 
    if user_info.bDeskStation >= 0 and chatInfo and chatInfo.content then
        local ui_chair = self.GameDataManager:getUIChairByServerChair( user_info.bDeskStation )
        if chatInfo.content.expression and self.pos_expression_list and self.pos_expression_list[ui_chair] then
            --表情
            if chatInfo.content.expression > self.expression_count then            
                chatInfo.content.expression = 1                
            end
            local path = string.format("common/expression/expression_%02d/node.csb",chatInfo.content.expression)
            local game_armature = cc.CSLoader:createNode(path)
            local action = cc.CSLoader:createTimeline(path)
            action:gotoFrameAndPlay(0,false)
            game_armature:runAction(action)
            action:setLastFrameCallFunc(function()
                game_armature:removeFromParent()
            end)
            game_armature:setScale( self.expression_scale )
            game_armature:setPosition( self.pos_expression_list[ui_chair] )
            game_armature:setZOrder( self.expression_zorder )
            self:addChild(game_armature)
        elseif chatInfo.content.QuickText and self.pos_chat_list and self.pos_chat_list[ui_chair] then
            -- 快速文本 
            local chatBox = ChatBox:create( self.quick_text_list[chatInfo.content.QuickText] , 2.5 , self.direction_list[ui_chair] , self.max_text_width)
            chatBox:setPosition( self.pos_chat_list[ui_chair] )
            chatBox:setZOrder( self.chat_zorder )
            self:addChild(chatBox)
            self:playQuickTextSound(chatInfo.content.QuickText , user_info)
        elseif chatInfo.content.Text and self.pos_chat_list and self.pos_chat_list[ui_chair] then
            -- 文字聊天
            local chatBox = ChatBox:create( chatInfo.content.Text , 2.5 , self.direction_list[ui_chair] , self.max_text_width )
            chatBox:setPosition( self.pos_chat_list[ui_chair] )
            chatBox:setZOrder( self.chat_zorder )
            self:addChild( chatBox )
        end        
    end 
end

--@desc  播放快速文字音效
--@param index 快速文字的下标
function MJGameSceneBase:playQuickTextSound( text_index , user_info )
    MJEffectManager:PlayEffect("msg_".. text_index ..".mp3",user_info.bDeskStation + 1)
end

---- 聊天           有玩家发送了文字聊天信息
--function MJGameSceneBase:playerChat( user_info , desk_info , chatInfo ) 
--end

---- 聊天           有玩家发送了表情信息
--function MJGameSceneBase:playerExpression( user_info , desk_info , expressionInfo ) 
--end


--@desc  被T           玩家被房主T出房间
--@param code 1.不能踢自己 2.不是房主不能T人 3.要踢出的用户不在房间中 4.你不在房间中不能操作 5.玩家已曾踢除 10.请求参数有误 -1.其它原因
function MJGameSceneBase:playerByMasterBoot( user_info , desk_info , code )


    if code == 0 then
        print("MJGameSceneBase 你已经被房主踢出房间")
--        api_show_Msg_Box("你已经被房主踢出房间！",function()
--            self:Exit_game()
--        end)
    elseif code == 1 then
        api_show_Msg_Box("不能踢自己！")
    elseif code == 2 then
        api_show_Msg_Box("不是房主不能T人！")
    elseif code == 3 then
        api_show_Msg_Box("要踢出的用户不在房间中！")
    elseif code == 4 then
        api_show_Msg_Box("你不在房间中不能操作！") 
    elseif code == 5 then
        api_show_Msg_Box("玩家已曾踢除！") 
    elseif code == 10 then
        api_show_Msg_Box("请求参数有误！") 
    end
    
end



--@desc  申请加分数结果返回
--@param code 0.发送成功，待房主给你回应 1.您不在房间中，不可以申请 2.房间已解散 3.错误内容类型码 10-参数错误 -1.其他原因
function MJGameSceneBase:playeApplyAddPointBack( code )
end

--@desc  给玩家加分结果返回
--@param code 1.房主身份验证不通过; 2.您不是房主没权加分 3.桌子不存在; 4.该玩家没有进入过本桌子; 10.请求参数有误; -1.其它原因
--@param add_point 本次添加的分数
function MJGameSceneBase:playeMasterAddPointBack( user_info , desk_info , code , add_point)
end

--@desc  桌子激活
--@param code 0.激活桌子成功 1.未进入房间 2.你不是房主无权限 3.桌子已曾激活 -1.其它原因
function MJGameSceneBase:deskStart()
    
end

--@desc  聊天开关改变
--@param code 1.不是房主不能操作 2.不在房间中不能操作 3.不能重复操作 10.请求参数有误 -1.其它原因
function MJGameSceneBase:changeChatSwitch( isOpen , code )
end

--@desc  进入开关改变
--@param code 1.不是房主不能操作 2.不在房间中不能操作 3.不能重复操作 10.请求参数有误 -1.其它原因
function MJGameSceneBase:changeEnterDeskSwitch( isOpen , code )
end


-- 掉线通知
-- code 1.socket错误 2.心跳超时
function MJGameSceneBase:onOffLineNotify( code , desc )
    print("----MJGameSceneBase:onOffLineNotify  code = " .. tostring(code) .. "  desc = " .. tostring(desc))
    
    if code == 1 then
        api_show_Msg_Box(" 你已经掉线 ！",function()
            self:Exit_game(5)
        end , true)
    elseif code == 2 then
        api_show_Msg_Box(" 你已经掉线 ！",function()
            self:Exit_game(5)
        end , true )
    elseif code == 3 then
        api_show_Msg_Box("你的账号在另一个移动端登录" , function()
            self:Exit_game()
        end , true )
    end

end


--@desc  桌子解散
--@param code 1.房主解散 2.对局已打满  3.桌子限期已到  4大家同意解散 
--@param user_info 有玩家请求解散时对应玩家的信息
function MJGameSceneBase:onDeskDismiss( code , user_info )
    print("MJGameSceneBase:onDeskDismiss")
    if code == 1 then
        if self.GameDataManager:getMasterUserID() == self:getSelfInfo().dwUserID then
            --房主解散成功后直接退出房间到大厅
            self:Exit_game()
        else
            api_show_Msg_Box("房主解散房间" , function()
                self:Exit_game()
            end)
        end
    elseif code == 2 then
--        api_show_Msg_Box("对局已打满",function()
--            self:Exit_game()
--        end)
    elseif code == 3 then
        api_show_Msg_Box("桌子限期已到",function()
            self:Exit_game()
        end)
    elseif code == 4 then
        api_show_Msg_Box("超过半数玩家同意解散房间",function()
            
        end)
    end
end


--@desc  桌子解散
--@param code 1.有玩家申请解散房间 2.有玩家同意解散房间 3.有玩家拒绝解散房间
--@param user_info 对应玩家的信息
function MJGameSceneBase:onDeskDismissAction( code , user_info )
    print("MJGameSceneBase:onDeskDismissAction")
    if code == 1 then
        print("有玩家申请解散房间")

        if user_info.dwUserID ~= self:getSelfInfo().dwUserID then
            
             self.dismissDeskTip = api_show_Msg_Tip("是否同意解散房间",
             function()
                --同意解散房间
                self:reqDissmisDesk(2)
                self.dismissDeskTip:stopAllActions()
                self.dismissDeskTip:setVisible(false)
                self.dismissDeskTip = nil
             end,
             function()
                --不同意解散房间
                self:reqDissmisDesk(3)
                self.dismissDeskTip = nil
             end
             ,true)

        end

    elseif code == 2 then
        print("有玩家同意解散房间")

    elseif code == 3 then
        print("有玩家拒绝解散房间")
        if self.dismissDeskTip ~= nil then
            self.dismissDeskTip:removeFromParent()
        end

        if user_info.dwUserID ~= self:getSelfInfo().dwUserID then
            api_show_Msg_Box("玩家 ".. user_info.nickName .." 拒绝解散房间")
        end
    end
end


--@desc  房主请求解散失败时的返回
--@param code 0.解散命令已成功，但是要等待游戏结束 1.未在房间里没权解散 2.不是房主无权解散 3.桌子已经解散不需要重复解散 -1.其他原因 
function MJGameSceneBase:onMasterDismissDeskFail( code  )
    print("MJGameSceneBase:onMasterDismissDeskFail" .. code)
    if code == 0 then
        --api_show_Msg_Box("解散命令已成功，但是要等待游戏结束")
    elseif code == 1 then
        api_show_Msg_Box("未在房间里没权解散")
    elseif code == 2 then
        api_show_Msg_Box("不是房主无权解散")
    elseif code == 3 then
        api_show_Msg_Box("桌子已经解散不需要重复解散")
    end
end

--@desc  玩家 申请解散 失败时的返回
--@param code 1.在这之前已经有玩家申请了
function MJGameSceneBase:onPlayerDismissDeskFail( code  )
    print("MJGameSceneBase:onPlayerDismissDeskFail")
    if code == 1 then
        api_show_Msg_Box("已经有玩家申请了解散房间")
    end
end

--@desc  玩家 同意拒绝解散房间 失败时的返回
--@param code 1.没有玩家申请解散 2.参数错误 3.已经同意了，不用再次同意
function MJGameSceneBase:onAgreePlayerDismissDeskFail( code  )
    print("MJGameSceneBase:onAgreePlayerDismissDeskFail")

end

--@desc  玩家 拒绝解散房间 失败时的返回
--@param code 1.没有玩家申请解散 2.参数错误
function MJGameSceneBase:onRefusePlayerDismissDeskFail( code  )
    print("MJGameSceneBase:onRefusePlayerDismissDeskFail")
    if code == 1 then
        api_show_Msg_Box("拒绝解散房间失败,没有玩家申请解散")
    else
        api_show_Msg_Box("拒绝解散房间失败,参数错误")
    end
end

--@desc  收到申请信息
function MJGameSceneBase:onHasApplyInfo()
end

--同桌游使用到的--用户同意
function MJGameSceneBase:UserAgreeGame(code,data)
    
end

--大厅游戏结算消息
function MJGameSceneBase:game_finish(code,data)
    
end

--用户金币变化
function MJGameSceneBase:user_score_change(code,score_info)
    
end

--用户人数过少，需要重新排队
function MJGameSceneBase:user_need_repeat_queue()
    
end

--@desc 其他玩家掉线通知
function MJGameSceneBase:other_player_offline( user_info )
    --api_show_Msg_Box("【"..user_info.nickName .."】掉线")
    print("----------其他玩家掉线通知")
    self.GameLayer.MJPlayersHeadLayer:onOtherPlayerOffline(user_info)
    
end

--去掉同ip提示功能
function MJGameSceneBase:check_ip_same( desk_max_people )
    
end

function MJGameSceneBase:play_game_video( game_data )
    if self.GameVideoLayer == nil then
        self.GameVideoLayer = GameVideoLayer:create( self )
        self.GameVideoLayer:init_event()
        self:addChild( self.GameVideoLayer )
        self.GameVideoLayer:setZOrder( 99999 )
        self.GameVideoLayer:begin_video( game_data )
    end
end

--@desc 退出游戏的接口
--@param command_id 退出到大厅的操作命令ID 
--  nil 的话不需要做任何操作
--  1.打开某桌子的战绩界面 code 为对应的房间信息
--  2.打开提示框（只有一个确定按钮的） code 为提示的内容
function MJGameSceneBase:Exit_game( command_id, code )
    
    GameSceneModule:getInstance():onDestroy()
    MJGameSceneBase.super.Exit_game(self, command_id, code )
end
--------------------------------------end------------------------------------------





------------------------------------游戏消息---------------------------------------
function MJGameSceneBase:HandleGameMessage(msgID,msgData)

    if msgID == nil then
        return
    end

    if msgID == MJMessageID.ASS_GM_GAME_STATION then
        self.GameDataManager:setIsGetGameStation(true)
    end

    if self.GameDataManager:IsGetGameStation() == false and g_server_obj~=nil and g_server_obj:get_server_type() ~= "video" then
        return 
    end

    
    --通知游戏数据管理类
    self.GameDataManager:OnRcvGameMessage(msgID,msgData)
    --通知游戏层级管理类
    self.GameLayerManager:OnRcvGameMessage(msgID,msgData)
    --通知游戏场景中的每个层
    self.GameLayer:OnRcvGameMessage(msgID,msgData)
    
end

--获取数据管理类
function MJGameSceneBase:getGameDataManager()
    return self.GameDataManager
end

--获取层级管理类
function MJGameSceneBase:getGameLayerManager()
    return self.GameLayerManager
end


--------------------------------------end------------------------------------------

return MJGameSceneBase
--endregion
