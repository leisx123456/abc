--region *.lua
--Date
--此文件由[BabeLua]插件自动生成


local MJMessageID = class("MJMessageID")
MJMessageID.ASS_GAME_BEGIN = 120                    --服务器下发麻将游戏开始
MJMessageID.ASS_AUTO_REQ = 126                      --来自客户端的托管请求
MJMessageID.ASS_AUTO_INFO = 127                     --服务端下发某玩家托管消息
MJMessageID.ASS_MAKE_NT_INFO = 132 				    --服务器下发定庄的消息
MJMessageID.ASS_FETCH_HANDCARDS = 136               --服务器下发手牌数据
MJMessageID.ASS_TIAO_CARDS = 137                    --服务器下发跳牌数据
MJMessageID.ASS_TOKEN = 140                         --给某个玩家令牌
MJMessageID.ASS_OUT_CARD_REQ = 141                  --客户端的出牌请求
MJMessageID.ASS_OUT_CARD_INFO = 142                 --服务器报告某玩家的出牌结果
MJMessageID.ASS_ACT_NOTIFY = 143                    --服务器下发某玩家可以执行某项运动(吃碰杠胡)
MJMessageID.ASS_ACT_REQ = 144                       --客户端向服务器提出进行某动作的请求(吃碰杠胡)
MJMessageID.ASS_ACT_INFO = 145                      --服务器返回某玩家动作执行结果
MJMessageID.ASS_RESULT = 150                        --回合算分
MJMessageID.ASS_PIAO_NOTIFY = 170                   --服务器通知客户端加飘
MJMessageID.ASS_PIAO_REQ = 171                      --客户端向服务器请求加飘
MJMessageID.ASS_PIAO_INFO = 172                     --服务器报告某个玩家加飘
MJMessageID.ASS_NIAOCARDS_INFO = 173                --服务器下发鸟牌
MJMessageID.ASS_PAIJU_INFO = 176                    --服务端向客户端发送牌局信息
MJMessageID.ASS_ASSIGN_MAKENT_FINISH = 179 		    --客户端向服务器发送播放完定庄动画
    
MJMessageID.ASS_GM_AGREE_GAME = 1                   --同意游戏
MJMessageID.ASS_GM_GAME_STATION = 2                 --断线重连
MJMessageID.ASS_DISMISS_DESK = 3                    --申请解散桌子
MJMessageID.ASS_AGREE_DISMISS_DESK = 4              --申请同意解散桌子
MJMessageID.ASS_QUIT_REQ = 6                        --玩家申请站起

return MJMessageID

--endregion
