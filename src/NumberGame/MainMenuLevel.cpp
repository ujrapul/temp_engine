#include "MainMenuLevel.hpp"

#include "Scene.hpp"
#include "TextBox.hpp"
#include "TextButton.hpp"
#include "Engine.hpp"
#include "Camera.hpp"
#include "LevelParser.hpp"

namespace Game::Scene::MainMenuLevel
{
  namespace
  {
    using namespace Temp;

    struct Data
    {
      TextBox::Data gameTextBox{};
      TextButton::Data playButton{};
      TextButton::Data quitButton{};
    };

    Data gameData{};

     void PlayCallback(Temp::Scene::Data &scene, Temp::Component::Hoverable::Data &)
     {
       scene.state = Temp::Scene::State::LEAVE;
     }

     void QuitCallback(Temp::Scene::Data &, Temp::Component::Hoverable::Data &)
     {
       Engine::Quit(Engine::engine);
     }

    void Construct(Temp::Scene::Data &scene)
    {
      LevelParser::Parse(scene, "MainMenu.level");
      gameData = {};

      Temp::Camera::ResetView();

      Temp::Scene::Construct(scene);

      gameData.gameTextBox = *static_cast<TextBox::Data*>(Temp::Scene::GetObject(scene, "NumberGameTextBox").data);
      gameData.playButton = *static_cast<TextButton::Data*>(Temp::Scene::GetObject(scene, "PlayTextButton").data);
      gameData.quitButton = *static_cast<TextButton::Data*>(Temp::Scene::GetObject(scene, "QuitTextButton").data);
      
      TextButton::SetHoverableCallbacks(scene, gameData.playButton, PlayCallback);
      TextButton::SetHoverableCallbacks(scene, gameData.quitButton, QuitCallback);
      
      // gameData.playButton.

      // TextBox::Construct(scene, gameData.gameTextBox);
      // Temp::Component::Hoverable::Data playHoverable{PlayCallback, HoverEnter, HoverLeave, &gameData.playButton, -20, 0, 9, 4};
      // Temp::Component::Hoverable::Data quitHoverable{QuitCallback, HoverEnter, HoverLeave, &gameData.quitButton, -20, -7, 9, 4};
      // TextButton::Construct(scene, gameData.playButton, playHoverable);
      // TextButton::Construct(scene, gameData.quitButton, quitHoverable);
    }

    void Update(Temp::Scene::Data &scene, float deltaTime)
    {
      Temp::TextButton::Update(scene, gameData.playButton, deltaTime);
    }

    void Destruct(Temp::Scene::Data &scene)
    {
      // TextBox::Destruct(gameData.gameTextBox);
      // Temp::TextButton::Destruct(gameData.playButton);
      // Temp::TextButton::Destruct(gameData.quitButton);
     Temp::Scene::Destruct(scene);
    }

    void DrawUpdate(Temp::Scene::Data &scene)
    {
      Temp::Camera::UpdateFontOrthoScale(scene, (720.f / Temp::Camera::GetHeight()));
    }

    void DrawConstruct(Temp::Scene::Data &scene)
    {
      Temp::TextBox::DrawConstruct(scene, gameData.gameTextBox);
      Temp::TextButton::DrawConstruct(scene, gameData.playButton);
      Temp::TextButton::DrawConstruct(scene, gameData.quitButton);
    }
    
    void DrawDestruct(Temp::Scene::Data &scene)
    {
      Temp::TextBox::DrawDestruct(scene, gameData.gameTextBox);
      Temp::TextButton::DrawDestruct(scene, gameData.playButton);
      Temp::TextButton::DrawDestruct(scene, gameData.quitButton);
    }
    
    void DrawReload(Temp::Scene::Data &scene, int shaderIdx)
    {
      Temp::TextBox::DrawReload(scene, gameData.gameTextBox, shaderIdx);
      Temp::TextButton::DrawReload(scene, gameData.playButton, shaderIdx);
      Temp::TextButton::DrawReload(scene, gameData.quitButton, shaderIdx);
    }
  }

  Temp::Scene::SceneFns Create()
  {
    Temp::Scene::SceneFns scene;
    scene.ConstructFunc = Construct;
    scene.Update = Update;
    scene.DestructFunc = Destruct;
    scene.DrawConstructFunc = DrawConstruct;
    scene.DrawUpdateFunc = DrawUpdate;
    scene.DrawDestructFunc = DrawDestruct;
    scene.DrawReloadFunc = DrawReload;

    return scene;
  }
}
