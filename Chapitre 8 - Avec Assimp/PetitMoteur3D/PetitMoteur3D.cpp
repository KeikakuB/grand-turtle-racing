// PetitMoteur3D.cpp�: d�finit le point d'entr�e pour l'application.
//

#include "stdafx.h"
#include "PetitMoteur3D.h"
#include "MoteurWindows.h"
#include <exception>

using namespace PM3D;

int APIENTRY _tWinMain(HINSTANCE hInstance,
  HINSTANCE hPrevInstance,
  LPTSTR    lpCmdLine,
  int       nCmdShow)
{
  // Pour ne pas avoir d'avertissement
  UNREFERENCED_PARAMETER(hPrevInstance);
  UNREFERENCED_PARAMETER(lpCmdLine);
  UNREFERENCED_PARAMETER(nCmdShow);

  try
  {
    CMoteurWindows::Init();
    // Cr�ation de l'objet Moteur
    CMoteurWindows& rMoteur = CMoteurWindows::Get();

    // Sp�cifiques � une application Windows
    rMoteur.SetWindowsAppInstance(hInstance);

    // Initialisation du moteur
    rMoteur.Initialisations();

    // Boucle d'application
    rMoteur.Run();

    return (int) 1;
  }

  catch ( const std::exception E )
  {
#ifdef _DEBUG
    throw;
#endif
#ifndef _DEBUG
    wchar_t message[ 35 ];
    mbtowc(message, E.what(), 34);

    MessageBox(NULL, message, L"Erreur", MB_ICONWARNING);

    return (int) 99;
#endif
  }

  catch ( int codeErreur )
  {
#ifdef _DEBUG
    throw;
#endif
#ifndef _DEBUG
    wchar_t szErrMsg[ MAX_LOADSTRING ];	// Un message d'erreur selon le code

    ::LoadString(hInstance, codeErreur, szErrMsg, MAX_LOADSTRING);
    ::MessageBox(NULL, szErrMsg, L"Erreur", MB_ICONWARNING);

    return (int) 99; // POURQUOI 99???
#endif
  }
}

int _tmain(int argc, _TCHAR* argv[])
{
  return _tWinMain(GetModuleHandle(NULL), 0, nullptr, true);
}