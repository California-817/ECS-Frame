--具体的业务id对应的函数
function Msg1()

end

function Msg2()

end

---------------------------------------------------------------

--初始化函数
function Init()
    ThreadMgr.newLuaService("../../service/func1.lua")
    Log.info("[lua] main service Init")
    return true
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
