--region *.lua
--Date
--此文件由[BabeLua]插件自动生成

--module("MJAlgorithm", package.seeall)


local is_can_consist_3x_L_times = 0
local is_can_consist_3_L_times = 0

---------------------------------------------------------------------------------------
--无赖子相关

local MJAlgorithm = class("MJAlgorithm")

--获取当前打哪张牌可以听牌(无赖子)
function MJAlgorithm:getOutTingCards(handcards,isCan7Dui)
    print("getOutTingCards")
    local outTingCards = {}
    local handcardsSingle = self:getSingle(handcards)

    for i,v in pairs (handcardsSingle) do
        local temp = clone(handcards)  
        
        --移除当前选中的牌 
        self:removeOneItemInTable(temp,v)

        --print("getOutTingCards  111")
        --看打出这张牌是否可以听某张牌
        for j = 1,30 do
            if j%10 ~= 0 then
                
                --平胡和七对子检测
                if self:is_can_consist_3x_2(j,temp) then
                    --outTingCards[#outTingCards + 1] = v
                    print("get a ting card " .. v)
                    table.insert(outTingCards,v)
                    break
                end

                if isCan7Dui then 
                    if self:is_can_consist_7dui(j,temp) then
                        --outTingCards[#outTingCards + 1] = v
                        print("get a ting card " .. v)
                        table.insert(outTingCards,v)
                        break
                    end
                end

            end
        end
    end

    return outTingCards
end

--获取当前打哪张牌可以听牌(无赖子,带风牌)
function MJAlgorithm:getOutTingCardsHaveFeng(handcards,isCan7Dui)
    print("getOutTingCards")
    local outTingCards = {}
    local handcardsSingle = self:getSingle(handcards)

    for i,v in pairs (handcardsSingle) do
        local temp = clone(handcards)  
        
        --移除当前选中的牌 
        self:removeOneItemInTable(temp,v)

        --print("getOutTingCards  111")
        --看打出这张牌是否可以听某张牌
        for j = 1,37 do
            if j%10 ~= 0 then
                
                --平胡和七对子检测
                if self:is_can_consist_3x_2(j,temp) then
                    --outTingCards[#outTingCards + 1] = v
                    print("get a ting card " .. v)
                    table.insert(outTingCards,v)
                    break
                end

                if isCan7Dui then 
                    if self:is_can_consist_7dui(j,temp) then
                        --outTingCards[#outTingCards + 1] = v
                        print("get a ting card " .. v)
                        table.insert(outTingCards,v)
                        break
                    end
                end

            end
        end
    end

    return outTingCards
end

--获取所有听得牌(无赖子) [不带风牌]
function MJAlgorithm:getTingCards(handcards,isCan7Dui)
    local tingCards = {}
    for i = 1,30 do
        --遍历所有牌 如果当前的牌和手牌可以组成 3x + 2 的牌型  则可以听这张牌
        if i%10 ~= 0 then


            if isCan7Dui then
                if self:is_can_consist_7dui(i,handcards) or self:is_can_consist_3x_2(i,handcards) then
                    table.insert(tingCards,i)
                end
            else
                if self:is_can_consist_3x_2(i,handcards)  then
                    --tingCards[#tingCards + 1] = i
                    table.insert(tingCards,i)
                    --print("find ting card " .. i)
                end
            end

            
        end
    end

    return tingCards
end

--获取所有听得牌(无赖子) [带风牌]
function MJAlgorithm:getTingCardsHaveFeng(handcards,isCan7Dui)
    local tingCards = {}
    
    for i = 1,37 do
        --遍历所有牌 如果当前的牌和手牌可以组成 3x + 2 的牌型  则可以听这张牌
        if i%10 ~= 0 then
            if isCan7Dui then
                if self:is_can_consist_7dui(i,handcards) or self:is_can_consist_3x_2(i,handcards) then
                    table.insert(tingCards,i)
                end
            else
                if self:is_can_consist_3x_2(i,handcards)  then
                    --tingCards[#tingCards + 1] = i
                    table.insert(tingCards,i)
                    --print("find ting card " .. i)
                end
            end
        end
    end

    return tingCards
end

--手牌是否可以组成 3x + 2 的牌型(无赖子)
function MJAlgorithm:is_can_consist_3x_2(outcard,handcards)
    
    local allcards = clone(handcards)
    allcards[#allcards+1] = outcard

    local size = #allcards
    if size % 3 ~= 2 then
		return false;
    end

    if size == 2 and outcard == handcards[1] then
        return true
    end

    --找出所有的对子
    local duis = self:get_dui_values(allcards)
    --遍历拿出对子的所有情况 看是否能组成3x
    for i,duivalue in pairs (duis) do
        local allcardsTemp = clone(allcards)
        --移除对子
        for j = 1,2  do
            self:removeOneItemInTable(allcardsTemp,duivalue)

        end
        --判断移除对子的牌是否能组成3x
        if self:is_can_consist_3x(allcardsTemp) then
            return true
        end
    end
    --如果所有对子都没能组成 3x_2 则不能听这张牌
    return false

end


--手牌是否可以组成 3x 的牌型(无赖子)
function MJAlgorithm:is_can_consist_3x(handcards)
    
    local size = #handcards;
    if size == 0 then
		return true
    end
    if size % 3 ~= 0 then
		return false;
    end
    --去重后再判断每张手牌是否可以组成顺刻
    local hancardsSingle = self:getSingle(handcards)
    for i,v in pairs (hancardsSingle) do
        --获取顺子或者刻子()
        local isCanGet3,allshunkecards = self:is_can_consist_3(v,handcards)
        if isCanGet3 == false then
            return false
        end
    end
   
    --获取当前手牌中的所有顺子

    local allshunkecards = self:get_all_3(handcards)
    if #allshunkecards*3 < size then
        return false
    else
        
        for l,shunkecards in pairs (allshunkecards) do
                --移除顺子或者刻子
            local temphandcards = clone(handcards)
            for j,movecardvalue in pairs (shunkecards) do
                self:removeOneItemInTable(temphandcards,movecardvalue)

            end
            --递归继续判断
            if  self:is_can_consist_3x(temphandcards) then
                return true
            end
        end
    end

    return false
end



--获取单张牌在手牌(handcards)中组成顺刻的所有情况
--某张牌(value) 是否能在手牌(handcards)中组成顺子或者刻子(无赖子)
function MJAlgorithm:get_3(value,handcards)
    
    --当前手牌能组成三张的所有情况
    local all_3x_cards = {}

    --找刻子
    if self:get_value_num(value,handcards) >= 3 then
        table.insert(all_3x_cards,{value,value,value})
    end

    --找顺子(万条筒)
    if value < 30 then

        --向上找顺子
        if value%10 >= 1 and value%10<=7 then
            if self:get_value_num(value+1,handcards) >= 1 and self:get_value_num(value+2,handcards) >= 1  then
                table.insert(all_3x_cards ,{value,value+1,value+2})
            end
        end

        --左右找顺子
        if value%10 >= 2 and value%10<=8 then
            if self:get_value_num(value-1,handcards) >= 1 and self:get_value_num(value+1,handcards) >= 1  then
                table.insert(all_3x_cards ,{value-1,value,value+1})
            end
        end
        
        --向下找顺子
        if value%10 >= 3 and value%10<=9 then
            if self:get_value_num(value-2,handcards) >= 1 and self:get_value_num(value-1,handcards) >= 1 then
                table.insert(all_3x_cards ,{value-2,value-1,value})
            end
        end
    end

    if #all_3x_cards > 0 then
        return true,all_3x_cards
    else
        return false,nil
    end

end

--获取手牌里所有顺刻情况(无赖子)
function MJAlgorithm:get_all_3(handcards)
    local  allshunkeSinge  = {};

    local handcardsSingle = self:getSingle(handcards)
    for i,v in pairs (handcardsSingle) do
        --获取本张牌能的到的顺刻
        local isCanGet3,allshunkecards = self:get_3(v,handcards)
        if isCanGet3 then
            for i,shunke in pairs (allshunkecards) do
                if self:is_shunke_in_table(shunke,allshunke) == false then
                    table.insert(allshunkeSinge,shunke)
                end
            end
        end
    end

    return allshunkeSinge
end



--是否能组成数字或者刻子(无赖子)
function MJAlgorithm:is_can_consist_3(value,handcards)
    
    --当前手牌能组成三张的所有情况
    --local all_3x_cards = {}

    --找刻子
    if self:get_value_num(value,handcards) >= 3 then
        return true
    end

    --找顺子(万条筒)
    if value < 30 then
        --向上找顺子
        if self:get_value_num(value+1,handcards) >= 1 and self:get_value_num(value+2,handcards) >= 1 and value%10 >= 1 and value%10<=7 then
            return true
        end
        --左右找顺子
        if self:get_value_num(value-1,handcards) >= 1 and self:get_value_num(value+1,handcards) >= 1 and value%10 >= 2 and value%10<=8 then
            return true
        end
        --向下找顺子
        if self:get_value_num(value-2,handcards) >= 1 and self:get_value_num(value-1,handcards) >= 1 and value%10 >= 3 and value%10<=9 then
            return true
        end
    end
    return false
end

--手牌中所有可以组成对子的值的表(无赖子)
function MJAlgorithm:get_dui_values(handcards)
    local values = {}
    for i,v in pairs (handcards) do
        if self:get_value_num(v,handcards) > 1 then
              
              if self:IsInTable(v,values) == false then
                values[#values + 1] = v  
              end
        end
    end
    return values
end

--是否能组成7对(无赖子)
function MJAlgorithm:is_can_consist_7dui(outcard,handcards)

    if (#handcards) ~= 13 then
        return false
    end

    local allcards = clone(handcards)
    allcards[#allcards+1] = outcard

    local size = #allcards
    if size % 2 ~= 0 then
		return false;
    end

    local hancardsSingle = self:getSingle(allcards)
    for i,v in pairs(hancardsSingle) do
        if self:get_value_num(v,allcards) % 2 ~= 0 then
            return false
        end
    end

    return true

end

---------------------------------------------------------------------------------------
--带赖子相关


--获取当前打哪张牌可以听牌(有赖子)  laizis 当前赖子表 isCan7Dui 是否可以胡七对子
function MJAlgorithm:getOutTingCards_L(handcards,laizis,isCan7Dui)
    print("getOutTingCards_L")

    is_can_consist_3x_L_times = 0
    is_can_consist_3_L_times = 0

    local size = #handcards 
    if size%3 ~= 2 then
        return {}
    end

    local handcardsTemp = clone(handcards)
    local laiziNum = 0  --所有赖子个数
    local laiziNumTable = {}--每个赖子有多少
    for i,v in pairs(laizis) do
        laiziNum = laiziNum + self:get_value_num(tonumber(v),handcardsTemp)
        laiziNumTable[i] = self:get_value_num(tonumber(v),handcardsTemp)
    end


    if laiziNum == 0 then
        return self:getOutTingCards(handcardsTemp,isCan7Dui)
    end

    --移除赖子
    for i,v in pairs(laizis) do
        for j = 1,laiziNumTable[i] do
            self:removeOneItemInTable(handcardsTemp,laizis[i])
       end
    end


    local outTingCards = {}
    local handcardsSingle = self:getSingle(handcardsTemp)
    for i,v in pairs (handcardsSingle) do
        print("test move card "..v.."can ting ?")
        local temp = clone(handcardsTemp) 
        self:removeOneItemInTable(temp,v)



        if self:is_can_consist_3x_2_L(-1,temp,laiziNum + 1) then
            print("检测到移除这张牌当前可以听牌")
            table.insert(outTingCards,v)
        elseif isCan7Dui  and self:is_can_consist_7dui_L(-1,temp,laiziNum + 1) then
            print("检测到移除这张牌当前可以听七对子")
            table.insert(outTingCards,v)
        end

      
    end
    print("getOutTingCards_L  end")
    print("is_can_consist_3x_L_times = " .. is_can_consist_3x_L_times)
    print("is_can_consist_3_L_times = " .. is_can_consist_3_L_times)
    return outTingCards
end


--获取带癞子听的牌  handcards 手牌数据 (注意 要传一份克隆的table)
function MJAlgorithm:getTingCards_L(handcards,isCan7Dui)

   local handcardsTemp = clone(handcards)

   is_can_consist_3x_L_times = 0
   is_can_consist_3_L_times = 0

   local allTingCards = {}
   --有几个赖子
   local laiziNum = self:get_value_num(35,handcardsTemp)
   
   if laiziNum > 0 and laiziNum < 4 then
        --移除赖子
        for i = 1,laiziNum do
            self:removeOneItemInTable(handcardsTemp,35)
        end
        for l = 1,30 do
            --遍历所有牌 如果当前的牌和手牌可以组成 3x + 2 的牌型  则可以听这张牌
            if l%10 ~= 0 then

                if self:is_can_consist_3x_2_L(l,handcardsTemp,laiziNum) then
                    table.insert(allTingCards,l)
                elseif self:is_can_consist_7dui_L(l,handcardsTemp,laiziNum) and isCan7Dui then
                    table.insert(allTingCards,l)
                end

            end
        end
   elseif laiziNum == 4 then
        print("laiziNum = 4")
        --四个红中直接胡
        --自摸任意牌均可胡
        return {1,2,3,4,5,6,7,8,9,
                11,12,13,14,15,16,17,18,19,
                21,22,23,24,25,26,27,28,29,}
   elseif laiziNum == 0 then
        --无赖子
        print(" no laizi ")
        return self:getTingCards(handcardsTemp,isCan7Dui)
   end

   --去重

   print("is_can_consist_3x_L_times = " .. is_can_consist_3x_L_times)
   print("is_can_consist_3_L_times = " .. is_can_consist_3_L_times)
   return self:getSingle(allTingCards)

end

--手牌是否可以组成 3x + 2 的牌型(有赖子)
-- outcard 是否可以听这张牌   handcards 去除赖子后的手牌 , laizinum 当前手牌赖子个数
function MJAlgorithm:is_can_consist_3x_2_L(outcard,handcards,laizinum)

    local allcards = clone(handcards)
    if outcard > 0 then
        table.insert(allcards,outcard)
    end
    

    local size = #allcards + laizinum
    if size % 3 ~= 2 then
		return false;
    end

    if size == 2 then
        if outcard == handcards[1]  or laizinum >= 1 then
            return true
        end
    end


    --------------------------------------------------------------
    --第一种算法 遍历对子
    --找出所有的对子
    local duis = self:get_dui_values_L(allcards,laizinum)
    --遍历拿出对子的所有情况 看是否能组成3x
    for i,dui in pairs (duis) do
        local laizinumTemp = clone(laizinum)
        local removeduicards = clone(allcards)
        --移除对子
        for j = 1,2  do
            if dui[j] == 35 then
                --如果对子中有赖子 更新赖子的个数
                laizinumTemp = laizinumTemp - 1 
            else
                self:removeOneItemInTable(removeduicards,dui[j])
            end
        end
        --判断移除对子后的牌是否能组成3x 
        if self:is_can_consist_3x_L(removeduicards,laizinumTemp) then
            return true
        end
    end

    --如果所有对子都没能组成 3x_2 则不能听这张牌
    return false
    --------------------------------------------------------------
    
end

--手牌是否可以组成 3x 的牌型(有赖子)
function MJAlgorithm:is_can_consist_3x_L(handcards,laizinum)
    is_can_consist_3x_L_times = is_can_consist_3x_L_times + 1
    
    local size = #handcards + laizinum;
    if size == 0 then
		return true
    end
    if size % 3 ~= 0 then
		return false;
    end


    --移除纯刻子
    self:removePureKezi(handcards)
    local size_after_remove = #handcards + laizinum;
    if size_after_remove == 0 then
		return true
    end

    local size_after_remove = #handcards + laizinum;
    if size_after_remove == 0 then
		return true
    end

    --去重后再判断每张手牌是否可以组成顺刻
    local handcardsSingle = self:getSingle(handcards)
    for i,v in pairs (handcardsSingle) do
        --获取顺子或者刻子()
        local isCanConsist3,shunke,needLaiziNum = self:is_can_consist_3_L(v,handcards,laizinum)
        if isCanConsist3 == false then
            --如果有哪张牌不能组成顺子或者刻子 返回false
            return false
        else
            --如果当前牌能组成顺子的话  移除这个顺子 继续判断
            local temphandcards = clone(handcards)
            local laizinumTemp = clone(laizinum)
            for i,v in pairs(shunke) do
                self:removeOneItemInTable(temphandcards,v)
            end
            laizinumTemp = laizinumTemp - needLaiziNum
            return self:is_can_consist_3x_L(temphandcards,laizinumTemp)
        end
    end


    return false
end

--某张牌(value) 是否能在手牌(handcards)中组成顺子或者刻子(有赖子)

--返回值  是否能组顺子刻子, 删掉的手牌 , 需要的赖子数量
function MJAlgorithm:is_can_consist_3_L(value,handcards,laizinum)
    is_can_consist_3_L_times =  is_can_consist_3_L_times + 1
    
    --当前手牌能组成三张的所有情况
    --local all_3x_cards = {}

    
    --找顺子(万条筒)
    if value < 30 then

        --向上找顺子
        if value%10 >= 1 and value%10<=7 then
            if self:get_value_num(value+1,handcards) >= 1 and self:get_value_num(value+2,handcards) >= 1  then
                --table.insert(all_3x_cards ,{value,value+1,value+2})
                return true , {value,value+1,value+2} , 0
            end
            if self:get_value_num(value+1,handcards) >= 1 and laizinum >= 1 then
                --table.insert(all_3x_cards ,{value,value+1,35})
                return true , {value,value+1} , 1 
            end
            if laizinum >= 1 and self:get_value_num(value+2,handcards) >= 1  then
                --table.insert(all_3x_cards ,{value,value+2,35})
                return true , {value,value+2,35} , 1
            end
        end

        --左右找顺子
        if value%10 == 8 then
            
            if laizinum >= 1 and self:get_value_num(value+1,handcards) >= 1  then
                --table.insert(all_3x_cards ,{35,value,value+1})
                --table.insert(all_3x_cards ,{value,value+1,35})
                return true , {value,value+1,35} , 1
            end
            if laizinum >= 1 and self:get_value_num(value,handcards) >= 1 then
                 return true , {value,value,35} , 1
            end
        end

      
    end

    --找刻子
    if self:get_value_num(value,handcards) >= 3 then
        return true , {value ,value ,value} , 0
    end

    if self:get_value_num(value,handcards) == 2 and laizinum >= 1 then
        return true , {value,value} , 1
    end

    if self:get_value_num(value,handcards) == 1 and laizinum >= 2 then
        return true , {value} , 2
    end

    return false
end


--获取单张牌在手牌(handcards)中组成顺刻的所有情况
--某张牌(value) 是否能在手牌(handcards)中组成顺子或者刻子(有赖子)
function MJAlgorithm:get_3_L(value,handcards,laizinum)
    
    --当前手牌能组成三张的所有情况
    local all_3x_cards = {}

    --找刻子
    if self:get_value_num(value,handcards) >= 3 then
        table.insert(all_3x_cards,{value,value,value})
    end

    if self:get_value_num(value,handcards) >= 2 and laizinum >= 1 then
        --table.insert(all_3x_cards,{35,value,value})
        table.insert(all_3x_cards,{value,value,35})
    end
   
    if self:get_value_num(value,handcards) >= 1 
    and laizinum >= 2 then
        --table.insert(all_3x_cards,{35,35,value})
        table.insert(all_3x_cards,{value,35,35})
    end

    --找顺子(万条筒)
    if value < 30 then

        --向上找顺子
        if value%10 >= 1 and value%10<=7 then
            if self:get_value_num(value+1,handcards) >= 1 and self:get_value_num(value+2,handcards) >= 1  then
                table.insert(all_3x_cards ,{value,value+1,value+2})
            end
            if self:get_value_num(value+1,handcards) >= 1 and laizinum >= 1 then
                table.insert(all_3x_cards ,{value,value+1,35})
            end
            if laizinum >= 1 and self:get_value_num(value+2,handcards) >= 1  then
                --table.insert(all_3x_cards ,{value,35,value+2})
                table.insert(all_3x_cards ,{value,value+2,35})
            end
        end

        --左右找顺子
        if value%10 >= 2 and value%10<=8 then
            if self:get_value_num(value-1,handcards) >= 1 and self:get_value_num(value+1,handcards) >= 1  then
                table.insert(all_3x_cards ,{value-1,value,value+1})
            end
            if laizinum >= 1 and self:get_value_num(value+1,handcards) >= 1  then
                --table.insert(all_3x_cards ,{35,value,value+1})
                table.insert(all_3x_cards ,{value,value+1,35})
            end
            if self:get_value_num(value-1,handcards) >= 1 and laizinum >= 1  then
                table.insert(all_3x_cards ,{value-1,value,35})
            end
        end
        
        --向下找顺子
        if value%10 >= 3 and value%10<=9 then
            if self:get_value_num(value-2,handcards) >= 1 and self:get_value_num(value-1,handcards) >= 1 then
                table.insert(all_3x_cards ,{value-2,value-1,value})
            end
            if laizinum >= 1 and self:get_value_num(value-1,handcards) >= 1 then
                table.insert(all_3x_cards ,{35,value-1,value})
            end
            if self:get_value_num(value-2,handcards) >= 1 and laizinum >= 1 then
                table.insert(all_3x_cards ,{value-2,35,value})
            end
        end
    end

    if #all_3x_cards > 0 then
        return true,all_3x_cards
    else
        return false,nil
    end

end



--获取手牌里所有顺刻情况
function MJAlgorithm:get_all_3_L(handcards,laizinum)
    local  allshunkeSinge  = {};

    local handcardsSingle = self:getSingle(handcards)
    for i,v in pairs (handcardsSingle) do
        --获取本张牌能的到的顺刻
        local isCanGet3,allshunkecards = self:get_3_L(v,handcards,laizinum)
        if isCanGet3 then
            for i,shunke in pairs (allshunkecards) do
                if self:is_shunke_in_table(shunke,allshunke) == false then
                    table.insert(allshunkeSinge,shunke)
                end
            end
        end
    end

    return allshunkeSinge
end

--是否一个顺刻已经在表中
function MJAlgorithm:is_shunke_in_table(shunke,tbl)
    if tbl == nil or #tbl == 0 then
        return false
    end

    for i,sk in pairs(tbl) do
        if shunke[1] == sk[1] and shunke[2] == sk[2] and shunke[3] == sk[3] then
            return true
        end
    end

    return false
end



--手牌中所有可以组成对子的对子的表(有赖子 返回 {{35,11},{11,11,},{35,35}} 类似这样的对子) 
function MJAlgorithm:get_dui_values_L(handcards,laizinum)
    local allduis = {}
    for i,v in pairs (handcards) do
        if self:get_value_num(v,handcards) > 1 then
              
            --纯对子情况
            if self:is_dui_in_table({v,v},allduis) == false then
                table.insert(allduis,{v,v})
            end
        end
        if self:get_value_num(v,handcards) >= 1 then
              --赖子配对子情况
            if v ~= 35 and laizinum > 0 then
                if self:is_dui_in_table({35,v},allduis) == false then
                    table.insert(allduis,{35,v})
                end
            end
        end
    end

    --赖子做对子
    if laizinum >= 2 then
       table.insert(allduis,{35,35})
    end

    return allduis
end

--某个对子是否在表中
function MJAlgorithm:is_dui_in_table(dui,tbl)
    for i,v in pairs (tbl) do
        if #v >= 2 then
            if dui[1] == v[1] and dui[2] == v[2] then 
                return true
            end
        end
    end
    return false
end

--是否可以组成七对子(带癞子)
function MJAlgorithm:is_can_consist_7dui_L(outcard,handcards,laizinum)
   
    local allcards = clone(handcards)

    if outcard > 0 then
        allcards[#allcards+1] = outcard
    end

    local size = #allcards + laizinum
    if size % 2 ~= 0 then
		return false
    end

    if size~=14 then
        return false
    end
    
    local needLaizi = 0 --需要的赖子数量

    local hancardsSingle = self:getSingle(allcards)

    for i,v in pairs(hancardsSingle) do
        if self:get_value_num(v,allcards)%2 ~= 0 then
            needLaizi = needLaizi + 1
        end
    end
    if needLaizi <= laizinum then
        return true
    else
        return false
    end


end

function MJAlgorithm:removePureShunzi(cards)
    for i= 1,30 do
        if i%10 < 8 then
            if self:get_value_num(i,cards)>0 and get_value_num(i+1,cards)>0 and get_value_num(i+2,cards)>0 then
                self:removeOneItemInTable(cards,i)
                self:removeOneItemInTable(cards,i+1)
                self:removeOneItemInTable(cards,i+2)
                i= i-1
            end
        end
    end
    
end

function MJAlgorithm:removePureKezi(cards)
     for i= 1,30 do
        if self:get_value_num(i,cards)>2 then
            for j=1,3 do
                self:removeOneItemInTable(cards,i)
            end
        end
     end
end

---------------------------------------------------------------------------------------
--通用

--删除某个表中对应的某一个值


function MJAlgorithm:removeOneItemInTable( tbl , item)
    for i,v in pairs (tbl) do
        if item == v then
            table.remove(tbl,i)
            break
        end
    end
end

--表中是否包含某个值
function MJAlgorithm:IsInTable(value, tbl)
    if tbl then
        for k,v in ipairs(tbl) do
          if v == value then
            return true
          end
        end
    end
    return false
end


--获取一张牌在手牌中的数量
function MJAlgorithm:get_value_num(cardvalue,handcards)
    local num = 0
    for i,v in pairs (handcards) do
        if tonumber(v) == tonumber(cardvalue) then
            num = num + 1
        end
    end

    return num
end

--获取一个表去重后的表(不改变原有表)
function MJAlgorithm:getSingle(tbl)
    
    local kv = {}
    for i,v in pairs (tbl) do
        kv[v] = v 
    end

    local result = {}
    for i,v in pairs (kv) do
        table.insert(result,v)
    end

    table.sort(result,function(a,b)
        return a<b
    end)

    return result
end



---------------------------------------------------------------------------------------

--自定义排序

--自定义排序  红中放到前面
function MJAlgorithm:UserSort(cards,laizs)
    
    if cards == nil or #cards == 0 then
        return
    end

    table.sort(cards, function(a, b)

        if laizs == nil or #laizs <= 0 then
            --无赖子情况
            return a<b
        else
            --有赖子情况
            local aIsLaizi = self:IsInTable(a,laizs)
             local bIsLaizi = self:IsInTable(b,laizs)

            if aIsLaizi and bIsLaizi then
                return a<b
            elseif aIsLaizi  then
                return true
            elseif bIsLaizi then
                return false
            else
                return a<b
            end
        end

    end )
end


return MJAlgorithm


---------------------------------------------------------------------------------------

--endregion
