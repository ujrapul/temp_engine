#include "UT_ComponentData.hpp"
#include "UT_ComponentContainer.hpp"
#include "UT_Event.hpp"
#include "UT_Math.hpp"
#include "UT_Hoverable.hpp"
#include "UT_LevelParser.hpp"

int main()
{
  using namespace Temp;
  
  Logger::logType = Logger::LogType::NOOP;
  Component::UnitTests::Run();
  Component::Container::UnitTests::Run();
  Component::Hoverable::UnitTests::Run();
  Event::UnitTests::Run();
  Math::UnitTests::Run();
  Logger::logType = Logger::LogType::COUT;
  LevelParser::UnitTests::Run();
  Logger::logType = Logger::LogType::NOOP;
  
  std::cout << "Unit Tests Passed!" << std::endl;

  return 0;
}
