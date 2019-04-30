#pragma once
#include "Singleton.h"
#include "dispositifD3D11.h"
#include "d3dx11effect.h"
#include <string>
namespace PM3D
{
  class LoaderSingleton final : public Singleton<LoaderSingleton>
  {
  public:
    friend class Singleton<LoaderSingleton>;
    void CompileAndLoadVertexShader(
      const std::string& id,
      ID3D11VertexShader** vertex_shader,
      ID3D11InputLayout** input_layout,
      D3D11_INPUT_ELEMENT_DESC* layout,
      UINT number_of_elements,
      UINT shader_params_size,
      ID3D11Buffer** constant_buffer);
    void CompileAndLoadPixelShader(const std::string& id, ID3D11PixelShader** pixel_shader);
    void CompileAndLoadEffectShader(
      const std::string& id,
      ID3D11SamplerState** sample_state,
      ID3D11InputLayout** input_layout,
      D3D11_INPUT_ELEMENT_DESC* layout,
      UINT number_of_elements,
      UINT shader_params_size,
      ID3D11Buffer** constant_buffer,
      ID3DX11Effect** effect,
      ID3DX11EffectTechnique** technique,
      ID3DX11EffectPass** pass);
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
      ID3DX11EffectPass** pass2);
  private:
    LoaderSingleton(CDispositifD3D11* disp);
    CDispositifD3D11* pDispositif;
  };
}