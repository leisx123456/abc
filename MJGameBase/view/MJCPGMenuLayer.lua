--region *.lua
--Date
--次类只处理 自己 吃碰杠胡过 
--此文件由[BabeLua]插件自动生成

local MJMessageID = import("..MJMessageID")
local EatChoiceLayer = import("..view.EatChoiceLayer")
local GangChoiceLayer = import("..view.GangChoiceLayer")
local ActModule = import("..module.ActModule")


local MJCPGMenuLayer = class("MJCPGMenuLayer")




local EatChoiceLayerTag = 1001
local GangChoiceLayerTag = 1002

--[[
@param visibles 吃碰杠胡过按钮可用数组
]]
function MJCPGMenuLayer:ctor(ui_project, ui_root)
    
    self.menuPos = {
        cc.p(240,257), 
        cc.p(400,257), 
        cc.p(560,257),
        cc.p(720,257),
        cc.p(880,257),
        cc.p(1040,257),
    }
    
    self.ui_project = ui_project
    self.ui_root = ui_root
    

    self.button_c = ui_project:getChildByName("Button_c")
    self.button_c:addClickEventListener(function() 
        self:chiCallback()
    end)
    
    self.button_p = ui_project:getChildByName("Button_p")
    self.button_p:addClickEventListener(function() 
        self:pengCallback()
    end)

    self.button_g = ui_project:getChildByName("Button_g")
    
    self.button_g:addClickEventListener(function() 
       self:gangCallback()
    end)

    self.button_h = ui_project:getChildByName("Button_h")
    self.button_h:addClickEventListener(function() 
       self:huCallback()
    end)

    self.button_pass = ui_project:getChildByName("Button_pass")
    self.button_pass:addClickEventListener(function() 
        self:passCallback()
    end)

    local h_node = cc.CSLoader:createNode("animate/hu/hu01.csb")
    local h_action = cc.CSLoader:createTimeline("animate/hu/hu01.csb")
    h_node:runAction(h_action)
    h_action:gotoFrameAndPlay(0)
    h_node:setPosition(self.menuPos[4])
    self.ui_root:addChild(h_node)
    self.h_node = h_node

    local g_node = cc.CSLoader:createNode("animate/gang/gang01.csb")
    local g_action = cc.CSLoader:createTimeline("animate/gang/gang01.csb")
    g_node:runAction(g_action)
    g_action:gotoFrameAndPlay(0)
    g_node:setPosition(self.menuPos[3])
    self.ui_root:addChild(g_node)
    self.g_node = g_node

    local p_node = cc.CSLoader:createNode("animate/peng/peng01.csb")
    local p_action = cc.CSLoader:createTimeline("animate/peng/peng01.csb")
    p_node:runAction(p_action)
    p_action:gotoFrameAndPlay(0)
    p_node:setPosition(self.menuPos[2])
    self.ui_root:addChild(p_node)
    self.p_node = p_node

    local c_node = cc.CSLoader:createNode("animate/chl/chl01.csb")
    local c_action = cc.CSLoader:createTimeline("animate/chl/chl01.csb")
    c_node:runAction(c_action)
    c_action:gotoFrameAndPlay(0)
    c_node:setPosition(self.menuPos[1])
    self.ui_root:addChild(c_node)
    self.c_node = c_node    

end


function MJCPGMenuLayer:chiCallback()
    print("chi")

    --如果只有一种吃法就直接吃 否则就显示吃的选择界面
    local eatFlag = self.resp_json.usChiFlags
    print("eatFlag = " .. eatFlag)
        
    if eatFlag == ActModule.EatType.usT 
        or eatFlag == ActModule.EatType.usZ
        or eatFlag == ActModule.EatType.usW  then
        --直接发送吃牌请求
        local request = {}
        request.ActFlag = ActModule.MJActFlag.Chi
        request.EatFlag = eatFlag
        GameSceneModule:getInstance():getGameScene():getHelper():sendGameMessage(MJMessageID.ASS_ACT_REQ,request)

    elseif eatFlag == ActModule.EatType.usTZ 
        or eatFlag == ActModule.EatType.usZW 
        or eatFlag == ActModule.EatType.usTW then
        --显示吃牌选择界面
        print("show 2 eatchoices")
        local eatChoiceLayer = EatChoiceLayer:create(self.ui_root,2,self.resp_json)
        self.ui_root:addChild(eatChoiceLayer,1,EatChoiceLayerTag)
    else
        print("show 3 eatchoices")
        local eatChoiceLayer =  EatChoiceLayer:create(self.ui_root,3,self.resp_json)
        self.ui_root:addChild(eatChoiceLayer,1,EatChoiceLayerTag)
    end

    self:ui_close()
end

function MJCPGMenuLayer:pengCallback()
    print("peng")
    local request = {}
    request.ActFlag = ActModule.MJActFlag.Peng
    GameSceneModule:getInstance():getGameScene():getHelper():sendGameMessage(MJMessageID.ASS_ACT_REQ,request)
    self:ui_close()
end

function MJCPGMenuLayer:gangCallback()
    print("gang")
    if self.resp_json.iGangNums == 1 then
        local request = {}
        request.ActFlag = ActModule.MJActFlag.Gang
        request.nSelIndex = 0 --此值为杠的选择
        GameSceneModule:getInstance():getGameScene():getHelper():sendGameMessage(MJMessageID.ASS_ACT_REQ,request)
    elseif  self.resp_json.iGangNums == 2 then 
        local  gangChoiceLayer = GangChoiceLayer:create(self.ui_root,2,self.resp_json)
        self.ui_root:addChild(gangChoiceLayer,1,GangChoiceLayerTag)

    elseif self.resp_json.iGangNums == 3 then 
        local  gangChoiceLayer = GangChoiceLayer:create(self.ui_root,3,self.resp_json)
        self.ui_root:addChild(gangChoiceLayer,1,GangChoiceLayerTag)
    end

    self:ui_close()
end

function MJCPGMenuLayer:huCallback()
    print("hu")
    local request = {}
    request.ActFlag = ActModule.MJActFlag.Hu
    GameSceneModule:getInstance():getGameScene():getHelper():sendGameMessage(MJMessageID.ASS_ACT_REQ,request)
    self:ui_close()
end

function MJCPGMenuLayer:passCallback()
    print("pass")

    local iscanhandoutcard = GameSceneModule:getInstance():getGameScene():getGameDataManager():getIsCanHandoutCard()

    --如果有两个玩家同时有动作  动作优先级低的人发送过的请求  会有两个人持有令牌
    if iscanhandoutcard == false then
        local request = {}
        request.ActFlag = ActModule.MJActFlag.Guo
        GameSceneModule:getInstance():getGameScene():getHelper():sendGameMessage(MJMessageID.ASS_ACT_REQ,request)
    end

    self:ui_close()
end


--显示出牌选择界面
function MJCPGMenuLayer:ui_open()
    self.ui_project:setVisible(true)
end

--收到可以执行某些动作 resp_json 动作的必要信息
function MJCPGMenuLayer:onActNotify(resp_json)
    print("MJCPGMenuLayer:onActNotify")
    
    if resp_json == nil then
        return
    end


    if resp_json.usFlags == nil then
        return
    end
    self.resp_json = resp_json

    local usFlags = resp_json.usFlags   --是否有动作

    if usFlags>0 then
        local buttons = {self.button_pass,self.button_c,self.button_p,self.button_g,self.button_h}
        local nodes = {self.button_pass,self.c_node,self.p_node,self.g_node,self.h_node}
        local visibles = {true,resp_json.isCanC,resp_json.isCanP,resp_json.isCanG,resp_json.isCanH}
          
        self.button_pass:setVisible(true)
        local index = 4
        local show_index = 5
        for i = 1,5 do
            if visibles[i] then
                buttons[i]:setPosition(self.menuPos[show_index])
                nodes[i]:setPosition(self.menuPos[show_index])
                show_index = show_index - 1
            end
            buttons[i]:setVisible(visibles[i])
            nodes[i]:setVisible(visibles[i])
        end

    end
    
    self:ui_open()
end



--重新显示吃碰杠菜单
function MJCPGMenuLayer:reShowMenuLayer()
    self:onActNotify(self.resp_json)
end

function MJCPGMenuLayer:onActInfo(resp_json)
    self:ui_close()
    


    local eatChoiceLayer = self.ui_root:getChildByTag(EatChoiceLayerTag)
    if eatChoiceLayer~= nil then
        eatChoiceLayer:removeFromParent()
    end

    local gangChoiceLayer = self.ui_root:getChildByTag(GangChoiceLayerTag)
    if gangChoiceLayer~= nil then
        gangChoiceLayer:removeFromParent()
    end
end

function MJCPGMenuLayer:onGetToken(resp_json)
    self:ui_close()
end

--收到当前游戏状态信息
function MJCPGMenuLayer:onGameStation(resp_json)
    print("MJCPGMenuLayer:onGameStation")
    if resp_json.GSID==24 then          --跳牌状态
        
    elseif resp_json.GSID==26 then      --玩家打牌状态
        --当前是否有动作
        if resp_json.actResp.usFlags >0 and resp_json.actResp.usFlags%2 == 0 then
            self:onActNotify(resp_json.actResp)
        end
    end

    print("MJCPGMenuLayer:onGameStation end")

end



function MJCPGMenuLayer:ui_close()
    self.ui_project:setVisible(false)
    self.c_node:setVisible(false)
    self.p_node:setVisible(false)
    self.g_node:setVisible(false)
    self.h_node:setVisible(false)
end




function MJCPGMenuLayer:OnRcvGameMessage(msgID,msgData)
    if msgID == MJMessageID.ASS_GAME_BEGIN then
        

    elseif msgID == MJMessageID.ASS_MAKE_NT_INFO then
        
    elseif msgID == MJMessageID.ASS_FETCH_HANDCARDS then
        
    elseif msgID == MJMessageID.ASS_TOKEN then
        self:onGetToken(msgData)
    elseif msgID == MJMessageID.ASS_TIAO_CARDS then
        
    elseif msgID == MJMessageID.ASS_OUT_CARD_INFO then
        
    elseif msgID == MJMessageID.ASS_ACT_NOTIFY then
        self:onActNotify(msgData)
    elseif msgID == MJMessageID.ASS_ACT_INFO then
        self:onActInfo(msgData)
    elseif msgID == MJMessageID.ASS_RESULT then
        
    elseif msgID == MJMessageID.ASS_PIAO_NOTIFY then
        
    elseif msgID == MJMessageID.ASS_PIAO_INFO then
        
    elseif msgID == MJMessageID.ASS_NIAOCARDS_INFO then
        
    elseif msgID == MJMessageID.ASS_PAIJU_INFO then
        
    elseif msgID == MJMessageID.ASS_GM_AGREE_GAME then

    elseif msgID == MJMessageID.ASS_GM_GAME_STATION then
        self:onGameStation(msgData)
    end


end


return MJCPGMenuLayer


--endregion
