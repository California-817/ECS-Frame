print("[lua] main lua called")
local engine= EcsEngine(100)
engine:LogInfo("lua call c++ interface success")

--具体的业务id对应的函数
function Msg1()
    
end
function Msg2()
    
end

---------------------------------------------------------------

--初始化函数
function Init()

end
--帧更新函数
function Update()
    
end
--注册消息函数
function RegisterMsgFunc()
    
end
--销毁函数
function Dispose()

end