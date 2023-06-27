#include "EngineUtils.hpp"
#include <mach-o/dyld.h>

namespace Temp
{
  namespace
  {
#ifdef __linux__
    inline std::filesystem::path GetApplicationDirectory()
    {
      char buffer[PATH_MAX];
      ssize_t len = readlink("/proc/self/exe", buffer, sizeof(buffer) - 1);
      if (len == -1)
      {
        std::cout << "FAILED TO READ EXEC DIR!";
        return "";
      }
      
      std::filesystem::path executablePath(buffer);
      std::filesystem::path path = executablePath.parent_path();
      
      return path;
    }
#elif __APPLE__
    inline std::filesystem::path GetApplicationDirectory()
    {
      char executablePath[PATH_MAX];
      uint32_t size = sizeof(executablePath);
      if (_NSGetExecutablePath(executablePath, &size) == 0)
      {
        std::filesystem::path executableDir(executablePath);
        return executableDir.parent_path();
      }
      return std::filesystem::path(); // Return an empty path if failed to retrieve the directory
    }
#endif
    std::filesystem::path applicationDirectory{};
  }
  
  const std::filesystem::path& ApplicationDirectory()
  {
    static std::filesystem::path applicationDirectory{GetApplicationDirectory()};
    return applicationDirectory;
  }
}
