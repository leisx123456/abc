--region *.lua
--Date
--此文件由[BabeLua]插件自动生成


local PlayerInfoLayer = import("..view.PlayerInfoLayer")
local MJMessageID = import("..MJMessageID")
local ActModule = import("..module.ActModule")
local MJGameDataManager = class("MJGameDataManager")

--引用具体麻将的玩家数据模型
local UserDataModule = import("..."..GAME_ID..".module.UserDataModule")
--引用具体麻将的数据管理类

function MJGameDataManager:ctor(gameDataModule)

    self.gameDataModule = gameDataModule

end

function MJGameDataManager:init()
    
end

function MJGameDataManager:setAllRound(round)
    self.gameDataModule.AllRound = round
end

function MJGameDataManager:getAllRound()
    return self.gameDataModule.AllRound
end

function MJGameDataManager:getAllUserData()
    return self.gameDataModule.AllUserData
end


--根据服务器获取玩家的信息
function MJGameDataManager:getUserData(chair)
    return self.gameDataModule.AllUserData[chair + 1]
end

--初始化某个座位上玩家的数据
function MJGameDataManager:initUserInfo(index)
    self.gameDataModule.AllUserData[index]:initData()
end

function MJGameDataManager:setSelfChair(chair)
    print("MJGameDataManager:setSelfChair====>",chair)
    if chair and chair >= 0 then
        self.gameDataModule.chair = chair
    end
end

function MJGameDataManager:getSelfChair()
    return self.gameDataModule.chair
end

--设置自己当前是否托管
function MJGameDataManager:setSelfIsAuto(isAuto)
    if isAuto ~= nil then
       self.gameDataModule.AllUserData[self:getSelfChair() + 1].IsAuto = isAuto
    end
end

--获取当前自己是否脱托管
function MJGameDataManager:getSelfIsAuto()
    if self:getSelfChair() < 0 then
        return false
    end
    return self.gameDataModule.AllUserData[self:getSelfChair() + 1].IsAuto
end

function MJGameDataManager:getSelfHandCards()
    return self.gameDataModule.AllUserData[self:getSelfChair() + 1].HandCards
end

function MJGameDataManager:getHandCards(chair)
    return self.gameDataModule.AllUserData[chair + 1].HandCards
end


--根据服务器椅子号获取UI上的位置
function MJGameDataManager:getUIChairByServerChair(chair)

    if PLAYER_COUNT == 2 then
        if self.gameDataModule.chair == chair then
            return 1
        else
            return 3
        end
    elseif PLAYER_COUNT == 3 then
        if self.gameDataModule.chair == chair then
            return 1
        elseif (self.gameDataModule.chair + 1) % PLAYER_COUNT == chair then
            return 2
        else
            return 4
        end
    end
    return (4 + tonumber(chair) - self.gameDataModule.chair)%4+1
  
end

function MJGameDataManager:getLastOutUIChair()
    if self.gameDataModule.OutCardUser == -1 then
        return -1
    end
    return self:getUIChairByServerChair(self.gameDataModule.OutCardUser)
end


--获取客户端的椅子号
function MJGameDataManager:getClientChair(chair)
    
    if PLAYER_COUNT == 2 then
        if chair == 1 then
            return 2
        end

    elseif PLAYER_COUNT == 3 then
        if chair == 2 then
            return  3
        end
    end
    return chair
end



function MJGameDataManager:getServerChairByUIChair(UIChair)
    
    for i = 0, PLAYER_COUNT-1 do
        local serverChair = self:getUIChairByServerChair(i)
        if serverChair == UIChair then
            return i
        end
    end
    return -1

end

--是否可以胡七对子
function MJGameDataManager:getIsCanHu7Dui()
    if GameSceneModule:getInstance():getGameScene():getGameType() == "tz1" then
        --金币场
        return true
    end
    local deskInfo = GameSceneModule:getInstance():getGameScene():getDeskInfo()
    local isCan7Dui = false
    if deskInfo and deskInfo.desk_select_config.Select then
        for i,v in pairs(deskInfo.desk_select_config.Select) do
            if v.id == 2 then
                isCan7Dui = true
                break
            end
        end
    end
    
    return isCan7Dui
end


--重新设置聊天表情的显示位置
function MJGameDataManager:resetEmojiShowPositions()

    local emojiPos = {}
    local chatPos = {}

    if self:getGameStation() <= GameStation.GS_WAIT_ARGEE then
        emojiPos = EMOJI_POSITIONS_NOTSTART
        chatPos = CHAT_POSITIONS_NOTSTART
    else
        emojiPos = EMOJI_POSITIONS
        chatPos = CHAT_POSITIONS
    end

    --初始化表情的位置
    GameSceneModule:getInstance():getGameScene():initExpression(
        emojiPos,
        1.0,
        15,
        2
    )

    --初始化文本聊天的位置
    GameSceneModule:getInstance():getGameScene():initChatText(
        chatPos,
        {4,2,4,4},
        CHAT_CONTENTS,
        300,
        1.0,
        2
    )

end





function MJGameDataManager:setNTChair(NTChair)
    if NTChair and NTChair >= 0 then
        self.gameDataModule.NTChair = NTChair
    end
end

function MJGameDataManager:getNTChair()
    return self.gameDataModule.NTChair
end

function MJGameDataManager:setIsCanHandoutCard(isCanOut)
    self.gameDataModule.isCanHandoutCard = isCanOut
end

--是否已经获取到游戏状态
function MJGameDataManager:IsGetGameStation()
    return self.gameDataModule.isGetGameStation 
end

--设置是否已经获取到游戏状态
function MJGameDataManager:setIsGetGameStation(isGet)
    self.gameDataModule.isGetGameStation = isGet
end


--获取是否当前可以出牌
function MJGameDataManager:getIsCanHandoutCard()
    return self.gameDataModule.isCanHandoutCard
end

--设置是否打开查听功能
function MJGameDataManager:setIsOpenCheckTing(isOpen)
    self.gameDataModule.isOpenCheckTing = isOpen
end

function MJGameDataManager:setIsHaveOutTingCards(isHave)
    self.gameDataModule.isHaveOutTingCards = isHave
end

function MJGameDataManager:getIsHaveOutTingCards()
    return self.gameDataModule.isHaveOutTingCards
end


--设置是否打开查听功能
function MJGameDataManager:getIsOpenCheckTing()
    return self.gameDataModule.isOpenCheckTing
end


--设置当前游戏状态
function MJGameDataManager:setGameStation(gameStation)
    if gameStation  then
        self.gameDataModule.gameStation = gameStation
    end
end

--获取当前游戏状态
function MJGameDataManager:getGameStation()
    return self.gameDataModule.gameStation
end

function MJGameDataManager:getSelfUserID()
    local selfInfo = GameSceneModule:getInstance():getGameScene():getSelfInfo()
    if selfInfo then
        return selfInfo.dwUserID
    else
        return nil
    end
    
end

function MJGameDataManager:getMasterUserID()
    if GameSceneModule:getInstance():getGameScene():getGameType() == "tz1" then
        --金币场
        return nil
    end
    return GameSceneModule:getInstance():getGameScene():getDeskInfo().mastUserID
end

function MJGameDataManager:getPaijuInfo()
    return self.gameDataModule.PaijuInfo
end
function MJGameDataManager:getShowCards()
    return self.gameDataModule.ShowCards
end

--更新已经可见的牌
function MJGameDataManager:updateShowCards(cards)
    if cards == nil or #cards == 0 then
        return 
    end

    for i,v in pairs (cards) do
        if v > 0 then
            self.gameDataModule.ShowCards[v] =  self.gameDataModule.ShowCards[v] + 1
        end
    end
end


--游戏开始
function MJGameDataManager:onGameStart(msgData)
    for i = 1,PLAYER_COUNT do
        self.gameDataModule.AllUserData[i]:initData()
    end
    self:setGameStation(GameStation.GS_PLAYING)
    self:cacheUserInfo()
    self:resetEmojiShowPositions()

end

--有玩家托管
function MJGameDataManager:onAuto(msgData)
    print("MJGameDataManager:onAuto")
    local chair = msgData.chair
    local isAuto = msgData.IsAuto

    if chair ~= nil and isAuto ~= nil then
        print("update user is auto")
        self.gameDataModule.AllUserData[chair + 1].IsAuto = isAuto
    end

end

--选庄
function MJGameDataManager:onMakeNT(msgData)
    --更新游戏状态
    self:setGameStation(GameStation.GS_PLAYING)
    --更新庄家
    self.gameDataModule.NTChair = msgData.NtUser

    if GameSceneModule:getInstance():getGameScene():getGameType() == "tz1"or isJinbi then
        --重新从大厅获取一次玩家信息
        self:reCacheUserInfo()
    end

end

--服务器下发手牌
function MJGameDataManager:onFetchCards(msgData)
    if msgData.Cards then
        for i = 1 , PLAYER_COUNT do
            if i<= #msgData.Cards and i<= #self.gameDataModule.AllUserData then
                self.gameDataModule.AllUserData[i].HandCards = clone(msgData.Cards[i])
                self.gameDataModule.AllUserData[i].HandNums = #msgData.Cards[i]  
            end
        end
    end
    self:updateShowCards(msgData.Cards[self.gameDataModule.chair+1])
end


--庄家跳牌
function MJGameDataManager:onTiaoCards(msgData)
    if msgData.NTUser == self.gameDataModule.chair then
        self:updateShowCards({msgData.tiaoFetch})
    end
end


--有玩家出牌
function MJGameDataManager:onOutCardInfo(msgData)
    
    self.gameDataModule.OutCardUser = msgData.outCardUser

    local ui_chair_index = self:getUIChairByServerChair(tonumber(msgData.outCardUser))
    if ui_chair_index == 1 then
        self.gameDataModule.AllUserData[msgData.outCardUser + 1].HandCards = msgData.HandCards
        self.gameDataModule.AllUserData[msgData.outCardUser + 1].HandNums = #msgData.HandCards
        
    end
    --更新当前出牌人的数量
    self.gameDataModule.AllUserData[msgData.outCardUser + 1].OutedNums = msgData.outCardNum
    self.gameDataModule.AllUserData[msgData.outCardUser + 1].OutedCards[msgData.outCardNum] = msgData.outCardValue
    if msgData.outCardUser == self.gameDataModule.chair then
        self.gameDataModule.isCanHandoutCard = false
    else
        self:updateShowCards({msgData.outCardValue})
    end

    

end

function MJGameDataManager:onSelfOutCardRequest(card_value)
    self.gameDataModule.AllUserData[self.gameDataModule.chair + 1]:onSelfOutCardRequest(card_value)
end


--某玩家收到令牌
function MJGameDataManager:onGetToken(msgData)

    self.gameDataModule.TokenUser = msgData.TokenbyUser
    if msgData.TokenbyUser == self.gameDataModule.chair then

        self.gameDataModule.isCanHandoutCard = true
        
        if msgData.TokenbFetch then
            self:updateShowCards({msgData.TokenFetcherVal})
            
            self.gameDataModule.AllUserData[msgData.TokenbyUser + 1]:fetchOneCard(msgData.TokenFetcherVal)
        end
    end
    
end

function MJGameDataManager:getLastOutCard()
    local index =  self.gameDataModule.AllUserData[self.gameDataModule.TokenUser + 1].OutedNums
    return self.gameDataModule.AllUserData[self.gameDataModule.TokenUser + 1].OutedCards[index]
end

--某玩家执行某个动作
function MJGameDataManager:onActInfo(msgData)
    
    print("MJGameDataManager:onActInfo")
    --当前不能出牌
    self.gameDataModule.isCanHandoutCard = false

    --更新动作发起者的牌数量(吃 碰 明杠等)
    if msgData.OutCardNum then
        self.gameDataModule.AllUserData[msgData.byFromUser+1].OutedNums = msgData.OutCardNum
    end

    
    if msgData.usActFlag ==  ActModule.MJActFlag.Chi
       or msgData.usActFlag ==  ActModule.MJActFlag.Peng
       or msgData.usActFlag ==  ActModule.MJActFlag.Gang then
        self.gameDataModule.AllUserData[msgData.byToUser+1].CPGRecords[msgData.nIdx + 1].from = clone(msgData.byFromUser)
        self.gameDataModule.AllUserData[msgData.byToUser+1].CPGRecords[msgData.nIdx + 1].values = clone(msgData.cpgCards)
        self.gameDataModule.AllUserData[msgData.byToUser+1].CPGRecords[msgData.nIdx + 1].actFlag = clone(msgData.usActFlag)
    end
    

    if msgData.usActFlag ==  ActModule.MJActFlag.Chi then
        local chair_index = self:getUIChairByServerChair(msgData.byToUser)
        if chair_index == 1 then
            self.gameDataModule.AllUserData[self:getSelfChair() + 1].HandCards = msgData.HandCards 
            self.gameDataModule.AllUserData[self:getSelfChair() + 1]:sortHandCards(self.gameDataModule.AllLaizies)
        end
        if msgData.byToUser ~= self.gameDataModule.chair then
            if msgData.chiFlag == ActModule.EatType.usT then
                self:updateShowCards( {msgData.cpgCards[2],msgData.cpgCards[3]} )
            elseif msgData.chiFlag == ActModule.EatType.usZ then
                self:updateShowCards( {msgData.cpgCards[1],msgData.cpgCards[3]} )
            else
                self:updateShowCards( {msgData.cpgCards[1],msgData.cpgCards[2]} )
            end
        end
        
        
    elseif msgData.usActFlag ==  ActModule.MJActFlag.Peng then
        local chair_index = self:getUIChairByServerChair(msgData.byToUser)
        if chair_index == 1 then
            self.gameDataModule.AllUserData[self:getSelfChair() + 1].HandCards = msgData.HandCards 
            self.gameDataModule.AllUserData[self:getSelfChair() + 1]:sortHandCards(self.gameDataModule.AllLaizies)
        end
        if msgData.byToUser ~= self.gameDataModule.chair then
            self:updateShowCards({msgData.cpgCards[1],msgData.cpgCards[2]})
        end
        
    elseif msgData.usActFlag ==  ActModule.MJActFlag.Gang then
        local chair_index = self:getUIChairByServerChair(msgData.byToUser)
        if chair_index == 1 then
            self.gameDataModule.AllUserData[self:getSelfChair() + 1].HandCards = msgData.HandCards 
            self.gameDataModule.AllUserData[self:getSelfChair() + 1]:sortHandCards(self.gameDataModule.AllLaizies)
        end
        if msgData.byToUse ~= self.gameDataModule.chair then
            local gangType = msgData.gangType
            if gangType == ActModule.CPGType.BuGang then
                self:updateShowCards({msgData.cpgCards[1]})
            elseif gangType == ActModule.CPGType.MingGang then
                self:updateShowCards({msgData.cpgCards[1],msgData.cpgCards[2],msgData.cpgCards[3]})
            elseif gangType == ActModule.CPGType.AnGang then
                self:updateShowCards(msgData.cpgCards)
            end
        end
        
    elseif msgData.usActFlag ==  ActModule.MJActFlag.Hu then
        --对胡牌玩家手牌进行排序
        for i,v in pairs(msgData.byToUsers) do
            
            local chair_index = self:getUIChairByServerChair(v)
            if chair_index == 1 then
                self.gameDataModule.AllUserData[self:getSelfChair() + 1].HandCards = msgData.HandCards 
            end

            if g_server_obj:get_server_type()=="video" then
                self:sortN_1(v,msgData.HandCards[v+1])
            else
                self:sortN_1(v,msgData.HandCards[i])
            end
        end
        
    end
     
end

--对牌的前n-1张牌进项排序
function MJGameDataManager:sortN_1(chair,cards)
    
    local HandCardsTemp = {}
    if cards == nil then
        return
    end
    for j = 1,#cards-1  do
        HandCardsTemp[j] = cards[j]
    end
                
    self.gameDataModule.AllUserData[chair + 1].HandCards = HandCardsTemp
    self.gameDataModule.AllUserData[chair + 1]:sortHandCards(self.gameDataModule.AllLaizies)
    self.gameDataModule.AllUserData[chair + 1].HandCards[#cards] = cards[#cards]
end




function MJGameDataManager:onGameResult(msgData)
    for i,v in pairs(self.gameDataModule.ShowCards) do
        self.gameDataModule.ShowCards[i] = 0
    end

    --游戏结束重新初始化每个人的手牌
    for i = 1,PLAYER_COUNT do
        self.gameDataModule.AllUserData[i].HandCards = {0,0,0,0,0,0,0,0,0,0,0,0,0}
    end
    

    self:setGameStation(GameStation.GS_WAIT_NEXT_ROUND)
    --更新客户端缓存的玩家信息中的分数

    if GameSceneModule:getInstance():getGameScene():getGameType() ~= "tz1" then

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

        if isJinbi == false then
            --更新客户端缓存的玩家信息中的分数
            for i = 1,PLAYER_COUNT  do
                if msgData.TotalScore then
                    print(" GameScene:onGameResul 游戏结束 更新玩家缓存信息")
                    self.gameDataModule.AllUserData[i].dwMoney = msgData.TotalScore[i]
                    print(" self.PlayersInfos[i].dwMoney  ==  " , self.gameDataModule.AllUserData[i].dwMoney)
                end
            end
        end
    else
        for i = 1,PLAYER_COUNT  do
            self.gameDataModule.AllUserData[i].dwMoney = msgData.TotalScore[i]
            print("结算消息更新玩家分数 self.PlayersInfos[i].dwMoney  ==  " , self.gameDataModule.AllUserData[i].dwMoney)
        end
    end
    --对玩家的手牌进行重新排序
    for i = 1,PLAYER_COUNT  do
        
        if msgData.HandCards[i] then
            if #msgData.HandCards[i] % 3 == 2 then
                self:sortN_1(i-1,msgData.HandCards[i])
            else
                self.gameDataModule.AllUserData[i].HandCards = msgData.HandCards[i]
                self.gameDataModule.AllUserData[i]:sortHandCards(self.gameDataModule.AllLaizies)
            end
        end
    end
    
end

--游戏状态
function MJGameDataManager:onGameStation(msgData)
    print("MJGameDataManager:onGameStation")

    if msgData.TokenUser then
        self.gameDataModule.TokenUser = msgData.TokenUser
    end
    
    --缓存玩家数据
    self:cacheUserInfo()
    --更新游戏状态
    if msgData.GSID ~= nil then
        self:setGameStation(msgData.GSID)
        self:resetEmojiShowPositions()
    end
    --更新自己的椅子号
    if msgData.chair ~= nil then
        self:setSelfChair(msgData.chair)
    end
    if msgData.HandCards then
        for i = 1,PLAYER_COUNT do
            if msgData.HandCards[i] then
                self.gameDataModule.AllUserData[i].HandCards = msgData.HandCards[i]
                if #msgData.HandCards[i] % 3 == 2 then
                    self:sortN_1(i-1,msgData.HandCards[i])
                else
                    self.gameDataModule.AllUserData[i]:sortHandCards(self.gameDataModule.AllLaizies)
                end
                
                self.gameDataModule.AllUserData[i].HandNums = #msgData.HandCards[i]
                if msgData.OutedNums then
                    self.gameDataModule.AllUserData[i].OutedNums = msgData.OutedNums[i]
                end
                if msgData.OutedCards then
                    self.gameDataModule.AllUserData[i].OutedCards = msgData.OutedCards[i]
                end
            end
        end
    end
    
    --此时缓存的玩家信息中 如果是圈子模式玩家的分数可能是错误的 更新玩家的分数为服务器下发的总分数
    if GameSceneModule:getInstance():getGameScene():getGameType() ~= "tz1" then
        if msgData.TotalScore  then
            for i,v in pairs(self.gameDataModule.AllUserData) do
                if v.bDeskStation >= 0 and msgData.TotalScore[i] then
                    v.dwMoney = msgData.TotalScore[i]
                end
            end
        end
    end

    if msgData.TokenUser == self.gameDataModule.chair and #msgData.HandCards[self.gameDataModule.chair + 1] % 3 == 2 then
        self.gameDataModule.isCanHandoutCard = true
    end

    for i = 1,PLAYER_COUNT  do
        --更新别人的牌中可见的牌数量
        if msgData.OutedNums and msgData.OutedCards then
            if i <= #msgData.OutedNums and msgData.OutedNums[i] > 0 then
                self:updateShowCards(msgData.OutedCards[i])
            end
        end
        
        local a = self:getShowCards()
        local a1 = 1

        --更新吃碰杠的牌中可见的牌数量
        if msgData.CGPNums and msgData.CPGRecords then
             for j = 1,msgData.CGPNums[i]  do
                self.gameDataModule.AllUserData[i].CPGRecords[j].from = msgData.CPGRecords[i][j].from
                self.gameDataModule.AllUserData[i].CPGRecords[j].values = msgData.CPGRecords[i][j].val
                self.gameDataModule.AllUserData[i].CPGRecords[j].actFlag = msgData.CPGRecords[i][j].usFlags
                self:updateShowCards(msgData.CPGRecords[i][j].val)
            end
        end
    end

    --只更新自己的手牌中可见的牌数量
    if msgData.HandCards then
        self:updateShowCards(msgData.HandCards[msgData.chair + 1])
    end
end

function MJGameDataManager:onCmdChangeCards(cards)
    local selfChair = self:getSelfChair()
    local selfHandCards = self.gameDataModule.AllUserData[selfChair + 1].HandCards
    if  selfHandCards == nil or  #selfHandCards <= 0 then
        return
    end
    for i,value in pairs (cards) do
        selfHandCards[i] = tonumber(value)
    end
end


--缓存已经坐下的玩家信息
function MJGameDataManager:cacheUserInfo()

    for key, sitPlayer in pairs(GameSceneModule:getInstance():getGameScene():getSitDownPlayerMap()) do
        if self.gameDataModule.AllUserData[sitPlayer.bDeskStation + 1] == nil 
        or self.gameDataModule.AllUserData[sitPlayer.bDeskStation + 1].dwUserID ~= sitPlayer.dwUserID
        or self.gameDataModule.AllUserData[sitPlayer.bDeskStation + 1].bDeskStation ~= sitPlayer.bDeskStation  then
            
            --注意 : 此处不能直接用 self.gameDataModule.AllUserData[sitPlayer.bDeskStation + 1] = sitPlayer  会破坏继承关系

            self.gameDataModule.AllUserData[sitPlayer.bDeskStation + 1].dwUserID = sitPlayer.dwUserID               --玩家UserID
            self.gameDataModule.AllUserData[sitPlayer.bDeskStation + 1].status = sitPlayer.status                --玩家状态(用于判断当前是否离线)
            self.gameDataModule.AllUserData[sitPlayer.bDeskStation + 1].nickName = sitPlayer.nickName              --昵称
            self.gameDataModule.AllUserData[sitPlayer.bDeskStation + 1].bBoy = sitPlayer.bBoy               --性别
            self.gameDataModule.AllUserData[sitPlayer.bDeskStation + 1].avatarUrl = sitPlayer.avatarUrl             --头像URL
            self.gameDataModule.AllUserData[sitPlayer.bDeskStation + 1].isVertual = sitPlayer.isVertual
            self.gameDataModule.AllUserData[sitPlayer.bDeskStation + 1].dwUserIP = sitPlayer.dwUserIP              --玩家IP
            self.gameDataModule.AllUserData[sitPlayer.bDeskStation + 1].bLogoID = sitPlayer.bLogoID   
            self.gameDataModule.AllUserData[sitPlayer.bDeskStation + 1].dwMoney = sitPlayer.dwMoney               --玩家金币数
            self.gameDataModule.AllUserData[sitPlayer.bDeskStation + 1].bDeskNo = sitPlayer.bDeskNo               --桌子号
            self.gameDataModule.AllUserData[sitPlayer.bDeskStation + 1].dwProfit = sitPlayer.dwProfit
            self.gameDataModule.AllUserData[sitPlayer.bDeskStation + 1].bDeskStation = sitPlayer.bDeskStation          --玩家的椅子号
            self.gameDataModule.AllUserData[sitPlayer.bDeskStation + 1].behavior = sitPlayer.behavior

        end
    end
end

--重新从大厅获取玩家信息
function MJGameDataManager:reCacheUserInfo()
    for key, sitPlayer in pairs(GameSceneModule:getInstance():getGameScene():getSitDownPlayerMap()) do
        if sitPlayer then
            self.gameDataModule.AllUserData[sitPlayer.bDeskStation + 1].dwUserID = sitPlayer.dwUserID               --玩家UserID
            self.gameDataModule.AllUserData[sitPlayer.bDeskStation + 1].status = sitPlayer.status                --玩家状态(用于判断当前是否离线)
            self.gameDataModule.AllUserData[sitPlayer.bDeskStation + 1].nickName = sitPlayer.nickName              --昵称
            self.gameDataModule.AllUserData[sitPlayer.bDeskStation + 1].bBoy = sitPlayer.bBoy               --性别
            self.gameDataModule.AllUserData[sitPlayer.bDeskStation + 1].avatarUrl = sitPlayer.avatarUrl             --头像URL
            self.gameDataModule.AllUserData[sitPlayer.bDeskStation + 1].isVertual = sitPlayer.isVertual
            self.gameDataModule.AllUserData[sitPlayer.bDeskStation + 1].dwUserIP = sitPlayer.dwUserIP              --玩家IP
            self.gameDataModule.AllUserData[sitPlayer.bDeskStation + 1].bLogoID = sitPlayer.bLogoID   
            self.gameDataModule.AllUserData[sitPlayer.bDeskStation + 1].dwMoney = sitPlayer.dwMoney               --玩家金币数
            self.gameDataModule.AllUserData[sitPlayer.bDeskStation + 1].bDeskNo = sitPlayer.bDeskNo               --桌子号
            self.gameDataModule.AllUserData[sitPlayer.bDeskStation + 1].dwProfit = sitPlayer.dwProfit
            self.gameDataModule.AllUserData[sitPlayer.bDeskStation + 1].bDeskStation = sitPlayer.bDeskStation          --玩家的椅子号
            self.gameDataModule.AllUserData[sitPlayer.bDeskStation + 1].behavior = sitPlayer.behavior
        end
    end
end


--处理麻将玩家公共数据
function MJGameDataManager:OnRcvGameMessage(msgID,msgData)

    if msgID == MJMessageID.ASS_GAME_BEGIN then
        self:onGameStart(msgData)
    elseif msgID == MJMessageID.ASS_AUTO_INFO then
        self:onAuto(msgData)
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
        self:onGameResult(msgData)
    elseif msgID == MJMessageID.ASS_PIAO_NOTIFY then
        
    elseif msgID == MJMessageID.ASS_PIAO_INFO then
        
    elseif msgID == MJMessageID.ASS_NIAOCARDS_INFO then
        
    elseif msgID == MJMessageID.ASS_PAIJU_INFO then
        self.gameDataModule.PaijuInfo = msgData
        self:onGameResult(msgData.ResultInfo)
    elseif msgID == MJMessageID.ASS_GM_AGREE_GAME then
        
    elseif msgID == MJMessageID.ASS_GM_GAME_STATION then
        self:onGameStation(msgData)
    end
    
end



return MJGameDataManager


--endregion
