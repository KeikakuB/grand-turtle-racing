#include "stdafx.h"
#include "TriggerZoneCollisionHandler.h"
#include "IGameLogicManager.h"
#include "GameSingleton.h"

namespace PM3D
{
  TriggerZoneCollisionHandler::TriggerZoneCollisionHandler()
  {
  }

  void TriggerZoneCollisionHandler::onContact(const physx::PxContactPair& contact_pair)
  {
    // end game
    auto logic_manager = Game<IGameLogicManager>();
    logic_manager->SetEndGame(true);
  }
}