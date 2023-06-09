#include "GameLevel.hpp"

#include "Camera.hpp"
#include "Components/ComponentType.hpp"
#include "Coordinator.hpp"
#include "Grid.hpp"
#include "Scene.hpp"
#include "TextBox.hpp"
#include "TextButton.hpp"
#include <unordered_set>

namespace Game::Scene::GameLevel
{
  namespace
  {
    float fontScale = 0.04;

    struct Data
    {
      std::array<Player::Data, 2> players{};
      std::unordered_set<int> numbersUsed{};
      std::array<int, 10> numbersUnused{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
      Grid::Data grid{};
      TextBox::Data playerTurnTextBox{"Player 1's Turn", -20, 40, fontScale, (Entity)-1};
      TextBox::Data player1TextBox{"Player 1's numbers", -85, 10, fontScale, (Entity)-1};
      TextBox::Data player1NumbersTextBox{"", -85, 0, fontScale, (Entity)-1};
      TextBox::Data player2TextBox{"Player 2's numbers", 40, 10, fontScale, (Entity)-1};
      TextBox::Data player2NumbersTextBox{"", 40, 0, fontScale, (Entity)-1};
      TextBox::Data numbersLeftTextBox{"Numbers Left: 0 1 2 3 4 5 6 7 8 9", -35, -40, fontScale, (Entity)-1};
      int currentNumber{-1};
      int numbersLeft{10};
      bool player1Turn{true};
      bool acceptInput{true};
    };

    struct Data2
    {
      TextBox::Data playerWinTextBox{"Player 1 Wins!", -20, 10, fontScale, (Entity)-1};
      TextBox::Data player1ScoreTextBox{"Player 1 Score: ", -20, 0, fontScale, (Entity)-1};
      TextBox::Data player2ScoreTextBox{"Player 2 Score: ", -20, -5, fontScale, (Entity)-1};
      TextBox::Data quitTextBox{"Press Q to quit", -20, -15, fontScale, (Entity)-1};
      TextBox::Data replayTextBox{"Press 0-9 to replay", -20, -22, fontScale, (Entity)-1};
    };

    Data gameData{};
    Data2 gameData2{};
    int playerWinState = 0;
    int player1Score = 0;
    int player2Score = 0;
    bool replay = false;
    Temp::Input::KeyEventData *keyEventDataPtr = nullptr;

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

    void UpdateText(Temp::Scene::Data &sceneData, int currentValue)
    {
      std::string appendText = std::to_string(currentValue);
      if (gameData.numbersLeft > 2)
      {
        appendText += " ";
      }
      if (gameData.player1Turn)
      {
        TextBox::UpdateText(sceneData,
                            gameData.player1NumbersTextBox,
                            gameData.player1NumbersTextBox.text + appendText);
      }
      else
      {
        TextBox::UpdateText(sceneData,
                            gameData.player2NumbersTextBox,
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
                          gameData.numbersLeftTextBox,
                          numbersLeftText);
      TextBox::UpdateText(sceneData,
                          gameData.playerTurnTextBox,
                          gameData.player1Turn ? "Player 2's Turn" : "Player 1's Turn");
    }

    void UpdateNumbers(Temp::Scene::Data &sceneData, Entity player, int currentValue)
    {
      Grid::UpdateNumbers(sceneData, &gameData.grid, player, currentValue);
      UpdateText(sceneData, currentValue);
    }

    bool PrintBoard(Temp::Scene::Data &data, float /*deltaTime*/)
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
          GetCollectedValue(data, gameData.players[0].entity).push_back(gameData.currentNumber + '0');
          UpdateNumbers(data, gameData.players[0].entity, gameData.currentNumber);
        }
        else
        {
          GetCollectedValue(data, gameData.players[1].entity).push_back(gameData.currentNumber + '0');
          UpdateNumbers(data, gameData.players[1].entity, gameData.currentNumber);
        }
        --gameData.numbersLeft;
        gameData.player1Turn = !gameData.player1Turn;
        gameData.numbersUsed.insert(gameData.currentNumber);
        gameData.currentNumber = -1;
      }

      return false;
    }

    void Construct(Temp::Scene::Data &data)
    {      
      Temp::Camera::ResetView();
      Temp::Camera::UpdateOrthoScale(data, 0.1f * (gameData.grid.gridSize / 50.f) * (720.f / Temp::Camera::GetHeight()));
      Temp::Camera::UpdateFontOrthoScale(data, 0.1f * (70.f / 50.f) * (720.f / Temp::Camera::GetHeight()));

      gameData = {};
      //gameData2 = {};
      playerWinState = 0;
      player1Score = 0;
      player2Score = 0;
      replay = false;

//      Coordinator::Init(data.coordinator);

      gameData.grid.gridSize = 100;

      Temp::TextBox::Construct(data, gameData.playerTurnTextBox);
      Temp::TextBox::Construct(data, gameData.player1TextBox);
      Temp::TextBox::Construct(data, gameData.player1NumbersTextBox);
      Temp::TextBox::Construct(data, gameData.player2TextBox);
      Temp::TextBox::Construct(data, gameData.player2NumbersTextBox);
      Temp::TextBox::Construct(data, gameData.numbersLeftTextBox);
      Grid::Construct(data, &gameData.grid);

      Entity Player1 = Temp::Scene::CreateEntity(data);
      Temp::Scene::AddComponent<Component::Type::COLLECTED_VALUE>(data, Player1, {});
      Temp::Scene::AddComponent<Component::Type::SCORE>(data, Player1, {});
      gameData.players[0].entity = Player1;

      Entity Player2 = Temp::Scene::CreateEntity(data);
      Temp::Scene::AddComponent<Component::Type::COLLECTED_VALUE>(data, Player2, {});
      Temp::Scene::AddComponent<Component::Type::SCORE>(data, Player2, {});
      gameData.players[1].entity = Player2;

      Temp::Camera::TranslateView({0.75, 0.5, 0});

      Temp::Input::AddCallback(inputCallback, *keyEventDataPtr, Temp::Input::KeyboardCode::KB_0);
      Temp::Input::AddCallback(inputCallback, *keyEventDataPtr, Temp::Input::KeyboardCode::KB_1);
      Temp::Input::AddCallback(inputCallback, *keyEventDataPtr, Temp::Input::KeyboardCode::KB_2);
      Temp::Input::AddCallback(inputCallback, *keyEventDataPtr, Temp::Input::KeyboardCode::KB_3);
      Temp::Input::AddCallback(inputCallback, *keyEventDataPtr, Temp::Input::KeyboardCode::KB_4);
      Temp::Input::AddCallback(inputCallback, *keyEventDataPtr, Temp::Input::KeyboardCode::KB_5);
      Temp::Input::AddCallback(inputCallback, *keyEventDataPtr, Temp::Input::KeyboardCode::KB_6);
      Temp::Input::AddCallback(inputCallback, *keyEventDataPtr, Temp::Input::KeyboardCode::KB_7);
      Temp::Input::AddCallback(inputCallback, *keyEventDataPtr, Temp::Input::KeyboardCode::KB_8);
      Temp::Input::AddCallback(inputCallback, *keyEventDataPtr, Temp::Input::KeyboardCode::KB_9);
    }

    void Construct2(Temp::Scene::Data &data)
    {
      gameData2 = {};
      replay = false;
//      Coordinator::Init(data.coordinator);
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
      Temp::TextBox::Construct(data, gameData2.playerWinTextBox);
      Temp::TextBox::Construct(data, gameData2.player1ScoreTextBox);
      Temp::TextBox::Construct(data, gameData2.player2ScoreTextBox);
      Temp::TextBox::Construct(data, gameData2.quitTextBox);
      Temp::TextBox::Construct(data, gameData2.replayTextBox);

      Temp::Input::AddCallback(inputCallback2, *keyEventDataPtr, Temp::Input::KeyboardCode::KB_0);
      Temp::Input::AddCallback(inputCallback2, *keyEventDataPtr, Temp::Input::KeyboardCode::KB_1);
      Temp::Input::AddCallback(inputCallback2, *keyEventDataPtr, Temp::Input::KeyboardCode::KB_2);
      Temp::Input::AddCallback(inputCallback2, *keyEventDataPtr, Temp::Input::KeyboardCode::KB_3);
      Temp::Input::AddCallback(inputCallback2, *keyEventDataPtr, Temp::Input::KeyboardCode::KB_4);
      Temp::Input::AddCallback(inputCallback2, *keyEventDataPtr, Temp::Input::KeyboardCode::KB_5);
      Temp::Input::AddCallback(inputCallback2, *keyEventDataPtr, Temp::Input::KeyboardCode::KB_6);
      Temp::Input::AddCallback(inputCallback2, *keyEventDataPtr, Temp::Input::KeyboardCode::KB_7);
      Temp::Input::AddCallback(inputCallback2, *keyEventDataPtr, Temp::Input::KeyboardCode::KB_8);
      Temp::Input::AddCallback(inputCallback2, *keyEventDataPtr, Temp::Input::KeyboardCode::KB_9);
    }

    void Update(Temp::Scene::Data &data, float deltaTime)
    {
      if (PrintBoard(data, deltaTime))
      {
        Temp::Input::RemoveCallback(inputCallback, *keyEventDataPtr, Temp::Input::KeyboardCode::KB_0);
        Temp::Input::RemoveCallback(inputCallback, *keyEventDataPtr, Temp::Input::KeyboardCode::KB_1);
        Temp::Input::RemoveCallback(inputCallback, *keyEventDataPtr, Temp::Input::KeyboardCode::KB_2);
        Temp::Input::RemoveCallback(inputCallback, *keyEventDataPtr, Temp::Input::KeyboardCode::KB_3);
        Temp::Input::RemoveCallback(inputCallback, *keyEventDataPtr, Temp::Input::KeyboardCode::KB_4);
        Temp::Input::RemoveCallback(inputCallback, *keyEventDataPtr, Temp::Input::KeyboardCode::KB_5);
        Temp::Input::RemoveCallback(inputCallback, *keyEventDataPtr, Temp::Input::KeyboardCode::KB_6);
        Temp::Input::RemoveCallback(inputCallback, *keyEventDataPtr, Temp::Input::KeyboardCode::KB_7);
        Temp::Input::RemoveCallback(inputCallback, *keyEventDataPtr, Temp::Input::KeyboardCode::KB_8);
        Temp::Input::RemoveCallback(inputCallback, *keyEventDataPtr, Temp::Input::KeyboardCode::KB_9);
        data.state = Temp::Scene::State::LEAVE;
      }
    }

    void Update2(Temp::Scene::Data &data, float /*deltaTime*/)
    {
      if (replay)
      {
        Temp::Input::RemoveCallback(inputCallback2, *keyEventDataPtr, Temp::Input::KeyboardCode::KB_0);
        Temp::Input::RemoveCallback(inputCallback2, *keyEventDataPtr, Temp::Input::KeyboardCode::KB_1);
        Temp::Input::RemoveCallback(inputCallback2, *keyEventDataPtr, Temp::Input::KeyboardCode::KB_2);
        Temp::Input::RemoveCallback(inputCallback2, *keyEventDataPtr, Temp::Input::KeyboardCode::KB_3);
        Temp::Input::RemoveCallback(inputCallback2, *keyEventDataPtr, Temp::Input::KeyboardCode::KB_4);
        Temp::Input::RemoveCallback(inputCallback2, *keyEventDataPtr, Temp::Input::KeyboardCode::KB_5);
        Temp::Input::RemoveCallback(inputCallback2, *keyEventDataPtr, Temp::Input::KeyboardCode::KB_6);
        Temp::Input::RemoveCallback(inputCallback2, *keyEventDataPtr, Temp::Input::KeyboardCode::KB_7);
        Temp::Input::RemoveCallback(inputCallback2, *keyEventDataPtr, Temp::Input::KeyboardCode::KB_8);
        Temp::Input::RemoveCallback(inputCallback2, *keyEventDataPtr, Temp::Input::KeyboardCode::KB_9);
        data.state = Temp::Scene::State::LEAVE;
      }
    }

    void DrawConstruct(Temp::Scene::Data &data)
    {
      Grid::DrawConstruct(data, &gameData.grid);
      Temp::TextBox::DrawConstruct(data, gameData.playerTurnTextBox);
      Temp::TextBox::DrawConstruct(data, gameData.player1TextBox);
      Temp::TextBox::DrawConstruct(data, gameData.player1NumbersTextBox);
      Temp::TextBox::DrawConstruct(data, gameData.player2TextBox);
      Temp::TextBox::DrawConstruct(data, gameData.player2NumbersTextBox);
      Temp::TextBox::DrawConstruct(data, gameData.numbersLeftTextBox);
    }

    void DrawConstruct2(Temp::Scene::Data &data)
    {
      Temp::TextBox::DrawConstruct(data, gameData2.playerWinTextBox);
      Temp::TextBox::DrawConstruct(data, gameData2.player1ScoreTextBox);
      Temp::TextBox::DrawConstruct(data, gameData2.player2ScoreTextBox);
      Temp::TextBox::DrawConstruct(data, gameData2.quitTextBox);
      Temp::TextBox::DrawConstruct(data, gameData2.replayTextBox);
    }

    void DrawUpdate(Temp::Scene::Data &data)
    {
      Grid::DrawUpdate(data, &gameData.grid);
      TextBox::UpdateRender(data, gameData.player1NumbersTextBox);
      TextBox::UpdateRender(data, gameData.player2NumbersTextBox);
      TextBox::UpdateRender(data, gameData.numbersLeftTextBox);
      TextBox::UpdateRender(data, gameData.playerTurnTextBox);
      Temp::Camera::UpdateOrthoScale(data, 0.1f * (gameData.grid.gridSize / 50.f) * (720.f / Temp::Camera::GetHeight()));
      Temp::Camera::UpdateFontOrthoScale(data, 0.1f * (70.f / 50.f) * (720.f / Temp::Camera::GetHeight()));
    }

    void DrawUpdate2(Temp::Scene::Data &data)
    {
      Temp::Camera::UpdateFontOrthoScale(data, 0.1f * (70.f / 50.f) * (720.f / Temp::Camera::GetHeight()));
    }

    void Destruct(Temp::Scene::Data &)
    {
//      Coordinator::Destruct(data.coordinator);
//      Temp::Scene::Destruct(data);
      Temp::TextBox::Destruct(gameData.playerTurnTextBox);
      Temp::TextBox::Destruct(gameData.player1TextBox);
      Temp::TextBox::Destruct(gameData.player1NumbersTextBox);
      Temp::TextBox::Destruct(gameData.player2TextBox);
      Temp::TextBox::Destruct(gameData.player2NumbersTextBox);
      Temp::TextBox::Destruct(gameData.numbersLeftTextBox);
      Grid::Destruct(&gameData.grid);
//      delete gameData.grid;
    }
    
    void Destruct2(Temp::Scene::Data &)
    {
//      Coordinator::Destruct(data.coordinator);
//      Temp::Scene::Destruct(data);
      Temp::TextBox::Destruct(gameData2.playerWinTextBox);
      Temp::TextBox::Destruct(gameData2.player1ScoreTextBox);
      Temp::TextBox::Destruct(gameData2.player2ScoreTextBox);
      Temp::TextBox::Destruct(gameData2.quitTextBox);
      Temp::TextBox::Destruct(gameData2.replayTextBox);
    }
    
    void DrawDestruct(Temp::Scene::Data &data)
    {
      Grid::DrawDestruct(data, &gameData.grid);
      Temp::TextBox::DrawDestruct(data, gameData.playerTurnTextBox);
      Temp::TextBox::DrawDestruct(data, gameData.player1TextBox);
      Temp::TextBox::DrawDestruct(data, gameData.player1NumbersTextBox);
      Temp::TextBox::DrawDestruct(data, gameData.player2TextBox);
      Temp::TextBox::DrawDestruct(data, gameData.player2NumbersTextBox);
      Temp::TextBox::DrawDestruct(data, gameData.numbersLeftTextBox);
    }
    
    void DrawDestruct2(Temp::Scene::Data &data)
    {
      Temp::TextBox::DrawDestruct(data, gameData2.playerWinTextBox);
      Temp::TextBox::DrawDestruct(data, gameData2.player1ScoreTextBox);
      Temp::TextBox::DrawDestruct(data, gameData2.player2ScoreTextBox);
      Temp::TextBox::DrawDestruct(data, gameData2.quitTextBox);
      Temp::TextBox::DrawDestruct(data, gameData2.replayTextBox);
    }
    
    void DrawReload(Temp::Scene::Data &data, int shaderIdx)
    {
      Grid::DrawReload(data, &gameData.grid);
      Temp::TextBox::DrawReload(data, gameData.playerTurnTextBox, shaderIdx);
      Temp::TextBox::DrawReload(data, gameData.player1TextBox, shaderIdx);
      Temp::TextBox::DrawReload(data, gameData.player1NumbersTextBox, shaderIdx);
      Temp::TextBox::DrawReload(data, gameData.player2TextBox, shaderIdx);
      Temp::TextBox::DrawReload(data, gameData.player2NumbersTextBox, shaderIdx);
      Temp::TextBox::DrawReload(data, gameData.numbersLeftTextBox, shaderIdx);
    }
    
    void DrawReload2(Temp::Scene::Data &data, int shaderIdx)
    {
      Temp::TextBox::DrawReload(data, gameData2.playerWinTextBox, shaderIdx);
      Temp::TextBox::DrawReload(data, gameData2.player1ScoreTextBox, shaderIdx);
      Temp::TextBox::DrawReload(data, gameData2.player2ScoreTextBox, shaderIdx);
      Temp::TextBox::DrawReload(data, gameData2.quitTextBox, shaderIdx);
      Temp::TextBox::DrawReload(data, gameData2.replayTextBox, shaderIdx);
    }
  }

  Temp::Scene::SceneFns Create(Temp::Input::KeyEventData &keyEventData)
  {
    Temp::Scene::SceneFns scene;
    scene.ConstructFunc = Construct;
    scene.Update = Update;
    scene.DrawConstructFunc = DrawConstruct;
    scene.DrawUpdateFunc = DrawUpdate;
    scene.DestructFunc = Destruct;
    scene.DrawDestructFunc = DrawDestruct;
    scene.DrawReloadFunc = DrawReload;

    keyEventDataPtr = &keyEventData;

    return scene;
  }

  // This is here only for testing. In an actual game, it's better to create a separate file
  // to host this data
  Temp::Scene::SceneFns Create2(Temp::Input::KeyEventData & /*keyEventData*/)
  {
    Temp::Scene::SceneFns scene;
    scene.ConstructFunc = Construct2;
    scene.Update = Update2;
    scene.DrawConstructFunc = DrawConstruct2;
    scene.DrawUpdateFunc = DrawUpdate2;
    scene.DestructFunc = Destruct2;
    scene.DrawDestructFunc = DrawDestruct2;
    scene.DrawReloadFunc = DrawReload2;

    return scene;
  }
}
