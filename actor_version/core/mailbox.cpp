#include "mailbox.h"
#include "macro.h"
#include "log.h"
#include "thread_mgr.h"
namespace ecsfrm
{
    static Logger::ptr g_logger = LOG_REGISTER("system");
    void MailBox::AddPacket(Packet *packet)
    {
        LOCK_GUARD(lock, _mail_mutex);
        _mail_list.push_back(packet);
    }
    void MailBox::RegisterMsgFunc(int msgid, MsgFunc func)
    {
        _id2func_map[msgid] = func;
    }
    void MailBox::ProcessPacket()
    {
        std::list<Packet *> copy_list;
        {
            LOCK_GUARD(lock, _mail_mutex);
            _mail_list.swap(copy_list);
        }
        for (auto &packet : copy_list)
        {
            if (_id2func_map.find(packet->GetMsgId()) == _id2func_map.end())
            {
                LOG_WARN(g_logger) << "handle packet failed, no register func msgid=" << packet->GetMsgId();
                continue;
            }
            int ret = _id2func_map[packet->GetMsgId()](packet); // 执行回调函数
            if (!ret)
            {
                LOG_WARN(g_logger) << "handle func error which msgid=" << packet->GetMsgId() << " ,ret=" << ret;
            }
            // packet好像在这里并不需要销毁 todo 当前会内存泄漏
        }
        copy_list.clear();
    }
    void MailBox::DispatchPacket(Packet *packet)
    {
        ThreadMgr::GetInstance()->DispatchPacket(packet);
    }
    void MailBox::SendNetPacket(Packet *packet)
    {
        ThreadMgr::GetInstance()->SendNetPacket(packet);
    }
} // namespace ecsfrm
