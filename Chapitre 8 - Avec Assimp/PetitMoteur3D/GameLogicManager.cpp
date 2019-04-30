#include "stdafx.h"
#include "GameLogicManager.h"

namespace PM3D
{
  namespace meta
  {
    REGISTER_COMPONENT_CPP_ID(GameLogicManager);
    template<>
    component_interface_id_list_t GetComponentInterfaces<GameLogicManager>() noexcept { return{ GetInterfaceId<IGameLogicManager>() }; }
  }
  GameLogicManager::GameLogicManager()
    : is_game_over_{ false },
    is_game_exit_{ false },
    is_game_end_{ false },
    is_game_restarted_{ false },
    is_game_pause_{ true },
    is_game_start_{ true },
    turtle_velocity_{},
    turtle_pose_{},
    turtle_life_{}
  {
  }

  GameLogicManager::~GameLogicManager()
  {
  }

  void GameLogicManager::SetGameOver(bool val)
  {
    is_game_over_ = val;
  }

  bool GameLogicManager::IsGameOver()
  {
    return is_game_over_;
  }

  void GameLogicManager::SetStartGame()
  {
    is_game_start_ = false;
  }

  bool GameLogicManager::IsStartGame()
  {
    return is_game_start_;
  }

  bool GameLogicManager::IsEndGame()
  {
    return is_game_end_;
  }

  void GameLogicManager::RunTask()
  {
  }

  void GameLogicManager::UpdateTurtleVelocityValue(float velocity)
  {
    turtle_velocity_ = velocity;
  }

  void GameLogicManager::UpdateTurtleLifeValue(int life)
  {
    turtle_life_ = life;
  }

  float GameLogicManager::GetTurtleVelocity() const
  {
    return turtle_velocity_ * 1000;
  }

  int GameLogicManager::GetTurtleLife() const
  {
    return turtle_life_;
  }

  void GameLogicManager::SetEndGame(bool val)
  {
    is_game_end_ = val;
  }

  bool GameLogicManager::IsGameRestarted()
  {
    return is_game_restarted_;
  }
  void GameLogicManager::SetGameRestarted(bool val)
  {
    is_game_restarted_ = val;
  }
  void GameLogicManager::SetTurtlePose(physx::PxTransform pose)
  {
    turtle_pose_ = pose;
  }
  void GameLogicManager::SetTerrainPose(physx::PxTransform pose)
  {
    terrain_pose_ = pose;
  }
  physx::PxTransform GameLogicManager::GetTurtlePose()
  {
    return turtle_pose_;
  }
  physx::PxTransform GameLogicManager::GetTerrainPose()
  {
    return turtle_pose_;
  }
  void GameLogicManager::SetGameExit()
  {
    is_game_exit_ = true;
  }
  bool GameLogicManager::IsGameExit()
  {
    return is_game_exit_;
  }
  bool GameLogicManager::IsGamePause()
  {
    return is_game_pause_;
  }
  void GameLogicManager::SetGamePause(bool val)
  {
    is_game_pause_ = val;
  }
}