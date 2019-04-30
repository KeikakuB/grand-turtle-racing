#include "stdafx.h"
#include "GameSingleton.h"

#include "RenderManager.h"
#include "SimulationManager.h"
#include "WindowsInputManager.h"
#include "UpdateManager.h"
#include "WorldManager.h"
#include "CameraManager.h"

#include <algorithm>

namespace PM3D
{
  GameSingleton::GameSingleton() {}

  void GameSingleton::Tick()
  {
    for ( auto& m : managers_ )
    {
      m.second->RunTask();
    }
  }
}