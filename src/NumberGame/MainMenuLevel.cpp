#include "MainMenuLevel.hpp"

#include "Scene.hpp"
#include "TextBox.hpp"
#include "TextButton.hpp"
#include "Engine.hpp"
#include "Camera.hpp"

namespace Game::Scene::MainMenuLevel
{
  namespace
  {
    using namespace Temp;

    float fontScale = 0.04f;
    std::mutex mtx{};

    struct Data
    {
      TextBox::Data gameTextBox{"NUMBER GAME", -20, 10, fontScale, 0};
      // TextBox::Data quitTextBox{"Quit", -20, -7, fontScale, (Entity)-1};
      TextButton::Data playButton{0, {"Play ", -20, 0, fontScale, 0}};
      TextButton::Data quitButton{0, {"Quit", -20, -7, fontScale, 0}};
    };

    Data gameData{};

    void PlayCallback(Temp::Scene::Data *scene, Temp::Component::Hoverable::Data *)
    {
      scene->state = Temp::Scene::State::LEAVE;
    }

    void QuitCallback(Temp::Scene::Data *, Temp::Component::Hoverable::Data *)
    {
      Engine::Quit(Engine::engine);
    }

    void Construct(Temp::Scene::Data *data)
    {
      gameData = {};

      Temp::Camera::ResetView();

      Temp::Scene::Construct(data);

      TextBox::Construct(data, &gameData.gameTextBox);
      // TextBox::Data quitTextBox{"Quit", -20, -7, fontScale, (Entity)-1};
      Temp::Component::Hoverable::Data hoverable{PlayCallback, -20, 0, 40, 50};
      Temp::Component::Hoverable::Data hoverable2{QuitCallback, -20, -7, 40, 50};
      TextButton::Construct(data, &gameData.playButton, hoverable);
      TextButton::Construct(data, &gameData.quitButton, hoverable2);
    }

    void Update(Temp::Scene::Data *, float)
    {
    }

    void Destruct(Temp::Scene::Data *data)
    {
      Temp::Scene::Destruct(data);
    }

    void Draw(Temp::Scene::Data *data)
    {
      Temp::Scene::Draw(data);
      // TextButton::UpdateRender(data, &gameData.playButton);
      // TextButton::UpdateRender(data, &gameData.quitButton);
      Temp::Camera::UpdateFontOrthoScale(data, 0.15 * (720.f / Temp::Camera::GetHeight()));
    }

    void DrawConstruct(Temp::Scene::Data *data)
    {
      Temp::TextBox::ConstructRender(data, &gameData.gameTextBox);
      Temp::TextButton::ConstructRender(data, &gameData.playButton);
      Temp::TextButton::ConstructRender(data, &gameData.quitButton);
    }
  }

  Temp::Scene::Data *Create()
  {
    Temp::Scene::Data *scene = new Temp::Scene::Data();
    scene->Construct = Construct;
    scene->Update = Update;
    scene->DestructFunc = Destruct;
    scene->DrawFunc = Draw;
    scene->DrawConstructFunc = DrawConstruct;

    return scene;
  }
}