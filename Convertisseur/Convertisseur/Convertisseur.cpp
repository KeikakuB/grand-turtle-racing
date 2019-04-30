// Convertisseur.cpp : définit le point d'entrée pour l'application console.
//

#include "stdafx.h"
#include "terrain.h"

int _tmain(int argc, _TCHAR* argv[])
{
	CTerrain* terrain = new CTerrain;

	terrain->LireFichierHeightmap();
	terrain->ConstruireTerrain();
	terrain->CalculNormales();
	terrain->ConstruireIndex();
	terrain->EnregistrerTout();

	delete terrain;

	return 0;
}

