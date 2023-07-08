#pragma once

#include "LevelParser.hpp"
#include "TextBox.hpp"
#include "TextButton.hpp"
#include "Math.hpp"

namespace Temp::LevelParser::UnitTests
{
  void Run()
  {
    Scene::Data scene;
    Assert("Test Level Parsing", Parse(scene, "Test.level"));

    auto& objects = scene.objects;
    auto& table = scene.objectsNameTable;

    Assert("Test Level Parsing objects size after Parse", objects.size() == 2);
    auto* object0 = static_cast<TextBox::Data*>(objects[0].data);
    auto* object1 = static_cast<TextButton::Data*>(objects[1].data);
    auto &subObject10 = object1->textBox;
    auto &constructData1 = *static_cast<Component::Hoverable::Data*>(objects[1].constructData);
    for (auto& [k,v] : table)
    {
      std::cout << "TEST: " << v->name << std::endl;
    }
    Assert("Test Level Parsing object 1 valid", objects[0].type == SceneObject::Type::TEXTBOX
                                                && objects[0].name == "NumberGameTextBox"
                                                && object0->text == "NumberGame"
                                                && object0->x == 0
                                                && object0->y == 40
                                                && Math::Abs(object0->scale - 0.04) < Math::EPSILON
                                                && table.at(objects[0].name)->data == objects[0].data
                                                );
    Assert("Test Level Parsing object 2 valid", objects[1].type == SceneObject::Type::TEXTBUTTON
                                                && objects[1].name == "NumberGameTextButton"
                                                && subObject10.text == "NumberGame1"
                                                && subObject10.x == -678
                                                && subObject10.y == 50
                                                && Math::Abs(subObject10.scale - 0.0345) < Math::EPSILON
                                                && constructData1.x == -20
                                                && constructData1.y == 0
                                                && constructData1.width == 9
                                                && constructData1.height == 4
                                                && table.at(objects[1].name)->data == objects[1].data
                                                );
  }
}