--region *.lua
--Date
--此文件由[BabeLua]插件自动生成

local MJGameDataModule = class("MJGameDataModule")


--
function MJGameDataModule:ctor(AllUserData)
    self.AllUserData = AllUserData
    self.NTChair = -1                   --庄家的椅子号
    self.chair = -1                     --自己的椅子号
    self.gameStation = -1               --当前游戏状态
    self.isCanHandoutCard = false       --当前是否可以出牌
    self.isHaveOutTingCards = false     -- 当前是否有打出可以听的牌
    self.isOpenCheckTing = false        --是否打开查听功能
    self.isGetGameStation = false       --是否收到断线重连消息
    self.PaijuInfo = {}                 --牌局消息
    self.TokenUser = -1                 --当前持玩家
    self.OutCardUser = -1               --当前出牌玩家(用于显示出牌上方的提示)
    self.isShowErrorBox = false         --当前是否显示错误提示
    --当前已经可见的索引就是牌值  例如 ShowCardsModule.ShowCards[5]  代表当前已经可见几个5万 (包括自己手中的)
    self.ShowCards = {0,0,0,0,0,0,0,0,0,0,
                         0,0,0,0,0,0,0,0,0,0,
                         0,0,0,0,0,0,0,0,0,0,
                         0,0,0,0,0,0,0,0,0,0,}
   
    --游戏中所有的赖子
    self.AllLaizies = {}
end

function MJGameDataModule:initData()
    
    print("MJUserDataModule:initData")
    self.AllRound = 4                   --总局数
    self.NTChair = -1                   --庄家的椅子号
    self.chair = -1                     --自己的椅子号
    self.gameStation = -1               --当前游戏状态
    self.isCanHandoutCard = false       --当前是否可以出牌
    self.isHaveOutTingCards = false     -- 当前是否有打出可以听的牌
    self.isOpenCheckTing = false        --是否打开查听功能
    self.isGetGameStation = false       --是否收到断线重连消息
    self.PaijuInfo = {}                 --牌局消息
    self.TokenUser = -1                 --当前持玩家
    self.OutCardUser = -1               --当前出牌玩家(用于显示出牌上方的提示)

    --当前已经可见的索引就是牌值  例如 ShowCardsModule.ShowCards[5]  代表当前已经可见几个5万 (包括自己手中的)
    self.ShowCards = {0,0,0,0,0,0,0,0,0,0,
                         0,0,0,0,0,0,0,0,0,0,
                         0,0,0,0,0,0,0,0,0,0,
                         0,0,0,0,0,0,0,0,0,0,}
end



return MJGameDataModule

--endregion
