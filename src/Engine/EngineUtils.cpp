#include "EngineUtils.hpp"

namespace Temp
{
  namespace
  {
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

    std::filesystem::path applicationDirectory{};
  }

  const std::filesystem::path& ApplicationDirectory()
  {
    static std::filesystem::path applicationDirectory{GetApplicationDirectory()};
    return applicationDirectory;
  }
}