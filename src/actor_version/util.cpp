#include "util.h"
#include "log.h"
#include <unistd.h>
#include <fcntl.h>
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
} // namespace ecsfrm
