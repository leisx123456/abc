--region *.lua
--Date
--麻将游戏最底层的UI管理类
--此文件由[BabeLua]插件自动生成

local MJMessageID = import("..MJMessageID")
local GSModule = import("..."..GAME_ID..".module.GSModule")
local ActModule = import("..module.ActModule")


local MJGameVideoLayer = class("MJGameVideoLayer" , function()
    return cc.CSLoader:createNode("ui_csb/GameVideoLayer.csb")
end)

function MJGameVideoLayer:ctor( game_scene )
    self.game_scene = game_scene


    local Panel_video = self:getChildByName("Panel_video")
    --返回按钮
    local Button_back = Panel_video:getChildByName("Button_back")
    Button_back:addClickEventListener( function()
         self.game_scene:Exit_game( 4 , self.game_scene:getDeskInfo().roomID )
    end)
    local Image_menu = Panel_video:getChildByName("Image_menu")

    --快退按钮
    local Button_backward = Image_menu:getChildByName("Button_backward")
        Button_backward:addClickEventListener( function()
            --Music:playEffect_click()
            print("Button_backward clicked")
            if  self.m_nextIndex - 1 > 0 then
                self.m_nextIndex = self.m_nextIndex - 1
            end

            if self.is_pause then
                self:play_data_atonce()
            end

        end)
    
    self.is_pause = false
    self.l_is_pase_data = false
    self.l_is_update_scene = false
    self.event_call = {}
    --暂停按钮
    local Button_pause = Image_menu:getChildByName("Button_pause")
    self.Button_pause = Button_pause
        Button_pause:addClickEventListener( function()
            --Music:playEffect_click()
            if self.is_pause == false then
                self.is_pause = true
                self.Button_pause:loadTextures("ui_res/video/jixu.png","ui_res/video/jixu.png","")


            else
                self.is_pause = false
                self.Button_pause:loadTextures("ui_res/video/pause.png","ui_res/video/pause.png","")
                performWithDelay( self , function()
                    self:play_data()
                end , 0.5)
            end
        end)


    --快进那妞
    local Button_advance = Image_menu:getChildByName("Button_advance")
        Button_advance:addClickEventListener( function()
            --Music:playEffect_click()

            if  self.m_nextIndex + 1 < #self.game_video_data then
                self.m_nextIndex = self.m_nextIndex + 1
            end

            if self.is_pause then
                self:play_data_atonce()
            end
        end)

    --重播按钮
    --若点击重播后出现之前碰杠牌留在新开始的界面上,是因为play_data()中的clone(self.game_data[self.m_index])出现问题
    --导致无法进行深拷贝,在子类进行重写MJGSModule的构造即可,可参考监利红中的svn的12660版本
    local Button_return = Image_menu:getChildByName("Button_return")
        Button_return:addClickEventListener( function()
            --Music:playEffect_click()
            
            print(" ----------录像-----  重新播放 ")
            if self.m_nextIndex ~= 1 then
                self.m_nextIndex = 1
                self.is_pause = true

                local agree_data = {}
                agree_data.ID = MJMessageID.ASS_GM_AGREE_GAME
                agree_data.data = {
                    chair = GameSceneModule:getInstance():getGameScene():getGameDataManager():getSelfChair()
                }
                self:l_agree_game(MJMessageID.ASS_GM_AGREE_GAME,agree_data)
                performWithDelay( self , function()
                    self.Button_pause:loadTextures("video/pause.png","video/pause.png","")
                    --self.game_scene:tryRemoveGameResultLayer()
                    local layerManager = GameSceneModule:getInstance():getGameScene():getGameLayerManager()
                    layerManager:hideGameResultLayer()
                    self.is_pause = false
                    self:play_data()
                    end , 1)
            end
            

        end)

    self.Text_press = Panel_video:getChildByName("Text_press")

    self.Panel_player = {}
    self.Button_chi = {}
    self.Button_peng = {}
    self.Button_gang = {}
    self.Button_bu = {}
    self.Button_hu = {}
    self.Button_pass = {}

    for i =1 , 4 do
        self.Panel_player[i] = Panel_video:getChildByName("Panel_player_"..i)
        self.Panel_player[i]:setVisible( false )
        self.Button_chi[i] = self.Panel_player[i]:getChildByName("Button_chi")
        self.Button_peng[i] = self.Panel_player[i]:getChildByName("Button_peng")
        self.Button_gang[i] = self.Panel_player[i]:getChildByName("Button_gang")
        self.Button_bu[i] = self.Panel_player[i]:getChildByName("Button_bu")
        self.Button_hu[i] = self.Panel_player[i]:getChildByName("Button_hu")
        self.Button_pass[i] = self.Panel_player[i]:getChildByName("Button_pass")
    end

    self:init_event()
end

function MJGameVideoLayer:hideAction( )
    for i = 1 , 4 do
        self.Panel_player[i]:setVisible( false )
    end
end


function MJGameVideoLayer:showSelfAction( chair , action )
    local i = GameSceneModule:getInstance():getGameScene():getGameDataManager():getUIChairByServerChair(chair)
    self.Panel_player[i]:setVisible( true )
    self.Button_chi[i]:setBright( action.isCanC  )
    self.Button_peng[i]:setBright( action.isCanP )
    self.Button_gang[i]:setBright( action.isCanG )
    self.Button_bu[i]:setBright( false )
    self.Button_hu[i]:setBright( action.isCanH )
    self.Button_pass[i]:setBright( true )

    if action.isCanG and action.nGType then
        for j = 1 , #action.nGType do
            if action.nGType[j] == 2 then
                -- 补杠
                self.Button_bu[i]:setBright( true )
            elseif action.nGType[j] == 3 then
                -- 暗杠)
                self.Button_gang[i]:setBright( true )
            end
        end        
    end
    self.Button_hu[i]:setBright( action.isCanH )
end

function MJGameVideoLayer:showOtherAction( chair , action )
    local i = GameSceneModule:getInstance():getGameScene():getGameDataManager():getUIChairByServerChair(chair)
    local pass = false
    local function sss( val )
        if val == true then
            pass = true
            return true
        end
        return false
    end

    self.Panel_player[i]:setVisible( true )
    self.Button_chi[i]:setBright( sss( action.isCanC )  )
    self.Button_peng[i]:setBright( sss( action.isCanP ) )
    self.Button_gang[i]:setBright( sss( action.isCanG ) )
    self.Button_bu[i]:setBright( false )
    self.Button_hu[i]:setBright( sss( action.isCanH ) )
    self.Button_pass[i]:setBright( pass )
end

function MJGameVideoLayer:begin_video( game_data )
    luaLog("开始播放游戏录像数据")
    self.m_index = 0                                -- 当前播放的下标
    self.m_nextIndex = 1                            -- 下一帧所需要播放的下标（受快进与快退的影响）
    self.game_video_data = clone( game_data )       -- 录像数据的步骤帧
    self.game_data = {} 
    --直接创建一个具体麻将的断线重连数据module
    self.game_data[0] = GSModule:create()

                                -- 所有的数据帧
    self.Text_press:setString( "1/"..#self.game_video_data)
    performWithDelay( self , function ()
            self:play_data()
        end, 0.5 )
end




--@desc  移除cards 数组里 的 card 值
--@param count 需要移除的个数 默认为1
local function l_removeCard( cards , card , count)
    local ret = {}
    local org_card = clone(card)
    count = count or 1
    for i=1 , #cards do
        if cards[i] == card then
            for j = count-1 , 0 , -1 do
                if cards[ i + j ] == card then
                    table.remove( cards , i + j )
                end
            end
            return nil
        end
    end
end

--@desc 从小到大排序
local function sort( cards )
    table.sort( cards , function(a,b)
        return a < b
    end)
end

--@desc 更新场景
function MJGameVideoLayer:update_scene( id , data )
    if self.l_is_update_scene then
        luaLog("播放游戏数据：180_"..id)
        --dump( data , "---- data ----" , 10 )
        self.game_scene:HandleGameMessage(id,data)
    end
end

--@desc 同意游戏
function MJGameVideoLayer:l_agree_game(game_data_cmd , cmd_data )
    self:update_scene( cmd_data.ID ,cmd_data.data)
end


--@desc 游戏开始
function MJGameVideoLayer:l_game_begin( game_data_cmd , cmd_data )
    print(" ----------录像-----  游戏开始 ")
    self:update_scene( cmd_data.ID , cmd_data.data )
end

--@desc 选择庄家
function MJGameVideoLayer:l_select_banker( game_data_cmd , cmd_data )
    print(" ----------录像-----  选庄 ")
    if self.l_is_pase_data then
        game_data_cmd.NTUser        = cmd_data.data.NtUser
        game_data_cmd.RoundTimes    = cmd_data.data.RoundTimes
    end
    self:update_scene( cmd_data.ID , cmd_data.data )
    return 3
end



--@desc 发牌
function MJGameVideoLayer:l_send_card( game_data_cmd , cmd_data )
    print(" ----------录像-----  发牌 ")
    if self.l_is_pase_data then
        game_data_cmd.HandCards = clone(cmd_data.data.Cards)
        for i = 1 , PLAYER_COUNT do
            sort(game_data_cmd.HandCards[i])
            game_data_cmd.HandNums[i] = #game_data_cmd.HandCards[i]
        end            
        game_data_cmd.WallLeft  = clone(cmd_data.data.WallLeft)
    end
    self:update_scene( cmd_data.ID , cmd_data.data )
end

--@desc 庄家跳牌
function MJGameVideoLayer:l_tiao_card( game_data_cmd , cmd_data )
    print(" ----------录像-----  专家跳牌 ")
    if self.l_is_pase_data then
        game_data_cmd.TokenUser = clone(cmd_data.data.NTUser)
        game_data_cmd.WallLeft = game_data_cmd.WallLeft -1
        table.insert( game_data_cmd.HandCards[game_data_cmd.TokenUser+1] , cmd_data.data.tiaoFetch )
        sort( game_data_cmd.HandCards[game_data_cmd.TokenUser+1] )
        game_data_cmd.HandNums[game_data_cmd.TokenUser+1] = #game_data_cmd.HandCards[game_data_cmd.TokenUser+1]
    end
    self:update_scene( cmd_data.ID , cmd_data.data )
end

--@desc 摸牌
function MJGameVideoLayer:l_mo_card( game_data_cmd , cmd_data )
    
    if cmd_data.data.moInfo.TokenbFetch  == false then
        return 0
    end
        print(" ----------录像-----  摸牌 ")
    if self.l_is_pase_data and  cmd_data.data.moInfo.TokenFetcherVal then
        game_data_cmd.TokenUser = clone(cmd_data.data.moInfo.TokenbyUser)
        game_data_cmd.WallLeft = game_data_cmd.WallLeft -1
        table.insert( game_data_cmd.HandCards[game_data_cmd.TokenUser+1] , cmd_data.data.moInfo.TokenFetcherVal )
        sort( game_data_cmd.HandCards[game_data_cmd.TokenUser+1] )
        game_data_cmd.HandNums[game_data_cmd.TokenUser+1] = #game_data_cmd.HandCards[game_data_cmd.TokenUser+1]
    end
    self:update_scene( cmd_data.ID , cmd_data.data.moInfo )
    if cmd_data.data.action then
        self:showSelfAction( cmd_data.data.moInfo.TokenbyUser , cmd_data.data.action )         
    end  
end

--@desc 出牌
function MJGameVideoLayer:l_out_card( game_data_cmd , cmd_data )
    print(" ----------录像-----  出牌 ")
    local index = cmd_data.data.OutInfo.outCardUser+1
    if self.l_is_pase_data then
        l_removeCard(  game_data_cmd.HandCards[ index ] , cmd_data.data.OutInfo.outCardValue)
        game_data_cmd.HandNums[ index ] = #game_data_cmd.HandCards[ index ]
        table.insert( game_data_cmd.OutedCards[ index ] , cmd_data.data.OutInfo.outCardValue )
        game_data_cmd.OutedNums[ index ] = #game_data_cmd.OutedCards[ index ]
        game_data_cmd.outCardNum = cmd_data.data.OutInfo.outCardNum

    end
    cmd_data.data.OutInfo.HandCards = clone(game_data_cmd.HandCards[ index ])
    cmd_data.data.OutInfo.handCardNums = #game_data_cmd.HandCards[ index ]
    
    self:update_scene( cmd_data.ID , cmd_data.data.OutInfo )
    
    -- 某玩家出牌后，其他玩家可以进行吃碰杠胡等操作
    if cmd_data.data.action and self.l_is_update_scene then
        for i = 1 , #cmd_data.data.action do
            if cmd_data.data.action[i] ~= nil then
                self:showOtherAction( cmd_data.data.action[i].chair , cmd_data.data.action[i].data )
            end
        end            
    end       
end

--@desc 玩家操作 吃碰杠
function MJGameVideoLayer:l_user_action( game_data_cmd , cmd_data )
    print(" ----------录像-----  吃碰杠 ")
    local index = nil
    if cmd_data.data.byToUser then
        index = cmd_data.data.byToUser + 1
    end
    print("MJGameVideoLayer:l_user_action========>>to",index,",flag=",cmd_data.data.usActFlag)
    print("before===================")
    dump(game_data_cmd.HandCards[ index ])
    if self.l_is_pase_data then
        if cmd_data.data.usActFlag == 4 then
            -- 吃
            for i = 1 , 3 do
                if i ~= cmd_data.data.chiFlag then
                    l_removeCard( game_data_cmd.HandCards[index] , cmd_data.data.cpgCards[i]  , 1 )
                end
            end
        elseif cmd_data.data.usActFlag == 8 then
            -- 碰
            l_removeCard( game_data_cmd.HandCards[index] , cmd_data.data.cpgCards[1]  , 2 )
        elseif cmd_data.data.usActFlag == 16 then
            -- 杠
            if cmd_data.data.gangType == 3 then
                -- 明杠
                l_removeCard( game_data_cmd.HandCards[index] , cmd_data.data.cpgCards[1] , 3 )
            elseif cmd_data.data.gangType == 5 then
                -- 暗杠
                l_removeCard( game_data_cmd.HandCards[index] , cmd_data.data.cpgCards[1] , 4 )
            elseif cmd_data.data.gangType == 4 then
                -- 补杠
                l_removeCard( game_data_cmd.HandCards[index] , cmd_data.data.cpgCards[1] , 1 )
            end
        elseif cmd_data.data.usActFlag == ActModule.MJActFlag.Hu then
            
            --有玩家胡牌
            for i,v in pairs(cmd_data.data.byToUsers) do
                 --有玩家放炮
                if cmd_data.data.byFromUser ~= v then
                    table.insert( game_data_cmd.HandCards[v+1] , game_data_cmd.OutedCards[cmd_data.data.byFromUser+1][#game_data_cmd.OutedCards[cmd_data.data.byFromUser+1]])
                end
            end
            --table.insert( game_data_cmd.HandCards[index] , game_data_cmd.OutedCards[cmd_data.data.byFromUser+1][#game_data_cmd.OutedCards[cmd_data.data.byFromUser+1]])
                
        end

        if cmd_data.data.byToUser then
            if cmd_data.data.byFromUser ~= cmd_data.data.byToUser then
                -- 处理获得了别人打的牌
                table.remove(  game_data_cmd.OutedCards[cmd_data.data.byFromUser+1] , #game_data_cmd.OutedCards[cmd_data.data.byFromUser+1] )
                game_data_cmd.OutedNums[cmd_data.data.byFromUser+1] = #game_data_cmd.OutedCards[cmd_data.data.byFromUser+1]
            end
        else
            -- 处理获得了别人打的牌
            if cmd_data.data.byFromUser ~= cmd_data.data.byToUsers[1] then
                table.remove(  game_data_cmd.OutedCards[cmd_data.data.byFromUser+1] , #game_data_cmd.OutedCards[cmd_data.data.byFromUser+1] )
                game_data_cmd.OutedNums[cmd_data.data.byFromUser+1] = #game_data_cmd.OutedCards[cmd_data.data.byFromUser+1]
            end
        end
        
        
        if cmd_data.data.usActFlag == ActModule.MJActFlag.Hu then
            for i,v in pairs(cmd_data.data.byToUsers) do
                game_data_cmd.HandNums[v+1] = #game_data_cmd.HandCards[v+1]
            end
        else
            game_data_cmd.HandNums[index] = #game_data_cmd.HandCards[index]
            if cmd_data.data.nIdx then
                local usFlagsValue = nil
                if cmd_data.data.usActFlag == 16 then
                    if cmd_data.data.gangType == 5 then --暗杠
                        usFlagsValue = 5
                    else
                        usFlagsValue = 3
                    end
                end
                game_data_cmd.CPGRecords[index][cmd_data.data.nIdx+1] = {
                    usFlags = usFlagsValue,
                    val = cmd_data.data.cpgCards,
                    --from=cmd_data.data.from,
                }
            
                game_data_cmd.CPGRecords[index][cmd_data.data.nIdx+1].val[4] = game_data_cmd.CPGRecords[index][cmd_data.data.nIdx+1].val[4] or 0

                game_data_cmd.CGPNums[index] = #game_data_cmd.CPGRecords[index]
            end  
        end
          
    end

    if cmd_data.data.usActFlag == ActModule.MJActFlag.Hu then
        cmd_data.data.HandCards = clone(game_data_cmd.HandCards)
        for i,v in pairs(cmd_data.data.byToUsers) do
            cmd_data.data.iHandNums = #game_data_cmd.HandCards[ v+1]
        end
        
    else
        cmd_data.data.HandCards = clone(game_data_cmd.HandCards[ index ])
        cmd_data.data.iHandNums = #game_data_cmd.HandCards[ index ]    
    end

    
    
    dump(cmd_data.data.HandCards)
    
    self:update_scene( cmd_data.ID , cmd_data.data )
end


--@desc 结算
function MJGameVideoLayer:l_game_resoult( game_data_cmd , cmd_data )
    print(" ----------录像-----  结算 ")
    self:update_scene( cmd_data.ID , cmd_data.data.ResultInfo )
end


function MJGameVideoLayer:init_event()
    self.event_call[MJMessageID.ASS_GAME_BEGIN] = handler(self , self.l_game_begin )            -- 游戏开始
    self.event_call[MJMessageID.ASS_MAKE_NT_INFO] = handler(self , self.l_select_banker )       -- 选庄
    self.event_call[MJMessageID.ASS_FETCH_HANDCARDS] = handler(self , self.l_send_card )        -- 发牌
    self.event_call[MJMessageID.ASS_TIAO_CARDS] = handler(self , self.l_tiao_card )             -- 庄家跳牌
    self.event_call[MJMessageID.ASS_TOKEN] = handler(self , self.l_mo_card )                    -- 摸牌
    self.event_call[MJMessageID.ASS_OUT_CARD_INFO] = handler(self , self.l_out_card )           -- 出牌
    self.event_call[MJMessageID.ASS_ACT_INFO] = handler(self , self.l_user_action )             -- 操作（吃碰杠）
    self.event_call[MJMessageID.ASS_RESULT] = handler(self , self.l_game_resoult )              -- 结算
end

--@desc     设置断线重连数据
--@param    cmd_data   当前的操作命令
--@param    is_update_scene 是否需要刷新场景数据
function MJGameVideoLayer:set_game_data(  video_index , is_update_scene )
    -- 是否需要解析数据 （已经已经解析过了就不需要再解析了）
    self.l_is_update_scene = is_update_scene
    if self.game_data[video_index] == nil then
        self.game_data[video_index] = clone( self.game_data[ video_index - 1 ] )
        self.l_is_pase_data = true
    else
        self.l_is_pase_data = false
    end
    self:hideAction()
    local ret = 0.7
    if self.event_call[self.game_video_data[video_index].ID] then
        local event_ret = self.event_call[self.game_video_data[video_index].ID]( self.game_data[video_index] , clone(self.game_video_data[video_index]) )
        if event_ret then
            ret = event_ret
        end
    end
    return ret
end

--@desc  播放数据
--@param space 操作数据 0. 暂停在这一帧 , 大于0 快进的步数 , 小于0 快退的步数
--@param perGameData  之前的数据
function MJGameVideoLayer:play_data()
    if self.is_pause == true then
        -- 暂停了
        self.is_wait_paly = true
        return nil
    end

    if #self.game_video_data < self.m_nextIndex then
        api_show_tips("录像已播完")
        self.is_pause = true
        self.Button_pause:loadTextures("video/jixu.png","video/jixu.png","")
        return nil
    end

    local delay = 1.0

    if self.m_nextIndex > self.m_index then
        -- 向终点播放
        if self.m_nextIndex > self.m_index + 1 then
            for index = self.m_index+1 , self.m_nextIndex-1 do
                --执行快进的命令
                self:set_game_data( index )
            end
            -- 发送断线重连数据
            self.m_nextIndex = self.m_nextIndex - 1
            local resp_json = clone(self.game_data[self.m_nextIndex])
            resp_json.chair = self.game_scene:getSelfInfo().bDeskStation
            resp_json.GSID = 26
            dump( resp_json , "断线重连数据" , 10 )
            self.game_scene:HandleGameMessage(MJMessageID.ASS_GM_GAME_STATION , resp_json)
            self.m_index = self.m_nextIndex
            delay = 0.2
        else
            self.m_index = self.m_nextIndex
            delay = self:set_game_data( self.m_nextIndex , true )       
            self.Text_press:setString( self.m_nextIndex.."/"..#self.game_video_data)
        end
        
    else
        -- 向起点回退
        self.m_index = self.m_nextIndex

        -- 发送断线重连数据
       local resp_json = clone(self.game_data[self.m_index])
       resp_json.chair = self.game_scene:getSelfInfo().bDeskStation
       resp_json.GSID = 26
       self.game_scene:HandleGameMessage(MJMessageID.ASS_GM_GAME_STATION , resp_json)
        -- 执行当前这一帧的录像数据
        --快退的时候不用再处理出牌消息了 
        if MJMessageID.ASS_OUT_CARD_INFO ~= self.game_video_data[self.m_index].ID 
        and MJMessageID.ASS_RESULT ~= self.game_video_data[self.m_index].ID then
            delay = self:set_game_data( self.m_index , true )
        end

        self.Text_press:setString( self.m_nextIndex.."/"..#self.game_video_data)
    end
    self.m_nextIndex = self.m_nextIndex + 1
    performWithDelay( self , function()
        self:play_data()
    end , delay)
end


--立刻播放某一帧的录像
function MJGameVideoLayer:play_data_atonce()
   
    if #self.game_video_data < self.m_nextIndex then
        api_show_tips("录像已播完")
        self.is_pause = true
        self.Button_pause:loadTextures("video/jixu.png","video/jixu.png","")
        return nil
    end

    --self.game_scene:tryRemoveGameResultLayer()
    local layerManager = GameSceneModule:getInstance():getGameScene():getGameLayerManager()
    layerManager:hideGameResultLayer()
    if self.m_nextIndex > self.m_index then
        -- 向终点播放
        if self.m_nextIndex > self.m_index + 1 then
            for index = self.m_index+1 , self.m_nextIndex-1 do
                --执行快进的命令
                self:set_game_data( index , true)
            end
            -- 发送断线重连数据
            self.m_nextIndex = self.m_nextIndex - 1
            local resp_json = clone(self.game_data[self.m_nextIndex])
            resp_json.chair = self.game_scene:getSelfInfo().bDeskStation
            resp_json.GSID = 26
            dump( resp_json , "断线重连数据" , 10 )
            self.game_scene:HandleGameMessage(MJMessageID.ASS_GM_GAME_STATION , resp_json)
            self.m_index = self.m_nextIndex
           
            self.Text_press:setString( self.m_nextIndex.."/"..#self.game_video_data)
        end
        
    else
        -- 向起点回退
        self.m_index = self.m_nextIndex

        -- 发送断线重连数据
       local resp_json = clone(self.game_data[self.m_index])
       resp_json.chair = self.game_scene:getSelfInfo().bDeskStation
       resp_json.GSID = 26
       self.game_scene:HandleGameMessage(MJMessageID.ASS_GM_GAME_STATION , resp_json)
       if MJMessageID.ASS_OUT_CARD_INFO ~= self.game_video_data[self.m_index].ID 
        and MJMessageID.ASS_RESULT ~= self.game_video_data[self.m_index].ID then
            self:set_game_data( self.m_index , true )
       end

       -- 执行当前这一帧的录像数据
        self.Text_press:setString( self.m_nextIndex.."/"..#self.game_video_data)
    end
   
end

return MJGameVideoLayer

--endregion
