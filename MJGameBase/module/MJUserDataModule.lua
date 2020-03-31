--region *.lua
--Date
--此文件由[BabeLua]插件自动生成
local MJUserDataModule = class("MJUserDataModule")

local CPGNode = {
    actFlag = 0,                    --当前动作类型
    from = -1,                      --动作来源
    values = {0,0,0,0},             --牌栏的牌
    extend = nil                    --拓展字段
}

function MJUserDataModule:ctor()

    self.dwUserID = 0                   --玩家UserID
    self.status = -1                    --玩家状态(用于判断当前是否离线)
    self.nickName = ""                  --昵称
    self.bBoy = true                    --性别
    self.avatarUrl = ""                 --头像URL
    self.isVertual = false
    self.dwUserIP = ""                  --玩家IP
    self.bLogoID = -1   
    self.dwMoney = 0                    --玩家金币数
    self.bDeskNo = -1                   --桌子号
    self.dwProfit = 0 
    self.bDeskStation = -1              --玩家的椅子号
    self.behavior = {}

    --游戏数据
    self.totalPoint     = 0             --当前总分数
    self.CPGRecords   =   {
        clone(CPGNode),
        clone(CPGNode),
        clone(CPGNode),
        clone(CPGNode)
        }                           --吃碰杠牌记录   
    self.CGPNums        =   0           --吃碰杠牌数量
    self.HandCards      =   {0,0,0,0,0,0,0,0,0,0,0,0,0}          --手牌
    self.HandNums       =   0           --手牌数量
    self.OutedCards     =   {}          --出的牌
    self.OutedNums      =   0           --出的牌数量
    self.IsAuto         =   false       --是否托管

end

function MJUserDataModule:initData()
    print("MJUserDataModule:initData")

    --平台数据
    self.dwUserID = 0               --玩家UserID
    self.status = -1                --玩家状态(用于判断当前是否离线)
    self.nickName = ""              --昵称
    self.bBoy = true               --性别
    self.avatarUrl = ""             --头像URL
    self.isVertual = false
    self.dwUserIP = ""              --玩家IP
    self.bLogoID = -1   
    self.dwMoney = 0                --玩家金币数
    self.bDeskNo = -1               --桌子号
    self.dwProfit = 0 
    self.bDeskStation = -1          --玩家的椅子号
    self.behavior = {}

    --游戏数据
    self.totalPoint = 0             --当前总分数
    self.CPGRecords   =   {
        CPGNode,
        CPGNode,
        CPGNode,
        CPGNode
        }                           --吃碰杠牌记录   
    self.CGPNums      =   0         --吃碰杠牌数量
    self.HandCards    =   {}        --手牌
    self.HandNums     =   0         --手牌数量
    self.OutedCards   =   {}        --出的牌
    self.OutedNums    =   0         --出的牌数量
    

end

function MJUserDataModule:sortHandCards(laizies)
    table.sort(self.HandCards,function(a,b)
        return a<b
    end)
end


function MJUserDataModule:fetchOneCard(card_value)
    self.HandNums = self.HandNums + 1
    table.insert(self.HandCards,card_value)
end

function MJUserDataModule:onSelfOutCardRequest(card_value)

    for i,v in pairs (self.HandCards) do
        if v == card_value then
            table.remove(self.HandCards,i)
            break
        end
    end
end


return MJUserDataModule
--endregion
