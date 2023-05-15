#pragma once

#include "Engine/Scene.hpp"
#include "Engine/Input.hpp"
#include <unordered_set>

namespace Game
{
  namespace Component
  {
    namespace Type
    {
      const uint8_t VALUE             = Temp::Component::Type::MAX;
      const uint8_t COLLECTED_VALUE   = Temp::Component::Type::MAX + 1;
      const uint8_t SCORE             = Temp::Component::Type::MAX + 2;
      const uint8_t MAX               = 255;
    }
    
    template <uint8_t> struct MapToComponentDataType_t;
    template <> struct MapToComponentDataType_t<Type::VALUE> { using type = char; };
    template <> struct MapToComponentDataType_t<Type::COLLECTED_VALUE> {
      using type = std::vector<MapToComponentDataType_t<Type::VALUE>::type>;
    };
    template <> struct MapToComponentDataType_t<Type::SCORE> { using type = int; };
    
    template <uint8_t T>
    using MapToComponentDataType = typename MapToComponentDataType_t<T>::type;
    
    namespace Container
    {
      void Init(Temp::Component::Container::Data& data)
      {
        Temp::Component::Container::Init<Type::VALUE, MapToComponentDataType<Type::VALUE>>(data);
        Temp::Component::Container::Init<Type::COLLECTED_VALUE, MapToComponentDataType<Type::COLLECTED_VALUE>>(data);
        Temp::Component::Container::Init<Type::SCORE, MapToComponentDataType<Type::SCORE>>(data);
      }
      
      void Destruct(Temp::Component::Container::Data& data)
      {
        delete Temp::Component::Container::GetComponentArray<Type::VALUE, MapToComponentDataType<Type::VALUE>>(data);
        delete Temp::Component::Container::GetComponentArray<Type::COLLECTED_VALUE, MapToComponentDataType<Type::COLLECTED_VALUE>>(data);
        delete Temp::Component::Container::GetComponentArray<Type::SCORE, MapToComponentDataType<Type::SCORE>>(data);
      }
    }
  }
  
  namespace Coordinator
  {
    void Init(Temp::Coordinator::Data& data)
    {
      Temp::Coordinator::Init(data);
      Component::Container::Init(data.componentData);
    }
  }
}

namespace Game
{
  namespace Scene
  {
    namespace Player
    {
      struct Data
      {
        Temp::Entity entity{};
      };
      
      void addScore(Temp::Entity player, Temp::Scene::Data* data, int value)
      {
        Temp::Component::Container::Get<Game::Component::Type::SCORE, Component::MapToComponentDataType<Game::Component::Type::SCORE>>
          (data->coordinator.componentData, player) += value;
      }
      
      int getScore(Temp::Entity player, Temp::Scene::Data* data)
      {
        return Temp::Component::Container::Get<Game::Component::Type::SCORE, Component::MapToComponentDataType<Game::Component::Type::SCORE>>
          (data->coordinator.componentData, player);
      }
    }
    
    namespace GameLevel
    {
      using namespace Temp;
      
      struct Data
      {
        std::array<Player::Data, 2> players{};
        std::unordered_set<int> numbersUsed{};
        std::array<int, 10> numbersUnused{0,1,2,3,4,5,6,7,8,9};
        int currentNumber{-1};
        int numbersLeft{10};
        bool player1Turn{true};
        bool acceptInput{true};
        std::mutex mutex;
      };
      
      // TODO: Currently a global but we'll hide this in .cpp file later
      Data gameData{};
      
      const int BOARD_WIDTH = 50;
      
      void inputCallback(int keyCode)
      {
        std::scoped_lock<std::mutex>(gameData.mutex);
        if (!gameData.acceptInput)
          return;
        
        std::string out = Temp::Input::convertKeyCode(keyCode, false, false);
        try {
          gameData.currentNumber = stoi(out);
        } catch (std::exception& e) {
          gameData.currentNumber = -1;
        }
        gameData.acceptInput = false;
      }
      
      void UpdateNumbers(Temp::Scene::Data* sceneData, Entity player, int currentValue)
      {
        for (const auto& entity : sceneData->entities) {
          Math::Vec2 pos = Temp::Component::Container::Get<Temp::Component::Type::POSITION2D, Temp::Component::MapToComponentDataType<Temp::Component::Type::POSITION2D>>
            (sceneData->coordinator.componentData, entity);
          char& value = Temp::Component::Container::Get<Game::Component::Type::VALUE, Component::MapToComponentDataType<Game::Component::Type::VALUE>>
            (sceneData->coordinator.componentData, entity);
          if (value - '0' == currentValue) {
            value = '-';
            Player::addScore(player, sceneData, 1);
          }
          if (pos.y == BOARD_WIDTH - 1) {
//            std::cout << std::endl;
            if (pos.x == BOARD_WIDTH - 1) {
              break;
            }
          }
        }
      }
      
      void PrintBoard(Temp::Scene::Data* data, float deltaTime)
      {
        static float currTime = 0;
        currTime += deltaTime;
//        index %= data->coordinator.componentData.positions.mapping.size;
        if (currTime < 0.0166666f) {
          return;
        }
        currTime = 0;
        
        system("clear");

        std::cout << std::endl;
        for (const auto& entity : data->entities) {
          Math::Vec2 pos = Temp::Component::Container::Get<Temp::Component::Type::POSITION2D, Temp::Component::MapToComponentDataType<Temp::Component::Type::POSITION2D>>
            (data->coordinator.componentData, entity);
          std::cout << Temp::Component::Container::Get<Game::Component::Type::VALUE, Component::MapToComponentDataType<Game::Component::Type::VALUE>>
            (data->coordinator.componentData, entity)
                    << " ";
          if (pos.y == BOARD_WIDTH - 1) {
            std::cout << std::endl;
            if (pos.x == BOARD_WIDTH - 1) {
              break;
            }
          }
        }
        
        std::scoped_lock<std::mutex>(gameData.mutex);
        std::cout << "Player 1's numbers: ";
        for (auto num : Temp::Component::Container::Get<Component::Type::COLLECTED_VALUE, Component::MapToComponentDataType<Component::Type::COLLECTED_VALUE>>
             (data->coordinator.componentData, gameData.players[0].entity)) {
          std::cout << num << " ";
        }
        std::cout << std::endl;
        
        std::cout << "Player 2's numbers: ";
        for (auto num : Temp::Component::Container::Get<Component::Type::COLLECTED_VALUE, Component::MapToComponentDataType<Component::Type::COLLECTED_VALUE>>
             (data->coordinator.componentData, gameData.players[1].entity)) {
          std::cout << num << " ";
        }
        std::cout << std::endl << std::endl;
        
        std::cout << "Numbers left: ";
        for (auto num : gameData.numbersUnused) {
          if (num >= 0)
            std::cout << num << " ";
        }
        std::cout << std::endl << std::endl;
        
        if (gameData.numbersLeft == 0) {
          std::cout << "Player 1 Score: " << Player::getScore(gameData.players[0].entity, data) << std::endl;
          std::cout << "Player 2 Score: " << Player::getScore(gameData.players[1].entity, data) << std::endl;
          std::cout << std::endl;
          if (Player::getScore(gameData.players[0].entity, data) >
              Player::getScore(gameData.players[1].entity, data)) {
            std::cout << "PLAYER 1 WINS!" << std::endl;
          } else if (Player::getScore(gameData.players[0].entity, data) <
                     Player::getScore(gameData.players[1].entity, data)) {
            std::cout << "PLAYER 2 WINS!" << std::endl;
          } else {
            std::cout << "PLAYERS TIED!" << std::endl;
          }
          exit(0);
        }
        
        if (gameData.player1Turn) {
          std::cout << "Player 1 choose a number" << std::endl;
        } else {
          std::cout << "Player 2 choose a number" << std::endl;
        }
        gameData.acceptInput = true;
        if (gameData.currentNumber >= 0 && gameData.numbersUsed.find(gameData.currentNumber) == gameData.numbersUsed.end()) {
          if (gameData.player1Turn) {
            Temp::Component::Container::Get<Component::Type::COLLECTED_VALUE, Component::MapToComponentDataType<Component::Type::COLLECTED_VALUE>>
              (data->coordinator.componentData,
               gameData.players[0].entity).push_back(gameData.currentNumber + '0');
            UpdateNumbers(data, gameData.players[0].entity, gameData.currentNumber);
          } else {
            Temp::Component::Container::Get<Component::Type::COLLECTED_VALUE, Component::MapToComponentDataType<Component::Type::COLLECTED_VALUE>>
              (data->coordinator.componentData,
               gameData.players[1].entity).push_back(gameData.currentNumber + '0');
            UpdateNumbers(data, gameData.players[1].entity, gameData.currentNumber);
          }
          --gameData.numbersLeft;
          gameData.player1Turn = !gameData.player1Turn;
          gameData.numbersUsed.insert(gameData.currentNumber);
          gameData.numbersUnused[gameData.currentNumber] = -1;
          gameData.currentNumber = -1;
        }
      }
      
      void Construct(Temp::Scene::Data* data)
      {
        int count = 0;
        int row = 0;
        int column = 0;
        srand(static_cast<uint>(time(NULL)));
        for (auto& entity : data->entities) {
          if (count >= BOARD_WIDTH * BOARD_WIDTH) {
            break;
          }
          entity = Temp::Coordinator::CreateEntity(data->coordinator);
          Temp::Coordinator::AddComponent<Temp::Component::Type::POSITION2D, Temp::Component::MapToComponentDataType<Temp::Component::Type::POSITION2D>>
            (data->coordinator,
             entity,
             Math::Vec2{(float)row, (float)column});
          Temp::Coordinator::AddComponent<Component::Type::VALUE, Component::MapToComponentDataType<Component::Type::VALUE>>
            (data->coordinator,
             entity,
             '0' + rand() % 10);
          
          column = ++column % BOARD_WIDTH;
          row = column == 0 ? row + 1 : row;
          ++count;
        }
0        Entity Player1 = count++;
        data->entities[Player1] = Temp::Coordinator::CreateEntity(data->coordinator);
        Temp::Coordinator::AddComponent<Component::Type::COLLECTED_VALUE, Component::MapToComponentDataType<Component::Type::COLLECTED_VALUE>>
          (data->coordinator,
           Player1,
           {});
        Temp::Coordinator::AddComponent<Component::Type::SCORE, Component::MapToComponentDataType<Component::Type::SCORE>>
          (data->coordinator,
           Player1,
           {});
        gameData.players[0].entity = Player1;
        
        Entity Player2 = count++;
        data->entities[Player2] = Temp::Coordinator::CreateEntity(data->coordinator);
        Temp::Coordinator::AddComponent<Component::Type::COLLECTED_VALUE, Component::MapToComponentDataType<Component::Type::COLLECTED_VALUE>>
          (data->coordinator,
           Player2,
           {});
        Temp::Coordinator::AddComponent<Component::Type::SCORE, Component::MapToComponentDataType<Component::Type::SCORE>>
          (data->coordinator,
           Player2,
           {});
        gameData.players[1].entity = Player2;
      }
      
      void Update(Temp::Scene::Data* data, float deltaTime)
      {
        PrintBoard(data, deltaTime);
      }
      
      void Destruct(Temp::Scene::Data* data)
      {
        for (auto& entity : data->entities) {
          Temp::Coordinator::DestroyEntity(data->coordinator, entity);
          entity = 0;
        }
        Component::Container::Destruct(data->coordinator.componentData);
      }
      
      Temp::Scene::Data* Create(Temp::Input::KeyEventData& keyEventData)
      {
        Temp::Scene::Data* scene = new Temp::Scene::Data();
        Coordinator::Init(scene->coordinator);
        scene->Construct = Construct;
        scene->Update = Update;
        scene->Destruct = Destruct;
        
        for (int keyCode = 18; keyCode <= 23; ++keyCode) {
          Temp::Input::AddCallback(inputCallback, keyEventData, keyCode);
        }
        Temp::Input::AddCallback(inputCallback, keyEventData, 25);
        Temp::Input::AddCallback(inputCallback, keyEventData, 26);
        Temp::Input::AddCallback(inputCallback, keyEventData, 28);
        Temp::Input::AddCallback(inputCallback, keyEventData, 29); //0
        return scene;
      }
    }
  }
}
