--region *.lua
--Date
--此文件由[BabeLua]插件自动生成

local MJEffectManager = class("MJEffectManager")
local ActModule = import("..module.ActModule")


--不区分男女方言相关
EffectEnum = {
    ANN     = "ann.mp3",
    WIN     = "win.mp3",
    LOSE    = "lose.mp3",
    KAIS    = "kais.mp3",
    PAOD    = "paod.mp3",
    DAOJS   = "daojs.mp3",
    LIK     = "lik.mp3",
    JIAR    = "jiar.mp3", 
    ZHUAP   = "zhuap.mp3",
    CHUP    = "chup.mp3",
    QIEP    = "qiep.mp3",
    SHOUP   = "shoup.mp3",
    DIANP   = "dianp.mp3",
    XIAOX   = "xiaox.mp3",
    TOUZ    = "touz.mp3",
}
--播放音效(根据男女方言)
function MJEffectManager:PlayEffect(effectname,ui_chair_index)
    local filename = self:getEffectPath(ui_chair_index)..effectname
    print("------------filename = " .. filename)

    local isExit = cc.FileUtils:getInstance():isFileExist(filename)

    if isExit == true then
        Music:playEffect(filename,false)
    else
        --api_show_Msg_Box("音效不存在  " .. filename )
    end

end

--播放出牌音效 ui_chair_index : 玩家的服务器椅子号
function MJEffectManager:PlayCardEffect(value,ui_chair_index)
    if IS_HAVE_TWO_MJZ_AUDIO then
        if value < 0  then
            self:playOtherEffect(EffectEnum.CHUP,false)
        elseif value < 10 then
            self:PlayEffect("w_".. value%10 .."_".. tostring(math.random(2)) .. ".mp3",ui_chair_index)
        elseif value < 20 then
            self:PlayEffect("s_".. value%10 .."_".. tostring(math.random(2)) .. ".mp3",ui_chair_index)
        elseif value < 30 then
            self:PlayEffect("t_".. value%10 .."_".. tostring(math.random(2)) .. ".mp3",ui_chair_index)
        elseif value < 38 then
            self:PlayEffect("f_".. value%10 .."_".. tostring(math.random(2)) .. ".mp3",ui_chair_index)
        end
    else
        if value < 0  then
            self:playOtherEffect(EffectEnum.CHUP,false)
        elseif value < 10 then
            self:PlayEffect("w_".. value%10 ..".mp3",ui_chair_index)
        elseif value < 20 then
            self:PlayEffect("s_".. value%10 ..".mp3",ui_chair_index)
        elseif value < 30 then
            self:PlayEffect("t_".. value%10 ..".mp3",ui_chair_index)
        elseif value < 38 then
            self:PlayEffect("f_".. value%10 ..".mp3",ui_chair_index)
        end
    end

    
end

--播放出牌动作音效
function MJEffectManager:PlayActEffect(act , ui_chair_index)
    math.randomseed(os.time())
    if act == ActModule.CPGType.Guo then
        
    elseif act == ActModule.CPGType.Chi then
        self:PlayEffect("chi_" .. tostring(math.random(2)) .. ".mp3", ui_chair_index)
    elseif act == ActModule.CPGType.Peng then
        self:PlayEffect("peng_" .. tostring(math.random(3)) .. ".mp3", ui_chair_index)
    elseif act == ActModule.CPGType.MingGang or act == ActModule.CPGType.AnGang or act == ActModule.CPGType.BuGang then
        self:PlayEffect("gang_" .. tostring(math.random(1)) .. ".mp3", ui_chair_index)
    --elseif act == ActModule.CPGType.BuGang then            
    --  self:PlayEffect("buz.mp3", ui_chair_index)
    elseif act == ActModule.MJActFlag.Hu then
        self:PlayEffect("hu_" .. tostring(math.random(3)) .. ".mp3", ui_chair_index)
    end
end

--播放自摸音效
function MJEffectManager:playZimoEffect(ui_chair_index)
    self:PlayEffect("zim_" .. tostring(math.random(2)) .. ".mp3" , ui_chair_index)
end


--获取当前配置音效路径
function MJEffectManager:getEffectPath(ui_chair_index)

    local GameDataMnger = GameSceneModule:getInstance():getGameScene():getGameDataManager()
    local allUserData = GameDataMnger:getAllUserData();
    local isBoy = allUserData[ui_chair_index].bBoy
    
    print("ui_chair_index = " .. ui_chair_index)
    if isBoy then
        print(" isBoy  == " ,isBoy)
    end

    
    local isFangyan = (UserData:getLanguage() == 0)
    local path = ""
    if isBoy == 1 then
        if isFangyan then
            path = path .. "sound/fangyannan/"
        else
            path = path .. "sound/putongnan/"
        end
    else
        if isFangyan then
            path = path.. "sound/fangyannv/"
        else
            path = path .. "sound/putongnv/"
        end
    end
    return path
end

----------------------------------------------------------------------

--播放其他音效
function MJEffectManager:playOtherEffect(effectname,isLoop)
    local filename = "sound/other/" .. effectname 
    Music:playEffect(filename,isLoop)
end


--播放倒计时音效
function MJEffectManager:playCountdownEffect()
    local filename = "sound/other/daojs.mp3"
    --self.sound_id = AudioEngine.playEffect(filename, true)

    local isExit = cc.FileUtils:getInstance():isFileExist(filename)
    if isExit == true then
        if UserData:getSound() == 1 then
            self.sound_id = AudioEngine.playEffect(filename, true)
            performWithDelay(GameSceneModule:getInstance():getGameScene(),
            function()
                self:stopCountdownEffect()
            end
            ,3
            )
        end
    else
        api_show_Msg_Box("音效不存在  " .. filename )
    end

end

--停止播放倒计时音效
function MJEffectManager:stopCountdownEffect()
    print("MJEffectManager:stopCountdownEffect")
    if self.sound_id ~= nil then
        
        AudioEngine.stopEffect(self.sound_id)
        self.sound_id = nil
    end
    
end


return MJEffectManager

--endregion
