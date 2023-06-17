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
    std::mutex mtx{};
    struct Data
    {
      std::array<Player::Data, 2> players{};
      std::unordered_set<int> numbersUsed{};
      std::array<int, 10> numbersUnused{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
      Grid::Data grid{};
      TextBox::Data playerTurnTextBox{"Player 1's Turn", -0.55, 0.9, 0.0006f, (Entity)-1};
      TextBox::Data player1TextBox{"Player 1's numbers", -1.6, 0, 0.0006f, (Entity)-1};
      TextBox::Data player1NumbersTextBox{"", -1.6, -0.1, 0.0006f, (Entity)-1};
      TextBox::Data player2TextBox{"Player 2's numbers", 0.9, 0, 0.0006f, (Entity)-1};
      TextBox::Data player2NumbersTextBox{"", 0.9, -0.1, 0.0006f, (Entity)-1};
      TextBox::Data numbersLeftTextBox{"Numbers Left: 0 1 2 3 4 5 6 7 8 9", -0.55, -0.9, 0.0006f, (Entity)-1};
      int currentNumber{-1};
      int numbersLeft{10};
      bool player1Turn{true};
      bool acceptInput{true};
    };

    struct Data2
    {
      TextBox::Data playerWinTextBox{"Player 1 Wins!", -0.25, 0.2, 0.0006f, (Entity)-1};
      TextBox::Data player1ScoreTextBox{"Player 1 Score: ", -0.25, 0, 0.0006f, (Entity)-1};
      TextBox::Data player2ScoreTextBox{"Player 2 Score: ", -0.25, -0.1, 0.0006f, (Entity)-1};
      TextBox::Data quitTextBox{"Press Q to quit", -0.25, -0.3, 0.0006f, (Entity)-1};
      TextBox::Data replayTextBox{"Press 0-9 to replay", -0.25, -0.4, 0.0006f, (Entity)-1};
    };

    Data gameData{};
    Data2 gameData2{};
    int playerWinState = 0;
    int player1Score = 0;
    int player2Score = 0;
    bool replay = false;
    Temp::Input::KeyEventData *keyEventDataPtr = nullptr;

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

    void inputCallback2(Temp::Input::KeyboardCode)
    {
      replay = true;
    }

    void UpdateText(Temp::Scene::Data *sceneData, int currentValue)
    {
      std::string appendText = std::to_string(currentValue);
      if (gameData.numbersLeft > 2)
      {
        appendText += " ";
      }
      if (gameData.player1Turn)
      {
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

      std::string numbersLeftText = "Numbers Left:";
      for (int num : gameData.numbersUnused)
      {
        if (num >= 0)
        {
          numbersLeftText += " ";
          numbersLeftText += std::to_string(num);
        }
      }
      TextBox::UpdateText(sceneData,
                          &gameData.numbersLeftTextBox,
                          numbersLeftText);
      TextBox::UpdateText(sceneData,
                          &gameData.playerTurnTextBox,
                          gameData.player1Turn ? "Player 2's Turn" : "Player 1's Turn");
    }

    void UpdateNumbers(Temp::Scene::Data *sceneData, Entity player, int currentValue)
    {
      Grid::UpdateNumbers(sceneData, &gameData.grid, player, currentValue);
      UpdateText(sceneData, currentValue);
    }

    bool PrintBoard(Temp::Scene::Data *data, float /*deltaTime*/)
    {
      if (gameData.numbersLeft == 0)
      {
        player1Score = Player::getScore(gameData.players[0].entity, data);
        player2Score = Player::getScore(gameData.players[1].entity, data);
        if (Player::getScore(gameData.players[0].entity, data) >
            Player::getScore(gameData.players[1].entity, data))
        {
          playerWinState = 0;
        }
        else if (Player::getScore(gameData.players[0].entity, data) <
                 Player::getScore(gameData.players[1].entity, data))
        {
          playerWinState = 1;
        }
        else
        {
          playerWinState = 2;
        }
        return true;
      }

      gameData.acceptInput = true;
      if (gameData.currentNumber >= 0 && gameData.numbersUsed.find(gameData.currentNumber) == gameData.numbersUsed.end())
      {
        gameData.numbersUnused[gameData.currentNumber] = -1;
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
        gameData.currentNumber = -1;
      }

      return false;
    }

    void Draw(Temp::Scene::Data *data);

    void Construct(Temp::Scene::Data *data)
    {
      std::scoped_lock<std::mutex> lock(mtx);

      gameData = {};

      Coordinator::Init(data->coordinator);

      int count = 0;

      gameData.grid.gridSize = 10;

      Temp::TextBox::Construct(data, &gameData.playerTurnTextBox);
      Temp::TextBox::Construct(data, &gameData.player1TextBox);
      Temp::TextBox::Construct(data, &gameData.player1NumbersTextBox);
      Temp::TextBox::Construct(data, &gameData.player2TextBox);
      Temp::TextBox::Construct(data, &gameData.player2NumbersTextBox);
      Temp::TextBox::Construct(data, &gameData.numbersLeftTextBox);
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
      Temp::Camera::TranslateView({0.1, 2 * gameData.grid.gridSize / 50.f, 0});
      data->DrawFunc = Draw;
    }

    void Construct2(Temp::Scene::Data *data)
    {
      Coordinator::Init(data->coordinator);
      switch (playerWinState)
      {
      case 0:
        gameData2.playerWinTextBox.text = "Player 1 Wins!";
        break;
      case 1:
        gameData2.playerWinTextBox.text = "Player 2 Wins!";
        break;
      case 2:
        gameData2.playerWinTextBox.text = "Players Tied!";
        break;
      default:
        break;
      };
      gameData2.player1ScoreTextBox.text += std::to_string(player1Score);
      gameData2.player2ScoreTextBox.text += std::to_string(player2Score);
      Temp::TextBox::Construct(data, &gameData2.playerWinTextBox);
      Temp::TextBox::Construct(data, &gameData2.player1ScoreTextBox);
      Temp::TextBox::Construct(data, &gameData2.player2ScoreTextBox);
      Temp::TextBox::Construct(data, &gameData2.quitTextBox);
      Temp::TextBox::Construct(data, &gameData2.replayTextBox);

      Temp::Input::AddCallback(inputCallback2, *keyEventDataPtr, Temp::Input::KeyboardCode::KB_0);
    }

    void Update(Temp::Scene::Data *data, float deltaTime)
    {
      if (PrintBoard(data, deltaTime))
      {
        data->state = Temp::Scene::State::LEAVE;
      }
    }

    void Update2(Temp::Scene::Data *data, float /*deltaTime*/)
    {
      if (replay)
      {
        data->state = Temp::Scene::State::LEAVE;
      }
    }

    void NoOp(Temp::Scene::Data *)
    {
    }

    void Draw(Temp::Scene::Data *data)
    {
      std::scoped_lock<std::mutex> lock(mtx);

      Temp::Scene::Draw(data);
      Grid::UpdateUVOffsets(data, &gameData.grid);
      TextBox::UpdateTextRender(data, &gameData.player1NumbersTextBox);
      TextBox::UpdateTextRender(data, &gameData.player2NumbersTextBox);
      TextBox::UpdateTextRender(data, &gameData.numbersLeftTextBox);
      TextBox::UpdateTextRender(data, &gameData.playerTurnTextBox);
    }

    void Destruct(Temp::Scene::Data *data)
    {
      std::scoped_lock<std::mutex> lock(mtx);
      data->DrawFunc = NoOp;

      for (auto &entity : data->entities)
      {
        Temp::Scene::DestroyEntity(*data, entity);
        entity = 0;
      }
      Temp::Scene::Destruct(data);
    }
  }

  Temp::Scene::Data *Create(Temp::Input::KeyEventData &keyEventData)
  {
    Temp::Scene::Data *scene = new Temp::Scene::Data();
    scene->Construct = Construct;
    scene->Update = Update;
    scene->DestructFunc = Destruct;
    scene->DrawFunc = Draw;

    keyEventDataPtr = &keyEventData;

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

  // This is here only for testing. In an actual game, it's better to create a separate file
  // to host this data
  Temp::Scene::Data *Create2(Temp::Input::KeyEventData & /*keyEventData*/)
  {
    Temp::Scene::Data *scene = new Temp::Scene::Data();
    scene->Construct = Construct2;
    scene->Update = Update2;
    // scene->Destruct = Destruct;
    // scene->Draw = Draw2;

    // Temp::Input::AddCallback(inputCallback2, keyEventData, Temp::Input::KeyboardCode::KB_1);
    // Temp::Input::AddCallback(inputCallback2, keyEventData, Temp::Input::KeyboardCode::KB_2);
    // Temp::Input::AddCallback(inputCallback2, keyEventData, Temp::Input::KeyboardCode::KB_3);
    // Temp::Input::AddCallback(inputCallback2, keyEventData, Temp::Input::KeyboardCode::KB_4);
    // Temp::Input::AddCallback(inputCallback2, keyEventData, Temp::Input::KeyboardCode::KB_5);
    // Temp::Input::AddCallback(inputCallback2, keyEventData, Temp::Input::KeyboardCode::KB_6);
    // Temp::Input::AddCallback(inputCallback2, keyEventData, Temp::Input::KeyboardCode::KB_7);
    // Temp::Input::AddCallback(inputCallback2, keyEventData, Temp::Input::KeyboardCode::KB_8);
    // Temp::Input::AddCallback(inputCallback2, keyEventData, Temp::Input::KeyboardCode::KB_9);
    return scene;
  }
}
