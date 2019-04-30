#pragma once

#include "moteur.h"
#include "dispositifD3D11.h"

namespace PM3D
{
#define MAX_LOADSTRING 100

  class CMoteurWindows : public CMoteur<CMoteurWindows, CDispositifD3D11>
  {
  public:
    CMoteurWindows() = default;
    ~CMoteurWindows() = default;
    void SetWindowsAppInstance(HINSTANCE hInstance);
  protected:
    virtual int InitialisationsSpecific();
    virtual bool RunSpecific();
    virtual CDispositifD3D11* CreationDispositifSpecific(const CDS_MODE cdsMode);
    virtual void BeginRenderSceneSpecific();
    virtual void EndRenderSceneSpecific();
  private:
    // Fonctions "Callback" -- Doivent être statiques
    static LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
    static INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
    ATOM	MyRegisterClass(HINSTANCE hInstance);
    bool InitAppInstance();
    int Show();
    HACCEL hAccelTable;						// handle Windows de la table des accélérateurs
    static HINSTANCE hAppInstance;			// handle Windows de l'instance actuelle de l'application
    HWND hMainWnd;							// handle Windows de la fenêtre principale
    TCHAR szWindowClass[ MAX_LOADSTRING ];	// le nom de la classe de fenêtre principale
  };
}
