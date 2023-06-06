// #pragma GCC diagnostic push
// #pragma GCC diagnostic ignored "-Wpedantic"

#include "Math.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <filesystem>

namespace Temp::ImageLoader
{
  struct Color3
  {
    union
    {
      struct
      {
        unsigned char r;
        unsigned char g;
        unsigned char b;
      };
      Math::Vec3i data;
    };

    constexpr Color3() : r(0), g(0), b(0) {}

    constexpr int &operator[](int i)
    {
      return (int &)data[i];
    }
  };

  struct Color4
  {
    union
    {
      struct
      {
        unsigned char r;
        unsigned char g;
        unsigned char b;
        unsigned char a;
      };
      Math::Vec4i data;
    };
    constexpr Color4() : r(0), g(0), b(0), a(0) {}

    constexpr int &operator[](int i)
    {
      return (int &)data[i];
    }
  };

  std::vector<Color3> loadJPEG(const std::string &filename, int &width, int &height, int& nrChannels)
  {
    std::ifstream file(filename, std::ios::binary);
    if (!file)
    {
      std::cerr << "Failed to open file: " << filename << std::endl;
      return {};
    }

    // Check SOI marker
    char marker[2];
    file.read(marker, 2);
    if (file.gcount() < 2 || marker[0] != 0xFF || marker[1] != 0xD8)
    {
      std::cerr << "SOI: Invalid JPEG file: " << filename << std::endl;
      return {};
    }

    // Find Start of Frame (SOF) marker
    bool foundSOF = false;
    while (!foundSOF)
    {
      char markerData[4];
      file.read(markerData, 4);
      if (file.gcount() < 4)
      {
        std::cerr << "SOF: Invalid JPEG file: " << filename << std::endl;
        return {};
      }

      unsigned char markerByte1 = static_cast<unsigned char>(markerData[0]);
      unsigned char markerByte2 = static_cast<unsigned char>(markerData[1]);
      unsigned short markerLength = (markerData[2] << 8) | markerData[3];

      if (markerByte1 != 0xFF)
      {
        std::cerr << "MarkerByte1: Invalid JPEG file: " << filename << std::endl;
        return {};
      }

      if (markerByte2 == 0xC0 || markerByte2 == 0xC2)
      { // SOF0 or SOF2 marker
        foundSOF = true;

        unsigned char sofData[6];
        file.read(reinterpret_cast<char *>(sofData), 6);
        if (file.gcount() < 6)
        {
          std::cerr << "MarkerByte2: Invalid JPEG file: " << filename << std::endl;
          return {};
        }

        height = (sofData[1] << 8) | sofData[2];
        width = (sofData[3] << 8) | sofData[4];
        nrChannels = sofData[5] / 8; // Divide by 8 to get the number of channels (bits per component)
      }
      else
      {
        // Skip marker segment
        file.seekg(markerLength - 2, std::ios_base::cur);
      }
    }

    // Calculate number of pixels
    std::size_t numPixels = width * height;

    // Read pixel data
    std::vector<Color3> pixels(numPixels);
    for (std::size_t i = 0; i < numPixels; ++i)
    {
      file.read(reinterpret_cast<char *>(&pixels[i].r), 1);
      file.read(reinterpret_cast<char *>(&pixels[i].g), 1);
      file.read(reinterpret_cast<char *>(&pixels[i].b), 1);
    }

    return pixels;
  }

  inline std::vector<Color4> loadPNG(const std::string &filename, int &width, int &height, int &numChannels)
  {
    std::ifstream file(filename, std::ios::binary);
    if (!file)
    {
      std::cerr << "Failed to open file: " << filename << std::endl;
      return {};
    }

    // Check PNG signature
    char signature[8];
    file.read(signature, 8);
    if (file.gcount() < 8 || signature[0] != 0x89 || signature[1] != 'P' || signature[2] != 'N' ||
        signature[3] != 'G' || signature[4] != 0x0D || signature[5] != 0x0A || signature[6] != 0x1A ||
        signature[7] != 0x0A)
    {
      std::cerr << "Invalid PNG file: " << filename << std::endl;
      return {};
    }

    // Read IHDR chunk
    unsigned char ihdrLengthBytes[4];
    file.read(reinterpret_cast<char *>(ihdrLengthBytes), 4);
    unsigned int ihdrLength = (ihdrLengthBytes[0] << 24) | (ihdrLengthBytes[1] << 16) |
                              (ihdrLengthBytes[2] << 8) | ihdrLengthBytes[3];

    if (ihdrLength != 13)
    {
      std::cerr << "Invalid IHDR chunk in PNG file: " << filename << std::endl;
      return {};
    }

    char ihdrType[4];
    file.read(ihdrType, 4);
    if (file.gcount() < 4 || ihdrType[0] != 'I' || ihdrType[1] != 'H' || ihdrType[2] != 'D' ||
        ihdrType[3] != 'R')
    {
      std::cerr << "Invalid IHDR chunk in PNG file: " << filename << std::endl;
      return {};
    }

    unsigned char ihdrData[13];
    file.read(reinterpret_cast<char *>(ihdrData), 13);

    width = (ihdrData[0] << 24) | (ihdrData[1] << 16) | (ihdrData[2] << 8) | ihdrData[3];
    height = (ihdrData[4] << 24) | (ihdrData[5] << 16) | (ihdrData[6] << 8) | ihdrData[7];
    unsigned char bitDepth = ihdrData[8];
    unsigned char colorType = ihdrData[9];
    unsigned char compressionMethod = ihdrData[10];
    unsigned char filterMethod = ihdrData[11];
    unsigned char interlaceMethod = ihdrData[12];

    if (bitDepth != 8 || compressionMethod != 0 || filterMethod != 0 || interlaceMethod != 0 ||
        (colorType != 2 && colorType != 6))
    {
      std::cerr << "Unsupported PNG format: " << filename << std::endl;
      return {};
    }

    // Calculate number of channels
    numChannels = (colorType == 2) ? 3 : 4;

    // Skip the rest of the IHDR chunk
    unsigned char crc[4];
    file.read(reinterpret_cast<char *>(crc), 4);

    // Read image data
    std::vector<Color4> image(width * height);
    for (int y = 0; y < height; ++y)
    {
      unsigned char filterType;
      file.read(reinterpret_cast<char *>(&filterType), 1);

      for (int x = 0; x < width; ++x)
      {
        Color4 &pixel = image[y * width + x];
        file.read(reinterpret_cast<char *>(&pixel.r), 1);
        file.read(reinterpret_cast<char *>(&pixel.g), 1);
        file.read(reinterpret_cast<char *>(&pixel.b), 1);

        if (colorType == 6)
        {
          file.read(reinterpret_cast<char *>(&pixel.a), 1);
        }
        else
        {
          pixel.a = 255; // Set alpha channel to fully opaque
        }
      }
    }

    return image;
  }
}

// #pragma GCC diagnostic pop
