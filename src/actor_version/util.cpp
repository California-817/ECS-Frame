#include "util.h"
#include "log.h"
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
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
    std::vector<std::string> Util::GetFilesBySuffix(const std::string &path, const std::string &suffix)
    {
        std::vector<std::string> files;

        DIR *dir = opendir(path.c_str());
        if (dir == nullptr)
        {
            LOG_ERROR(logger) << "Failed to open directory: " << path;
            files.push_back(std::string("Failed to open directory: ") + path);
            return files;
        }

        struct dirent *entry;
        while ((entry = readdir(dir)) != nullptr)
        {
            std::string filename = entry->d_name;

            if (filename == "." || filename == "..")
            {
                continue;
            }

            std::string fullPath = path + "/" + filename;

            struct stat fileInfo;
            if (stat(fullPath.c_str(), &fileInfo) == 0 && S_ISREG(fileInfo.st_mode))
            {
                if (filename.length() >= suffix.length() &&
                    filename.substr(filename.length() - suffix.length()) == suffix)
                {
                    files.push_back(fullPath);
                }
            }
        }

        closedir(dir);
        return files;
    }
} // namespace ecsfrm
