--region *.lua
--Date
--此文件由[BabeLua]插件自动生成
--麻将层级管理类  用于添加删除游戏自定义层级


local GameResultLayer = import("..."..GAME_ID..".view.GameResultLayer")

local PlayersScoreLayer = import("..view.PlayersScoreLayer")
local PlayerInfoLayer = import("..view.PlayerInfoLayer")
local CheckTingLayer = import("..view.CheckTingLayer")
local MessageID = import("..MJMessageID")

local MJGameLayerManager = class("MJGameLayerManager")

local GameResultLayerTag = 1001
local PlayerInfoLayerTag = 1002
local CheckTingLayerTag  = 1003
local PaiJuInfoLayerTag  = 1004


function MJGameLayerManager:ctor()
    
    self.m_PlayerInfoLayer = nil        --玩家信息层
    self.m_CheckTingLayer = nil         --查听信息层
    self.m_PaiJuInfoLayer = nil         --总结算层
end



--显示玩家的信息层
function MJGameLayerManager:showPlayerInfoLayer(ui_root,userInfo,position)
    if self.m_PlayerInfoLayer then
        self:hidePlayerInfoLayer()
    end

    if userInfo == nil then
        return
    end

    self.m_PlayerInfoLayer = PlayerInfoLayer:create(userInfo,self)
    self.m_PlayerInfoLayer:setPosition(position)
    ui_root:addChild(self.m_PlayerInfoLayer,1)

end

--隐藏玩家的信息层
function MJGameLayerManager:hidePlayerInfoLayer()

    if self.m_PlayerInfoLayer then
        self.m_PlayerInfoLayer:removeFromParent()
        self.m_PlayerInfoLayer = nil
    end
end

function MJGameLayerManager:showCheckTingLayer(ui_root,tingCards,showCards)
    if self.m_CheckTingLayer then
        self:hideCheckTingLayer()
    end
    if tingCards == nil or #tingCards == 0 then
        return
    end

    self.m_CheckTingLayer = CheckTingLayer:create(self,tingCards,showCards)
    ui_root:addChild(self.m_CheckTingLayer)
end

function MJGameLayerManager:hideCheckTingLayer()
    if self.m_CheckTingLayer then
        self.m_CheckTingLayer:removeFromParent()
        self.m_CheckTingLayer = nil
    end
end



--显示单局结算层
function MJGameLayerManager:showGameResultLayer(msgData,isShowFinishButton)
    
    local gameScene = GameSceneModule:getInstance():getGameScene()
    local resultLayer = gameScene:getChildByTag(GameResultLayerTag)
    if resultLayer then
        resultLayer:removeFromParent()
    end
    
    local GameResultLayer = GameResultLayer:create(gameScene,msgData,isShowFinishButton,gameScene:getGameDataManager(),self)
    gameScene:addChild(GameResultLayer,10000,GameResultLayerTag)
    
end

--隐藏单局结算层
function MJGameLayerManager:hideGameResultLayer()

    local resultLayer = GameSceneModule:getInstance():getGameScene():getChildByTag(GameResultLayerTag)
    if resultLayer then
        resultLayer:removeFromParent()
    end
end


function MJGameLayerManager:showPlayersScoreLayer()
    
    local gameScene = GameSceneModule:getInstance():getGameScene()
    gameScene:addChild(PlayersScoreLayer:create(gameScene,gameScene:getGameDataManager():getPaijuInfo()),3)
end


function MJGameLayerManager:showReconnectErrorBox(code)
    local gameScene = GameSceneModule:getInstance():getGameScene()
    
    local isShowErrorBox = gameScene:getGameDataManager().gameDataModule.isShowErrorBox
    if isShowErrorBox == true then
        return
    end
    gameScene:getGameDataManager().gameDataModule.isShowErrorBox = true

    performWithDelay( gameScene , function()
        api_show_Msg_Box("你的网络不稳定,请点击确定重新进入房间,错误码 :"..code , function()
            gameScene:Exit_game(5)
        end , true )
    end , 0.2)
end




function MJGameLayerManager:OnRcvGameMessage(msgID,msgData)

    print("MJGameLayerManager:OnRcvGameMessage")
    if msgID == MessageID.ASS_GAME_BEGIN then
        
    elseif msgID == MessageID.ASS_MAKE_NT_INFO then
        
    elseif msgID == MessageID.ASS_FETCH_HANDCARDS then
        
    elseif msgID == MessageID.ASS_TOKEN then
        
    elseif msgID == MessageID.ASS_TIAO_CARDS then
        
    elseif msgID == MessageID.ASS_OUT_CARD_INFO then
        
    elseif msgID == MessageID.ASS_ACT_NOTIFY then
        
    elseif msgID == MessageID.ASS_ACT_INFO then
       
    elseif msgID == MessageID.ASS_RESULT then
        self:showGameResultLayer(msgData,false)
    elseif msgID == MessageID.ASS_PAIJU_INFO then
        
        if msgData.ResultInfo then
            self:showGameResultLayer(msgData.ResultInfo,true)
        end
    elseif msgID == MessageID.ASS_GM_AGREE_GAME then
        
    elseif msgID == MessageID.ASS_GM_GAME_STATION then
        
    end

end



return MJGameLayerManager


--endregion
