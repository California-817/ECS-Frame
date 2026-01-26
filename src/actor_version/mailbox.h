#ifndef __ESC_FRAME_MAILBOX_H__
#define __ESC_FRAME_MAILBOX_H__
#include <list>
#include <mutex>
#include<unordered_map>
#include <functional>
#include "packet.h"
namespace ecsfrm
{
    /// @brief 每个actor的邮箱类
    class MailBox
    {
    public:
        /// @brief 消息处理函数
        typedef std::function<int(Packet *)> MsgFunc;
        virtual ~MailBox() = default;
        /// @brief 添加一个包
        /// @param packet
        void AddPacket(Packet *packet);
        /// @brief 注册id对应的消息处理函数
        /// @param msgid
        /// @param func
        void RegisterMsgFunc(int msgid, MsgFunc func);
        /// @brief 判断id是否是该MailBox所关注的消息id
        /// @param id
        /// @return bool
        bool IsFollowMsgId(int id) const { return _id2func_map.find(id) != _id2func_map.end(); }
        /// @brief 处理mailbox中所有消息--执行注册的回调函数
        void ProcessPacket();
    protected:
        std::list<Packet *> _mail_list;
        std::mutex _mail_mutex;
        std::unordered_map<int, MsgFunc> _id2func_map;
    };
} // namespace ecsfrm

#endif