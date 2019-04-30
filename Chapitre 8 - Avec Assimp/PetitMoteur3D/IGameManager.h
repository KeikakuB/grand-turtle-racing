#pragma once
namespace PM3D
{
  class IGameManager
  {
  public:
    virtual void RunTask() = 0;
    virtual ~IGameManager() = default;
  };
}