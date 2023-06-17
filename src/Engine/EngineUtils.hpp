#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <limits.h>
#include <unistd.h>

namespace Temp
{
    const std::filesystem::path &ApplicationDirectory();

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