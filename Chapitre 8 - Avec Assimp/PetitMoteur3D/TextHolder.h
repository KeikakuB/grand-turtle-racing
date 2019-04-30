#pragma once

#include <string.h>
#include "dispositifD3D11.h"
#include <Gdiplus.h>
#include <string>
#pragma comment(lib, "gdiplus.lib")

namespace PM3D
{
  class TextHolder
  {
  public:
    TextHolder(int largeur, int hauteur, Gdiplus::Font* pPolice);
    ~TextHolder();
    void Ecrire(std::wstring s);
    ID3D11ShaderResourceView* GetTextureView() { return pTextureView; }

    static void Init();
    static void Close();

    void ChangeColorBrush(Gdiplus::Color* couleur);

  protected:
    UINT TexWidth;
    UINT TexHeight;

    ID3D11Texture2D *pTexture;
    IDXGISurface1* pSurface;
    ID3D11ShaderResourceView* pTextureView;

    Gdiplus::Font* pFont;
    Gdiplus::Bitmap* pCharBitmap;
    Gdiplus::Graphics* pCharGraphics;
    Gdiplus::SolidBrush* pBlackBrush;

    // Variables statiques pour GDI+
    static	ULONG_PTR token;
  };
}
