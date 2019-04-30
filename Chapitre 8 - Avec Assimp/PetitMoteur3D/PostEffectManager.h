#pragma once
#include "Meta.h"
#include "GameManagerComponent.h"
#include "IPostEffectManager.h"
#include "ITimeManager.h"
#include "PanneauPE.h"
#include "SettingsStructs.h"

#include <queue>
namespace PM3D
{
  class PostEffectManager :
    public GameManagerComponent, public IPostEffectManager
  {
  public:
    PostEffectManager();
    virtual void RunTask() override;
    virtual void StartPostEffect() override;
    virtual void EndPostEffect() override;
    virtual void DrawPostEffect() override;
    virtual void SetDefaultTechnique(const std::string& name) override;
    virtual void EnqueueTechnique(const std::string& name, ITimeManager::duration time_to_show, ITimeManager::duration delay_at_end) override;

  private:
    struct TechniqueEvent
    {
      PostEffectTechniqueSettings settings;
      ITimeManager::time_point start_time;
      ITimeManager::time_point end_time;
      ITimeManager::time_point end_time_with_delay;
    };
    struct EnqueueTechniqueCallParams
    {
      std::string name;
      ITimeManager::duration time_to_show;
      ITimeManager::duration delay_at_end;
    };
    float GetCurrentTimeParam();

    CPanneauPE post_effect_panel_;
    std::string default_technique_;
    std::queue<TechniqueEvent> technique_events_;
    std::queue<EnqueueTechniqueCallParams> events_just_added_;
  };
  namespace meta
  {
    REGISTER_COMPONENT_H_ID(PostEffectManager);
  }
}
