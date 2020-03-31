--region *.lua
--Date
--此文件由[BabeLua]插件自动生成


--断线重连数据
local MJGSModule = class("MJGSModule")

function MJGSModule:ctor()

    self.GSID         =   26
    self.NTUser       =   0
    self.RoundTimes   =   1
    self.CPGRecords   =   {{},{},{},{}}
    self.CGPNums      =   {0,0,0,0}
    self.HandCards    =   {{},{},{},{}}
    self.HandNums     =   { 0, 0, 0, 0 }
    self.PiaoResp     =   {{},{},{},{}}
    self.OutedCards   =   {{},{},{},{}}
    self.OutedNums    =   {0,0,0,0}
    self.chair        =   0
    self.isWatchUser  =   false
    self.actResp      =   {
                                isCanC = false,
                                usFlags = 0,
                                isCanG = false,
                                isCanH = false,
                                isCanP = false,
                          }
    self.WallLeftNums =   0
    self.WallLeft     =   0
    self.TokenUser    =   0

end

return MJGSModule

--endregion
