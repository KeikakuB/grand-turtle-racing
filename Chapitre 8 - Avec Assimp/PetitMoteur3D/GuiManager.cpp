#include "stdafx.h"
#include "GuiManager.h"
#include "GameSingleton.h"
#include "IInputManager.h"
#include "ISpriteManager.h"
#include "ITimeManager.h"
#include "IGameLogicManager.h"
#include "IPostEffectManager.h"
#include "Ids.h"
#include "IWorldManager.h"
#include "HealthComponent.h"

namespace PM3D
{
  namespace meta
  {
    REGISTER_COMPONENT_CPP_ID(GuiManager);
    template<>
    component_interface_id_list_t GetComponentInterfaces<GuiManager>() noexcept { return{ GetInterfaceId<IGuiManager>() }; }
  }

  GuiManager::GuiManager() :
    oFamily{ L"Arial", NULL },
    pPolice{ std::make_unique<Gdiplus::Font>(&oFamily, 16.00, Gdiplus::FontStyleBold, Gdiplus::UnitPixel) },
    text_turtle_velocity_{ 256, 256, pPolice.get() },
    text_turtle_life_{ 256, 256, pPolice.get() },
    text_elasped_time_{ 256, 256, pPolice.get() },
    text_how_to_quit_{ 1024, 256, pPolice.get() },
    text_how_to_win_{ 1024, 256, pPolice.get() },
    text_how_to_start_{ 1024, 256, pPolice.get() },
    text_how_to_gameover_{ 1024, 256, pPolice.get() },
    is_paused_{ false },
    is_end_menu_show_{ false },
    is_gameover_menu_show_{ false }
  {
  }

  void GuiManager::RunTask()
  {
    using namespace std::chrono_literals;
    static const auto kPauseDelay = 300ms;
    auto time_manager = Game<ITimeManager>();
    text_elasped_time_.Ecrire(L"Time: " + std::to_wstring(time_manager->nowGameTime().time_since_epoch().count()) + L"s");
    auto logic_manager = Game<IGameLogicManager>();
    text_turtle_velocity_.Ecrire(L"Velocity: " + std::to_wstring(logic_manager->GetTurtleVelocity()));
    text_turtle_life_.Ecrire(L"Stamina: " + std::to_wstring(logic_manager->GetTurtleLife()));

    auto input_manager = Game<IInputManager>();
    if ( input_manager )
    {
      if ( logic_manager->IsStartGame() )
      {
        if ( input_manager->IsKeyDown(KeyboardKey::Enter) )
        {
          HidePauseMenu();
          logic_manager->SetStartGame();
          logic_manager->SetGamePause(false);
        }
      }
      else
      {
        auto pe_manager = Game<IPostEffectManager>();
        auto now_time = time_manager->nowWorldTime();
        if ( pe_manager && now_time - last_time_added_post_effect_ > ITimeManager::duration{ kPauseDelay } )
        {
          bool something_pressed_ = false;
          if ( input_manager->IsKeyDown(KeyboardKey::Zero) )
          {
            something_pressed_ = true;
            pe_manager->SetDefaultTechnique(ids::post_effect_techniques::kNul);
          }
          else if ( input_manager->IsKeyDown(KeyboardKey::One) )
          {
            something_pressed_ = true;
            pe_manager->EnqueueTechnique(ids::post_effect_techniques::kRadialBlur, 2500ms, 500ms);
          }
          else if ( input_manager->IsKeyDown(KeyboardKey::Two) )
          {
            something_pressed_ = true;
            pe_manager->EnqueueTechnique(ids::post_effect_techniques::kGradualBlur, 2500ms, 500ms);
          }
          else if ( input_manager->IsKeyDown(KeyboardKey::Three) )
          {
            something_pressed_ = true;
            pe_manager->EnqueueTechnique(ids::post_effect_techniques::kGrowingPeephole, 2500ms, 500ms);
          }
          else if ( input_manager->IsKeyDown(KeyboardKey::Four) )
          {
            something_pressed_ = true;
            pe_manager->EnqueueTechnique(ids::post_effect_techniques::kHorizontalShuttersClose, 2500ms, 0ms);
            pe_manager->EnqueueTechnique(ids::post_effect_techniques::kHorizontalShuttersOpen, 2500ms, 0ms);
          }
          else if ( input_manager->IsKeyDown(KeyboardKey::Five) )
          {
            something_pressed_ = true;
            pe_manager->EnqueueTechnique(ids::post_effect_techniques::kHorizontalShuttersClose, 1000ms, 0ms);
            pe_manager->EnqueueTechnique(ids::post_effect_techniques::kHorizontalShuttersOpen, 1000ms, 0ms);
          }
          else if ( input_manager->IsKeyDown(KeyboardKey::Six) )
          {
            something_pressed_ = true;
            pe_manager->EnqueueTechnique(ids::post_effect_techniques::kHorizontalShuttersClose, 500ms, 0ms);
            pe_manager->EnqueueTechnique(ids::post_effect_techniques::kHorizontalShuttersOpen, 500ms, 0ms);
          }
          else if ( input_manager->IsKeyDown(KeyboardKey::Seven) )
          {
            something_pressed_ = true;
            pe_manager->EnqueueTechnique(ids::post_effect_techniques::kRageVision, 2500ms, 0ms);
            pe_manager->EnqueueTechnique(ids::post_effect_techniques::kRageVision, 1500ms, 0ms);
            pe_manager->EnqueueTechnique(ids::post_effect_techniques::kRageVision, 1000ms, 0ms);
            pe_manager->EnqueueTechnique(ids::post_effect_techniques::kRageVision, 500ms, 0ms);
          }
          else if ( input_manager->IsKeyDown(KeyboardKey::Eight) )
          {
            something_pressed_ = true;
            pe_manager->EnqueueTechnique(ids::post_effect_techniques::kTiles, 2500ms, 0ms);
          }
          else if ( input_manager->IsKeyDown(KeyboardKey::Nine) )
          {
            something_pressed_ = true;
          }
          if ( something_pressed_ )
          {
            last_time_added_post_effect_ = now_time;
          }
        }
        if ( input_manager->IsKeyDown(KeyboardKey::Escape) && ( !logic_manager->IsEndGame() && !logic_manager->IsGameOver() )
          && now_time - last_time_paused_ > ITimeManager::duration{ kPauseDelay } )
        {
          last_time_paused_ = now_time;
          if ( is_paused_ )
          {
            time_manager->setGameRate(pre_pause_game_rate_);
            logic_manager->SetGamePause(false);
            HidePauseMenu();
          }
          else
          {
            pre_pause_game_rate_ = time_manager->getGameRate();
            time_manager->setGameRate(0.0f);
            logic_manager->SetGamePause(true);
            ShowPauseMenu();
          }
          is_paused_ = !is_paused_;
        }
        if ( is_paused_ && input_manager->IsKeyDown(KeyboardKey::Enter) )
        {
          logic_manager->SetGameExit();
        }
        if ( logic_manager->IsEndGame() )
        {
          if ( is_end_menu_show_ )
          {
            if ( input_manager->IsKeyDown(KeyboardKey::R) )
            {
              HideEndGameMenu();
              is_end_menu_show_ = false;
              logic_manager->SetEndGame(false);
              time_manager->setGameRate(1.0f);
              logic_manager->SetGameRestarted(true);
              logic_manager->SetGamePause(false);

              for ( auto& s : Game<IWorldManager>()->GetGameObjects() )
              {
                auto turtle_health = s->As<IHealthComponentInterface>();
                if ( turtle_health )
                {
                  turtle_health->SetHealth(1000);
                  break;
                }
              }
            }
            if ( input_manager->IsKeyDown(KeyboardKey::Enter) )
            {
              logic_manager->SetGameExit();
            }
          }
          else
          {
            time_manager->setGameRate(0.0f);
            ShowEndGameMenu();
            is_end_menu_show_ = true;
            logic_manager->SetGamePause(true);
          }
        }
        if ( logic_manager->IsGameOver() )
        {
          if ( is_gameover_menu_show_ )
          {
            if ( input_manager->IsKeyDown(KeyboardKey::R) )
            {
              HideGameOverMenu();
              is_gameover_menu_show_ = false;
              logic_manager->SetGameOver(false);
              time_manager->setGameRate(1.0f);
              logic_manager->SetGameRestarted(true);
              logic_manager->SetGamePause(false);
            }
            if ( input_manager->IsKeyDown(KeyboardKey::Enter) )
            {
              logic_manager->SetGameExit();
            }
          }
          else
          {
            time_manager->setGameRate(0.0f);
            ShowGameOverMenu();
            is_gameover_menu_show_ = true;
            logic_manager->SetGamePause(true);
          }
        }
      }
    }
  }

  void GuiManager::SetupGameGui()
  {
    auto sprite_manager = Game<ISpriteManager>();

    int hauteur = GameSingleton::Get().GetDispositif()->GetHauteur();
    int largeur = GameSingleton::Get().GetDispositif()->GetLargeur();
    int new_largeur_logo = ( largeur - 550 ) / 2;
    int new_largeur_win = ( largeur - 400 ) / 2;
    int new_largeur_pause = ( largeur - 640 ) / 2;
    int new_largeur_restart = ( largeur - 480 ) / 2;
    int new_largeur_start = ( largeur - 300 ) / 2;

    Gdiplus::Color* white_color = new Gdiplus::Color(255, 255, 255, 255);
    Gdiplus::Color* black_color = new Gdiplus::Color(255, 0, 0, 0);

    sprite_manager->AddTextSprite(SpriteGroup::kGuiInGameGroup, text_turtle_life_.GetTextureView(), 0, 257);
    //    sprite_manager->AddTextSprite(SpriteGroup::kGuiInGameGroup, text_turtle_velocity_.GetTextureView(), 0, 257);
    sprite_manager->AddTextSprite(SpriteGroup::kGuiInGameGroup, text_elasped_time_.GetTextureView(), largeur - 150, 257);

    text_turtle_velocity_.ChangeColorBrush(white_color);
    text_elasped_time_.ChangeColorBrush(white_color);
    text_turtle_life_.ChangeColorBrush(white_color);
    text_turtle_velocity_.Ecrire(L"TURTLE VELOCITY/STAMINA PLACEHOLDER");
    text_turtle_life_.Ecrire(L"TURTLE LIFE");

    sprite_manager->AddSprite(SpriteGroup::kGuiPauseMenuGroup, ".//data//Firebools.dds", new_largeur_logo, hauteur / 2);
    sprite_manager->AddTextSprite(SpriteGroup::kGuiPauseMenuGroup, text_how_to_quit_.GetTextureView(), new_largeur_pause, 900);
    text_how_to_quit_.ChangeColorBrush(black_color);
    text_how_to_quit_.Ecrire(L"PRESS ENTER TO EXIT - USE NUM KEYS TO SWITCH BETWEEN POST EFFECTS");

    sprite_manager->AddSprite(SpriteGroup::kGuiEndGameGroup, ".//data//Firebools.dds", new_largeur_logo, hauteur / 4);
    sprite_manager->AddSprite(SpriteGroup::kGuiEndGameGroup, ".//data//Win_White.dds", new_largeur_win, hauteur / 2);
    sprite_manager->AddTextSprite(SpriteGroup::kGuiEndGameGroup, text_how_to_win_.GetTextureView(), new_largeur_restart, 900);
    text_how_to_win_.ChangeColorBrush(black_color);
    text_how_to_win_.Ecrire(L"PRESS ENTER TO EXIT - PRESS 'R' TO RESTART THE GAME");

    sprite_manager->AddSprite(SpriteGroup::kGuiGameOverGroup, ".//data//Firebools.dds", new_largeur_logo, hauteur / 4);
    sprite_manager->AddSprite(SpriteGroup::kGuiGameOverGroup, ".//data//Lose_White.dds", new_largeur_win, hauteur / 2);
    sprite_manager->AddTextSprite(SpriteGroup::kGuiGameOverGroup, text_how_to_gameover_.GetTextureView(), new_largeur_restart, 900);
    text_how_to_gameover_.ChangeColorBrush(black_color);
    text_how_to_gameover_.Ecrire(L"PRESS ENTER TO EXIT - PRESS 'R' TO RESTART THE GAME");

    sprite_manager->AddSprite(SpriteGroup::kGuiStartGroup, ".//data//Title_Screen.dds", ( largeur - 580 ) / 2, 600);
    sprite_manager->AddTextSprite(SpriteGroup::kGuiStartGroup, text_how_to_start_.GetTextureView(), new_largeur_start, 900);
    text_how_to_start_.ChangeColorBrush(black_color);
    text_how_to_start_.Ecrire(L"PRESS 'ENTER' TO START THE GAME");

    ShowStartMenu();
  }

  void GuiManager::ShowPauseMenu()
  {
    auto sprite_manager = Game<ISpriteManager>();
    sprite_manager->EnableDrawFor(SpriteGroup::kGuiPauseMenuGroup);
    sprite_manager->DisableDrawFor(SpriteGroup::kGuiEndGameGroup);
    sprite_manager->DisableDrawFor(SpriteGroup::kGuiGameOverGroup);
    sprite_manager->DisableDrawFor(SpriteGroup::kGuiStartGroup);
    sprite_manager->DisableDrawFor(SpriteGroup::kGuiInGameGroup);
  }

  void GuiManager::HidePauseMenu()
  {
    auto sprite_manager = Game<ISpriteManager>();
    sprite_manager->DisableDrawFor(SpriteGroup::kGuiPauseMenuGroup);
    sprite_manager->DisableDrawFor(SpriteGroup::kGuiEndGameGroup);
    sprite_manager->DisableDrawFor(SpriteGroup::kGuiGameOverGroup);
    sprite_manager->DisableDrawFor(SpriteGroup::kGuiStartGroup);
    sprite_manager->EnableDrawFor(SpriteGroup::kGuiInGameGroup);
  }

  void GuiManager::ShowEndGameMenu()
  {
    is_end_menu_show_ = true;
    auto sprite_manager = Game<ISpriteManager>();
    sprite_manager->EnableDrawFor(SpriteGroup::kGuiEndGameGroup);
    sprite_manager->DisableDrawFor(SpriteGroup::kGuiPauseMenuGroup);
    sprite_manager->DisableDrawFor(SpriteGroup::kGuiGameOverGroup);
    sprite_manager->DisableDrawFor(SpriteGroup::kGuiStartGroup);
    sprite_manager->DisableDrawFor(SpriteGroup::kGuiInGameGroup);
  }
  void GuiManager::HideEndGameMenu()
  {
    is_end_menu_show_ = false;
    auto sprite_manager = Game<ISpriteManager>();
    sprite_manager->DisableDrawFor(SpriteGroup::kGuiEndGameGroup);
    sprite_manager->DisableDrawFor(SpriteGroup::kGuiGameOverGroup);
    sprite_manager->DisableDrawFor(SpriteGroup::kGuiStartGroup);
    sprite_manager->DisableDrawFor(SpriteGroup::kGuiPauseMenuGroup);
    sprite_manager->EnableDrawFor(SpriteGroup::kGuiInGameGroup);
  }

  void GuiManager::ShowGameOverMenu()
  {
    is_gameover_menu_show_ = true;
    auto sprite_manager = Game<ISpriteManager>();
    sprite_manager->EnableDrawFor(SpriteGroup::kGuiGameOverGroup);
    sprite_manager->DisableDrawFor(SpriteGroup::kGuiEndGameGroup);
    sprite_manager->DisableDrawFor(SpriteGroup::kGuiPauseMenuGroup);
    sprite_manager->DisableDrawFor(SpriteGroup::kGuiStartGroup);
    sprite_manager->DisableDrawFor(SpriteGroup::kGuiInGameGroup);
  }
  void GuiManager::HideGameOverMenu()
  {
    is_gameover_menu_show_ = false;
    auto sprite_manager = Game<ISpriteManager>();
    sprite_manager->DisableDrawFor(SpriteGroup::kGuiEndGameGroup);
    sprite_manager->DisableDrawFor(SpriteGroup::kGuiGameOverGroup);
    sprite_manager->DisableDrawFor(SpriteGroup::kGuiStartGroup);
    sprite_manager->DisableDrawFor(SpriteGroup::kGuiPauseMenuGroup);
    sprite_manager->EnableDrawFor(SpriteGroup::kGuiInGameGroup);
  }

  void GuiManager::ShowStartMenu()
  {
    auto sprite_manager = Game<ISpriteManager>();
    sprite_manager->DisableDrawFor(SpriteGroup::kGuiPauseMenuGroup);
    sprite_manager->DisableDrawFor(SpriteGroup::kGuiGameOverGroup);
    sprite_manager->DisableDrawFor(SpriteGroup::kGuiEndGameGroup);
    sprite_manager->DisableDrawFor(SpriteGroup::kGuiInGameGroup);
    sprite_manager->EnableDrawFor(SpriteGroup::kGuiStartGroup);
  }
}