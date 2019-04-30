#include "stdafx.h"
#include "PostEffectManager.h"
#include "GameSingleton.h"
#include "ConfigSingleton.h"
#include "Ids.h"
#include <algorithm>

#include <chrono>

namespace PM3D
{
  namespace meta
  {
    REGISTER_COMPONENT_CPP_ID(PostEffectManager);
    template<>
    component_interface_id_list_t GetComponentInterfaces<PostEffectManager>() noexcept { return{ GetInterfaceId<IPostEffectManager>() }; }
  }
  PostEffectManager::PostEffectManager()
    : post_effect_panel_{ ConfigSingleton::Get().GetNumberOfSettings<PostEffectTechniqueSettings>() },
    default_technique_{ ids::post_effect_techniques::kNul }
  {
    SetDefaultTechnique(default_technique_);
  }

  //void PostEffectManager::SetTechniqueByIndex(int index)
  //{
  //  using namespace std::chrono_literals;
  //  SetTechniqueByIndexWithDelay(index, 1000ms);
  //}

  //void PostEffectManager::SetTechniqueByIndexWithDelay(int index, ITimeManager::duration delay)
  //{
  //  post_effect_panel_.SetTechniqueByIndex(index);
  //  UpdateCurrentTimeBounds(delay);
  //  post_effect_panel_.SetCurrentTimeParam(GetCurrentTimeParam());
  //}

  float PostEffectManager::GetCurrentTimeParam()
  {
#undef min
    auto time_manager = Game<ITimeManager>();
    if ( !technique_events_.empty() )
    {
      const TechniqueEvent& event = technique_events_.front();
      ITimeManager::duration time_since_start = time_manager->nowWorldTime() - event.start_time;
      ITimeManager::duration total_time = event.end_time - event.start_time;
      float new_time = static_cast<float>( time_since_start.count() / total_time.count() );
      return std::min(new_time, 1.0f);
    }
    else
    {
      return 0.0f;
    }
  }

  void PostEffectManager::SetDefaultTechnique(const std::string& name)
  {
    default_technique_ = name;
  }

  void PostEffectManager::EnqueueTechnique(const std::string& name, ITimeManager::duration time_to_show, ITimeManager::duration delay_at_end)
  {
    EnqueueTechniqueCallParams params
    {
      name,
      time_to_show,
      delay_at_end
    };
    events_just_added_.push(params);
  }

  void PM3D::PostEffectManager::RunTask()
  {
    while ( !events_just_added_.empty() )
    {
      EnqueueTechniqueCallParams params = events_just_added_.front();
      ITimeManager::time_point end_of_last_event =
        !technique_events_.empty() ?
        technique_events_.back().end_time_with_delay :
        Game<ITimeManager>()->nowWorldTime();
      PostEffectTechniqueSettings sett = ConfigSingleton::Get().FetchById<PostEffectTechniqueSettings>(params.name);
      TechniqueEvent new_event
      {
        sett,
        end_of_last_event,
        end_of_last_event + params.time_to_show,
        end_of_last_event + params.time_to_show + params.delay_at_end
      };
      technique_events_.push(new_event);
      events_just_added_.pop();
    }
    if ( technique_events_.empty() )
    {
      post_effect_panel_.SetTechniqueByName(default_technique_);
      post_effect_panel_.SetCurrentTimeParam(GetCurrentTimeParam());
    }
    else
    {
      TechniqueEvent current_event;
      do
      {
        current_event = technique_events_.front();
        auto time_manager = Game<ITimeManager>();
        if ( time_manager->nowWorldTime() > current_event.end_time_with_delay )
        {
          technique_events_.pop();
        }
        else
        {
          break;
        }
      } while ( !technique_events_.empty() );
      post_effect_panel_.SetTechniqueByName(current_event.settings.id);
      post_effect_panel_.SetCurrentTimeParam(GetCurrentTimeParam());
    }
  }

  void PM3D::PostEffectManager::StartPostEffect()
  {
    post_effect_panel_.DebutPostEffect();
  }

  void PM3D::PostEffectManager::EndPostEffect()
  {
    post_effect_panel_.FinPostEffect();
  }

  void PM3D::PostEffectManager::DrawPostEffect()
  {
    post_effect_panel_.Draw();
  }
}