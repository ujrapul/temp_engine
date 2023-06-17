#include "GameLevel.hpp"

#include "Camera.hpp"
#include "Components/ComponentType.hpp"
#include "Coordinator.hpp"
#include "Grid.hpp"
#include "Scene.hpp"
#include "TextBox.hpp"
#include <unordered_set>

namespace Game::Scene::GameLevel
{
  namespace
  {
    struct Data
    {
      std::array<Player::Data, 2> players{};
      std::unordered_set<int> numbersUsed{};
      std::array<int, 10> numbersUnused{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
      Grid::Data grid{};
      TextBox::Data player1NumbersTextBox{"Player 1's numbers", -1, -0.8, 0.0006f, (Entity)-1};
      TextBox::Data player1Numbers2TextBox{"1's numbers", -1.5, 0, 0.0006f, (Entity)-1};
      TextBox::Data player2NumbersTextBox{"Player 2's numbers", -1, -0.9, 0.0006f, (Entity)-1};
      TextBox::Data player2Numbers2TextBox{"2's numbers", 1, -0, 0.0006f, (Entity)-1};
      int currentNumber{-1};
      int numbersLeft{10};
      bool player1Turn{true};
      bool acceptInput{true};
    };

    Data gameData{};

    constexpr int BOARD_WIDTH = 50;

    void inputCallback(Temp::Input::KeyboardCode keyCode)
    {
      if (!gameData.acceptInput)
        return;

      std::string out = Temp::Input::convertKeyCode(keyCode, false, false);
      try
      {
        gameData.currentNumber = stoi(out);
      }
      catch (std::exception &e)
      {
        gameData.currentNumber = -1;
      }
      gameData.acceptInput = false;
    }

    void UpdateText(Temp::Scene::Data *sceneData, int currentValue)
    {
      std::string appendText = std::to_string(currentValue);
      if (gameData.numbersLeft > 2)
      {
        appendText += ", ";
      }
      if (gameData.player1Turn)
      {
        std::cout << gameData.player1NumbersTextBox.text + (currentValue + ", ") << std::endl;
        TextBox::UpdateText(sceneData,
                            &gameData.player1NumbersTextBox,
                            gameData.player1NumbersTextBox.text + appendText);
      }
      else
      {
        TextBox::UpdateText(sceneData,
                            &gameData.player2NumbersTextBox,
                            gameData.player2NumbersTextBox.text + appendText);
      }
    }

    void UpdateNumbers(Temp::Scene::Data *sceneData, Entity player, int currentValue)
    {
      Grid::UpdateNumbers(sceneData, &gameData.grid, player, currentValue);
      UpdateText(sceneData, currentValue);
    }

    bool PrintBoard(Temp::Scene::Data *data, float deltaTime)
    {
      static float currTime = 0;
      currTime += deltaTime;
      //        index %= data->coordinator.componentData.positions.mapping.size;
      if (currTime < 0.0166666f)
      {
        return false;
      }
      currTime = 0;

      system("clear");

      std::cout << "Numbers left: ";
      for (auto num : gameData.numbersUnused)
      {
        if (num >= 0)
          std::cout << num << " ";
      }
      std::cout << std::endl
                << std::endl;

      if (gameData.numbersLeft == 0)
      {
        std::cout << "Player 1 Score: " << Player::getScore(gameData.players[0].entity, data) << std::endl;
        std::cout << "Player 2 Score: " << Player::getScore(gameData.players[1].entity, data) << std::endl;
        std::cout << std::endl;
        if (Player::getScore(gameData.players[0].entity, data) >
            Player::getScore(gameData.players[1].entity, data))
        {
          std::cout << "PLAYER 1 WINS!" << std::endl;
        }
        else if (Player::getScore(gameData.players[0].entity, data) <
                 Player::getScore(gameData.players[1].entity, data))
        {
          std::cout << "PLAYER 2 WINS!" << std::endl;
        }
        else
        {
          std::cout << "PLAYERS TIED!" << std::endl;
        }
        return true;
      }

      if (gameData.player1Turn)
      {
        std::cout << "Player 1 choose a number" << std::endl;
      }
      else
      {
        std::cout << "Player 2 choose a number" << std::endl;
      }
      gameData.acceptInput = true;
      if (gameData.currentNumber >= 0 && gameData.numbersUsed.find(gameData.currentNumber) == gameData.numbersUsed.end())
      {
        if (gameData.player1Turn)
        {
          GetCollectedValue(*data, gameData.players[0].entity).push_back(gameData.currentNumber + '0');
          UpdateNumbers(data, gameData.players[0].entity, gameData.currentNumber);
        }
        else
        {
          GetCollectedValue(*data, gameData.players[1].entity).push_back(gameData.currentNumber + '0');
          UpdateNumbers(data, gameData.players[1].entity, gameData.currentNumber);
        }
        --gameData.numbersLeft;
        gameData.player1Turn = !gameData.player1Turn;
        gameData.numbersUsed.insert(gameData.currentNumber);
        gameData.numbersUnused[gameData.currentNumber] = -1;
        gameData.currentNumber = -1;
      }

      return false;
    }

    void Construct(Temp::Scene::Data *data)
    {
      Coordinator::Init(data->coordinator);

      int count = 0;

      gameData.grid.gridSize = 50;

      Temp::TextBox::Construct(data, &gameData.player1NumbersTextBox);
      Temp::TextBox::Construct(data, &gameData.player2NumbersTextBox);
      Temp::TextBox::Construct(data, &gameData.player1Numbers2TextBox);
      Temp::TextBox::Construct(data, &gameData.player2Numbers2TextBox);
      Grid::Construct(data, &gameData.grid);

      Entity Player1 = count++;
      data->entities[Player1] = Temp::Scene::CreateEntity(*data);
      Temp::Scene::AddComponent<Component::Type::COLLECTED_VALUE>(*data, Player1, {});
      Temp::Scene::AddComponent<Component::Type::SCORE>(*data, Player1, {});
      gameData.players[0].entity = Player1;

      Entity Player2 = count++;
      data->entities[Player2] = Temp::Scene::CreateEntity(*data);
      Temp::Scene::AddComponent<Component::Type::COLLECTED_VALUE>(*data, Player2, {});
      Temp::Scene::AddComponent<Component::Type::SCORE>(*data, Player2, {});
      gameData.players[1].entity = Player2;

      float scale = 32.f * gameData.grid.gridSize / 50.f;
      Temp::Camera::UpdateOrthoScale(data, scale);
      Temp::Camera::TranslateView({0, 5 * gameData.grid.gridSize / 50.f, 0});
    }

    void Update(Temp::Scene::Data *data, float deltaTime)
    {
      if (PrintBoard(data, deltaTime))
      {
        data->state = Temp::Scene::State::LEAVE;
      }
    }

    void Draw(Temp::Scene::Data *data)
    {
      Temp::Scene::Draw(data);
      Grid::UpdateUVOffsets(data, &gameData.grid);
    }

    void Destruct(Temp::Scene::Data *data)
    {
      for (auto &entity : data->entities)
      {
        Temp::Scene::DestroyEntity(*data, entity);
        entity = 0;
      }
      Temp::Scene::Destruct(*data);
    }
  }

  Temp::Scene::Data *Create(Temp::Input::KeyEventData &keyEventData)
  {
    Temp::Scene::Data *scene = new Temp::Scene::Data();
    scene->Construct = Construct;
    scene->Update = Update;
    scene->Destruct = Destruct;
    scene->Draw = Draw;

    Temp::Input::AddCallback(inputCallback, keyEventData, Temp::Input::KeyboardCode::KB_0);
    Temp::Input::AddCallback(inputCallback, keyEventData, Temp::Input::KeyboardCode::KB_1);
    Temp::Input::AddCallback(inputCallback, keyEventData, Temp::Input::KeyboardCode::KB_2);
    Temp::Input::AddCallback(inputCallback, keyEventData, Temp::Input::KeyboardCode::KB_3);
    Temp::Input::AddCallback(inputCallback, keyEventData, Temp::Input::KeyboardCode::KB_4);
    Temp::Input::AddCallback(inputCallback, keyEventData, Temp::Input::KeyboardCode::KB_5);
    Temp::Input::AddCallback(inputCallback, keyEventData, Temp::Input::KeyboardCode::KB_6);
    Temp::Input::AddCallback(inputCallback, keyEventData, Temp::Input::KeyboardCode::KB_7);
    Temp::Input::AddCallback(inputCallback, keyEventData, Temp::Input::KeyboardCode::KB_8);
    Temp::Input::AddCallback(inputCallback, keyEventData, Temp::Input::KeyboardCode::KB_9);
    return scene;
  }
}
