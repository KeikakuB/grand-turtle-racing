# grand-turtle-racing
3D racing game using a custom game engine I designed and built inspired by the engine built by @gaspardpetit (https://github.com/DDJV-INF740/GameEngine-src) and using the DirectX 10 knowledge provided by François Jean.

# Dev
Follow these instructions in order to compile the game using Visual Studio 2017 Community Edition.

## Setting up Boost
1) Download boost (http://www.boost.org/users/download/) Version 1.70.0
2) Unzip it into the folder C:\boost\boost_1_70_0
3) If you'd like to place boost in another folder then you can change
  the boost.props file with your desired path.
4) Run the "Developer Command Prompt for VS 2017" within the boost_1_70_0 folder.
  - Run "bootstrap"
  - After, Run ".\b2"
  - NB: this can take a while.

Now, the boost library is ready to be used within our project.

## Setting up PhysX
1) Unzip the PhysX.7z file from the dependencies folder into the C drive (eg. C:\PhysX).
2) Ensure that the PhysX.props file point to the correct folder (PhysX emplacement).

you can now use physx with
#include "PxPhysicsAPI.h"
// and
using namespace physx;

## Compile
- You can now open the GTA IV.sln file using VS 2017 Community Edition.

## WARNINGS
- The game should only be compiled using the Debug configuration because compiling the game using the Release configuration causes the game to run too fast (or you can look into what's causing the issue and fix it).
