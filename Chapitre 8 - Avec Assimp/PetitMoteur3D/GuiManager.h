#pragma once
#include "Meta.h"
#include "GameManagerComponent.h"
#include "IGuiManager.h"
#include "TextHolder.h"
#include "ITimeManager.h"

#include <memory>

namespace PM3D
{
  class GuiManager :
    public GameManagerComponent, public IGuiManager
  {
  public:
    GuiManager();
    virtual void RunTask() override;
    void SetupGameGui();
    void ShowPauseMenu();
    void HidePauseMenu();
    void ShowEndGameMenu();
    void HideEndGameMenu();
    void ShowGameOverMenu();
    void HideGameOverMenu();
    void ShowStartMenu();
  private:
    ITimeManager::time_point last_time_paused_;
    ITimeManager::time_point last_time_added_post_effect_;
    bool is_paused_;
    bool is_end_menu_show_;
    bool is_gameover_menu_show_;
    float pre_pause_game_rate_;
    const Gdiplus::FontFamily oFamily;
    std::unique_ptr<Gdiplus::Font> pPolice;
    TextHolder text_turtle_velocity_;
    TextHolder text_turtle_life_;
    TextHolder text_elasped_time_;
    TextHolder text_how_to_quit_;
    TextHolder text_how_to_win_;
    TextHolder text_how_to_start_;
    TextHolder text_how_to_gameover_;
  };
  namespace meta
  {
    REGISTER_COMPONENT_H_ID(GuiManager);
  }
}
