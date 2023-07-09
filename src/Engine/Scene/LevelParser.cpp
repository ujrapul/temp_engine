#include "LevelParser.hpp"
#include "TextBox.hpp"
#include "TextButton.hpp"
#include "Logger.hpp"
#include <string_view>
#include <cstring>

namespace Temp::LevelParser
{
  namespace
  {
    // Pass SceneObjectMap
    struct GlobalData
    {
      Scene::Data &scene;
      std::istringstream &f;
      std::string &line;
      int &lineNumber;
    };

    void AddSceneObject(SceneObject::Data object, SceneObject::Type::Type type, GlobalData &data)
    {
      std::string name = object.name;
      object.type = type;
      data.scene.objects.push_back(object);
      data.scene.objectsNameIdxTable[name] = (int)data.scene.objects.size() - 1;
    }

    bool GetLine(std::istringstream &f, std::string &l, int &lnum)
    {
      ++lnum;
      return std::getline(f, l) ? true : false;
    }

    void ReadFile(std::string &contents, const std::string &path)
    {
      std::ifstream in(path, std::ios::in | std::ios::binary);
      if (in)
      {
        in.seekg(0, std::ios::end);
        contents.resize(in.tellg());
        in.seekg(0, std::ios::beg);
        in.read(&contents[0], contents.size());
        in.close();
      }
      else
      {
        throw(errno);
      }
    }

    bool IgnoreLine(const std::string_view line)
    {
      return line.starts_with("//") || line == "";
    }

    std::vector<std::string_view> SplitString(const std::string_view str, const char *delimiter)
    {
      std::vector<std::string_view> tokens;
      std::size_t start = 0;
      std::size_t end = str.find(delimiter);

      while (end != std::string_view::npos)
      {
        tokens.push_back(str.substr(start, end - start));
        start = end + strlen(delimiter);
        end = str.find(delimiter, start);
      }

      tokens.push_back(str.substr(start));

      return tokens;
    }

    bool ParseTextBox(TextBox::Data &textBox, std::string &name, GlobalData &data)
    {
      while (GetLine(data.f, data.line, data.lineNumber) && data.line.find("}") == std::string::npos)
      {
        if (IgnoreLine(data.line))
        {
          continue;
        }
        auto tokens = SplitString(data.line, ": ");
        if (tokens.size() != 2)
        {
          Logger::Log("Invalid delimiter at line " + std::to_string(data.lineNumber) + ": " + data.line);
          return false;
        }
        if (tokens[0].find("Name") != std::string::npos)
        {
          name = tokens[1];
        }
        else if (tokens[0].find("Position") != std::string::npos)
        {
          auto position = SplitString(tokens[1], " ");
          if (position.size() != 2)
          {
            Logger::Log("Invalid position at line " + std::to_string(data.lineNumber) + ": " + data.line);
            return false;
          }
          textBox.x = std::strtof(position[0].data(), nullptr);
          textBox.y = std::strtof(position[1].data(), nullptr);
        }
        else if (tokens[0].find("Scale") != std::string::npos)
        {
          textBox.scale = std::strtof(tokens[1].data(), nullptr);
        }
        else if (tokens[0].find("Text") != std::string::npos)
        {
          textBox.text = tokens[1];
        }
      }
      return true;
    }

    bool ParseHoverable(Component::Hoverable::Data &hoverable, GlobalData &data)
    {
      while (GetLine(data.f, data.line, data.lineNumber) && data.line.find("}") == std::string::npos)
      {
        if (IgnoreLine(data.line))
        {
          continue;
        }
        auto tokens = SplitString(data.line, ": ");
        if (tokens.size() != 2)
        {
          Logger::Log("Invalid delimiter at line " + std::to_string(data.lineNumber) + ": " + data.line);
          return false;
        }
        if (tokens[0].find("Position") != std::string::npos)
        {
          auto position = SplitString(tokens[1], " ");
          if (position.size() != 2)
          {
            Logger::Log("Invalid position at line " + std::to_string(data.lineNumber) + ": " + data.line);
            return false;
          }
          hoverable.x = std::strtof(position[0].data(), nullptr);
          hoverable.y = std::strtof(position[1].data(), nullptr);
        }
        else if (tokens[0].find("Size") != std::string::npos)
        {
          auto size = SplitString(tokens[1], " ");
          if (size.size() != 2)
          {
            Logger::Log("Invalid size at line " + std::to_string(data.lineNumber) + ": " + data.line);
            return false;
          }
          hoverable.width = std::strtof(size[0].data(), nullptr);
          hoverable.height = std::strtof(size[1].data(), nullptr);
        }
      }
      return true;
    }

    bool ParseTextBox(GlobalData &data)
    {
      SceneObject::Data object;
      if (GetLine(data.f, data.line, data.lineNumber) && data.line.find("{") != std::string::npos)
      {
        auto *textBox = new TextBox::Data();
        if (!ParseTextBox(*textBox, object.name, data))
        {
          delete textBox;
          return false;
        }
        object.data = textBox;
      }
      AddSceneObject(object, SceneObject::Type::TEXTBOX, data);
      return true;
    }

    bool ParseTextButton(GlobalData &data)
    {
      SceneObject::Data object;
      if (GetLine(data.f, data.line, data.lineNumber) && data.line.find("{") != std::string::npos)
      {
        auto *textButton = new TextButton::Data();
        while (GetLine(data.f, data.line, data.lineNumber) && data.line.find("}") == std::string::npos)
        {
          if (IgnoreLine(data.line))
          {
            continue;
          }
          auto tokens = SplitString(data.line, ": ");
          if (tokens[0].find("Name") != std::string::npos)
          {
            object.name = tokens[1];
          }
          else if (tokens[0].find("TextBox") != std::string::npos)
          {
            if (GetLine(data.f, data.line, data.lineNumber) && data.line.find("{") != std::string::npos)
            {
              std::string name;
              TextBox::Data textBox;
              if (!ParseTextBox(textBox, name, data))
              {
                delete textButton;
                return false;
              }
              textButton->textBox = textBox;
            }
          }
          else if (tokens[0].find("Hoverable") != std::string::npos)
          {
            if (GetLine(data.f, data.line, data.lineNumber) && data.line.find("{") != std::string::npos)
            {
              auto* hoverable = new Component::Hoverable::Data();
              if (!ParseHoverable(*hoverable, data))
              {
                delete textButton;
                return false;
              }
              object.constructData = hoverable;
            }
          }
        }
        object.data = textButton;
      }
      AddSceneObject(object, SceneObject::Type::TEXTBUTTON, data);
      return true;
    }
  }

  bool Parse(Scene::Data &scene, const std::string &file)
  {
    int lineNumber = 0;
    scene.objects.clear();
    scene.objectsNameIdxTable.clear();
    auto path = AssetsDirectory() / "Levels" / file;
    std::string contents;
    ReadFile(contents, path);
    std::istringstream f(contents);
    std::string line;
    line.reserve(4096);
    GlobalData data{scene, f, line, lineNumber};
    while (GetLine(f, line, lineNumber))
    {
      // std::cout << line << std::endl;
      if (IgnoreLine(line))
      {
        continue;
      }
      if (parseTable.find(line) == parseTable.end())
      {
        Logger::Log("Invalid object name at line " + std::to_string(lineNumber) + ": " + line);
        return false;
      }
      switch (parseTable.at(line))
      {
      case SceneObject::Type::TEXTBOX:
        if (!ParseTextBox(data))
        {
          return false;
        }
        break;
      case SceneObject::Type::TEXTBUTTON:
        if (!ParseTextButton(data))
        {
          return false;
        }
      default:
        break;
      }
    }
    return true;
  }
}
