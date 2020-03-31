--region *.lua
--Date
--此文件由[BabeLua]插件自动生成





--local MJMessageID = import("app.platform.game.MJGameBase.MJMessageID")
local MJMessageID = import("..MJMessageID")

local GameSceneModule = class("GameSceneModule")



function GameSceneModule:getInstance()
    if not self.GameSceneModule then
       self.GameSceneModule = self.new()
    end
    return self.GameSceneModule
end

function GameSceneModule:onDestroy()
    if self and self.GameSceneModule then
        self.GameSceneModule = nil
    end
end

function GameSceneModule:init(GameScene)
    
    self.GameScene = GameScene
    --print("*********************************dong test setGameMessageHandler = 1111")
 
end

function GameSceneModule:getGameScene()
    return self.GameScene
end


return GameSceneModule

--endregion
