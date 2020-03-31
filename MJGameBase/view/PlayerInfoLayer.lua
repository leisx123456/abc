--region *.lua
--Date
--此文件由[BabeLua]插件自动生成


local PlayerInfoLayer = class("PlayerInfoLayer",function() 
        return cc.CSLoader:createNode("ui_csb/PlayerInfoLayer.csb") 
    end)

function PlayerInfoLayer:ctor(userInfo,parent)
    
    self.parent = parent
    local Panel_user_info = self:getChildByName("Panel_user_info")

    local Image_bg = Panel_user_info:getChildByName("Image_bg")
    self.Image_bg = Image_bg


    local Text_Nick = Panel_user_info:getChildByName("Text_Nick")
    local Text_ID = Panel_user_info:getChildByName("Text_ID")
    local Text_IP = Panel_user_info:getChildByName("Text_IP")
    
    Text_Nick:setText(api_get_ascll_sub_str_by_ui(userInfo.nickName,16))
    Text_ID:setText(userInfo.dwUserID)
    Text_IP:setText(userInfo.dwUserIP)




    local touch_listener = cc.EventListenerTouchOneByOne:create()
    touch_listener:setSwallowTouches(false) 
    touch_listener:registerScriptHandler(handler(self, self.onTouchBegan), cc.Handler.EVENT_TOUCH_BEGAN)
    local eventDispatcher = self:getEventDispatcher()      
    eventDispatcher:addEventListenerWithSceneGraphPriority(touch_listener, self.Image_bg) 

end


function PlayerInfoLayer:onTouchBegan(touch, event)
    
    local p = self.Image_bg:convertToNodeSpace(touch:getLocation())
    local  rect = cc.rect(0, 0, self.Image_bg:getContentSize().width, self.Image_bg:getContentSize().height)
    if cc.rectContainsPoint(rect, p) then

    else
        self.parent:hidePlayerInfoLayer()
    end
end


return PlayerInfoLayer

--endregion
