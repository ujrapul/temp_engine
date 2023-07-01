#include "UT_ComponentData.hpp"
#include "UT_ComponentContainer.hpp"
#include "UT_Math.hpp"

int main()
{
  using namespace Temp;
  
  Logger::logType = Logger::LogType::NOOP;
  Component::UnitTests::Run();
  Component::Container::UnitTests::Run();
  Math::UnitTests::Run();
  
  std::cout << "Unit Tests Passed!" << std::endl;

  return 0;
}
