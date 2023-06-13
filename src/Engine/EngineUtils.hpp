#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <limits.h>
#include <unistd.h>

namespace Temp
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

    static const std::filesystem::path ApplicationDirectory = GetApplicationDirectory();

    inline const char *LoadFileAsString(const char *filePath)
    {
        std::ifstream fileStream(filePath, std::ios::ate | std::ios::binary);
        if (!fileStream)
        {
            std::cerr << "Failed to open file: " << filePath << std::endl;
            return nullptr;
        }

        std::streamsize fileSize = fileStream.tellg();
        fileStream.seekg(0, std::ios::beg);

        // Allocate memory for the file content
        char *charArray = new char[fileSize + 1];
        fileStream.read(charArray, fileSize);
        charArray[fileSize] = '\0';

        return charArray;
    }
}