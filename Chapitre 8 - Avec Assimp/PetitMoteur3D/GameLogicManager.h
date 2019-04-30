#pragma once
#include "Meta.h"
#include "GameManagerComponent.h"
#include "IGameLogicManager.h"
namespace PM3D
{
  class GameLogicManager :
    public GameManagerComponent, public IGameLogicManager
  {
  public:
    GameLogicManager();
    ~GameLogicManager();
    virtual void SetGameOver(bool val) override;
    virtual void SetGameExit() override;
    virtual void SetStartGame() override;
    virtual void SetEndGame(bool val) override;
    virtual bool IsGameOver() override;
    virtual bool IsGameExit() override;
    virtual bool IsStartGame() override;
    virtual bool IsEndGame() override;
    virtual void RunTask() override;
    virtual void UpdateTurtleVelocityValue(float velocity) override;
    virtual float GetTurtleVelocity() const override;
    virtual void UpdateTurtleLifeValue(int life) override;
    virtual int GetTurtleLife() const override;
    virtual bool IsGameRestarted() override;
    virtual void SetGameRestarted(bool val) override;
    virtual bool IsGamePause() override;
    virtual void SetGamePause(bool val) override;
    virtual void SetTurtlePose(physx::PxTransform pose) override;
    virtual void SetTerrainPose(physx::PxTransform pose) override;
    virtual  physx::PxTransform GetTurtlePose() override;
    virtual  physx::PxTransform GetTerrainPose() override;

  private:
    bool is_game_over_;
    bool is_game_end_;
    bool is_game_restarted_;
    bool is_game_start_;
    bool is_game_exit_;
    bool is_game_pause_;
    float turtle_velocity_;
    int turtle_life_;
    physx::PxTransform turtle_pose_;
    physx::PxTransform terrain_pose_;
  };
  namespace meta
  {
    REGISTER_COMPONENT_H_ID(GameLogicManager);
  }
}
