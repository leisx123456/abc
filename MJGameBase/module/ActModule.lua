--region *.lua
--Date
--此文件由[BabeLua]插件自动生成

--吃碰杠等玩家动作的标示
local ActModule = class("ActModule")

--游戏中动作的标识，一般来说动作优先级别越高，开关位越高
ActModule.MJActFlag = {
    Guo = 2,            --过
    Chi = 4,            --吃
    Peng = 8,           --碰
    Gang = 16,          --杠
    Ting = 32,          --听
    Hu = 16*16*16,   --胡
}

--吃碰杠动作的类型
ActModule.CPGType = {

    UNKNOWN		= 0,	--未知类型
	Chi			= 1,	--吃
	Peng		= 2,	--碰
	MingGang	= 3,	--明杠
	BuGang		= 4,	--补杠
	AnGang		= 5,	--暗杠
    SpecialGang = 6,    --特殊杠
}

--吃的类型
ActModule.EatType = {
    usT	= 1,			--吃头
    usZ	= 2,			--吃中
	usW	= 4,			--吃尾
	usTZ = 3,			--吃头 吃中
	usTW = 5,			--吃头 吃尾
	usZW = 6,			--吃中 吃尾
	usTZW = 7,			--吃头 吃中 吃尾
}

return ActModule
--endregion
