#include "stdafx.h"
#include "LoaderSingleton.h"
#include "SettingsStructs.h"
#include "ConfigSingleton.h"
#include "ChargeurAssimp.h"
#include "util.h"
#include "resource.h"

using namespace UtilitairesDX;
namespace PM3D
{
  LoaderSingleton::LoaderSingleton(CDispositifD3D11* disp)
  {
    pDispositif = disp;
  }
  void LoaderSingleton::CompileAndLoadVertexShader(
    const std::string& id,
    ID3D11VertexShader** vertex_shader,
    ID3D11InputLayout** input_layout,
    D3D11_INPUT_ELEMENT_DESC* layout,
    UINT number_of_elements,
    UINT shader_params_size,
    ID3D11Buffer** constant_buffer)
  {
    ID3D11Device* pD3DDevice = pDispositif->GetD3DDevice();
    auto settings = ConfigSingleton::Get().FetchById<VertexShaderSettings>(id);

    ID3DBlob* pVSBlob = NULL;
    DXEssayer(D3DCompileFromFile(settings.filename.c_str(),
      NULL, NULL,
      settings.name.c_str(),
      settings.target.c_str(),
      D3DCOMPILE_ENABLE_STRICTNESS,
      0,
      &pVSBlob, 0), DXE_FICHIER_VS);

    DXEssayer(pD3DDevice->CreateVertexShader(pVSBlob->GetBufferPointer(),
      pVSBlob->GetBufferSize(),
      NULL,
      vertex_shader),
      DXE_CREATION_VS);

    // Créer l'organisation des sommets
    *input_layout = NULL;
    DXEssayer(pD3DDevice->CreateInputLayout(layout,
      number_of_elements,
      pVSBlob->GetBufferPointer(),
      pVSBlob->GetBufferSize(),
      input_layout),
      DXE_CREATIONLAYOUT);

    pVSBlob->Release(); //  On n'a plus besoin du blob

    // Création d'un tampon pour les constantes du VS
    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));

    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = shader_params_size;
    bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bd.CPUAccessFlags = 0;
    pD3DDevice->CreateBuffer(&bd, NULL, constant_buffer);
  }
  void LoaderSingleton::CompileAndLoadPixelShader(const std::string& id, ID3D11PixelShader** pixel_shader)
  {
    ID3DBlob* pPSBlob = NULL;
    auto settings = ConfigSingleton::Get().FetchById<PixelShaderSettings>(id);
    DXEssayer(D3DCompileFromFile(settings.filename.c_str(),
      NULL, NULL,
      settings.name.c_str(),
      settings.target.c_str(),
      D3DCOMPILE_ENABLE_STRICTNESS,
      0,
      &pPSBlob, NULL), DXE_FICHIER_PS);

    DXEssayer(pDispositif->GetD3DDevice()->CreatePixelShader(pPSBlob->GetBufferPointer(),
      pPSBlob->GetBufferSize(),
      NULL,
      pixel_shader),
      DXE_CREATION_PS);

    pPSBlob->Release(); //  On n'a plus besoin du blob
  }
  void LoaderSingleton::CompileAndLoadEffectShader(
    const std::string& id,
    ID3D11SamplerState** sample_state,
    ID3D11InputLayout** input_layout,
    D3D11_INPUT_ELEMENT_DESC* layout,
    UINT number_of_elements,
    UINT shader_params_size,
    ID3D11Buffer** constant_buffer,
    ID3DX11Effect** effect,
    ID3DX11EffectTechnique** technique,
    ID3DX11EffectPass** pass)
  {
    auto settings = ConfigSingleton::Get().FetchById<EffectShaderSettings>(id);

    ID3D11Device* pD3DDevice = pDispositif->GetD3DDevice();

    // Création d'un tampon pour les constantes du VS
    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));

    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = shader_params_size;
    bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bd.CPUAccessFlags = 0;
    HRESULT hr = pD3DDevice->CreateBuffer(&bd, NULL, constant_buffer);

    // Pour l'effet
    ID3DBlob* pFXBlob = NULL;
    DXEssayer(D3DCompileFromFile(settings.filename.c_str(), 0, 0, 0,
      settings.target.c_str(), 0, 0, &pFXBlob, 0),
      DXE_ERREURCREATION_FX);

    D3DX11CreateEffectFromMemory(pFXBlob->GetBufferPointer(),
      pFXBlob->GetBufferSize(), 0,
      pD3DDevice, effect);

    pFXBlob->Release();

    *technique = ( *effect )->GetTechniqueByIndex(0);
    *pass = ( *technique )->GetPassByIndex(0);

    // Créer l'organisation des sommets pour le VS de notre effet
    D3DX11_PASS_SHADER_DESC effectVSDesc;
    ( *pass )->GetVertexShaderDesc(&effectVSDesc);

    D3DX11_EFFECT_SHADER_DESC effectVSDesc2;
    effectVSDesc.pShaderVariable->GetShaderDesc(effectVSDesc.ShaderIndex,
      &effectVSDesc2);

    const void *vsCodePtr = effectVSDesc2.pBytecode;
    unsigned vsCodeLen = effectVSDesc2.BytecodeLength;

    *input_layout = NULL;
    DXEssayer(pD3DDevice->CreateInputLayout(layout,
      number_of_elements,
      vsCodePtr,
      vsCodeLen,
      input_layout),
      DXE_CREATIONLAYOUT);

    // Initialisation des paramètres de sampling de la texture
    D3D11_SAMPLER_DESC samplerDesc;

    samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.MipLODBias = 0.0f;
    samplerDesc.MaxAnisotropy = 4;
    samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
    samplerDesc.BorderColor[ 0 ] = 0;
    samplerDesc.BorderColor[ 1 ] = 0;
    samplerDesc.BorderColor[ 2 ] = 0;
    samplerDesc.BorderColor[ 3 ] = 0;
    samplerDesc.MinLOD = 0;
    samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

    // Création de l'état de sampling
    pD3DDevice->CreateSamplerState(&samplerDesc, sample_state);
  }
  void LoaderSingleton::CompileAndLoadEffectShader(
    const std::string& id,
    ID3D11SamplerState** sample_state,
    ID3D11InputLayout** input_layout,
    D3D11_INPUT_ELEMENT_DESC* layout,
    UINT number_of_elements,
    UINT shader_params_size,
    ID3D11Buffer** constant_buffer,
    ID3DX11Effect** effect,
    ID3DX11EffectTechnique** technique,
    ID3DX11EffectPass** pass1,
    ID3DX11EffectPass** pass2)
  {
    auto settings = ConfigSingleton::Get().FetchById<EffectShaderSettings>(id);

    ID3D11Device* pD3DDevice = pDispositif->GetD3DDevice();

    // Création d'un tampon pour les constantes du VS
    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));

    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = shader_params_size;
    bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bd.CPUAccessFlags = 0;
    HRESULT hr = pD3DDevice->CreateBuffer(&bd, NULL, constant_buffer);

    // Pour l'effet
    ID3DBlob* pFXBlob = NULL;
    DXEssayer(D3DCompileFromFile(settings.filename.c_str(), 0, 0, 0,
      settings.target.c_str(), 0, 0, &pFXBlob, 0),
      DXE_ERREURCREATION_FX);

    D3DX11CreateEffectFromMemory(pFXBlob->GetBufferPointer(),
      pFXBlob->GetBufferSize(), 0,
      pD3DDevice, effect);

    pFXBlob->Release();

    *technique = ( *effect )->GetTechniqueByIndex(0);
    *pass1 = ( *technique )->GetPassByIndex(0);
    *pass2 = ( *technique )->GetPassByIndex(1);
    {
      // Créer l'organisation des sommets pour le VS de notre effet
      D3DX11_PASS_SHADER_DESC effectVSDesc;
      ( *pass1 )->GetVertexShaderDesc(&effectVSDesc);

      D3DX11_EFFECT_SHADER_DESC effectVSDesc2;
      effectVSDesc.pShaderVariable->GetShaderDesc(effectVSDesc.ShaderIndex,
        &effectVSDesc2);

      const void *vsCodePtr = effectVSDesc2.pBytecode;
      unsigned vsCodeLen = effectVSDesc2.BytecodeLength;

      *input_layout = NULL;
      DXEssayer(pD3DDevice->CreateInputLayout(layout,
        number_of_elements,
        vsCodePtr,
        vsCodeLen,
        input_layout),
        DXE_CREATIONLAYOUT);
    }
    {
      // Créer l'organisation des sommets pour le VS de notre effet
      D3DX11_PASS_SHADER_DESC effectVSDesc;
      ( *pass2 )->GetVertexShaderDesc(&effectVSDesc);

      D3DX11_EFFECT_SHADER_DESC effectVSDesc2;
      effectVSDesc.pShaderVariable->GetShaderDesc(effectVSDesc.ShaderIndex,
        &effectVSDesc2);

      const void *vsCodePtr = effectVSDesc2.pBytecode;
      unsigned vsCodeLen = effectVSDesc2.BytecodeLength;

      *input_layout = NULL;
      DXEssayer(pD3DDevice->CreateInputLayout(layout,
        number_of_elements,
        vsCodePtr,
        vsCodeLen,
        input_layout),
        DXE_CREATIONLAYOUT);
    }
    // Initialisation des paramètres de sampling de la texture
    D3D11_SAMPLER_DESC samplerDesc;

    samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.MipLODBias = 0.0f;
    samplerDesc.MaxAnisotropy = 4;
    samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
    samplerDesc.BorderColor[ 0 ] = 0;
    samplerDesc.BorderColor[ 1 ] = 0;
    samplerDesc.BorderColor[ 2 ] = 0;
    samplerDesc.BorderColor[ 3 ] = 0;
    samplerDesc.MinLOD = 0;
    samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

    // Création de l'état de sampling
    pD3DDevice->CreateSamplerState(&samplerDesc, sample_state);
  }
}