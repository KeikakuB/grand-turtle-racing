#pragma once
#include "Singleton.h"
#include "ConfigSingleton.h"
#include "LoaderSingleton.h"

#include "GameSingleton.h"

#include "RenderManager.h"
#include "SimulationManager.h"
#include "WindowsInputManager.h"
#include "UpdateManager.h"
#include "WorldManager.h"
#include "CameraManager.h"
#include "TextureManager.h"
#include "SpriteManager.h"
#include "TimeManager.h"
#include "GuiManager.h"
#include "Camera.h"
#include "TurtleGo.h"
#include "Sandbank.h"
#include "ObstacleGo.h"
#include "FoodGo.h"
#include "InvisibleBox.h"
#include "TargetComponent.h"
#include "Math.h"
#include "BillboardGo.h"
#include "SkyboxGo.h"
#include "TriggerZone.h"

#include "Ids.h"

#include "IPoseComponentInterface.h"

#include "ThirdPersonCamera.h"
#include "FirstPersonCamera.h"

#include "ICamera.h"
#include "TimeManager.h"
#include "TextHolder.h"
#include "IGameLogicManager.h"
#include "GameLogicManager.h"
#include "PostEffectManager.h"

#include <chrono>
#include <iostream>

namespace PM3D
{
  enum ManagerOrdering
  {
    kGameLogicManager,
    kTimeManager,
    kInputManager,
    kTextureManager,
    kSpriteManager,
    kGuiManager,
    kWorldManager,
    kSimulationManager,
    kUpdateManager,
    kCameraManager,
    kPostEffectManager,
    kRenderManager
  };
  const int IMAGESPARSECONDE = 60;

  //
//   TEMPLATE : CMoteur
//
//   BUT : Template servant à construire un objet Moteur qui implantera les
//         aspects "génériques" du moteur de jeu
//
//   COMMENTAIRES :
//
//        Comme plusieurs de nos objets représenteront des éléments uniques
//		  du système (ex: le moteur lui-même, le lien vers
//        le dispositif Direct3D), l'utilisation d'un singleton
//        nous simplifiera plusieurs aspects.
//
  template <class T, class TClasseDispositif> class CMoteur :public Singleton<T>
  {
  public:

    virtual void Run()
    {
      auto time_manager = Game<ITimeManager>();
      auto logic_manager = Game<IGameLogicManager>();
      while ( !logic_manager->IsGameExit() )
      {
        // Propre à la plateforme - (Conditions d'arrêt, interface, messages)
        bool bBoucle = RunSpecific();

        if ( !bBoucle ) { return; }
        // Est-il temps de rendre l'image?
        if ( time_manager->shouldShowNextFrame() )
        {
          // Affichage optimisé
          pDispositif->Present();
          GameSingleton::Get().Tick();
        }
      }
    }

    virtual int Initialisations()
    {
      using namespace std::chrono_literals;
      GameSingleton::Init();

      // Propre à la plateforme
      InitialisationsSpecific();

      // * Initialisation du dispositif de rendu
      pDispositif = CreationDispositifSpecific(CDS_FENETRE);

      GameSingleton& game = GameSingleton::Get();
      game.SetDispositif(pDispositif);

      //Initialize all the singletons
      ConfigSingleton::Init("data/config/config.json");
      LoaderSingleton::Init(pDispositif);
      TextHolder::Init();

      auto logic_manager = game.AddManagerComponent<GameLogicManager>(kGameLogicManager);
      auto time_manager = game.AddManagerComponent<TimeManager>(kTimeManager);
      auto texture_manager = game.AddManagerComponent<TextureManager>(kTextureManager);
      auto sprite_manager = game.AddManagerComponent<SpriteManager>(kSpriteManager);
      auto gui_manager = game.AddManagerComponent<GuiManager>(kGuiManager);
      auto world_manager = game.AddManagerComponent<WorldManager>(kWorldManager);
      auto simulation_manager = game.AddManagerComponent<SimulationManager>(kSimulationManager);
      auto update_manager = game.AddManagerComponent<UpdateManager>(kUpdateManager);
      auto camera_manager = game.AddManagerComponent<CameraManager>(kCameraManager);
      auto pe_manager = game.AddManagerComponent<PostEffectManager>(kPostEffectManager);
      auto render_manager = game.AddManagerComponent<RenderManager>(kRenderManager, [&]() { BeginRenderSceneSpecific(); }, [&]() { EndRenderSceneSpecific(); });

      sprite_manager->EnableDrawFor(SpriteGroup::kGenericGroup);
      sprite_manager->EnableDrawFor(SpriteGroup::kGuiInGameGroup);
      sprite_manager->EnableDrawFor(SpriteGroup::kGuiPauseMenuGroup);
      sprite_manager->EnableDrawFor(SpriteGroup::kLevelGroup);
      sprite_manager->EnableDrawFor(SpriteGroup::kSkyboxGroup);

      world_manager->Spawn<SkyboxGo>(physx::PxTransform::createIdentity());

      static const float terrain_width = 1000;
      static const float terrain_length = 5000;
      static const float width_size = 50;
      static const float wall_width = 270;
      int wall_side = floor(terrain_length / wall_width);
      int wall_back = floor(terrain_width / wall_width);

      auto log_mng = Game<IGameLogicManager>();

      auto sandbank = world_manager->Spawn<Sandbank>(physx::PxTransform::createIdentity(), terrain_width, terrain_length, width_size);
      const physx::PxVec3& terrain_pos = sandbank->As<IPoseComponentInterface>()->GetTransform().p;

      if ( log_mng )
      {
        log_mng->SetTerrainPose(sandbank->As<IPoseComponentInterface>()->GetTransform());
      }

      world_manager->Spawn<InvisibleBox>(physx::PxTransform{ physx::PxVec3{ terrain_pos.x, terrain_pos.y, terrain_pos.z + 500.0f } }, terrain_width, terrain_length, width_size);

      for ( int i = -( wall_back / 2 ) - 2; i < ( wall_back / 2 ) + 2; ++i )
      {
        world_manager->Spawn<ObstacleGo>(physx::PxTransform{ terrain_pos.x + ( i * wall_width ), terrain_pos.y - terrain_length / 2, terrain_pos.z + ( width_size / 2 ) + 10.0f }, ObstacleGo::WALL);
        world_manager->Spawn<ObstacleGo>(physx::PxTransform{ terrain_pos.x + ( i * wall_width ), terrain_pos.y - terrain_length / 2 + 10.0f, terrain_pos.z + ( width_size / 2 ), physx::PxQuat{ physx::PxPiDivTwo*i, physx::PxVec3{ 0,0,1 } } }, ObstacleGo::PALMTREE);
      }
      for ( int i = -( wall_side / 2 ) - 1; i < ( wall_side / 2 ) + 3; ++i )
      {
        world_manager->Spawn<ObstacleGo>(physx::PxTransform{ terrain_pos.x - terrain_width / 2, terrain_pos.y + ( i * wall_width ), terrain_pos.z + ( width_size / 2 ) + 10.0f, physx::PxQuat{ physx::PxPiDivTwo, physx::PxVec3{ 0,0,1 } } }, ObstacleGo::WALL);
        world_manager->Spawn<ObstacleGo>(physx::PxTransform{ terrain_pos.x + terrain_width / 2, terrain_pos.y + ( i * wall_width ), terrain_pos.z + ( width_size / 2 ) + 10.0f , physx::PxQuat{ physx::PxPiDivTwo, physx::PxVec3{ 0,0,1 } } }, ObstacleGo::WALL);
      }

      world_manager->Spawn<TriggerZone>(physx::PxTransform{ physx::PxVec3{ terrain_pos.x - terrain_width * 3 / 2, terrain_pos.y, terrain_pos.z - 5.0f } }, terrain_width * 2, terrain_length * 3, width_size);
      world_manager->Spawn<TriggerZone>(physx::PxTransform{ physx::PxVec3{ terrain_pos.x + terrain_width * 3 / 2, terrain_pos.y, terrain_pos.z - 5.0f } }, terrain_width * 2, terrain_length * 3, width_size);
      world_manager->Spawn<TriggerZone>(physx::PxTransform{ physx::PxVec3{ terrain_pos.x, terrain_pos.y + terrain_length * 3 / 2, terrain_pos.z - 1.0f } }, terrain_width * 3, terrain_length * 2, width_size);
      world_manager->Spawn<TriggerZone>(physx::PxTransform{ physx::PxVec3{ terrain_pos.x, terrain_pos.y - terrain_length * 3 / 2, terrain_pos.z - 1.0f } }, terrain_width * 3, terrain_length * 2, width_size);

      auto turtle = world_manager->Spawn<TurtleGo>(physx::PxTransform{ terrain_pos.x, terrain_pos.y - ( terrain_length * 9 / 10 ) / 2, terrain_pos.z + ( width_size / 2 ) + 1 , physx::PxQuat{ physx::PxPi, physx::PxVec3{ 0,0,1 } } });
      const physx::PxVec3 turt_pos = turtle->As<IPoseComponentInterface>()->GetTransform().p;

      for ( int i = -( wall_side / 2 ) - 1; i < ( wall_side / 2 ) + 1; ++i )
      {
        for ( int j = -( wall_back / 2 ) - 1; j < ( wall_back / 2 ); ++j )
        {
          float r1 = static_cast <float> ( rand() ) / ( static_cast <float> ( RAND_MAX / wall_width ) );
          float r2 = static_cast <float> ( rand() ) / ( static_cast <float> ( RAND_MAX / wall_width ) );
          world_manager->Spawn<FoodGo>(physx::PxTransform{ terrain_pos.x + ( j * wall_width ) + r1, terrain_pos.y + ( i * wall_width ) + r2, terrain_pos.z + ( width_size / 2 ) + 1 });
        }
      }

      world_manager->Spawn<ObstacleGo>(physx::PxTransform{ physx::PxVec3{ terrain_pos.x, terrain_pos.y, terrain_pos.z + ( width_size / 2 ) } }, ObstacleGo::CASTLE);
      world_manager->Spawn<ObstacleGo>(physx::PxTransform{ physx::PxVec3{ terrain_pos.x + ( terrain_width / 2 ) * 1 / 10, terrain_pos.y - ( terrain_length / 2 ) * 7 / 10, terrain_pos.z + ( width_size / 2 ) } }, ObstacleGo::BUCKET);
      world_manager->Spawn<ObstacleGo>(physx::PxTransform{ physx::PxVec3{ terrain_pos.x - ( terrain_width / 2 ) * 1 / 10, terrain_pos.y - ( terrain_length / 2 ) * 2 / 10, terrain_pos.z + ( width_size / 2 ) } }, ObstacleGo::SHOVEL);
      world_manager->Spawn<ObstacleGo>(physx::PxTransform{ physx::PxVec3{ terrain_pos.x + ( terrain_width / 2 ) * 1 / 10, terrain_pos.y + ( terrain_length / 2 ) * 3 / 10, terrain_pos.z + ( width_size / 2 ) } }, ObstacleGo::SIFTER);
      world_manager->Spawn<ObstacleGo>(physx::PxTransform{ physx::PxVec3{ terrain_pos.x - ( terrain_width / 2 ) * 3 / 10, terrain_pos.y + ( terrain_length / 2 ) * 4 / 10, terrain_pos.z + ( width_size / 2 ) } }, ObstacleGo::RAKE);

      auto third_person_camera = world_manager->Spawn<ThirdPersonCamera>(physx::PxTransform::createIdentity(), turtle->GetId());
      auto cam3p_id = camera_manager->AddCamera(third_person_camera);

      auto first_person_camera = world_manager->Spawn<FirstPersonCamera>(physx::PxTransform::createIdentity(), turtle->GetId());
      auto cam1p_id = camera_manager->AddCamera(first_person_camera);

      camera_manager->SetMainCamera(cam3p_id);

      world_manager->SetLight(physx::PxTransform{ -50.0f, 0.0f, 10000.0f });

      sprite_manager->AddPanelSprite(SpriteGroup::kGenericGroup, ".\\data\\Firebools.dds", physx::PxTransform{ terrain_pos.x, terrain_pos.y - ( terrain_length * 9 / 10 ) / 2, terrain_pos.z + 80 ,physx::PxQuat{ -physx::PxPi, physx::PxVec3{ -1,0,0 } } }, 50, 50);
      //sprite_manager->AddPanelSprite(SpriteGroup::kGenericGroup, ".\\data\\Firebools.dds", physx::PxTransform{ 0, 0, -2000, physx::PxQuat{ 0.0f, 0.0f, 0.0f, 1 } }, 5000, 5000);
      //sprite_manager->AddPanelSprite(SpriteGroup::kGenericGroup, ".\\data\\Firebools.dds", physx::PxTransform{ 0, 0, 2000, physx::PxQuat{ 0.0f, 0.0f, 0.0f, 1 } }, 5000, 5000);

      gui_manager->SetupGameGui();

      //pe_manager->SetDefaultTechnique(ids::post_effect_techniques::kTiles);

      return 0;
    }
  protected:
    // Constructeur par défaut
    CMoteur() = default;
    // Destructeur
    ~CMoteur()
    {
      Cleanup();
    }
    // Spécifiques - Doivent être implantés
    virtual bool RunSpecific() = 0;
    virtual int InitialisationsSpecific() = 0;
    virtual TClasseDispositif* CreationDispositifSpecific(const CDS_MODE cdsMode) = 0;
    virtual void BeginRenderSceneSpecific() = 0;
    virtual void EndRenderSceneSpecific() = 0;

    virtual void Cleanup()
    {
      // Détruire le dispositif
      if ( pDispositif )
      {
        delete pDispositif;
        pDispositif = NULL;
      }
    }

  protected:
    // Le dispositif de rendu
    TClasseDispositif* pDispositif;
  };
} // namespace PM3D
