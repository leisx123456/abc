--region *.lua
--Date
--此文件由[BabeLua]插件自动生成

local CheckTingLayer = class("CheckTingLayer",function() 
        return cc.CSLoader:createNode("ui_csb/CheckTingLayer.csb") 
    end)


--tingcards 当前可以听到的所有牌
--showcards 当前所有已经看到的牌
function CheckTingLayer:ctor(layerManager,tingcards,showcards)
    print("CheckTingLayer:ctor")
    self.gameLayerManager = layerManager
    self.showcards = showcards
    

    local Image_card_item = self:getChildByName("Image_card_item")
    Image_card_item:setVisible(false)
    self.Image_card_item = Image_card_item
    local Panel_check_ting_5 = self:getChildByName("Panel_check_ting_5")
    Panel_check_ting_5:setVisible(false)
    local Panel_check_ting_10 = self:getChildByName("Panel_check_ting_10")
    Panel_check_ting_10:setVisible(false)
    local Panel_check_ting_10more = self:getChildByName("Panel_check_ting_10more")
    Panel_check_ting_10more:setVisible(false)
    local Panel_check_ting_all = self:getChildByName("Panel_check_ting_all")
    Panel_check_ting_all:setVisible(false)
    


    local tingcardsNum = #tingcards
    if tingcardsNum <= 5 then
        --只显示一排
        self.Image_bg = Panel_check_ting_5:getChildByName("Image_bg")
        
        for i = 1,tingcardsNum do
            local item_clone = self:cloneCardItem(tingcards[i])
            item_clone:setPosition(cc.p(180 + (i-1) * 170 ,70))
            
            print(" x = ",180 + (i-1) * 170, "    y = ",70)
            self.Image_bg:addChild(item_clone)
            if i == tingcardsNum then -- 找到最后一张，获取他的坐标点
                local xItemPos = item_clone:getPositionX()
                --计算背景图的大小
                local xWidth = xItemPos+100
                self.Image_bg:setContentSize(cc.size(xWidth,self.Image_bg:getContentSize().height))
            end
        end
        
        self.Image_bg:setAnchorPoint(ccp(0.5, 0.5))
        self.Image_bg:setPositionX(WinSize.width/2)

        Panel_check_ting_5:setVisible(true)
    elseif tingcardsNum>5 and tingcardsNum <= 10 then
        --显示两排
        self.Image_bg = Panel_check_ting_10:getChildByName("Image_bg")
        for i = 1,tingcardsNum do
            local item_clone = self:cloneCardItem(tingcards[i])
            if i<= 5 then
                item_clone:setPosition(cc.p(180 + (i-1) * 170 ,181))
            else
                item_clone:setPosition(cc.p(180 + (i-1-5) * 170 ,72))
            end
            self.Image_bg:addChild(item_clone)
        end
        Panel_check_ting_10:setVisible(true)
    elseif tingcardsNum < 27 then

        self.Image_bg = Panel_check_ting_10more:getChildByName("Image_scroll_bg")

        --显示10+ 可滑动
        local ScrollView_cards = Panel_check_ting_10more:getChildByName("ScrollView_cards")
        --设置滚动区域大小
        local width = 862 +  (math.ceil(tingcardsNum/2)-5)* 170
        print("width = " .. width)
        ScrollView_cards:setInnerContainerSize(cc.size(width,253))
        for i = 1,tingcardsNum do
            local item_clone = self:cloneCardItem(tingcards[i])
            if i<= 5 then
                item_clone:setPosition(cc.p(90 + (i-1) * 170 ,181))
            elseif i>5 and i<= 10 then
                item_clone:setPosition(cc.p(90 + (i-1-5) * 170 ,72))
            else
                local x = 90 + (math.ceil(i/2)-1) * 170
                local y = 0
                if i%2 == 1 then
                    y = 181
                else
                    y = 72
                end
                item_clone:setPosition(cc.p(x,y))
            end
            ScrollView_cards:addChild(item_clone)
        end

        Panel_check_ting_10more:setVisible(true)
    elseif tingcardsNum == 27 or tingcardsNum==34 or tingcardsNum==35 then
        --胡所有牌(TODO 转转麻将没有风字牌 其他麻将如果需要查听这里可能需要优化)
        self.Image_bg = Panel_check_ting_all:getChildByName("Image_bg")
        Panel_check_ting_all:setVisible(true)
        
        local node1 = self.Image_bg:getChildByName("FileNode_1")
        node1:setVisible(false)
    end

    local touch_listener = cc.EventListenerTouchOneByOne:create()
    touch_listener:setSwallowTouches(false) 
    touch_listener:registerScriptHandler(handler(self, self.onTouchBegan), cc.Handler.EVENT_TOUCH_BEGAN)
    local eventDispatcher = self:getEventDispatcher()      
    eventDispatcher:addEventListenerWithSceneGraphPriority(touch_listener, self.Image_bg) 

   
end


function CheckTingLayer:onTouchBegan(touch, event)
    
    local p = self.Image_bg:convertToNodeSpace(touch:getLocation())
    local  rect = cc.rect(0, 0, self.Image_bg:getContentSize().width, self.Image_bg:getContentSize().height)
    if cc.rectContainsPoint(rect, p) then

    else
        
        self.gameLayerManager:hideCheckTingLayer()
    end
end

function CheckTingLayer:cloneCardItem(value)
    
    
    local itemNode = cc.CSLoader:createNode("ui_csb/CheckTingItemNode.csb")
    local item = itemNode:getChildByName("Image_card_item")
    local Image_card_Node = item:getChildByName("Image_card")
    local Image_card = Image_card_Node:getChildByName("Image_card")

    local image = Image_card:getChildByName("Image_value")
    image:loadTexture("res/game_res/MJGameBase/ui_res/Mahjong/Big/Big_"..tostring(value)..".png")

    local Text_remain = item:getChildByName("Text_remain")

    local remainNum = 4-self.showcards[value]
    if remainNum < 0 then remainNum = 0 end
    Text_remain:setText(tonumber(remainNum))
    item:setVisible(true)
    return itemNode
end



return CheckTingLayer

--endregion
