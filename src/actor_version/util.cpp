#include "util.h"
#include "log.h"
#include <unistd.h>
#include <fcntl.h>
#include <chrono>
namespace ecsfrm
{
    static Logger::ptr logger = LogSystemUtil::RegisterLogger("system");
    void Util::SetNonBlock(SOCKET sock)
    {
        int flag = fcntl(sock, F_GETFL, 0);
        int ret = fcntl(sock, F_SETFL, flag | O_NONBLOCK);
        if (ret < 0)
        {
            LOG_ERROR(logger) << "fcntl set nonblock error fd=" << sock << " error " << strerror(errno);
        }
    }
    uint64_t Util::GetTimeStamp()
    {
        return std::chrono::duration_cast<std::chrono::milliseconds>(
                   std::chrono::system_clock::now().time_since_epoch())
            .count();
    }
    int Util::GetYearDay()
    {
        auto timeValue = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
        auto tt = std::chrono::system_clock::to_time_t(timeValue);
        struct tm *ptm = localtime(&tt);
        return ptm->tm_yday;
    }
} // namespace ecsfrm
