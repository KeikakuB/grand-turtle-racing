#pragma once
#include "Meta.h"
namespace PM3D
{
  class IGameLogicManager
  {
  public:
    virtual void UpdateTurtleVelocityValue(float velocity) = 0;
    virtual void UpdateTurtleLifeValue(int life) = 0;
    virtual float GetTurtleVelocity() const = 0;
    virtual int GetTurtleLife() const = 0;
    virtual void SetGameOver(bool val) = 0;
    virtual void SetGameExit() = 0;
    virtual void SetStartGame() = 0;
    virtual void SetEndGame(bool val) = 0;
    virtual bool IsStartGame() = 0;
    virtual bool IsGameOver() = 0;
    virtual bool IsGameExit() = 0;
    virtual bool IsEndGame() = 0;
    virtual void SetGameRestarted(bool val) = 0;
    virtual bool IsGameRestarted() = 0;
    virtual void SetGamePause(bool val) = 0;
    virtual bool IsGamePause() = 0;
    virtual void SetTurtlePose(physx::PxTransform pose) = 0;
    virtual void SetTerrainPose(physx::PxTransform pose) = 0;
    virtual  physx::PxTransform GetTurtlePose() = 0;
    virtual  physx::PxTransform GetTerrainPose() = 0;
    virtual ~IGameLogicManager() = default;
  };
  namespace meta
  {
    REGISTER_INTERFACE_H_ID(IGameLogicManager);
  }
}